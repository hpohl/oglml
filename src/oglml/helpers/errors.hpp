#ifndef OGLML_HELPERS_ERRORS_HPP
#define OGLML_HELPERS_ERRORS_HPP

#include <cstddef>

#include <oglml/helpers/compilerinfo.hpp>

#define OGLML_DIM_ERROR_MSG_ "Incompatible dimensions."

#define OGLML_ALIGNED_ERROR_MSG_ "Memory is not aligned. " \
    "Access it manually by using the index operators."

#define OGLML_INDEX_DUPLICATES_ERROR_MSG "Indices contain duplicates."

namespace oglml {
    template <std::size_t n1, std::size_t n2>
    void dimAssert()
    { static_assert(n1 == n2, OGLML_DIM_ERROR_MSG_); }

    template <std::size_t n1, std::size_t n2>
    struct DimCheck { static_assert(n1 == n2, OGLML_DIM_ERROR_MSG_); };

    template <std::size_t n1, std::size_t n2>
    struct DimProcessable { static_assert(n1 == n2 || n1 == 1 || n2 == 1,
                                          OGLML_DIM_ERROR_MSG_); };
}

#endif // OGLML_HELPERS_ERRORS_HPP
