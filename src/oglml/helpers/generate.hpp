#ifndef OGLML_HELPERS_GENERATE_HPP
#define OGLML_HELPERS_GENERATE_HPP

#include <type_traits>

namespace oglml {
    namespace detail {

        template <typename T>
        typename std::remove_reference<T>::type generate()
        { return *static_cast<typename std::remove_reference<T>::type*>(0); }

    } // namespace detail
} // namespace oglml

#endif // OGLML_HELPERS_GENERATE_HPP
