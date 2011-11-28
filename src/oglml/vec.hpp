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

            template <std::size_t n, typename T, class SP>
            struct SwizzlerUnion {
                typedef typename SP::template Container<n, T> Container;

                // Structs
                template <std::size_t... indices>
                struct DefineSwizzler {

                    oglml_constexpr static std::size_t nindices = sizeof...(indices);
                    oglml_constexpr static bool defineSwizzlers = Container::defineSwizzlers;

                    typedef vec::StaticSwizzlerStorage<n, T, SP, indices...> SwizzlerSP;
                    typedef Vec<nindices, T, SwizzlerSP> VecT;

                    typedef typename Select<defineSwizzlers, VecT, oglml::detail::Empty>::Result Result;
                };

                // Swizzlers + data
                union {
                    // Contains the data, public accessable
                    Container data;

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
                typename SP::template Container<n, T> data;
            };

            template <std::size_t n, typename T, class SP>
            struct CreateDataImpl {
                typedef typename Select<SP::template Container<n, T>::defineSwizzlers,
                        SwizzlerUnion<n, T, SP>, JustData<n, T, SP> >::Result Result;
            };

        } // namespace detail

    } // namespace vec



    template <std::size_t n, typename T, class SP>
    class Vec : public BaseVec<n, T>, public vec::detail::CreateDataImpl<n, T, SP>::Result
    {
    public:
        // Conditions
        static_assert(n > 1, "An OGLML Vec has to have at least 2 dimensions.");

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
