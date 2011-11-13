#ifndef OGLML_VEC_SWIZZLER_HPP
#define OGLML_VEC_SWIZZLER_HPP

#include <oglml/vec/info.hpp>
#include <oglml/vec/expression.hpp>
#include <oglml/vec/swizzlerfwd.hpp>
#include <oglml/vec/funcs.hpp>
#include <oglml/vec/storagepolicies/swizzler.hpp>
#include <oglml/helpers/errors.hpp>
#include <oglml/helpers/indextools.hpp>
#include <oglml/helpers/traits.hpp>

namespace oglml {
    // Overloaded assign function
    template <class T, std::size_t... indices, typename First, typename... Args>
    vec::detail::Swizzler<T, indices...>&
    assign(vec::detail::Swizzler<T, indices...>& swizz, const First& first, const Args&... args) {
        static_assert(!detail::HasDuplicates<indices...>::result,
                      OGLML_INDEX_DUPLICATES_ERROR_MSG);
        nassign::Assignment<0>::run(swizz, first, args...);
        return swizz;
    }

    namespace vec {
        namespace detail {

            // Swizzler
            template <class Tvec, std::size_t... tindices>
            class Swizzler : public BaseSwizzler {
            public:                
                typedef Swizzler<Tvec, tindices...> ThisType;
                typedef typename Tvec::Container Container;

                // Constants
                oglml_constexpr static std::size_t indexcount = sizeof...(tindices);
                oglml_constexpr static bool valid = oglml::detail::CheckIndices<Tvec::n, tindices...>::passed;
                oglml_constexpr static bool containsDuplicates =
                        oglml::detail::HasDuplicates<tindices...>::result;

                // Vec info
                oglml_constexpr static std::size_t n = indexcount;
                typedef typename Tvec::T T;
                typedef typename Tvec::ReturnT ReturnT;
                typedef typename Tvec::ConstReturnT ConstReturnT;

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
                { static_assert(!containsDuplicates, OGLML_INDEX_DUPLICATES_ERROR_MSG); };

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
                template <class Tex>
                typename vec::detail::VecFunc<ThisType&, Tex>::Result operator=(const Tex& rhs)
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

    // Swizzler funcs
    // Static
    template <std::size_t... indices, class Tex>
    vec::detail::Swizzler<Tex, indices...>& swizzle(Tex& ex) {
        return *reinterpret_cast<vec::detail::Swizzler<Tex, indices...>*>(&ex);
    }

    template <std::size_t... indices, class Tex>
    const vec::detail::Swizzler<Tex, indices...>& swizzle(const Tex& ex) {
        return *reinterpret_cast<const vec::detail::Swizzler<Tex, indices...>*>(&ex);
    }

    // Runtime
    template <class Tex, typename... Args>
    const Vec<Tex::n, typename Tex::T, vec::SwizzlerStorage<Tex> > swizzle
    (const Tex& ex, const Args&... args) {
        static_assert(sizeof...(Args) == Tex::n, "Too many/less swizzle parameters.");
        std::vector<int> indices = { args... };

        Vec<Tex::n, typename Tex::T, vec::SwizzlerStorage<Tex> > ret;

        ret.data.setHost(&ex);
        for (std::size_t i = 0; i < Tex::n; ++i)
            ret.data.setIndex(i, indices[i]);
        return ret;
    }

    template <class Tex, typename... Args>
    Vec<Tex::n, typename Tex::T, vec::SwizzlerStorage<Tex> > swizzle
    (Tex& ex, const Args&... args)
    { return swizzle(const_cast<const Tex&>(ex), args...); }

    // Additional overload to prevent runtime index duplicates
    template <std::size_t n, typename T, class Host, typename First, typename... Args>
    Vec<n, T, vec::SwizzlerStorage<Host> >&
    assign(Vec<n, T, vec::SwizzlerStorage<Host> >& vec, const First& first, const Args&... args) {
        assert(!vec.data.duplicates());
        nassign::Assignment<0>::run(vec, first, args...);
        return vec;
    }

} // namespace oglml

#endif // OGLML_VEC_SWIZZLER_HPP
