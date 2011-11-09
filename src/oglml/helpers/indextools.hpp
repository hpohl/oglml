#ifndef OGLML_HELPERS_INDEXTOOLS_HPP
#define OGLML_HELPERS_INDEXTOOLS_HPP

#include <cstddef>

#include <oglml/helpers/compilerinfo.hpp>
#include <oglml/helpers/select.hpp>

namespace oglml {
    namespace detail {

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

    } // namespace detail
} // namespace oglml

#endif // OGLML_HELPERS_INDEXTOOLS_HPP
