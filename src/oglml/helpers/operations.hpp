#ifndef OGLML_HELPERS_OPERATIONS_HPP
#define OGLML_HELPERS_OPERATIONS_HPP

#include <iostream>

#include <cmath>
#include <cstddef>

#include <oglml/helpers/constants.hpp>

namespace oglml {

    // Mathematical ops
    struct Plus {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs + rhs)
        { return lhs + rhs; }

        template <typename T1, typename T2>
        static T1& runassign(T1& lhs, const T2& rhs)
        { return lhs += rhs; }
    };

    struct Minus {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs - rhs)
        { return lhs - rhs; }

        template <typename T1, typename T2>
        static T1& runassign(T1& lhs, const T2& rhs)
        { return lhs -= rhs; }
    };

    struct Multiplies {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs * rhs)
        { return lhs * rhs; }

        template <typename T1, typename T2>
        static T1& runassign(T1& lhs, const T2& rhs)
        { return lhs *= rhs; }
    };

    struct Divides {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs / rhs)
        { return lhs / rhs; }

        template <typename T1, typename T2>
        static T1& runassign(T1& lhs, const T2& rhs)
        { return lhs /= rhs; }
    };

    struct Modulus {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs / rhs)
        { return lhs % rhs; }

        template <typename T1, typename T2>
        static T1& runassign(T1& lhs, const T2& rhs)
        { return lhs %= rhs; }
    };

    struct Pow {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(std::pow(lhs, rhs))
        { return std::pow(lhs, rhs); }
    };

    // Single argument ops
    struct Radians {
        template <typename T>
        static auto run(const T& v) -> decltype((pi / 180.0) * v)
        { return (pi / 180.0) * v; }
    };

    struct Degrees {
        template <typename T>
        static auto run(const T& v) -> decltype((180.0 / pi) * v)
        { return (180.0 / pi) * v; }
    };

    struct Promotion {
        template <typename T>
        static auto run(const T& v) -> decltype(+v)
        { return +v; }
    };

    struct Negation {
        template <typename T>
        static auto run(const T& v) -> decltype(-v)
        { return -v; }
    };

    struct Exp {
        template <typename T>
        static auto run(const T& v) -> decltype(std::exp(v))
        { return std::exp(v); }
    };

    struct Exp2 {
        template <typename T>
        static auto run(const T& v) -> decltype(std::exp2(v))
        { return std::exp2(v); }
    };

    struct Log {
        template <typename T>
        static auto run(const T& v) -> decltype(std::log(v))
        { return std::log(v); }
    };

    struct Log2 {
        template <typename T>
        static auto run(const T& v) -> decltype(std::log2(v))
        { return std::log2(v); }
    };

    struct Sqrt {
        template <typename T>
        static auto run(const T& v) -> decltype(std::sqrt(v))
        { return std::sqrt(v); }
    };

    struct InverseSqrt {
        template <typename T>
        static auto run(const T& v) -> decltype(std::sqrt(v))
        { return static_cast<T>(1.0) / std::sqrt(v); }
    };

    namespace detail {

        // Helper funcs to prevent code to be blowed
        template <std::size_t n, typename T1, typename T2>
        inline T1& assignArray(T1& lhs, const T2& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                lhs[i] = rhs[i];
            return lhs;
        }

        template <std::size_t begin, std::size_t n, typename T1, typename T2>
        inline T1& assignArray(T1& lhs, const T2& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                lhs[begin + i] = rhs[i];
            return lhs;
        }

        template <class Op, std::size_t n, typename T1, typename T2>
        inline T1& opassignArray(T1& lhs, const T2& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                Op::runassign(lhs[i], rhs[i]);
            return lhs;
        }

        template <class Op, std::size_t begin, std::size_t n, typename T1, typename T2>
        inline T1& opassignArray(T1& lhs, const T2& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                Op::runassign(lhs[begin + i], rhs[i]);
            return lhs;
        }


        template <std::size_t n, typename T1, typename T2>
        inline T1& assignValue(T1& lhs, const T2& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                lhs[i] = rhs;
            return lhs;
        }

        template <std::size_t begin, std::size_t n, typename T1, typename T2>
        inline T1& assignValue(T1& lhs, const T2& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                lhs[begin + i] = rhs;
            return lhs;
        }

        template <class Op, std::size_t n, typename T1, typename T2>
        inline T1& opassignValue(T1& lhs, const T2& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                Op::runassign(lhs[i], rhs);
            return lhs;
        }

        template <class Op, std::size_t begin, std::size_t n, typename T1, typename T2>
        inline T1& assignValue(T1& lhs, const T2& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                Op::runassign(lhs[begin + i], rhs);
            return lhs;
        }

    } // namespace detail

} // namespace oglml

#endif // OGLML_HELPERS_OPERATIONS_HPP
