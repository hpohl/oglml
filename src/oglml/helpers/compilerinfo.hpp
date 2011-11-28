#ifndef OGLML_HELPERS_COMPILERINFO_HPP
#define OGLML_HELPERS_COMPILERINFO_HPP

// Reset to defaults
#define oglml_constexpr constexpr
#define oglml_constexpr_if_available constexpr

// Enable all C++11 features
#define OGLML_CXX11_CONSTEXPR
#define OGLML_CXX11_TEMPLATE_ALIASES
#define OGLML_CXX11_UNRESTRICTED_UNIONS

// ---------------------------------------
// GCC Specific
#ifdef __GNUC__
#define OGLML_GNUC __GNUC__
#define OGLML_GNUC_MINOR __GNUC_MINOR__
#define OGLML_GNUC_PATCHLEVEL __GNUC_PATCHLEVEL__

// Makro to get GCC version
#if defined(OGLML_GNUC)
# if defined(OGLML_GNUC_PATCHLEVEL)
#  define OGLML_GNUC_VERSION (OGLML_GNUC * 10000 \
                            + OGLML_GNUC_MINOR * 100 \
                            + OGLML_GNUC_PATCHLEVEL)
# else
#  define OGLML_GNUC_VERSION (OGLML_GNUC * 10000 \
                            + OGLML_GNUC_MINOR * 100)
# endif
#endif

#define OGLML_GCC_VERSION_48 40800
#define OGLML_GCC_VERSION_47 40700
#define OGLML_GCC_VERSION_46 40600
#define OGLML_GCC_VERSION_45 40500
#define OGLML_GCC_VERSION_44 40400

// C++11 features
// Constexpr
#if OGLML_GNUC_VERSION < OGLML_GCC_VERSION_46
#undef OGLML_CXX11_CONSTEXPR
#undef oglml_constexpr
#define oglml_constexpr const
#undef oglml_constexpr_if_available
#define oglml_constexpr_if_available
#endif

// Template aliases
#if OGLML_GNUC_VERSION < OGLML_GCC_VERSION_48
#undef OGLML_CXX11_TEMPLATE_ALIASES
#endif

// Unrestricted unions
#if OGLML_GNUC_VERSION < OGLML_GCC_VERSION_46
#undef OGLML_CXX11_UNRESTRICTED_UNIONS
#endif

#endif // __GNUC__
// -----------------------------------------



#endif // OGLML_HELPERS_COMPILERINFO_HPP
