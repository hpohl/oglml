#ifndef OGLML_MAT_INFO_HPP
#define OGLML_MAT_INFO_HPP

#include <cstddef>

#include <type_traits>

#include <oglml/helpers/compilerinfo.hpp>

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

    } // namespace mat

} // namespace oglml

#endif // OGLML_MAT_INFO_HPP
