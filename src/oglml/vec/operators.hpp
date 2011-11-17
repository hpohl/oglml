#ifndef OGLML_VEC_OPERATORS_HPP
#define OGLML_VEC_OPERATORS_HPP

#include <type_traits>

#include <oglml/vec/info.hpp>
#include <oglml/vec/expression.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/autoreturn.hpp>
#include <oglml/helpers/traits.hpp>

namespace oglml {
    // Global operators
    // Expression OP Expression
    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs, Trhs>::result>::type,
    typename CreateExpressionVec<Plus, Tlhs, Trhs>::Result>::Result
    operator+(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateExpressionVec<Plus, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs, Trhs>::result>::type,
    typename CreateExpressionVec<Minus, Tlhs, Trhs>::Result>::Result
    operator-(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateExpressionVec<Minus, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs, Trhs>::result>::type,
    typename CreateExpressionVec<Multiplies, Tlhs, Trhs>::Result>::Result
    operator*(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateExpressionVec<Multiplies, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs, Trhs>::result>::type,
    typename CreateExpressionVec<Divides, Tlhs, Trhs>::Result>::Result
    operator/(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateExpressionVec<Divides, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs, Trhs>::result>::type,
    typename CreateExpressionVec<Modulus, Tlhs, Trhs>::Result>::Result
    operator%(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateExpressionVec<Modulus, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    // Expression OP value
    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs>::result &&
    !vec::detail::IsMVec<Trhs>::result>::type,
    typename CreateLhsExpressionVec<Plus, Tlhs, Trhs>::Result>::Result
    operator+(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateLhsExpressionVec<Plus, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs>::result &&
    !vec::detail::IsMVec<Trhs>::result>::type,
    typename CreateLhsExpressionVec<Minus, Tlhs, Trhs>::Result>::Result
    operator-(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateLhsExpressionVec<Minus, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs>::result &&
    !vec::detail::IsMVec<Trhs>::result>::type,
    typename CreateLhsExpressionVec<Multiplies, Tlhs, Trhs>::Result>::Result
    operator*(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateLhsExpressionVec<Multiplies, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs>::result &&
    !vec::detail::IsMVec<Trhs>::result>::type,
    typename CreateLhsExpressionVec<Divides, Tlhs, Trhs>::Result>::Result
    operator/(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateLhsExpressionVec<Divides, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Tlhs>::result &&
    !vec::detail::IsMVec<Trhs>::result>::type,
    typename CreateLhsExpressionVec<Modulus, Tlhs, Trhs>::Result>::Result
    operator%(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateLhsExpressionVec<Modulus, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    // Value OP expression
    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Trhs>::result &&
    !vec::detail::IsMVec<Tlhs>::result>::type,
    typename CreateRhsExpressionVec<Plus, Tlhs, Trhs>::Result>::Result
    operator+(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateRhsExpressionVec<Plus, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Trhs>::result &&
    !vec::detail::IsMVec<Tlhs>::result>::type,
    typename CreateRhsExpressionVec<Minus, Tlhs, Trhs>::Result>::Result
    operator-(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateRhsExpressionVec<Minus, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Trhs>::result &&
    !vec::detail::IsMVec<Tlhs>::result>::type,
    typename CreateRhsExpressionVec<Multiplies, Tlhs, Trhs>::Result>::Result
    operator*(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateRhsExpressionVec<Multiplies, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Trhs>::result &&
    !vec::detail::IsMVec<Tlhs>::result>::type,
    typename CreateRhsExpressionVec<Divides, Tlhs, Trhs>::Result>::Result
    operator/(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateRhsExpressionVec<Divides, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class Tlhs, class Trhs>
    const typename detail::GetSecond
    <typename std::enable_if<vec::detail::IsMVec<Trhs>::result &&
    !vec::detail::IsMVec<Tlhs>::result>::type,
    typename CreateRhsExpressionVec<Modulus, Tlhs, Trhs>::Result>::Result
    operator%(const Tlhs& lhs, const Trhs& rhs) {
        typename CreateRhsExpressionVec<Modulus, Tlhs, Trhs>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

} // namespace oglml

#endif // OGLML_VEC_OPERATORS_HPP
