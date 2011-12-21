#ifndef OGLML_MAT_HPP
#define OGLML_MAT_HPP

#include <cstddef>

#include <oglml/mat/storagepolicies/default.hpp>
#include <oglml/mat/info.hpp>
#include <oglml/mat/funcs.hpp>
#include <oglml/mat/operators.hpp>
#include <oglml/vecfwd.hpp>
#include <oglml/matfwd.hpp>

/*! \file mat.hpp
*/

namespace oglml {

    template <std::size_t cols, std::size_t rows, typename T, class SP>
    class Mat : public BaseMat<cols, rows, T> {
    public:
        // Typedefs
        typedef Mat<cols, rows, T, SP> ThisType;
        typedef typename SP::template Container<cols, rows, T> Container;
        typedef typename Container::ReturnT ReturnT;
        typedef typename Container::ConstReturnT ConstReturnT;

    private:
        Container mData;

    public:

        // Ctor and dtor
        Mat() = default;

        template <typename Tm, class SPm>
        Mat(const Mat<cols, rows, Tm, SPm>& m)
        { assign(*this, m); }

        template <typename First, typename... Args>
        explicit Mat(const First& first, const Args&... args)
        { assign(*this, first, args...); }

        ~Mat() = default;

        // Index operator
        ReturnT operator[](std::size_t i)
        { return mData[i]; }

        ConstReturnT operator[](std::size_t i) const
        { return mData[i]; }

        // Assignment operator
        template <typename Tm, class SPm>
        ThisType& operator=(const Mat<cols, rows, Tm, SPm>& rhs)
        { assign(*this, rhs); return *this; }

        // Promotion & negation
        Mat<cols, rows, T> operator+()
        { return promote(*this); }

        Mat<cols, rows, T> operator-()
        { return negate(*this); }

    };

    namespace glsl {

        typedef Mat<2, 2, float> mat2x2;
        typedef Mat<2, 3, float> mat2x3;
        typedef Mat<2, 4, float> mat2x4;
        typedef Mat<3, 2, float> mat3x2;
        typedef Mat<3, 3, float> mat3x3;
        typedef Mat<3, 4, float> mat3x4;
        typedef Mat<4, 2, float> mat4x2;
        typedef Mat<4, 3, float> mat4x3;
        typedef Mat<4, 4, float> mat4x4;

        typedef mat2x2 mat2;
        typedef mat3x3 mat3;
        typedef mat4x4 mat4;

        typedef Mat<2, 2, double> dmat2x2;
        typedef Mat<2, 3, double> dmat2x3;
        typedef Mat<2, 4, double> dmat2x4;
        typedef Mat<3, 2, double> dmat3x2;
        typedef Mat<3, 3, double> dmat3x3;
        typedef Mat<3, 4, double> dmat3x4;
        typedef Mat<4, 2, double> dmat4x2;
        typedef Mat<4, 3, double> dmat4x3;
        typedef Mat<4, 4, double> dmat4x4;

        typedef dmat2x2 dmat2;
        typedef dmat3x3 dmat3;
        typedef dmat4x4 dmat4;

    } // namespace glsl

} // namespace oglml

#endif // OGLML_MAT_HPP
