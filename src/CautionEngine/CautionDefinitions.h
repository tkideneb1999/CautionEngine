#pragma once

// Compiler specific macros...
#if defined (_MSC_VER)

#ifdef CAUTION_SHARED
	#define CAUTION_CLASS __declspec(dllexport)
#else
	#define CAUTION_CLASS __declspec(dllimport)
#endif

////////////////////// Disable Warning from the compiler (MSVC) //////////////////////
#pragma warning(disable: 4251)
// 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'
// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4251?view=msvc-170

#pragma warning(disable: 4244)
// 'conversion' conversion from 'type1' to 'type2', possible loss of data
// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-levels-3-and-4-c4244?view=msvc-170

#pragma warning(disable: 4305)
// 'conversion': truncation from 'type1' to 'type2'
// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4305?view=msvc-170

#pragma warning(disable: 4018)
// 'token' : signed/unsigned mismatch
// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4018?view=msvc-170

#pragma warning(disable: 4267)
// 'var' : conversion from 'size_t' to 'type', possible loss of data
// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4267?view=msvc-170
#endif