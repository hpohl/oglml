#ifndef OGLML_VEC_FUNCS_HPP
#define OGLML_VEC_FUNCS_HPP

#include <iostream>

#include <oglml/vec/info.hpp>
#include <oglml/vec/expression.hpp>
#include <oglml/vec/swizzlerfwd.hpp>
#include <oglml/helpers/errors.hpp>
#include <oglml/helpers/indextools.hpp>
#include <oglml/helpers/operations.hpp>

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

            template <typename Tex, typename Tv, typename... Args>
            static typename vec::detail::VecFunc<void, Tv>::Result run(Tex& ex,
                            const Tv& v, const Args&... args) {
                static_assert((begin + Tv::n) <= Tex::n, "Too many assignment parameters.");
                Assignment<begin + Tv::n>::run(detail::assignArray<begin, Tv::n>(ex, v), args...);
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
            <!vec::detail::IsVec<First>::result && ((sizeof...(Args) > 0) || (begin + 1 == Tex::n))>::type
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

} // namespace oglml

#endif // OGLML_VEC_FUNC_HPP
