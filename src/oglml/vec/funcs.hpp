#ifndef OGLML_VEC_FUNCS_HPP
#define OGLML_VEC_FUNCS_HPP

#include <iostream>

#include <cmath>

#include <oglml/vec/info.hpp>
#include <oglml/vec/expression.hpp>
#include <oglml/vec/swizzlerfwd.hpp>
#include <oglml/vec/storagepolicies/default.hpp>
#include <oglml/helpers/errors.hpp>
#include <oglml/helpers/indextools.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/constants.hpp>

namespace oglml {
    // Helper makro to do no code duplicates
#define OGLML_VEC_DEFINE_NONVEC_FUNC(_FUNC_NAME_, _EX_) \
    template <typename T> \
    inline const typename \
    std::enable_if<!vec::detail::IsVec<T>::result, T>::type \
    _FUNC_NAME_(const T& x) { \
        return _EX_::run(x); \
    }

#define OGLML_VEC_DEFINE_EXPR_FUNC(_FUNC_NAME_, _EX_) \
    template <typename T> \
    inline const typename detail::GetSecond \
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type, \
    typename CreateCalcExpressionVec<_EX_, T>::Result>::Result \
    _FUNC_NAME_(const T& x) { \
        typename CreateCalcExpressionVec<_EX_, T>::Result vex; \
        vex.data.init(x); \
        return vex; \
    }

#define OGLML_VEC_DEFINE_2EXPR_FUNC(_FUNC_NAME_, _EX_) \
    template <typename Tlhs, typename Trhs> \
    inline const typename std::enable_if< \
    vec::detail::IsVec<Tlhs, Trhs>::result && (Tlhs::n == Trhs::n), \
    Vec<Tlhs::n, bool, vec::ExpressionStorage<_EX_, Tlhs, Trhs> > >::type \
    _FUNC_NAME_(const Tlhs& lhs, const Trhs& rhs) { \
        Vec<Tlhs::n, bool, vec::ExpressionStorage<_EX_, Tlhs, Trhs> > vex; \
        vex.data.init(lhs, rhs); \
        return vex; \
    }


    // Vector funcs

    // -------------------------------------------------------------------------
    // Assignments
    namespace nassign {

        template <std::size_t begin>
        struct Assignment {

            template <class Tex, typename... Args>
            static typename std::enable_if<!sizeof...(Args)>::type run
            (const Tex&, const Args&...) {
                static_assert(begin == Tex::n, "Not enough assignment parameters.");
            }

            template <typename Tex, typename First, typename... Args>
            static typename std::enable_if<vec::detail::IsVec<First>::result>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert((begin + First::n) <= Tex::n, "Too many assignment parameters.");
                Assignment<begin + First::n>::run(detail::assignArray<begin, First::n>(ex, first), args...);
            }

            template <class Tex, typename First, typename... Args>
            static typename std::enable_if
            <!vec::detail::IsVec<First>::result && !sizeof...(Args) && begin == 0>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert(Tex::n > 0, "Too many assignment parameters.");
                detail::assignValue<Tex::n>(ex, first);
            }

            template <class Tex, typename First, typename... Args>
            static typename std::enable_if
            <!vec::detail::IsVec<First>::result && ((sizeof...(Args) > 0) || (begin > 0))>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert((begin + 1) <= Tex::n, "Too many assignment parameters.");
                ex[begin] = first;
                Assignment<begin + 1>::run(ex, args...);
            }
        };

    } // namespace nassign

    template <class Tex, typename First, typename... Args>
    typename vec::detail::VecFunc<Tex&, Tex>::Result
    assign(Tex& ex, const First& first, const Args&... args)
    { nassign::Assignment<0>::run(ex, first, args...); return ex; }

    // Op assign
    namespace nopassign {

        template <std::size_t begin, class Op>
        struct Assignment {

            template <class Tex, typename... Args>
            static typename std::enable_if<!sizeof...(Args)>::type run
            (const Tex&, const Args&...) {
                static_assert(begin == Tex::n, "Not enough assignment parameters.");
            }

            template <typename Tex, typename First, typename... Args>
            static typename std::enable_if<vec::detail::IsVec<First>::result>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert((begin + First::n) <= Tex::n, "Too many assignment parameters.");
                Assignment<begin + First::n, Op>::run(detail::opassignArray<Op, begin, First::n>
                                                      (ex, first), args...);
            }

            template <class Tex, typename First, typename... Args>
            static typename std::enable_if
            <!vec::detail::IsVec<First>::result && !sizeof...(Args) && begin == 0>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert(Tex::n > 0, "Too many assignment parameters.");
                detail::opassignValue<Op, Tex::n>(ex, first);
            }

            template <class Tex, typename First, typename... Args>
            static typename std::enable_if
            <!vec::detail::IsVec<First>::result && ((sizeof...(Args) > 0) || (begin > 0))>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert((begin + 1) <= Tex::n, "Too many assignment parameters.");
                Op::run(ex[begin], first);
                Assignment<begin + 1, Op>::run(ex, args...);
            }

        };

    } // namespace nopassign


    template <class Op, class Tex, typename First, typename... Args>
    typename vec::detail::VecFunc<Tex&, Tex>::Result
    opassign(Tex& ex, const First& first, const Args&... args)
    { nopassign::Assignment<0, Op>::run(ex, first, args...); return ex; }

    // -------------------------------------------------------------------------
    // General funcs    
    // Compare
    template <class Tlhs, class Trhs>
    inline const typename std::enable_if<
    vec::detail::IsVec<Tlhs, Trhs>::result && (Tlhs::n == Trhs::n), bool>::type
    compare(const Tlhs& lhs, const Trhs& rhs) {
        dimAssert<Tlhs::n, Trhs::n>();
        for (std::size_t i = 0; i < Tlhs::n; ++i) {
            if (lhs[i] != rhs[i])
                return false;
        }
        return true;
    }

    template <class Tlhs, typename Trhs>
    inline const typename std::enable_if<
    vec::detail::IsVec<Tlhs>::result && !vec::detail::IsVec<Trhs>::result, bool>::type
    compare(const Tlhs& lhs, const Trhs& rhs) {
        for (std::size_t i = 0; i < Tlhs::n; ++i) {
            if (lhs[i] != rhs)
                return false;
        }
        return true;
    }

    template <class Tlhs, typename Trhs>
    inline const typename std::enable_if<
    !vec::detail::IsVec<Tlhs>::result && vec::detail::IsVec<Trhs>::result, bool>::type
    compare(const Tlhs& lhs, const Trhs& rhs) {
        return equal(rhs, lhs);
    }

    // Equal
    OGLML_VEC_DEFINE_2EXPR_FUNC(lessThan, LessThan)
    OGLML_VEC_DEFINE_2EXPR_FUNC(lessThanEqual, LessThanEqual)

    OGLML_VEC_DEFINE_2EXPR_FUNC(greaterThan, GreaterThan)
    OGLML_VEC_DEFINE_2EXPR_FUNC(greaterThanEqual, GreaterThanEqual)

    OGLML_VEC_DEFINE_2EXPR_FUNC(equal, Equal)
    OGLML_VEC_DEFINE_2EXPR_FUNC(notEqual, NotEqual)

    // Promotion
    OGLML_VEC_DEFINE_EXPR_FUNC(promote, Promotion)

    // Negation
    OGLML_VEC_DEFINE_EXPR_FUNC(negate, Negation)

    // Print
    template <class T>
    inline typename oglml::vec::detail::VecFunc<void, T>::Result
    print(const T& vec) {
        for (std::size_t i = 0; i < T::n; ++i) {
            std::cout << vec[i];
            if (i < T::n - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    // -------------------------------------------------------------------------
    // GLSL math funcs
    // Abs
    OGLML_VEC_DEFINE_EXPR_FUNC(abs, Abs)

    // Ceil
    OGLML_VEC_DEFINE_EXPR_FUNC(ceil, Ceil)

    // Cross
    template <typename T1, typename T2>
    inline const typename std::enable_if<
    vec::detail::IsVec<T1, T2>::result && (T1::n == 3) && (T2::n == 3),
    Vec<3, float> >::type
    cross(const T1& v1, const T2& v2) {
        return Vec<3, float, vec::DefaultStorage>(
                    v1[1] * v2[2] - v2[1] * v1[2],
                    v1[2] * v2[0] - v2[2] * v1[0],
                    v1[0] * v2[1] - v2[0] * v1[1]
                    );
    }

    // Degrees
    OGLML_VEC_DEFINE_NONVEC_FUNC(degrees, Degrees)
    OGLML_VEC_DEFINE_EXPR_FUNC(degrees, Degrees)

    // Exponentation
    OGLML_VEC_DEFINE_EXPR_FUNC(exp, Exp)
    OGLML_VEC_DEFINE_EXPR_FUNC(exp2, Exp2)

    // Floor
    OGLML_VEC_DEFINE_EXPR_FUNC(floor, Floor)

    // Fract
    OGLML_VEC_DEFINE_NONVEC_FUNC(fract, Fract)
    OGLML_VEC_DEFINE_EXPR_FUNC(fract, Fract)

    // Inverse sqrt
    OGLML_VEC_DEFINE_NONVEC_FUNC(inversesqrt, InverseSqrt)
    OGLML_VEC_DEFINE_EXPR_FUNC(inversesqrt, InverseSqrt)

    // Logarithm
    OGLML_VEC_DEFINE_EXPR_FUNC(log, Log)
    OGLML_VEC_DEFINE_EXPR_FUNC(log2, Log2)

    // Radians
    OGLML_VEC_DEFINE_NONVEC_FUNC(radians, Radians)
    OGLML_VEC_DEFINE_EXPR_FUNC(radians, Radians)

    // Round
    OGLML_VEC_DEFINE_EXPR_FUNC(round, Round)

    // Squareroot
    OGLML_VEC_DEFINE_EXPR_FUNC(sqrt, Sqrt)

    // Trunc
    OGLML_VEC_DEFINE_EXPR_FUNC(trunc, Trunc)


    // --------------------------------------------------------------------------
    // Make some funcs visible
    namespace glsl {

        using oglml::degrees;
        using oglml::inversesqrt;
        using oglml::radians;

    } // namespace glsl

} // namespace oglml

#endif // OGLML_VEC_FUNC_HPP
