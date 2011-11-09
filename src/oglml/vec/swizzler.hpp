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
    template <class I, std::size_t... indices, typename First, typename... Args>
    typename vec::detail::Swizzler<I, indices...>::ExpressionT& assign
    (vec::detail::Swizzler<I, indices...>& swizz, const First& first, const Args&... args) {
        static_assert(!detail::HasDuplicates<indices...>::result,
                      OGLML_INDEX_DUPLICATES_ERROR_MSG);
        return assign(static_cast<typename vec::detail::Swizzler<I, indices...>
                      ::ExpressionT&>(swizz), first, args...);
    }

    namespace vec {
        namespace detail {

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
                oglml_constexpr static bool valid = oglml::detail::CheckIndices<n, tindices...>::passed;
                oglml_constexpr static bool containsDuplicates =
                        oglml::detail::HasDuplicates<tindices...>::result;

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

    // Swizzler funcs
    // Static
    template <std::size_t... indices, class I>
    vec::detail::Swizzler
    <typename vec::detail::ExprInfo2HostInfo<I>::Result, indices...>& swizzle
    (vec::Expression<I>& ex) {
        return *reinterpret_cast<vec::detail::Swizzler
                <typename vec::detail::ExprInfo2HostInfo<I>::Result, indices...>*>
                (&ex);
    }

    template <std::size_t... indices, class I>
    const vec::detail::Swizzler
    <typename vec::detail::ExprInfo2HostInfo<I>::Result, indices...>& swizzle
    (const vec::Expression<I>& ex) {
        return *reinterpret_cast<const vec::detail::Swizzler
                <typename vec::detail::ExprInfo2HostInfo<I>::Result, indices...>*>
                (&ex);
    }

    // Runtime
    template <class I, typename... Args>
    const Vec<I::n, typename I::T, vec::SwizzlerStorage<vec::Expression<I> > > swizzle
    (const vec::Expression<I>& ex, const Args&... args) {
        static_assert(sizeof...(Args) == I::n, "Too many/less swizzle parameters.");
        std::vector<int> indices = { args... };

        Vec<I::n, typename I::T, vec::SwizzlerStorage<vec::Expression<I> > > ret;

        ret.data.setHost(&ex);
        for (std::size_t i = 0; i < I::n; ++i)
            ret.data.setIndex(i, indices[i]);
        return ret;
    }

    template <class I, typename... Args>
    Vec<I::n, typename I::T, vec::SwizzlerStorage<vec::Expression<I> > > swizzle
    (vec::Expression<I>& ex, const Args&... args)
    { return swizzle(const_cast<const vec::Expression<I>&>(ex), args...); }

} // namespace oglml

#endif // OGLML_VEC_SWIZZLER_HPP
