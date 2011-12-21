#ifndef OGLML_VEC_HPP
#define OGLML_VEC_HPP

/*! \file vec.hpp
*/

#include <iostream>
#include <cstddef>

#include <oglml/vec/storagepolicies/default.hpp>
#include <oglml/vec/storagepolicies/staticswizzler.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/autoreturn.hpp>
#include <oglml/vec/detail.hpp>
#include <oglml/vec/funcs.hpp>
#include <oglml/vec/operators.hpp>
#include <oglml/vecfwd.hpp>

namespace oglml {

    template <std::size_t n, typename T, class SP>
    struct Vec : public BaseVec<n, T>, public vec::detail::CreateDataImpl<n, T, SP>::Result
    {
        //! This type of vec
        typedef Vec<n, T, SP> ThisType;

        //! The container type given by the storage policy
        typedef typename SP::template Container<n, T> Container;

        //! The type of the data implementation the vec derived from
        typedef typename vec::detail::CreateDataImpl<n, T, SP>::Result DataImpl;


        //! Default constructor
        Vec() = default;

        //! GLSL constructor
        template <typename First, typename... Args>
        Vec(const First& first, const Args&... args)
        { assign(*this, first, args...); }

        //! Default destructor
        ~Vec() = default;

        //! Index operator
        T& operator[](std::size_t i)
        { return DataImpl::data[i]; }

        //! Index operator - const version
        const T& operator[](std::size_t i) const
        { return DataImpl::data[i]; }

        //! Promotion
        Vec<n, T> operator+() const
        { return promote(*this); }

        //! Negation
        Vec<n, T> operator-() const
        { return negate(*this); }

        //! Assignment operator
        template <typename Rhs>
        ThisType& operator=(const Rhs& rhs)
        { return assign(*this, rhs); }

        //! Addition assignment operator
        template <typename Rhs>
        ThisType& operator+=(const Rhs& rhs)
        { return opassign<Plus>(*this, rhs); }

        //! Subtraction assignment operator
        template <typename Rhs>
        ThisType& operator-=(const Rhs& rhs)
        { return opassign<Minus>(*this, rhs); }

        //! Multiplies assignment operator
        template <typename Rhs>
        ThisType& operator*=(const Rhs& rhs)
        { return opassign<Multiplies>(*this, rhs); }

        //! Divides assignment operator
        template <typename Rhs>
        ThisType& operator/=(const Rhs& rhs)
        { return opassign<Divides>(*this, rhs); }

        //! Modulus assignment operator
        template <typename Rhs>
        ThisType& operator%=(const Rhs& rhs)
        { return opassign<Modulus>(*this, rhs); }

        //! Returns the vector length
        oglml_constexpr_if_available static std::size_t length()
        { return n; }
    };

    namespace glsl {
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

#ifdef OGLML_CXX11_TEMPLATE_ALIASES

        // Template aliases
        template <std::size_t n>
        using vec = Vec<n, float>;

        template <std::size_t n>
        using ivec = Vec<n, int>;

        template <std::size_t n>
        using uvec = Vec<n, unsigned int>;

        template <std::size_t n>
        using bvec = Vec<n, bool>;

        template <std::size_t n>
        using dvec = Vec<n, double>;


        template <typename T>
        using vec2 = Vec<2, T>;

        template <typename T>
        using vec3 = Vec<3, T>;

        template <typename T>
        using vec4 = Vec<4, T>;

#endif // OGLML_CXX11_TEMPLATE_ALIASES

    } // namespace glsl

    using namespace glsl;

} // namespace oglml

#endif // OGLML_VEC_HPP
