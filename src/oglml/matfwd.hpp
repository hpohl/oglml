#ifndef OGLML_MATFWD_HPP
#define OGLML_MATFWD_HPP

#include <oglml/mat/storagepolicies/default.hpp>

namespace oglml {

    template <std::size_t, std::size_t, typename, class = mat::DefaultStorage>
    class Mat;

} // namespace oglml

#endif // OGLML_MATFWD_HPP
