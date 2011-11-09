#ifndef OGLML_VEC_STORAGEPOLICIES_SWIZZLER_HPP
#define OGLML_VEC_STORAGEPOLICIES_SWIZZLER_HPP

#include <vector>

#include <cstddef>
#include <cassert>

#include <oglml/helpers/operations.hpp>

namespace oglml {

    namespace vec {

        template <class Host>
        struct SwizzlerStorage {

            template <std::size_t n, typename T>
            class Container {
                Host* mHost;
                std::size_t mIndices[n];

            public:
                // Typedefs
                typedef T& ReturnT;
                typedef const T& ConstReturnT;

                // Basic
                ReturnT operator[](std::size_t i)
                { assert(i < n); return (*mHost)[mIndices[i]]; }

                ConstReturnT operator[](std::size_t i) const
                { assert(i < n); return (*mHost)[mIndices[i]]; }

                // Extensions
                void setHost(Host* h)
                { mHost = h; }

                void setHost(const Host* h)
                { mHost = const_cast<Host*>(h); }

                void setIndex(std::size_t pos, std::size_t i)
                { assert(pos < n); mIndices[pos] = i; }

                void setIndices(const std::vector<std::size_t>& indices) {
                    assert(indices.size() == n);
                    oglml::detail::assignArray<n>(mIndices, indices);
                }
            };

        };

    } // namespace vec

} // namespace oglml

#endif // OGLML_VEC_STORAGEPOLICIES_SWIZZLER_HPP
