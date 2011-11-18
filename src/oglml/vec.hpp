#ifndef OGLML_VEC_HPP
#define OGLML_VEC_HPP

#include <iostream>
#include <cstddef>

#include <oglml/vecfwd.hpp>
#include <oglml/vec/info.hpp>
#include <oglml/vec/storagepolicies/default.hpp>
#include <oglml/vec/expression.hpp>
#include <oglml/vec/funcs.hpp>
#include <oglml/vec/operators.hpp>
#include <oglml/vec/swizzler.hpp>
#include <oglml/helpers/operations.hpp>
#include <oglml/helpers/autoreturn.hpp>

namespace oglml {

    namespace detail {
        template <std::size_t tn, typename tT>
        struct MakeInfo {
            typedef tT T;
            oglml_constexpr static std::size_t n = tn;
        };
    } // namespace detail

    template <std::size_t n, typename T, class SP = vec::DefaultStorage>
    class Vec : public BaseVec, public detail::MakeInfo<n, T> {
    public:
        // Typedefs
        typedef Vec<n, T, SP> ThisType;
        typedef typename SP::template Container<n, T> Container;
        typedef typename Container::ReturnT ReturnT;
        typedef typename Container::ConstReturnT ConstReturnT;

        // Ctor and Dtor
        Vec()
        { }

        template <typename First, typename... Args>
        Vec(const First& first, const Args&... args)
        { assign(*this, first, args...); }

        ~Vec()
        { }

        // Index operator
        ReturnT operator[](std::size_t i)
        { return data[i]; }

        ConstReturnT operator[](std::size_t i) const
        { return data[i]; }

        // Assignment operator
        template <typename Trhs>
        ThisType& operator=(const Trhs& rhs)
        { return assign(*this, rhs); }

        // Op Assignments
        // Plus
        template <typename Trhs>
        ThisType& operator+=(const Trhs& rhs)
        { return opassign<Plus>(*this, rhs); }

        template <typename Trhs>
        ThisType& operator-=(const Trhs& rhs)
        { return opassign<Minus>(*this, rhs);  }

        template <typename Trhs>
        ThisType& operator*=(const Trhs& rhs)
        { return opassign<Multiplies>(*this, rhs);  }

        template <typename Trhs>
        ThisType& operator/=(const Trhs& rhs)
        { return opassign<Divides>(*this, rhs);  }

        template <typename Trhs>
        ThisType& operator%=(const Trhs& rhs)
        { return opassign<Modulus>(*this, rhs);  }

        // Member funcs
        oglml_constexpr_if_available static std::size_t length()
        { return n; }

        // Swizzlers + data
        union {
            // Contains the data, public accessable
            Container data;

            // Swizzlers
            // One index
            vec::detail::Swizzler<ThisType, 0> x, r, s;
            vec::detail::Swizzler<ThisType, 1> y, g, t;
            vec::detail::Swizzler<ThisType, 2> z, b, p;
            vec::detail::Swizzler<ThisType, 3> w, a, q;

            // Two indices
            vec::detail::Swizzler<ThisType, 0, 0> xx, rr, ss;
            vec::detail::Swizzler<ThisType, 0, 1> xy, rg, st;
            vec::detail::Swizzler<ThisType, 0, 2> xz, rb, sp;
            vec::detail::Swizzler<ThisType, 0, 3> xw, ra, sq;

            vec::detail::Swizzler<ThisType, 1, 0> yx, gr, ts;
            vec::detail::Swizzler<ThisType, 1, 1> yy, gg, tt;
            vec::detail::Swizzler<ThisType, 1, 2> yz, gb, tp;
            vec::detail::Swizzler<ThisType, 1, 3> yw, ga, tq;

            vec::detail::Swizzler<ThisType, 2, 0> zx, br, ps;
            vec::detail::Swizzler<ThisType, 2, 1> zy, bg, pt;
            vec::detail::Swizzler<ThisType, 2, 2> zz, bb, pp;
            vec::detail::Swizzler<ThisType, 2, 3> zw, ba, pq;

            vec::detail::Swizzler<ThisType, 3, 0> wx, ar, qs;
            vec::detail::Swizzler<ThisType, 3, 1> wy, ag, qt;
            vec::detail::Swizzler<ThisType, 3, 2> wz, ab, qp;
            vec::detail::Swizzler<ThisType, 3, 3> ww, aa, qq;

            // Three indices
            vec::detail::Swizzler<ThisType, 0, 0, 0> xxx, rrr, sss;
            vec::detail::Swizzler<ThisType, 0, 0, 1> xxy, rrg, sst;
            vec::detail::Swizzler<ThisType, 0, 0, 2> xxz, rrb, ssp;
            vec::detail::Swizzler<ThisType, 0, 0, 3> xxw, rra, ssq;
            vec::detail::Swizzler<ThisType, 0, 1, 0> xyx, rgr, sts;
            vec::detail::Swizzler<ThisType, 0, 1, 1> xyy, rgg, stt;
            vec::detail::Swizzler<ThisType, 0, 1, 2> xyz, rgb, stp;
            vec::detail::Swizzler<ThisType, 0, 1, 3> xyw, rga, stq;
            vec::detail::Swizzler<ThisType, 0, 2, 0> xzx, rbr, sps;
            vec::detail::Swizzler<ThisType, 0, 2, 1> xzy, rbg, spt;
            vec::detail::Swizzler<ThisType, 0, 2, 2> xzz, rbb, spp;
            vec::detail::Swizzler<ThisType, 0, 2, 3> xzw, rba, spq;
            vec::detail::Swizzler<ThisType, 0, 3, 0> xwx, rar, sqs;
            vec::detail::Swizzler<ThisType, 0, 3, 1> xwy, rag, sqt;
            vec::detail::Swizzler<ThisType, 0, 3, 2> xwz, rab, sqp;
            vec::detail::Swizzler<ThisType, 0, 3, 3> xww, raa, sqq;

            vec::detail::Swizzler<ThisType, 1, 0, 0> yxx, grr, tss;
            vec::detail::Swizzler<ThisType, 1, 0, 1> yxy, grg, tst;
            vec::detail::Swizzler<ThisType, 1, 0, 2> yxz, grb, tsp;
            vec::detail::Swizzler<ThisType, 1, 0, 3> yxw, gra, tsq;
            vec::detail::Swizzler<ThisType, 1, 1, 0> yyx, ggr, tts;
            vec::detail::Swizzler<ThisType, 1, 1, 1> yyy, ggg, ttt;
            vec::detail::Swizzler<ThisType, 1, 1, 2> yyz, ggb, ttp;
            vec::detail::Swizzler<ThisType, 1, 1, 3> yyw, gga, ttq;
            vec::detail::Swizzler<ThisType, 1, 2, 0> yzx, gbr, tps;
            vec::detail::Swizzler<ThisType, 1, 2, 1> yzy, gbg, tpt;
            vec::detail::Swizzler<ThisType, 1, 2, 2> yzz, gbb, tpp;
            vec::detail::Swizzler<ThisType, 1, 2, 3> yzw, gba, tpq;
            vec::detail::Swizzler<ThisType, 1, 3, 0> ywx, gar, tqs;
            vec::detail::Swizzler<ThisType, 1, 3, 1> ywy, gag, tqt;
            vec::detail::Swizzler<ThisType, 1, 3, 2> ywz, gab, tqp;
            vec::detail::Swizzler<ThisType, 1, 3, 3> yww, gaa, tqq;

            vec::detail::Swizzler<ThisType, 2, 0, 0> zxx, brr, pss;
            vec::detail::Swizzler<ThisType, 2, 0, 1> zxy, brg, pst;
            vec::detail::Swizzler<ThisType, 2, 0, 2> zxz, brb, psp;
            vec::detail::Swizzler<ThisType, 2, 0, 3> zxw, bra, psq;
            vec::detail::Swizzler<ThisType, 2, 1, 0> zyx, bgr, pts;
            vec::detail::Swizzler<ThisType, 2, 1, 1> zyy, bgg, ptt;
            vec::detail::Swizzler<ThisType, 2, 1, 2> zyz, bgb, ptp;
            vec::detail::Swizzler<ThisType, 2, 1, 3> zyw, bga, ptq;
            vec::detail::Swizzler<ThisType, 2, 2, 0> zzx, bbr, pps;
            vec::detail::Swizzler<ThisType, 2, 2, 1> zzy, bbg, ppt;
            vec::detail::Swizzler<ThisType, 2, 2, 2> zzz, bbb, ppp;
            vec::detail::Swizzler<ThisType, 2, 2, 3> zzw, bba, ppq;
            vec::detail::Swizzler<ThisType, 2, 3, 0> zwx, bar, pqs;
            vec::detail::Swizzler<ThisType, 2, 3, 1> zwy, bag, pqt;
            vec::detail::Swizzler<ThisType, 2, 3, 2> zwz, bab, pqp;
            vec::detail::Swizzler<ThisType, 2, 3, 3> zww, baa, pqq;

            vec::detail::Swizzler<ThisType, 3, 0, 0> wxx, arr, qss;
            vec::detail::Swizzler<ThisType, 3, 0, 1> wxy, arg, qst;
            vec::detail::Swizzler<ThisType, 3, 0, 2> wxz, arb, qsp;
            vec::detail::Swizzler<ThisType, 3, 0, 3> wxw, ara, qsq;
            vec::detail::Swizzler<ThisType, 3, 1, 0> wyx, agr, qts;
            vec::detail::Swizzler<ThisType, 3, 1, 1> wyy, agg, qtt;
            vec::detail::Swizzler<ThisType, 3, 1, 2> wyz, agb, qtp;
            vec::detail::Swizzler<ThisType, 3, 1, 3> wyw, aga, qtq;
            vec::detail::Swizzler<ThisType, 3, 2, 0> wzx, abr, qps;
            vec::detail::Swizzler<ThisType, 3, 2, 1> wzy, abg, qpt;
            vec::detail::Swizzler<ThisType, 3, 2, 2> wzz, abb, qpp;
            vec::detail::Swizzler<ThisType, 3, 2, 3> wzw, aba, qpq;
            vec::detail::Swizzler<ThisType, 3, 3, 0> wwx, aar, qqs;
            vec::detail::Swizzler<ThisType, 3, 3, 1> wwy, aag, qqt;
            vec::detail::Swizzler<ThisType, 3, 3, 2> wwz, aab, qqp;
            vec::detail::Swizzler<ThisType, 3, 3, 3> www, aaa, qqq;

            // Four indices
            vec::detail::Swizzler<ThisType, 0, 0, 0, 0> xxxx, rrrr, ssss;
            vec::detail::Swizzler<ThisType, 0, 0, 0, 1> xxxy, rrrg, ssst;
            vec::detail::Swizzler<ThisType, 0, 0, 0, 2> xxxz, rrrb, sssp;
            vec::detail::Swizzler<ThisType, 0, 0, 0, 3> xxxw, rrra, sssq;
            vec::detail::Swizzler<ThisType, 0, 0, 1, 0> xxyx, rrgr, ssts;
            vec::detail::Swizzler<ThisType, 0, 0, 1, 1> xxyy, rrgg, sstt;
            vec::detail::Swizzler<ThisType, 0, 0, 1, 2> xxyz, rrgb, sstp;
            vec::detail::Swizzler<ThisType, 0, 0, 1, 3> xxyw, rrga, sstq;
            vec::detail::Swizzler<ThisType, 0, 0, 2, 0> xxzx, rrbr, ssps;
            vec::detail::Swizzler<ThisType, 0, 0, 2, 1> xxzy, rrbg, sspt;
            vec::detail::Swizzler<ThisType, 0, 0, 2, 2> xxzz, rrbb, sspp;
            vec::detail::Swizzler<ThisType, 0, 0, 2, 3> xxzw, rrba, sspq;
            vec::detail::Swizzler<ThisType, 0, 0, 3, 0> xxwx, rrar, ssqs;
            vec::detail::Swizzler<ThisType, 0, 0, 3, 1> xxwy, rrag, ssqt;
            vec::detail::Swizzler<ThisType, 0, 0, 3, 2> xxwz, rrab, ssqp;
            vec::detail::Swizzler<ThisType, 0, 0, 3, 3> xxww, rraa, ssqq;
            vec::detail::Swizzler<ThisType, 0, 1, 0, 0> xyxx, rgrr, stss; // ----
            vec::detail::Swizzler<ThisType, 0, 1, 0, 1> xyxy, rgrg, stst;
            vec::detail::Swizzler<ThisType, 0, 1, 0, 2> xyxz, rgrb, stsp;
            vec::detail::Swizzler<ThisType, 0, 1, 0, 3> xyxw, rgra, stsq;
            vec::detail::Swizzler<ThisType, 0, 1, 1, 0> xyyx, rggr, stts;
            vec::detail::Swizzler<ThisType, 0, 1, 1, 1> xyyy, rggg, sttt;
            vec::detail::Swizzler<ThisType, 0, 1, 1, 2> xyyz, rggb, sttp;
            vec::detail::Swizzler<ThisType, 0, 1, 1, 3> xyyw, rgga, sttq;
            vec::detail::Swizzler<ThisType, 0, 1, 2, 0> xyzx, rgbr, stps;
            vec::detail::Swizzler<ThisType, 0, 1, 2, 1> xyzy, rgbg, stpt;
            vec::detail::Swizzler<ThisType, 0, 1, 2, 2> xyzz, rgbb, stpp;
            vec::detail::Swizzler<ThisType, 0, 1, 2, 3> xyzw, rgba, stpq;
            vec::detail::Swizzler<ThisType, 0, 1, 3, 0> xywx, rgar, stqs;
            vec::detail::Swizzler<ThisType, 0, 1, 3, 1> xywy, rgag, stqt;
            vec::detail::Swizzler<ThisType, 0, 1, 3, 2> xywz, rgab, stqp;
            vec::detail::Swizzler<ThisType, 0, 1, 3, 3> xyww, rgaa, stqq;
            vec::detail::Swizzler<ThisType, 0, 2, 0, 0> xzxx, rbrr, spss; // ----
            vec::detail::Swizzler<ThisType, 0, 2, 0, 1> xzxy, rbrg, spst;
            vec::detail::Swizzler<ThisType, 0, 2, 0, 2> xzxz, rbrb, spsp;
            vec::detail::Swizzler<ThisType, 0, 2, 0, 3> xzxw, rbra, spsq;
            vec::detail::Swizzler<ThisType, 0, 2, 1, 0> xzyx, rbgr, spts;
            vec::detail::Swizzler<ThisType, 0, 2, 1, 1> xzyy, rbgg, sptt;
            vec::detail::Swizzler<ThisType, 0, 2, 1, 2> xzyz, rbgb, sptp;
            vec::detail::Swizzler<ThisType, 0, 2, 1, 3> xzyw, rbga, sptq;
            vec::detail::Swizzler<ThisType, 0, 2, 2, 0> xzzx, rbbr, spps;
            vec::detail::Swizzler<ThisType, 0, 2, 2, 1> xzzy, rbbg, sppt;
            vec::detail::Swizzler<ThisType, 0, 2, 2, 2> xzzz, rbbb, sppp;
            vec::detail::Swizzler<ThisType, 0, 2, 2, 3> xzzw, rbba, sppq;
            vec::detail::Swizzler<ThisType, 0, 2, 3, 0> xzwx, rbar, spqs;
            vec::detail::Swizzler<ThisType, 0, 2, 3, 1> xzwy, rbag, spqt;
            vec::detail::Swizzler<ThisType, 0, 2, 3, 2> xzwz, rbab, spqp;
            vec::detail::Swizzler<ThisType, 0, 2, 3, 3> xzww, rbaa, spqq;
            vec::detail::Swizzler<ThisType, 0, 3, 0, 0> xwxx, rarr, sqss; // ----
            vec::detail::Swizzler<ThisType, 0, 3, 0, 1> xwxy, rarg, sqst;
            vec::detail::Swizzler<ThisType, 0, 3, 0, 2> xwxz, rarb, sqsp;
            vec::detail::Swizzler<ThisType, 0, 3, 0, 3> xwxw, rara, sqsq;
            vec::detail::Swizzler<ThisType, 0, 3, 1, 0> xwyx, ragr, sqts;
            vec::detail::Swizzler<ThisType, 0, 3, 1, 1> xwyy, ragg, sqtt;
            vec::detail::Swizzler<ThisType, 0, 3, 1, 2> xwyz, ragb, sqtp;
            vec::detail::Swizzler<ThisType, 0, 3, 1, 3> xwyw, raga, sqtq;
            vec::detail::Swizzler<ThisType, 0, 3, 2, 0> xwzx, rabr, sqps;
            vec::detail::Swizzler<ThisType, 0, 3, 2, 1> xwzy, rabg, sqpt;
            vec::detail::Swizzler<ThisType, 0, 3, 2, 2> xwzz, rabb, sqpp;
            vec::detail::Swizzler<ThisType, 0, 3, 2, 3> xwzw, raba, sqpq;
            vec::detail::Swizzler<ThisType, 0, 3, 3, 0> xwwx, raar, sqqs;
            vec::detail::Swizzler<ThisType, 0, 3, 3, 1> xwwy, raag, sqqt;
            vec::detail::Swizzler<ThisType, 0, 3, 3, 2> xwwz, raab, sqqp;
            vec::detail::Swizzler<ThisType, 0, 3, 3, 3> xwww, raaa, sqqq;

            vec::detail::Swizzler<ThisType, 1, 0, 0, 0> yxxx, grrr, tsss;
            vec::detail::Swizzler<ThisType, 1, 0, 0, 1> yxxy, grrg, tsst;
            vec::detail::Swizzler<ThisType, 1, 0, 0, 2> yxxz, grrb, tssp;
            vec::detail::Swizzler<ThisType, 1, 0, 0, 3> yxxw, grra, tssq;
            vec::detail::Swizzler<ThisType, 1, 0, 1, 0> yxyx, grgr, tsts;
            vec::detail::Swizzler<ThisType, 1, 0, 1, 1> yxyy, grgg, tstt;
            vec::detail::Swizzler<ThisType, 1, 0, 1, 2> yxyz, grgb, tstp;
            vec::detail::Swizzler<ThisType, 1, 0, 1, 3> yxyw, grga, tstq;
            vec::detail::Swizzler<ThisType, 1, 0, 2, 0> yxzx, grbr, tsps;
            vec::detail::Swizzler<ThisType, 1, 0, 2, 1> yxzy, grbg, tspt;
            vec::detail::Swizzler<ThisType, 1, 0, 2, 2> yxzz, grbb, tspp;
            vec::detail::Swizzler<ThisType, 1, 0, 2, 3> yxzw, grba, tspq;
            vec::detail::Swizzler<ThisType, 1, 0, 3, 0> yxwx, grar, tsqs;
            vec::detail::Swizzler<ThisType, 1, 0, 3, 1> yxwy, grag, tsqt;
            vec::detail::Swizzler<ThisType, 1, 0, 3, 2> yxwz, grab, tsqp;
            vec::detail::Swizzler<ThisType, 1, 0, 3, 3> yxww, graa, tsqq;
            vec::detail::Swizzler<ThisType, 1, 1, 0, 0> yyxx, ggrr, ttss; // ----
            vec::detail::Swizzler<ThisType, 1, 1, 0, 1> yyxy, ggrg, ttst;
            vec::detail::Swizzler<ThisType, 1, 1, 0, 2> yyxz, ggrb, ttsp;
            vec::detail::Swizzler<ThisType, 1, 1, 0, 3> yyxw, ggra, ttsq;
            vec::detail::Swizzler<ThisType, 1, 1, 1, 0> yyyx, gggr, ttts;
            vec::detail::Swizzler<ThisType, 1, 1, 1, 1> yyyy, gggg, tttt;
            vec::detail::Swizzler<ThisType, 1, 1, 1, 2> yyyz, gggb, tttp;
            vec::detail::Swizzler<ThisType, 1, 1, 1, 3> yyyw, ggga, tttq;
            vec::detail::Swizzler<ThisType, 1, 1, 2, 0> yyzx, ggbr, ttps;
            vec::detail::Swizzler<ThisType, 1, 1, 2, 1> yyzy, ggbg, ttpt;
            vec::detail::Swizzler<ThisType, 1, 1, 2, 2> yyzz, ggbb, ttpp;
            vec::detail::Swizzler<ThisType, 1, 1, 2, 3> yyzw, ggba, ttpq;
            vec::detail::Swizzler<ThisType, 1, 1, 3, 0> yywx, ggar, ttqs;
            vec::detail::Swizzler<ThisType, 1, 1, 3, 1> yywy, ggag, ttqt;
            vec::detail::Swizzler<ThisType, 1, 1, 3, 2> yywz, ggab, ttqp;
            vec::detail::Swizzler<ThisType, 1, 1, 3, 3> yyww, ggaa, ttqq;
            vec::detail::Swizzler<ThisType, 1, 2, 0, 0> yzxx, gbrr, tpss; // ----
            vec::detail::Swizzler<ThisType, 1, 2, 0, 1> yzxy, gbrg, tpst;
            vec::detail::Swizzler<ThisType, 1, 2, 0, 2> yzxz, gbrb, tpsp;
            vec::detail::Swizzler<ThisType, 1, 2, 0, 3> yzxw, gbra, tpsq;
            vec::detail::Swizzler<ThisType, 1, 2, 1, 0> yzyx, gbgr, tpts;
            vec::detail::Swizzler<ThisType, 1, 2, 1, 1> yzyy, gbgg, tptt;
            vec::detail::Swizzler<ThisType, 1, 2, 1, 2> yzyz, gbgb, tptp;
            vec::detail::Swizzler<ThisType, 1, 2, 1, 3> yzyw, gbga, tptq;
            vec::detail::Swizzler<ThisType, 1, 2, 2, 0> yzzx, gbbr, tpps;
            vec::detail::Swizzler<ThisType, 1, 2, 2, 1> yzzy, gbbg, tppt;
            vec::detail::Swizzler<ThisType, 1, 2, 2, 2> yzzz, gbbb, tppp;
            vec::detail::Swizzler<ThisType, 1, 2, 2, 3> yzzw, gbba, tppq;
            vec::detail::Swizzler<ThisType, 1, 2, 3, 0> yzwx, gbar, tpqs;
            vec::detail::Swizzler<ThisType, 1, 2, 3, 1> yzwy, gbag, tpqt;
            vec::detail::Swizzler<ThisType, 1, 2, 3, 2> yzwz, gbab, tpqp;
            vec::detail::Swizzler<ThisType, 1, 2, 3, 3> yzww, gbaa, tpqq;
            vec::detail::Swizzler<ThisType, 1, 3, 0, 0> ywxx, garr, tqss; // ----
            vec::detail::Swizzler<ThisType, 1, 3, 0, 1> ywxy, garg, tqst;
            vec::detail::Swizzler<ThisType, 1, 3, 0, 2> ywxz, garb, tqsp;
            vec::detail::Swizzler<ThisType, 1, 3, 0, 3> ywxw, gara, tqsq;
            vec::detail::Swizzler<ThisType, 1, 3, 1, 0> ywyx, gagr, tqts;
            vec::detail::Swizzler<ThisType, 1, 3, 1, 1> ywyy, gagg, tqtt;
            vec::detail::Swizzler<ThisType, 1, 3, 1, 2> ywyz, gagb, tqtp;
            vec::detail::Swizzler<ThisType, 1, 3, 1, 3> ywyw, gaga, tqtq;
            vec::detail::Swizzler<ThisType, 1, 3, 2, 0> ywzx, gabr, tqps;
            vec::detail::Swizzler<ThisType, 1, 3, 2, 1> ywzy, gabg, tqpt;
            vec::detail::Swizzler<ThisType, 1, 3, 2, 2> ywzz, gabb, tqpp;
            vec::detail::Swizzler<ThisType, 1, 3, 2, 3> ywzw, gaba, tqpq;
            vec::detail::Swizzler<ThisType, 1, 3, 3, 0> ywwx, gaar, tqqs;
            vec::detail::Swizzler<ThisType, 1, 3, 3, 1> ywwy, gaag, tqqt;
            vec::detail::Swizzler<ThisType, 1, 3, 3, 2> ywwz, gaab, tqqp;
            vec::detail::Swizzler<ThisType, 1, 3, 3, 3> ywww, gaaa, tqqq;

            vec::detail::Swizzler<ThisType, 2, 0, 0, 0> zxxx, brrr, psss;
            vec::detail::Swizzler<ThisType, 2, 0, 0, 1> zxxy, brrg, psst;
            vec::detail::Swizzler<ThisType, 2, 0, 0, 2> zxxz, brrb, pssp;
            vec::detail::Swizzler<ThisType, 2, 0, 0, 3> zxxw, brra, pssq;
            vec::detail::Swizzler<ThisType, 2, 0, 1, 0> zxyx, brgr, psts;
            vec::detail::Swizzler<ThisType, 2, 0, 1, 1> zxyy, brgg, pstt;
            vec::detail::Swizzler<ThisType, 2, 0, 1, 2> zxyz, brgb, pstp;
            vec::detail::Swizzler<ThisType, 2, 0, 1, 3> zxyw, brga, pstq;
            vec::detail::Swizzler<ThisType, 2, 0, 2, 0> zxzx, brbr, psps;
            vec::detail::Swizzler<ThisType, 2, 0, 2, 1> zxzy, brbg, pspt;
            vec::detail::Swizzler<ThisType, 2, 0, 2, 2> zxzz, brbb, pspp;
            vec::detail::Swizzler<ThisType, 2, 0, 2, 3> zxzw, brba, pspq;
            vec::detail::Swizzler<ThisType, 2, 0, 3, 0> zxwx, brar, psqs;
            vec::detail::Swizzler<ThisType, 2, 0, 3, 1> zxwy, brag, psqt;
            vec::detail::Swizzler<ThisType, 2, 0, 3, 2> zxwz, brab, psqp;
            vec::detail::Swizzler<ThisType, 2, 0, 3, 3> zxww, braa, psqq;
            vec::detail::Swizzler<ThisType, 2, 1, 0, 0> zyxx, bgrr, ptss; // ----
            vec::detail::Swizzler<ThisType, 2, 1, 0, 1> zyxy, bgrg, ptst;
            vec::detail::Swizzler<ThisType, 2, 1, 0, 2> zyxz, bgrb, ptsp;
            vec::detail::Swizzler<ThisType, 2, 1, 0, 3> zyxw, bgra, ptsq;
            vec::detail::Swizzler<ThisType, 2, 1, 1, 0> zyyx, bggr, ptts;
            vec::detail::Swizzler<ThisType, 2, 1, 1, 1> zyyy, bggg, pttt;
            vec::detail::Swizzler<ThisType, 2, 1, 1, 2> zyyz, bggb, pttp;
            vec::detail::Swizzler<ThisType, 2, 1, 1, 3> zyyw, bgga, pttq;
            vec::detail::Swizzler<ThisType, 2, 1, 2, 0> zyzx, bgbr, ptps;
            vec::detail::Swizzler<ThisType, 2, 1, 2, 1> zyzy, bgbg, ptpt;
            vec::detail::Swizzler<ThisType, 2, 1, 2, 2> zyzz, bgbb, ptpp;
            vec::detail::Swizzler<ThisType, 2, 1, 2, 3> zyzw, bgba, ptpq;
            vec::detail::Swizzler<ThisType, 2, 1, 3, 0> zywx, bgar, ptqs;
            vec::detail::Swizzler<ThisType, 2, 1, 3, 1> zywy, bgag, ptqt;
            vec::detail::Swizzler<ThisType, 2, 1, 3, 2> zywz, bgab, ptqp;
            vec::detail::Swizzler<ThisType, 2, 1, 3, 3> zyww, bgaa, ptqq;
            vec::detail::Swizzler<ThisType, 2, 2, 0, 0> zzxx, bbrr, ppss; // ----
            vec::detail::Swizzler<ThisType, 2, 2, 0, 1> zzxy, bbrg, ppst;
            vec::detail::Swizzler<ThisType, 2, 2, 0, 2> zzxz, bbrb, ppsp;
            vec::detail::Swizzler<ThisType, 2, 2, 0, 3> zzxw, bbra, ppsq;
            vec::detail::Swizzler<ThisType, 2, 2, 1, 0> zzyx, bbgr, ppts;
            vec::detail::Swizzler<ThisType, 2, 2, 1, 1> zzyy, bbgg, pptt;
            vec::detail::Swizzler<ThisType, 2, 2, 1, 2> zzyz, bbgb, pptp;
            vec::detail::Swizzler<ThisType, 2, 2, 1, 3> zzyw, bbga, pptq;
            vec::detail::Swizzler<ThisType, 2, 2, 2, 0> zzzx, bbbr, ppps;
            vec::detail::Swizzler<ThisType, 2, 2, 2, 1> zzzy, bbbg, pppt;
            vec::detail::Swizzler<ThisType, 2, 2, 2, 2> zzzz, bbbb, pppp;
            vec::detail::Swizzler<ThisType, 2, 2, 2, 3> zzzw, bbba, pppq;
            vec::detail::Swizzler<ThisType, 2, 2, 3, 0> zzwx, bbar, ppqs;
            vec::detail::Swizzler<ThisType, 2, 2, 3, 1> zzwy, bbag, ppqt;
            vec::detail::Swizzler<ThisType, 2, 2, 3, 2> zzwz, bbab, ppqp;
            vec::detail::Swizzler<ThisType, 2, 2, 3, 3> zzww, bbaa, ppqq;
            vec::detail::Swizzler<ThisType, 2, 3, 0, 0> zwxx, barr, pqss; // ----
            vec::detail::Swizzler<ThisType, 2, 3, 0, 1> zwxy, barg, pqst;
            vec::detail::Swizzler<ThisType, 2, 3, 0, 2> zwxz, barb, pqsp;
            vec::detail::Swizzler<ThisType, 2, 3, 0, 3> zwxw, bara, pqsq;
            vec::detail::Swizzler<ThisType, 2, 3, 1, 0> zwyx, bagr, pqts;
            vec::detail::Swizzler<ThisType, 2, 3, 1, 1> zwyy, bagg, pqtt;
            vec::detail::Swizzler<ThisType, 2, 3, 1, 2> zwyz, bagb, pqtp;
            vec::detail::Swizzler<ThisType, 2, 3, 1, 3> zwyw, baga, pqtq;
            vec::detail::Swizzler<ThisType, 2, 3, 2, 0> zwzx, babr, pqps;
            vec::detail::Swizzler<ThisType, 2, 3, 2, 1> zwzy, babg, pqpt;
            vec::detail::Swizzler<ThisType, 2, 3, 2, 2> zwzz, babb, pqpp;
            vec::detail::Swizzler<ThisType, 2, 3, 2, 3> zwzw, baba, pqpq;
            vec::detail::Swizzler<ThisType, 2, 3, 3, 0> zwwx, baar, pqqs;
            vec::detail::Swizzler<ThisType, 2, 3, 3, 1> zwwy, baag, pqqt;
            vec::detail::Swizzler<ThisType, 2, 3, 3, 2> zwwz, baab, pqqp;
            vec::detail::Swizzler<ThisType, 2, 3, 3, 3> zwww, baaa, pqqq;

            vec::detail::Swizzler<ThisType, 3, 0, 0, 0> wxxx, arrr, qsss;
            vec::detail::Swizzler<ThisType, 3, 0, 0, 1> wxxy, arrg, qsst;
            vec::detail::Swizzler<ThisType, 3, 0, 0, 2> wxxz, arrb, qssp;
            vec::detail::Swizzler<ThisType, 3, 0, 0, 3> wxxw, arra, qssq;
            vec::detail::Swizzler<ThisType, 3, 0, 1, 0> wxyx, argr, qsts;
            vec::detail::Swizzler<ThisType, 3, 0, 1, 1> wxyy, argg, qstt;
            vec::detail::Swizzler<ThisType, 3, 0, 1, 2> wxyz, argb, qstp;
            vec::detail::Swizzler<ThisType, 3, 0, 1, 3> wxyw, arga, qstq;
            vec::detail::Swizzler<ThisType, 3, 0, 2, 0> wxzx, arbr, qsps;
            vec::detail::Swizzler<ThisType, 3, 0, 2, 1> wxzy, arbg, qspt;
            vec::detail::Swizzler<ThisType, 3, 0, 2, 2> wxzz, arbb, qspp;
            vec::detail::Swizzler<ThisType, 3, 0, 2, 3> wxzw, arba, qspq;
            vec::detail::Swizzler<ThisType, 3, 0, 3, 0> wxwx, arar, qsqs;
            vec::detail::Swizzler<ThisType, 3, 0, 3, 1> wxwy, arag, qsqt;
            vec::detail::Swizzler<ThisType, 3, 0, 3, 2> wxwz, arab, qsqp;
            vec::detail::Swizzler<ThisType, 3, 0, 3, 3> wxww, araa, qsqq;
            vec::detail::Swizzler<ThisType, 3, 1, 0, 0> wyxx, agrr, qtss; // ----
            vec::detail::Swizzler<ThisType, 3, 1, 0, 1> wyxy, agrg, qtst;
            vec::detail::Swizzler<ThisType, 3, 1, 0, 2> wyxz, agrb, qtsp;
            vec::detail::Swizzler<ThisType, 3, 1, 0, 3> wyxw, agra, qtsq;
            vec::detail::Swizzler<ThisType, 3, 1, 1, 0> wyyx, aggr, qtts;
            vec::detail::Swizzler<ThisType, 3, 1, 1, 1> wyyy, aggg, qttt;
            vec::detail::Swizzler<ThisType, 3, 1, 1, 2> wyyz, aggb, qttp;
            vec::detail::Swizzler<ThisType, 3, 1, 1, 3> wyyw, agga, qttq;
            vec::detail::Swizzler<ThisType, 3, 1, 2, 0> wyzx, agbr, qtps;
            vec::detail::Swizzler<ThisType, 3, 1, 2, 1> wyzy, agbg, qtpt;
            vec::detail::Swizzler<ThisType, 3, 1, 2, 2> wyzz, agbb, qtpp;
            vec::detail::Swizzler<ThisType, 3, 1, 2, 3> wyzw, agba, qtpq;
            vec::detail::Swizzler<ThisType, 3, 1, 3, 0> wywx, agar, qtqs;
            vec::detail::Swizzler<ThisType, 3, 1, 3, 1> wywy, agag, qtqt;
            vec::detail::Swizzler<ThisType, 3, 1, 3, 2> wywz, agab, qtqp;
            vec::detail::Swizzler<ThisType, 3, 1, 3, 3> wyww, agaa, qtqq;
            vec::detail::Swizzler<ThisType, 3, 2, 0, 0> wzxx, abrr, qpss; // ----
            vec::detail::Swizzler<ThisType, 3, 2, 0, 1> wzxy, abrg, qpst;
            vec::detail::Swizzler<ThisType, 3, 2, 0, 2> wzxz, abrb, qpsp;
            vec::detail::Swizzler<ThisType, 3, 2, 0, 3> wzxw, abra, qpsq;
            vec::detail::Swizzler<ThisType, 3, 2, 1, 0> wzyx, abgr, qpts;
            vec::detail::Swizzler<ThisType, 3, 2, 1, 1> wzyy, abgg, qptt;
            vec::detail::Swizzler<ThisType, 3, 2, 1, 2> wzyz, abgb, qptp;
            vec::detail::Swizzler<ThisType, 3, 2, 1, 3> wzyw, abga, qptq;
            vec::detail::Swizzler<ThisType, 3, 2, 2, 0> wzzx, abbr, qpps;
            vec::detail::Swizzler<ThisType, 3, 2, 2, 1> wzzy, abbg, qppt;
            vec::detail::Swizzler<ThisType, 3, 2, 2, 2> wzzz, abbb, qppp;
            vec::detail::Swizzler<ThisType, 3, 2, 2, 3> wzzw, abba, qppq;
            vec::detail::Swizzler<ThisType, 3, 2, 3, 0> wzwx, abar, qpqs;
            vec::detail::Swizzler<ThisType, 3, 2, 3, 1> wzwy, abag, qpqt;
            vec::detail::Swizzler<ThisType, 3, 2, 3, 2> wzwz, abab, qpqp;
            vec::detail::Swizzler<ThisType, 3, 2, 3, 3> wzww, abaa, qpqq;
            vec::detail::Swizzler<ThisType, 3, 3, 0, 0> wwxx, aarr, qqss; // ----
            vec::detail::Swizzler<ThisType, 3, 3, 0, 1> wwxy, aarg, qqst;
            vec::detail::Swizzler<ThisType, 3, 3, 0, 2> wwxz, aarb, qqsp;
            vec::detail::Swizzler<ThisType, 3, 3, 0, 3> wwxw, aara, qqsq;
            vec::detail::Swizzler<ThisType, 3, 3, 1, 0> wwyx, aagr, qqts;
            vec::detail::Swizzler<ThisType, 3, 3, 1, 1> wwyy, aagg, qqtt;
            vec::detail::Swizzler<ThisType, 3, 3, 1, 2> wwyz, aagb, qqtp;
            vec::detail::Swizzler<ThisType, 3, 3, 1, 3> wwyw, aaga, qqtq;
            vec::detail::Swizzler<ThisType, 3, 3, 2, 0> wwzx, aabr, qqps;
            vec::detail::Swizzler<ThisType, 3, 3, 2, 1> wwzy, aabg, qqpt;
            vec::detail::Swizzler<ThisType, 3, 3, 2, 2> wwzz, aabb, qqpp;
            vec::detail::Swizzler<ThisType, 3, 3, 2, 3> wwzw, aaba, qqpq;
            vec::detail::Swizzler<ThisType, 3, 3, 3, 0> wwwx, aaar, qqqs;
            vec::detail::Swizzler<ThisType, 3, 3, 3, 1> wwwy, aaag, qqqt;
            vec::detail::Swizzler<ThisType, 3, 3, 3, 2> wwwz, aaab, qqqp;
            vec::detail::Swizzler<ThisType, 3, 3, 3, 3> wwww, aaaa, qqqq;
        };
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
    } // namespace glsl

} // namespace oglml

#endif // OGLML_VEC_HPP
