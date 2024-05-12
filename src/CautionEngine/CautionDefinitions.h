#pragma once

// Compiler specific macros...
#if defined (_MSC_VER)
	#ifdef CAUTION_SHARED
		#define CAUTION_API __declspec(dllexport)
	#else
		#define CAUTION_API __declspec(dllimport)
	#endif
#endif