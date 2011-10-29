#ifndef OGLML_HELPERS_CONSTEXPR_HPP
#define OGLML_HELPERS_CONSTEXPR_HPP

namespace oglml {

    namespace constexp {
        template <typename T>
        constexpr T max(T a, T b)
        { return a > b ? a : b; }
    }
}

#endif // OGLML_HELPERS_CONSTEXPR_HPP
