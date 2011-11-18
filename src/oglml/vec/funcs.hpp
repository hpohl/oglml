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

    // Assign
    namespace nassign {

        template <std::size_t begin>
        struct Assignment {

            template <class Tex, typename... Args>
            static typename std::enable_if<!sizeof...(Args)>::type run
            (const Tex&, const Args&...) {
                static_assert(begin == Tex::n, "Not enough assignment parameters.");
            }

            template <typename Tex, typename First, typename... Args>
            static typename std::enable_if<vec::detail::IsMVec<First>::result>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert((begin + First::n) <= Tex::n, "Too many assignment parameters.");
                Assignment<begin + First::n>::run(detail::assignArray<begin, First::n>(ex, first), args...);
            }

            template <class Tex, typename First, typename... Args>
            static typename std::enable_if
            <!vec::detail::IsMVec<First>::result && !sizeof...(Args) && begin == 0>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert(Tex::n > 0, "Too many assignment parameters.");
                detail::assignValue<Tex::n>(ex, first);
            }

            template <class Tex, typename First, typename... Args>
            static typename std::enable_if
            <!vec::detail::IsMVec<First>::result && ((sizeof...(Args) > 0) || (begin > 0))>::type
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

    // Exponentation
    /*template <typename Tx, typename Ty>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tx, Ty>::result>::type,
    typename CreateExpressionVec<Exp, Tx, Ty>::Result>::Result
    exp(const Tx& x, const Ty& y) {
        typename CreateExpressionVec<Exp, Tx, Ty>::Result vex;
        vex.data.init(x, y);
        return vex;
    }*/

    // Logarithm
    template <typename T>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<T>::result>::type,
    typename CreateCalcExpressionVec<Log, T>::Result>::Result
    log(const T& x) {
        typename CreateCalcExpressionVec<Log, T>::Result vex;
        vex.data.init(x);
        return vex;
    }

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
            static typename std::enable_if<vec::detail::IsMVec<First>::result>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert((begin + First::n) <= Tex::n, "Too many assignment parameters.");
                Assignment<begin + First::n, Op>::run(detail::opassignArray<Op, begin, First::n>
                                                      (ex, first), args...);
            }

            template <class Tex, typename First, typename... Args>
            static typename std::enable_if
            <!vec::detail::IsMVec<First>::result && !sizeof...(Args) && begin == 0>::type
            run(Tex& ex, const First& first, const Args&... args) {
                static_assert(Tex::n > 0, "Too many assignment parameters.");
                detail::opassignValue<Op, Tex::n>(ex, first);
            }

            template <class Tex, typename First, typename... Args>
            static typename std::enable_if
            <!vec::detail::IsMVec<First>::result && ((sizeof...(Args) > 0) || (begin > 0))>::type
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

    // Pow
    template <typename Tx, typename Ty>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tx, Ty>::result>::type,
    typename CreateExpressionVec<Pow, Tx, Ty>::Result>::Result
    pow(const Tx& x, const Ty& y) {
        typename CreateExpressionVec<Pow, Tx, Ty>::Result vex;
        vex.data.init(x, y);
        return vex;
    }

    // Print
    template <class T>
    typename oglml::vec::detail::VecFunc<void, T>::Result print(const T& vec) {
        for (std::size_t i = 0; i < T::n; ++i) {
            std::cout << vec[i];
            if (i < T::n - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    // Radians
    template <typename T>
    inline const T radians(const T& degrees)
    { return (pi / 180.0) * degrees; }

    // Degrees
    template <typename T>
    inline const T degrees(const T& radians)
    { return (180.0 / pi) * radians; }


} // namespace oglml

#endif // OGLML_VEC_FUNC_HPP
