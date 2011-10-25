/*
Needs to be compiled with -std=c++0x and a compiler which supports unrestricted unions,
e.g. GCC 4.6
*/

#ifndef NEON_CORE_VEC_
#define NEON_CORE_VEC_

#ifndef NO_NEON
#include <neon/core/def.h>
#endif

#include <iostream>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <tuple>

#ifndef NO_NEON
#include <neon/core/select.h>
NEON_CORE_BEGIN_NAMESPACE_;
#endif

#define NEON_DIMENSION_CHECK(n1_, n2_) \
static_assert(n1_ == n2_, "Incompatible dimensionality between vectors.");

#define NEON_MIN_DIMENSION_CHECK(n1_, minN_) \
static_assert(n1_ >= minN_, "Too few dimensions.");

#define NEON_MAX_DIMENSION_CHECK(n1_, maxN_) \
static_assert(n1_ <= maxN_, "Too many dimensions.");

namespace vec {
    template <std::size_t n, typename T>
    class DefaultStorage;
}

template <std::size_t n, typename T, class SP = vec::DefaultStorage<n, T> >
union Vec;

namespace vec {

#ifndef NEON_COMPILATION
// Type selection
template <bool flag, typename T, typename U>
struct Select {
    typedef T Result;
};

template <typename T, typename U>
struct Select<false, T, U> {
    typedef U Result;
};
#endif

template <typename T>
struct True {
    constexpr static bool value = true;
};

// Policies
template <std::size_t n, typename T>
class DefaultStorage {
    T data_[n];

public:
    inline T& operator[](std::size_t i) {
        return data_[i];
    }

    inline const T& operator[](std::size_t i) const {
        return data_[i];
    }
};

// Ops
struct Plus {
    template <typename T1, typename T2>
    static inline T1 op(const T1& a, const T2& b) {
        return a + b;
    }

    template <typename T1, typename T2>
    static inline T1& opTo(T1& a, const T2& b) {
        return a += b;
    }
};

struct Minus {
    template <typename T1, typename T2>
    static inline T1 op(const T1& a, const T2& b) {
        return a - b;
    }

    template <typename T1, typename T2>
    static inline T1& opTo(T1& a, const T2& b) {
        return a -= b;
    }
};

struct Mult {
    template <typename T1, typename T2>
    static inline T1 op(const T1& a, const T2& b) {
        return a * b;
    }

    template <typename T1, typename T2>
    static inline T1& opTo(T1& a, const T2& b) {
        return a *= b;
    }
};

struct Div {
    template <typename T1, typename T2>
    static inline T1 op(const T1& a, const T2& b) {
        return a / b;
    }

    template <typename T1, typename T2>
    static inline T1& opTo(T1& a, const T2& b) {
        return a /= b;
    }
};


// Forward declarations
template <bool isArray>
struct Assignment;

template <bool isArray>
struct Comparison;

template <bool isArray, class Op>
struct Operation;


// Private namespace
class Private {
    // --------------------------------------------------------
    // --------------------------------------------------------
    template <std::size_t i>
    struct Index {
        constexpr static std::size_t n = i;
    };

    template <typename I, std::size_t... indices>
    struct IndexEntry;

    template <std::size_t first, std::size_t... indices>
    struct IndexEntry<Index<0>, first, indices...> {
        constexpr static std::size_t n = first;
    };

    template <typename I, std::size_t first, std::size_t... indices>
    struct IndexEntry<I, first, indices...> {
        constexpr static std::size_t n = IndexEntry<Index<I::n - 1>, indices...>::n;
    };

    // --------------------------------------------------------------
    template <std::size_t i>
    struct NDim {
        constexpr static std::size_t n = i;
    };

    template <typename N, std::size_t... indices>
    struct Check {
        constexpr static bool passed = true;
    };

    struct Fail {
        constexpr static bool passed = false;
    };

    template <typename N, std::size_t first, std::size_t... indices>
    struct Check<N, first, indices...> {
        constexpr static bool thisPassed = first < N::n;
        constexpr static bool passed = Select<thisPassed, Check<N, indices...>, Fail>::Result::passed;
    };

    // -------------------------------------------------
    template <std::size_t n, typename T, class SP, std::size_t... indices>
    struct View {

        constexpr static std::size_t nIndices = sizeof...(indices);
        static const std::size_t cindices[nIndices]; // GCC 4.7 needed maybe, could be a constexpr

        constexpr static unsigned indexAt(std::size_t i) {
            return cindices[i];
        }

        template <std::size_t i>
        struct IndexAt {
            constexpr static std::size_t result = IndexEntry<Index<i>, indices...>::n;
            static_assert(result < n, "Index overflow.");
        };

        constexpr static bool validIndices = Check<NDim<n>, indices...>::passed;

        // The compile time one
        template <std::size_t i>
        T& get() {
            NEON_MAX_DIMENSION_CHECK(i + 1, n);
            static_assert(validIndices, "Invalid indices.");
            return (*reinterpret_cast<Vec<n, T, SP>*>(this)).get<IndexAt<i>::result>();
        }
        // |
        template <std::size_t i>
        const T& get() const {
            NEON_MAX_DIMENSION_CHECK(i + 1, n);
            static_assert(validIndices, "Invalid indices.");
            return (*reinterpret_cast<const Vec<n, T, SP>*>(this)).get<IndexAt<i>::result>();
        }

        // The runtime one
        T& operator[](std::size_t i) {
            static_assert(validIndices, "Invalid indices.");
            assert(i < nIndices);
            return (*reinterpret_cast<Vec<n, T, SP>*>(this))[indexAt(i)];
        }
        // |
        const T& operator[](std::size_t i) const {
            static_assert(validIndices, "Invalid indices.");
            assert(i < nIndices);
            return (*reinterpret_cast<const Vec<n, T, SP>*>(this))[indexAt(i)];
        }

        // ------------------------------------------------------
        // Convert back (only for views with one index)
        inline operator T&() {
            static_assert(nIndices == 1, "Only views with one index can be converted back.");
            return (*reinterpret_cast<Vec<n, T, SP>*>(this)).get<IndexAt<0>::result>();
        }

        inline operator const T&() const {
            static_assert(nIndices == 1, "Only views with one index can be converted back.");
            return (*reinterpret_cast<const Vec<n, T, SP>*>(this)).get<IndexAt<0>::result>();
        }

        // =
        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1>
        inline View<n, T, SP, indices...>& operator=(const View<n1, T1, SP1, indices1...>& v) {
            NEON_DIMENSION_CHECK(nIndices, v.nIndices);
            for (std::size_t i = 0; i < nIndices; ++i)
                (*this)[i] = v[i];
            return *this;
        }

        template <std::size_t n1, typename T1, class SP1>
        inline View<n, T, SP, indices...>& operator=(const Vec<n1, T1, SP1>& v) {
            NEON_DIMENSION_CHECK(nIndices, n1);
            for (std::size_t i = 0; i < nIndices; ++i)
                (*this)[i] = v[i];
            return *this;
        }

        template <typename T1>
        inline View<n, T, SP, indices...>& operator=(const T1& v) {
            return Assignment<std::is_pointer<T1>::value || std::is_array<T1>::value>::assign(*this, v);
        }

        // Negation
        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1>
        friend inline Vec<sizeof...(indices1), T1, SP1> operator-(const View<n1, T1, SP1, indices1...>& other);

        // == & !=
        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2, std::size_t... indices2>
        friend inline bool operator==(const View<n1, T1, SP1, indices1...>& a, const View<n2, T2, SP2, indices2...>& b);

        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2, std::size_t... indices2>
        friend inline bool operator!=(const View<n1, T1, SP1, indices1...>& a, const View<n2, T2, SP2, indices2...>& b);


        template <std::size_t n1, typename T1, std::size_t... indices1, std::size_t n2, typename T2, class SP1>
        friend inline bool operator==(const View<n1, T1, SP1, indices1...>& a, const Vec<n2, T2, SP1>& b);

        template <std::size_t n1, typename T1, std::size_t... indices1, std::size_t n2, typename T2, class SP1>
        friend inline bool operator==(const Vec<n2, T2, SP1>& a, const View<n1, T1, SP1, indices1...>& b);

        template <std::size_t n1, typename T1, std::size_t... indices1, std::size_t n2, typename T2, class SP1>
        friend inline bool operator!=(const View<n1, T1, SP1, indices1...>& a, const Vec<n2, T2, SP1>& b);

        template <std::size_t n1, typename T1, std::size_t... indices1, std::size_t n2, typename T2, class SP1>
        friend inline bool operator!=(const Vec<n2, T2, SP1>& a, const View<n1, T1, SP1, indices1...>& b);


        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
        friend inline bool operator==(const View<n1, T1, SP1, indices1...>& a, const T2& b);

        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
        friend inline bool operator==(const T2& a, const View<n1, T1, SP1, indices1...>& b);

        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
        friend inline bool operator!=(const View<n1, T1, SP1, indices1...>& a, const T2& b);

        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
        friend inline bool operator!=(const T2& a, const View<n1, T1, SP1, indices1...>& b);


        // +
        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2, std::size_t... indices2>
        friend inline Vec<sizeof...(indices1), T1, vec::DefaultStorage<sizeof...(indices1), T1> > operator+(const View<n1, T1, SP1, indices1...>& a, const View<n2, T2, SP2, indices2...>& b);

        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
        friend inline Vec<sizeof...(indices1), T1, vec::DefaultStorage<sizeof...(indices1), T1> > operator+(const View<n1, T1, SP1, indices1...>& a, const T2& b);

        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
        friend inline Vec<sizeof...(indices1), T1, vec::DefaultStorage<sizeof...(indices1), T1> > operator+(const T2& a, const View<n1, T1, SP1, indices1...>& b);

        template <std::size_t n1, typename T1, class SP1, std::size_t... indices1>
        inline View<n, T, SP, indices...>& operator+=(const View<n1, T1, SP1, indices1...>& other) {
            NEON_DIMENSION_CHECK(nIndices, other.nIndices);
            return Operation
                    <True<T1>::value, Plus>::addTo(*this, other);
        }

        template <std::size_t n1, typename T1, class SP1>
        inline View<n, T, SP, indices...>& operator+=(const Vec<n1, T1, SP1>& other) {
            NEON_DIMENSION_CHECK(nIndices, n1);
            return Operation
                    <True<T1>::value, Plus>::addTo(*this, other);
        }

        template <typename T1>
        inline View<n, T, SP, indices...>& operator+=(const T1& other) {
            return Operation
                    <std::is_pointer<T1>::value ||
                    std::is_array<T1>::value, Plus>::addTo(*this, other);
        }
    };

    template <std::size_t, typename> friend union Vec;
};


// --------------------------------------------
template <std::size_t begin, bool isArray>
struct VecUnknownTypeAssignment {
    template <std::size_t n1, typename T1, class SP1, typename First, typename... Args>
    static inline void assign(Vec<n1, T1, SP1>& target, const First& first,
                              const Args&... args);
};

template <std::size_t begin>
struct VecUnknownTypeAssignment<begin, true> {
    template <std::size_t n1, typename T1, class SP1, typename First, typename... Args>
    static inline void assign(Vec<n1, T1, SP1>& target, const First& first,
                              const Args&... args);
};

template <std::size_t begin>
struct VecAssignment {
    template <std::size_t n1, typename T1, class SP1, typename... Args>
    static inline void assign(Vec<n1, T1, SP1>& target, Args... args) {
        static_assert(begin == n1, "Not enaugh constructor parameters.");
    }

    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2,
              typename... Args>
    static inline void assign(Vec<n1, T1, SP1>& target, const Vec<n2, T2, SP2>& first,
                              const Args&... args) {
        static_assert(begin + (n2 - 1) < n1, "Too many constructor parameters.");
        for (std::size_t i = 0; i < n2; ++i)
            target[begin + i] = first[i];
        VecAssignment<begin + n2>::assign(target, args...);
    }

    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2,
              std::size_t... indices, typename... Args>
    static inline void assign(Vec<n1, T1, SP1>& target, const vec::Private::View<n2, T2, SP2,
                              indices...>& first, const Args&... args) {
        static_assert(begin + (first.nIndices - 1) < n1, "Too many constructor parameters.");
        for (std::size_t i = 0; i < first.nIndices; ++i)
            target[begin + i] = first[i];
        VecAssignment<begin + first.nIndices>::assign(target, args...);
    }

    template <std::size_t n1, typename T1, class SP1, typename First, typename... Args>
    static inline void assign(Vec<n1, T1, SP1>& target, const First& first,
                              const Args&... args) {
        static_assert(begin < n1, "Too many constructor parameters.");
        VecUnknownTypeAssignment
                <begin, std::is_pointer<First>::value ||
                std::is_array<First>::value>::assign(target, first, args...);
    }
};

template <std::size_t begin, bool isArray>
template <std::size_t n1, typename T1, class SP1, typename First, typename... Args>
inline void VecUnknownTypeAssignment<begin, isArray>::assign(Vec<n1, T1, SP1>& target,
                                                             const First& first,
                                                             const Args&... args) {
    static_assert(begin < n1, "Too many constructor parameters.");
    target[begin] = first;
    VecAssignment<begin + 1>::assign(target, args...);
}

template <std::size_t begin>
template <std::size_t n1, typename T1, class SP1, typename First, typename... Args>
inline void VecUnknownTypeAssignment<begin, true>::assign(Vec<n1, T1, SP1>& target,
                                                          const First& first,
                                                          const Args&... args) {
    static_assert(begin < n1, "Too many constructor parameters.");
    static_assert(sizeof...(Args) == 0,
                  "If you assign an array to a vector, "\
                  "it has to be the last parameter, because the length cannot be determined.");
    for (std::size_t i = 0; i < n1 - begin; ++i)
        target[begin + i] = first[i];
}

// ------------------------------------------
template <bool isArray>
struct Assignment {
    template <std::size_t n1, typename T1, class SP1, typename T2>
    static inline Vec<n1, T1, SP1>& assign(Vec<n1, T1, SP1>& v, const T2& other) {
        for (std::size_t i = 0; i < n1; ++i)
            v[i] = other;
        return v;
    }

    template <std::size_t n1, typename T1, class SP1, std::size_t... indices, typename T2>
    static inline Private::View<n1, T1, SP1, indices...>& assign(Private::View<n1, T1, SP1, indices...>& v, const T2& other) {
        for (std::size_t i = 0; i < v.nIndices; ++i)
            v[i] = other;
        return v;
    }
};

template <>
struct Assignment<true> {
    template <std::size_t n1, typename T1, class SP1, typename T2>
    static inline Vec<n1, T1, SP1>& assign(Vec<n1, T1, SP1>& v, const T2& other) {
        for (std::size_t i = 0; i < n1; ++i)
            v[i] = other[i];
        return v;
    }


    template <std::size_t n1, typename T1, class SP1, std::size_t... indices, typename T2>
    static inline Private::View<n1, T1, SP1, indices...>& assign(Private::View<n1, T1, SP1, indices...>& v, const T2& other) {
        for (std::size_t i = 0; i < v.nIndices; ++i)
            v[i] = other[i];
        return v;
    }
};

// ---------------------------------------------
template <bool isArray>
struct Comparison {
    template <std::size_t n1, typename T1, class SP1, typename T2>
    static bool compare(const Vec<n1, T1, SP1>& v, const T2& other) {
        for (std::size_t i = 0; i < n1; ++i)
            if (v[i] != other)
                return false;
        return true;
    }

    template <std::size_t n1, typename T1, class SP1, std::size_t... indices, typename T2>
    static inline bool compare(const Private::View<n1, T1, SP1, indices...>& v, const T2& other) {
        for (std::size_t i = 0; i < v.nIndices; ++i)
            if (v[i] != other)
                return false;
        return true;
    }
};

template <>
struct Comparison<true> {
    template <std::size_t n1, typename T1, class SP1, typename T2>
    static inline bool compare(const Vec<n1, T1, SP1>& v, const T2& other) {
        for (std::size_t i = 0; i < n1; ++i)
            if (v[i] != other[i])
                return false;
        return true;
    }

    template <std::size_t n1, typename T1, class SP1, std::size_t... indices, typename T2>
    static inline bool compare(const Private::View<n1, T1, SP1, indices...>& v, const T2& other) {
        for (std::size_t i = 0; i < v.nIndices; ++i)
            if (v[i] != other[i])
                return false;
        return true;
    }
};





























template <typename T>
struct IsArray {
    constexpr static bool value = std::is_pointer<T>::value;
};

template <std::size_t n, typename T, class SP>
struct IsArray<Vec<n, T, SP> > {
    constexpr static bool value = true;
};

template <std::size_t n, typename T, class SP, std::size_t... indices>
struct IsArray<Private::View<n, T, SP, indices...> > {
    constexpr static bool value = true;
};

// -----
template <class Operator, std::size_t count, bool isArray>
struct LoopOperation {

    template <typename T1, typename T2, typename T3>
    static T1& go(T1& v, const T2& a, const T3& b) {
        for (std::size_t i = 0; i < count; ++i)
            v[i] = Operator::op(a[i], b);
        return v;
    }

};

template <class Operator, std::size_t count>
struct LoopOperation<Operator, count, true> {

    template <typename T1, typename T2, typename T3>
    static T1& go(T1& v, const T2& a, const T3& b) {
        for (std::size_t i = 0; i < count; ++i)
            v[i] = Operator::op(a[i], b[i]);
        return v;
    }

};

// -----
template <class Operator, std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
Vec<n2, T2> operate(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b) {
    NEON_DIMENSION_CHECK(n1, n2);
    Vec<n2, T2> v;
    return LoopOperation<Operator, n2, true>::go(v, a, b);
}
































// ---------------------------------------------
template <bool isArray, class Op>
struct Operation {
    template <std::size_t count, typename T1, typename T2>
    static inline T1& loopOpTo(T1& a, const T2& b) {
        for (std::size_t i = 0; i < count; ++i)
            Op::opTo(a[i], b);
        return a;
    }

    template <std::size_t count, typename T1, typename T2, typename T3>
    static inline T1& loopOp(T1& a, const T2& b, const T3& c) {
        for (std::size_t i = 0; i < count; ++i)
            a[i] = Op::op(b[i], c);
        return a;
    }


    // ----
    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> op(const Vec<n1, T1, SP>& a, const T2& b) {
        Vec<n1, T1, SP> v;
        return loopOp<n1>(v, a, b);
    }

    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> op(const T2& a, const Vec<n1, T1, SP>& b) {
        Vec<n1, T1, SP> v;
        return loopOp<n1>(v, a, b);
    }

    // ----
    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > op
    (const Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        Vec<a.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        return loopOp<a.nIndices>(v, a, b);
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > op
    (const T2& a, const Private::View<n1, T1, SP, indices...>& b) {
        Vec<b.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        return loopOp<b.nIndices>(v, a, b);
    }

    // ----
    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP>& opTo(Vec<n1, T1, SP>& a, const T2& b) {
        return loopOpTo<n1>(a, b);
    }

    // ----
    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Private::View<n1, T1, SP, indices...>& opTo
    (Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        return loopOpTo<a.nIndices>(a, b);
    }
};

template <class Op>
struct Operation<true, Op> {
    template <std::size_t count, typename T1, typename T2>
    static inline T1& loopOpTo(T1& a, const T2& b) {
        for (std::size_t i = 0; i < count; ++i)
            Op::opTo(a[i], b[i]);
        return a;
    }

    template <std::size_t count, typename T1, typename T2, typename T3>
    static inline T1& loopOp(T1& a, const T2& b, const T3& c) {
        for (std::size_t i = 0; i < count; ++i)
            a[i] = Op::op(b[i], c[i]);
        return a;
    }


    // ----
    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
    static inline Vec<n1, T1, SP1> op(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b) {
        NEON_DIMENSION_CHECK(n1, n2);
        Vec<n1, T1, SP1> v;
        return loopOp<n1>(v, a, b);
    }

    // ----
    template <std::size_t n1, typename T1, class SP1, std::size_t... indices,
              std::size_t n2, typename T2, class SP2>
    static inline Vec<n2, T1, vec::DefaultStorage<n2, T1> > op
    (const Private::View<n1, T1, SP1, indices...> &a, const Vec<n2, T2, SP2>& b) {
        NEON_DIMENSION_CHECK(a.nIndices, n2);
        Vec<n2, T1, vec::DefaultStorage<n2, T1> > v;
        return loopOp<a.nIndices>(v, a, b);
    }

    template <std::size_t n1, typename T1, class SP1, std::size_t... indices,
              std::size_t n2, typename T2, class SP2>
    static inline Vec<n2, T1, vec::DefaultStorage<n2, T1> > op
    (const Vec<n2, T2, SP2>& a, const Private::View<n1, T1, SP1, indices...> &b) {
        NEON_DIMENSION_CHECK(n2, b.nIndices);
        Vec<n2, T1, vec::DefaultStorage<n2, T1> > v;
        return loopOp<b.nIndices>(v, a, b);
    }


    // ----
    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > op
    (const Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        Vec<a.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        return loopOp<a.nIndices>(v, a, b);
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > op
    (const T2& a, const Private::View<n1, T1, SP, indices...>& b) {
        Vec<b.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        return loopOp<b.nIndices>(v, a, b);
    }

    // ----
    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> op(const Vec<n1, T1, SP>& a, const T2& b) {
        Vec<n1, T1, SP> v;
        return loopOp<n1>(v, a, b);
    }

    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> op(const T2& a, const Vec<n1, T1, SP>& b) {
        Vec<n1, T1, SP> v;
        return loopOp<n1>(v, a, b);
    }

    // ----
    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP>& opTo(Vec<n1, T1, SP>& a, const T2& b) {
        return loopOpTo<n1>(a, b);
    }

    // ----
    template <std::size_t n1, typename T1, class SP1,
              std::size_t n2, typename T2, class SP2, std::size_t... indices>
    static inline Vec<n1, T1, SP1>& opTo
    (Vec<n1, T1, SP1>& a, const Private::View<n2, T2, SP2, indices...> &b) {
        NEON_DIMENSION_CHECK(n1, b.nIndices);
        return loopOpTo<n1>(a, b);
    }

    template <std::size_t n1, typename T1, class SP1,
              std::size_t n2, typename T2, class SP2, std::size_t... indices>
    static inline Vec<n1, T1, SP1>& opTo
    (const Private::View<n2, T2, SP2, indices...> &a, Vec<n1, T1, SP1>& b) {
        NEON_DIMENSION_CHECK(n1, a.nIndices);
        return loopOpTo<n1>(a, b);
    }

    // ----
    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Private::View<n1, T1, SP, indices...>& opTo
    (Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        return loopOpTo<a.nIndices>(a, b);
    }
};

// ---------------------------------------------
template <bool isArray>
struct Addition {
    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> add(const Vec<n1, T1, SP>& a, const T2& b) {
        Vec<n1, T1, SP> v;
        for (std::size_t i = 0; i < n1; ++i)
            v[i] = a[i] + b;
        return v;
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > add(const Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        Vec<a.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        for (std::size_t i = 0; i < a.nIndices; ++i)
            v[i] = a[i] + b;
        return v;
    }

    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP>& addTo(Vec<n1, T1, SP>& a, const T2& b) {
        for (std::size_t i = 0; i < n1; ++i)
            a[i] += b;
        return a;
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Private::View<n1, T1, SP, indices...>& addTo(Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        for (std::size_t i = 0; i < a.nIndices; ++i)
            a[i] += b;
        return a;
    }
};

template <>
struct Addition<true> {
    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> add(const Vec<n1, T1, SP>& a, const T2& b) {
        Vec<n1, T1, SP> v;
        for (std::size_t i = 0; i < n1; ++i)
            v[i] = a[i] + b[i];
        return v;
    }


    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > add(const Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        Vec<a.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        for (std::size_t i = 0; i < a.nIndices; ++i)
            v[i] = a[i] + b[i];
        return v;
    }

    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP>& addTo(Vec<n1, T1, SP>& a, const T2& b) {
        for (std::size_t i = 0; i < n1; ++i)
            a[i] += b[i];
        return a;
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Private::View<n1, T1, SP, indices...>& addTo(Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        for (std::size_t i = 0; i < a.nIndices; ++i)
            a[i] += b[i];
        return a;
    }
};

// ---------------------------------------------
template <bool isArray>
struct Substraction {
    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> substract(const Vec<n1, T1, SP>& a, const T2& b) {
        Vec<n1, T1, SP> v;
        for (std::size_t i = 0; i < n1; ++i)
            v[i] = a[i] - b;
        return v;
    }

    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> substract(const T2& a, const Vec<n1, T1, SP>& b) {
        Vec<n1, T1, SP> v;
        for (std::size_t i = 0; i < n1; ++i)
            v[i] = a - b[i];
        return v;
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > substract(const Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        Vec<a.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        for (std::size_t i = 0; i < a.nIndices; ++i)
            v[i] = a[i] - b;
        return v;
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > substract(const T2& a, const Private::View<n1, T1, SP, indices...>& b) {
        Vec<a.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        for (std::size_t i = 0; i < a.nIndices; ++i)
            v[i] = a - b[i];
        return v;
    }

    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP>& substractFrom(Vec<n1, T1, SP>& a, const T2& b) {
        for (std::size_t i = 0; i < n1; ++i)
            a[i] -= b;
        return a;
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Private::View<n1, T1, SP, indices...>& substractFrom(Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        for (std::size_t i = 0; i < a.nIndices; ++i)
            a[i] -= b;
        return a;
    }
};

template <>
struct Substraction<true> {
    template <std::size_t n1, typename T1, class SP1, typename T2, class SP2>
    static inline Vec<n1, T1, SP1> substract(const Vec<n1, T1, SP1>& a, const Vec<n1, T2, SP2>& b) {
        Vec<n1, T1, SP1> v;
        for (std::size_t i = 0; i < n1; ++i)
            v[i] = a[i] - b[i];
        return v;
    }

    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> substract(const Vec<n1, T1, SP>& a, const T2& b) {
        Vec<n1, T1, SP> v;
        for (std::size_t i = 0; i < n1; ++i)
            v[i] = a[i] - b[i];
        return v;
    }

    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP> substract(const T2& a, const Vec<n1, T1, SP>& b) {
        Vec<n1, T1, SP> v;
        for (std::size_t i = 0; i < n1; ++i)
            v[i] = a[i] - b[i];
        return v;
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > substract(const Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        Vec<a.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        for (std::size_t i = 0; i < a.nIndices; ++i)
            v[i] = a[i] - b[i];
        return v;
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Vec<sizeof...(indices), T1, vec::DefaultStorage<sizeof...(indices), T1> > substract(const T2& a, const Private::View<n1, T1, SP, indices...>& b) {
        Vec<a.nIndices, T1, vec::DefaultStorage<sizeof...(indices), T1> > v;
        for (std::size_t i = 0; i < a.nIndices; ++i)
            v[i] = a[i] - b[i];
        return v;
    }

    template <std::size_t n1, typename T1, class SP, typename T2>
    static inline Vec<n1, T1, SP>& substractFrom(Vec<n1, T1, SP>& a, const T2& b) {
        for (std::size_t i = 0; i < n1; ++i)
            a[i] -= b[i];
        return a;
    }

    template <std::size_t n1, typename T1, class SP,  std::size_t... indices, typename T2>
    static inline Private::View<n1, T1, SP, indices...>& substractFrom(Private::View<n1, T1, SP, indices...>& a, const T2& b) {
        for (std::size_t i = 0; i < a.nIndices; ++i)
            a[i] -= b[i];
        return a;
    }
};

template <std::size_t n, typename T, class SP, std::size_t... indices>
const std::size_t Private::View<n, T, SP, indices...>::cindices[] = { indices... };


// View global operators
// Negation
template <std::size_t n1, typename T1, class SP1, std::size_t...indices1>
inline Vec<sizeof...(indices1), T1, SP1> operator-(const Private::View<n1, T1, SP1, indices1...>& other) {
    Vec<other.nIndices, T1, vec::DefaultStorage<sizeof...(indices1), T1> > v;
    for (std::size_t i = 0; i < other.nIndices; ++i)
        v[i] = -other[i];
    return v;
}

// == & !=
template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2, std::size_t... indices2>
inline bool operator==(const Private::View<n1, T1, SP1, indices1...>& a, const Private::View<n2, T2, SP2, indices2...>& b) {
    NEON_DIMENSION_CHECK(a.nIndices, b.nIndices);
    return Comparison<true>::compare(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2, std::size_t... indices2>
inline bool operator!=(const Private::View<n1, T1, SP1, indices1...>& a, const Private::View<n2, T2, SP2, indices2...>& b) {
    return !(a == b);
}


template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2>
inline bool operator==(const Private::View<n1, T1, SP1, indices1...>& a, const Vec<n2, T2, SP2>& b) {
    NEON_DIMENSION_CHECK(a.nIndices, n2);
    return Comparison<true>::compare(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2>
inline bool operator==(const Vec<n2, T2, SP2>& a, const Private::View<n1, T1, SP1, indices1...>& b) {
    return (b == a);
}

template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2>
inline bool operator!=(const Private::View<n1, T1, SP1, indices1...>& a, const Vec<n2, T2, SP2>& b) {
    return !(a == b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2>
inline bool operator!=(const Vec<n2, T2, SP2>& a, const Private::View<n1, T1, SP1, indices1...>& b) {
    return !(a == b);
}


template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
inline bool operator==(const Private::View<n1, T1, SP1, indices1...>& a, const T2& b) {
    return Comparison
            <std::is_pointer<T2>::value || std::is_array<T2>::value>::compare(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
inline bool operator==(const T2& a, const Private::View<n1, T1, SP1, indices1...>& b) {
    return b == a;
}

template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
inline bool operator!=(const Private::View<n1, T1, SP1, indices1...>& a, const T2& b) {
    return !(a == b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
inline bool operator!=(const T2& a, const Private::View<n1, T1, SP1, indices1...>& b) {
    return !(a == b);
}

// +
template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, std::size_t n2, typename T2, class SP2, std::size_t... indices2>
inline Vec<sizeof...(indices1), T1, SP1> operator+(const Private::View<n1, T1, SP1, indices1...>& a, const Private::View<n2, T2, SP2, indices2...>& b) {
    NEON_DIMENSION_CHECK(a.nIndices, b.nIndices);
    return Addition
            <True<T1>::value>::add(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
inline Vec<sizeof...(indices1), T1, vec::DefaultStorage<sizeof...(indices1), T1> > operator+(const Private::View<n1, T1, SP1, indices1...>& a, const T2& b) {
    return Addition
            <std::is_pointer<T2>::value || std::is_array<T2>::value>::add(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t... indices1, typename T2>
inline Vec<sizeof...(indices1), T1, vec::DefaultStorage<sizeof...(indices1), T1> > operator+(const T2& a, const Private::View<n1, T1, SP1, indices1...>& b) {
    return b + a;
}

}











// ******************************************************************************* //
// ******************************************************************************* //
// ******************************************************************************* //
template <std::size_t n, typename T, class SP>
union Vec {
public:
    inline Vec();

    template <typename T1, class SP1>
    inline Vec(const Vec<n, T1, SP1>& other);
    
    template <std::size_t n1, typename T1, class SP1>
    explicit inline Vec(const Vec<n1, T1, SP1>& other);

    template <typename... Args>
    inline Vec(const Args&... args);

    inline ~Vec();

    // ******************************************************************************* //
    // Operator declarations

    // [] --------------------------------------------------------------------
    inline T& operator[](std::size_t i);
    inline const T& operator[](std::size_t i) const;

    // = -------------------------------------------------------------------
    inline Vec<n, T, SP>& operator=(const Vec<n, T, SP>& other);

    template <std::size_t n1, typename T1, class SP1>
    inline Vec<n, T, SP>& operator=(const Vec<n1, T1, SP1>& other);

    template <typename T1>
    inline Vec<n, T, SP>& operator=(const T1& other);

    // Negation
    template <std::size_t n1, typename T1, class SP1>
    friend inline Vec<n1, T1, SP1> operator-(const Vec<n1, T1, SP1>& other);

    // + --------------------------------------------------------------------------------
    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
    friend inline Vec<n1, T1, SP1> operator+(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b);

    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2, std::size_t... indices>
    friend inline Vec<n1, T1, SP1> operator+(const Vec<n1, T1, SP1>& a, const vec::Private::View<n2, T2, SP2, indices...>& b);

    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2, std::size_t... indices>
    friend inline Vec<n1, T1, SP1> operator+(const vec::Private::View<n2, T2, SP2, indices...>& a, const Vec<n1, T1, SP1>& b);

    template <std::size_t n1, typename T1, class SP1, typename T2>
    friend inline Vec<n1, T1, SP1> operator+(const Vec<n1, T1, SP1>& a, const T2& b);

    template <std::size_t n1, typename T1, class SP1, typename T2>
    friend inline Vec<n1, T1, SP1> operator+(const T2& a, const Vec<n1, T1, SP1>& b);

    template <std::size_t n1, typename T1, class SP1>
    inline Vec<n, T, SP>& operator+=(const Vec<n1, T1, SP1>& other);

    template <std::size_t n1, typename T1, class SP1, std::size_t... indices>
    inline Vec<n, T, SP>& operator+=(const vec::Private::View<n1, T1, SP1, indices...>& other);

    template <typename T1>
    inline Vec<n, T, SP>& operator+=(const T1& other);

    // - --------------------------------------------------------------------------------
    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
    friend inline Vec<n1, T1, SP1> operator-(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b);

    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2, std::size_t... indices>
    friend inline Vec<n1, T1, SP1> operator-(const Vec<n1, T1, SP1>& a, const vec::Private::View<n2, T2, SP2, indices...>& b);

    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2, std::size_t... indices>
    friend inline Vec<n1, T1, SP1> operator-(const vec::Private::View<n2, T2, SP2, indices...>& a, const Vec<n1, T1, SP1>& b);

    template <std::size_t n1, typename T1, class SP1, typename T2>
    friend inline Vec<n1, T1, SP1> operator-(const Vec<n1, T1, SP1>& a, const T2& b);

    template <std::size_t n1, typename T1, class SP1, typename T2>
    friend inline Vec<n1, T1, SP1> operator-(const T2& a, const Vec<n1, T1, SP1>& b);

    template <std::size_t n1, typename T1, class SP1>
    inline Vec<n, T, SP>& operator-=(const Vec<n1, T1, SP1>& other);

    template <std::size_t n1, typename T1, class SP1, std::size_t... indices>
    inline Vec<n, T, SP>& operator-=(const vec::Private::View<n1, T1, SP1, indices...>& other);

    template <typename T1>
    inline Vec<n, T, SP>& operator-=(const T1& other);

    // == & != -----------------------------------------------------------------------
    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
    friend inline bool operator==(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b);

    template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
    friend inline bool operator!=(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b);


    template <std::size_t n1, typename T1, class SP1, typename T2>
    friend inline bool operator==(const Vec<n1, T1, SP1>& a, const T2& b);

    template <std::size_t n1, typename T1, class SP1, typename T2>
    friend inline bool operator==(const T2& a, const Vec<n1, T1, SP1>& b);

    template <std::size_t n1, typename T1, class SP1, typename T2>
    friend inline bool operator!=(const Vec<n1, T1, SP1>& a, const T2& b);

    template <std::size_t n1, typename T1, class SP1, typename T2>
    friend inline bool operator!=(const T2& a, const Vec<n1, T1, SP1>& b);

    // ******************************************************************************* //
    // GLSL compatibility
    // ---------------------------------------------
    vec::Private::View<n, T, SP, 0> x;
    vec::Private::View<n, T, SP, 1> y;
    vec::Private::View<n, T, SP, 2> z;
    vec::Private::View<n, T, SP, 3> w;

    vec::Private::View<n, T, SP, 0> s;
    vec::Private::View<n, T, SP, 1> t;

    vec::Private::View<n, T, SP, 0> r;
    vec::Private::View<n, T, SP, 1> g;
    vec::Private::View<n, T, SP, 2> b;
    vec::Private::View<n, T, SP, 3> a;

    // - - - - - - - - - - - - -
    vec::Private::View<n, T, SP, 0, 1> xy;
    vec::Private::View<n, T, SP, 1, 0> yx;

    vec::Private::View<n, T, SP, 0, 1> rg;
    vec::Private::View<n, T, SP, 1, 0> gr;

    vec::Private::View<n, T, SP, 0, 1> st;
    vec::Private::View<n, T, SP, 1, 0> ts;

    // - - - - - - - - - - - - -
    vec::Private::View<n, T, SP, 0, 1, 2> xyz;
    vec::Private::View<n, T, SP, 2, 1, 0> zyx;

    vec::Private::View<n, T, SP, 0, 1, 2> rgb;
    vec::Private::View<n, T, SP, 2, 1, 0> bgr;

    vec::Private::View<n, T, SP, 0, 1, 2> str;
    vec::Private::View<n, T, SP, 2, 1, 0> rts;

    // - - - - - - - - - - - - -
    vec::Private::View<n, T, SP, 0, 1, 2, 3> xyzw;
    vec::Private::View<n, T, SP, 3, 2, 1, 0> wzyx;

    vec::Private::View<n, T, SP, 0, 1, 2, 3> rgba;
    vec::Private::View<n, T, SP, 3, 2, 1, 0> abgr;

    vec::Private::View<n, T, SP, 0, 1, 2, 3> strq;
    vec::Private::View<n, T, SP, 3, 2, 1, 0> qrts;

    // ******************************************************************************* //
    // Additional methods
    // ------------------------------------------
    template <typename T1 = float>
    inline T1 length() const;

    template <std::size_t i>
    inline T& get();

    template <std::size_t i>
    inline const T& get() const;

    template <std::size_t begin = 0, typename First, typename... Args>
    inline void assign(const First& first, const Args&... args);

    inline void normalize();
    inline Vec<n, float, vec::DefaultStorage<n, float> > normalized() const;
    inline void display() const;

private:

    T data_[n];
};





// ******************************************************************************* //
// Constructors
// ------------------------------------------------------------------
template <std::size_t n, typename T, class SP>
inline Vec<n, T, SP>::Vec() {
    for (std::size_t i = 0; i < n; ++i)
        data_[i] = 0;
}

template <std::size_t n, typename T, class SP>
template <typename T1, class SP1>
inline Vec<n, T, SP>::Vec(const Vec<n, T1, SP1>& other) {
    for (std::size_t i = 0; i < n; ++i)
        data_[i] = other[i];
}

template <std::size_t n, typename T, class SP>
template <std::size_t n1, typename T1, class SP1>
inline Vec<n, T, SP>::Vec(const Vec<n1, T1, SP1>& other) {
        for (std::size_t i = 0; i < (n < n1 ? n : n1); ++i)
		(*this)[i] = other[i];
        if (n1 < n) {
                for (std::size_t i = 0; i < n - n1; ++i)
                        (*this)[n1 + i] = 0;
	}
}

template <std::size_t n, typename T, class SP>
template <typename... Args>
inline Vec<n, T, SP>::Vec(const Args&... args) {
    assign(args...);
}

// Destructors
template <std::size_t n, typename T, class SP>
inline Vec<n, T, SP>::~Vec() {
}









// ******************************************************************************* //
// Additional methods
// ---------------------------------------------------
template <std::size_t n, typename T, class SP>
inline void Vec<n, T, SP>::display() const {
    for (std::size_t i = 0; i < n; ++i) {
        std::cout << data_[i];
        if (i < n - 1)
            std::cout << ", ";
    }
    std::cout << std::endl;
}

template <std::size_t n, typename T, class SP>
template <typename T1>
inline T1 Vec<n, T, SP>::length() const {
    T1 f = 0;
    for (std::size_t i = 0; i < n; ++i)
        f += (*this)[i] * (*this)[i];
    return std::sqrt(f);
}

template <std::size_t n, typename T, class SP>
template <std::size_t i>
T& Vec<n, T, SP>::get() {
    static_assert(i < n, "Index overflow.");
    return data_[i];
}

template <std::size_t n, typename T, class SP>
template <std::size_t i>
const T& Vec<n, T, SP>::get() const {
    static_assert(i < n, "Index overflow.");
    return data_[i];
}

template <std::size_t n, typename T, class SP>
template <std::size_t begin, typename First, typename... Args>
inline void Vec<n, T, SP>::assign(const First& first, const Args&... args) {
    vec::VecAssignment<begin>::assign(*this, first, args...);
}

template <std::size_t n, typename T, class SP>
inline void Vec<n, T, SP>::normalize() {
	static_assert(!std::is_integral<T>::value, "Can't normalize a vector with an integral type.");
    float l = length();
    for (std::size_t i = 0; i < n; ++i)
        (*this)[i] /= l;
}

template <std::size_t n, typename T, class SP>
inline Vec<n, float, vec::DefaultStorage<n, float> > Vec<n, T, SP>::normalized() const {
    Vec<n, float, vec::DefaultStorage<n, float> > v(*this);
    v.normalize();
    return v;
}







// ******************************************************************************* //
// operators
// -------------------------------------------------------------------
// =
template <std::size_t n, typename T, class SP>
inline Vec<n, T, SP>& Vec<n, T, SP>::operator=(const Vec<n, T, SP>& other) {
    return vec::Assignment<true>::assign(*this, other);
}

template <std::size_t n, typename T, class SP>
template <std::size_t n1, typename T1, class SP1>
inline Vec<n, T, SP>& Vec<n, T, SP>::operator=(const Vec<n1, T1, SP1>& other) {
    NEON_DIMENSION_CHECK(n, n1);
    return vec::Assignment<true>::assign(*this, other);
}

template <std::size_t n, typename T, class SP>
template <typename T1>
inline Vec<n, T, SP>& Vec<n, T, SP>::operator=(const T1& other) {
    return vec::Assignment
            <std::is_pointer<T1>::value || std::is_array<T1>::value>::assign(*this, other);
}

// -------------------------------------------------------------------
// Negation
template <std::size_t n1, typename T1, class SP1>
inline Vec<n1, T1, SP1> operator-(const Vec<n1, T1, SP1>& other) {
    Vec<n1, T1, SP1> v(other);
    for (int i = 0; i < n1; ++i)
        v[i] = -v[i];
    return v;
}

// -------------------------------------------------------------------
// []
template <std::size_t n, typename T, class SP>
inline T& Vec<n, T, SP>::operator[](std::size_t i) {
    assert(i < n);
    return data_[i];
}

template <std::size_t n, typename T, class SP>
inline const T& Vec<n, T, SP>::operator[](std::size_t i) const {
    assert(i < n);
    return data_[i];
}

// -------------------------------------------------------------------
// +
template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
inline Vec<n1, T1, SP1> operator+(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b) {
    return vec::operate<vec::Plus>(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2, std::size_t... indices>
inline Vec<n1, T1, SP1> operator+(const Vec<n1, T1, SP1>& a, const vec::Private::View<n2, T2, SP2, indices...>& b) {
    NEON_DIMENSION_CHECK(n1, b.nIndices);
    return vec::Operation<true, vec::Plus>::op(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2, std::size_t... indices>
inline Vec<n1, T1, SP1> operator+(const vec::Private::View<n2, T2, SP2, indices...>& a, const Vec<n1, T1, SP1>& b) {
    return vec::Operation<true, vec::Plus>::op(a, b);
}

template <std::size_t n1, typename T1, class SP1, typename T2>
inline Vec<n1, T1, SP1> operator+(const Vec<n1, T1, SP1>& a, const T2& b) {
    return vec::Operation
            <std::is_pointer<T2>::value || std::is_array<T2>::value, vec::Plus>::op(a, b);
}

template <std::size_t n1, typename T1, class SP1, typename T2>
inline Vec<n1, T1, SP1> operator+(const T2& a, const Vec<n1, T1, SP1>& b) {
    return vec::Operation
            <std::is_pointer<T2>::value || std::is_array<T2>::value, vec::Plus>::op(a, b);
}

template <std::size_t n, typename T, class SP>
template <std::size_t n1, typename T1, class SP1>
inline Vec<n, T, SP>& Vec<n, T, SP>::operator+=(const Vec<n1, T1, SP1>& other) {
    NEON_DIMENSION_CHECK(n, n1);
    return vec::Operation<true, vec::Plus>::opTo(*this, other);
}

template <std::size_t n, typename T, class SP>
template <std::size_t n1, typename T1, class SP1, std::size_t... indices>
inline Vec<n, T, SP>& Vec<n, T, SP>::operator+=(const vec::Private::View<n1, T1, SP1, indices...>& other) {
    NEON_DIMENSION_CHECK(n, other.nIndices);
    return vec::Operation<true, vec::Plus>::opTo(*this, other);
}

template <std::size_t n, typename T, class SP>
template <typename T1>
inline Vec<n, T, SP>& Vec<n, T, SP>::operator+=(const T1& other) {
    return vec::Operation<std::is_pointer<T1>::value ||
            std::is_array<T1>::value, vec::Plus>::opTo(*this, other);
}
// TODO Implement Operation
// -------------------------------------------------------------------
// -
template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
inline Vec<n1, T1, SP1> operator-(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b) {
    NEON_DIMENSION_CHECK(n1, n2);
    return vec::Substraction<true>::substract(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2, std::size_t... indices>
inline Vec<n1, T1, SP1> operator-(const Vec<n1, T1, SP1>& a, const vec::Private::View<n2, T2, SP2, indices...>& b) {
    NEON_DIMENSION_CHECK(n1, b.nIndices);
    return vec::Substraction<true>::substract(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2, std::size_t... indices>
inline Vec<n1, T1, SP1> operator-(const vec::Private::View<n2, T2, SP2, indices...>& a, const Vec<n1, T1, SP1>& b) {
    return vec::Substraction<true>::substract(a, b);
}

template <std::size_t n1, typename T1, class SP1, typename T2>
inline Vec<n1, T1, SP1> operator-(const Vec<n1, T1, SP1>& a, const T2& b) {
    return vec::Substraction
            <std::is_pointer<T2>::value || std::is_array<T2>::value>::substract(a, b);
}

template <std::size_t n1, typename T1, class SP1, typename T2>
inline Vec<n1, T1, SP1> operator-(const T2& a, const Vec<n1, T1, SP1>& b) {
    return vec::Substraction
            <std::is_pointer<T2>::value || std::is_array<T2>::value>::substract(a, b);
}

template <std::size_t n, typename T, class SP>
template <std::size_t n1, typename T1, class SP1>
inline Vec<n, T, SP>& Vec<n, T, SP>::operator-=(const Vec<n1, T1, SP1>& other) {
    NEON_DIMENSION_CHECK(n, n1);
    return vec::Substraction<true>::substractFrom(*this, other);
}

template <std::size_t n, typename T, class SP>
template <std::size_t n1, typename T1, class SP1, std::size_t... indices>
inline Vec<n, T, SP>& Vec<n, T, SP>::operator-=(const vec::Private::View<n1, T1, SP1, indices...>& other) {
    NEON_DIMENSION_CHECK(n, other.nIndices);
    return vec::Substraction<true>::substractFrom(*this, other);
}

template <std::size_t n, typename T, class SP>
template <typename T1>
inline Vec<n, T, SP>& Vec<n, T, SP>::operator-=(const T1& other) {
    return vec::Substraction
            <std::is_pointer<T1>::value || std::is_array<T1>::value>::substractFrom(*this, other);
}


// -------------------------------------------------------------------
// == & !=
template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
inline bool operator==(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b) {
    NEON_DIMENSION_CHECK(n1, n2);
    return vec::Comparison<true>::compare(a, b);
}

template <std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
inline bool operator!=(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b) {
    return !(a == b);
}

template <std::size_t n1, typename T1, class SP1, typename T2>
inline bool operator==(const Vec<n1, T1, SP1>& a, const T2& b) {
    return vec::Comparison
            <std::is_pointer<T2>::value || std::is_array<T2>::value>::compare(a, b);
}

template <std::size_t n1, typename T1, class SP1, typename T2>
inline bool operator==(const T2& a, const Vec<n1, T1, SP1>& b) {
    return b == a;
}

template <std::size_t n1, typename T1, class SP1, typename T2>
inline bool operator!=(const Vec<n1, T1, SP1>& a, const T2& b) {
    return !(a == b);
}

template <std::size_t n1, typename T1, class SP1, typename T2>
inline bool operator!=(const T2& a, const Vec<n1, T1, SP1>& b) {
    return !(b == a);
}







// ******************************************************************************* //
// Additional functions
// ---------------------------------------------------------
template <typename T = float, std::size_t n, typename T1, class SP, typename T2>
T dot(const Vec<n, T1, SP>& a, const T2& b) {
    T res = 0;
    for (std::size_t i = 0; i < n; ++i)
        res += a[i] * b[i];
    return res;
}

template <typename T = float, std::size_t n, typename T1, class SP, typename T2>
T dot(const T2& a, const Vec<n, T1, SP>& b) {
    return dot(b, a);
}


template <typename T = float, std::size_t n1, typename T1, class SP1, std::size_t n2, typename T2, class SP2>
T dot(const Vec<n1, T1, SP1>& a, const Vec<n2, T2, SP2>& b) {
    NEON_DIMENSION_CHECK(n1, n2);
    T res = 0;
    for (std::size_t i = 0; i < n1; ++i)
        res += a[i] * b[i];
    return res;
}


/*template <typename T = float, std::size_t n1, typename T1, std::size_t n2, typename T2, std::size_t... indices>
T dot(const Vec<n1, T1, SP1>& a, const vec::Private::View<n2, T2, SP2, indices...>& b) {
    NEON_DIMENSION_CHECK(n1, b.nIndices);
    T res = 0;
    for (std::size_t i = 0; i < n1; ++i)
        res += a[i] * b[i];
    return res;
}

template <typename T = float, std::size_t n1, typename T1, std::size_t n2, typename T2, std::size_t... indices>
T dot(const vec::Private::View<n2, T2, SP2, indices...>& a, const Vec<n1, T1, SP1>& b) {
    return dot(b, a);
}


template <typename T = float, std::size_t n, typename T1, std::size_t... indices, typename T2>
T dot(const vec::Private::View<n, T1, indices...>& a, const T2& b) {
    T res = 0;
    for (std::size_t i = 0; i < n; ++i)
        res += a[i] * b[i];
    return res;
}

template <typename T = float, std::size_t n, typename T1, std::size_t... indices, typename T2>
T dot(const T2& a, const vec::Private::View<n, T1, indices...>& b) {
    return dot(b, a);
}*/


// -----------------------------------------------------------
template <typename T1, typename T2>
Vec<3, T1> cross(const Vec<3, T1>& a, const Vec<3, T2>& b) {
    Vec<3, T1> v;
    v[0] = a[1] * b[2] - a[2] * b[1];
    v[1] = a[2] * b[0] - a[0] * b[2];
    v[2] = a[0] * b[1] - a[1] * b[0];
    return v.normalized();
}


template <typename T1, typename T2>
Vec<3, T1> cross(const Vec<3, T1>& a, const T2& b) {
    Vec<3, T1> v;
    v[0] = a[1] * b[2] - a[2] * b[1];
    v[1] = a[2] * b[0] - a[0] * b[2];
    v[2] = a[0] * b[1] - a[1] * b[0];
    return v.normalized();
}

template <typename T1, typename T2>
Vec<3, T1> cross(const T2& a, const Vec<3, T1>& b) {
    Vec<3, T1> v;
    v[0] = a[1] * b[2] - a[2] * b[1];
    v[1] = a[2] * b[0] - a[0] * b[2];
    v[2] = a[0] * b[1] - a[1] * b[0];
    return v.normalized();
}


/*template <typename T1, std::size_t n, typename T2, std::size_t... indices>
Vec<3, T1> cross(const Vec<3, T1>& a, const vec::Private::View<n, T2, indices...>& b) {
    NEON_DIMENSION_CHECK(3, b.nIndices);
    Vec<3, T1> v;
    v[0] = a[1] * b[2] - a[2] * b[1];
    v[1] = a[2] * b[0] - a[0] * b[2];
    v[2] = a[0] * b[1] - a[1] * b[0];
    return v.normalized();
}

template <typename T1, std::size_t n, typename T2, std::size_t... indices>
Vec<3, T1> cross(const vec::Private::View<n, T2, indices...>& a, const Vec<3, T1>& b) {
    NEON_DIMENSION_CHECK(3, a.nIndices);
    Vec<3, T1> v;
    v[0] = a[1] * b[2] - a[2] * b[1];
    v[1] = a[2] * b[0] - a[0] * b[2];
    v[2] = a[0] * b[1] - a[1] * b[0];
    return v.normalized();
}


template <typename T1, std::size_t n, std::size_t... indices, typename T2>
Vec<3, T1> cross(const vec::Private::View<n, T2, indices...>& a, const T2& b) {
    NEON_DIMENSION_CHECK(3, a.nIndices);
    Vec<3, T1> v;
    v[0] = a[1] * b[2] - a[2] * b[1];
    v[1] = a[2] * b[0] - a[0] * b[2];
    v[2] = a[0] * b[1] - a[1] * b[0];
    return v.normalized();
}

template <typename T1, std::size_t n, std::size_t... indices, typename T2>
Vec<3, T1> cross(const T2& a, const vec::Private::View<n, T2, indices...>& b) {
    NEON_DIMENSION_CHECK(3, a.nIndices);
    Vec<3, T1> v;
    v[0] = a[1] * b[2] - a[2] * b[1];
    v[1] = a[2] * b[0] - a[0] * b[2];
    v[2] = a[0] * b[1] - a[1] * b[0];
    return v.normalized();
}

// ------------------------------------------------------
template <typename T = float, std::size_t n, typename T1, typename T2>
T distance(const Vec<n, T1>& a, const Vec<n, T2>& b) {
    return (a - b).length();
}


// ------------------------------------------------------
template <std::size_t n, typename T1, typename T2>
Vec<n, T1> reflect(const Vec<n, T1>& i, const Vec<n, T2>& n) {
    return i - 2 * dot(n, i) * n;
}

template <std::size_t n, typename T1, typename T2>
void swap(Vec<n, T1>& a, Vec<n, T2>& b) {
    Vec<n, T1> temp = a;
    a = b;
    b = temp;
}

template <std::size_t n, typename T1, typename T2>
Vec<n, T1> min(const Vec<n, T1>& a, const Vec<n, T2>& b) {
    Vec<n, T1> v;
    for (std::size_t i = 0; i < n; ++i)
        v[i] = std::min(a[i], b[i]);
    return v;
}

template <std::size_t n, typename T1, typename T2>
Vec<n, T1> max(const Vec<n, T1>& a, const Vec<n, T2>& b) {
    Vec<n, T1> v;
    for (std::size_t i = 0; i < n; ++i)
        v[i] = std::max(a[i], b[i]);
    return v;
}

template <typename T = float, std::size_t n, typename T1, typename T2>
Vec<n, T1> angleBetween(const Vec<n, T1>& a, const Vec<n, T2>& b) {
    return std::acos(dot<T>(a.normalized(), b.normalized()));
}*/







// ******************************************************************************* //
// Typedefs
// ----------------------------------------------------
typedef Vec<2, float> Vec2f;
typedef Vec<3, float> Vec3f;
typedef Vec<4, float> Vec4f;

typedef Vec<2, int> Vec2i;
typedef Vec<3, int> Vec3i;
typedef Vec<4, int> Vec4i;

typedef Vec<2, double> Vec2d;
typedef Vec<3, double> Vec3d;
typedef Vec<4, double> Vec4d;

typedef Vec<2, short> Vec2s;
typedef Vec<3, short> Vec3s;
typedef Vec<4, short> Vec4s;

typedef Vec<2, char> Vec2c;
typedef Vec<3, char> Vec3c;
typedef Vec<4, char> Vec4c;

#ifdef NO_NEON
typedef Vec<2, float> vec2;
typedef Vec<3, float> vec3;
typedef Vec<4, float> vec4;
#endif

#ifndef NO_NEON
NEON_CORE_END_NAMESPACE_;
#endif

#endif // NEON_CORE_VEC_

