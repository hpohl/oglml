#ifndef OGLML_MAT_OPERATORS_HPP
#define OGLML_MAT_OPERATORS_HPP

#include <oglml/helpers/autoreturn.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/mat/funcs.hpp>
#include <oglml/mat/info.hpp>
#include <oglml/vec/info.hpp>

namespace oglml {

#define OGLML_DEFINE_MAT_OPERATOR(_NAME_, _EX_) \
    template <typename Tlhs, typename Trhs> \
    inline auto operator _NAME_(const Tlhs& lhs, const Trhs& rhs) \
    -> typename detail::GetSecond< \
    typename std::enable_if<mat::OneIsMat<Tlhs, Trhs>::result>::type, \
    decltype(detail::operate<_EX_>(lhs, rhs)) \
    >::Result \
    { return detail::operate<_EX_>(lhs, rhs); }

    OGLML_DEFINE_MAT_OPERATOR(+, Plus)
    OGLML_DEFINE_MAT_OPERATOR(-, Minus)
    OGLML_DEFINE_MAT_OPERATOR(/, Divides)

    template <typename Tlhs, typename Trhs>
    inline auto operator*(const Tlhs& lhs, const Trhs& rhs)
    -> typename detail::GetSecond<

    typename std::enable_if<(mat::IsMat<Tlhs>::result ^ mat::IsMat<Trhs>::result) &&
    !vec::OneIsVec<Tlhs, Trhs>::result>::type,

    decltype(detail::operate<Multiplies>(lhs, rhs))
    >::Result
    { return detail::operate<Multiplies>(lhs, rhs); }


    // Matrix multiplication
    namespace detail {

        template <std::size_t cols1, std::size_t rows1, typename T1,
                  std::size_t cols2, std::size_t rows2, typename T2>
        struct CreateMatrixMultReturnT {

            static_assert(cols1 == rows2 && rows1 == cols2,
                          "Matrices cannot be multiplicated.");

            oglml_constexpr static std::size_t cols = cols2;
            oglml_constexpr static std::size_t rows = rows1;

            typedef Mat<cols, rows, decltype
                    (Multiplies::run(std::declval<T1>(), std::declval<T2>()))> Result;

        };

        /*template <std::size_t cols, std::size_t rows, typename Tm,
                  std::size_t n, typename Tv>
        struct CreateMatrixVecMultReturnT {

            static_assert(cols == n, "Multiplication illegal.");

            oglml_constexpr static std::size_t cols = n;
            oglml_constexpr static std::size_t rows = rows;

            typedef const Mat<cols, rows, decltype
                    (Multiplies::run(std::declval<Tm>(), std::declval<Tv>()))> Result;

        };*/

    } // namespace detail

    template <std::size_t cols1, std::size_t rows1, typename T1, class SP1,
              std::size_t cols2, std::size_t rows2, typename T2, class SP2>
    inline const typename detail::CreateMatrixMultReturnT
    <cols1, rows1, T1, cols2, rows2, T2>::Result
    operator*(const Mat<cols1, rows1, T1, SP1>& m1,
              const Mat<cols2, rows2, T2, SP2>& m2) {

        typename detail::CreateMatrixMultReturnT
            <cols1, rows1, T1, cols2, rows2, T2>::Result ret;

        for (std::size_t col = 0; col < ret.cols; ++col) {
            for (std::size_t row = 0; row < ret.rows; ++row) {

                ret[col][row] = 0.0;
                for (std::size_t i = 0; i < m1.cols; ++i)
                    ret[col][row] += m1[i][row] * m2[col][i];

            }
        }
        return ret;
    }

    /*template <std::size_t cols, std::size_t rows, typename Tm, class SPm,
              std::size_t n, typename Tv, class SPv>
    inline typename detail::CreateMatrixVecMultReturnT
    <cols, rows, Tm, SPm, n, Tv, SPv>::Result
    operator*(const Mat<cols, rows, Tm, SPm>& m, const Vec<)*/


} // namespace oglml

#endif // OGLML_MAT_OPERATORS_HPP
