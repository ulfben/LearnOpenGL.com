#pragma once
#if defined(_MSC_VER)
#define DISABLE_WARNING_PUSH           __pragma(warning( push ))
#define DISABLE_WARNING_POP            __pragma(warning( pop )) 
#define DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))

#define DISABLE_WARNING_USE_GSL_AT			DISABLE_WARNING(26446) //Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
#define DISABLE_WARNING_USE_GSL_FINALLY     DISABLE_WARNING(26448) //Consider using gsl::finally if final action is intended.

#elif defined(__GNUC__) || defined(__clang__)
#define DO_PRAGMA(X) _Pragma(#X)
#define DISABLE_WARNING_PUSH           DO_PRAGMA(GCC diagnostic push)
#define DISABLE_WARNING_POP            DO_PRAGMA(GCC diagnostic pop) 
#define DISABLE_WARNING(warningName)   DO_PRAGMA(GCC diagnostic ignored #warningName)

#define DISABLE_WARNING_PRAGMA_ONCE_IN_MAIN_FILE    DISABLE_WARNING(-Wpragma-once-outside-header)
#define DISABLE_WARNING_UNREFERENCED_FUNCTION            DISABLE_WARNING(-Wunused-function)

#else
#define DISABLE_WARNING_PUSH
#define DISABLE_WARNING_POP
#define DISABLE_WARNING_USE_GSL_AT
#define DISABLE_WARNING_USE_GSL_FINALLY

#endif

