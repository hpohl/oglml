#ifndef OGLML_VEC_HPP
#define OGLML_VEC_HPP

#include <iostream>
#include <cassert>

#include <oglml/helpers/select.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/generate.hpp>
#include <oglml/helpers/errors.hpp>

namespace oglml {

    namespace vec {
        struct DefaultStorage;
    }

    // Forward declaration
    template <std::size_t n, typename T, class SP = vec::DefaultStorage>
    struct Vec;


    namespace vec {
        struct BaseExpression { };

        // Expression-Template
        template <std::size_t n, class Derived, typename ReturnType, typename ConstReturnType>
        class Expression : public BaseExpression {
        public:
            ReturnType operator[](std::size_t i)
            { return downCast()[i]; }

            ConstReturnType operator[](std::size_t i) const
            { return downCast()[i]; }

        private:
            Derived& downCast()
            { return *static_cast<Derived*>(this); }

            const Derived& downCast() const
            { return *static_cast<const Derived*>(this); }
        };

        // Storage policies
        struct DefaultStorage {

            constexpr static bool returnsReference = true;

            template <std::size_t n, typename T>
            class Container {
            public:

                typedef T& ReturnType;
                typedef const T& ConstReturnType;

                T& operator[](std::size_t i)
                { assert(i < n); return mData[i]; }

                const T& operator[](std::size_t i) const
                { assert(i < n); return mData[i]; }

            private:
                T mData[n];
            };

        };

        template <class Op,
                  std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
                  std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
        struct ExpressionStorage : private oglml::DimCheck<nlhs, nrhs> {
            constexpr static bool returnsReference = false;

            template <std::size_t n, typename T>
            class Container {
            public:
                typedef decltype(Op::run(detail::generate<CRlhs>(), detail::generate<CRrhs>())) ReturnType;
                typedef const ReturnType ConstReturnType;

                ReturnType operator[](std::size_t i) const
                { return Op::run((*lhs)[i], (*rhs)[i]); }

                void initialize(const Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
                                const Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs)
                { this->lhs = &lhs; this->rhs = &rhs; }

            private:
                const Expression<nlhs, Dlhs, Rlhs, CRlhs>* lhs;
                const Expression<nrhs, Drhs, Rrhs, CRrhs>* rhs;
            };

        };

        template <class Op,
                  std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
        struct LhsExpressionStorage {
            constexpr static bool returnsReference = false;

            template <std::size_t n, typename T>
            class Container {
            public:
                typedef decltype(Op::run(detail::generate<CRlhs>(), detail::generate<Trhs>())) ReturnType;
                typedef const ReturnType ConstReturnType;

                ReturnType operator[](std::size_t i) const
                { return Op::run((*lhs)[i], *rhs); }

                void initialize(const Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
                                const Trhs& rhs)
                { this->lhs = &lhs; this->rhs = &rhs; }

            private:
                const Expression<nlhs, Dlhs, Rlhs, CRlhs>* lhs;
                const Trhs* rhs;
            };

        };

        // Only for Swizzler
        namespace swizzler {

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

        } // namespace swizzler

        // Swizzler definition
        template <std::size_t n, typename T, class SP, std::size_t... indices>
        class Swizzler : public Expression<n, Swizzler<n, T, SP, indices...>,
                typename SP::template Container<n, T>::ReturnType,
                typename SP::template Container<n, T>::ConstReturnType> {
        public:
            // Typedefs
            typedef typename SP::template Container<n, T> Host;
            typedef typename Host::ReturnType ReturnType;
            typedef typename Host::ConstReturnType ConstReturnType;

            // Friends
            template <std::size_t, typename, class>
            friend struct Vec;

            // Constants
            constexpr static bool valid = swizzler::Check<n, indices...>::passed;
            constexpr static std::size_t nIndices = sizeof...(indices);

            // Helpers
            constexpr static void validate()
            { static_assert(valid, "Indices are not valid."); }

            // Indices
            static const std::size_t indexArray[nIndices];

            // Get indices
            static const std::size_t index(std::size_t i)
            { assert(i < nIndices); validate(); return indexArray[i]; }

            // Index operator
            ReturnType operator[](std::size_t i)
            { return host()[index(i)]; }

            ConstReturnType operator[](std::size_t i) const
            { return host()[index(i)]; }

        private:
            Swizzler() { }
            Swizzler(const Swizzler&) { }
            ~Swizzler() { }

            Host& host()
            { return *reinterpret_cast<Host*>(this); }

            const Host& host() const
            { return *reinterpret_cast<const Host*>(this); }
        };

        template <std::size_t n, typename T, class SP, std::size_t... indices>
        const std::size_t Swizzler<n, T, SP, indices...>::indexArray[nIndices] = { indices... };

    } // namespace vec

    // Global operators
    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    Vec<nlhs, decltype(detail::generate<CRlhs>() + detail::generate<CRrhs>()),
    vec::ExpressionStorage<Plus,
    nlhs, Dlhs, Rlhs, CRlhs,
    nrhs, Drhs, Rrhs, CRrhs> > operator+
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
     const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<nlhs, decltype(detail::generate<CRlhs>() + detail::generate<CRrhs>()),
                vec::ExpressionStorage<Plus, nlhs, Dlhs, Rlhs, CRlhs,
                nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
    Vec<nlhs, decltype(detail::generate<CRlhs>() + detail::generate<Trhs>()),
    vec::LhsExpressionStorage<Plus,
    nlhs, Dlhs, Rlhs, CRlhs, Trhs> > operator+
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs, const Trhs& rhs) {
        Vec<nlhs, decltype(detail::generate<CRlhs>() + detail::generate<Trhs>()),
            vec::LhsExpressionStorage<Plus, nlhs, Dlhs, Rlhs, CRlhs, Trhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
    Vec<nlhs, decltype(detail::generate<CRlhs>() * detail::generate<Trhs>()),
    vec::LhsExpressionStorage<Multiplies,
    nlhs, Dlhs, Rlhs, CRlhs, Trhs> > operator*
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs, const Trhs& rhs) {
        Vec<nlhs, decltype(detail::generate<CRlhs>() * detail::generate<Trhs>()),
            vec::LhsExpressionStorage<Multiplies, nlhs, Dlhs, Rlhs, CRlhs, Trhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }


    // Global helper funcs
    template <std::size_t n, class D, typename R, typename CR>
    void print(const vec::Expression<n, D, R, CR>& e) {
        for (std::size_t i = 0; i < n; ++i) {
            std::cout << e[i];
            if (i < n - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    // Vec definition
    template <std::size_t n, typename T, class SP>
    struct Vec : public vec::Expression<n, Vec<n, T, SP>,
            typename SP::template Container<n, T>::ReturnType,
            typename SP::template Container<n, T>::ConstReturnType> {
        // Typedefs
        typedef typename SP::template Container<n, T> Container;
        typedef typename Container::ReturnType ReturnType;
        typedef typename Container::ConstReturnType ConstReturnType;

        // Helper to declare swizzlers
        template <std::size_t... indices> struct Swizzler
        { typedef vec::Swizzler<n, T, SP, indices...> Type; };

        // Ctor and dtor // TODO Assignment
        Vec() { }

        Vec(T a, T b, T c) {
            (*this)[0] = a;
            (*this)[1] = b;
            (*this)[2] = c;
        }

        Vec(const Vec& rhs) : data(rhs.data) { }

        template <std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
        Vec(const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                (*this)[i] = rhs[i];
        }

        ~Vec() { }

        // Index operator
        ReturnType operator[](std::size_t i)
        { return data[i]; }

        ConstReturnType operator[](std::size_t i) const
        { return data[i]; }

        // Data
        union {
            Container data;

            // Swizzle masks
            typename Swizzler<0>::Type x;
            typename Swizzler<0, 1, 2>::Type xyz;
            typename Swizzler<2, 1, 0>::Type zyx;
        };
    };

    // GLSL Types
    typedef Vec<2, float> vec2;
    typedef Vec<3, float> vec3;
    typedef Vec<4, float> vec4;

} // namespace oglml

#endif // OGLML_VEC_HPP
