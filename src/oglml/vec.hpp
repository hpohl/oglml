#ifndef OGLML_VEC_HPP
#define OGLML_VEC_HPP

#include <iostream>
#include <cstddef>

#include <oglml/vecfwd.hpp>
#include <oglml/vec/storagepolicies/default.hpp>
#include <oglml/vec/expression.hpp>
#include <oglml/vec/funcs.hpp>
#include <oglml/vec/operators.hpp>
#include <oglml/vec/swizzler.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/autoreturn.hpp>

namespace oglml {

    template <std::size_t n, typename T, class SP = vec::DefaultStorage>
    class Vec : public vec::Expression<vec::expression::Info<n, T, Vec<n, T, SP>,
            typename SP::template Container<n, T>::ReturnT,
            typename SP::template Container<n, T>::ConstReturnT> > {
    public:
        // Typedefs
        typedef Vec<n, T, SP> ThisType;
        typedef typename SP::template Container<n, T> Container;
        typedef typename Container::ReturnT ReturnT;
        typedef typename Container::ConstReturnT ConstReturnT;
        typedef vec::Expression<vec::expression::Info<n, T, ThisType, ReturnT, ConstReturnT> > ExpressionT;
        typedef vec::detail::VecInfo<n, T, SP> VecInfo;

        // Ctor and Dtor
        Vec()
        { }

        template <typename First, typename... Args>
        Vec(const First& first, const Args&... args)
        { assign(*this, first, args...); }

        ~Vec()
        { }

        // Index operator
        ReturnT operator[](std::size_t i)
        { return data[i]; }

        ConstReturnT operator[](std::size_t i) const
        { return data[i]; }

        // Assignment operator
        template <typename Trhs>
        ExpressionT& operator=(const Trhs& rhs)
        { return assign(*this, rhs); }

        // Swizzlers + data
        union {
            // Contains the data, public accessable
            Container data;

            // Swizzlers
            vec::detail::Swizzler<VecInfo, 0> x;
            vec::detail::Swizzler<VecInfo, 1> y;
            vec::detail::Swizzler<VecInfo, 2> z;
            vec::detail::Swizzler<VecInfo, 3> w;

            vec::detail::Swizzler<VecInfo, 0, 1> xy;
            vec::detail::Swizzler<VecInfo, 1, 0> yx;

            vec::detail::Swizzler<VecInfo, 0, 1, 2> xyz;
        };
    };

    // GLSL Types
    // bool
    typedef Vec<2, bool> bvec2;
    typedef Vec<3, bool> bvec3;
    typedef Vec<4, bool> bvec4;

    // integer
    typedef Vec<2, int> ivec2;
    typedef Vec<3, int> ivec3;
    typedef Vec<4, int> ivec4;

    // unsigned int
    typedef Vec<2, unsigned int> uvec2;
    typedef Vec<3, unsigned int> uvec3;
    typedef Vec<4, unsigned int> uvec4;

    // float
    typedef Vec<2, float> vec2;
    typedef Vec<3, float> vec3;
    typedef Vec<4, float> vec4;

    // double
    typedef Vec<2, double> dvec2;
    typedef Vec<3, double> dvec3;
    typedef Vec<4, double> dvec4;

} // namespace oglml

#endif // OGLML_VEC_HPP
