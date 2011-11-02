#ifndef OGLML_VEC_HPP
#define OGLML_VEC_HPP

#include <iostream>
#include <memory>
#include <tuple>
#include <cassert>

#include <oglml/helpers/select.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/generate.hpp>
#include <oglml/helpers/errors.hpp>
#include <oglml/helpers/traits.hpp>
#include <oglml/helpers/constexpr.hpp>
#include <oglml/helpers/autoreturn.hpp>
#include <oglml/helpers/compilerinfo.hpp>

namespace oglml {

    namespace vec {
        // Storage policies forward declaration
        struct DefaultStorage;

        enum Alignment {
            Aligned = true, NotAligned = false
        };

        enum Constness {
            Const = true, NonConst = false
        };

        template <Alignment al = Aligned, Constness con = NonConst>
        struct PointerStorage;

        template <Constness con = NonConst>
        struct ArrayPtrStorage;

        template <class Op,
                  std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
                  std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
        struct ExpressionStorage;

        template <class Op, std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
        struct LhsExpressionStorage;
    }

    // Forward declaration
    template <std::size_t n, typename T, class SP = vec::DefaultStorage>
    struct Vec;

    namespace vec {
        struct BaseExpression { };

        // Expression-Template
        template <std::size_t n, class Derived, typename ReturnType, typename ConstReturnType>
        class Expression : public BaseExpression {
        public:
            ReturnType operator[](std::size_t i)
            { return downCast()[i]; }

            ConstReturnType operator[](std::size_t i) const
            { return downCast()[i]; }

            template <std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
            void swap(Expression<nrhs, Drhs, Rrhs, CRrhs>& ex)
            { downCast().swap(ex); }

        private:
            Derived& downCast()
            { return *static_cast<Derived*>(this); }

            const Derived& downCast() const
            { return *static_cast<const Derived*>(this); }
        };


        // Storage policies
        struct DefaultStorage {

            oglml_constexpr static bool aligned = true;

            template <std::size_t n, typename T>
            class Container {
            public:

                typedef T& ReturnType;
                typedef const T& ConstReturnType;

                ReturnType operator[](std::size_t i)
                { assert(i < n); return mData[i]; }

                ConstReturnType operator[](std::size_t i) const
                { assert(i < n); return mData[i]; }

                template <std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
                void swap(Expression<nrhs, Drhs, Rrhs, CRrhs>& ex) {
                    dimAssert<nrhs, n>();
                    T temp[n];
                    priv::assignArray<0, n>(temp, *this);
                    priv::assignArray<0, n>(*this, ex);
                    priv::assignArray<0, n>(ex, temp);
                }

                /*template <std::size_t nrhs, typename Trhs>
                void swap(Vec<nrhs, Trhs, DefaultStorage>& rhs) {
                    std::cout << "yes" << std::endl;
                    std::swap(mData, rhs.data.mData);
                }*/

            private:
                T mData[n];
            };

        };

        template <Alignment al, Constness con>
        struct PointerStorage {

            oglml_constexpr static bool aligned = al;

            template <std::size_t n, typename T>
            class Container {
            public:

                typedef T& ReturnType;
                typedef const T& ConstReturnType;
                typedef std::shared_ptr<typename Select<con, const T, T>::Result> PointerType;

                ReturnType operator[](std::size_t i)
                { assert(i < n); return mData.get()[i]; }

                ConstReturnType operator[](std::size_t i) const
                { assert(i < n); return mData.get()[i]; }

                void setPointer(const std::shared_ptr<T>& ptr)
                { static_assert(!con, "Pointer must be non-constant."); mData = ptr; }

                void setPointer(const std::shared_ptr<const T>& ptr)
                { static_assert(con, "Pointer must be constant."); mData = ptr; }

            private:
                PointerType mData;
            };

        };

        template <Constness con>
        struct ArrayPtrStorage {

            oglml_constexpr static bool aligned = true;

            template <std::size_t n, typename T>
            class Container {
            public:

                typedef T& ReturnType;
                typedef const T& ConstReturnType;
                typedef typename Select<con, const T*, T*>::Result PtrType;

                ReturnType operator[](std::size_t i)
                { assert(i < n); return mData[i]; }

                ConstReturnType operator[](std::size_t i) const
                { assert(i < n); return mData[i]; }

                void setPointer(T* ptr)
                { mData = ptr; }

                void setPointer(const T* ptr)
                { mData = ptr; }

            private:
                PtrType mData;
            };

        };

        template <class Op,
                  std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
                  std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
        struct ExpressionStorage : private oglml::DimProcessable<nlhs, nrhs> {
            oglml_constexpr static bool aligned = false;

            template <std::size_t n, typename T>
            class Container {
            public:
                typedef decltype(Op::run(priv::generate<CRlhs>(), priv::generate<CRrhs>())) ReturnType;
                typedef const ReturnType ConstReturnType;

                template <typename CR, typename Tv, int ti>
                static CR access(const Tv& obj, std::size_t i, priv::Int2Type<ti>)
                { return obj[i]; }

                template <typename CR, typename Tv>
                static CR access(const Tv& obj, std::size_t, priv::Int2Type<1>)
                { return obj[0]; }

                ReturnType operator[](std::size_t i) const
                { return Op::run(access<CRlhs>(*mLhs, i, priv::Int2Type<nlhs>()), access<CRrhs>(*mRhs, i, priv::Int2Type<nrhs>())); }

                void initialize(const Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
                                const Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs)
                { mLhs = &lhs; mRhs = &rhs; }

            private:
                const Expression<nlhs, Dlhs, Rlhs, CRlhs>* mLhs;
                const Expression<nrhs, Drhs, Rrhs, CRrhs>* mRhs;
            };

        };

        template <class Op,
                  std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
        struct LhsExpressionStorage {
            oglml_constexpr static bool aligned = false;

            template <std::size_t n, typename T>
            class Container {
            public:
                typedef decltype(Op::run(priv::generate<CRlhs>(), priv::generate<Trhs>())) ReturnType;
                typedef const ReturnType ConstReturnType;

                ReturnType operator[](std::size_t i) const
                { return Op::run((*lhs)[i], *rhs); }

                void initialize(const Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
                                const Trhs& rhs)
                { this->lhs = &lhs; this->rhs = &rhs; }

            private:
                const Expression<nlhs, Dlhs, Rlhs, CRlhs>* lhs;
                const Trhs* rhs;
            };

        };

        template <class Op, typename Tlhs,
                  std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
        struct RhsExpressionStorage {
            oglml_constexpr static bool aligned = false;

            template <std::size_t n, typename T>
            class Container {
            public:
                typedef decltype(Op::run(priv::generate<Tlhs>(), priv::generate<CRrhs>())) ReturnType;
                typedef const ReturnType ConstReturnType;

                ReturnType operator[](std::size_t i) const
                { return Op::run(*lhs, (*rhs)[i]); }

                void initialize(const Tlhs& lhs,
                                const Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs)
                { this->lhs = &lhs; this->rhs = &rhs; }

            private:
                const Tlhs* lhs;
                const Expression<nrhs, Drhs, Rrhs, CRrhs>* rhs;
            };

        };

        // Only for Swizzler
        namespace swizzler {

            // Find index
            template <std::size_t n>
            struct Found {
                oglml_constexpr static bool found = true;
                oglml_constexpr static std::size_t index = n;
            };

            template <std::size_t...>
            struct FindIndex { };

            template <std::size_t idx, std::size_t passed, std::size_t... indices>
            struct FindIndex<idx, passed, indices...> {
                oglml_constexpr static bool found = false;
                oglml_constexpr static std::size_t index = 0;
            };

            template <std::size_t idx, std::size_t passed, std::size_t first, std::size_t... indices>
            struct FindIndex<idx, passed, first, indices...> {
                oglml_constexpr static bool hereFound = (idx == first);

                oglml_constexpr static bool found =
                        Select<hereFound, Found<passed>,
                FindIndex<idx, passed + 1, indices...> >::Result::found;

                oglml_constexpr static std::size_t index =
                        Select<hereFound, Found<passed>,
                FindIndex<idx, passed + 1, indices...> >::Result::index;
            };

            // Access
            template <std::size_t...>
            struct Access { };

            template <std::size_t idx, std::size_t first, std::size_t... args>
            struct Access<idx, first, args...> {
                oglml_constexpr static std::size_t result = Access<idx - 1, args...>::result;
            };

            template <std::size_t first, std::size_t... args>
            struct Access<0, first, args...> {
                oglml_constexpr static std::size_t result = first;
            };

            // Find Backwards
            template <std::size_t n>
            struct FoundBackwards {
                oglml_constexpr static bool found = true;
                oglml_constexpr static std::size_t index = n;
            };

            template <std::size_t idx, std::size_t n, std::size_t... indices>
            struct FindIndexBackwards {
                oglml_constexpr static std::size_t thisIdx = Access<n, indices...>::result;
                oglml_constexpr static bool hereFound = (idx == thisIdx);
                oglml_constexpr static bool found =
                        Select<hereFound, FoundBackwards<n>,
                FindIndexBackwards<idx, n - 1, indices...> >::Result::found;
                oglml_constexpr static std::size_t index = Select<hereFound, FoundBackwards<n>,
                FindIndexBackwards<idx, n - 1, indices...> >::Result::index;
            };

            template <std::size_t idx, std::size_t... indices>
            struct FindIndexBackwards<idx, 0, indices...> {
                oglml_constexpr static bool found = false;
                oglml_constexpr static std::size_t index = 0;
            };

            // Find duplications
            namespace cdupl {

                struct DuplicateFound {
                    oglml_constexpr static bool found = true;
                };

                template <std::size_t n, std::size_t... indices>
                struct SearchDuplicates {
                    oglml_constexpr static std::size_t thisIdx = Access<sizeof...(indices) - n, indices...>::result;
                    oglml_constexpr static bool thisFound =
                            (FindIndex<thisIdx, 0, indices...>::found &&
                             FindIndexBackwards<thisIdx, sizeof...(indices) - 1, indices...>::found) &&
                            (FindIndex<thisIdx, 0, indices...>::index !=
                            FindIndexBackwards<thisIdx, sizeof...(indices) - 1, indices...>::index);

                    oglml_constexpr static bool found = Select<thisFound,
                    DuplicateFound, SearchDuplicates<n - 1, indices...>>::Result::found;
                };

                template <std::size_t... indices>
                struct SearchDuplicates<0, indices...> {
                    oglml_constexpr static bool found = false;
                };

            } // namespace cdupl

            template <std::size_t... indices>
            struct ContainsDuplicates {
                oglml_constexpr static bool result =
                        cdupl::SearchDuplicates<sizeof...(indices) - 1, indices...>::found;
            };

            // Validate indices
            struct Fail {
                oglml_constexpr static bool passed = false;
            };

            template <std::size_t n, std::size_t... indices>
            struct Check {
                oglml_constexpr static bool passed = true;
            };

            template <std::size_t n, std::size_t first, std::size_t... indices>
            struct Check<n, first, indices...> {
                oglml_constexpr static bool thisPassed = first < n;
                oglml_constexpr static bool passed = Select<thisPassed, Check<n, indices...>, Fail>::Result::passed;
            };

        } // namespace swizzler

        // Swizzler definition
        template <std::size_t n, typename T, class SP, std::size_t... indices>
        class Swizzler : public Expression<sizeof...(indices), Swizzler<n, T, SP, indices...>,
                typename SP::template Container<n, T>::ReturnType,
                typename SP::template Container<n, T>::ConstReturnType> {
        public:
            // Check template parameters
            static_assert(n > 0, "The host vector must have at least one dimension.");
            static_assert(sizeof...(indices) > 0, "At least one index must be given.");
            static_assert(!swizzler::ContainsDuplicates<indices...>::result, "The indices contain duplicates.");

            // Typedefs
            typedef typename SP::template Container<n, T> Host;
            typedef typename Host::ReturnType ReturnType;
            typedef typename Host::ConstReturnType ConstReturnType;

            // Friends
            template <std::size_t, typename, class>
            friend struct Vec;

            // Helper funcs
        private:
            Host& host()
            { return *reinterpret_cast<Host*>(this); }

            const Host& host() const
            { return *reinterpret_cast<const Host*>(this); }
        public:

            // Constants
            oglml_constexpr static bool aligned = SP::aligned;
            oglml_constexpr static bool valid = swizzler::Check<n, indices...>::passed;
            oglml_constexpr static std::size_t nIndices = sizeof...(indices);

            // Helpers
            static void validate()
            { static_assert(valid, "Indices are not valid."); }

            // Indices
            static const std::size_t indexArray[nIndices];

            // Get indices
            static std::size_t index(std::size_t i)
            { assert(i < nIndices); validate(); return indexArray[i]; }

            // Index operator
            ReturnType operator[](std::size_t i)
            { return host()[index(i)]; }

            ConstReturnType operator[](std::size_t i) const
            { return host()[index(i)]; }

            // Cast operator
            operator ReturnType()
            { dimAssert<nIndices, 1>(); return host()[index(0)]; }

            operator ConstReturnType() const
            { dimAssert<nIndices, 1>(); return host()[index(0)]; }

        private:
            Swizzler() { }
            Swizzler(const Swizzler&) { }
        };

        template <std::size_t n, typename T, class SP, std::size_t... indices>
        const std::size_t Swizzler<n, T, SP, indices...>::indexArray[nIndices] = { indices... };

    } // namespace vec

    // Global operators
    // Helper
    template <std::size_t n1, std::size_t n2>
    struct Max {
        oglml_constexpr static std::size_t result = n1 > n2 ? n1 : n2;
    };

    // expression OP expression
    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    const Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() + priv::generate<CRrhs>()),
    vec::ExpressionStorage<Plus,
    nlhs, Dlhs, Rlhs, CRlhs,
    nrhs, Drhs, Rrhs, CRrhs> > operator+
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
     const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() + priv::generate<CRrhs>()),
                vec::ExpressionStorage<Plus, nlhs, Dlhs, Rlhs, CRlhs,
                nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    const Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() - priv::generate<CRrhs>()),
    vec::ExpressionStorage<Minus,
    nlhs, Dlhs, Rlhs, CRlhs,
    nrhs, Drhs, Rrhs, CRrhs> > operator-
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
     const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() - priv::generate<CRrhs>()),
                vec::ExpressionStorage<Minus, nlhs, Dlhs, Rlhs, CRlhs,
                nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    const Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() * priv::generate<CRrhs>()),
    vec::ExpressionStorage<Multiplies,
    nlhs, Dlhs, Rlhs, CRlhs,
    nrhs, Drhs, Rrhs, CRrhs> > operator*
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
     const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() * priv::generate<CRrhs>()),
                vec::ExpressionStorage<Multiplies, nlhs, Dlhs, Rlhs, CRlhs,
                nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    const Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() / priv::generate<CRrhs>()),
    vec::ExpressionStorage<Divides,
    nlhs, Dlhs, Rlhs, CRlhs,
    nrhs, Drhs, Rrhs, CRrhs> > operator/
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
     const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<Max<nlhs, nrhs>::result, decltype(priv::generate<CRlhs>() / priv::generate<CRrhs>()),
                vec::ExpressionStorage<Divides, nlhs, Dlhs, Rlhs, CRlhs,
                nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    // expression OP value
    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
    typename priv::GetFirst<const Vec<nlhs, decltype(priv::generate<CRlhs>() + priv::generate<Trhs>()),
    vec::LhsExpressionStorage<Plus, nlhs, Dlhs, Rlhs, CRlhs, Trhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Trhs>::value>::type>::Result operator+
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs, const Trhs& rhs) {
        Vec<nlhs, decltype(priv::generate<CRlhs>() + priv::generate<Trhs>()),
            vec::LhsExpressionStorage<Plus, nlhs, Dlhs, Rlhs, CRlhs, Trhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
    typename priv::GetFirst<const Vec<nlhs, decltype(priv::generate<CRlhs>() - priv::generate<Trhs>()),
    vec::LhsExpressionStorage<Minus, nlhs, Dlhs, Rlhs, CRlhs, Trhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Trhs>::value>::type>::Result operator-
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs, const Trhs& rhs) {
        Vec<nlhs, decltype(priv::generate<CRlhs>() - priv::generate<Trhs>()),
            vec::LhsExpressionStorage<Minus, nlhs, Dlhs, Rlhs, CRlhs, Trhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
    typename priv::GetFirst<const Vec<nlhs, decltype(priv::generate<CRlhs>() * priv::generate<Trhs>()),
    vec::LhsExpressionStorage<Multiplies, nlhs, Dlhs, Rlhs, CRlhs, Trhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Trhs>::value>::type>::Result operator*
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs, const Trhs& rhs) {
        Vec<nlhs, decltype(priv::generate<CRlhs>() * priv::generate<Trhs>()),
            vec::LhsExpressionStorage<Multiplies, nlhs, Dlhs, Rlhs, CRlhs, Trhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs, typename Trhs>
    typename priv::GetFirst<const Vec<nlhs, decltype(priv::generate<CRlhs>() / priv::generate<Trhs>()),
    vec::LhsExpressionStorage<Divides, nlhs, Dlhs, Rlhs, CRlhs, Trhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Trhs>::value>::type>::Result operator/
    (const vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs, const Trhs& rhs) {
        Vec<nlhs, decltype(priv::generate<CRlhs>() / priv::generate<Trhs>()),
            vec::LhsExpressionStorage<Divides, nlhs, Dlhs, Rlhs, CRlhs, Trhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    // value OP expression
    template <typename Tlhs, std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    typename priv::GetFirst<const Vec<nrhs, decltype(priv::generate<Tlhs>() + priv::generate<CRrhs>()),
    vec::RhsExpressionStorage<Plus, Tlhs, nrhs, Drhs, Rrhs, CRrhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Tlhs>::value>::type>::Result operator+
    (const Tlhs& lhs, const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<nrhs, decltype(priv::generate<Tlhs>() + priv::generate<CRrhs>()),
            vec::RhsExpressionStorage<Plus, Tlhs, nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <typename Tlhs, std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    typename priv::GetFirst<const Vec<nrhs, decltype(priv::generate<Tlhs>() - priv::generate<CRrhs>()),
    vec::RhsExpressionStorage<Minus, Tlhs, nrhs, Drhs, Rrhs, CRrhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Tlhs>::value>::type>::Result operator-
    (const Tlhs& lhs, const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<nrhs, decltype(priv::generate<Tlhs>() - priv::generate<CRrhs>()),
            vec::RhsExpressionStorage<Minus, Tlhs, nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <typename Tlhs, std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    typename priv::GetFirst<const Vec<nrhs, decltype(priv::generate<Tlhs>() * priv::generate<CRrhs>()),
    vec::RhsExpressionStorage<Multiplies, Tlhs, nrhs, Drhs, Rrhs, CRrhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Tlhs>::value>::type>::Result operator*
    (const Tlhs& lhs, const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<nrhs, decltype(priv::generate<Tlhs>() * priv::generate<CRrhs>()),
            vec::RhsExpressionStorage<Multiplies, Tlhs, nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    template <typename Tlhs, std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    typename priv::GetFirst<const Vec<nrhs, decltype(priv::generate<Tlhs>() / priv::generate<CRrhs>()),
    vec::RhsExpressionStorage<Divides, Tlhs, nrhs, Drhs, Rrhs, CRrhs> >,
    typename std::enable_if<!std::is_base_of<vec::BaseExpression, Tlhs>::value>::type>::Result operator/
    (const Tlhs& lhs, const vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        Vec<nrhs, decltype(priv::generate<Tlhs>() / priv::generate<CRrhs>()),
            vec::RhsExpressionStorage<Divides, Tlhs, nrhs, Drhs, Rrhs, CRrhs> > vex;
        vex.data.initialize(lhs, rhs);
        return vex;
    }

    namespace vec {
        // Make operators visible for Swizzler
        using oglml::operator+;
        using oglml::operator-;
        using oglml::operator*;
        using oglml::operator/;
    }

    // Vector cast
    template <std::size_t n, typename T>
    inline const Vec<n, T, vec::PointerStorage<vec::Aligned, vec::Const> > vector_cast
    (const std::shared_ptr<const T>& ptr) {
        Vec<n, T, vec::PointerStorage<vec::Aligned, vec::Const> > v;
        v.data.setPointer(ptr);
        return v;
    }

    template <std::size_t n, typename T>
    inline Vec<n, T, vec::PointerStorage<vec::Aligned> > vector_cast(const std::shared_ptr<T>& ptr) {
        Vec<n, T, vec::PointerStorage<vec::Aligned> > v;
        v.data.setPointer(ptr);
        return v;
    }

    // Pointer cast
    /*template <std::size_t n, class D, typename R, typename CR>
    inline R* pointer_cast(vec::Expression<n, D, R, CR>& ex) {
        static_assert(ex.aligned, OGLML_ALIGNED_ERROR_MSG_);
    }*/

    /*template <std::size_t n, typename T>
    inline const typename priv::GetFirst<Vec<n, T>,
    typename std::enable_if<std::is_pointer<T>::value>::type>::Result vector_cast(const T& ptr) {
        Vec<n, typename std::remove_pointer<T>::type> v;
        return priv::assignArray<0, n>(v, ptr);
    }*/

    /*template <std::size_t n, typename T>
    inline Vec<n, typename std::remove_pointer<T>::type, vec::ArrayPtrStorage<> > vector_cast(T array) {
        Vec<n, typename std::remove_pointer<T>::type, vec::ArrayPtrStorage<> > v;
        v.data.setPointer(array);
        return v;
    }*/


    // Global helper funcs
    // Assign
    namespace assignment {

        template <std::size_t begin>
        struct Assignment {

            // Using SFINAE
            template <std::size_t n, class D, typename R, typename CR, typename... Args>
            static typename std::enable_if<sizeof...(Args) == 0>::type run
            (const vec::Expression<n, D, R, CR>&, const Args&...) {
                static_assert(begin == n, "Not enough assignment parameters.");
            }

            template <std::size_t ne, class De, typename Re, typename CRe,
                      std::size_t nv, class Dv, typename Rv, typename CRv, typename... Args>
            static void run(vec::Expression<ne, De, Re, CRe>& ex,
                            const vec::Expression<nv, Dv, Rv, CRv>& v, const Args&... args) {
                static_assert((begin + nv) <= ne, "Too many assignment parameters.");
                Assignment<begin + nv>::run(priv::assignArray<begin, nv>(ex, v), args...);
            }

            template <std::size_t ne, class De, typename Re, typename CRe, typename First, typename... Args>
            static typename std::enable_if<!std::is_base_of<vec::BaseExpression, First>::value>::type run
            (vec::Expression<ne, De, Re, CRe>& ex, const First& first, const Args&... args) {
                static_assert((begin + 1) <= ne, "Too many assignment parameters.");
                ex[begin] = first;
                Assignment<begin + 1>::run(ex, args...);
            }

        };

    }

    template <std::size_t n, class D, typename R, typename CR, typename First, typename... Args>
    vec::Expression<n, D, R, CR>& assign(vec::Expression<n, D, R, CR>& ex, const First& first, const Args&... args) {
        assignment::Assignment<0>::run(ex, first, args...);
        return ex;
    }

    // Print
    template <std::size_t n, class D, typename R, typename CR>
    void print(const vec::Expression<n, D, R, CR>& e) {
        for (std::size_t i = 0; i < n; ++i) {
            std::cout << e[i];
            if (i < n - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    // Swap
    template <std::size_t nlhs, class Dlhs, typename Rlhs, typename CRlhs,
              std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
    void swap(vec::Expression<nlhs, Dlhs, Rlhs, CRlhs>& lhs,
              vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& rhs) {
        lhs.swap(rhs);
    }

    // Vec definition
    struct BaseVec { };

    template <std::size_t n, typename T, class SP>
    struct Vec : public BaseVec, public vec::Expression<n, Vec<n, T, SP>,
            typename SP::template Container<n, T>::ReturnType,
            typename SP::template Container<n, T>::ConstReturnType> {
        // Typedefs
        typedef typename SP::template Container<n, T> Container;
        typedef typename Container::ReturnType ReturnType;
        typedef typename Container::ConstReturnType ConstReturnType;
        typedef Vec<n, T, SP> ThisType;
        typedef vec::Expression<n, Vec<n, T, SP>, ReturnType, ConstReturnType>
        ExpressionType;

        // Constants
        oglml_constexpr static bool aligned = Container::aligned;

        // Helper to declare swizzlers
        template <std::size_t... indices> struct Swizzler
        { typedef vec::Swizzler<n, T, SP, indices...> Type; };

        // Ctor and dtor
        Vec() { }

        Vec(const Vec& rhs) : data(rhs.data) { }

        template <typename First, typename... Args>
        Vec(const First& first, const Args&... args)
        { assign(*this, first, args...); }

        ~Vec() { }

        // Index operator
        ReturnType operator[](std::size_t i)
        { return data[i]; }

        ConstReturnType operator[](std::size_t i) const
        { return data[i]; }

        // Assignment operator
        template <typename Trhs>
        ExpressionType& operator=(const Trhs& rhs)
        { return assign(*this, rhs); }

        // Swap
        template <std::size_t nrhs, class Drhs, typename Rrhs, typename CRrhs>
        void swap(vec::Expression<nrhs, Drhs, Rrhs, CRrhs>& ex)
        { data.swap(ex); }

        // Data
        union {
            Container data;

#ifdef OGLML_CXX11_UNRESTRICTED_UNIONS
            // Swizzle masks
            typename Swizzler<0>::Type x;
            typename Swizzler<1>::Type y;
            typename Swizzler<0, 1>::Type xy;
            typename Swizzler<1, 0>::Type yx;
            typename Swizzler<0, 1, 2>::Type xyz;
            typename Swizzler<2, 1, 0>::Type zyx;
#endif // OGLML_CXX11_UNRESTRICTED_UNIONS
        };
    };

    // GLSL Types
    // bool
    typedef Vec<2, bool> bvec2;
    typedef Vec<3, bool> bvec3;
    typedef Vec<4, bool> bvec4;

    // integer
    typedef Vec<2, int> ivec2;
    typedef Vec<3, int> ivec3;
    typedef Vec<4, int> ivec4;

    // unsigned int
    typedef Vec<2, unsigned int> uvec2;
    typedef Vec<3, unsigned int> uvec3;
    typedef Vec<4, unsigned int> uvec4;

    // float
    typedef Vec<2, float> vec2;
    typedef Vec<3, float> vec3;
    typedef Vec<4, float> vec4;

    // double
    typedef Vec<2, double> dvec2;
    typedef Vec<3, double> dvec3;
    typedef Vec<4, double> dvec4;

} // namespace oglml

#endif // OGLML_VEC_HPP
