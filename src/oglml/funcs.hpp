#ifndef OGLML_FUNCS_HPP
#define OGLML_FUNCS_HPP

/*! \file funcs.hpp
*/

#include <oglml/vec.hpp>
#include <oglml/mat.hpp>

namespace oglml {

#ifndef OGLML_DOXYGEN_SKIP

#define OGLML_GLSL_FUNC(_NAME_) \
    namespace glsl { \
        using oglml::_NAME_; \
    }

#define OGLML_FUNC_1ARG(_NAME_, _EX_) \
    template <typename T> \
    inline auto _NAME_(const T& v) \
    OGLML_AUTO_RETURN(_EX_::run<const T&>(v)) \
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

#endif // OGLML_DOXYGEN_SKIP

    // -------------------------------------------------------------- //
    // Angle and trigonometry functions                               //
    // -------------------------------------------------------------- //

    /*! \fn template <typename T> inline auto radians(const T& v)
      \brief Converts degrees to radians
    */

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
    OGLML_FUNC_1ARG(ceil, Ceil)
    OGLML_FUNC_1ARG(fract, Fract)

    template <typename T1, typename T2>
    inline auto mod(const T1& v1, const T2& v2)
    OGLML_AUTO_RETURN(v1 % v2)

    // modf // TODO

    OGLML_FUNC_2ARG(min, Min)
    OGLML_FUNC_2ARG(max, Max)

    /*template <std::size_t n, typename T, class SP, typename Tmin, typename Tmax>
    inline Vec<n, T> clamp*/


    // -------------------------------------------------------------- //
    // Geometric functions                                            //
    // -------------------------------------------------------------- //

    // Length
    //! \brief Returns the length of a vector
    template <std::size_t n, typename T, class SP>
    inline float length(const Vec<n, T, SP>& v) {
        float ret = 0;
        for (std::size_t i = 0; i < n; ++i)
            ret += Pow::run(v[i], 2.0);
        return Sqrt::run(ret);
    }

    //! \brief Returns the length of a vector
    template <std::size_t n, class SP>
    inline double length(const Vec<n, double, SP> &v) {
        double ret = 0;
        for (std::size_t i = 0; i < n; ++i)
            ret += Pow::run(v[i], 2.0);
        return Sqrt::run(ret);
    }

    OGLML_GLSL_FUNC(length)


    // Distance
    //! \brief Returns the distance between two vectors
    template <std::size_t n1, typename T1, class SP1,
              std::size_t n2, typename T2, class SP2>
    inline float distance(const Vec<n1, T1, SP1>& v1, const Vec<n2, T2, SP2>& v2)
    { return length(v1 - v2); }

    //! \brief Returns the distance between two vectors
    template <std::size_t n1, class SP1, std::size_t n2, class SP2>
    inline double distance(const Vec<n1, double, SP1>& v1,
                           const Vec<n2, double, SP2>& v2)
    { return length(v1 - v2); }

    OGLML_GLSL_FUNC(distance)


    // Dot
    //! Calculates the dot product
    template <std::size_t n, typename T1, class SP1,
                             typename T2, class SP2>
    inline float dot(const Vec<n, T1, SP1>& v1, const Vec<n, T2, SP2>& v2) {
        float ret = 0;
        for (std::size_t i = 0; i < n; ++i)
            ret += v1[i] * v2[i];
        return ret;
    }

    //! Calculates the dot product
    template <std::size_t n, class SP1, class SP2>
    inline double dot(const Vec<n, double, SP1>& v1, const Vec<n, double, SP2>& v2) {
        double ret = 0;
        for (std::size_t i = 0; i < n; ++i)
            ret += v1[i] * v2[i];
        return ret;
    }

    OGLML_GLSL_FUNC(dot)


    // Cross
#ifndef OGLML_DOXYGEN_SKIP
    namespace ncross {

        template <typename T1, typename T2>
        struct CreateReturnT {
            typedef decltype(std::declval<T1>() * std::declval<T2>() -
                             std::declval<T2>() * std::declval<T1>()) T;
            typedef Vec<3, T> Result;
        };

    } // namespace ncross
#endif // OGLML_DOXYGEN_SKIP

    //! Returns the cross product
    template <typename T1, class SP1, typename T2, class SP2>
    inline typename ncross::CreateReturnT<T1, T2>::Result
    cross(const Vec<3, T1, SP1>& v1, const Vec<3, T2, SP2>& v2) {
        return typename ncross::CreateReturnT<T1, T2>::Result(
        v1[1] * v2[2] - v2[1] * v1[2],
        v1[2] * v2[0] - v2[2] * v1[0],
        v1[0] * v2[1] - v2[0] * v1[1]);
    }

    OGLML_GLSL_FUNC(cross)


    // Normalize
    template <std::size_t n, typename T, class SP>
    inline auto normalize(const Vec<n, T, SP>& v)
    OGLML_AUTO_RETURN(Vec<n, T>(v / length(v)))

    OGLML_GLSL_FUNC(normalize)


    // Faceforward
    template <std::size_t n, typename Tn, class SPn,
    typename Ti, class SPi, std::size_t nnref, typename Tnnref, class SPnnref>
    inline Vec<nnref, Tnnref> faceforward(const Vec<n, Tn, SPn>& nv,
                                          const Vec<n, Ti, SPi>& i,
                                          const Vec<nnref, Tnnref, SPnnref>& nref)
    { return (dot(nref, i) < 0.0 ? nv : -nv); }

    OGLML_GLSL_FUNC(faceforward)


    // Reflect
    template <std::size_t ni, typename Ti, class SPi,
              std::size_t nn, typename Tn, class SPn>
    inline Vec<ni, Tn> reflect(const Vec<ni, Ti, SPi>& i,
                               const Vec<nn, Tn, SPn>& n)
    { return i - 2.0 * dot(n, i) * n; }

    OGLML_GLSL_FUNC(reflect)


    // Refract
    template <std::size_t ni, typename Ti, class SPi,
              std::size_t nn, typename Tn, class SPn>
    inline Vec<ni, Tn> refract(const Vec<ni, Ti, SPi>& i,
                               const Vec<nn, Tn, SPn>& n,
                               float eta) {
        float k = 1.0 - eta * eta * (1.0 - dot(n, i) * dot(n, i));
        if (k < 0.0)
            return Vec<ni, Tn>(0.0);
        else
            return eta * i - (eta * dot(n, i) + sqrt(k)) * n;
    }

    OGLML_GLSL_FUNC(refract)



    // -------------------------------------------------------------- //
    // Vector relational functions                                    //
    // -------------------------------------------------------------- //

#ifndef OGLML_DOXYGEN_SKIP
#define OGLML_VEC_RELATIONAL_FUNC(_NAME_, _EX_) \
    template <std::size_t n, typename T1, class SP1, typename T2, class SP2> \
    inline Vec<n, bool> _NAME_(const Vec<n, T1, SP1>& v1, \
                                 const Vec<n, T2, SP2>& v2) { \
        Vec<n, bool> ret; \
        for (std::size_t i = 0; i < n; ++i) \
            ret[i] = (v1[i] _EX_ v2[i]); \
        return ret; \
    } \
    \
    OGLML_GLSL_FUNC(_NAME_)
#endif // OGLML_DOXYGEN_SKIP

    OGLML_VEC_RELATIONAL_FUNC(lessThan, <)
    OGLML_VEC_RELATIONAL_FUNC(lessThanEqual, <=)
    OGLML_VEC_RELATIONAL_FUNC(greaterThan, >)
    OGLML_VEC_RELATIONAL_FUNC(greaterThanEqual, >=)
    OGLML_VEC_RELATIONAL_FUNC(equal, ==)
    OGLML_VEC_RELATIONAL_FUNC(notEqual, !=)


    template <std::size_t n, class SP>
    inline bool any(const Vec<n, bool, SP>& v) {
        for (std::size_t i = 0; i < n; ++i) {
            if (v[i])
                return true;
        }
        return false;
    }

    OGLML_GLSL_FUNC(any)

    template <std::size_t n, class SP>
    inline bool all(const Vec<n, bool, SP>& v) {
        for (std::size_t i = 0; i < n; ++i) {
            if (!v[i])
                return false;
        }
        return true;
    }

    OGLML_GLSL_FUNC(all)

    template <std::size_t n, class SP>
    inline Vec<n, bool> operator!(const Vec<n, bool, SP>& v) {
        Vec<n, bool> ret;
        for (std::size_t i = 0; i < n; ++i)
            ret[i] = !v[i];
        return ret;
    }

}

#endif // OGLML_FUNCS_HPP
