#ifndef OGLML_VEC_SWIZZLER_HPP
#define OGLML_VEC_SWIZZLER_HPP

#include <oglml/vec/expression.hpp>
#include <oglml/helpers/compilerinfo.hpp>
#include <oglml/helpers/select.hpp>
#include <oglml/helpers/errors.hpp>

namespace oglml {
    namespace vec {
        namespace detail {

            // Index validators
            namespace swizzler {

                struct CheckFailed {
                    oglml_constexpr static bool passed = false;
                };

                template <std::size_t n, std::size_t... indices>
                struct CheckIndices {
                    oglml_constexpr static bool passed = true;
                };

                template <std::size_t n, std::size_t first,  std::size_t... indices>
                struct CheckIndices<n, first, indices...> {
                    oglml_constexpr static bool thisPassed = first < n;
                    oglml_constexpr static bool passed = Select<thisPassed,
                        CheckIndices<n, indices...>, CheckFailed>::Result::passed;
                };

            } // namespace swizzler

            // Template to collect Vec informations
            template <std::size_t tn, typename tT, class tSP>
            struct VecInfo {
                oglml_constexpr static std::size_t n = tn;
                typedef tT T;
                typedef tSP SP;

                // Prepare information
                typedef typename SP::template Container<n, T> Container;
                typedef typename Container::ReturnT ReturnT;
                typedef typename Container::ConstReturnT ConstReturnT;
            };

            // Swizzler
            template <class I, std::size_t... tindices>
            class Swizzler : public vec::Expression<vec::expression::Info<
                    sizeof...(tindices), typename I::T, Swizzler<I, tindices...>,
                    typename I::ReturnT, typename I::ConstReturnT> > {

            public:
                oglml_constexpr static std::size_t n = I::n;

                typedef typename I::T T;
                typedef typename I::Container Container;
                typedef typename I::ReturnT ReturnT;
                typedef typename I::ConstReturnT ConstReturnT;

                // Constants
                oglml_constexpr static std::size_t indexcount = sizeof...(tindices);
                oglml_constexpr static bool valid = swizzler::CheckIndices<n, tindices...>::passed;

            private:
#ifdef OGLML_CXX11_UNRESTRICTED_UNIONS

                Swizzler() = default;
                ~Swizzler() = default;

#endif // OGLML_CXX11_UNRESTRICTED_UNIONS

                Container& host()
                { return *reinterpret_cast<Container*>(this); }

                const Container& host() const
                { return *reinterpret_cast<const Container*>(this); }

            public:
                // Validate indices
                static oglml_constexpr_if_available void validate()
                { static_assert(valid, "Indices are not valid."); }

                // Indices as array
                static const std::size_t indices[indexcount];

                // Grab indices
                static std::size_t index(std::size_t i)
                { validate(); assert(i < indexcount); return indices[i]; }

                // Index operator
                ReturnT operator[](std::size_t i)
                { return host()[index(i)]; }

                ConstReturnT operator[](std::size_t i) const
                { return host()[index(i)]; }

                // Cast operator
                operator ReturnT()
                { dimAssert<indexcount, 1>(); return host()[index(0)]; }

                operator ConstReturnT() const
                { dimAssert<indexcount, 1>(); return host()[index(0)]; }
            };

            // Swizzler static
            template <class I, std::size_t... tindices>
            const std::size_t Swizzler<I, tindices...>::indices[indexcount] = { tindices... };

        } // namespace detail
    } // namespace vec
} // namespace oglml

#endif // OGLML_VEC_SWIZZLER_HPP
