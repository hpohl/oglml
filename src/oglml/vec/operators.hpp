#ifndef OGLML_VEC_OPERATORS_HPP
#define OGLML_VEC_OPERATORS_HPP

#include <type_traits>

#include <oglml/vec/expression.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/autoreturn.hpp>
#include <oglml/helpers/traits.hpp>

namespace oglml {
    // Helper to prevent the compiler to be lazy
#define OGLML_VEC_NO_EXPRESSION(T, R) \
    typename detail::GetFirst<R, typename std::enable_if \
    <!std::is_base_of<vec::BaseExpression, T>::value>::type>::Result

    // Global operators
    // Expression OP Expression
    template <class I1, class I2>
    const typename CreateExpressionVec<Plus, I1, I2>::Result operator+
    (const vec::Expression<I1>& lhs, const vec::Expression<I2>& rhs) {
        typename CreateExpressionVec<Plus, I1, I2>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class I1, class I2>
    const typename CreateExpressionVec<Minus, I1, I2>::Result operator-
    (const vec::Expression<I1>& lhs, const vec::Expression<I2>& rhs) {
        typename CreateExpressionVec<Minus, I1, I2>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class I1, class I2>
    const typename CreateExpressionVec<Multiplies, I1, I2>::Result operator*
    (const vec::Expression<I1>& lhs, const vec::Expression<I2>& rhs) {
        typename CreateExpressionVec<Multiplies, I1, I2>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class I1, class I2>
    const typename CreateExpressionVec<Divides, I1, I2>::Result operator/
    (const vec::Expression<I1>& lhs, const vec::Expression<I2>& rhs) {
        typename CreateExpressionVec<Divides, I1, I2>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    // Expression OP value
    /*template <class I, typename T>
    const typename detail::GetFirst<typename CreateLhsExpressionVec<Plus, I, T>::Result,
    typename std::enable_if<std::is_base_of<T, vec::BaseExpression>::value>::type>::Result operator+
    (const vec::Expression<I>& lhs, const T& rhs) {
        typename CreateLhsExpressionVec<Plus, I, T>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }*/

    template <class I, typename T>
    const typename detail::GetSecond
    <typename std::enable_if<!std::is_base_of<vec::BaseExpression, T>::value>::type,
    typename CreateLhsExpressionVec<Plus, I, T>::Result>::Result operator+
    (const vec::Expression<I>& lhs, const T& rhs) {
        typename CreateLhsExpressionVec<Plus, I, T>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class I, typename T>
    const typename detail::GetSecond
    <typename std::enable_if<!std::is_base_of<vec::BaseExpression, T>::value>::type,
    typename CreateLhsExpressionVec<Minus, I, T>::Result>::Result operator-
    (const vec::Expression<I>& lhs, const T& rhs) {
        typename CreateLhsExpressionVec<Minus, I, T>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class I, typename T>
    const typename detail::GetSecond
    <typename std::enable_if<!std::is_base_of<vec::BaseExpression, T>::value>::type,
    typename CreateLhsExpressionVec<Multiplies, I, T>::Result>::Result operator*
    (const vec::Expression<I>& lhs, const T& rhs) {
        typename CreateLhsExpressionVec<Multiplies, I, T>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

    template <class I, typename T>
    const typename detail::GetSecond
    <typename std::enable_if<!std::is_base_of<vec::BaseExpression, T>::value>::type,
    typename CreateLhsExpressionVec<Divides, I, T>::Result>::Result operator/
    (const vec::Expression<I>& lhs, const T& rhs) {
        typename CreateLhsExpressionVec<Divides, I, T>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

} // namespace oglml

#endif // OGLML_VEC_OPERATORS_HPP
