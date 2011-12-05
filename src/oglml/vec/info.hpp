#ifndef OGLML_VEC_INFO_HPP
#define OGLML_VEC_INFO_HPP

#include <cstddef>

#include <oglml/helpers/errors.hpp>
#include <oglml/helpers/select.hpp>
#include <oglml/helpers/traits.hpp>
#include <oglml/helpers/compilerinfo.hpp>
#include <oglml/vecfwd.hpp>

namespace oglml {

    namespace vec {
        namespace detail {

            struct VecIdentifier { };

        } // namespace detail
    } // namespace vec

    template <std::size_t nt, typename Tt>
    struct BaseVec : public vec::detail::VecIdentifier {
        // Conditions
        static_assert(nt > 1, "An OGLML Vec must have at least 2 dimensions.");

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
        struct OneIsVec {
            oglml_constexpr static bool result = IsVec<T1>::result || IsVec<T2>::result;
        };

        template <typename T1, typename T2>
        struct BothAreVecs {
            oglml_constexpr static bool result = IsVec<T1>::result && IsVec<T2>::result;
        };

        template <typename T1, typename T2>
        struct SameDim {
            oglml_constexpr static bool conditions = BothAreVecs<T1, T2>::result;
            oglml_constexpr static bool result = conditions && (T1::n == T2::n);
        };

        template <typename T1, typename T2>
        struct OneIsVecOrSameDim {
            oglml_constexpr static bool result =
                    Select<BothAreVecs<T1, T2>::result,
            SameDim<T1, T2>, OneIsVec<T1, T2> >::Result::result;
        };

        namespace detail {

            template <class Op,
                      std::size_t nlhs, typename Tlhs, class SPlhs,
                      std::size_t nrhs, typename Trhs, class SPrhs>
            struct CreateVec : private DimCheck<nlhs, nrhs> {
                oglml_constexpr static std::size_t n = nlhs;
                typedef decltype(Op::run(std::declval<Tlhs>(), std::declval<Trhs>())) T;
                typedef vec::DefaultStorage SP;

                typedef Vec<n, T, SP> Result;
            };

            template <class Op,
                      std::size_t nlhs, typename Tlhs, class SPlhs,
                      typename Trhs>
            struct CreateVecFromLhs {
                oglml_constexpr static std::size_t n = nlhs;
                typedef decltype(Op::run(std::declval<Tlhs>(), std::declval<Trhs>())) T;
                typedef vec::DefaultStorage SP;

                typedef Vec<n, T, SP> Result;
            };

            template <class Op,
                      typename Tlhs,
                      std::size_t nrhs, typename Trhs, class SPrhs>
            struct CreateVecFromRhs {
                oglml_constexpr static std::size_t n = nrhs;
                typedef decltype(Op::run(std::declval<Tlhs>(), std::declval<Trhs>())) T;
                typedef vec::DefaultStorage SP;

                typedef Vec<n, T, SP> Result;
            };


        } // namespace detail
    } // namespace vec
} // namespace oglml

#endif // OGLML_VEC_INFO_HPP
