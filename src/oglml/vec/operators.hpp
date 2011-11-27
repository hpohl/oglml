#ifndef OGLML_VEC_OPERATORS_HPP
#define OGLML_VEC_OPERATORS_HPP

#include <type_traits>

#include <oglml/vecfwd.hpp>
#include <oglml/vec/info.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/autoreturn.hpp>
#include <oglml/helpers/traits.hpp>

namespace oglml {

    namespace detail {

        template <class Op, typename Tlhs, typename Trhs>
        struct CreateVecOpReturn : private std::enable_if<
                vec::OneIsVecOrSameDim<Tlhs, Trhs>::result, Empty>::type {

            oglml_constexpr static bool lhsVec = vec::IsVec<Tlhs>::result;
            oglml_constexpr static bool rhsVec = vec::IsVec<Trhs>::result;
            oglml_constexpr static bool bothVecs = lhsVec && rhsVec;

            typedef typename Select<lhsVec, Tlhs, Trhs>::Result VecType;

            oglml_constexpr static std::size_t n = VecType::n;

            typedef decltype(Op::run(std::declval<typename Tlhs::T>(),
                                     std::declval<typename Trhs::T>())) T;

            typedef Vec<n, T> Result;
        };

    } // namespace detail

    // Global operators
    template <typename Tlhs, typename Trhs>
    typename detail::CreateVecOpReturn<Plus, Tlhs, Trhs>::Result
    operator+(const Tlhs& lhs, const Trhs& rhs) {
    }

    // -----------------------------------------------------------------------
    // Comparison operator
    template <typename Tlhs, typename Trhs>
    bool operator==(const Tlhs& lhs, const Trhs& rhs)
    { return compare(lhs, rhs); }

    template <typename Tlhs, typename Trhs>
    bool operator!=(const Tlhs& lhs, const Trhs& rhs)
    { return !compare(lhs, rhs); }

} // namespace oglml

#endif // OGLML_VEC_OPERATORS_HPP
