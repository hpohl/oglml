#ifndef OGLML_VEC_FUNCS_HPP
#define OGLML_VEC_FUNCS_HPP

#include <iostream>

#include <cmath>

#include <oglml/vec/info.hpp>
#include <oglml/vec/expression.hpp>
#include <oglml/vec/swizzlerfwd.hpp>
#include <oglml/helpers/errors.hpp>
#include <oglml/helpers/indextools.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/constants.hpp>

namespace oglml {
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
    inline const typename oglml::vec::detail::VecFunc<bool, Tlhs, Trhs>::Result
    equal(const Tlhs& lhs, const Trhs& rhs) {
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
    equal(const Tlhs& lhs, const Trhs& rhs) {
        for (std::size_t i = 0; i < Tlhs::n; ++i) {
            if (lhs[i] != rhs)
                return false;
        }
        return true;
    }

    template <class Tlhs, typename Trhs>
    inline const typename std::enable_if<
    !vec::detail::IsVec<Tlhs>::result && vec::detail::IsVec<Trhs>::result, bool>::type
    equal(const Tlhs& lhs, const Trhs& rhs) {
        return equal(rhs, lhs);
    }

    template <typename Tlhs, typename Trhs>
    inline auto notEqual(const Tlhs& lhs, const Trhs& rhs) -> decltype(!equal(lhs, rhs)) {
        return !equal(lhs, rhs);
    }

    // Promotion
    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<Promotion, T>::Result>::Result
    promote(const T& x) {
        typename CreateCalcExpressionVec<Promotion, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

    // Negation
    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<Negation, T>::Result>::Result
    negate(const T& x) {
        typename CreateCalcExpressionVec<Negation, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

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
    // Degrees
    template <typename T>
    inline const typename
    std::enable_if<!vec::detail::IsVec<T>::result, T>::type
    degrees(const T& x) {
        return Degrees::run(x);
    }

    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<Degrees, T>::Result>::Result
    degrees(const T& x) {
        typename CreateCalcExpressionVec<Degrees, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

    // Exponentation
    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<Exp, T>::Result>::Result
    exp(const T& x) {
        typename CreateCalcExpressionVec<Exp, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<Exp2, T>::Result>::Result
    exp2(const T& x) {
        typename CreateCalcExpressionVec<Exp2, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

    // Inverse sqrt
    template <typename T>
    inline const typename
    std::enable_if<!vec::detail::IsVec<T>::result, T>::type
    inversesqrt(const T& x)
    { return InverseSqrt::run(x); }

    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<InverseSqrt, T>::Result>::Result
    inversesqrt(const T& x) {
        typename CreateCalcExpressionVec<InverseSqrt, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

    // Logarithm
    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<Log, T>::Result>::Result
    log(const T& x) {
        typename CreateCalcExpressionVec<Log, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<Log2, T>::Result>::Result
    log2(const T& x) {
        typename CreateCalcExpressionVec<Log2, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

    // Pow
    template <typename Tx, typename Ty>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<Tx, Ty>::result>::type,
    typename CreateExpressionVec<Pow, Tx, Ty>::Result>::Result
    pow(const Tx& x, const Ty& y) {
        typename CreateExpressionVec<Pow, Tx, Ty>::Result vex;
        vex.data.init(x, y);
        return vex;
    }

    // Radians
    template <typename T>
    inline const typename
    std::enable_if<!vec::detail::IsVec<T>::result, T>::type
    radians(const T& x) {
        return Radians::run(x);
    }

    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<Radians, T>::Result>::Result
    radians(const T& x) {
        typename CreateCalcExpressionVec<Radians, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

    // Squareroot
    template <typename T>
    inline const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsVec<T>::result>::type,
    typename CreateCalcExpressionVec<Sqrt, T>::Result>::Result
    sqrt(const T& x) {
        typename CreateCalcExpressionVec<Sqrt, T>::Result vex;
        vex.data.init(x);
        return vex;
    }



    // --------------------------------------------------------------------------
    // Make some funcs visible
    namespace glsl {

        using oglml::degrees;
        using oglml::inversesqrt;
        using oglml::radians;

    } // namespace glsl

} // namespace oglml

#endif // OGLML_VEC_FUNC_HPP
