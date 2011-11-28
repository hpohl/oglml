#ifndef OGLML_MAT_HPP
#define OGLML_MAT_HPP

#include <cstddef>

#include <oglml/mat/storagepolicies/default.hpp>
#include <oglml/vecfwd.hpp>

namespace oglml {

    template <std::size_t cols, std::size_t rows,
              typename T, class SP = mat::DefaultStorage>
    class Mat {
    public:
        // Typedefs
        typedef Mat<cols, rows, SP> ThisType;
        typedef typename SP::template Container<cols, rows, T> Container;
        typedef typename Container::ReturnT ReturnT;
        typedef typename Container::ConstReturnT ConstReturnT;

    private:
        Container mData;

    public:

        // Index operator
        ReturnT operator[](std::size_t i)
        { return mData[i]; }

        ConstReturnT operator[](std::size_t i) const
        { return mData[i]; }

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

        typedef Mat<2, 2, double> dmat2x2;
        typedef Mat<2, 3, double> dmat2x3;
        typedef Mat<2, 4, double> dmat2x4;
        typedef Mat<3, 2, double> dmat3x2;
        typedef Mat<3, 3, double> dmat3x3;
        typedef Mat<3, 4, double> dmat3x4;
        typedef Mat<4, 2, double> dmat4x2;
        typedef Mat<4, 3, double> dmat4x3;
        typedef Mat<4, 4, double> dmat4x4;

    } // namespace glsl

} // namespace oglml

#endif // OGLML_MAT_HPP
