#pragma once


#if not defined(zyra_begin_)
#define zyra_begin_ namespace zyra {
#endif // zyra_begin_
#if not defined(zyra_end_)
#define zyra_end_ }
#endif // zyra_end_

#if not defined(zyra_always_inline)
#if defined(_MSC_VER)
#define zyra_always_inline __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define zyra_always_inline inline __attribute__((always_inline))
#else // fallback compiler
#define zyra_always_inline inline
#endif // compiler
#endif // zyra_always_inline