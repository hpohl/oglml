#ifndef OGLML_FUNCS_HPP
#define OGLML_FUNCS_HPP

#include <oglml/helpers/autoreturn.hpp>
#include <oglml/vec.hpp>

namespace oglml {

#define OGLML_GLSL_FUNC(_NAME_) \
    namespace glsl { \
        using oglml::_NAME_; \
    }

#define OGLML_FUNC_1ARG(_NAME_, _EX_) \
    template <typename T> \
    inline auto _NAME_(const T& v) \
    OGLML_AUTO_RETURN(_EX_::run(v)) \
    \
    template <std::size_t n, typename T, class SP> \
    inline auto _NAME_(const Vec<n, T, SP>& v) \
    OGLML_AUTO_RETURN(detail::operate<_EX_>(v)) \
    \
    OGLML_GLSL_FUNC(_NAME_)

#define OGLML_FUNC_2ARG(_NAME_, _EX_) \
    template <typename T1, typename T2> \
    inline auto _NAME_(const T1& x, const T2& y) \
    OGLML_AUTO_RETURN(_EX_::run(x, y)) \
    \
    template <std::size_t nlhs, typename Tlhs, class SPlhs, \
              std::size_t nrhs, typename Trhs, class SPrhs> \
    inline auto _NAME_(const Vec<nlhs, Tlhs, SPlhs>& lhs, const Vec<nrhs, Trhs, SPrhs>& rhs) \
    OGLML_AUTO_RETURN(detail::operate<_EX_>(lhs, rhs)) \
    \
    template <std::size_t n, typename T, class SP> \
    inline auto _NAME_(const Vec<n, T, SP>& lhs, float rhs) \
    OGLML_AUTO_RETURN(detail::operate<_EX_>(lhs, rhs)) \
    \
    template <std::size_t n, typename T, class SP> \
    inline auto _NAME_(float lhs, const Vec<n, T, SP>& rhs) \
    OGLML_AUTO_RETURN(detail::operate<_EX_>(lhs, rhs)) \
    \
    OGLML_GLSL_FUNC(_NAME_)

    // -------------------------------------------------------------- //
    // Angle and trigonometry functions                               //
    // -------------------------------------------------------------- //
    OGLML_FUNC_1ARG(radians, Radians)
    OGLML_FUNC_1ARG(degrees, Degrees)

    OGLML_FUNC_1ARG(sin, Sin)
    OGLML_FUNC_1ARG(cos, Cos)
    OGLML_FUNC_1ARG(tan, Tan)

    OGLML_FUNC_1ARG(asin, ASin)
    OGLML_FUNC_1ARG(acos, ACos)
    OGLML_FUNC_1ARG(atan, ATan)

    OGLML_FUNC_1ARG(sinh, SinH)
    OGLML_FUNC_1ARG(cosh, CosH)
    OGLML_FUNC_1ARG(tanh, TanH)

    OGLML_FUNC_1ARG(asinh, ASinH)
    OGLML_FUNC_1ARG(acosh, ACosH)
    OGLML_FUNC_1ARG(atanh, ATanH)

    // -------------------------------------------------------------- //
    // Exponential functions                                          //
    // -------------------------------------------------------------- //
    OGLML_FUNC_2ARG(pow, Pow)

    OGLML_FUNC_1ARG(exp, Exp)
    OGLML_FUNC_1ARG(log, Log)

    OGLML_FUNC_1ARG(exp2, Exp2)
    OGLML_FUNC_1ARG(log2, Log2)

    OGLML_FUNC_1ARG(sqrt, Sqrt)
    OGLML_FUNC_1ARG(inversesqrt, InverseSqrt)

    // -------------------------------------------------------------- //
    // Common functions                                               //
    // -------------------------------------------------------------- //
    OGLML_FUNC_1ARG(abs, Abs)
    OGLML_FUNC_1ARG(sign, Sign)
    OGLML_FUNC_1ARG(floor, Floor)
    OGLML_FUNC_1ARG(trunc, Trunc)
    OGLML_FUNC_1ARG(round, Round)
    //OGLML_FUNC_1ARG(roundEven, RoundEven) // TODO
    // ... TODO

    // -------------------------------------------------------------- //
    // Geometric functions                                            //
    // -------------------------------------------------------------- //

    // Length
    template <std::size_t n, typename T, class SP>
    float length(const Vec<n, T, SP>& v) {
        float ret = 0;
        for (std::size_t i = 0; i < n; ++i)
            ret += Pow::run(v[i], 2.0);
        return Sqrt::run(ret);
    }

    template <std::size_t n, class SP>
    double length(const Vec<n, double, SP> &v) {
        double ret = 0;
        for (std::size_t i = 0; i < n; ++i)
            ret += Pow::run(v[i], 2.0);
        return Sqrt::run(ret);
    }

    OGLML_GLSL_FUNC(length)


    // Distance
    template <std::size_t n1, typename T1, class SP1,
              std::size_t n2, typename T2, class SP2>
    float distance(const Vec<n1, T1, SP1>& v1, const Vec<n2, T2, SP2>& v2)
    { return length(v1 - v2); }

    template <std::size_t n1, class SP1, std::size_t n2, class SP2>
    double distance(const Vec<n1, double, SP1>& v1, const Vec<n2, double, SP2>& v2)
    { return length(v1 - v2); }

    OGLML_GLSL_FUNC(distance)


    // Dot
    template <std::size_t n, typename T1, class SP1,
                             typename T2, class SP2>
    float dot(const Vec<n, T1, SP1>& v1, const Vec<n, T2, SP2>& v2) {
        float ret = 0;
        for (std::size_t i = 0; i < n; ++i)
            ret += v1[i] * v2[i];
        return ret;
    }

    template <std::size_t n, class SP1, class SP2>
    double dot(const Vec<n, double, SP1>& v1, const Vec<n, double, SP2>& v2) {
        double ret = 0;
        for (std::size_t i = 0; i < n; ++i)
            ret += v1[i] * v2[i];
        return ret;
    }

    OGLML_GLSL_FUNC(dot)


    // Cross
    /*template <std::size_t n1, typename T1, class SP1,
              std::size_t n2, typename T2, class SP2>
    */ // TODO
}

#endif // OGLML_FUNCS_HPP
