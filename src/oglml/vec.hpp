#ifndef GLML_VEC_H
#define GLML_VEC_H

#include <iostream>
#include <cassert>
#include <type_traits>

#define GLML_NAMESPACE_NAME glml
#define GLML_VEC_NAMESPACE_NAME vec

// No template alias support in GCC yet
#define GLML_VEC_SP template <std::size_t, typename> class

namespace GLML_NAMESPACE_NAME {

    // Helpers
    template <typename T>
    struct IsArray {
        constexpr static bool result = std::is_array<T>::value || std::is_pointer<T>::value;
    };

    template <bool flag, typename T, typename U>
    struct Select {
        typedef T Result;
    };

    template <typename T, typename U>
    struct Select<false, T, U> {
        typedef U Result;
    };

    namespace GLML_VEC_NAMESPACE_NAME {
    // Forward declarations for policies
    // - Storage policies
        template <std::size_t n, typename T>
        class DefaultStorage;
    }

    // Vec forward declarations
    template <std::size_t n, typename T, GLML_VEC_SP SP = vec::DefaultStorage>
    union Vec;

    // Everything belongs to Vec
    namespace GLML_VEC_NAMESPACE_NAME {

        // Storage Policies
        template <std::size_t n, typename T>
        class DefaultStorage {
        public:
            // Infos
            constexpr static bool aligned = true;

            inline T& operator[](std::size_t i) {
                assert(i < n);
                return mData[i];
            }

            inline const T& operator[](std::size_t i) const {
                assert(i < n);
                return mData[i];
            }

        private:
            T mData[n];
        };

        /*template <std::size_t n, typename T, std::size_t nHost, GLML_VEC_SP SPHost, std::size_t... Indices>
        class UnconfiguredViewStorage {
        public:
            typedef Vec<nHost, T, SPHost> HostVec;

            //static const
            //static const std::size_t mIndices[nIndices];

            inline T& operator[](std::size_t i) {
                assert(i < n);
                return *(reinterpret_cast<HostVec*>(this));
            }

            inline const T& operator[](std::size_t i) const {
                assert(i < n);
                return *(reinterpret_cast<HostVec*>(this));
            }

        private:

        };*/

        // Only for View
        namespace view {

            struct Fail {
                constexpr static bool passed = false;
            };

            template <std::size_t n, std::size_t... indices>
            struct Check {
                constexpr static bool passed = true;
            };

            template <std::size_t n, std::size_t first, std::size_t... indices>
            struct Check<n, first, indices...> {
                constexpr static bool thisPassed = first < n;
                constexpr static bool passed = Select<thisPassed, Check<n, indices...>, Fail>::Result::passed;
            };

        } // namespace view

        // Private namespace for Vec
        class Private {

            // Private storage policies
            template <std::size_t n, typename T>
            class PointerStorage {
            public:
                constexpr static bool aligned = false;

                inline T& operator[](std::size_t i) {
                    assert(i < n);
                    return *mData[i];
                }

                inline const T& operator[](std::size_t i) const {
                    assert(i < n);
                    return *mData[i];
                }

                inline void setPointer(std::size_t i, T* ptr) {
                    assert(i < n);
                    mData[i] = ptr;
                }

            private:
                T* mData[n];
            };

            // View class
            template <std::size_t n, typename T, GLML_VEC_SP SP, std::size_t... indices>
            class View {
            public:
                // Assertions
                static_assert(sizeof...(indices) > 0, "A View has to have at least one index.");

                // Typedefs
                typedef Vec<n, T, SP> VecType;

                // Constants
                constexpr static std::size_t nIndices = sizeof...(indices);
                constexpr static bool valid = view::Check<n, indices...>::passed;
                static const std::size_t mIndices[nIndices];

                // Check if indices are valid
                constexpr static inline void check()
                { static_assert(valid, "Indices are not valid."); }

                // Get indices
                static const inline std::size_t index(std::size_t i)
                { check(); return mIndices[i]; }

                // Index operator
                inline T& operator[](std::size_t i) {
                    assert(i < n);
                    return (*reinterpret_cast<VecType*>(this))[index(i)];
                }

                inline const T& operator[](std::size_t i) const {
                    assert(i < n);
                    return (*reinterpret_cast<const Vec<n, T, SP>*>(this))[index(i)];
                }

                // Convertion
                inline operator T&() {
                    static_assert(nIndices == 0, "Only views with one index can be converted.");
                    return *this[0];
                }

                inline operator const T&() const {
                    static_assert(nIndices == 0, "Only views with one index can be converted.");
                    return *this[0];
                }
            };

        }; //Private namespace for Vec

        // View statics
        template <std::size_t n, typename T, GLML_VEC_SP SP, std::size_t... indices>
        const std::size_t Private::View<n, T, SP, indices...>::mIndices[nIndices] = { indices... };

    } // namespace GLML_VEC_NAMESPACE_NAME

    // Assignment
    namespace assignm {

        template <bool isArray, std::size_t begin>
        struct UnknownTypeAssignment {
            template <std::size_t nv, typename Tv, GLML_VEC_SP SPv,
                      typename Tf, typename... Args>
            static inline void run(Vec<nv, Tv, SPv>& v, const Tf& f, const Args&... args);
        };

        template <std::size_t begin>
        struct UnknownTypeAssignment<true, begin> {
            template <std::size_t nv, typename Tv, GLML_VEC_SP SPv,
                      typename Tf, typename... Args>
            static inline void run(Vec<nv, Tv, SPv>& v, const Tf& f, const Args&... args);
        };

        template <std::size_t begin>
        struct Assignment {
            template <std::size_t nv, typename Tv, GLML_VEC_SP SPv, typename... Args>
            static inline void run(Vec<nv, Tv, SPv>&, const Args&...) {
                static_assert(sizeof...(Args) == 0, "GLML corrupted.");
                static_assert(begin == nv, "Not enough assignment parameters.");
            }

            template <std::size_t nv, typename Tv, GLML_VEC_SP SPv,
                      std::size_t nf, typename Tf, GLML_VEC_SP SPf, std::size_t... indices,
                      typename... Args>
            static inline void run(Vec<nv, Tv, SPv>& v, const vec::Private::View<nf, Tf, SPf, indices...>& f,
                               const Args&... args) {
                constexpr static std::size_t step = f.nIndices;
                static_assert((begin + step) <= nv, "Too many assignment paramters.");
                for (std::size_t i = 0; i < step; ++i)
                    v[begin + i] = f[i];
                Assignment<begin + step>::run(v, args...);
            }

            template <std::size_t nv, typename Tv, GLML_VEC_SP SPv,
                      std::size_t nf, typename Tf, GLML_VEC_SP SPf, typename... Args>
            static inline void run(Vec<nv, Tv, SPv> &v, const Vec<nf, Tf, SPf>& f, const Args&... args) {
                constexpr static std::size_t step = nf;
                static_assert((begin + step) <= nv, "Too many assignment parameters.");
                for (std::size_t i = 0; i < step; ++i)
                    v[begin + i] = f[i];
                Assignment<begin + step>::run(v, args...);
            }

            template <std::size_t nv, typename Tv, GLML_VEC_SP SPv,
                      typename Tf, typename... Args>
            static inline void run(Vec<nv, Tv, SPv>& v, const Tf& f, const Args&... args)
            { UnknownTypeAssignment<IsArray<Tf>::result, begin>::run(v, f, args...); }
        };

        template <bool isArray, std::size_t begin>
        template <std::size_t nv, typename Tv, GLML_VEC_SP SPv,
                  typename Tf, typename... Args>
        inline void UnknownTypeAssignment<isArray, begin>::run
        (Vec<nv, Tv, SPv>& v, const Tf& f, const Args&... args) {
            constexpr static std::size_t step = 1;
            static_assert((begin + step) <= nv, "Too many assignment parameters.");
            v[begin] = f;
            Assignment<begin + step>::run(v, args...);
        }

        template <std::size_t begin>
        template <std::size_t nv, typename Tv, GLML_VEC_SP SPv,
                  typename Tf, typename... Args>
        inline void UnknownTypeAssignment<true, begin>::run
        (Vec<nv, Tv, SPv>& v, const Tf& f, const Args&... args) {
            static_assert(sizeof...(args) == 0, "Assignment array paramters of " \
                          "undefined length always have to be the last.");
            for (std::size_t i = 0; i < (nv - begin); ++i)
                v[begin + i] = f[i];
        }

    } // namespace assignm

    template <typename Tv, typename First, typename... Args>
    inline void assign(Tv& v, const First& first, const Args&... args)
    { assignm::Assignment<0>::run(v, first, args...); }

    // Display
    template <std::size_t n, typename T, GLML_VEC_SP SP>
    inline void display(const Vec<n, T, SP>& v) {
        for (std::size_t i = 0; i < n; ++i) {
            std::cout << v[i];
            if (i < n - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    // Swizzle
    // Static swizzle
    template <std::size_t... indices, std::size_t n, typename T, GLML_VEC_SP SP>
    inline vec::Private::View<n, T, SP, indices...>& swizzle(Vec<n, T, SP>& v) {
        return *reinterpret_cast<vec::Private::View<n, T, SP, indices...>*>(&v);
    }

    template <std::size_t... indices, std::size_t n, typename T, GLML_VEC_SP SP>
    inline vec::Private::View<n, T, SP, indices...>& swizzle(const Vec<n, T, SP>& v) {
        return *reinterpret_cast<const vec::Private::View<n, T, SP, indices...>*>(&v);
    }


    // Runtime swizzle
    template <std::size_t n, typename T, GLML_VEC_SP SP, typename... Indices>
    inline Vec<sizeof...(Indices), T, vec::Private::PointerStorage> swizzle
    (Vec<n, T, SP>& v, Indices... indices) {
        const std::size_t cindices[sizeof...(Indices)] = { indices... };
        Vec<sizeof...(Indices), T, vec::Private::PointerStorage> vp;

        for (std::size_t i = 0; i < sizeof...(Indices); ++i)
            vp.mData.setPointer(i, &v[cindices[i]]);

        return vp;
    }



    // Union Vec definition
    template <std::size_t n, typename T, GLML_VEC_SP SP>
    union Vec {
    public:
        // Public to all other Vecs
        template <std::size_t, typename, GLML_VEC_SP>
        friend union Vec;

        // Assertions
        static_assert(!IsArray<T>::result, "You may not store an array/pointer.");

        // Typedefs
        typedef SP<n, T> ConfiguredSP;

        // Friends
        /*template <std::size_t nF, typename TF, GLML_VEC_SP SPF, typename... IndicesF>
        friend inline Vec<sizeof...(IndicesF), TF, vec::Private::PointerStorage> glml::vec::swizzle
        (Vec<nF, TF, SPF>& v, IndicesF... indices);*/

        // Ctor, dtor
        Vec()
        { }

        template <typename First, typename... Args>
        Vec(const First& first, const Args&... args)
        { assign(first, args...); }

        // Index operator
        T& operator[](std::size_t i)
        { return mData[i]; }

        const T& operator[](std::size_t i) const
        { return mData[i]; }

        // Additional methods
        template <typename First, typename... Args>
        void assign(const First& first, const Args&... args)
        { glml::assign(*this, first, args...); }

        void display() const
        { glml::display(*this); }

        template <std::size_t... indices>
        vec::Private::View<n, T, SP, indices...>& swizzle()
        { return glml::swizzle<indices...>(*this); }

        template <std::size_t... indices>
        vec::Private::View<n, T, SP, indices...>& swizzle() const
        { return glml::swizzle<indices...>(*this); }

        template <typename... Indices>
        Vec<sizeof...(Indices), T, vec::Private::PointerStorage> swizzle(Indices... indices)
        { return glml::swizzle(*this, indices...); }

        // GLSL Compatibility
        // One index
        vec::Private::View<n, T, SP, 0> x;
        vec::Private::View<n, T, SP, 1> y;
        vec::Private::View<n, T, SP, 2> z;
        vec::Private::View<n, T, SP, 3> w;

        // Two indices
        vec::Private::View<n, T, SP, 0, 1> xy;
        vec::Private::View<n, T, SP, 0, 1> yx;

        // Three indices
        vec::Private::View<n, T, SP, 0, 1, 2> xyz;

        // The data
        ConfiguredSP mData;
    };

} // namespace GLML_NAMESPACE_VEC

#endif // GLML_VEC_H
