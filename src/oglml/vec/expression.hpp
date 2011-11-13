#ifndef OGLML_VEC_EXPRESSION_HPP
#define OGLML_VEC_EXPRESSION_HPP

#include <type_traits>
#include <cstddef>
#include <cassert>

#include <oglml/helpers/compilerinfo.hpp>
#include <oglml/helpers/generate.hpp>

namespace oglml {

    namespace vec {
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

        template <class Op, typename Tlhs, typename Trhs>
        struct MergeVecs {
            oglml_constexpr static std::size_t n =
                    merge::MergeDimensions<Tlhs::n, Trhs::n>::result;
            typedef typename merge::MergeTypes<Op, typename Tlhs::T, typename Trhs::T>::Result  T;
            typedef T ReturnT;
            typedef T ConstReturnT;
        };

        template <class Op, typename Tlhs, typename Trhs>
        struct MergeVecAndType {
            oglml_constexpr static std::size_t n = Tlhs::n;
            typedef typename merge::MergeTypes<Op, typename Tlhs::T, Trhs>::Result T;
            typedef T ReturnT;
            typedef T ConstReturnT;
        };

        template <class Op, typename Tlhs, typename Trhs>
        struct MergeTypeAndVec {
            oglml_constexpr static std::size_t n = Trhs::n;
            typedef typename merge::MergeTypes<Op, Tlhs, typename Trhs::T>::Result T;
            typedef T ReturnT;
            typedef T ConstReturnT;
        };

        // Storage policies
        template <class Op, class Tlhs, class Trhs>
        struct ExpressionStorage {

            template <std::size_t n, typename T>
            class Container {
                typedef MergeVecs<Op, Tlhs, Trhs> Merged;

                const Tlhs* mLhs;
                const Trhs* mRhs;

            public:
                // Typedefs
                typedef typename Merged::ReturnT ReturnT;
                typedef typename Merged::ReturnT ConstReturnT;

                // Basic
                const ReturnT operator[](std::size_t i) const
                { assert(i < Merged::n); return Op::run(((*mLhs)[i]), (*mRhs)[i]); }

                // Extensions
                void init(const Tlhs& lhs, const Trhs& rhs)
                { mLhs = &lhs; mRhs = &rhs; }
            };

        };

        template <class Op, class Tlhs, class Trhs>
        struct LhsExpressionStorage {

            template <std::size_t n, typename T>
            class Container {
                typedef MergeVecAndType<Op, Tlhs, Trhs> Merged;

                const Tlhs* mLhs;
                const Trhs* mRhs;

            public:
                // Typedefs
                typedef typename Merged::ReturnT ReturnT;
                typedef typename Merged::ReturnT ConstReturnT;

                // Basic
                const ReturnT operator[](std::size_t i) const
                { assert(i < Merged::n); return Op::run(((*mLhs)[i]), (*mRhs)); }

                // Extensions
                void init(const Tlhs& lhs, const Trhs& rhs)
                { mLhs = &lhs; mRhs = &rhs; }
            };

        };

        template <class Op, class Tlhs, class Trhs>
        struct RhsExpressionStorage {

            template <std::size_t n, typename T>
            class Container {
                typedef MergeTypeAndVec<Op, Tlhs, Trhs> Merged;

                const Tlhs* mLhs;
                const Trhs* mRhs;

            public:
                // Typedefs
                typedef typename Merged::ReturnT ReturnT;
                typedef typename Merged::ReturnT ConstReturnT;

                // Basic
                const ReturnT operator[](std::size_t i) const
                { assert(i < Merged::n); return Op::run((*mLhs), (*mRhs)[i]); }

                // Extensions
                void init(const Tlhs& lhs, const Trhs& rhs)
                { mLhs = &lhs; mRhs = &rhs; }
            };

        };

    } // namespace vec

    // Template to create Vecs based on expressions
    template <class Op, class Tlhs, class Trhs>
    struct CreateExpressionVec {
        typedef typename vec::MergeVecs<Op, Tlhs, Trhs> Merged;
        typedef Vec<Merged::n, typename Merged::T,
        vec::ExpressionStorage<Op, Tlhs, Trhs> > Result;
    };

    template <class Op, class Tlhs, class Trhs>
    struct CreateLhsExpressionVec {
        typedef typename vec::MergeVecAndType<Op, Tlhs, Trhs> Merged;
        typedef Vec<Merged::n, typename Merged::T,
        vec::LhsExpressionStorage<Op, Tlhs, Trhs> > Result;
    };

    template <class Op, class Tlhs, class Trhs>
    struct CreateRhsExpressionVec {
        typedef typename vec::MergeTypeAndVec<Op, Tlhs, Trhs> Merged;
        typedef Vec<Merged::n, typename Merged::T,
        vec::RhsExpressionStorage<Op, Tlhs, Trhs> > Result;
    };

} // namespace oglml

#endif // OGLML_VEC_EXPRESSION_HPP
