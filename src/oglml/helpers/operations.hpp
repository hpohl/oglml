#ifndef OGLML_HELPERS_OPERATIONS_HPP
#define OGLML_HELPERS_OPERATIONS_HPP

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

}

#endif // OGLML_HELPERS_OPERATIONS_HPP
