#ifndef OGLML_VEC_STORAGEPOLICIES_STATICSWIZZLER_HPP
#define OGLML_VEC_STORAGEPOLICIES_STATICSWIZZLER_HPP

#include <vector>
#include <algorithm>

#include <cstddef>
#include <cassert>

#include <oglml/helpers/errors.hpp>
#include <oglml/helpers/indextools.hpp>
#include <oglml/helpers/operations.hpp>

namespace oglml {

    namespace vec {

        template <std::size_t nHost, typename THost, class SPHost, std::size_t... tindices>
        struct StaticSwizzlerStorage {

            // Constants
            oglml_constexpr static std::size_t indexcount = sizeof...(tindices);
            oglml_constexpr static bool valid = oglml::detail::CheckIndices<nHost, tindices...>::passed;
            oglml_constexpr static bool containsDuplicates =
                    oglml::detail::HasDuplicates<tindices...>::result;

            // Indices as array
            static const std::size_t indices[indexcount];

            // Validate indices
            static oglml_constexpr_if_available void validate()
            { static_assert(valid, "Indices are not valid."); }

            static oglml_constexpr_if_available void noDuplicates()
            { static_assert(!containsDuplicates, OGLML_INDEX_DUPLICATES_ERROR_MSG); };

            // Grab indices
            static std::size_t index(std::size_t i)
            { validate(); assert(i < indexcount); return indices[i]; }

            template <std::size_t n, typename T>
            class Container {

                typedef typename SPHost::template Container<nHost, THost> ContainerHost;

                ContainerHost& host()
                { return *reinterpret_cast<ContainerHost*>(this); }

                const ContainerHost& host() const
                { return *reinterpret_cast<const ContainerHost*>(this); }

            public:

                // Constants
                oglml_constexpr static bool defineSwizzlers = false;

                // Basic
                T& operator[](std::size_t i)
                { return host()[index(i)];  }

                const T& operator[](std::size_t i) const
                { return host()[index(i)];  }
            };

        };

        template <std::size_t nHost, typename THost, class SPHost, std::size_t... tindices>
        const std::size_t StaticSwizzlerStorage<nHost, THost, SPHost, tindices...>::indices[indexcount] = { tindices... };

    } // namespace vec

} // namespace oglml

#endif // OGLML_VEC_STORAGEPOLICIES_STATICSWIZZLER_HPP
