#ifndef OGLML_VEC_FUNCS_HPP
#define OGLML_VEC_FUNCS_HPP

#include <iostream>

#include <cmath>

#include <oglml/vec/storagepolicies/staticswizzler.hpp>
#include <oglml/vec/storagepolicies/swizzler.hpp>
#include <oglml/helpers/autoreturn.hpp>
#include <oglml/vec/info.hpp>

namespace oglml {

    namespace detail {

        template <class Op, std::size_t n, typename T, class SP>
        inline const Vec<n, decltype(Op::run(std::declval<T>()))>
        operate(const Vec<n, T, SP>& v) {
            Vec<n, decltype(Op::run(std::declval<T>()))> r;
            for (std::size_t i = 0; i < n; ++i)
                r[i] = Op::run(v[i]);
            return r;
        }

        template <class Op,
                  std::size_t nlhs, typename Tlhs, class SPlhs,
                  std::size_t nrhs, typename Trhs, class SPrhs>
        const typename vec::detail::CreateVec
        <Op, nlhs, Tlhs, SPlhs, nrhs, Trhs, SPrhs>::Result
        operate(const Vec<nlhs, Tlhs, SPlhs>& lhs, const Vec<nrhs, Trhs, SPrhs>& rhs) {
            typename vec::detail::CreateVec
                    <Op, nlhs, Tlhs, SPlhs, nrhs, Trhs, SPrhs>::Result ret;
            for (std::size_t i = 0; i < nlhs; ++i)
                ret[i] = Op::run(lhs[i], rhs[i]);
            return ret;
        }

        template <class Op,
                  std::size_t nlhs, typename Tlhs, class SPlhs,
                  typename Trhs>
        const typename detail::GetSecond<
        typename std::enable_if<!vec::IsVec<Trhs>::result>::type,
        typename vec::detail::CreateVecFromLhs<Op, nlhs, Tlhs, SPlhs, Trhs>::Result
        >::Result
        operate(const Vec<nlhs, Tlhs, SPlhs>& lhs, const Trhs& rhs) {
            typename vec::detail::CreateVecFromLhs
                    <Op, nlhs, Tlhs, SPlhs, Trhs>::Result ret;
            for (std::size_t i = 0; i < nlhs; ++i)
                ret[i] = Op::run(lhs[i], rhs);
            return ret;
        }

        template <class Op,
                  typename Tlhs,
                  std::size_t nrhs, typename Trhs, class SPrhs>
        const typename detail::GetSecond<
        typename std::enable_if<!vec::IsVec<Tlhs>::result>::type,
        typename vec::detail::CreateVecFromRhs<Op, Tlhs, nrhs, Trhs, SPrhs>::Result
        >::Result
        operate(const Tlhs& lhs, const Vec<nrhs, Trhs, SPrhs>& rhs) {
            typename vec::detail::CreateVecFromRhs
                    <Op, Tlhs, nrhs, Trhs, SPrhs>::Result ret;
            for (std::size_t i = 0; i < nrhs; ++i)
                ret[i] = Op::run(lhs, rhs[i]);
            return ret;
        }

    } // namespace detail

    // -------------------------------------------------------------------------
    // Assignments
    namespace nassign {

        template <std::size_t begin>
        struct Assignment {

            template <std::size_t n, typename T, class SP, typename... Args>
            static typename std::enable_if<!sizeof...(Args)>::type run
            (const Vec<n, T, SP>&, const Args&...) {
                static_assert(begin == n, "Not enough assignment parameters.");
            }

            template <std::size_t nt, typename Tt, class SPt,
                      std::size_t nv, typename Tv, class SPv, typename... Args>
            static void
            run(Vec<nt, Tt, SPt>& t, const Vec<nv, Tv, SPv>& v, const Args&... args) {
                static_assert((begin + nv) <= nt, "Too many assignment parameters.");
                Assignment<begin + nv>::run
                        (detail::assignArray<begin, nv>(t, v), args...);
            }

            template <std::size_t nt, typename Tt, class SPt, typename First, typename... Args>
            static typename std::enable_if
            <!sizeof...(Args) && begin == 0>::type
            run(Vec<nt, Tt, SPt>& t, const First& first, const Args&... args) {
                detail::assignValue<nt>(t, first);
            }

            template <std::size_t nt, typename Tt, class SPt, typename First, typename... Args>
            static typename std::enable_if
            <((sizeof...(Args) > 0) || (begin > 0))>::type
            run(Vec<nt, Tt, SPt>& t, const First& first, const Args&... args) {
                static_assert((begin + 1) <= nt, "Too many assignment parameters.");
                t[begin] = first;
                Assignment<begin + 1>::run(t, args...);
            }
        };

    } // namespace nassign

    template <std::size_t n, typename T, class SP, typename... Args>
    Vec<n, T, SP>& assign(Vec<n, T, SP>& v, const Args&... args)
    { nassign::Assignment<0>::run(v, args...); return v; }

    template <std::size_t n, typename T,
              std::size_t nhost, typename Thost, class SPhost, std::size_t... indices,
              typename... Args>
    Vec<n, T, vec::StaticSwizzlerStorage<nhost, Thost, SPhost, indices...> >&
    assign(Vec<n, T,
           vec::StaticSwizzlerStorage<nhost, Thost, SPhost, indices...> >& v,
           const Args&... args) {
        vec::StaticSwizzlerStorage<nhost, Thost, SPhost, indices...>::noDuplicates();
        nassign::Assignment<0>::run(v, args...);
        return v;
    }

    template <std::size_t n, typename T, class Host, typename... Args>
    Vec<n, T, vec::SwizzlerStorage<Host> >&
    assign(Vec<n, T, vec::SwizzlerStorage<Host> >& v, const Args&... args) {
        assert(!v.data.duplicates());
        nassign::Assignment<0>::run(v, args...);
        return v;
    }

    namespace nopassign {

        template <class Op, std::size_t begin>
        struct Assignment {

            template <std::size_t n, typename T, class SP, typename... Args>
            static typename std::enable_if<!sizeof...(Args)>::type run
            (const Vec<n, T, SP>&, const Args&...) {
                static_assert(begin == n, "Not enough assignment parameters.");
            }

            template <std::size_t nt, typename Tt, class SPt,
                      std::size_t nv, typename Tv, class SPv, typename... Args>
            static void
            run(Vec<nt, Tt, SPt>& t, const Vec<nv, Tv, SPv>& v, const Args&... args) {
                static_assert((begin + nv) <= nt, "Too many assignment parameters.");
                Assignment<Op, begin + nv>::run
                        (detail::opassignArray<Op, begin, nv>(t, v), args...);
            }

            template <std::size_t nt, typename Tt, class SPt, typename First, typename... Args>
            static typename std::enable_if
            <!sizeof...(Args) && begin == 0>::type
            run(Vec<nt, Tt, SPt>& t, const First& first, const Args&... args) {
                detail::opassignValue<Op, nt>(t, first);
            }

            template <std::size_t nt, typename Tt, class SPt, typename First, typename... Args>
            static typename std::enable_if
            <((sizeof...(Args) > 0) || (begin > 0))>::type
            run(Vec<nt, Tt, SPt>& t, const First& first, const Args&... args) {
                static_assert((begin + 1) <= nt, "Too many assignment parameters.");
                Op::runassign(t[begin], first);
                Assignment<Op, begin + 1>::run(t, args...);
            }
        };

    } // namespace nassign

    template <class Op, std::size_t n, typename T, class SP, typename... Args>
    typename std::enable_if<!std::is_base_of
    <vec::BaseStaticSwizzlerStorage, SP>::result, Vec<n, T, SP>&>::type
    opassign(Vec<n, T, SP>& v, const Args&... args)
    { nopassign::Assignment<Op, 0>::run(v, args...); return v; }

    template <class Op, std::size_t n, typename T,
              std::size_t nhost, typename Thost, class SPhost, std::size_t... indices,
              typename... Args>
    Vec<n, T, vec::StaticSwizzlerStorage<nhost, Thost, SPhost, indices...> >&
    opassign(Vec<n, T,
           vec::StaticSwizzlerStorage<nhost, Thost, SPhost, indices...> >& v,
           const Args&... args) {
        vec::StaticSwizzlerStorage<nhost, Thost, SPhost, indices...>::noDuplicates();
        nopassign::Assignment<Op, 0>::run(v, args...);
        return v;
    }

    // ----------------------------------------------------------------------------
    // General funcs
#define OGLML_VEC_FUNC_1ARG(_NAME_, _EX_) \
    template <std::size_t n, typename T, class SP> \
    auto _NAME_(const Vec<n, T, SP>& v) \
    OGLML_AUTO_RETURN(detail::operate<_EX_>(v))

    // Compare
    template <std::size_t nlhs, typename Tlhs, class SPlhs,
              std::size_t nrhs, typename Trhs, class SPrhs>
    bool compare(const Vec<nlhs, Tlhs, SPlhs>& lhs,
                 const Vec<nrhs, Trhs, SPrhs>& rhs) {
        dimAssert<nlhs, nrhs>();
        for (std::size_t i = 0; i < nlhs; ++i) {
            if (lhs[i] != rhs[i])
                return false;
        }
        return true;
    }

    // Negate
    OGLML_VEC_FUNC_1ARG(negate, Negation)

    // Print
    template <std::size_t n, typename T, class SP>
    void print(const Vec<n, T, SP>& v) {
        for (std::size_t i = 0; i < n; ++i) {
            std::cout << v[i];
            if (i < n - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    // Promote
    OGLML_VEC_FUNC_1ARG(promote, Promotion)

    // Swizzle
    // Static
    template <std::size_t... indices, std::size_t n, typename T, class SP>
    Vec<sizeof...(indices), T, vec::StaticSwizzlerStorage<n, T, SP, indices...> >&
    swizzle(Vec<n, T, SP>& v) {
        typedef vec::StaticSwizzlerStorage<n, T, SP, indices...> SPs;
        return *reinterpret_cast<Vec<sizeof...(indices), T, SPs>*>(&v.data);
    }

    template <std::size_t... indices, std::size_t n, typename T, class SP>
    const Vec<sizeof...(indices), T, vec::StaticSwizzlerStorage<n, T, SP, indices...> >&
    swizzle(const Vec<n, T, SP>& v) {
        typedef vec::StaticSwizzlerStorage<n, T, SP, indices...> SPs;
        return *reinterpret_cast<const Vec<sizeof...(indices), T, SPs>*>(&v.data);
    }

    // Runtime
    template <std::size_t n, typename T, class SP, typename... Args>
    Vec<sizeof...(Args), T, vec::SwizzlerStorage<Vec<n, T, SP> > >
    swizzle(Vec<n, T, SP>& v, const Args&... indices) {
        const std::size_t indicesArray[] = { indices... };
        Vec<sizeof...(Args), T, vec::SwizzlerStorage<Vec<n, T, SP> > > ret;
        ret.data.setHost(&v);
        for (std::size_t i = 0; i < sizeof...(indices); ++i)
            ret.data.setIndex(i, indicesArray[i]);
        return ret;
    }

    template <std::size_t n, typename T, class SP, typename... Args>
    const Vec<sizeof...(Args), T, vec::SwizzlerStorage<Vec<n, T, SP> > >
    swizzle(const Vec<n, T, SP>& v, const Args&... indices) {
        const std::size_t indicesArray[] = { indices... };
        Vec<sizeof...(Args), T, vec::SwizzlerStorage<Vec<n, T, SP> > > ret;
        ret.data.setHost(&v);
        for (std::size_t i = 0; i < sizeof...(indices); ++i)
            ret.data.setIndex(i, indicesArray[i]);
        return ret;
    }

} // namespace oglml

#endif // OGLML_VEC_FUNC_HPP
