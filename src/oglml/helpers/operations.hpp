#ifndef OGLML_HELPERS_OPERATIONS_HPP
#define OGLML_HELPERS_OPERATIONS_HPP

#include <iostream>

#include <cmath>
#include <cstddef>

#include <oglml/helpers/autoreturn.hpp>
#include <oglml/helpers/constants.hpp>

namespace oglml {
#define OGLML_DEFINE_MATH_OPERATION(_NAME_, _EX_) \
    struct _NAME_ { \
        template <typename T1, typename T2> \
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs _EX_ rhs) \
        { return lhs _EX_ rhs; } \
    \
        template <typename T1, typename T2> \
        static T1& runassign(T1& lhs, const T2& rhs) \
        { return lhs _EX_##= rhs; } \
    };

    // Mathematical ops
    OGLML_DEFINE_MATH_OPERATION(Plus, +)
    OGLML_DEFINE_MATH_OPERATION(Minus, -)
    OGLML_DEFINE_MATH_OPERATION(Multiplies, *)
    OGLML_DEFINE_MATH_OPERATION(Divides, /)
    OGLML_DEFINE_MATH_OPERATION(Modulus, %)

    struct Pow {
        template <typename T1, typename T2>
        static auto run(const T1& lhs, const T2& rhs) -> decltype(std::pow(lhs, rhs))
        { return std::pow(lhs, rhs); }
    };

#define OGLML_DEFINE_EQUAL_OPERATION(_NAME_, _EX_) \
    struct _NAME_ { \
        template <typename T1, typename T2> \
        static auto run(const T1& lhs, const T2& rhs) -> decltype(lhs _EX_ rhs) \
        { return lhs _EX_ rhs; } \
    };

    OGLML_DEFINE_EQUAL_OPERATION(LessThan, <)
    OGLML_DEFINE_EQUAL_OPERATION(LessThanEqual, <=)
    OGLML_DEFINE_EQUAL_OPERATION(GreaterThan, <)
    OGLML_DEFINE_EQUAL_OPERATION(GreaterThanEqual, <=)
    OGLML_DEFINE_EQUAL_OPERATION(Equal, ==)
    OGLML_DEFINE_EQUAL_OPERATION(NotEqual, !=)

    // Single argument ops
#define OGLML_DEFINE_OPERATION(_NAME_, _EX_) \
    struct _NAME_ { \
        template <typename T> \
        static auto run(const T& v) -> decltype(_EX_) \
        { return (_EX_); } \
    };

    OGLML_DEFINE_OPERATION(Promotion, +v)
    OGLML_DEFINE_OPERATION(Negation, -v)

    OGLML_DEFINE_OPERATION(Radians, (pi / 180.0) * v)
    OGLML_DEFINE_OPERATION(Degrees, (180.0 / pi) * v)

    OGLML_DEFINE_OPERATION(Sin, std::sin(v))
    OGLML_DEFINE_OPERATION(Cos, std::cos(v))
    OGLML_DEFINE_OPERATION(Tan, std::tan(v))

    OGLML_DEFINE_OPERATION(ASin, std::asin(v))
    OGLML_DEFINE_OPERATION(ACos, std::acos(v))
    OGLML_DEFINE_OPERATION(ATan, std::atan(v))

    OGLML_DEFINE_OPERATION(SinH, std::sinh(v))
    OGLML_DEFINE_OPERATION(CosH, std::cosh(v))
    OGLML_DEFINE_OPERATION(TanH, std::tanh(v))

    OGLML_DEFINE_OPERATION(ASinH, std::asinh(v))
    OGLML_DEFINE_OPERATION(ACosH, std::acosh(v))
    OGLML_DEFINE_OPERATION(ATanH, std::atanh(v))

    OGLML_DEFINE_OPERATION(Exp, std::exp(v))
    OGLML_DEFINE_OPERATION(Exp2, std::exp2(v))
    OGLML_DEFINE_OPERATION(Log, std::log(v))
    OGLML_DEFINE_OPERATION(Log2, std::log2(v))
    OGLML_DEFINE_OPERATION(Sqrt, std::sqrt(v))
    OGLML_DEFINE_OPERATION(InverseSqrt, 1.0 / std::sqrt(v))

    OGLML_DEFINE_OPERATION(Abs, std::abs(v))
    OGLML_DEFINE_OPERATION(Sign, v > 0.0 ? 1.0 : (v == 0.0 ? 0.0 : -1.0))
    OGLML_DEFINE_OPERATION(Floor, std::floor(v))
    OGLML_DEFINE_OPERATION(Trunc, std::trunc(v))
    OGLML_DEFINE_OPERATION(Round, std::round(v))
    OGLML_DEFINE_OPERATION(Ceil, std::ceil(v))
    OGLML_DEFINE_OPERATION(Fract, v - std::floor(v))


    // Other ops
    struct Min {
        template <typename T1, typename T2>
        static auto run(const T1& v1, const T2& v2)
        OGLML_AUTO_RETURN(std::min(v1, v2))
    };

    struct Max {
        template <typename T1, typename T2>
        static auto run(const T1& v1, const T2& v2)
        OGLML_AUTO_RETURN(std::max(v1, v2))
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
        inline T1& opassignValue(T1& lhs, const T2& rhs) {
            for (std::size_t i = 0; i < n; ++i)
                Op::runassign(lhs[begin + i], rhs);
            return lhs;
        }

        // ---------------
        template <class Op, class Ret, typename T1, typename T2>
        inline Ret execOp(const T1& lhs, const T2& rhs) {
            Ret r;
            for (std::size_t i = 0; i < Ret::n; ++i)
                r[i] = Op::run(lhs[i], rhs[i]);
            return r;
        }

    } // namespace detail

} // namespace oglml

#endif // OGLML_HELPERS_OPERATIONS_HPP
