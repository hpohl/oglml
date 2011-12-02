#ifndef OGLML_HELPERS_AUTORETURN_HPP
#define OGLML_HELPERS_AUTORETURN_HPP

#include <type_traits>

#define OGLML_AUTO_RETURN(...) -> typename std::decay<decltype(__VA_ARGS__)>::type \
{ return __VA_ARGS__; }

#endif // OGLML_HELPERS_AUTORETURN_HPP
