#ifndef OGLML_HELPERS_SELECT_HPP
#define OGLML_HELPERS_SELECT_HPP

namespace oglml {

    // Type selection
    template <bool flag, typename T1, typename T2>
    struct Select {
        typedef T1 Result;
    };

    template <typename T1, typename T2>
    struct Select<false, T1, T2> {
        typedef T2 Result;
    };

} // namespace oglml

#endif // OGLML_HELPERS_SELECT_HPP
