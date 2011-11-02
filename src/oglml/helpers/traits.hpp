#ifndef OGLML_HELPERS_TRAITS_HPP
#define OGLML_HELPERS_TRAITS_HPP

namespace oglml {

    namespace priv {
        template <typename T, typename...>
        struct GetFirst {
            typedef T Result;
        };

        template <int i>
        struct Int2Type { };
    }

}

#endif // OGLML_HELPERS_TRAITS_HPP
