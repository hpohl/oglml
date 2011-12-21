#ifndef OGLML_OPERATORS_HPP
#define OGLML_OPERATORS_HPP

/*! \file operators.hpp
  \brief Contains all operators between different oglml types
*/

#include <oglml/funcs.hpp>

namespace oglml {

#ifndef OGLML_DOXYGEN_SKIP

    // Matrix multiplication
    namespace detail {

        // Column vector
        template <std::size_t colsm, std::size_t rowsm, typename Tm,
                  std::size_t n, typename Tv>
        struct CreateMatrixVecMultReturnT {

            static_assert(rowsm == n, "Invalid multiplication.");

            oglml_constexpr static std::size_t cols = n;
            oglml_constexpr static std::size_t rows = rowsm;

            typedef Mat<cols, rows, decltype
                    (Multiplies::run(std::declval<Tm>(), std::declval<Tv>()))> Result;

        };

        // Row vector
        template <std::size_t n, typename Tv,
                  std::size_t colsm, std::size_t rowsm, typename Tm>
        struct CreateVecMatrixMultReturnT {

            static_assert(n == colsm, "Invalid multiplication.");

            oglml_constexpr static std::size_t cols = colsm;
            oglml_constexpr static std::size_t rows = n;

            typedef Mat<cols, rows, decltype
                    (Multiplies::run(std::declval<Tm>(), std::declval<Tv>()))> Result;

        };

    } // namespace detail

#endif // OGLML_DOXYGEN_SKIP


    template <std::size_t cols, std::size_t rows, typename Tm, class SPm,
              std::size_t n, typename Tv, class SPv>
    inline typename detail::CreateMatrixVecMultReturnT<cols, rows, Tm, n, Tv>::Result
    operator*(const Mat<cols, rows, Tm, SPm>& m, const Vec<n, Tv, SPv>& v) {

        typename detail::CreateMatrixVecMultReturnT
            <cols, rows, Tm, n, Tv>::Result ret;

        for (std::size_t col = 0; col < ret.cols; ++col) {
            for (std::size_t row = 0; row < ret.rows; ++row) {

                ret[col][row] = 0.0;
                for (std::size_t i = 0; i < m.cols; ++i) {
                    ret[col][row] += m[i][row] * v[col];
                }

            }
        }
        return ret;
    }

    template <std::size_t n, typename Tv, class SPv,
              std::size_t cols, std::size_t rows, typename Tm, class SPm>
    inline typename detail::CreateVecMatrixMultReturnT<n, Tv, cols, rows, Tm>::Result
    operator*(const Vec<n, Tv, SPv>& v, const Mat<cols, rows, Tm, SPm>& m) {

        typename detail::CreateVecMatrixMultReturnT
            <n, Tv, cols, rows, Tm>::Result ret;

        for (std::size_t col = 0; col < ret.cols; ++col) {
            for (std::size_t row = 0; row < ret.rows; ++row) {

                ret[col][row] = 0.0;
                for (std::size_t i = 0; i < m.rows; ++i) {
                    ret[col][row] += m[col][i] * v[row];
                }

            }
        }
        return ret;
    }

} // namespace oglml

#endif // OGLML_OPERATORS_HPP
