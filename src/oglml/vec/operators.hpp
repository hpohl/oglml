#ifndef OGLML_VEC_OPERATORS_HPP
#define OGLML_VEC_OPERATORS_HPP

#include <type_traits>

#include <oglml/helpers/errors.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/autoreturn.hpp>
#include <oglml/vec/funcs.hpp>
#include <oglml/vec/info.hpp>

namespace oglml {

#define OGLML_DEFINE_VEC_OPERATOR(_NAME_, _EX_) \
    template <typename Tlhs, typename Trhs> \
    auto operator _NAME_(const Tlhs& lhs, const Trhs& rhs) \
    -> typename detail::GetSecond< \
    typename std::enable_if<vec::OneIsVec<Tlhs, Trhs>::result>::type, \
    decltype(detail::operate<_EX_>(lhs, rhs)) \
    >::Result \
    { return detail::operate<_EX_>(lhs, rhs); }

    OGLML_DEFINE_VEC_OPERATOR(+, Plus)
    OGLML_DEFINE_VEC_OPERATOR(-, Minus)
    OGLML_DEFINE_VEC_OPERATOR(*, Multiplies)
    OGLML_DEFINE_VEC_OPERATOR(/, Divides)
    OGLML_DEFINE_VEC_OPERATOR(%, Modulus)

    // -----------------------------------------------------------------------
    // Comparison operator
    template <std::size_t nlhs, typename Tlhs, class SPlhs,
              std::size_t nrhs, typename Trhs, class SPrhs>
    bool operator==(Vec<nlhs, Tlhs, SPlhs>& lhs, const Vec<nrhs, Trhs, SPrhs>& rhs)
    { return compare(lhs, rhs); }

    template <std::size_t nlhs, typename Tlhs, class SPlhs,
              std::size_t nrhs, typename Trhs, class SPrhs>
    bool operator!=(Vec<nlhs, Tlhs, SPlhs>& lhs, const Vec<nrhs, Trhs, SPrhs>& rhs)
    { return !compare(lhs, rhs); }

} // namespace oglml

#endif // OGLML_VEC_OPERATORS_HPP
