#ifndef OGLML_VEC_HPP
#define OGLML_VEC_HPP

#include <iostream>
#include <tuple>
#include <cassert>

#include <oglml/helpers/select.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/generate.hpp>
#include <oglml/helpers/errors.hpp>
#include <oglml/helpers/traits.hpp>
#include <oglml/helpers/constexpr.hpp>

namespace oglml {

    namespace vec {
        // Storage policies forward declaration
        struct DefaultStorage;

        template <class Op,
                  std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
                  std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
        struct ExpressionStorage;

        template <class Op, std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
        struct LhsExpressionStorage;
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
        struct ExpressionStorage : private oglml::DimProcessable<nlhs, nrhs> {
            constexpr static bool returnsReference = false;

            template <std::size_t n, typename T>
            class Container {
            public:
                typedef decltype(Op::run(priv::generate<CRlhs>(), priv::generate<CRrhs>())) ReturnType;
                typedef const ReturnType ConstReturnType;

                template <typename CR, typename Tv, int ti>
                static CR access(const Tv& obj, std::size_t i, Int2Type<ti>)
                { return obj[i]; }

                template <typename CR, typename Tv>
                static CR access(const Tv& obj, std::size_t, Int2Type<1>)
                { return obj[0]; }

                ReturnType operator[](std::size_t i) const
                { return Op::run(access<CRlhs>(*lhs, i, Int2Type<nlhs>()), access<CRrhs>(*rhs, i, Int2Type<nrhs>())); }

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
                typedef decltype(Op::run(priv::generate<CRlhs>(), priv::generate<Trhs>())) ReturnType;
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
        class Swizzler : public Expression<sizeof...(indices), Swizzler<n, T, SP, indices...>,
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
            static std::size_t index(std::size_t i)
            { assert(i < nIndices); validate(); return indexArray[i]; }

            // Index operator
            ReturnType operator[](std::size_t i)
            { return host()[index(i)]; }

            ConstReturnType operator[](std::size_t i) const
            { return host()[index(i)]; }

            // Cast operator
            operator ReturnType()
            { dimAssert<nIndices, 1>(); return host()[index(0)]; }

            operator ConstReturnType() const
            { dimAssert<nIndices, 1>(); return host()[index(0)]; }

        private:
            Swizzler() { }
            Swizzler(const Swizzler&) { }

            Host& host()
            { return *reinterpret_cast<Host*>(this); }

            const Host& host() const
            { return *reinterpret_cast<const Host*>(this); }
        };

        template <std::size_t n, typename T, class SP, std::size_t... indices>
        const std::size_t Swizzler<n, T, SP, indices...>::indexArray[nIndices] = { indices... };

    } // namespace vec

    // Global operators
    // Helper
    template <std::size_t n1, std::size_t n2>
    struct Max {
        constexpr static std::size_t result = n1 > n2 ? n1 : n2;
    };

    // expression OP expression
    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    const Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() + priv::generate<CRrhs>()),
    vec::ExpressionStorage<Plus,
    nlhs, Dlhs, Rlhs, CRlhs,
    nrhs, Drhs, Rrhs, CRrhs> > operator+
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
     const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() + priv::generate<CRrhs>()),
                vec::ExpressionStorage<Plus, nlhs, Dlhs, Rlhs, CRlhs,
                nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    const Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() - priv::generate<CRrhs>()),
    vec::ExpressionStorage<Minus,
    nlhs, Dlhs, Rlhs, CRlhs,
    nrhs, Drhs, Rrhs, CRrhs> > operator-
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
     const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() - priv::generate<CRrhs>()),
                vec::ExpressionStorage<Minus, nlhs, Dlhs, Rlhs, CRlhs,
                nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    const Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() * priv::generate<CRrhs>()),
    vec::ExpressionStorage<Multiplies,
    nlhs, Dlhs, Rlhs, CRlhs,
    nrhs, Drhs, Rrhs, CRrhs> > operator*
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
     const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() * priv::generate<CRrhs>()),
                vec::ExpressionStorage<Multiplies, nlhs, Dlhs, Rlhs, CRlhs,
                nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    const Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() / priv::generate<CRrhs>()),
    vec::ExpressionStorage<Divides,
    nlhs, Dlhs, Rlhs, CRlhs,
    nrhs, Drhs, Rrhs, CRrhs> > operator/
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
     const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() / priv::generate<CRrhs>()),
                vec::ExpressionStorage<Divides, nlhs, Dlhs, Rlhs, CRlhs,
                nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    // expression OP value
    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
    typename GetFirst<const Vec<nlhs, decltype(priv::generate<CRlhs>() + priv::generate<Trhs>()),
    vec::LhsExpressionStorage<Plus, nlhs, Dlhs, Rlhs, CRlhs, Trhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Trhs>::value>::type>::Result operator+
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs, const Trhs& rhs) {
        Vec<nlhs, decltype(priv::generate<CRlhs>() + priv::generate<Trhs>()),
            vec::LhsExpressionStorage<Plus, nlhs, Dlhs, Rlhs, CRlhs, Trhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
    typename GetFirst<const Vec<nlhs, decltype(priv::generate<CRlhs>() * priv::generate<Trhs>()),
    const vec::LhsExpressionStorage<Multiplies, nlhs, Dlhs, Rlhs, CRlhs, Trhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Trhs>::value>::type>::Result operator*
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs, const Trhs& rhs) {
        Vec<nlhs, decltype(priv::generate<CRlhs>() * priv::generate<Trhs>()),
            vec::LhsExpressionStorage<Multiplies, nlhs, Dlhs, Rlhs, CRlhs, Trhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    namespace vec {
        // Make operators visible for Swizzler
        using oglml::operator+;
        using oglml::operator-;
        using oglml::operator*;
        using oglml::operator/;
    }


    // Global helper funcs
    // Assign
    namespace assignment {

        template <std::size_t begin>
        struct Assignment {

            // Using SFINAE
            template <std::size_t n, class D, typename R, typename CR, typename... Args>
            static typename std::enable_if<sizeof...(Args) == 0>::type run
            (const vec::Expression<n, D, R, CR>&, const Args&...) {
                static_assert(begin == n, "Not enough assignment parameters.");
            }

            template <std::size_t ne, class De, typename Re, typename CRe,
                      std::size_t nv, class Dv, typename Rv, typename CRv, typename... Args>
            static void run(vec::Expression<ne, De, Re, CRe>& ex,
                            const vec::Expression<nv, Dv, Rv, CRv>& v, const Args&... args) {
                static_assert((begin + nv) <= ne, "Too many assignment parameters.");
                Assignment<begin + nv>::run(assignArray<begin, nv>(ex, v), args...);
            }

            template <std::size_t ne, class De, typename Re, typename CRe, typename First, typename... Args>
            static typename std::enable_if<!std::is_base_of<vec::BaseExpression, First>::value>::type run
            (vec::Expression<ne, De, Re, CRe>& ex, const First& first, const Args&... args) {
                static_assert((begin + 1) <= ne, "Too many assignment parameters.");
                ex[begin] = first;
                Assignment<begin + 1>::run(ex, args...);
            }

        };

    }

    template <std::size_t n, class D, typename R, typename CR, typename First, typename... Args>
    void assign(vec::Expression<n, D, R, CR>& ex, const First& first, const Args&... args) {
        assignment::Assignment<0>::run(ex, first, args...);
    }

    // Print
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

        // Ctor and dtor
        Vec() { }

        Vec(const Vec& rhs) : data(rhs.data) { }

        /*template <std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
        Vec(const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                (*this)[i] = rhs[i];
        }*/

        template <typename First, typename... Args>
        Vec(const First& first, const Args&... args)
        { assign(*this, first, args...); }

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
            typename Swizzler<1>::Type y;
            typename Swizzler<0, 1>::Type xy;
            typename Swizzler<0, 1>::Type yx;
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
