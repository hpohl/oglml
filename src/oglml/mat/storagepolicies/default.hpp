#ifndef OGLML_MAT_STORAGEPOLICIES_DEFAULT_HPP
#define OGLML_MAT_STORAGEPOLICIES_DEFAULT_HPP

#include <cstddef>
#include <cassert>

#include <oglml/vecfwd.hpp>

namespace oglml {

    namespace mat {

        struct DefaultStorage {

            template <std::size_t cols, std::size_t rows, typename T>
            class Container {
                typedef Vec<rows, T> DataType;

                DataType mData[cols];

            public:
                // Typedefs
                typedef DataType& ReturnT;
                typedef const DataType& ConstReturnT;

                // Basic
                ReturnT operator[](std::size_t i)
                { assert(i < cols); return mData[i]; }

                ConstReturnT operator[](std::size_t i) const
                { assert(i < cols); return mData[i]; }

                // Extensions
                DataType* ptr()
                { return mData; }

                const DataType* ptr() const
                { return mData; }
            };

        };

    } // namespace mat

} // namespace oglml

#endif // OGLML_MAT_STORAGEPOLICIES_DEFAULT_HPP
