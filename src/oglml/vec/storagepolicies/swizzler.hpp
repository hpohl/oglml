#ifndef OGLML_VEC_STORAGEPOLICIES_SWIZZLER_HPP
#define OGLML_VEC_STORAGEPOLICIES_SWIZZLER_HPP

#include <vector>
#include <algorithm>

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
                typedef typename Host::ReturnT ReturnT;
                typedef typename Host::ConstReturnT ConstReturnT;

                // Helpers
                bool valid() const {
                    for (std::size_t i = 0; i < n; ++i) {
                        if (mIndices[i] >= Host::n)
                            return false;
                    }
                    return true;
                }

                bool duplicates() const {
                    for (std::size_t i = 0; i < n; ++i) {
                        for (std::size_t j = 0; j < n; ++j) {
                            if ((j != i) && (mIndices[i] == mIndices[j]))
                                return true;
                        }
                    }
                    return false;
                }

                // Basic
                ReturnT operator[](std::size_t i)
                { assert((i < n) && valid()); return (*mHost)[mIndices[i]]; }

                ConstReturnT operator[](std::size_t i) const
                { assert((i < n) && valid()); return (*mHost)[mIndices[i]]; }

                // Extensions
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
