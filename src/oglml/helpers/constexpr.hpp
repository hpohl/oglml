#ifndef OGLML_HELPERS_CONSTEXPR_HPP
#define OGLML_HELPERS_CONSTEXPR_HPP

#include <oglml/helpers/compilerinfo.hpp>

namespace oglml {

    namespace cnst {

        template <typename T>
        oglml_constexpr T max(T a, T b)
        { return a > b ? a : b; }

        template <typename T>
        oglml_constexpr T min(T a, T b)
        { return a < b ? a : b; }

        template <typename T>
        oglml_constexpr_if_available bool greater(T a, T b)
        { return a > b; }

    } // namespace cnst

} // namespace oglml

#endif // OGLML_HELPERS_CONSTEXPR_HPP
