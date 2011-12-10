#ifndef OGLML_VEC_HPP
#define OGLML_VEC_HPP

#include <iostream>
#include <cstddef>

#include <oglml/vec/storagepolicies/default.hpp>
#include <oglml/vec/storagepolicies/staticswizzler.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/autoreturn.hpp>
#include <oglml/vec/funcs.hpp>
#include <oglml/vec/operators.hpp>
#include <oglml/vecfwd.hpp>

namespace oglml {
    namespace vec {
        namespace detail {

            template <std::size_t n, typename T, class Container, std::size_t index>
            class ValueView {

                Container& host()
                { return *reinterpret_cast<Container*>(this); }

                const Container& host() const
                { return *reinterpret_cast<const Container*>(this); }

                oglml_constexpr_if_available static void check()
                { static_assert(index < n, "Invalid index."); }

            public:

                operator T&()
                { check(); return host()[index]; }

                operator const T&() const
                { check(); return host()[index]; }
            };

            template <std::size_t n, typename T, class SP>
            struct SwizzlerUnion {
                typedef typename SP::template Container<n, T> Container;

                // Structs
                template <std::size_t... indices>
                struct DefineSwizzler {

                    oglml_constexpr static std::size_t nindices = sizeof...(indices);

                    typedef vec::StaticSwizzlerStorage<n, T, SP, indices...> SwizzlerSP;
                    typedef Vec<nindices, T, SwizzlerSP> Result;
                };

#define OGLML_DEFINE_SWIZZLER_FUNC_2IDX(_IDX1_, _IDX2_, _NAME_) \
    typename DefineSwizzler<_IDX1_, _IDX2_>::Result& _NAME_() { \
        typedef typename DefineSwizzler<_IDX1_, _IDX2_>::Result RetT; \
        return *reinterpret_cast<RetT*>(&data); \
    } \
    \
    const typename DefineSwizzler<_IDX1_, _IDX2_>::Result& _NAME_() const { \
        typedef typename DefineSwizzler<_IDX1_, _IDX2_>::Result RetT; \
        return *reinterpret_cast<const RetT*>(&data); \
    }

#define OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(_IDX1_, _IDX2_, _NAME1_, _NAME2_, _NAME3_) \
    OGLML_DEFINE_SWIZZLER_FUNC_2IDX(_IDX1_, _IDX2_, _NAME1_) \
    OGLML_DEFINE_SWIZZLER_FUNC_2IDX(_IDX1_, _IDX2_, _NAME2_) \
    OGLML_DEFINE_SWIZZLER_FUNC_2IDX(_IDX1_, _IDX2_, _NAME3_)

#define OGLML_DEFINE_SWIZZLER_FUNC_3IDX(_IDX1_, _IDX2_, _IDX3_, _NAME_) \
    typename DefineSwizzler<_IDX1_, _IDX2_, _IDX3_>::Result& _NAME_() { \
        typedef typename DefineSwizzler<_IDX1_, _IDX2_, _IDX3_> \
                ::Result RetT; \
        return *reinterpret_cast<RetT*>(&data); \
    } \
    \
    const typename DefineSwizzler<_IDX1_, _IDX2_, _IDX3_>::Result& \
    _NAME_() const { \
        typedef typename DefineSwizzler<_IDX1_, _IDX2_, _IDX3_> \
                ::Result RetT; \
        return *reinterpret_cast<const RetT*>(&data); \
    }

#define OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(_IDX1_, _IDX2_, _IDX3_, _NAME1_, _NAME2_, _NAME3_) \
    OGLML_DEFINE_SWIZZLER_FUNC_3IDX(_IDX1_, _IDX2_, _IDX3_, _NAME1_) \
    OGLML_DEFINE_SWIZZLER_FUNC_3IDX(_IDX1_, _IDX2_, _IDX3_, _NAME2_) \
    OGLML_DEFINE_SWIZZLER_FUNC_3IDX(_IDX1_, _IDX2_, _IDX3_, _NAME3_)

#define OGLML_DEFINE_SWIZZLER_FUNC_4IDX(_IDX1_, _IDX2_, _IDX3_, _IDX4_, _NAME_) \
    typename DefineSwizzler<_IDX1_, _IDX2_, _IDX3_, _IDX4_>::Result& \
    _NAME_() { \
        typedef typename DefineSwizzler<_IDX1_, _IDX2_, _IDX3_, _IDX4_> \
                ::Result RetT; \
        return *reinterpret_cast<RetT*>(&data); \
    } \
    \
    const typename DefineSwizzler<_IDX1_, _IDX2_, _IDX3_, _IDX4_>::Result& \
    _NAME_() const { \
        typedef typename DefineSwizzler<_IDX1_, _IDX2_, _IDX3_, _IDX4_> \
                ::Result RetT; \
        return *reinterpret_cast<const RetT*>(&data); \
    }

#define OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(_IDX1_, _IDX2_, _IDX3_, _IDX4_, _NAME1_, _NAME2_, _NAME3_) \
    OGLML_DEFINE_SWIZZLER_FUNC_4IDX(_IDX1_, _IDX2_, _IDX3_, _IDX4_, _NAME1_) \
    OGLML_DEFINE_SWIZZLER_FUNC_4IDX(_IDX1_, _IDX2_, _IDX3_, _IDX4_, _NAME2_) \
    OGLML_DEFINE_SWIZZLER_FUNC_4IDX(_IDX1_, _IDX2_, _IDX3_, _IDX4_, _NAME3_)

#ifndef OGLML_CXX11_UNRESTRICTED_UNIONS

                // Two indices
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(0, 0, xx, rr, ss)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(0, 1, xy, rg, st)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(0, 2, xz, rb, sp)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(0, 3, xw, ra, sq)

                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(1, 0, yx, gr, ts)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(1, 1, yy, gg, tt)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(1, 2, yz, gb, tp)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(1, 3, yw, ga, tq)

                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(2, 0, zx, br, ps)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(2, 1, zy, bg, pt)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(2, 2, zz, bb, pp)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(2, 3, zw, ba, pq)

                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(3, 0, wx, ar, qs)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(3, 1, wy, ag, qt)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(3, 2, wz, ab, qp)
                OGLML_DEFINE_SWIZZLER_FUNCS_2IDX(3, 3, ww, aa, qq)

                // Three indices
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 0, 0, xxx, rrr, sss)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 0, 1, xxy, rrg, sst)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 0, 2, xxz, rrb, ssp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 0, 3, xxw, rra, ssq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 1, 0, xyx, rgr, sts)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 1, 1, xyy, rgg, stt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 1, 2, xyz, rgb, stp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 1, 3, xyw, rga, stq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 2, 0, xzx, rbr, sps)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 2, 1, xzy, rbg, spt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 2, 2, xzz, rbb, spp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 2, 3, xzw, rba, spq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 3, 0, xwx, rar, sqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 3, 1, xwy, rag, sqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 3, 2, xwz, rab, sqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(0, 3, 3, xww, raa, sqq)

                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 0, 0, yxx, grr, tss)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 0, 1, yxy, grg, tst)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 0, 2, yxz, grb, tsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 0, 3, yxw, gra, tsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 1, 0, yyx, ggr, tts)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 1, 1, yyy, ggg, ttt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 1, 2, yyz, ggb, ttp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 1, 3, yyw, gga, ttq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 2, 0, yzx, gbr, tps)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 2, 1, yzy, gbg, tpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 2, 2, yzz, gbb, tpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 2, 3, yzw, gba, tpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 3, 0, ywx, gar, tqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 3, 1, ywy, gag, tqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 3, 2, ywz, gab, tqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(1, 3, 3, yww, gaa, tqq)

                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 0, 0, zxx, brr, pss)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 0, 1, zxy, brg, pst)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 0, 2, zxz, brb, psp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 0, 3, zxw, bra, psq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 1, 0, zyx, bgr, pts)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 1, 1, zyy, bgg, ptt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 1, 2, zyz, bgb, ptp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 1, 3, zyw, bga, ptq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 2, 0, zzx, bbr, pps)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 2, 1, zzy, bbg, ppt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 2, 2, zzz, bbb, ppp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 2, 3, zzw, bba, ppq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 3, 0, zwx, bar, pqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 3, 1, zwy, bag, pqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 3, 2, zwz, bab, pqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(2, 3, 3, zww, baa, pqq)

                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 0, 0, wxx, arr, qss)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 0, 1, wxy, arg, qst)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 0, 2, wxz, arb, qsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 0, 3, wxw, ara, qsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 1, 0, wyx, agr, qts)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 1, 1, wyy, agg, qtt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 1, 2, wyz, agb, qtp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 1, 3, wyw, aga, qtq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 2, 0, wzx, abr, qps)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 2, 1, wzy, abg, qpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 2, 2, wzz, abb, qpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 2, 3, wzw, aba, qpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 3, 0, wwx, aar, qqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 3, 1, wwy, aag, qqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 3, 2, wwz, aab, qqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_3IDX(3, 3, 3, www, aaa, qqq)

                // Four indices
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 0, 0, xxxx, rrrr, ssss)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 0, 1, xxxy, rrrg, ssst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 0, 2, xxxz, rrrb, sssp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 0, 3, xxxw, rrra, sssq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 1, 0, xxyx, rrgr, ssts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 1, 1, xxyy, rrgg, sstt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 1, 2, xxyz, rrgb, sstp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 1, 3, xxyw, rrga, sstq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 2, 0, xxzx, rrbr, ssps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 2, 1, xxzy, rrbg, sspt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 2, 2, xxzz, rrbb, sspp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 2, 3, xxzw, rrba, sspq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 3, 0, xxwx, rrar, ssqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 3, 1, xxwy, rrag, ssqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 3, 2, xxwz, rrab, ssqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 0, 3, 3, xxww, rraa, ssqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 0, 0, xyxx, rgrr, stss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 0, 1, xyxy, rgrg, stst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 0, 2, xyxz, rgrb, stsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 0, 3, xyxw, rgra, stsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 1, 0, xyyx, rggr, stts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 1, 1, xyyy, rggg, sttt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 1, 2, xyyz, rggb, sttp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 1, 3, xyyw, rgga, sttq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 2, 0, xyzx, rgbr, stps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 2, 1, xyzy, rgbg, stpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 2, 2, xyzz, rgbb, stpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 2, 3, xyzw, rgba, stpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 3, 0, xywx, rgar, stqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 3, 1, xywy, rgag, stqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 3, 2, xywz, rgab, stqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 1, 3, 3, xyww, rgaa, stqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 0, 0, xzxx, rbrr, spss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 0, 1, xzxy, rbrg, spst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 0, 2, xzxz, rbrb, spsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 0, 3, xzxw, rbra, spsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 1, 0, xzyx, rbgr, spts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 1, 1, xzyy, rbgg, sptt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 1, 2, xzyz, rbgb, sptp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 1, 3, xzyw, rbga, sptq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 2, 0, xzzx, rbbr, spps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 2, 1, xzzy, rbbg, sppt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 2, 2, xzzz, rbbb, sppp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 2, 3, xzzw, rbba, sppq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 3, 0, xzwx, rbar, spqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 3, 1, xzwy, rbag, spqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 3, 2, xzwz, rbab, spqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 2, 3, 3, xzww, rbaa, spqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 0, 0, xwxx, rarr, sqss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 0, 1, xwxy, rarg, sqst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 0, 2, xwxz, rarb, sqsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 0, 3, xwxw, rara, sqsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 1, 0, xwyx, ragr, sqts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 1, 1, xwyy, ragg, sqtt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 1, 2, xwyz, ragb, sqtp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 1, 3, xwyw, raga, sqtq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 2, 0, xwzx, rabr, sqps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 2, 1, xwzy, rabg, sqpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 2, 2, xwzz, rabb, sqpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 2, 3, xwzw, raba, sqpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 3, 0, xwwx, raar, sqqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 3, 1, xwwy, raag, sqqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 3, 2, xwwz, raab, sqqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(0, 3, 3, 3, xwww, raaa, sqqq)

                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 0, 0, yxxx, grrr, tsss)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 0, 1, yxxy, grrg, tsst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 0, 2, yxxz, grrb, tssp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 0, 3, yxxw, grra, tssq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 1, 0, yxyx, grgr, tsts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 1, 1, yxyy, grgg, tstt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 1, 2, yxyz, grgb, tstp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 1, 3, yxyw, grga, tstq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 2, 0, yxzx, grbr, tsps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 2, 1, yxzy, grbg, tspt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 2, 2, yxzz, grbb, tspp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 2, 3, yxzw, grba, tspq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 3, 0, yxwx, grar, tsqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 3, 1, yxwy, grag, tsqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 3, 2, yxwz, grab, tsqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 0, 3, 3, yxww, graa, tsqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 0, 0, yyxx, ggrr, ttss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 0, 1, yyxy, ggrg, ttst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 0, 2, yyxz, ggrb, ttsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 0, 3, yyxw, ggra, ttsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 1, 0, yyyx, gggr, ttts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 1, 1, yyyy, gggg, tttt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 1, 2, yyyz, gggb, tttp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 1, 3, yyyw, ggga, tttq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 2, 0, yyzx, ggbr, ttps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 2, 1, yyzy, ggbg, ttpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 2, 2, yyzz, ggbb, ttpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 2, 3, yyzw, ggba, ttpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 3, 0, yywx, ggar, ttqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 3, 1, yywy, ggag, ttqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 3, 2, yywz, ggab, ttqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 1, 3, 3, yyww, ggaa, ttqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 0, 0, yzxx, gbrr, tpss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 0, 1, yzxy, gbrg, tpst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 0, 2, yzxz, gbrb, tpsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 0, 3, yzxw, gbra, tpsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 1, 0, yzyx, gbgr, tpts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 1, 1, yzyy, gbgg, tptt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 1, 2, yzyz, gbgb, tptp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 1, 3, yzyw, gbga, tptq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 2, 0, yzzx, gbbr, tpps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 2, 1, yzzy, gbbg, tppt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 2, 2, yzzz, gbbb, tppp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 2, 3, yzzw, gbba, tppq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 3, 0, yzwx, gbar, tpqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 3, 1, yzwy, gbag, tpqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 3, 2, yzwz, gbab, tpqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 2, 3, 3, yzww, gbaa, tpqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 0, 0, ywxx, garr, tqss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 0, 1, ywxy, garg, tqst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 0, 2, ywxz, garb, tqsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 0, 3, ywxw, gara, tqsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 1, 0, ywyx, gagr, tqts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 1, 1, ywyy, gagg, tqtt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 1, 2, ywyz, gagb, tqtp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 1, 3, ywyw, gaga, tqtq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 2, 0, ywzx, gabr, tqps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 2, 1, ywzy, gabg, tqpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 2, 2, ywzz, gabb, tqpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 2, 3, ywzw, gaba, tqpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 3, 0, ywwx, gaar, tqqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 3, 1, ywwy, gaag, tqqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 3, 2, ywwz, gaab, tqqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(1, 3, 3, 3, ywww, gaaa, tqqq)

                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 0, 0, zxxx, brrr, psss)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 0, 1, zxxy, brrg, psst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 0, 2, zxxz, brrb, pssp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 0, 3, zxxw, brra, pssq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 1, 0, zxyx, brgr, psts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 1, 1, zxyy, brgg, pstt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 1, 2, zxyz, brgb, pstp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 1, 3, zxyw, brga, pstq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 2, 0, zxzx, brbr, psps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 2, 1, zxzy, brbg, pspt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 2, 2, zxzz, brbb, pspp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 2, 3, zxzw, brba, pspq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 3, 0, zxwx, brar, psqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 3, 1, zxwy, brag, psqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 3, 2, zxwz, brab, psqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 0, 3, 3, zxww, braa, psqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 0, 0, zyxx, bgrr, ptss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 0, 1, zyxy, bgrg, ptst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 0, 2, zyxz, bgrb, ptsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 0, 3, zyxw, bgra, ptsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 1, 0, zyyx, bggr, ptts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 1, 1, zyyy, bggg, pttt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 1, 2, zyyz, bggb, pttp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 1, 3, zyyw, bgga, pttq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 2, 0, zyzx, bgbr, ptps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 2, 1, zyzy, bgbg, ptpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 2, 2, zyzz, bgbb, ptpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 2, 3, zyzw, bgba, ptpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 3, 0, zywx, bgar, ptqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 3, 1, zywy, bgag, ptqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 3, 2, zywz, bgab, ptqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 1, 3, 3, zyww, bgaa, ptqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 0, 0, zzxx, bbrr, ppss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 0, 1, zzxy, bbrg, ppst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 0, 2, zzxz, bbrb, ppsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 0, 3, zzxw, bbra, ppsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 1, 0, zzyx, bbgr, ppts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 1, 1, zzyy, bbgg, pptt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 1, 2, zzyz, bbgb, pptp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 1, 3, zzyw, bbga, pptq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 2, 0, zzzx, bbbr, ppps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 2, 1, zzzy, bbbg, pppt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 2, 2, zzzz, bbbb, pppp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 2, 3, zzzw, bbba, pppq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 3, 0, zzwx, bbar, ppqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 3, 1, zzwy, bbag, ppqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 3, 2, zzwz, bbab, ppqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 2, 3, 3, zzww, bbaa, ppqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 0, 0, zwxx, barr, pqss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 0, 1, zwxy, barg, pqst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 0, 2, zwxz, barb, pqsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 0, 3, zwxw, bara, pqsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 1, 0, zwyx, bagr, pqts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 1, 1, zwyy, bagg, pqtt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 1, 2, zwyz, bagb, pqtp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 1, 3, zwyw, baga, pqtq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 2, 0, zwzx, babr, pqps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 2, 1, zwzy, babg, pqpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 2, 2, zwzz, babb, pqpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 2, 3, zwzw, baba, pqpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 3, 0, zwwx, baar, pqqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 3, 1, zwwy, baag, pqqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 3, 2, zwwz, baab, pqqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(2, 3, 3, 3, zwww, baaa, pqqq)

                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 0, 0, wxxx, arrr, qsss)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 0, 1, wxxy, arrg, qsst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 0, 2, wxxz, arrb, qssp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 0, 3, wxxw, arra, qssq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 1, 0, wxyx, argr, qsts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 1, 1, wxyy, argg, qstt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 1, 2, wxyz, argb, qstp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 1, 3, wxyw, arga, qstq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 2, 0, wxzx, arbr, qsps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 2, 1, wxzy, arbg, qspt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 2, 2, wxzz, arbb, qspp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 2, 3, wxzw, arba, qspq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 3, 0, wxwx, arar, qsqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 3, 1, wxwy, arag, qsqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 3, 2, wxwz, arab, qsqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 0, 3, 3, wxww, araa, qsqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 0, 0, wyxx, agrr, qtss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 0, 1, wyxy, agrg, qtst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 0, 2, wyxz, agrb, qtsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 0, 3, wyxw, agra, qtsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 1, 0, wyyx, aggr, qtts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 1, 1, wyyy, aggg, qttt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 1, 2, wyyz, aggb, qttp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 1, 3, wyyw, agga, qttq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 2, 0, wyzx, agbr, qtps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 2, 1, wyzy, agbg, qtpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 2, 2, wyzz, agbb, qtpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 2, 3, wyzw, agba, qtpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 3, 0, wywx, agar, qtqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 3, 1, wywy, agag, qtqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 3, 2, wywz, agab, qtqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 1, 3, 3, wyww, agaa, qtqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 0, 0, wzxx, abrr, qpss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 0, 1, wzxy, abrg, qpst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 0, 2, wzxz, abrb, qpsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 0, 3, wzxw, abra, qpsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 1, 0, wzyx, abgr, qpts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 1, 1, wzyy, abgg, qptt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 1, 2, wzyz, abgb, qptp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 1, 3, wzyw, abga, qptq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 2, 0, wzzx, abbr, qpps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 2, 1, wzzy, abbg, qppt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 2, 2, wzzz, abbb, qppp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 2, 3, wzzw, abba, qppq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 3, 0, wzwx, abar, qpqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 3, 1, wzwy, abag, qpqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 3, 2, wzwz, abab, qpqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 2, 3, 3, wzww, abaa, qpqq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 0, 0, wwxx, aarr, qqss) // ----
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 0, 1, wwxy, aarg, qqst)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 0, 2, wwxz, aarb, qqsp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 0, 3, wwxw, aara, qqsq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 1, 0, wwyx, aagr, qqts)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 1, 1, wwyy, aagg, qqtt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 1, 2, wwyz, aagb, qqtp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 1, 3, wwyw, aaga, qqtq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 2, 0, wwzx, aabr, qqps)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 2, 1, wwzy, aabg, qqpt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 2, 2, wwzz, aabb, qqpp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 2, 3, wwzw, aaba, qqpq)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 3, 0, wwwx, aaar, qqqs)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 3, 1, wwwy, aaag, qqqt)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 3, 2, wwwz, aaab, qqqp)
                OGLML_DEFINE_SWIZZLER_FUNCS_4IDX(3, 3, 3, 3, wwww, aaaa, qqqq)

#endif // OGLML_CXX11_UNRESTRICTED_UNIONS

                // Swizzlers + data
                union {
                    // Contains the data, public accessable
                    Container data;

                    ValueView<n, T, Container, 0> x, r, s;
                    ValueView<n, T, Container, 1> y, g, t;
                    ValueView<n, T, Container, 2> z, b, p;
                    ValueView<n, T, Container, 3> w, a, q;

                    // Swizzlers
#ifdef OGLML_CXX11_UNRESTRICTED_UNIONS

                    // One index

                    // Two indices
                    typename DefineSwizzler<0, 0>::Result xx, rr, ss;
                    typename DefineSwizzler<0, 1>::Result xy, rg, st;
                    typename DefineSwizzler<0, 2>::Result xz, rb, sp;
                    typename DefineSwizzler<0, 3>::Result xw, ra, sq;

                    typename DefineSwizzler<1, 0>::Result yx, gr, ts;
                    typename DefineSwizzler<1, 1>::Result yy, gg, tt;
                    typename DefineSwizzler<1, 2>::Result yz, gb, tp;
                    typename DefineSwizzler<1, 3>::Result yw, ga, tq;

                    typename DefineSwizzler<2, 0>::Result zx, br, ps;
                    typename DefineSwizzler<2, 1>::Result zy, bg, pt;
                    typename DefineSwizzler<2, 2>::Result zz, bb, pp;
                    typename DefineSwizzler<2, 3>::Result zw, ba, pq;

                    typename DefineSwizzler<3, 0>::Result wx, ar, qs;
                    typename DefineSwizzler<3, 1>::Result wy, ag, qt;
                    typename DefineSwizzler<3, 2>::Result wz, ab, qp;
                    typename DefineSwizzler<3, 3>::Result ww, aa, qq;

                    // Three indices
                    typename DefineSwizzler<0, 0, 0>::Result xxx, rrr, sss;
                    typename DefineSwizzler<0, 0, 1>::Result xxy, rrg, sst;
                    typename DefineSwizzler<0, 0, 2>::Result xxz, rrb, ssp;
                    typename DefineSwizzler<0, 0, 3>::Result xxw, rra, ssq;
                    typename DefineSwizzler<0, 1, 0>::Result xyx, rgr, sts;
                    typename DefineSwizzler<0, 1, 1>::Result xyy, rgg, stt;
                    typename DefineSwizzler<0, 1, 2>::Result xyz, rgb, stp;
                    typename DefineSwizzler<0, 1, 3>::Result xyw, rga, stq;
                    typename DefineSwizzler<0, 2, 0>::Result xzx, rbr, sps;
                    typename DefineSwizzler<0, 2, 1>::Result xzy, rbg, spt;
                    typename DefineSwizzler<0, 2, 2>::Result xzz, rbb, spp;
                    typename DefineSwizzler<0, 2, 3>::Result xzw, rba, spq;
                    typename DefineSwizzler<0, 3, 0>::Result xwx, rar, sqs;
                    typename DefineSwizzler<0, 3, 1>::Result xwy, rag, sqt;
                    typename DefineSwizzler<0, 3, 2>::Result xwz, rab, sqp;
                    typename DefineSwizzler<0, 3, 3>::Result xww, raa, sqq;

                    typename DefineSwizzler<1, 0, 0>::Result yxx, grr, tss;
                    typename DefineSwizzler<1, 0, 1>::Result yxy, grg, tst;
                    typename DefineSwizzler<1, 0, 2>::Result yxz, grb, tsp;
                    typename DefineSwizzler<1, 0, 3>::Result yxw, gra, tsq;
                    typename DefineSwizzler<1, 1, 0>::Result yyx, ggr, tts;
                    typename DefineSwizzler<1, 1, 1>::Result yyy, ggg, ttt;
                    typename DefineSwizzler<1, 1, 2>::Result yyz, ggb, ttp;
                    typename DefineSwizzler<1, 1, 3>::Result yyw, gga, ttq;
                    typename DefineSwizzler<1, 2, 0>::Result yzx, gbr, tps;
                    typename DefineSwizzler<1, 2, 1>::Result yzy, gbg, tpt;
                    typename DefineSwizzler<1, 2, 2>::Result yzz, gbb, tpp;
                    typename DefineSwizzler<1, 2, 3>::Result yzw, gba, tpq;
                    typename DefineSwizzler<1, 3, 0>::Result ywx, gar, tqs;
                    typename DefineSwizzler<1, 3, 1>::Result ywy, gag, tqt;
                    typename DefineSwizzler<1, 3, 2>::Result ywz, gab, tqp;
                    typename DefineSwizzler<1, 3, 3>::Result yww, gaa, tqq;

                    typename DefineSwizzler<2, 0, 0>::Result zxx, brr, pss;
                    typename DefineSwizzler<2, 0, 1>::Result zxy, brg, pst;
                    typename DefineSwizzler<2, 0, 2>::Result zxz, brb, psp;
                    typename DefineSwizzler<2, 0, 3>::Result zxw, bra, psq;
                    typename DefineSwizzler<2, 1, 0>::Result zyx, bgr, pts;
                    typename DefineSwizzler<2, 1, 1>::Result zyy, bgg, ptt;
                    typename DefineSwizzler<2, 1, 2>::Result zyz, bgb, ptp;
                    typename DefineSwizzler<2, 1, 3>::Result zyw, bga, ptq;
                    typename DefineSwizzler<2, 2, 0>::Result zzx, bbr, pps;
                    typename DefineSwizzler<2, 2, 1>::Result zzy, bbg, ppt;
                    typename DefineSwizzler<2, 2, 2>::Result zzz, bbb, ppp;
                    typename DefineSwizzler<2, 2, 3>::Result zzw, bba, ppq;
                    typename DefineSwizzler<2, 3, 0>::Result zwx, bar, pqs;
                    typename DefineSwizzler<2, 3, 1>::Result zwy, bag, pqt;
                    typename DefineSwizzler<2, 3, 2>::Result zwz, bab, pqp;
                    typename DefineSwizzler<2, 3, 3>::Result zww, baa, pqq;

                    typename DefineSwizzler<3, 0, 0>::Result wxx, arr, qss;
                    typename DefineSwizzler<3, 0, 1>::Result wxy, arg, qst;
                    typename DefineSwizzler<3, 0, 2>::Result wxz, arb, qsp;
                    typename DefineSwizzler<3, 0, 3>::Result wxw, ara, qsq;
                    typename DefineSwizzler<3, 1, 0>::Result wyx, agr, qts;
                    typename DefineSwizzler<3, 1, 1>::Result wyy, agg, qtt;
                    typename DefineSwizzler<3, 1, 2>::Result wyz, agb, qtp;
                    typename DefineSwizzler<3, 1, 3>::Result wyw, aga, qtq;
                    typename DefineSwizzler<3, 2, 0>::Result wzx, abr, qps;
                    typename DefineSwizzler<3, 2, 1>::Result wzy, abg, qpt;
                    typename DefineSwizzler<3, 2, 2>::Result wzz, abb, qpp;
                    typename DefineSwizzler<3, 2, 3>::Result wzw, aba, qpq;
                    typename DefineSwizzler<3, 3, 0>::Result wwx, aar, qqs;
                    typename DefineSwizzler<3, 3, 1>::Result wwy, aag, qqt;
                    typename DefineSwizzler<3, 3, 2>::Result wwz, aab, qqp;
                    typename DefineSwizzler<3, 3, 3>::Result www, aaa, qqq;

                    // Four indices
                    typename DefineSwizzler<0, 0, 0, 0>::Result xxxx, rrrr, ssss;
                    typename DefineSwizzler<0, 0, 0, 1>::Result xxxy, rrrg, ssst;
                    typename DefineSwizzler<0, 0, 0, 2>::Result xxxz, rrrb, sssp;
                    typename DefineSwizzler<0, 0, 0, 3>::Result xxxw, rrra, sssq;
                    typename DefineSwizzler<0, 0, 1, 0>::Result xxyx, rrgr, ssts;
                    typename DefineSwizzler<0, 0, 1, 1>::Result xxyy, rrgg, sstt;
                    typename DefineSwizzler<0, 0, 1, 2>::Result xxyz, rrgb, sstp;
                    typename DefineSwizzler<0, 0, 1, 3>::Result xxyw, rrga, sstq;
                    typename DefineSwizzler<0, 0, 2, 0>::Result xxzx, rrbr, ssps;
                    typename DefineSwizzler<0, 0, 2, 1>::Result xxzy, rrbg, sspt;
                    typename DefineSwizzler<0, 0, 2, 2>::Result xxzz, rrbb, sspp;
                    typename DefineSwizzler<0, 0, 2, 3>::Result xxzw, rrba, sspq;
                    typename DefineSwizzler<0, 0, 3, 0>::Result xxwx, rrar, ssqs;
                    typename DefineSwizzler<0, 0, 3, 1>::Result xxwy, rrag, ssqt;
                    typename DefineSwizzler<0, 0, 3, 2>::Result xxwz, rrab, ssqp;
                    typename DefineSwizzler<0, 0, 3, 3>::Result xxww, rraa, ssqq;
                    typename DefineSwizzler<0, 1, 0, 0>::Result xyxx, rgrr, stss; // ----
                    typename DefineSwizzler<0, 1, 0, 1>::Result xyxy, rgrg, stst;
                    typename DefineSwizzler<0, 1, 0, 2>::Result xyxz, rgrb, stsp;
                    typename DefineSwizzler<0, 1, 0, 3>::Result xyxw, rgra, stsq;
                    typename DefineSwizzler<0, 1, 1, 0>::Result xyyx, rggr, stts;
                    typename DefineSwizzler<0, 1, 1, 1>::Result xyyy, rggg, sttt;
                    typename DefineSwizzler<0, 1, 1, 2>::Result xyyz, rggb, sttp;
                    typename DefineSwizzler<0, 1, 1, 3>::Result xyyw, rgga, sttq;
                    typename DefineSwizzler<0, 1, 2, 0>::Result xyzx, rgbr, stps;
                    typename DefineSwizzler<0, 1, 2, 1>::Result xyzy, rgbg, stpt;
                    typename DefineSwizzler<0, 1, 2, 2>::Result xyzz, rgbb, stpp;
                    typename DefineSwizzler<0, 1, 2, 3>::Result xyzw, rgba, stpq;
                    typename DefineSwizzler<0, 1, 3, 0>::Result xywx, rgar, stqs;
                    typename DefineSwizzler<0, 1, 3, 1>::Result xywy, rgag, stqt;
                    typename DefineSwizzler<0, 1, 3, 2>::Result xywz, rgab, stqp;
                    typename DefineSwizzler<0, 1, 3, 3>::Result xyww, rgaa, stqq;
                    typename DefineSwizzler<0, 2, 0, 0>::Result xzxx, rbrr, spss; // ----
                    typename DefineSwizzler<0, 2, 0, 1>::Result xzxy, rbrg, spst;
                    typename DefineSwizzler<0, 2, 0, 2>::Result xzxz, rbrb, spsp;
                    typename DefineSwizzler<0, 2, 0, 3>::Result xzxw, rbra, spsq;
                    typename DefineSwizzler<0, 2, 1, 0>::Result xzyx, rbgr, spts;
                    typename DefineSwizzler<0, 2, 1, 1>::Result xzyy, rbgg, sptt;
                    typename DefineSwizzler<0, 2, 1, 2>::Result xzyz, rbgb, sptp;
                    typename DefineSwizzler<0, 2, 1, 3>::Result xzyw, rbga, sptq;
                    typename DefineSwizzler<0, 2, 2, 0>::Result xzzx, rbbr, spps;
                    typename DefineSwizzler<0, 2, 2, 1>::Result xzzy, rbbg, sppt;
                    typename DefineSwizzler<0, 2, 2, 2>::Result xzzz, rbbb, sppp;
                    typename DefineSwizzler<0, 2, 2, 3>::Result xzzw, rbba, sppq;
                    typename DefineSwizzler<0, 2, 3, 0>::Result xzwx, rbar, spqs;
                    typename DefineSwizzler<0, 2, 3, 1>::Result xzwy, rbag, spqt;
                    typename DefineSwizzler<0, 2, 3, 2>::Result xzwz, rbab, spqp;
                    typename DefineSwizzler<0, 2, 3, 3>::Result xzww, rbaa, spqq;
                    typename DefineSwizzler<0, 3, 0, 0>::Result xwxx, rarr, sqss; // ----
                    typename DefineSwizzler<0, 3, 0, 1>::Result xwxy, rarg, sqst;
                    typename DefineSwizzler<0, 3, 0, 2>::Result xwxz, rarb, sqsp;
                    typename DefineSwizzler<0, 3, 0, 3>::Result xwxw, rara, sqsq;
                    typename DefineSwizzler<0, 3, 1, 0>::Result xwyx, ragr, sqts;
                    typename DefineSwizzler<0, 3, 1, 1>::Result xwyy, ragg, sqtt;
                    typename DefineSwizzler<0, 3, 1, 2>::Result xwyz, ragb, sqtp;
                    typename DefineSwizzler<0, 3, 1, 3>::Result xwyw, raga, sqtq;
                    typename DefineSwizzler<0, 3, 2, 0>::Result xwzx, rabr, sqps;
                    typename DefineSwizzler<0, 3, 2, 1>::Result xwzy, rabg, sqpt;
                    typename DefineSwizzler<0, 3, 2, 2>::Result xwzz, rabb, sqpp;
                    typename DefineSwizzler<0, 3, 2, 3>::Result xwzw, raba, sqpq;
                    typename DefineSwizzler<0, 3, 3, 0>::Result xwwx, raar, sqqs;
                    typename DefineSwizzler<0, 3, 3, 1>::Result xwwy, raag, sqqt;
                    typename DefineSwizzler<0, 3, 3, 2>::Result xwwz, raab, sqqp;
                    typename DefineSwizzler<0, 3, 3, 3>::Result xwww, raaa, sqqq;

                    typename DefineSwizzler<1, 0, 0, 0>::Result yxxx, grrr, tsss;
                    typename DefineSwizzler<1, 0, 0, 1>::Result yxxy, grrg, tsst;
                    typename DefineSwizzler<1, 0, 0, 2>::Result yxxz, grrb, tssp;
                    typename DefineSwizzler<1, 0, 0, 3>::Result yxxw, grra, tssq;
                    typename DefineSwizzler<1, 0, 1, 0>::Result yxyx, grgr, tsts;
                    typename DefineSwizzler<1, 0, 1, 1>::Result yxyy, grgg, tstt;
                    typename DefineSwizzler<1, 0, 1, 2>::Result yxyz, grgb, tstp;
                    typename DefineSwizzler<1, 0, 1, 3>::Result yxyw, grga, tstq;
                    typename DefineSwizzler<1, 0, 2, 0>::Result yxzx, grbr, tsps;
                    typename DefineSwizzler<1, 0, 2, 1>::Result yxzy, grbg, tspt;
                    typename DefineSwizzler<1, 0, 2, 2>::Result yxzz, grbb, tspp;
                    typename DefineSwizzler<1, 0, 2, 3>::Result yxzw, grba, tspq;
                    typename DefineSwizzler<1, 0, 3, 0>::Result yxwx, grar, tsqs;
                    typename DefineSwizzler<1, 0, 3, 1>::Result yxwy, grag, tsqt;
                    typename DefineSwizzler<1, 0, 3, 2>::Result yxwz, grab, tsqp;
                    typename DefineSwizzler<1, 0, 3, 3>::Result yxww, graa, tsqq;
                    typename DefineSwizzler<1, 1, 0, 0>::Result yyxx, ggrr, ttss; // ----
                    typename DefineSwizzler<1, 1, 0, 1>::Result yyxy, ggrg, ttst;
                    typename DefineSwizzler<1, 1, 0, 2>::Result yyxz, ggrb, ttsp;
                    typename DefineSwizzler<1, 1, 0, 3>::Result yyxw, ggra, ttsq;
                    typename DefineSwizzler<1, 1, 1, 0>::Result yyyx, gggr, ttts;
                    typename DefineSwizzler<1, 1, 1, 1>::Result yyyy, gggg, tttt;
                    typename DefineSwizzler<1, 1, 1, 2>::Result yyyz, gggb, tttp;
                    typename DefineSwizzler<1, 1, 1, 3>::Result yyyw, ggga, tttq;
                    typename DefineSwizzler<1, 1, 2, 0>::Result yyzx, ggbr, ttps;
                    typename DefineSwizzler<1, 1, 2, 1>::Result yyzy, ggbg, ttpt;
                    typename DefineSwizzler<1, 1, 2, 2>::Result yyzz, ggbb, ttpp;
                    typename DefineSwizzler<1, 1, 2, 3>::Result yyzw, ggba, ttpq;
                    typename DefineSwizzler<1, 1, 3, 0>::Result yywx, ggar, ttqs;
                    typename DefineSwizzler<1, 1, 3, 1>::Result yywy, ggag, ttqt;
                    typename DefineSwizzler<1, 1, 3, 2>::Result yywz, ggab, ttqp;
                    typename DefineSwizzler<1, 1, 3, 3>::Result yyww, ggaa, ttqq;
                    typename DefineSwizzler<1, 2, 0, 0>::Result yzxx, gbrr, tpss; // ----
                    typename DefineSwizzler<1, 2, 0, 1>::Result yzxy, gbrg, tpst;
                    typename DefineSwizzler<1, 2, 0, 2>::Result yzxz, gbrb, tpsp;
                    typename DefineSwizzler<1, 2, 0, 3>::Result yzxw, gbra, tpsq;
                    typename DefineSwizzler<1, 2, 1, 0>::Result yzyx, gbgr, tpts;
                    typename DefineSwizzler<1, 2, 1, 1>::Result yzyy, gbgg, tptt;
                    typename DefineSwizzler<1, 2, 1, 2>::Result yzyz, gbgb, tptp;
                    typename DefineSwizzler<1, 2, 1, 3>::Result yzyw, gbga, tptq;
                    typename DefineSwizzler<1, 2, 2, 0>::Result yzzx, gbbr, tpps;
                    typename DefineSwizzler<1, 2, 2, 1>::Result yzzy, gbbg, tppt;
                    typename DefineSwizzler<1, 2, 2, 2>::Result yzzz, gbbb, tppp;
                    typename DefineSwizzler<1, 2, 2, 3>::Result yzzw, gbba, tppq;
                    typename DefineSwizzler<1, 2, 3, 0>::Result yzwx, gbar, tpqs;
                    typename DefineSwizzler<1, 2, 3, 1>::Result yzwy, gbag, tpqt;
                    typename DefineSwizzler<1, 2, 3, 2>::Result yzwz, gbab, tpqp;
                    typename DefineSwizzler<1, 2, 3, 3>::Result yzww, gbaa, tpqq;
                    typename DefineSwizzler<1, 3, 0, 0>::Result ywxx, garr, tqss; // ----
                    typename DefineSwizzler<1, 3, 0, 1>::Result ywxy, garg, tqst;
                    typename DefineSwizzler<1, 3, 0, 2>::Result ywxz, garb, tqsp;
                    typename DefineSwizzler<1, 3, 0, 3>::Result ywxw, gara, tqsq;
                    typename DefineSwizzler<1, 3, 1, 0>::Result ywyx, gagr, tqts;
                    typename DefineSwizzler<1, 3, 1, 1>::Result ywyy, gagg, tqtt;
                    typename DefineSwizzler<1, 3, 1, 2>::Result ywyz, gagb, tqtp;
                    typename DefineSwizzler<1, 3, 1, 3>::Result ywyw, gaga, tqtq;
                    typename DefineSwizzler<1, 3, 2, 0>::Result ywzx, gabr, tqps;
                    typename DefineSwizzler<1, 3, 2, 1>::Result ywzy, gabg, tqpt;
                    typename DefineSwizzler<1, 3, 2, 2>::Result ywzz, gabb, tqpp;
                    typename DefineSwizzler<1, 3, 2, 3>::Result ywzw, gaba, tqpq;
                    typename DefineSwizzler<1, 3, 3, 0>::Result ywwx, gaar, tqqs;
                    typename DefineSwizzler<1, 3, 3, 1>::Result ywwy, gaag, tqqt;
                    typename DefineSwizzler<1, 3, 3, 2>::Result ywwz, gaab, tqqp;
                    typename DefineSwizzler<1, 3, 3, 3>::Result ywww, gaaa, tqqq;

                    typename DefineSwizzler<2, 0, 0, 0>::Result zxxx, brrr, psss;
                    typename DefineSwizzler<2, 0, 0, 1>::Result zxxy, brrg, psst;
                    typename DefineSwizzler<2, 0, 0, 2>::Result zxxz, brrb, pssp;
                    typename DefineSwizzler<2, 0, 0, 3>::Result zxxw, brra, pssq;
                    typename DefineSwizzler<2, 0, 1, 0>::Result zxyx, brgr, psts;
                    typename DefineSwizzler<2, 0, 1, 1>::Result zxyy, brgg, pstt;
                    typename DefineSwizzler<2, 0, 1, 2>::Result zxyz, brgb, pstp;
                    typename DefineSwizzler<2, 0, 1, 3>::Result zxyw, brga, pstq;
                    typename DefineSwizzler<2, 0, 2, 0>::Result zxzx, brbr, psps;
                    typename DefineSwizzler<2, 0, 2, 1>::Result zxzy, brbg, pspt;
                    typename DefineSwizzler<2, 0, 2, 2>::Result zxzz, brbb, pspp;
                    typename DefineSwizzler<2, 0, 2, 3>::Result zxzw, brba, pspq;
                    typename DefineSwizzler<2, 0, 3, 0>::Result zxwx, brar, psqs;
                    typename DefineSwizzler<2, 0, 3, 1>::Result zxwy, brag, psqt;
                    typename DefineSwizzler<2, 0, 3, 2>::Result zxwz, brab, psqp;
                    typename DefineSwizzler<2, 0, 3, 3>::Result zxww, braa, psqq;
                    typename DefineSwizzler<2, 1, 0, 0>::Result zyxx, bgrr, ptss; // ----
                    typename DefineSwizzler<2, 1, 0, 1>::Result zyxy, bgrg, ptst;
                    typename DefineSwizzler<2, 1, 0, 2>::Result zyxz, bgrb, ptsp;
                    typename DefineSwizzler<2, 1, 0, 3>::Result zyxw, bgra, ptsq;
                    typename DefineSwizzler<2, 1, 1, 0>::Result zyyx, bggr, ptts;
                    typename DefineSwizzler<2, 1, 1, 1>::Result zyyy, bggg, pttt;
                    typename DefineSwizzler<2, 1, 1, 2>::Result zyyz, bggb, pttp;
                    typename DefineSwizzler<2, 1, 1, 3>::Result zyyw, bgga, pttq;
                    typename DefineSwizzler<2, 1, 2, 0>::Result zyzx, bgbr, ptps;
                    typename DefineSwizzler<2, 1, 2, 1>::Result zyzy, bgbg, ptpt;
                    typename DefineSwizzler<2, 1, 2, 2>::Result zyzz, bgbb, ptpp;
                    typename DefineSwizzler<2, 1, 2, 3>::Result zyzw, bgba, ptpq;
                    typename DefineSwizzler<2, 1, 3, 0>::Result zywx, bgar, ptqs;
                    typename DefineSwizzler<2, 1, 3, 1>::Result zywy, bgag, ptqt;
                    typename DefineSwizzler<2, 1, 3, 2>::Result zywz, bgab, ptqp;
                    typename DefineSwizzler<2, 1, 3, 3>::Result zyww, bgaa, ptqq;
                    typename DefineSwizzler<2, 2, 0, 0>::Result zzxx, bbrr, ppss; // ----
                    typename DefineSwizzler<2, 2, 0, 1>::Result zzxy, bbrg, ppst;
                    typename DefineSwizzler<2, 2, 0, 2>::Result zzxz, bbrb, ppsp;
                    typename DefineSwizzler<2, 2, 0, 3>::Result zzxw, bbra, ppsq;
                    typename DefineSwizzler<2, 2, 1, 0>::Result zzyx, bbgr, ppts;
                    typename DefineSwizzler<2, 2, 1, 1>::Result zzyy, bbgg, pptt;
                    typename DefineSwizzler<2, 2, 1, 2>::Result zzyz, bbgb, pptp;
                    typename DefineSwizzler<2, 2, 1, 3>::Result zzyw, bbga, pptq;
                    typename DefineSwizzler<2, 2, 2, 0>::Result zzzx, bbbr, ppps;
                    typename DefineSwizzler<2, 2, 2, 1>::Result zzzy, bbbg, pppt;
                    typename DefineSwizzler<2, 2, 2, 2>::Result zzzz, bbbb, pppp;
                    typename DefineSwizzler<2, 2, 2, 3>::Result zzzw, bbba, pppq;
                    typename DefineSwizzler<2, 2, 3, 0>::Result zzwx, bbar, ppqs;
                    typename DefineSwizzler<2, 2, 3, 1>::Result zzwy, bbag, ppqt;
                    typename DefineSwizzler<2, 2, 3, 2>::Result zzwz, bbab, ppqp;
                    typename DefineSwizzler<2, 2, 3, 3>::Result zzww, bbaa, ppqq;
                    typename DefineSwizzler<2, 3, 0, 0>::Result zwxx, barr, pqss; // ----
                    typename DefineSwizzler<2, 3, 0, 1>::Result zwxy, barg, pqst;
                    typename DefineSwizzler<2, 3, 0, 2>::Result zwxz, barb, pqsp;
                    typename DefineSwizzler<2, 3, 0, 3>::Result zwxw, bara, pqsq;
                    typename DefineSwizzler<2, 3, 1, 0>::Result zwyx, bagr, pqts;
                    typename DefineSwizzler<2, 3, 1, 1>::Result zwyy, bagg, pqtt;
                    typename DefineSwizzler<2, 3, 1, 2>::Result zwyz, bagb, pqtp;
                    typename DefineSwizzler<2, 3, 1, 3>::Result zwyw, baga, pqtq;
                    typename DefineSwizzler<2, 3, 2, 0>::Result zwzx, babr, pqps;
                    typename DefineSwizzler<2, 3, 2, 1>::Result zwzy, babg, pqpt;
                    typename DefineSwizzler<2, 3, 2, 2>::Result zwzz, babb, pqpp;
                    typename DefineSwizzler<2, 3, 2, 3>::Result zwzw, baba, pqpq;
                    typename DefineSwizzler<2, 3, 3, 0>::Result zwwx, baar, pqqs;
                    typename DefineSwizzler<2, 3, 3, 1>::Result zwwy, baag, pqqt;
                    typename DefineSwizzler<2, 3, 3, 2>::Result zwwz, baab, pqqp;
                    typename DefineSwizzler<2, 3, 3, 3>::Result zwww, baaa, pqqq;

                    typename DefineSwizzler<3, 0, 0, 0>::Result wxxx, arrr, qsss;
                    typename DefineSwizzler<3, 0, 0, 1>::Result wxxy, arrg, qsst;
                    typename DefineSwizzler<3, 0, 0, 2>::Result wxxz, arrb, qssp;
                    typename DefineSwizzler<3, 0, 0, 3>::Result wxxw, arra, qssq;
                    typename DefineSwizzler<3, 0, 1, 0>::Result wxyx, argr, qsts;
                    typename DefineSwizzler<3, 0, 1, 1>::Result wxyy, argg, qstt;
                    typename DefineSwizzler<3, 0, 1, 2>::Result wxyz, argb, qstp;
                    typename DefineSwizzler<3, 0, 1, 3>::Result wxyw, arga, qstq;
                    typename DefineSwizzler<3, 0, 2, 0>::Result wxzx, arbr, qsps;
                    typename DefineSwizzler<3, 0, 2, 1>::Result wxzy, arbg, qspt;
                    typename DefineSwizzler<3, 0, 2, 2>::Result wxzz, arbb, qspp;
                    typename DefineSwizzler<3, 0, 2, 3>::Result wxzw, arba, qspq;
                    typename DefineSwizzler<3, 0, 3, 0>::Result wxwx, arar, qsqs;
                    typename DefineSwizzler<3, 0, 3, 1>::Result wxwy, arag, qsqt;
                    typename DefineSwizzler<3, 0, 3, 2>::Result wxwz, arab, qsqp;
                    typename DefineSwizzler<3, 0, 3, 3>::Result wxww, araa, qsqq;
                    typename DefineSwizzler<3, 1, 0, 0>::Result wyxx, agrr, qtss; // ----
                    typename DefineSwizzler<3, 1, 0, 1>::Result wyxy, agrg, qtst;
                    typename DefineSwizzler<3, 1, 0, 2>::Result wyxz, agrb, qtsp;
                    typename DefineSwizzler<3, 1, 0, 3>::Result wyxw, agra, qtsq;
                    typename DefineSwizzler<3, 1, 1, 0>::Result wyyx, aggr, qtts;
                    typename DefineSwizzler<3, 1, 1, 1>::Result wyyy, aggg, qttt;
                    typename DefineSwizzler<3, 1, 1, 2>::Result wyyz, aggb, qttp;
                    typename DefineSwizzler<3, 1, 1, 3>::Result wyyw, agga, qttq;
                    typename DefineSwizzler<3, 1, 2, 0>::Result wyzx, agbr, qtps;
                    typename DefineSwizzler<3, 1, 2, 1>::Result wyzy, agbg, qtpt;
                    typename DefineSwizzler<3, 1, 2, 2>::Result wyzz, agbb, qtpp;
                    typename DefineSwizzler<3, 1, 2, 3>::Result wyzw, agba, qtpq;
                    typename DefineSwizzler<3, 1, 3, 0>::Result wywx, agar, qtqs;
                    typename DefineSwizzler<3, 1, 3, 1>::Result wywy, agag, qtqt;
                    typename DefineSwizzler<3, 1, 3, 2>::Result wywz, agab, qtqp;
                    typename DefineSwizzler<3, 1, 3, 3>::Result wyww, agaa, qtqq;
                    typename DefineSwizzler<3, 2, 0, 0>::Result wzxx, abrr, qpss; // ----
                    typename DefineSwizzler<3, 2, 0, 1>::Result wzxy, abrg, qpst;
                    typename DefineSwizzler<3, 2, 0, 2>::Result wzxz, abrb, qpsp;
                    typename DefineSwizzler<3, 2, 0, 3>::Result wzxw, abra, qpsq;
                    typename DefineSwizzler<3, 2, 1, 0>::Result wzyx, abgr, qpts;
                    typename DefineSwizzler<3, 2, 1, 1>::Result wzyy, abgg, qptt;
                    typename DefineSwizzler<3, 2, 1, 2>::Result wzyz, abgb, qptp;
                    typename DefineSwizzler<3, 2, 1, 3>::Result wzyw, abga, qptq;
                    typename DefineSwizzler<3, 2, 2, 0>::Result wzzx, abbr, qpps;
                    typename DefineSwizzler<3, 2, 2, 1>::Result wzzy, abbg, qppt;
                    typename DefineSwizzler<3, 2, 2, 2>::Result wzzz, abbb, qppp;
                    typename DefineSwizzler<3, 2, 2, 3>::Result wzzw, abba, qppq;
                    typename DefineSwizzler<3, 2, 3, 0>::Result wzwx, abar, qpqs;
                    typename DefineSwizzler<3, 2, 3, 1>::Result wzwy, abag, qpqt;
                    typename DefineSwizzler<3, 2, 3, 2>::Result wzwz, abab, qpqp;
                    typename DefineSwizzler<3, 2, 3, 3>::Result wzww, abaa, qpqq;
                    typename DefineSwizzler<3, 3, 0, 0>::Result wwxx, aarr, qqss; // ----
                    typename DefineSwizzler<3, 3, 0, 1>::Result wwxy, aarg, qqst;
                    typename DefineSwizzler<3, 3, 0, 2>::Result wwxz, aarb, qqsp;
                    typename DefineSwizzler<3, 3, 0, 3>::Result wwxw, aara, qqsq;
                    typename DefineSwizzler<3, 3, 1, 0>::Result wwyx, aagr, qqts;
                    typename DefineSwizzler<3, 3, 1, 1>::Result wwyy, aagg, qqtt;
                    typename DefineSwizzler<3, 3, 1, 2>::Result wwyz, aagb, qqtp;
                    typename DefineSwizzler<3, 3, 1, 3>::Result wwyw, aaga, qqtq;
                    typename DefineSwizzler<3, 3, 2, 0>::Result wwzx, aabr, qqps;
                    typename DefineSwizzler<3, 3, 2, 1>::Result wwzy, aabg, qqpt;
                    typename DefineSwizzler<3, 3, 2, 2>::Result wwzz, aabb, qqpp;
                    typename DefineSwizzler<3, 3, 2, 3>::Result wwzw, aaba, qqpq;
                    typename DefineSwizzler<3, 3, 3, 0>::Result wwwx, aaar, qqqs;
                    typename DefineSwizzler<3, 3, 3, 1>::Result wwwy, aaag, qqqt;
                    typename DefineSwizzler<3, 3, 3, 2>::Result wwwz, aaab, qqqp;
                    typename DefineSwizzler<3, 3, 3, 3>::Result wwww, aaaa, qqqq;

#endif // OGLML_CXX11_UNRESTRICTED_UNIONS
                };
            };

            template <std::size_t n, typename T, class SP>
            struct JustData {
                typedef typename SP::template Container<n, T> Container;

                union {
                    Container data;

                    ValueView<n, T, Container, 0> x, r, s;
                    ValueView<n, T, Container, 1> y, g, t;
                    ValueView<n, T, Container, 2> z, b, p;
                    ValueView<n, T, Container, 3> w, a, q;
                };
            };

            template <std::size_t n, typename T, class SP>
            struct CreateDataImpl {
                typedef typename Select<SP::template Container<n, T>::defineSwizzlers,
                        SwizzlerUnion<n, T, SP>, JustData<n, T, SP> >::Result Result;
            };

        } // namespace detail
    } // namespace vec



    template <std::size_t n, typename T, class SP>
    struct Vec : public BaseVec<n, T>, public vec::detail::CreateDataImpl<n, T, SP>::Result
    {
        // Typedefs
        typedef Vec<n, T, SP> ThisType;
        typedef typename SP::template Container<n, T> Container;
        typedef typename vec::detail::CreateDataImpl<n, T, SP>::Result DataImpl;

        // Ctor and Dtor
        Vec() = default;

        template <typename First, typename... Args>
        Vec(const First& first, const Args&... args)
        { assign(*this, first, args...); }

        ~Vec() = default;

        // Index operator
        T& operator[](std::size_t i)
        { return DataImpl::data[i]; }

        const T& operator[](std::size_t i) const
        { return DataImpl::data[i]; }

        // Promotion & negation
        Vec<n, T> operator+() const
        { return promote(*this); }

        Vec<n, T> operator-() const
        { return negate(*this); }

        // Assignment operator
        template <typename Rhs>
        ThisType& operator=(const Rhs& rhs)
        { return assign(*this, rhs); }

        template <typename Rhs>
        ThisType& operator-=(const Rhs& rhs)
        { return opassign<Plus>(*this, rhs); }

        template <typename Rhs>
        ThisType& operator*=(const Rhs& rhs)
        { return opassign<Plus>(*this, rhs); }

        template <typename Rhs>
        ThisType& operator/=(const Rhs& rhs)
        { return opassign<Plus>(*this, rhs); }

        template <typename Rhs>
        ThisType& operator%=(const Rhs& rhs)
        { return opassign<Plus>(*this, rhs); }

        // Operation assignment operators
        template <typename Rhs>
        ThisType& operator+=(const Rhs& rhs)
        { return opassign<Plus>(*this, rhs); }

        // Member funcs
        oglml_constexpr_if_available static std::size_t length()
        { return n; }
    };

    namespace glsl {
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

#ifdef OGLML_CXX11_TEMPLATE_ALIASES

        // Template aliases
        template <std::size_t n>
        using vec = Vec<n, float>;

        template <std::size_t n>
        using ivec = Vec<n, int>;

        template <std::size_t n>
        using uvec = Vec<n, unsigned int>;

        template <std::size_t n>
        using bvec = Vec<n, bool>;

        template <std::size_t n>
        using dvec = Vec<n, double>;


        template <typename T>
        using vec2 = Vec<2, T>;

        template <typename T>
        using vec3 = Vec<3, T>;

        template <typename T>
        using vec4 = Vec<4, T>;

#endif // OGLML_CXX11_TEMPLATE_ALIASES

    } // namespace glsl

    using namespace glsl;

} // namespace oglml

#endif // OGLML_VEC_HPP
