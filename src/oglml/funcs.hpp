#ifndef OGLML_FUNCS_HPP
#define OGLML_FUNCS_HPP

#include <oglml/helpers/autoreturn.hpp>
#include <oglml/vec.hpp>

namespace oglml {

    namespace detail {

        template <class Op, std::size_t n, typename T, class SP>
        inline Vec<n, decltype(Op::run(std::declval<T>()))> operate(const Vec<n, T, SP>& v) {
            Vec<n, decltype(Op::run(std::declval<T>()))> r;
            for (std::size_t i = 0; i < n; ++i)
                r[i] = Op::run(v[i]);
            return r;
        }

    } // namespace detail

    // -------------------------------------------------------------- //
    // Angle and trigonometry functions                               //
    // -------------------------------------------------------------- //
#define OGLML_TRIG_FUNC_1ARG(_NAME_, _EX_) \
    inline auto _NAME_(float v) \
    OGLML_AUTO_RETURN(_EX_::run(v)) \
    \
    namespace glsl { \
        using oglml::_NAME_; \
    } \
    \
    template <std::size_t n, typename T, class SP> \
    inline auto _NAME_(const Vec<n, T, SP>& v) \
    OGLML_AUTO_RETURN(detail::operate<_EX_>(v))

    // Func defs
    OGLML_TRIG_FUNC_1ARG(radians, Radians)
    OGLML_TRIG_FUNC_1ARG(degrees, Degrees)

    OGLML_TRIG_FUNC_1ARG(sin, Sin)
    OGLML_TRIG_FUNC_1ARG(cos, Cos)
    OGLML_TRIG_FUNC_1ARG(tan, Tan)

    OGLML_TRIG_FUNC_1ARG(asin, ASin)
    OGLML_TRIG_FUNC_1ARG(acos, ACos)
    OGLML_TRIG_FUNC_1ARG(atan, ATan)

    OGLML_TRIG_FUNC_1ARG(sinh, SinH)
    OGLML_TRIG_FUNC_1ARG(cosh, CosH)
    OGLML_TRIG_FUNC_1ARG(tanh, TanH)

    OGLML_TRIG_FUNC_1ARG(asinh, ASinH)
    OGLML_TRIG_FUNC_1ARG(acosh, ACosH)
    OGLML_TRIG_FUNC_1ARG(atanh, ATanH)
}

#endif // OGLML_FUNCS_HPP
