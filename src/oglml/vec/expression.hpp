#ifndef OGLML_VEC_EXPRESSION_HPP
#define OGLML_VEC_EXPRESSION_HPP

#include <type_traits>
#include <cstddef>
#include <cassert>

#include <oglml/vec/info.hpp>
#include <oglml/helpers/compilerinfo.hpp>
#include <oglml/helpers/generate.hpp>

namespace oglml {

    namespace vec {

        // Belongs to Expression
        namespace expression {
            // Basic struct to collect data
            template <std::size_t tn, typename tT, class tDerived, typename tReturnT, typename tConstReturnT>
            struct Info {
                oglml_constexpr static std::size_t n = tn;

                typedef tT T;
                typedef tDerived Derived;
                typedef tReturnT ReturnT;
                typedef tConstReturnT ConstReturnT;
            };
        } // namespace expression

        // Expression
        struct BaseExpression { };

        template <class Info>
        class Expression : public BaseExpression {
        public:
            oglml_constexpr static std::size_t n = Info::n;

            typedef typename Info::T T;
            typedef typename Info::Derived Derived;
            typedef typename Info::ReturnT ReturnT;
            typedef typename Info::ConstReturnT ConstReturnT;

        private:
            // Helper meths to downcast
            Derived& down()
            { return *static_cast<Derived*>(this); }

            const Derived& down() const
            { return *static_cast<const Derived*>(this); }

        public:

            // Every derived has to implement the index operators
            ReturnT operator[](std::size_t i)
            { return down()[i]; }

            ConstReturnT operator[](std::size_t i) const
            { return down()[i]; }

        };


        // -----------------------------------------
        // Forward declarations
        template <class, class, class>
        struct ExpressionStorage;

        template <class, class, typename>
        struct LhsExpressionStorage;

        template <class, typename, class>
        struct RhsExpressionStorage;

        // Merge info to fit
        namespace merge {

            // Merge dimensions
            template <std::size_t n1, std::size_t n2>
            struct MergeDimensions {
                static_assert(n1 == n2 || n1 == 1 || n2 == 1,
                              "Cannot merge dimensions.");
                oglml_constexpr static std::size_t result = n1 > n2 ? n1 : n2;
            };

            template <class Op, typename T1, typename T2>
            struct MergeTypes {
                typedef decltype(Op::run(T1(), T2())) Result;
            };

        } // namespace merge

        template <class Op, class I1, class I2>
        struct MergeInfo {
            typedef expression::Info<
            merge::MergeDimensions<I1::n, I2::n>::result,
            typename merge::MergeTypes<Op, typename I1::T, typename I2::T>::Result,
            ExpressionStorage<Op, typename I1::T, typename I2::T>,
            typename merge::MergeTypes<Op, typename I1::T, typename I2::T>::Result,
            typename merge::MergeTypes<Op, typename I1::T, typename I2::T>::Result
            > Result;
        };

        template <class Op, class I, typename T>
        struct MergeInfoAndType {
            typedef expression::Info<
            I::n,
            typename merge::MergeTypes<Op, typename I::T, T>::Result,
            ExpressionStorage<Op, typename I::T, T>,
            typename merge::MergeTypes<Op, typename I::T, T>::Result,
            typename merge::MergeTypes<Op, typename I::T, T>::Result
            > Result;
        };

        // Storage policies
        template <class Op, class Ilhs, class Irhs>
        struct ExpressionStorage {

            template <std::size_t n, typename T>
            class Container {
                typedef vec::Expression<Ilhs> LhsT;
                typedef vec::Expression<Irhs> RhsT;
                typedef typename MergeInfo<Op, Ilhs, Irhs>::Result Merged;

                const LhsT* mLhs;
                const RhsT* mRhs;

            public:
                // Typedefs
                typedef T ReturnT;
                typedef T ConstReturnT;

                // Basic
                ReturnT operator[](std::size_t i) const
                { assert(i < Merged::n); return Op::run(((*mLhs)[i]), (*mRhs)[i]); }

                // Extensions
                void init(const LhsT& lhs, const RhsT& rhs)
                { mLhs = &lhs; mRhs = &rhs; }
            };

        };

        template <class Op, class Ilhs, typename Trhs>
        struct LhsExpressionStorage {

            template <std::size_t n, typename T>
            class Container {
                typedef vec::Expression<Ilhs> LhsT;
                typedef Trhs RhsT;
                typedef typename MergeInfoAndType<Op, Ilhs, Trhs>::Result Merged;

                const LhsT* mLhs;
                const RhsT* mRhs;

            public:
                // Typedefs
                typedef T ReturnT;
                typedef T ConstReturnT;

                // Basic
                ReturnT operator[](std::size_t i) const
                { assert(i < Merged::n); return Op::run(((*mLhs)[i]), *mRhs); }

                // Extensions
                void init(const LhsT& lhs, const RhsT& rhs)
                { mLhs = &lhs; mRhs = &rhs; }
            };

        };

    } // namespace vec

    // Template to create Vecs based on expressions
    template <class Op, class I1, class I2>
    struct CreateExpressionVec {
        typedef typename vec::MergeInfo<Op, I1, I2>::Result MergedInfo;
        typedef Vec<MergedInfo::n, typename MergedInfo::T,
        vec::ExpressionStorage<Op, I1, I2> > Result;
    };

    template <class Op, class I, typename T>
    struct CreateLhsExpressionVec {
        typedef typename vec::MergeInfoAndType<Op, I, T>::Result MergedInfo;
        typedef Vec<MergedInfo::n, typename MergedInfo::T,
        vec::LhsExpressionStorage<Op, I, T> > Result;
    };

} // namespace oglml

#endif // OGLML_VEC_EXPRESSION_HPP
