#ifndef OGLML_VECFWD_HPP
#define OGLML_VECFWD_HPP

/*! \file vecfwd.hpp
  \brief Contains the vec forward declaration
*/

#include <oglml/vec/storagepolicies/default.hpp>

namespace oglml {

    template <std::size_t, typename, class = vec::DefaultStorage>
    class Vec;

} // namespace oglml

#endif // OGLML_VECFWD_HPP
