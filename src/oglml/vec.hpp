#ifndef GLML_VEC_H
#define GLML_VEC_H

#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <cassert>
#include <type_traits>

#define GLML_NAMESPACE_NAME glml
#define GLML_VEC_NAMESPACE_NAME vec

// No template alias support in GCC yet
//#define class template <std::size_t, typename> class

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
        struct DefaultStorage;
    }

    // Vec forward declarations
    template <std::size_t n, typename T, typename SP = vec::DefaultStorage>
    union Vec;

    // Everything belongs to Vec
    namespace GLML_VEC_NAMESPACE_NAME {

        // Storage Policies
        struct DefaultStorage {
            template <std::size_t n, typename T>
            class Data {
            public:
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
        };

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
            struct PointerStorage {

                constexpr static bool aligned = false;

                template <std::size_t n, typename T>
                class Data {
                public:
                    T& operator[](std::size_t i) {
                        assert(i < n);
                        return *mData[i];
                    }

                    const T& operator[](std::size_t i) const {
                        assert(i < n);
                        return *mData[i];
                    }

                    void setPointer(std::size_t i, T* ptr) {
                        assert(i < n);
                        mData[i] = ptr;
                    }

                private:
                    T* mData[n];
                };
            };

            template <std::size_t nHost, class SPHost>
            struct ViewStorage {

               template <std::size_t n, typename T>
               class Data {
               public:
                   typedef Vec<nHost, T, SPHost> HostVec;

                   T& operator[](std::size_t i) {
                       assert(i < n);
                       return (*reinterpret_cast<HostVec*>(this))[i];
                   }

                   const T& operator[](std::size_t i) const {
                       assert(i < n);
                       return (*reinterpret_cast<const HostVec*>(this))[i];
                   }

                   void setIndices(const std::vector<std::size_t>& v)
                   { mIndices = v; }

               private:
                   std::vector<std::size_t> mIndices;
               };

            };

            // View class
            template <std::size_t n, typename T, class SP, std::size_t... indices>
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
                { static_assert(valid, "Indices are not valid."); } // TODO

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
        template <std::size_t n, typename T, class SP, std::size_t... indices>
        const std::size_t Private::View<n, T, SP, indices...>::mIndices[nIndices] = { indices... };

    } // namespace GLML_VEC_NAMESPACE_NAME

    // Assignment
    namespace assignm {

        template <bool isArray, std::size_t begin>
        struct UnknownTypeAssignment {
            template <std::size_t nv, typename Tv, class SPv,
                      typename Tf, typename... Args>
            static inline void run(Vec<nv, Tv, SPv>& v, const Tf& f, const Args&... args);
        };

        template <std::size_t begin>
        struct UnknownTypeAssignment<true, begin> {
            template <std::size_t nv, typename Tv, class SPv,
                      typename Tf, typename... Args>
            static inline void run(Vec<nv, Tv, SPv>& v, const Tf& f, const Args&... args);
        };

        template <std::size_t begin>
        struct Assignment {
            template <std::size_t nv, typename Tv, class SPv, typename... Args>
            static inline void run(Vec<nv, Tv, SPv>&, const Args&...) {
                static_assert(sizeof...(Args) == 0, "GLML corrupted.");
                static_assert(begin == nv, "Not enough assignment parameters.");
            }

            template <std::size_t nv, typename Tv, class SPv,
                      std::size_t nf, typename Tf, class SPf, std::size_t... indices,
                      typename... Args>
            static inline void run(Vec<nv, Tv, SPv>& v, const vec::Private::View<nf, Tf, SPf, indices...>& f,
                               const Args&... args) {
                constexpr static std::size_t step = f.nIndices;
                static_assert((begin + step) <= nv, "Too many assignment paramters.");
                for (std::size_t i = 0; i < step; ++i)
                    v[begin + i] = f[i];
                Assignment<begin + step>::run(v, args...);
            }

            template <std::size_t nv, typename Tv, class SPv,
                      std::size_t nf, typename Tf, class SPf, typename... Args>
            static inline void run(Vec<nv, Tv, SPv> &v, const Vec<nf, Tf, SPf>& f, const Args&... args) {
                constexpr static std::size_t step = nf;
                static_assert((begin + step) <= nv, "Too many assignment parameters.");
                for (std::size_t i = 0; i < step; ++i)
                    v[begin + i] = f[i];
                Assignment<begin + step>::run(v, args...);
            }

            template <std::size_t nv, typename Tv, class SPv,
                      typename Tf, typename... Args>
            static inline void run(Vec<nv, Tv, SPv>& v, const Tf& f, const Args&... args)
            { UnknownTypeAssignment<IsArray<Tf>::result, begin>::run(v, f, args...); }
        };

        template <bool isArray, std::size_t begin>
        template <std::size_t nv, typename Tv, class SPv,
                  typename Tf, typename... Args>
        inline void UnknownTypeAssignment<isArray, begin>::run
        (Vec<nv, Tv, SPv>& v, const Tf& f, const Args&... args) {
            constexpr static std::size_t step = 1;
            static_assert((begin + step) <= nv, "Too many assignment parameters.");
            v[begin] = f;
            Assignment<begin + step>::run(v, args...);
        }

        template <std::size_t begin>
        template <std::size_t nv, typename Tv, class SPv,
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


    // Length
    template <std::size_t n, typename T, class SP>
    inline float length(const Vec<n, T, SP>& v) {
        static_assert(n < 4, "Length calculations are only available for vectors with " \
                      "a dimension count of 2 or 3.");
        float len;
        for (std::size_t i = 0; i < n; ++i)
            len += v[i] * v[i];
        return std::sqrt(len);
    }


    // Print
    template <std::size_t n, typename T, class SP>
    inline void print(const Vec<n, T, SP>& v) {
        for (std::size_t i = 0; i < n; ++i) {
            std::cout << v[i];
            if (i < n - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }


    // Swizzle
    // Static swizzle
    template <std::size_t... indices, std::size_t n, typename T, class SP>
    inline vec::Private::View<n, T, SP, indices...>& swizzle(Vec<n, T, SP>& v) {
        return *reinterpret_cast<vec::Private::View<n, T, SP, indices...>*>(&v);
    }

    template <std::size_t... indices, std::size_t n, typename T, class SP>
    inline vec::Private::View<n, T, SP, indices...>& swizzle(const Vec<n, T, SP>& v) {
        return *reinterpret_cast<const vec::Private::View<n, T, SP, indices...>*>(&v);
    }

    // Runtime swizzle
    template <std::size_t n, typename T, class SP, typename... Indices>
    inline Vec<sizeof...(Indices), T, vec::Private::PointerStorage> swizzle
    (Vec<n, T, SP>& v, Indices... indices) {
        const std::vector<std::size_t> vindices = { indices... };
        Vec<sizeof...(Indices), T, vec::Private::PointerStorage> vp;
        for (std::size_t i = 0; i < sizeof...(Indices); ++i)
            vp.data.setPointer(i, &v[vindices[i]]);
        return vp;
    }

    template <std::size_t n, typename T, class SP, typename... Indices>
    inline const Vec<sizeof...(Indices), T, vec::Private::PointerStorage> swizzle
    (const Vec<n, T, SP>& v, Indices... indices) {
        std::vector<std::size_t> vindices = { indices... }; // TODO
        Vec<sizeof...(Indices), T, vec::Private::PointerStorage> vp;
        for (std::size_t i = 0; i < sizeof...(Indices); ++i)
            vp.data.setPointer(i, const_cast<T*>(&v[vindices[i]]));
        return vp;
    }

    // Union Vec definition
    template <std::size_t n, typename T, class SP>
    union Vec {
        // Public to all other Vecs
        template <std::size_t, typename, class>
        friend union Vec;

        // Typedefs
        typedef Vec<n, T, SP> ThisType;
        typedef typename SP::template Data<n, T> ConfiguredData;

        // Helper
    private:
        static ThisType& generate()
        { return static_cast<ThisType*>(0); }

        static const ThisType& generateConst()
        { return static_cast<const ThisType*>(0); }

        static ConfiguredData& generateData()
        { return static_cast<ConfiguredData*>(0); }

        static const ConfiguredData& generateConstData()
        { return static_cast<const ConfiguredData*>(0); }

    public:

        // Assertions
        static_assert(!IsArray<T>::result, "You may not store an array/pointer.");

        // Friends

        // Ctor, dtor
        Vec()
        { }

        Vec(const Vec& rhs) : data(rhs.data)
        { }

        template <typename First, typename... Args>
        Vec(const First& first, const Args&... args)
        { try { assign(first, args...); } catch(...) { std::abort(); } }

        ~Vec() { }

        // Index operator
        const T& operator[](std::size_t i) const
        { return data[i]; }

        T& operator[](std::size_t i) // TODO
        { return data[i]; }

        // Additional methods
        template <typename First, typename... Args>
        auto assign(const First& first, const Args&... args) ->
        decltype(glml::assign(ThisType::generate(), first, args...))
        { glml::assign(*this, first, args...); }

        decltype(glml::length(generate())) length() const
        { return glml::length(*this); }

        decltype(glml::print(ThisType::generate())) print() const
        { glml::print(*this); }

        template <std::size_t... indices>
        decltype(glml::swizzle<indices...>(ThisType::generate())) swizzle()
        { return glml::swizzle<indices...>(*this); }

        template <std::size_t... indices>
        const Vec<sizeof...(indices), T, vec::Private::PointerStorage> swizzle() const
        { return glml::swizzle<indices...>(*this); }

        template <typename... Indices>
        auto swizzle(Indices... indices) ->
        decltype(glml::swizzle(ThisType::generate(), indices...))
        { return glml::swizzle(*this, indices...); }

        template <typename... Indices>
        auto swizzle(Indices... indices) const ->
        decltype(glml::swizzle(ThisType::generate(), indices...))
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
        ConfiguredData data;
    };

    // Typedefs
    typedef Vec<2, float> vec2;
    typedef Vec<3, float> vec3;
    typedef Vec<4, float> vec4;

} // namespace GLML_NAMESPACE_VEC

#endif // GLML_VEC_H
