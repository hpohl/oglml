#ifndef OGLML_HELPERS_ERRORS_HPP
#define OGLML_HELPERS_ERRORS_HPP

#include <cstddef>

#define OGLML_DIM_ERROR_MSG_ "Incompatible dimensions."

namespace oglml {
    template <std::size_t n1, std::size_t n2>
    constexpr void dimAssert()
    { static_assert(n1 == n2, OGLML_DIM_ERROR_MSG_); }

    template <std::size_t n1, std::size_t n2>
    struct DimCheck { static_assert(n1 == n2, OGLML_DIM_ERROR_MSG_); };

    template <std::size_t n1, std::size_t n2>
    struct DimProcessable { static_assert(n1 == n2 || n1 == 1 || n2 == 1, OGLML_DIM_ERROR_MSG_); };
}

#endif // OGLML_HELPERS_ERRORS_HPP
