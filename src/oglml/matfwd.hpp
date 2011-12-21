#ifndef OGLML_MATFWD_HPP
#define OGLML_MATFWD_HPP

/*! \file matfwd.hpp
  \brief Contains the matrix forward declaration
*/

#include <oglml/mat/storagepolicies/default.hpp>

namespace oglml {

    template <std::size_t, std::size_t, typename, class = mat::DefaultStorage>
    class Mat;

} // namespace oglml

#endif // OGLML_MATFWD_HPP
