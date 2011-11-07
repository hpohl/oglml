#ifndef OGLML_HELPERS_TRAITS_HPP
#define OGLML_HELPERS_TRAITS_HPP

namespace oglml {

    namespace detail {

        template <typename T, typename...>
        struct GetFirst {
            typedef T Result;
        };

        template <typename, typename T, typename...>
        struct GetSecond {
            typedef T Result;
        };

        template <int i>
        struct Int2Type { };
    } // namespace detail

} // namespace oglml

#endif // OGLML_HELPERS_TRAITS_HPP
