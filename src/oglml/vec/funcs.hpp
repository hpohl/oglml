#ifndef OGLML_VEC_FUNCS_HPP
#define OGLML_VEC_FUNCS_HPP

#include <iostream>

#include <cmath>

namespace oglml {

    // -------------------------------------------------------------------------
    // Assignments
    namespace nassign {

        template <std::size_t begin>
        struct Assignment {

            template <std::size_t n, typename T, class SP, typename... Args>
            static typename std::enable_if<!sizeof...(Args)>::type run
            (const Vec<n, T, SP>&, const Args&...) {
                static_assert(begin == n, "Not enough assignment parameters.");
            }

            template <std::size_t nt, typename Tt, class SPt,
                      std::size_t nv, typename Tv, class SPv, typename... Args>
            static void
            run(Vec<nt, Tt, SPt>& t, const Vec<nv, Tv, SPv>& v, const Args&... args) {
                static_assert((begin + nv) <= nt, "Too many assignment parameters.");
                Assignment<begin + nv>::run
                        (detail::assignArray<begin, nv>(t, v), args...);
            }

            template <std::size_t nt, typename Tt, class SPt, typename First, typename... Args>
            static typename std::enable_if
            <!sizeof...(Args) && begin == 0>::type
            run(Vec<nt, Tt, SPt>& t, const First& first, const Args&... args) {
                detail::assignValue<nt>(t, first);
            }

            template <std::size_t nt, typename Tt, class SPt, typename First, typename... Args>
            static typename std::enable_if
            <((sizeof...(Args) > 0) || (begin > 0))>::type
            run(Vec<nt, Tt, SPt>& t, const First& first, const Args&... args) {
                static_assert((begin + 1) <= nt, "Too many assignment parameters.");
                t[begin] = first;
                Assignment<begin + 1>::run(t, args...);
            }
        };

    } // namespace nassign

    template <std::size_t n, typename T, class SP, typename... Args>
    Vec<n, T, SP>& assign(Vec<n, T, SP>& v, const Args&... args)
    { nassign::Assignment<0>::run(v, args...); return v; }


    // ----------------------------------------------------------------------------
    // General funcs
    template <std::size_t n, typename T, class SP>
    void print(const Vec<n, T, SP>& v) {
        for (std::size_t i = 0; i < n; ++i) {
            std::cout << v[i];
            if (i < n - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

} // namespace oglml

#endif // OGLML_VEC_FUNC_HPP
