#ifndef OGLML_VEC_OPERATORS_HPP
#define OGLML_VEC_OPERATORS_HPP

#include <oglml/vec/expression.hpp>
#include <oglml/helpers/operations.hpp>

namespace oglml {

    // Global operators
    template <class I1, class I2>
    typename CreateExpressionVec<Plus, I1, I2>::Result operator+
    (const vec::Expression<I1>& lhs, const vec::Expression<I2>& rhs) {
        typename CreateExpressionVec<Plus, I1, I2>::Result vex;
        vex.data.init(lhs, rhs);
        return vex;
    }

} // namespace oglml

#endif // OGLML_VEC_OPERATORS_HPP
