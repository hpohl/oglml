#ifndef OGLML_MAT_INFO_HPP
#define OGLML_MAT_INFO_HPP

#include <cstddef>

#include <type_traits>

#include <oglml/helpers/compilerinfo.hpp>
#include <oglml/helpers/errors.hpp>
#include <oglml/matfwd.hpp>

namespace oglml {
    namespace mat {
        namespace detail {

            struct MatIdentifier { };

        } // namespace detail
    } // namespace mat

    template <std::size_t tcols, std::size_t trows, typename tT>
    struct BaseMat : public mat::detail::MatIdentifier {
        oglml_constexpr static std::size_t cols = tcols;
        oglml_constexpr static std::size_t rows = trows;

        typedef tT T;
    };

    namespace mat {

        template <typename T>
        struct IsMat {
            oglml_constexpr static bool result =
                    std::is_base_of<detail::MatIdentifier, T>::value;
        };

        template <typename T1, typename T2>
        struct OneIsMat {
            oglml_constexpr static bool result = IsMat<T1>::result || IsMat<T2>::result;
        };

        namespace detail {

            template <class Op,
                      std::size_t cols1, std::size_t rows1, typename T1, class SP1,
                      std::size_t cols2, std::size_t rows2, typename T2, class SP2>
            struct CreateMat : private SizeCheck<cols1, cols2, rows1, rows2> {
                oglml_constexpr static std::size_t cols = cols1;
                oglml_constexpr static std::size_t rows = rows1;

                typedef decltype(Op::run(std::declval<T1>(), std::declval<T2>())) T;

                typedef Mat<cols, rows, T> Result;
            };

        } // namespace detail

    } // namespace mat

} // namespace oglml

#endif // OGLML_MAT_INFO_HPP
