#ifndef OGLML_VEC_STORAGEPOLICIES_POINTER_HPP
#define OGLML_VEC_STORAGEPOLICIES_POINTER_HPP

#include <vector>

#include <cstddef>
#include <cassert>

#include <oglml/helpers/operations.hpp>

namespace oglml {
    namespace vec {
        struct PointerStorage {

            template <std::size_t n, typename T>
            class Container {
                T* mDataPtr[n];

            public:
                // Typedefs
                typedef T& ReturnT;
                typedef const T& ConstReturnT;

                // Constants
                oglml_constexpr static bool defineSwizzlers = true;

                // Basic
                ReturnT operator[](std::size_t i)
                { assert(i < n); return *mDataPtr[i]; }

                ConstReturnT operator[](std::size_t i) const
                { assert(i < n); return *mDataPtr[i]; }

                // Extensions
                void setPointer(std::size_t pos, T* v)
                { assert(pos < n); mDataPtr[pos] = v; }

                void setPointers(const std::vector<T*>& v)
                { assert(v.size() == n); detail::assignArray<n>(mDataPtr, v); }
            };

        };
    } // namespace vec
} // namespace oglml

#endif // OGLML_VEC_STORAGEPOLICIES_POINTER_HPP
