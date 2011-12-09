#ifndef OGLML_MAT_FUNCS
#define OGLML_MAT_FUNCS

#include <iostream>
#include <type_traits>

#include <oglml/helpers/constexpr.hpp>
#include <oglml/helpers/traits.hpp>
#include <oglml/vec/info.hpp>
#include <oglml/mat/info.hpp>
#include <oglml/matfwd.hpp>

namespace oglml {

    // Assignments
    namespace nmassign {

        template <std::size_t tcol, std::size_t trow>
        struct Assignment;

        template <std::size_t col, std::size_t row, bool end>
        struct NextAssignmentCall {

            template <typename First, typename... Args>
            static void run(First& first, const Args&... args)
            { Assignment<col, row>::run(first, args...); }

        };

        template <std::size_t col, std::size_t row>
        struct NextAssignmentCall<col, row, true> {

            template <typename First, typename... Args>
            static void run(First&, const Args&...)
            {  }

        };

        template <std::size_t col, std::size_t row, typename>
        struct AreZero {
            oglml_constexpr static std::size_t result = (!col && !row);
        };

        template <std::size_t tcol, std::size_t trow>
        struct Assignment {

            template <std::size_t cols, std::size_t rows, std::size_t needed>
            struct EnoughSpace {
                oglml_constexpr static bool result =
                        (cols * rows) >= (tcol * rows + trow + needed);

                oglml_constexpr_if_available static void check()
                { static_assert(result, "Too many assignment parameters."); }
            };

            template <std::size_t jump = 1, std::size_t colsm, std::size_t rowsm,
                      typename Tm, class SPm, typename... Args>
            static void nextAssignment(Mat<colsm, rowsm, Tm, SPm>& m, const Args&... args) {

                oglml_constexpr static std::size_t argCount = sizeof...(Args);
                oglml_constexpr static bool noArgs = !argCount;

                oglml_constexpr static std::size_t nextrow = trow + jump;
                oglml_constexpr static std::size_t nextcol = tcol + ((trow + jump) / rowsm);

                oglml_constexpr static bool rowOverflow = (nextrow >= rowsm);
                oglml_constexpr static bool colOverflow = (nextcol >= colsm);

                // General results
                static_assert((noArgs && colOverflow) || !noArgs,
                              "Not enough assignment parameters.");

                oglml_constexpr static std::size_t resultrow =
                        rowOverflow ? (nextrow - rowsm) : nextrow;

                oglml_constexpr static std::size_t resultcol =
                        rowOverflow ? nextcol : tcol;

                NextAssignmentCall<resultcol, resultrow, noArgs>::run(m, args...);
            };

            // One assignment does everythings
            template <std::size_t n, typename T, class SP, typename First>
            static typename std::enable_if<
            !mat::IsMat<First>::result && tcol == 0 && trow == 0>::type
            run(Mat<n, n, T, SP>& m, const First& first) {
                for (std::size_t i = 0; i < n; ++i) {
                    for (std::size_t j = 0; j < n; ++j) {
                        if (i == j)
                            m[i][j] = first;
                        else
                            m[i][j] = 0.0;
                    }
                }
            }

            template <std::size_t colsm, std::size_t rowsm, typename Tm, class SPm,
                      std::size_t colsv, std::size_t rowsv, typename Tv, class SPv>
            static typename std::enable_if<AreZero<tcol, trow, Tm>::result>::type
            run(Mat<colsm, rowsm, Tm, SPm>& m, const Mat<colsv, rowsv, Tv, SPv>& v) {
                for (std::size_t row = 0; row < rowsm; ++row) {
                    for (std::size_t col = 0; col < colsm; ++col) {
                        if ((row >= rowsv) || (col >= colsv)) {
                            if (row == col)
                                m[col][row] = 1.0;
                            else
                                m[col][row] = 0.0;
                        } else
                            m[col][row] = v[col][row];
                    }
                }
            }


            // More than one argument
            template <std::size_t colsm, std::size_t rowsm, typename Tm, class SPm,
                      typename First, typename... Args>
            static typename std::enable_if<!vec::IsVec<First>::result>::type
            run(Mat<colsm, rowsm, Tm, SPm> &m, const First& first, const Args&... args) {
                m[tcol][trow] = first;
                nextAssignment(m, args...);
            }

            template <std::size_t colsm, std::size_t rowsm, typename Tm, class SPm,
                      std::size_t nv, typename Tv, class SPv, typename... Args>
            static void run(Mat<colsm, rowsm, Tm, SPm> &m,
                            const Vec<nv, Tv, SPv>& v, const Args&... args) {
                //EnoughSpace<colsm, rowsm, nv>::check();

                for (std::size_t i = 0; i < nv; ++i)
                    m[tcol + ((trow + i) / rowsm)][(trow + i) % rowsm] = v[i];

                nextAssignment<nv>(m, args...);
            }
        };

    } // namespace nmassign

    template <std::size_t cols, std::size_t rows, typename T, class SP,
              typename First, typename... Args>
    Mat<cols, rows, T, SP>& assign(Mat<cols, rows, T, SP>& m,
                                   const First& first, const Args&... args)
    { nmassign::Assignment<0, 0>::run(m, first, args...); return m; }


    // ----------------------------------------------------------------------------
    // General funcs

    // Print
    template <std::size_t cols, std::size_t rows, typename T, class SP>
    void print(const Mat<cols, rows, T, SP>& m) {
        for (std::size_t i = 0; i < cols; ++i) {
            print(m[i]);
        }
        std::cout << std::endl;
    }


} // namespace oglml

#endif // OGLML_MAT_FUNCS
