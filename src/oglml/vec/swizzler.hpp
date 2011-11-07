#ifndef OGLML_VEC_SWIZZLER_HPP
#define OGLML_VEC_SWIZZLER_HPP

#include <oglml/vec/expression.hpp>
#include <oglml/vec/funcs.hpp>
#include <oglml/helpers/compilerinfo.hpp>
#include <oglml/helpers/select.hpp>
#include <oglml/helpers/errors.hpp>

namespace oglml {
    namespace vec {
        namespace detail {

            // Index validators
            namespace swizzler {

                // Access single elements
                template <std::size_t...>
                struct Access { };

                template <std::size_t idx, std::size_t first, std::size_t... array>
                struct Access<idx, first, array...> {
                    oglml_constexpr static std::size_t result = Access<idx - 1, array...>::result;
                };

                template <std::size_t first, std::size_t... array>
                struct Access<0, first, array...> {
                    oglml_constexpr static std::size_t result = first;
                };

                // Check for duplicates
                namespace find {

                    struct Forward { };
                    struct Backward { };

                    namespace detail {

                        template <std::size_t passed>
                        struct Found {
                            oglml_constexpr static bool found = true;
                            oglml_constexpr static std::size_t index = passed;
                        };

                    } // namespace detail

                } // namespace find

                // Find forward
                namespace findf {

                    namespace detail {

                        template <std::size_t...>
                        struct Find { };

                        template <std::size_t v, std::size_t passed, std::size_t... array>
                        struct Find<v, passed, array...> {
                            oglml_constexpr static bool found = false;
                            oglml_constexpr static std::size_t index = passed;
                        };

                        template <std::size_t v, std::size_t passed, std::size_t first, std::size_t... array>
                        struct Find<v, passed, first, array...> {
                            oglml_constexpr static bool hereFound = (v == first);
                            typedef typename Select<hereFound, find::detail::Found<passed>,
                            Find<v, passed + 1, array...> >::Result Finder;
                            oglml_constexpr static bool found = Finder::found;
                            oglml_constexpr static std::size_t index = Finder::index;
                        };

                    } // namespace detail

                    template <std::size_t v, std::size_t... array>
                    struct Find {
                        typedef detail::Find<v, 0, array...> Finder;
                        oglml_constexpr static bool found = Finder::found;
                        oglml_constexpr static std::size_t index = Finder::index;
                    };

                } // namespace findf

                // Find backward
                namespace findb {

                    namespace detail {

                        template <std::size_t...>
                        struct Find { };

                        template <std::size_t v, std::size_t idx, std::size_t... array>
                        struct Find<v, idx, array...> {
                            oglml_constexpr static bool hereFound =
                                    (Access<idx, array...>::result == v);
                            typedef typename Select<hereFound, find::detail::Found<idx>,
                            Find<v, idx - 1, array...> >::Result Finder;
                            oglml_constexpr static bool found = Finder::found;
                            oglml_constexpr static std::size_t index = Finder::index;
                        };

                        template <std::size_t v, std::size_t... array>
                        struct Find<v, 0, array...> {
                            oglml_constexpr static bool found =
                                    (Access<0, array...>::result == v);
                            oglml_constexpr static std::size_t index = 0;
                        };

                    } // namespace detail

                    template <std::size_t v, std::size_t... array>
                    struct Find {
                        typedef detail::Find<v, sizeof...(array) - 1, array...> Finder;
                        oglml_constexpr static bool found = Finder::found;
                        oglml_constexpr static std::size_t index = Finder::index;
                    };

                } // namespace findb

                template <class Method, std::size_t v, std::size_t... array>
                struct Find {
                    typedef findf::Find<v, array...> Finder;
                    oglml_constexpr static bool found = Finder::found;
                    oglml_constexpr static std::size_t index = Finder::index;
                };

                template <std::size_t v, std::size_t... array>
                struct Find<find::Backward, v, array...> {
                    typedef findb::Find<v, array...> Finder;
                    oglml_constexpr static bool found = Finder::found;
                    oglml_constexpr static std::size_t index = Finder::index;
                };

                // Check for duplicates
                namespace dupl {

                    template <std::size_t val, std::size_t... array>
                    struct FindFWandBW {
                        oglml_constexpr static std::size_t forwards =
                                Find<find::Forward, val, array...>::index;
                        oglml_constexpr static std::size_t backwards =
                                Find<find::Backward, val, array...>::index;
                        oglml_constexpr static bool differs = (forwards != backwards);
                    };

                    struct Duplicated {
                        oglml_constexpr static bool result = true;
                    };

                    template <std::size_t idx, std::size_t... array>
                    class HasDuplicates {
                        oglml_constexpr static std::size_t val = Access<idx, array...>::result;
                        typedef FindFWandBW<val, array...> Finder;

                    public:
                        oglml_constexpr static bool result = Select<Finder::differs,
                        Duplicated, HasDuplicates<idx - 1, array...> >::Result::result;

                    };

                    template <std::size_t first, std::size_t... array>
                    class HasDuplicates<0, first, array...> {
                        oglml_constexpr static std::size_t val = Access<0, array...>::result;
                        typedef FindFWandBW<val, array...> Finder;

                    public:
                        oglml_constexpr static bool result = Finder::differs;
                    };

                } // namespace dupl

                template <std::size_t... array>
                struct HasDuplicates {
                    oglml_constexpr static bool result =
                            dupl::HasDuplicates<sizeof...(array) - 1, array...>::result;
                };

                // Check for index overflow
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

            // Swizzler
            template <class I, std::size_t... tindices>
            class Swizzler : public vec::Expression<vec::expression::Info<
                    sizeof...(tindices), typename I::T, Swizzler<I, tindices...>,
                    typename I::ReturnT, typename I::ConstReturnT> > {

            public:
                oglml_constexpr static std::size_t n = I::n;

                typedef Swizzler<I, tindices...> ThisType;
                typedef vec::Expression<vec::expression::Info<
                        sizeof...(tindices), typename I::T, Swizzler<I, tindices...>,
                        typename I::ReturnT, typename I::ConstReturnT> > ExpressionT;
                typedef typename I::T T;
                typedef typename I::Container Container;
                typedef typename I::ReturnT ReturnT;
                typedef typename I::ConstReturnT ConstReturnT;

                // Constants
                oglml_constexpr static std::size_t indexcount = sizeof...(tindices);
                oglml_constexpr static bool valid = swizzler::CheckIndices<n, tindices...>::passed;
                oglml_constexpr static bool containsDuplicates =
                        swizzler::HasDuplicates<tindices...>::result;

            private:
#ifdef OGLML_CXX11_UNRESTRICTED_UNIONS

                Swizzler() = delete;
                ~Swizzler() = delete;

#endif // OGLML_CXX11_UNRESTRICTED_UNIONS

                Container& host()
                { return *reinterpret_cast<Container*>(this); }

                const Container& host() const
                { return *reinterpret_cast<const Container*>(this); }

            public:
                // Validate indices
                static oglml_constexpr_if_available void validate()
                { static_assert(valid, "Indices are not valid."); }

                static oglml_constexpr_if_available void noDuplicates()
                { static_assert(!containsDuplicates, "Contains index duplicates."); };

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

                // Assignment operator
                template <class Irhs>
                ExpressionT& operator=(const vec::Expression<Irhs>& rhs)
                { noDuplicates(); return assign(*this, rhs); }

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
