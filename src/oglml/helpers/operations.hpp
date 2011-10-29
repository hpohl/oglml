#ifndef OGLML_HELPERS_OPERATIONS_HPP
#define OGLML_HELPERS_OPERATIONS_HPP

#include <cstddef>

namespace oglml {

    struct Plus {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs + rhs)
        { return lhs + rhs; }
    };

    struct Minus {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs - rhs)
        { return lhs - rhs; }
    };

    struct Multiplies {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs * rhs)
        { return lhs * rhs; }
    };

    struct Divides {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs / rhs)
        { return lhs / rhs; }
    };

    template <std::size_t begin, std::size_t n, typename T1, typename T2>
    inline T1& assignArray(T1& lhs, const T2& rhs) {
        for (std::size_t i = 0; i < n; ++i)
            lhs[begin + i] = rhs[i];
        return lhs;
    }

    template <std::size_t begin, std::size_t n, typename T1, typename T2>
    inline T1& assignValue(T1& lhs, const T2& rhs) {
        for (std::size_t i = 0; i < n; ++i)
            lhs[i] = rhs;
        return lhs;
    }

}

#endif // OGLML_HELPERS_OPERATIONS_HPP
