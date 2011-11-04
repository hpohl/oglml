#ifndef OGLML_VEC_STORAGEPOLICIES_DEFAULT_HPP
#define OGLML_VEC_STORAGEPOLICIES_DEFAULT_HPP

#include <cstddef>
#include <cassert>

namespace oglml {

    namespace vec {

        struct DefaultStorage {

            template <std::size_t n, typename T>
            class Container {
                T mData[n];

            public:
                // Typedefs
                typedef T& ReturnT;
                typedef const T& ConstReturnT;

                // Basic
                ReturnT operator[](std::size_t i)
                { assert(i < n); return mData[i]; }

                ConstReturnT operator[](std::size_t i) const
                { assert(i < n); return mData[i]; }

                // Extensions
                T* ptr()
                { return mData; }

                const T* ptr() const
                { return mData; }
            };

        };

    } // namespace vec

} // namespace oglml

#endif // OGLML_VEC_STORAGEPOLICIES_DEFAULT_HPP
