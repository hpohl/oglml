#ifndef OGLML_VEC_FUNCS_HPP
#define OGLML_VEC_FUNCS_HPP

#include <oglml/vec/expression.hpp>
#include <oglml/helpers/operations.hpp>

namespace oglml {
    // Vector funcs

    // Assign
    namespace nassign {

        template <std::size_t begin>
        struct Assignment {

            template <class I, typename... Args>
            static typename std::enable_if<sizeof...(Args) == 0>::type run
            (const vec::Expression<I>&, const Args&...) {
                static_assert(begin == I::n, "Not enough assignment parameters.");
            }

            template <class Iex, class Iv, typename... Args>
            static void run(vec::Expression<Iex>& ex,
                            const vec::Expression<Iv>& v, const Args&... args) {
                static_assert((begin + Iv::n) <= Iex::n, "Too many assignment parameters.");
                Assignment<begin + Iv::n>::run(detail::assignArray<begin, Iv::n>(ex, v), args...);
            }

            template <class I, typename First, typename... Args>
            static typename std::enable_if<!std::is_base_of<vec::BaseExpression, First>::value>::type run
            (vec::Expression<I>& ex, const First& first, const Args&... args) {
                static_assert((begin + 1) <= I::n, "Too many assignment parameters.");
                ex[begin] = first;
                Assignment<begin + 1>::run(ex, args...);
            }
        };

    } // namespace nassign

    template <class I, typename First, typename... Args>
    vec::Expression<I>& assign(vec::Expression<I>& ex, const First& first, const Args&... args)
    { nassign::Assignment<0>::run(ex, first, args...); return ex; }

    // Print
    template <class I>
    void print(const vec::Expression<I>& ex) {
        for (std::size_t i = 0; i < I::n; ++i) {
            std::cout << ex[i];
            if (i < I::n - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

} // namespace oglml

#endif // OGLML_VEC_FUNC_HPP
