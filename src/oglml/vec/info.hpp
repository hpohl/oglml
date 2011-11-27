#ifndef OGLML_VEC_INFO_HPP
#define OGLML_VEC_INFO_HPP

#include <cstddef>

#include <oglml/helpers/select.hpp>
#include <oglml/helpers/traits.hpp>
#include <oglml/helpers/compilerinfo.hpp>

namespace oglml {

    namespace vec {
        namespace detail {

            struct VecIdentifier { };

        } // namespace detail
    } // namespace vec

    template <std::size_t nt, typename Tt>
    struct BaseVec : public vec::detail::VecIdentifier {
        oglml_constexpr static std::size_t n = nt;
        typedef Tt T;
    };

    namespace vec {

        // Helpers to get vec information
        template <typename T>
        struct IsVec {
            oglml_constexpr static bool result =
                    std::is_base_of<detail::VecIdentifier, T>::value;
        };

        template <typename T1, typename T2>
        struct BothAreVecs {
            oglml_constexpr static bool result = IsVec<T1>::result && IsVec<T2>::result;
        };

        template <typename T1, typename T2>
        struct OneIsVec {
            oglml_constexpr static bool result = IsVec<T1>::result || IsVec<T2>::result;
        };

        template <typename T1, typename T2>
        struct SameDim {
            oglml_constexpr static bool conditions = BothAreVecs<T1, T2>::result;
            oglml_constexpr static bool result = conditions && (T1::n == T2::n);
        };

        template <typename T1, typename T2>
        struct OneIsVecOrSameDim {
            oglml_constexpr static bool result =
                    Select<OneIsVec<T1, T2>::result,
            SameDim<T1, T2>, oglml::detail::False>::Result::result;
        };

        namespace detail {

        } // namespace detail
    } // namespace vec
} // namespace oglml

#endif // OGLML_VEC_INFO_HPP
