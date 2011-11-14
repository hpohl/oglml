#ifndef OGLML_VEC_INFO_HPP
#define OGLML_VEC_INFO_HPP

#include <cstddef>

#include <oglml/vec/expression.hpp>
#include <oglml/helpers/select.hpp>
#include <oglml/helpers/traits.hpp>
#include <oglml/helpers/compilerinfo.hpp>

namespace oglml {

    // Base vec
    struct BaseVec { };

    namespace vec {
        namespace detail {

            // Available vec types
            struct BaseSwizzler { };
            struct DummyVec : public BaseVec {
                oglml_constexpr static std::size_t n = 2;
            };

            namespace isvec {

                template <bool b, typename R>
                struct Run { };

                template <typename R>
                struct Run<true, R> {
                    typedef R Result;
                };

                template <typename R, bool b>
                struct RRun { };

                template <typename R>
                struct RRun<R, true> {
                    typedef R Result;
                };

            } // namespace isvec

            template <typename R, typename T1, typename T2 = DummyVec>
            struct VecFunc : public isvec::Run<(std::is_base_of<BaseVec, T1>::value ||
                                                std::is_base_of<BaseSwizzler, T1>::value) &&
                                               (std::is_base_of<BaseVec, T2>::value ||
                                                std::is_base_of<BaseSwizzler, T2>::value), R>
            { };

            template <typename T1, typename T2 = DummyVec>
            struct IsVec {
                oglml_constexpr static bool result = (std::is_base_of<BaseVec, T1>::value ||
                                                      std::is_base_of<BaseSwizzler, T1>::value) &&
                                                     (std::is_base_of<BaseVec, T2>::value ||
                                                      std::is_base_of<BaseSwizzler, T2>::value);
            };

            template <typename T1, typename T2 = DummyVec>
            struct HasMultipleDim {
                oglml_constexpr static bool result = (T1::n > 1) && (T2::n > 1);
            };

            template <typename T1, typename T2 = DummyVec>
            struct IsMVec {
                oglml_constexpr static bool result = oglml::Select<IsVec<T1, T2>::result,
                HasMultipleDim<T1, T2>, oglml::detail::False>::Result::result;
            };


            template <typename T1, typename T2, typename R>
            struct RVecFunc : public isvec::RRun<R, (std::is_base_of<BaseVec, T1>::value ||
                                                 std::is_base_of<BaseSwizzler, T1>::value) &&
                                                (std::is_base_of<BaseVec, T2>::value ||
                                                 std::is_base_of<BaseSwizzler, T2>::value)>
             { };

        } // namespace detail
    } // namespace vec
} // namespace oglml

#endif // OGLML_VEC_INFO_HPP
