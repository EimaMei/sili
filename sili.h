/*
sili.h - v0.2.0 - a general-purpose programming library to replace the C/C++ standard libraries.
===========================================================================
	- YOU MUST DEFINE 'SI_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SI_IMPLEMENTATION
		#include "sili.h"

	- All other files should just include the library without the #define macro.

	- If you want to disable certain features, you can do:
		- #define SI_NO_OPTIONAL
		- #define SI_NO_MEMORY
		- #define SI_NO_VIRTUAL_MEMORY
		- #define SI_NO_ALLOCATOR
		- #define SI_NO_ARRAY
		- #define SI_NO_STRING
		- #define SI_NO_UNICODE
		- #define SI_NO_CHAR
		- #define SI_NO_HASHTABLE
		- #define SI_NO_SYSTEM
		- #define SI_NO_IO
		- #define SI_NO_THREAD
		- #define SI_NO_TIME
		- #define SI_NO_BIT
		- #define SI_NO_CPU
		- #define SI_NO_PRINT
		- #define SI_NO_DLL
		- #define SI_NO_MATH
		- #define SI_NO_ARGV
		- #define SI_NO_BENCHMARK
	before the SI_IMPLEMENTATION macro, as well as before any other include of
	`sili.h`.

	- NOTE: Disabling certain features may or may not cause compiler errors for
	certain spots that are dependent on them. Use this feature at your own risk.

===========================================================================
DOCUMENTATION
	- All functions, constant variables and macros contain a comment with a
	description of what they do above them, as well what they return (if anything).
	Macros that act like functions specifically use a specific declaration format
	because of their lack of typing.

	That being:
		/ argumentName - TYPE | otherArgumentName - KEYWORD | ...VALUES - TYPE*
		description of the macro. /
		#define smth(argumentName, otherArgumentName, .../ VALUES/)

	- More often than not a macro's argument will not be a specific type and
	instead some sort of keyword. In practice they're declared as fully capitalized
	words (eg. UINT). These keywords denote that the required argument for the
	macro can be anything as long as it fufills the original word's connotation.

	- General list of the macro keywords, their meanings and examples of them:
		- TYPE - any type name (siString, usize, void*).
		- TYPE* - any type name's pointer type (siString*, usize*, void**).
		- INT - any signed integer (50, -250LL, ISIZE_MAX).
		- UINT - any unsigned integer (50, 250ULL, USIZE_MAX).
		- FUNCTION - any function name that's visibly-exposed to the compiler.
		- EXPRESSION - any legal C expression (60, "hello", SI_RGB(255, 255, 255)).
		- VARIADIC - an indefinite amount of legal C expressions ("firstValue", 230, "third")
		- NAME - any regular text with no enquotes (test, var, len).
		- VAR - any variable that's visible-exposed to the compiler.
		- ANYTHING - anything that compiles without error.

===========================================================================
MACROS
	- For any of the macros to work, you must _always_ define it before including
	the library. Example:
	```c
		#define SI_IMPLEMENTATION
		#define SI_RELEASE_MODE
		#include "sili.h"
	```

	- SI_RELEASE_MODE - disables certain functionality that decrease performance
	(such as logging, error reporting, assertions, etc). Defining "NDEBUG" does
	the same thing.

	- SI_NO_ASSERTIONS_IN_HEADER - assertion functions inside the library itself
	get disabled, passively allowing for both correct and wrong states to function.
	However, these functions are re-enabled outside of the header.

	- SI_NO_ASSERTIONS - same as defining 'SI_NO_ASSERTIONS_IN_HEADER', however
	the assertion functions do not get re-enabled outside of the header.

	- SI_USE_BUFFER_GROW - adds a 'grow' member to the 'siBuffer' struct, alongside
	sligthly modifies the 'SI_USE_BUFFER_GROW' macro. This allows the user to set
	how many extra elements should be added to the array after each resize. The
	value of 'SI_USE_BUFFER_GROW' is also assumed to be the default growth number,
	meaning by default setting 'SI_USE_BUFFER_GROW' to 32 means that a new capacity
	for an array would be 'capacity + addLen + 32'. If you need to set a specific
	growth number for buffer, you have to set it via '<array>.grow = N;'.

	- SI_NO_ERROR_STRUCT - strips the 'siError' structure's members down to just
	the error code member. This gets turned on automatically via 'SI_RELEASE_MODE'.

	- SI_NO_ERROR_LOGS - disables sili's automatic logging for when a 'SI_ERROR'
	is declared. This gets turned on automatically via 'SI_RELEASE_MODE'.

	- SI_NO_INLINE_ASM - disables any usage of inline assembly in the header.

	- SI_USE_ATT_SYNTAX - makes 'si_asm()' utilize AT&T syntax instead of
	Intel's for GNU C compiler derivatives.

	- SI_NO_WINDOWS_H - disables the inclusion of the win32 API inside the header.

	- SI_NO_TYPEOF - disables the usage of the 'typeof' operator inside the header,
	alongside undefining macros that utilize it.

	- SI_USE_CUSTOM_HASH_FUNCTION - undefines the base implementation of
	'si__hashKey' inside this file, allowing the user to define their own hash
	key function.

	- SI_NO_HASH_OVERWRITE - disables the ovewrite of a pre-existing entry's
	value when using 'si_hashtableSet/WithHash'.

===========================================================================
CREDITS
	- Ginger Bill's 'gb.h' (https://github.com//gingerBill/gb) - inspired me to
	make the Sili Toolchain, as well as certain features were taken from the
	library and implemented here.

LICENSE:
	- This software is licensed under the zlib license (see the LICENSE at the
	bottom of the file).

WARNING
	- Sili is designed to be a library that's fast, modern, but also experimental.
	Because of that, some unwarranted results may occur when using the library,
	those being:
		1) Features not working as expected;
		2) Functions not being documented or only containing incomplete documentation;
		3) API breaking changes between releases;
		4) Little to no security checks for malicious code attempting to exploit
		parts of the code

*/

#if (_MSC_VER >= 1020) || ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
	#pragma once
#endif

#ifndef SI_INCLUDE_SI_H
#define SI_INCLUDE_SI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SI_VERSION_MAJOR
	#define SI_VERSION_MAJOR 0
	#define SI_VERSION_MINOR 2
	#define SI_VERSION_PATCH 0
#endif
#define SI_VERSION_CURRENT SI_VERSION(SI_VERSION_MAJOR, SI_VERSION_MINOR, SI_VERSION_PATCH)

/* major - UINT | minor - UINT | patch - UINT
 * Combines major, minor, and patch version numbers into a single integer. */
#define SI_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)

#ifndef SI_EXTERN
	#ifdef __cplusplus
		#define SI_EXTERN extern "C"
	#else
		#define SI_EXTERN extern
	#endif
#endif

#ifndef SIDEF
	#ifdef SI_STATIC
		#define SIDEF static
	#else
		#define SIDEF extern
	#endif
#endif


#if defined(__EMSCRIPTEN__) || defined(__wasm) || defined(__wasm32) || defined(__wasm32__) \
	|| defined(__wasm__) || defined(__WASM__) || defined(__wasm64__)
	#ifdef __EMSCRIPTEN__
		#define SI_SYSTEM_EMSCRIPTEN 1
	#else
		#define SI_SYSTEM_WASI 1
		#define SI_NO_CRT
	#endif
	#define SI_SYSTEM_STR "WebAssembly"
	#define SI_SYSTEM_IS_WASM 1

#elif defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && !defined(_WIN32))
	#define SI_SYSTEM_WINDOWS 1
	#define SI_SYSTEM_STR "Windows"
	#define SI_SYSTEM_IS_WINDOWS 1

#elif defined(__APPLE__) && defined(__MACH__)
	#include <TargetConditionals.h>
	#define SI_SYSTEM_IS_APPLE 1

	#if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
		#define SI_SYSTEM_IOS 1
		#define SI_SYSTEM_STR "iOS"
	#elif TARGET_OS_MAC == 1
		#define SI_SYSTEM_OSX 1
		#define SI_SYSTEM_STR "MacOS"
	#endif

#elif defined(__ANDROID__)
	#define SI_SYSTEM_ANDROID 1
	#define SI_SYSTEM_STR "Android"
	#define SI_SYSTEM_IS_UNIX 1

#elif defined(__linux__)
	#define SI_SYSTEM_LINUX 1
	#define SI_SYSTEM_STR "Linux"
	#define SI_SYSTEM_IS_UNIX 1

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	#define SI_SYSTEM_FREEBSD
	#define SI_SYSTEM_STR "FreeBSD"
	#define SI_SYSTEM_IS_UNIX 1


#elif defined(__OpenBSD__)
	#define SI_SYSTEM_OPENBSD
	#define SI_SYSTEM_STR "OpenBSD"
	#define SI_SYSTEM_IS_UNIX 1

#elif defined(unix) || defined(__unix__) || defined(__unix)
	#define SI_SYSTEM_UNIX_OTHER 1
	#define SI_SYSTEM_STR "Unix"
	#define SI_SYSTEM_IS_UNIX 1

#else
	#define SI_SYSTEM_UNKNOWN 1
	#define SI_SYSTEM_STR "Unknown"

#endif

#if defined(__GNUC__) && !defined(__clang__)
	#define SI_COMPILER_GCC 1
	#define SI_COMPILER_STR "GCC"
	#define SI_COMPILER_TYPEOF_SUPPORTS 1

	#if defined(__GNUC_PATCHLEVEL__)
		#define SI_COMPILER_VERSION	SI_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC__PATCHLEVEL__)
	#else
		#define SI_COMPILER_VERSION	SI_VERSION(__GNUC__, __GNUC_MINOR__, 0)
	#endif

#elif defined(_MSC_VER)
	#define SI_COMPILER_MSVC 1
	#define SI_COMPILER_STR "MSVC"

	#define SI_COMPILER_VERSION SI_VERSION(_MSC_VER / 100, _MSC_VER % 100, 0)

#elif defined(__clang__)
	#define SI_COMPILER_CLANG 1
	#define SI_COMPILER_STR "Clang"
	#define SI_COMPILER_TYPEOF_SUPPORTS 1

	#define SI_COMPILER_VERSION	SI_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)

#else
	#define SI_COMPILER_UNKNOWN 1
	#define SI_COMPILER_STR "Unknown"

	#define SI_COMPILER_VERSION SI_VERSION(0, 0, 0)

#endif


/* compiler - NAME | major - UINT | minor - UINT | patch - UINT
 * A pre-processor condition to check if the current compiler version passes
 * the minimum version. */
#define SI_COMPILER_CHECK_MIN(compiler, major, minor, patch) \
	(SI_COMPILER_ ## compiler && SI_COMPILER_VERSION >= SI_VERSION(major, minor, patch))

/* compiler - NAME | major - UINT | minor - UINT | patch - UINT
 * A pre-processor condition to check if the current compiler version is less than
 * or is equal to the maximum version. */
#define SI_COMPILER_CHECK_MAX(compiler, major, minor, patch) \
	(SI_COMPILER_ ## compiler && SI_COMPILER_VERSION <= SI_VERSION(major, minor, patch))

/* Checks if the currently-used compiler supports '#pragma once' functinoality. */
#define SI_COMPILER_HAS_PRAGMA_ONCE (SI_COMPILER_CLANG || SI_COMPILER_MSVC || SI_COMPILER_CHECK_MIN(GCC, 3, 4, 0))

#ifdef __cplusplus
	#ifdef __OBJC__
		#define SI_LANGUAGE_OBJ_CPP 1
		#define SI_LANGUAGE_STR "Objective-C++"
	#else
		#define SI_LANGUAGE_CPP 1
		#define SI_LANGUAGE_STR "C++"
	#endif

	#define SI_LANGUAGE_IS_CPP 1
#else
	#ifdef __OBJC__
		#define SI_LANGUAGE_OBJ_C 1
		#define SI_LANGUAGE_STR "Objective-C"
	#elif defined(__cplus__)
		#define SI_LANGUAGE_CPLUS 1
		#define SI_LANGUAGE_STR "C-Plus"
	#else
		#define SI_LANGUAGE_C 1
		#define SI_LANGUAGE_STR "C"
	#endif

	#define SI_LANGUAGE_IS_C 1
#endif

#if SI_LANGUAGE_IS_C
	#ifndef SI_STANDARD_VERSION
		/* TODO(EimaMei): Remove this when GCC will change 'C2x' to 'C23'. */
		#if defined(SI_COMPILER_GCC) && __STDC_VERSION__ == 202200L
			#define SI_STANDARD_VERSION SI_STANDARD_C23
		#elif defined(__STDC_VERSION__)
			#define SI_STANDARD_VERSION __STDC_VERSION__
		#else
			#define SI_STANDARD_VERSION 198900L
		#endif
	#endif

	#define SI_STANDARD_C89 198900L
	#define SI_STANDARD_C94 199409L
	#define SI_STANDARD_C99 199901l
	#define SI_STANDARD_C11 201112L
	#define SI_STANDARD_C17 201710L
	#define SI_STANDARD_C23 202311L

#else
	#ifndef SI_STANDARD_VERSION
		#define SI_STANDARD_VERSION __cplusplus
	#endif

	#define SI_STANDARD_CPP98 199711L
	#define SI_STANDARD_CPP11 201103L
	#define SI_STANDARD_CPP14 201402L
	#define SI_STANDARD_CPP17 201703L
	#define SI_STANDARD_CPP20 202002L
	#define SI_STANDARD_CPP23 202302L

#endif

/* language - NAME | standard - NAME
 * A pre-processor condition to check if the current language's standard version
 * passes the minimum version. */
#define SI_STANDARD_CHECK_MIN(language, standard) \
	(SI_LANGUAGE_IS_ ## language && SI_STANDARD_VERSION >= SI_STANDARD_ ## standard)

/* language - NAME | standard - NAME
 * A pre-processor condition to check if the current language's standard version
 * is less than or is equal to the maximum version. */
#define SI_STANDARD_CHECK_MAX(language, standard) \
	(SI_LANGUAGE_IS_ ## language && SI_STANDARD_VERSION <= SI_STANDARD_ ## standard)


#if defined(__i386__) || defined(__i386) || defined(i386) || defined(__IA32__) \
	|| defined(__X86__) || defined(_M_IX86) || defined(_M_I386) || defined(_X86_) \
	|| defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || defined(__386)
	#define SI_ARCH_I386 1
	#define SI_ARCH_STR "i386"

	#define SI_ARCH_IS_X86 1
	#define SI_ARCH_IS_32BIT 1

#elif defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || defined(__amd64__) \
	|| defined(__amd64) || defined(__x86_64)
	#define SI_ARCH_AMD64 1
	#define SI_ARCH_STR "amd64"

	#define SI_ARCH_IS_X86 1
	#define SI_ARCH_IS_64BIT 1

#elif defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
	#define SI_ARCH_PPC64 1
	#define SI_ARCH_STR "PowerPC64"

	#define SI_ARCH_IS_PPC 1
	#define SI_ARCH_IS_64BIT 1

#elif defined(__powerpc__) || defined(__powerpc) || defined(__POWERPC__) || defined(__ppc__) \
	|| defined(__PPC__) || defined(_M_PPC) || defined(_ARCH_PPC) || defined(__PPCGECKO__) \
	|| defined(__PPCBROADWAY__) || defined(_XENON) || defined(__ppc)
	#define SI_ARCH_PPC32 1
	#define SI_ARCH_STR "PowerPC32"

	#define SI_ARCH_IS_PPC 1
	#define SI_ARCH_IS_32BIT 1

#elif defined(__aarch64__) || defined(_M_ARM64)
	#define SI_ARCH_ARM64 1
	#define SI_ARCH_STR "ARM64"

	#define SI_ARCH_IS_ARM 1
	#define SI_ARCH_IS_64BIT 1

#elif defined(__arm__) || defined(__thumb__) || defined(__TARGET_ARCH_ARM) \
	|| defined(__TARGET_ARCH_THUMB) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) \
	|| defined(__arm)
	#define SI_ARCH_ARM32 1
	#define SI_ARCH_STR "ARM32"

	#define SI_ARCH_IS_ARM 1
	#define SI_ARCH_IS_32BIT 1

#elif defined(__riscv)
	#if __riscv_xlen == 32
		#define SI_ARCH_RISC_V32 1
		#define SI_ARCH_STR "RISC-V 32-bit"
		#define SI_ARCH_IS_32BIT 1
	#elif __riscv_xlen == 64
		#define SI_ARCH_RISC_V64 1
		#define SI_ARCH_STR "RISC-V 64-bit"
		#define SI_ARCH_IS_64BIT 1
	#endif
	#define SI_ARCH_IS_RISC 1

#elif SI_SYSTEM_IS_WASM
	#if defined(__wasm32__) || defined(__EMSCRIPTEN_32BIT__)
		#define SI_ARCH_WASM32 1
		#define SI_ARCH_STR "WASM 32-bit"
		#define SI_ARCH_IS_32BIT 1
	#elif defined(__wasm64__) || defined(__EMSCRIPTEN_64BIT__)
		#define SI_ARCH_WASM64 1
		#define SI_ARCH_STR "WASM 64-bit"
		#define SI_ARCH_IS_64BIT 1
	#endif
	#define SI_ARCH_IS_WASM 1

#else
	#define SI_ARCH_UNKNOWN 1
	#define SI_ARCH_STR "Unknown"
	#define SI_ARCH_IS_32BIT 1 /* Assume that stuff is 32-bit for the sake of it. */

#endif

#if !defined(SI_ENDIAN_STR) && (!defined(SI_ENDIAN_IS_LITTLE) || !defined(SI_ENDIAN_IS_BIG))
	#if SI_COMPILER_CHECK_MIN(GCC, 4, 6, 0) || SI_COMPILER_CHECK_MIN(CLANG, 3, 0, 0)
		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			#define SI_ENDIAN_IS_LITTLE 1
			#define SI_ENDIAN_STR "Little-endian"

		#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			#define SI_ENDIAN_IS_BIG 1
			#define SI_ENDIAN_STR "Big-endian"

		#else
			#define SI_ENDIAN_UNKNOWN 1
			#define SI_ENDIAN_STR "Unknown"

		#endif

	#else
		/* NOTE(EimaMei): A "portable"(?) way of checking endianness at compile
		 * time.
		 *
		 * Rationale: In the C89 specification, it states that multi-chars are
		 * legal but left up for implementation. This results in most compilers
		 * just appending the characters since that's cheap and fast, but at the
		 * cost of it it introduces endianness issues, which is why (I presume)
		 * GCC complains about it.
		 *
		 * Due to this, it's more or less somewhat safe to assume that if a
		 * multi-char is created in that way, then you can also assume the endian
		 * at compile time too! */
		#if 'ABCD' == 0x41424344UL
			#define SI_ENDIAN_IS_LITTLE 1
			#define SI_ENDIAN_STR "Little-endian"

		#elif 'ABCD' == 0x44434241UL
			#define SI_ENDIAN_IS_BIG 1
			#define SI_ENDIAN_STR "Big-endian"

		#else
			#define SI_ENDIAN_UNKNOWN 1
			#define SI_ENDIAN_STR "Unknown"

		#endif
	#endif
#endif

#ifndef SI_STATIC_ASSERT_MSG
	#if SI_STANDARD_CHECK_MIN(C, C11)
		/* condition - EXPRESSIONG | msg - cstring
		 * Stops the program from being compiled if the condition isn't met with
		 * an explanation as to why. */
		#define SI_STATIC_ASSERT_MSG(condition, msg) _Static_assert(condition, msg)

	#elif SI_STANDARD_CHECK_MIN(CPP, CPP11)
		/* condition - EXPRESSIONG | msg - cstring
		 * Stops the program from being compiled if the condition isn't met with
		 * an explanation as to why. */
		#define SI_STATIC_ASSERT_MSG(condition, msg) static_assert(condition, msg)

	#else
		/* condition - EXPRESSIONG | msg - cstring
		 * Stops the program from being compiled if the condition isn't met with
		 * an explanation as to why. */
		#define SI_STATIC_ASSERT_MSG(condition, msg) SI_STATIC_ASSERT_MSG1(condition, msg, __LINE__)

		#define SI_STATIC_ASSERT_MSG1(condition, msg, line) SI_STATIC_ASSERT_MSG2(condition, msg, line)
		#define SI_STATIC_ASSERT_MSG2(condition, msg, line) \
			typedef char __si_error_if_negative_##line[(condition) ? 1 : -1]

	#endif
#endif
/* condition - EXPRESSIONG
* Stops the program from being compiled if the condition isn't met. */
#define SI_STATIC_ASSERT(condition) SI_STATIC_ASSERT_MSG(condition, "")


#if SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE || SI_SYSTEM_EMSCRIPTEN
	#undef _GNU_SOURCE
	#undef _LARGEFILE64_SOURCE
	#undef __USE_POSIX199506

	#define _GNU_SOURCE
	#define _LARGEFILE64_SOURCE
	#define __USE_POSIX199506 1

	#include <stdlib.h>
	#include <unistd.h>

	#if !defined(SI_NO_MEMORY) && !defined(SI_NO_CRT)
		#include <memory.h>
	#endif

	#ifndef SI_NO_VIRTUAL_MEMORY
		#include <sys/mman.h>
	#endif

	#ifndef SI_NO_SYSTEM
		#include <errno.h>
	#endif

	#ifndef SI_NO_IO
		#include <fcntl.h>
		#include <sys/stat.h>
		#include <dirent.h>
	#endif

	#ifndef SI_NO_THREAD
		#include <pthread.h>
	#endif

	#ifndef SI_NO_PRINT
		#include <stdarg.h>
	#endif

	#ifndef SI_NO_DLL
		#include <dlfcn.h>
	#endif


	/* TODO(EimaMei): Check if we really need these macros. */
	#undef _GNU_SOURCE
	#undef _LARGEFILE64_SOURCE
	#undef __USE_POSIX199506

#elif SI_SYSTEM_IS_WINDOWS
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif

	#ifndef SI_NO_WINDOWS_H
		#define NOMINMAX            1
		#define WIN32_LEAN_AND_MEAN 1
		#define WIN32_MEAN_AND_LEAN 1
		#define VC_EXTRALEAN        1
		#include <windows.h>
		#include <aclapi.h>
		#include <shellapi.h>
		#undef NOMINMAX
		#undef WIN32_LEAN_AND_MEAN
		#undef WIN32_MEAN_AND_LEAN
		#undef VC_EXTRALEAN
	#endif

	#if SI_COMPILER_MSVC
		#include <malloc.h>
		#include <intrin.h>
	#endif

#endif

#if SI_SYSTEM_IS_APPLE
	#include <sys/socket.h>
	#include <sys/sysctl.h>

#elif SI_SYSTEM_IS_UNIX
	#include <sys/sendfile.h>

#elif SI_SYSTEM_EMSCRIPTEN
	#include <emscripten/emscripten.h>
	#include <wasi/api.h>

#elif SI_SYSTEM_WASI
	#ifndef SI_NO_PRINT
		#include <stdarg.h>
	#endif

	//#include <wasi/api.h>
#endif


#if defined(SI_RELEASE_MODE) || defined(NDEBUG)
	#undef SI_NO_ASSERTIONS
	#undef SI_NO_ERROR_LOGS
	#undef SI_NO_ERROR_STRUCT

	#define SI_NO_ASSERTIONS
	#define SI_NO_ALLOC_DEBUG_INFO
	#define SI_NO_ERROR_LOGS
	#define SI_NO_ERROR_STRUCT
#endif

#ifndef SI_NO_TYPE_DEFS
	#include <stdint.h>
	#include <stddef.h>

	typedef uint8_t   u8;
	typedef  int8_t   i8;
	typedef uint16_t u16;
	typedef  int16_t i16;
	typedef uint32_t u32;
	typedef  int32_t i32;
	typedef uint64_t u64;
	typedef  int64_t i64;


	typedef size_t      usize;
	typedef ptrdiff_t   isize;

	typedef float  f32;
	typedef double f64;

#endif

#ifndef b8
	typedef u8   b8; /* NOTE(EimaMei): Don't use this as the primary boolean type. */
	typedef u16 b16;
	typedef u32 b32; /* NOTE(EimaMei): Use this as the default! */
	typedef u64 b64;
#endif

SI_STATIC_ASSERT(sizeof(u8)    == sizeof(i8));
SI_STATIC_ASSERT(sizeof(u16)   == sizeof(i16));
SI_STATIC_ASSERT(sizeof(u32)   == sizeof(i32));
SI_STATIC_ASSERT(sizeof(u64)   == sizeof(i64));
SI_STATIC_ASSERT(sizeof(usize) == sizeof(isize));

SI_STATIC_ASSERT(sizeof(u8)  == 1);
SI_STATIC_ASSERT(sizeof(u16) == 2);
SI_STATIC_ASSERT(sizeof(u32) == 4);
SI_STATIC_ASSERT(sizeof(u64) == 8);

#if SI_ARCH_IS_32BIT
	SI_STATIC_ASSERT(sizeof(usize) == 4);
#else
	SI_STATIC_ASSERT(sizeof(usize) == 8);
#endif

SI_STATIC_ASSERT(sizeof(f32) == 4);
SI_STATIC_ASSERT(sizeof(f64) == 8);

#if SI_ARCH_IS_64BIT
	#ifndef USIZE_MAX
		#define USIZE_MAX (usize)(UINT64_MAX)
	#endif
	#ifndef ISIZE_MAX
		#define ISIZE_MAX (isize)(INT64_MAX)
	#endif
	#ifndef ISIZE_MIN
		#define ISIZE_MIN (isize)(INT64_MIN)
	#endif
#else
	#ifndef USIZE_MAX
		#define USIZE_MAX (usize)(UINT32_MAX)
	#endif
	#ifndef ISIZE_MAX
		#define ISIZE_MAX (isize)(INT32_MAX)
	#endif
	#ifndef ISIZE_MIN
		#define ISIZE_MIN (isize)(INT32_MIN)
	#endif

#endif

#ifndef FLOAT32_MIN
	#define FLOAT32_MIN (1.17549435e-38f)
#endif
#ifndef FLOAT32_MAX
	#define FLOAT32_MAX (3.40282347e+38f)
#endif
#ifndef FLOAT64_MIN
	#define FLOAT64_MIN (2.2250738585072014e-308)
#endif
#ifndef FLOAT64_MAX
	#define FLOAT64_MAX (1.7976931348623157e+308)
#endif



#if SI_STANDARD_CHECK_MAX(C, C17)
	#ifndef true
		#define true (1)
	#endif
	#ifndef false
		#define false (0)
	#endif
#endif

SI_STATIC_ASSERT(sizeof(b8)  == 1);
SI_STATIC_ASSERT(sizeof(b16) == 2);
SI_STATIC_ASSERT(sizeof(b32) == 4);
SI_STATIC_ASSERT(sizeof(b64) == 8);

SI_STATIC_ASSERT(true  == 1);
SI_STATIC_ASSERT(false == 0);


#ifndef force_inline
	#if SI_COMPILER_MSVC
		#define force_inline __forceinline
	#elif SI_COMPILER_CHECK_MIN(GCC, 3, 1, 0) || SI_COMPILER_CHECK_MIN(CLANG, 2, 6, 0)
		#define force_inline static inline __attribute__((always_inline))
	#else
		#define force_inline static inline
	#endif
#endif

#ifndef siIntern
	#define siIntern static
#endif

#if SI_COMPILER_MSVC
	/* asm - ASSEMBLY
	 * Inserts inline assembly into the program using MSVC assembly syntax. */
	#define si_asm(...) __asm  { __VA_ARGS__ }
#else
	#if SI_ARCH_IS_X86 && (SI_COMPILER_GCC || SI_COMPILER_CLANG)
		/* asmStr - cstring | ...IOR - ASM INPUT, OUTPUT OR REGISTERS
		 * Inserts inline assembly into the program using GNU C assembly syntax. */
		#define si_asm(asmStr, ...) \
			__asm__ __volatile__( \
				".intel_syntax noprefix" SI_ASM_NL \
				asmStr SI_ASM_NL \
				".att_syntax" \
				__VA_ARGS__ \
				)
	#else
		/* asmStr - cstring | ...IOR - ASM INPUT, OUTPUT OR REGISTERS
		 * Inserts inline assembly into the program using GNU C assembly syntax. */
		#define si_asm(asmStr, .../* IOR */) __asm__ __volatile__(asmStr __VA_ARGS__)
	#endif

	#define SI_ASM_INPUT(...) : __VA_ARGS__
	#define SI_ASM_OUTPUT(...) : __VA_ARGS__
	#define SI_ASM_REGISTERS(...) : __VA_ARGS__
	#define SI_ASM_NL "\n"
#endif



#if SI_STANDARD_CHECK_MIN(CPP, C17) || SI_STANDARD_CHECK_MIN(C, C23)
	/* Specifies a fallthrough for the compiler. */
	#define siFallthrough [[fallthrough]]

#elif SI_COMPILER_CHECK_MIN(GCC, 7, 0, 0) || SI_COMPILER_CHECK_MIN(CLANG, 3, 5, 0)
	/* Specifies a fallthrough for the compiler. */
	#define siFallthrough __attribute__((fallthrough))

#else
	/* Specifies a fallthrough for the compiler. */
	#define siFallthrough do {} while (0)

#endif


#ifndef restrict
	#if SI_LANGUAGE_IS_CPP || SI_STANDARD_CHECK_MAX(C, C89)
		#if SI_COMPILER_CLANG || SI_COMPILER_CHECK_MIN(GCC, 3, 1, 0)
			#define restrict __restrict
		#elif SI_COMPILER_CHECK_MIN(MSVC, 14, 0, 0)
			#define restrict __restrict
		#else
			#define restrict
		#endif
	#endif
#endif

#ifndef siNoreturn
	#if SI_STANDARD_CHECK_MIN(C, C23) || SI_STANDARD_CHECK_MIN(CPP, CPP11)
		/* Specifies that the function will not return anything. */
		#define siNoreturn [[noreturn]]

	#elif SI_STANDARD_CHECK_MIN(C, C11)
		/* Specifies that the function will not return anything. */
		#define siNoreturn _Noreturn

	#else
		#if defined(SI_COMPILER_CLANG) || SI_COMPILER_CHECK_MIN(GCC, 2, 5, 0)
			/* Specifies that the function will not return anything. */
			#define siNoreturn __attribute__((noreturn))

		#elif defined(SI_COMPILER_MSVC)
			/* Specifies that the function will not return anything. */
			#define siNoreturn __declspec(noreturn)

		#else
			/* Specifies that the function will not return anything. */
			#define siNoreturn
		#endif
	#endif
#endif

#if SI_STANDARD_CHECK_MIN(C, C11) || SI_STANDARD_CHECK_MIN(CPP, CPP11)
	/* memberName - NAME | unionMembers - TYPE NAME;<...>
	 * Defines an anonymous union, whose members can be accessed either through
	 * '<var>.<memberName>.<member>' or '<var>.<member>'. The later only compiles
	 * on C11 and onwards. */
	#define union_anonymous(memberName, .../*unionMembers*/) \
		union { \
			__VA_ARGS__ \
			union { \
				__VA_ARGS__ \
			} memberName; \
		}

#else
	/* memberName - NAME | unionMembers - TYPE NAME;<...>
	 * Defines an anonymous union, whose members can be accessed either through
	 * '<var>.<memberName>.<member>' or '<var>.<member>'. The later only compiles
	 * on C11 and onwards. */
	#define union_anonymous(memberName, .../*unionMembers*/) \
		union { \
			__VA_ARGS__ \
		} memberName

#endif

#ifndef rawptr
	/* 'void*' without implicit casting privileges. */
	typedef void* rawptr;
#endif


#ifndef cstring
	/* An immutable NULL-terminated C-string type. */
	typedef const char* cstring;
#endif

/*
	========================
	| Constants            |
	========================
*/

/* x - INT.
 * Converts kilobytes into bytes (JEDEC). */
#define SI_KILO(x) ((x) * 1024)
/* x - INT.
 * Converts megabytes into bytes (JEDEC). */
#define SI_MEGA(x) (SI_KILO(x) * 1024)
/* x - INT.
 * Converts gigabytes into bytes (JEDEC). */
#define SI_GIGA(x) (SI_MEGA(x) * 1024)
/* x - INT.
 * Converts terabytes into bytes (JEDEC). */
#define SI_TERA(x) (SI_GIGA(x) * 1024LL)

/* x - UINT
 * Shortcut for (1ull << x). */
#define SI_BIT(x) (1ULL << (x))


#ifndef nil
	#if SI_LANGUAGE_IS_CPP
		/* NULL value. */
		#define nil nullptr
	#else
		/* NULL value. */
		#define nil ((void*)0)
	#endif
#endif

SI_STATIC_ASSERT(sizeof(nil) == sizeof(void*));

/*
	========================
	| Declaration macros   |
	========================
*/

/* type - TYPE | name - NAME
 * Defines the enum with the given integer type. */
#define SI_ENUM(type, name) typedef type name; enum
/* ...x - ANYTHING
* Silences the 'unused identifier' warning for the specified expression. */
#define SI_UNUSED(.../* x */) (void)(__VA_ARGS__)
/* ...x - ANYTHING
* Discards the function's return and silences the 'unused return value' warning. */
#define SI_DISCARD(.../* x */) (void)!(__VA_ARGS__)

/*
	========================
	| Unary operators      |
	========================
*/
#if !defined(typeof) && !defined(SI_NO_TYPEOF)
	#if SI_STANDARD_CHECK_MIN(C, C23)
		#define SI_TYPEOF_USED 1

	#elif SI_COMPILER_TYPEOF_SUPPORTS
		/* ...VALUE - TYPE/EXPRESSION
		* Gets the value's type and expresses it as a regular type. */
		#define typeof(.../* VALUE */) __typeof__(__VA_ARGS__)

		#define SI_TYPEOF_USED 1

	#elif SI_STANDARD_CHECK_MIN(CPP, CPP11)
		/* ...VALUE - TYPE/EXPRESSION
		* Gets the value's type and expresses it as a regular type. */
		#define typeof(.../* VALUE */) decltype(__VA_ARGS__)

		#define SI_TYPEOF_USED 1

	#endif
#elif defined(typeof) && !defined(SI_NO_TYPEOF) && SI_COMPILER_TYPEOF_SUPPORTS
	#define SI_TYPEOF_USED 1

#endif


#ifndef si_sizeof
	/* expr - EXPRESSION
	 * Returns the size of the expression (in isize). */
	#define si_sizeof(expr) ((isize)sizeof(expr))
#endif

#ifndef countof
	/* value - ARRAY
	* Gets the static length of the given value (must be an array). */
	#define countof(.../*value*/) (isize)(sizeof(__VA_ARGS__) / sizeof((__VA_ARGS__)[0]))
#endif

#ifndef countof_str
	/* value - ARRAY
	* Gets the length of a static NULL-terminated C-string.*/
	#define countof_str(.../* value */) (countof(__VA_ARGS__) - 1)
#endif

SI_STATIC_ASSERT(countof("abcd") == 5);
SI_STATIC_ASSERT(countof_str("abcd") == 4);

#ifndef offsetof
	#if SI_COMPILER_CHECK_MIN(GCC, 3, 4, 0) || SI_COMPILER_CHECK_MIN(CLANG, 2, 6, 0)
		/* type - STRUCT TYPE | element - TYPE's member
		* Returns the offset of the specified member. */
		#define offsetof(type, element) __builtin_offsetof(type, element)
	#else
		/* type - STRUCT TYPE | element - TYPE's member
		* Returns the offset of the specified member. */
		#define offsetof(type, element) ((isize)&(((type*)nil)->element))
	#endif
#endif

#ifndef alignof
	#if SI_STANDARD_CHECK_MIN(C, C11)
		/* type - TYPE
		* Gets the alignment of a type. */
		#define alignof(type) _Alignof(type)
	#elif SI_COMPILER_GCC || SI_COMPILER_CLANG
		/* type - TYPE
		* Gets the alignment of a type. */
		#define alignof(type) __alignof__(type)
	#else
		/* type - TYPE
		* Gets the alignment of a type. */
		#define alignof(type) offsetof(struct { char c; type member; }, member)
	#endif
#endif

/*
	========================
	| Casting              |
	========================
*/
/* type - TYPE | value - EXPRESSION
 * Casts a value to the type provided. */
#define si_cast(type, value) ((type)((value)))

#ifdef SI_TYPEOF_USED
	/* type - TYPE | value - EXPRESSION
	 * Type prunes a value with the specified type ('typeof' is used to get value's type). */
	#define si_transmuteType(type, value) si_transmute(type, value, typeof(value))
#endif

#if SI_LANGUAGE_IS_C
	/* type - TYPE | value - EXPRESSION | valueType - TYPE
	 * Type prunes a value with the specified type. */
	#define si_transmute(type, value, valueType) ((union { valueType in; type out; }){value}.out)
#else
	/* type - TYPE | value - EXPRESSION | valueType - TYPE
	 * Type prunes a value with the specified type. */
	#define si_transmute(type, value, valueType) si_anarchyTransmute<type, valueType>(value)

extern "C++" {
	template<typename type, typename valueType>
	type si_anarchyTransmute(valueType value) {
		SI_STATIC_ASSERT(si_sizeof(type) == si_sizeof(valueType));
		type out;
		si_memcpy(&out, &value, si_sizeof(value));
		return out;
	}
}
#endif

/* ptr - void*
 * Converts the pointer's value into an u8. */
#define SI_TO_U8(ptr)  (*si_transmute(u8*, (const void*)(ptr), const void*))
/* ptr - void*
 * Converts the pointer's value into an u16. */
#define SI_TO_U16(ptr) (*si_transmute(u16*, (const void*)(ptr), const void*))
/* ptr - void*
 * Converts the pointer's value into an u32. */
#define SI_TO_U32(ptr) (*si_transmute(u32*, (const void*)(ptr), const void*))
/* ptr - void*
 * Converts the pointer's value into an u64. */
#define SI_TO_U64(ptr) (*si_transmute(u64*, (const void*)(ptr), const void*))



/*
	========================
	| General macros       |
	========================
*/

typedef struct siCallerLoc {
	isize filenameLen;
	cstring filename;
	isize functionLen;
	cstring function;
	i32 line;
} siCallerLoc;

/* TODO */
#define SI_CALLER_LOC (siCallerLoc){countof_str(__FILE__), __FILE__, countof_str(__func__), __func__, __LINE__}

#ifdef SI_TYPEOF_USED
	/* a - VARIABLE | b - VARIABLE
	 * Swaps the value of 'a' with 'b'; 'b' with 'a'. */
	#define si_swap(a, b) do { typeof((a)) tmp = (a); (a) = (b); (b) = tmp; } while (0)

#else
	/* a - VARIABLE | b - VARIABLE
	 * Swaps the value of 'a' with 'b'; 'b' with 'a'. */
	#define si_swap(a, b) do { \
		SI_ASSERT(si_sizeof(a) == si_sizeof(b));
		char tmp[si_sizeof(a)]; \
		si_memcopy(tmp, &(a), si_sizeof(a)); \
		si_memcopy(&(a), &(a), si_sizeof(a)); \
		si_memcopy(&(b), tmp, si_sizeof(a)); \
	} while (0)

#endif

#ifndef for_range
	#ifdef SI_TYPEOF_USED
		/* countvar - NAME | start - INT | end - INT
		 * A loop with that goes through 'end' - 'start' cycles whilst incrementing 'countVar'
		 * each time. */
		#define for_range(countVar, start, end) for_rangeEx(typeof(end), countVar, start, end)
	#else
		/* countvar - NAME | start - INT | end - INT
		 * A loop with that goes through 'end' - 'start' cycles whilst incrementing 'countVar'
		 * each time. */
		#define for_range(countVar, start, end) for_rangeEx(isize, countVar, start, end)
	#endif
#endif

#ifndef for_rangeEx
	/* type - TYPE | countvar - NAME | start - INT | end - INT
	 * A loop with that goes through 'end' - 'start' cycles whilst incrementing 'countVar'
	 * each time. */
	#define for_rangeEx(type, countVar, start, end) \
		for (type countVar = (start); (countVar) < (end); countVar += 1)
#endif


#if SI_COMPILER_CHECK_MIN(GCC, 3, 4, 0) || SI_COMPILER_CHECK_MIN(CLANG, 2, 6, 0)
	/* x - CONDITION
	 * Denotes that this statement is more likely to be expected. */
	#define SI_LIKELY(x)   (__builtin_expect(!!(x), 1))
	/* x - CONDITION
	 * Denotes that this statement is less likely to be expected. */
	#define SI_UNLIKELY(x) (__builtin_expect(!!(x), 0))
#else
	/* x - CONDITION
	 * Denotes that this statement is more likely to be expected. */
	#define SI_LIKELY(x) (x)
	/* x - CONDITION
	 * Denotes that this statement is less likely to be expected. */
	#define SI_UNLIKELY(x) (x)
#endif

#ifndef SI_DEBUG_TRAP
	#ifdef SI_COMPILER_MSVC
	 	#if SI_COMPILER_VERSION < SI_VERSION(7, 0, 0)
			#define SI_DEBUG_TRAP() __asm int 3
		#else
			#define SI_DEBUG_TRAP() __debugbreak()
		#endif
	#elif SI_COMPILER_CHECK_MIN(GCC, 4, 0, 0) || SI_COMPILER_CHECK_MIN(CLANG, 2, 6, 0)
		#define SI_DEBUG_TRAP() __builtin_trap()
	#else
		#define SI_DEBUG_TRAP() si_exit(1)
	#endif
#endif

#ifdef SI_NO_ASSERTIONS
	#undef SI_NO_ASSERTIONS_IN_HEADER
	#define SI_NO_ASSERTIONS_IN_HEADER
#endif

#ifndef SI_NO_ASSERTIONS_IN_HEADER
	/* condition - EXPRESSION
	 * Terminates the program if the condition is not met. */
	#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
	/* condition - EXPRESSION | message - cstring
	 * Terminates the program with a message if the condition is not met. */
	#define SI_ASSERT_MSG(condition, message) SI_ASSERT_FMT(condition, message, "")
	/* condition - EXPRESSION | message - cstring | ...fmt - VARIADIC
	 * Terminates the program with a formatted message if the condition is not met. */
	#define SI_ASSERT_FMT(condition, message, .../* fmt */) \
		SI_STOPIF(!(condition), si_panic(#condition, SI_CALLER_LOC, message, __VA_ARGS__))
	/* ptr - void*
	 * Terminates the program if a pointer is nil. */
	#define SI_ASSERT_NOT_NIL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL.")
	/* num - INT
	 * Terminates the program if the integer is negative. */
	#define SI_ASSERT_NOT_NEG(num) SI_ASSERT_MSG((num) >= 0, #num " must not be negative.")

#else
	#define SI_ASSERT(condition) do { } while(0)
	#define SI_ASSERT_MSG(condition, message) do {} while(0)
	#define SI_ASSERT_FMT(condition, message, ...) do {} while(0)
	#define SI_ASSERT_NOT_NIL(ptr) do { SI_UNUSED(ptr); } while(0)
	#define SI_ASSERT_NOT_NEG(num) do { SI_UNUSED(num); } while(0)

#endif /* SI_NO_ASSERTIONS_IN_HEADER */

/* Terminates the program immediately. */
#define SI_PANIC() SI_PANIC_MSG(nil)
/* message - cstring
 * Terminates the program immediately with a message. */
#define SI_PANIC_MSG(message) SI_PANIC_FMT(message, "")
/* message - cstring | ...FMT - VARIADIC
 * Terminates the program immediately with a formatted message. */
#define SI_PANIC_FMT(message, ...) si_panic("SI_PANIC()", SI_CALLER_LOC, message, __VA_ARGS__)

/* condition - EXPRESSION | ACTION - ANYTHING
 * Checks if the condition is true. If it is, 'action' will be executed. */
#define SI_STOPIF(condition, .../* ACTION */) if (condition) { __VA_ARGS__; } do {} while(0)


/*
	========================
	| Other                |
	========================
*/

/* An XY point structure. Both are 32-bit integers. */
typedef struct { i32 x, y; } siPoint;
/* x - i32 | y - i32
 * Macro to define an XY i32 point. */
#define SI_POINT(x, y) ((siPoint){x, y})
/* p1 - siPoint | p2 - siPoint
 * Does a quick comparison if the two points are different. */
#define si_pointCmp(p1, p2) (SI_TO_U64(&p1) == SI_TO_U64(&p2))

/* An RGBA structure. */
typedef struct { u8 r, g, b, a; } siColor;
/* r, g, b, a - u8
 * Macro to define an RGBA color. */
#define SI_RGBA(r, g, b, a) ((siColor){r, g, b, a})
/* r, g, b - u8
 * Macro to define an RGB color. */
#define SI_RGB(r, g, b) SI_RGBA(r, g, b, 255)
/* hex - u32
 * Converts an u32 hex value to RGB. */
#define SI_HEX(hex) SI_RGBA(((hex) >> 16) & 0xFF, ((hex) >> 8) & 0xFF, hex & 0xFF, 255)

/* Width and height i32 structure. */
typedef struct { i32 width, height; } siArea;
/* width - i32 | height - i32
 * Macro to define a signed area. */
#define SI_AREA(width, height) ((siArea){width, height})
/* p1 - siArea | p2 - siArea
 * Does a quick comparison if the two areas are different. */
#define si_areaCmp(a1, a2) (SI_TO_U64(&a1) == SI_TO_U64(&a2))

/* Poistion and area structure. */
typedef struct { i32 x, y, width, height; } siRect;
/* x - i32 | y - i32 | width - i32 | height - i32
 * Macro to define a rectangle from 4 32-bit integers. */
#define SI_RECT(x, y, width, height) (siRect){x, y, width, height}
/* x - i32 | y - i32 | area - siArea
 * Macro to define a rectangle from XY and area. */
#define SI_RECT_A(x, y, area) SI_RECT(x, y, (area).width, (area).height)
/* point - siPoint | width - i32 | height - i32
 * Macro to define a rectangle from point and width & height. */
#define SI_RECT_P(point, width, height) SI_RECT((point).x, (point).y, width, height)
/* point - siPoint | area - siArea
 * Macro to define a rectangle from position and area. */
#define SI_RECT_PA(point, area) SI_RECT((point).x, (point).y, (area).width, (area).height)

/* 2D vector structure. */
typedef struct { f32 x, y; } siVec2;
/* x - f32 | y - f32
 * Macro to define a 2D vector from two f32. */
#define SI_VEC2(x, y) ((siVec2){x, y})
/* area - siArea
 * Macro to define a 2D vector from an area. */
#define SI_VEC2_A(area) SI_VEC2((area).width, (area).height)
/* vec1Ptr - siVec2* |  vec2 - siVec2
 * Adds vec2 into vec1Ptr. */
#define si_vec2Add(vec1Ptr, vec2) (vec1Ptr)->x += (vec2).x; (vec1Ptr)->y += (vec2).y
/* vec1Ptr - siVec2* |  vec2 - siVec2
 * Subtracts vec2 from vec1Ptr. */
#define si_vec2Sub(vec1Ptr, vec2) (vec1Ptr)->x -= (vec2).x; (vec1Ptr)->y -= (vec2).y

/* 3D vector structure. */
typedef struct { f32 x, y, z; } siVec3;
/* x - f32 | y - f32 | z - f32
 * Macro to define a 3D vector from three f32. */
#define SI_VEC3(x, y, z) ((siVec3){x, y, z})
/* 4D vector structure. */
typedef struct { f32 x, y, z, w; } siVec4;
/* x - f32 | y - f32 | z - f32 | w - f32
 * Macro to define a 4D vector from three f32. */
#define SI_VEC4(x, y, z, w) ((siVec4){x, y, z, w})
/* rect - siRect
 * Macro to define a 4D vector from a rectangle. */
#define SI_VEC4_R(rect) SI_VEC4((f32)(rect).x, (f32)(rect).y, (f32)(rect).width, (f32)(rect).height)
/* vec1 - siVec2 | vec2 - siVec2
 * Macro to define a 4D vector from two 2D vectors. */
#define SI_VEC4_2V(vec1, vec2) SI_VEC4((vec1).x, (vec1).y, (vec2).x, (vec2).y)


#ifndef SI_PATH_MAX
	/* The maximum amount of UTF-8 bytes a singular path can hold. */
	#define SI_PATH_MAX SI_KILO(4)
#endif


#ifndef SI_NO_OPTIONAL
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siError & siOptional |
	========================
*/

/*
	========================
	|  siError             |
	========================
*/

/* A struct containing information about an error that was declared in a function. */
typedef struct siError {
	/* Error code. */
	i32 code;

#ifndef SI_NO_ERROR_STRUCT
	/* The line number where the error was declared. */
	i32 line;
	/* TODO */
	isize filenameLen;
	/* The file where the error was declared. */
	cstring filename;
	/* The time when the error was declared (in UTC+0). */
	i64 time;
#endif
} siError;

/* Sets the error code to zero. */
#define SI_ERROR_NIL (siError){.code = 0}


/* name - NAME
 * Defines a valid error logger function prototype. Returning 'false' terminates
 * the app. */
#define SI_ERROR_PROC(name) b32 name(siError* error, void* data)
/* Represents an error logger procedure. */
typedef SI_ERROR_PROC(siErrorProc);

/* code - i32
  * Declares an error with a generic error log. */
#define SI_ERROR(code) SI_ERROR_EX(code, nil, nil)
/* code - i32 | function - siErrorProc (nullable) | data - void* (nullable)
 * Declares an error with a specified error log function and pointer data. Specifying
 * nil for the function defaults to the generic error log. */
#define SI_ERROR_EX(code, function, data) si__errorDeclare(code, function, data, SI_CALLER_LOC)

/* condition - b32 | code - i32 | ...action - ACTION
 * If the condition is true, an error is declared with a generic error log and
 * the specified actions are executed. The result of 'SI_ERROR_EX' is saved in
 * the 'SI_ERROR_RES' variable. */
#define SI_ERROR_CHECK(condition, code, .../*action*/) \
	SI_ERROR_CHECK_EX(condition, code, nil, nil, __VA_ARGS__)
/* condition - b32 | code - i32 | func - siErrorProc (nullable) | data - void* (nullable) | ...action - ACTION
 * If the condition is true, an error is declared with a specified error log and
 * and the specified actions are executed. The result of 'SI_ERROR_EX' is saved
 * in the 'SI_ERROR_RES' variable. Specifying nil for the function defaults to the
 * generic error log. */
#define SI_ERROR_CHECK_EX(condition, code, func, data, .../*action*/) \
	SI_STOPIF(condition, siError SI_ERROR_RES = SI_ERROR_EX(code, func, data); SI_UNUSED(SI_ERROR_RES); __VA_ARGS__);

/* condition - b32 | code - i32
 * If the condition is true, an error is declared with a generic error log and
 * gets returned. */
#define SI_ERROR_CHECK_RET(condition, code) SI_ERROR_CHECK_EX_RET(condition, code, nil, nil)
/* condition - b32 | code - i32 | func - siErrorProc (nullable) | data - void* (nullable)
 * If the condition is true, an error is declared with a specified error log and
 * gets returned. Specifying nil for the function defaults to the generic error log.*/
#define SI_ERROR_CHECK_EX_RET(condition, code, func, data) \
	SI_ERROR_CHECK_EX(condition, code, func, data, return SI_ERROR_RES)



/* Function that internally handles error handling. No reason to call this. */
SIDEF siError si__errorDeclare(i32 error, siErrorProc proc, void* userData,
		siCallerLoc call);

/*
	========================
	|  siOptional          |
	========================
*/


/* type - TYPE
 * Defines an optional type using the specified type. */
#define si_optional_define(type) \
	typedef struct siOption_##type { \
		b32 hasValue; \
		union_anonymous(data, \
			type value; \
			siError error; \
		); \
	} siOption_##type


/* type - TYPE
 * Represents an object that may or may not contain a value. If the object has
 * no value, additional information _won't_ be written in the structure. */
#define siOption(type) si__OptionMacro(type)
/* type - TYPE
 * Represents an object that may or may not contain a value. If the object has
 * no value, additional information _will be_ written in the structure. */
#define siResult(type) siOption(type)


si_optional_define(u8);
si_optional_define(u16);
si_optional_define(u32);
si_optional_define(u64);
si_optional_define(usize);

si_optional_define(i8);
si_optional_define(i16);
si_optional_define(i32);
si_optional_define(i64);
si_optional_define(isize);

si_optional_define(f32);
si_optional_define(f64);

si_optional_define(rawptr);


/* type - TYPE | ...VALUE - EXPRESSION
 * Creates a returnable 'siOptional' value from the given value. */
#define SI_OPT(type, .../* VALUE */) (siOption(type)){true, .data = {__VA_ARGS__}}
/* type - TYPE
 * Creates a returnable 'siOptional' item that has no value inside.. */
#define SI_OPT_NIL(type) SI_OPT_ERR(type, SI_ERROR_NIL)
/* type - TYPE | errorV - siError
 * Creates a returnable 'siOptional' error value with a designated error. */
#define SI_OPT_ERR(type, errorV) (siOption(type)){false, .data = {.error = errorV}}


/* optionalVar - siOptional(TYPE) | defaultValue - EXPRESSION
 * Gets the value of the provided 'siObject' object. However if 'hasValue' is set
 * to false, return 'defaultValue' instead. */
#define si_optionalGetOrDefault(optionalVar, defaultValue) \
	((optionalVar).hasValue ? (optionalVar).data.value : (defaultValue))


/* condition - b32 | code - i32 | type - TYPE
 * If the condition is true, an error is declared with a generic error log and
 * an error optional value gets returned. */
#define SI_OPTION_CHECK(condition, code, type) SI_OPTION_CHECK_EX(condition, code, nil, nil, type)
/* condition - b32 | code - i32 | func - siErrorProc (nullable) | data - void* (nullable) | type - TYPE
 * If the condition is true, an error is declared with a specified error log and
 * an error optional value gets returned. Specifying nil for the function defaults
 * to the generic error log.*/
#define SI_OPTION_CHECK_EX(condition, code, func, param, type) \
	SI_ERROR_CHECK_EX(condition, code, func, param, return SI_OPT_ERR(type, SI_ERROR_RES))



#if 1
	#define si__OptionMacro(type) siOption_##type
#endif

#endif /* SI_NO_OPTIONAL */

#ifndef SI_NO_MEMORY
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siMemory             |
	========================
*/

/* Default memory alignment for the system. */
#ifndef SI_DEFAULT_MEMORY_ALIGNMENT
	#define SI_DEFAULT_MEMORY_ALIGNMENT (2 * si_sizeof(void*))
#endif

#ifndef SI_DEFAULT_PAGE_SIZE
	#if SI_SYSTEM_IS_WASM
		#define SI_DEFAULT_PAGE_SIZE SI_KILO(64)
	#elif SI_SYSTEM_IS_APPLE && SI_ARCH_ARM64
		#define SI_DEFAULT_PAGE_SIZE SI_KILO(16)
	#else
		#define SI_DEFAULT_PAGE_SIZE SI_KILO(4)
	#endif
#endif



/* Checks if the given alignment is in power of two. */
SIDEF b32 si_isPowerOfTwo(isize x);
/* Aligns the number to the specified alignment. */
SIDEF usize si_alignForward(usize num, isize alignment);


/* Adds a specified amount of bytes to the pointer. */
SIDEF void* si_pointerAdd(void* ptr, isize bytes);
/* Subtracts a specified amount of bytes from the pointer. */
SIDEF void* si_pointerSub(void* ptr, isize bytes);
/* Adds a specified amount of bytes to the const pointer. */
SIDEF const void* si_pointerAddConst(const void* ptr, isize bytes);
/* Subtracts a specified amount of bytes to the const pointer. */
SIDEF const void* si_pointerSubConst(const void* ptr, isize bytes);
/* Returns the difference between the end and beginning pointers. */
SIDEF isize si_pointerDiff(const void* begin, const void* end);
/* Returns true if a pointer between the start and end pointers. */
SIDEF b32 si_pointerBetween(const void* ptr, const void* start, const void* end);


/* Copies the given amount of bytes from the provided source into the specified
 * destination. The memory blocks cannot overlap each other. */
SIDEF isize si_memcopy(void* restrict dst, const void* restrict src, isize size);
/* Copies the given amount of bytes from the provided source into the specified
 * destination. The memory blocks can overlap each other. */
SIDEF isize si_memmove(void* restrict dst, const void* restrict src, isize size);
/* Sets the given amount of bytes from the provided data source to the specified
 * value. */
SIDEF isize si_memset(void* data, u8 value, isize size);
/* Compares the given of bytes from two specified pointers and returns either:
 * A) zero, if all bytes match B) less than zero, if the first clashing byte in
 * the first pointer is lower than in the second one C) more than zero, if the
 * first clashing byte in the first pointer is higher than the second one. */
SIDEF i32 si_memcompare(const void* ptr1, const void* ptr2, isize size);
/* Searches the given amount of bytes from the provided data source and returns
 * either a pointer containing the first occurence of the specified value, or a
 * nil pointer if there were no occurences. */
SIDEF void* si_memchr(const void* data, u8 value, isize size);

/* Moves the specified memory block to the left by the given amount of bytes. */
SIDEF isize si_memmoveLeft(void* src, isize size, isize moveBy);
/* Moves the specified memory block to the right by the given amount of bytes. */
SIDEF isize si_memmoveRight(void* src, isize size, isize moveBy);


/* Returns the length of a NULL-terminated C-string. */
SIDEF isize si_cstrLen(cstring string);

#endif /* SI_NO_MEMORY */

#ifndef SI_NO_VIRTUAL_MEMORY
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siVirtualMemory      |
	========================
*/

typedef struct siVirtualMemory {
	void* data;
	isize size;
} siVirtualMemory;

si_optional_define(siVirtualMemory);

/* TODO */
SIDEF siResult(siVirtualMemory) si_vmAlloc(void* address, isize size);
/* TODO */
SIDEF siError si_vmFree(siVirtualMemory vm);
/* TODO */
SIDEF siError si_vmPurge(siVirtualMemory vm);


#endif /* SI_NO_VIRTUAL_MEMORY */

#ifndef SI_NO_ALLOCATOR
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siAllocator          |
	========================
*/

SI_ENUM(i32, siAllocatorFuncEnum) {
	siAllocatorFunc_Alloc,
	siAllocatorFunc_Resize,
	siAllocatorFunc_Free,
	siAllocatorFunc_FreeAll,

	siAllocatorFunc_GetAvailable,
};

typedef struct siAllocatorDataResize {
	void* src;
	usize sizeOld, sizeNew;
} siAllocatorDataResize;

typedef struct siAllocatorFunc {
	siAllocatorFuncEnum type;
	union_anonymous(data,
		usize alloc;
		void* free;
		siAllocatorDataResize resize;
	);
} siAllocatorFunc;

/* name - NAME
 * Defines a valid allocator function prototype.*/
#define SI_ALLOCATOR_PROC(name) void* name(siAllocatorFunc function, void* data)
/* Represents an allocator procedure. */
typedef SI_ALLOCATOR_PROC(siAllocatorProc);

/* Represents an allocator type. */
typedef struct siAllocator { siAllocatorProc* proc; void* data; } siAllocator;


/* Allocates the specified amount of bytes of storage. */
SIDEF void* si_alloc(siAllocator alloc, isize bytes);
/* Reallocates the specified memory block from the given old size to the new. */
SIDEF void* si_realloc(siAllocator alloc, void* ptr, isize sizeOld, isize sizeNew);
/* Frees a previously allocated memory block from 'si_alloc'. */
SIDEF void si_free(siAllocator alloc, void* ptr);
/* Frees all of the previous allocated memory within the specified allocator.
 * NOTE: Not all allocators can support this function. */
SIDEF void si_freeAll(siAllocator alloc);

/* Returns the available space left inside the specified allocator. */
SIDEF isize si_allocatorAvailable(siAllocator alloc);


/* allocator - siAllocator* | type - TYPE
 * Allocates an exact amount of storage to fit the specified type. */
#define si_allocItem(allocator, type) (type*)si_alloc(allocator, si_sizeof(type))
/* allocator - siAllocator | type - TYPE | count - isize
 * Allocates an exact amount of storage to fit the specified array of types. */
#define si_allocArray(allocator, type, count) (type*)si_alloc(allocator, si_sizeof(type) * (count))


/*
 * Heap allocator
 *
 * Description:
 * - An allocator that utilizes 'malloc', 'realloc' and 'free' functions of C stdlib.
 *
 * Functionality:
 * - si_alloc - 'malloc(requestedSize)'.
 *
 * - si_realloc - 'realloc(newRequestedSize)'. The 'oldSize' argument is ignored.
 *
 * - si_free - 'free(ptr)'.
 *
 * - si_freeAll - UNSUPPORTED.
 *
 * NOTE:
 * 'si_allocatorAvailable' always returns an 'ISIZE_MAX'. */
SIDEF SI_ALLOCATOR_PROC(si_allocator_heap_proc);

/* Returns the heap allocator. */
SIDEF siAllocator si_allocatorHeap(void);

#ifndef si_stack
	/* bytes - usize
	 * Stack allocates the specified amount of bytes of storage. */
	#define si_stack(bytes) si_cast(void*, (u8[bytes]){0})
#endif

/* TODO */
#define si_stackAlloc(bytes) si_stack(bytes), (bytes)


#ifndef si_malloc
	/* bytes - isize
	 * Heap allocates the specified amount of bytes of storage. */
	#define si_malloc(bytes) si_alloc(si_allocatorHeap(), bytes)
	/* ptr - void*
	 * Deallocates a previously allocated memory block from 'si_malloc'. */
	#define si_mfree(ptr) si_free(si_allocatorHeap(), ptr)
#endif

/* type - TYPE
 * Heap allocates an exact amount of storage to fit the specified type. */
#define si_mallocItem(type) (type*)si_malloc(si_sizeof(type))
/* type - TYPE | count - isize
 * Heap allocates an exact amount of storage to fit the specified array of types. */
#define si_mallocArray(type, count) (type*)si_malloc(si_sizeof(type) * (count))

/*
	========================
	| siArena              |
	========================
*/

/* Reprents an arena allocator. */
typedef struct siAllocatorArena {
	siAllocator alloc;
	u8* ptr;
	isize offset;
	isize capacity;
	i32 alignment;
} siAllocatorArena;

/*
 * Arena allocator
 *
 * Description:
 * - An allocator that allocates one large memory region and linearly assigns
 * each allocated element to a free section of said region.
 *
 * - In practice, if one has an arena of 128 bytes and allocates 32 bytes, the
 * remaining 96 bytes will be available for further allocations.
 *
 *
 * Functionality:
 * - si_alloc - reserves 'aligned(requestedSize)' amount of bytes for each allocation.
 *   If the user surpasses the arena's capacity, a panic is thrown.
 *
 * - si_realloc - allocates 'aligned(newRequestedSize)' amount of bytes for the
 *   allocation. If the old size is bigger than the new one, nothing is done.
 *
 * - si_free - UNSUPPORTED.
 *
 * - si_freeAll - sets the internal offset to zero, effectively freeing everything.
 *
 *
 * Notes:
 * 1) Reallocations are expensive and not reccomended, as the old memory doesn't
 * get freed and instead a larger element is allocated and returned for the new
 * size.
 * 2) Individual element frees are not supported as that would be very expensive
 * to implement and remove the purpose of an arena (you're suppose to free the
 * entire arena, not just a few elements). */
SIDEF SI_ALLOCATOR_PROC(si_allocator_arena_proc);

/* Creates an arena allocator. */
SIDEF siAllocatorArena si_arenaMake(siAllocator alloc, isize capacity);
SIDEF siAllocatorArena si_arenaMakeEx(siAllocator alloc, isize capacity, i32 alignment);
SIDEF siAllocatorArena si_arenaMakePtr(void* ptr, isize capacity, i32 alignment);

/* Returns an arena allocator procedure. */
SIDEF siAllocator si_allocatorArena(siAllocatorArena* arena);

/* Destroys an arena allocator. */
SIDEF void si_arenaFree(siAllocatorArena* arena);

/*
 * Temporary (arena) memory allocator
 *
 * Stores the arena's offset at that current moment (basically a savepoint),
 * denoting that the user is currently allocating temporary, very short-lived
 * memory. When the end of the temporary memory is called, the saved offset
 * gets set as the new internal offset of the allocator. */
typedef struct siAllocatorArenaTmp {
	siAllocatorArena* arena;
	isize offset;
} siAllocatorArenaTmp;

/* Denotes the start of temporary memory. */
SIDEF siAllocatorArenaTmp si_arenaTmpStart(siAllocatorArena* arena);
/* Denotes the end of temporary memory. */
SIDEF void si_arenaTmpEnd(siAllocatorArenaTmp tmp);


/*
	========================
	| siLifo               |
	========================
*/

/* Reprents a LIFO stack-based allocator. */
typedef siAllocatorArena siAllocatorLifo;

/*
 * Stack-like LIFO (last in, first out) stack-based allocator
 *
 *
 * Description:
 * - An allocator that follows stack-based principles of the last allocated element
 * being the first to be deallocated. Works similiarly to an arena with the added
 * bonus of being able to any last pointer in the allocator.
 *
 *
 * Functionality:
 * - si_alloc - the allocator reserves 'aligned(si_sizeof(usize) + requestedSize)'
 *   amount of bytes for each allocation. The allocator stores how many bytes
 *   each pointer takes up so that every element can be freed at any given state.
 *
 * - si_realloc - UNSUPPORTED.
 *
 * - si_free - frees _everything_ up to that pointer. For example, if an allocated
 *   element is second-to-last, the last element also gets deallocated.
 *
 * - si_freeAll - sets the internal offset to zero, effectively freeing everything.
 *
 *
 * Notes:
 * 1) Compared to arenas, LIFO allocations are slightly more expensive in terms of
 * memory consumption, as each allocation requires to store one 'usize' for the
 * length.
 * 2) Reallocations are not supported as they would break the entire LIFO structure
 * and make individual frees impossible to do. */
SIDEF SI_ALLOCATOR_PROC(si_allocator_lifo_proc);

/* Creates a LIFO allocator. */
SIDEF siAllocatorLifo si_lifoMake(siAllocator alloc, isize capacity);
SIDEF siAllocatorLifo si_lifoMakeEx(siAllocator alloc, isize capacity, i32 alignment);
SIDEF siAllocatorLifo si_lifoMakePtr(void* ptr, isize capacity, i32 alignment);

/* Returns a LIFO allocator procedure. */
SIDEF siAllocator si_allocatorLifo(siAllocatorLifo* lifo);

/* Destroy a LIFO allocator. */
SIDEF void si_lifoFree(siAllocatorLifo* lifo);


/*
	========================
	| siAllocatorPool      |
	========================
*/

typedef struct siAllocatorPoolFreeNode {
	struct siAllocatorPoolFreeNode* next;
} siAllocatorPoolFreeNode;

/* Reprents a pool allocator. */
typedef struct siAllocatorPool {
	siAllocator alloc;
	u8* ptr;
	isize numChunks;
	isize chunkSize;
	i32 alignment;

	siAllocatorPoolFreeNode* head;
} siAllocatorPool;

/*
 * Pool allocator
 *
 * Description:
 * TODO
 *
 * Functionality:
 * - si_alloc - TODO
 *
 * - si_realloc - UNSUPPORTED.
 *
 * - si_free - TODO
 *
 * - si_freeAll - TODO */
SIDEF SI_ALLOCATOR_PROC(si_allocator_pool_proc);

/* Creates a pool allocator. */
SIDEF siAllocatorPool si_poolMake(siAllocator alloc, isize numChunks, isize chunkSize);
SIDEF siAllocatorPool si_poolMakeEx(siAllocator alloc, isize numChunks, isize chunkSize, i32 alignment);

/* Returns a pool allocator procedure. */
SIDEF siAllocator si_allocatorPool(siAllocatorPool* pool);

/* Destroys a pool allocator. */
SIDEF void si_poolFree(siAllocatorPool* pool);


//SIDEF SI_ALLOCATOR_PROC(si_allocator_scratch_proc);

#endif /* SI_NO_ALLOCATOR */

#ifndef SI_NO_ARRAY
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siArray             |
	========================
*/

typedef struct siBufferAny {
	isize len;
	void* data;
	isize typeSize;
} siBufferAny;

/* Defines a 'siBuffer' optional type. */
si_optional_define(siBufferAny);

/* type - TYPE
 * Represents a buffer with a specific type. */
#define siBuffer(type) siBufferAny

/* TODO */
#define for_eachBuf(name, buffer) for_eachBufEx(name, si__i, buffer)
/* TODO */
#define for_eachBufEx(name, indexName, buffer) \
	SI_ASSERT_NOT_NIL(buffer.data); \
	SI_ASSERT_MSG(si_sizeof(name) == (buffer).typeSize, "Invalid type specified."); \
	if ((buffer).len != 0) { si_memcopy(&name, (buffer).data, si_sizeof(name)); } \
	for (isize indexName = 0; indexName < (buffer).len; indexName += 1, \
		 si_memcopy(&name, (u8*)(buffer).data + indexName * si_sizeof(name), si_sizeof(name)))

/* TODO */
#define for_eachRefBuf(name, buffer) for_eachRefBufEx(name, si__i, buffer)
/* TODO */
#define for_eachRefbufEx(name, indexName, buffer) \
	SI_ASSERT_NOT_NIL(buffer.data); \
	SI_ASSERT_MSG(si_sizeof(*name) == (buffer).typeSize, "Invalid type specified."); \
	if ((buffer).len != 0) { si_memcopy(&name, (buffer).data, si_sizeof(void*)); } \
	for (isize indexName = 0; indexName < (buffer).len; indexName += 1, name += 1) \


/* TODO */
#define for_eachRevBuf(name, buffer) for_eachRevBufEx(name, si__i, buffer)
/* TODO */
#define for_eachRevBufEx(name, indexName, buffer) \
	SI_ASSERT_NOT_NIL(buffer.data); \
	SI_ASSERT_MSG(si_sizeof(name) == (buffer).typeSize, "Invalid type specified."); \
	if ((buffer).len != 0) { si_memcopy(&name, (u8*)(buffer).data + ((buffer).len - 1) * si_sizeof(name), si_sizeof(name)); } \
	for (isize indexName = (buffer).len - 1; indexName >= 0; indexName -= 1, \
		si_memcopy(&name, (u8*)((buffer).data) + indexName * si_sizeof(name), si_sizeof(name)))

/* TODO */
#define for_eachRevRefBuf(name, buffer) for_eachRevRefBufEx(name, si__i, buffer)
/* TODO */
#define for_eachRevRefBufEx(name, indexName, buffer) \
	SI_ASSERT_NOT_NIL(buffer.data); \
	SI_ASSERT_MSG(si_sizeof(*name) == (buffer).typeSize, "Invalid type specified."); \
	if ((buffer).len != 0) { si_copyAny(name, ((u8*)(buffer).data)[(buffer).len - 1 * si_sizeof(name)], si_sizeof(void*)); } \
	for (isize indexName = (buffer).len - 1; indexName >= 0; indexName += 1, name -= 1) \



/* type - TYPE | ...values - ANYTHING
 * Makes an array from the given type and values. */
#define SI_BUF(type, .../* values*/) SI_BUF_LEN(((type[]){__VA_ARGS__}), countof((type[]){__VA_ARGS__}))
/* TODO */
#define SI_BUF_ALLOC(type, length, alloc) SI_BUF_TYPE(si_allocArray(alloc, type, length), length, type)
/* TODO */
#define SI_BUF_LEN(buffer, length) SI_BUF_EX(buffer, length, si_sizeof(*(buffer)))
/* TODO */
#define SI_BUF_TYPE(buffer, length, type) SI_BUF_EX(buffer, length, si_sizeof(type))
/* TODO */
#define SI_BUF_EX(buffer, length, typeSize) (siBufferAny){length, buffer, typeSize}

/* bytes - usize
 * Stack allocates the specified amount of bytes and creates a siBuffer from it. */
#define SI_BUF_STACK(bytes) SI_BUF_STACK_EX(u8, bytes)
/* type - TYPE | len - usize
 * Stack allocates the specified amount of typed-elements and creates a siBuffer from it. */
#define SI_BUF_STACK_EX(type, len) SI_BUF_LEN((type*)si_stack(len * sizeof(type)), len)


/* Returns a slice from the specified start to the specified index end. */
SIDEF siBufferAny si_slice(siBufferAny buffer, isize start, isize end);
/* Returns a slice from the specified start to the end (&array[array.capacity]). */
SIDEF siBufferAny si_sliceEnd(siBufferAny buffer, isize start);
/* Returns a slice from index 0 to the specified end. */
SIDEF siBufferAny si_sliceStart(siBufferAny buffer, isize end);
/* Returns a slice from the specified start with a length. */
SIDEF siBufferAny si_sliceLen(siBufferAny buffer, isize start, isize length);

/* TODO */
SIDEF void* si_bufferGet(siBufferAny buffer, isize index);
/* TODO */
#define si_bufferGetType(buffer, index, type) (type*)si__bufferGetType(buffer, index, si_sizeof(type))
/* Returns a pointer to the first element (&array[0]) in the array. */
SIDEF void* si_bufferFront(siBufferAny buffer);
/* Returns a pointer to the past-the-end element (&array[array.len]) in the array. */
SIDEF void* si_bufferBack(siBufferAny buffer);
/* TODO */
SIDEF void si_bufferSet(siBufferAny buffer, isize index, const void* data);

/* Copies an element at the specified index into the given pointer and returns
 * true if the index is less than the array's length. Otherwise, 'false' is returned. */
SIDEF b32 si_bufferAtGet(siBufferAny buffer, isize index, void* out);
/* Copies the first element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF b32 si_bufferAtFront(siBufferAny buffer, void* out);
/* Copies the last element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF b32 si_bufferAtBack(siBufferAny buffer, void* out);

/* Searches for the specified pointer's content in the array (from the beginning).
 * If found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_bufferFind(siBufferAny buffer, void* valuePtr);
/* Searches for the specified pointer's content in the array (from the end). If
 * found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_bufferFindLast(siBufferAny buffer, void* valuePtr);
/* Returns the amount of times the specified pointer's value repeats in the array. */
SIDEF isize si_bufferFindCount(siBufferAny buffer, void* valuePtr);

/* Returns true if the two specified arrays are equal in length and contents. */
SIDEF b32 si_bufferEqual(siBufferAny lhs, siBufferAny rhs);


#if 1
SIDEF void* si__bufferGetType(siBufferAny buffer, isize index, isize sizeofTypeof);
#endif


/* Copies a number of bytes from the source buffer into the destination. The number
* of copied bytes is guaranteed to be less or equal to the destination's size.
* The memory blocks cannot overlap each other. Returns the amount of bytes that
* were copied. */
SIDEF isize si_memcopy_s(siBufferAny dst, const void* src, isize sizeSrc);
/* Copies a number of bytes from the source buffer into the destination. The number
* of copied bytes is guaranteed to be less or equal to the destination's size.
* The memory blocks can overlap each other. Returns the amount of bytes that
* were moved. */
SIDEF isize si_memmove_s(siBufferAny dst, const void* src, isize sizeSrc);



typedef struct siArrayAny {
	isize len;
	void* data;
	isize typeSize;

	siAllocator alloc;
	isize capacity;
	isize grow;
} siArrayAny;

/* Defines a 'siArray' optional type. */
si_optional_define(siArrayAny);


/* type - TYPE
 * Represents a buffer with a specific type. */
#define siArray(type) siArrayAny

/* TODO */
#define SI_BUF_ARR(array) SI_BUF_EX((array).data, (array).len, (array).typeSize)

#ifndef SI_BUFFER_NEW_CAP
	/* TODO */
	#define SI_BUFFER_NEW_CAP(bufferPtr, addLen) (2 * ((bufferPtr)->capacity + addLen))
#endif


/* alloc - siAllocator | type - TYPE | ...values - VARIADIC
 * Allocates an array from the specified type and static items. */
#define si_arrayMake(alloc, type, .../* values */) \
	si_arrayMakeEx((type[]){__VA_ARGS__}, si_sizeof(type), countof((type[]){__VA_ARGS__}), alloc)
/* Allocates 'count * typeSize' amount of bytes to make an array and copies the
 * same amount of bytes from the specified list to it, whilst setting the length
 * to 'count'. */
SIDEF siArrayAny si_arrayMakeEx(void* list, isize typeSize, isize count,
		siAllocator alloc);
/* Allocates 'capacity * typeSize' amount of bytes to make an array, whilst
 * setting the length to the specified size. */
SIDEF siArrayAny si_arrayMakeReserve(isize typeSize, isize length, isize capacity,
		siAllocator alloc);
/* Creates an array from a static buffer. */
SIDEF siArrayAny si_arrayCopy(siBufferAny array, siAllocator alloc);

/* Frees the allocated array. */
SIDEF void si_arrayFree(siArrayAny array);


/* TODO */
SIDEF void* si_arrayGet(siArrayAny array, isize index);
/* TODO */
#define si_arrayGetType(array, index, type) si_bufferGetType(SI_BUF_ARR(array), index, type)
/* Returns a pointer to the first element (&array[0]) in the array. */
SIDEF void* si_arrayFront(siArrayAny array);
/* Returns a pointer to the past-the-end element (&array[array.len]) in the array. */
SIDEF void* si_arrayBack(siArrayAny array);
/* TODO */
SIDEF void si_arraySet(siArrayAny array, isize index, void* data);

/* Copies an element at the specified index into the given pointer and returns
 * true if the index is less than the array's length. Otherwise, 'false' is returned. */
SIDEF b32 si_arrayAtGet(siArrayAny array, isize index, void* out);
/* Copies the first element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF b32 si_arrayAtFront(siArrayAny array, void* out);
/* Copies the last element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF b32 si_arrayAtBack(siArrayAny array, void* out);

/* Searches for the specified pointer's content in the array (from the beginning).
 * If found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_arrayFind(siArrayAny array, void* valuePtr);
/* Searches for the specified pointer's content in the array (from the end). If
 * found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_arrayFindLast(siArrayAny array, void* valuePtr);
/* Returns the amount of times the specified pointer's value repeats in the array. */
SIDEF isize si_arrayFindCount(siArrayAny array, void* valuePtr);

/* Returns true if the two specified arrays are equal in length and contents. */
SIDEF b32 si_arrayEqual(siArrayAny lhs, siArrayAny rhs);


/* Appends the specified pointer's value to the array. Returns true if the array
 * was reallocated. */
SIDEF b32 si_arrayAppend(siArrayAny* array, void* valuePtr);
/* Appends the specified list of items to the array. Returns true if the array
 * was reallocated. */
SIDEF b32 si_arrayAppendEx(siArrayAny* array, void* valueArray, isize count);
/* Erases the last item in the array. */
SIDEF void si_arrayPop(siArrayAny* array);
/* Sets the array's length to zero. */
SIDEF void si_arrayClear(siArrayAny* array);

/* Inserts the specified pointer's value at the given index of the array. Returns
 * true if the array was reallocated. */
SIDEF b32 si_arrayInsert(siArrayAny* array, isize index, void* value);
/* Inserts the specified list of items at the given index of the array. Returns
 * true if the array was reallocated. */
SIDEF b32 si_arrayInsertEx(siArrayAny* array, isize index, void* valueArray, isize count);
/* Erases the specified pointer's value at the given index of the array. */
SIDEF void si_arrayErase(siArrayAny* array, isize index);
/* Erases the specified list of items at the given index of the array. */
SIDEF void si_arrayEraseEx(siArrayAny* array, isize index, isize count);

/* Reverses the contents of the array. */
SIDEF void si_arrayReverse(siArrayAny array);
/* Fills the contents of the array with the specified pointer's value. Returns
 * true if the array was reallocated. */
SIDEF b32 si_arrayFill(siArrayAny* array, isize index, isize count, void* valuePtr);

/* Replaces all occurences of the firstly specified pointer's value with the
 * secondly specified one. */
SIDEF void si_arrayReplaceAll(siArrayAny array, void* restrict valueOld,
		void* restrict valueNew);
/* Replaces a given amount of the firstly specified pointer's value with the
 * secondly specified one. */
SIDEF void si_arrayReplace(siArrayAny array, void* restrict valueOld,
		void* restrict valueNew, isize amount);

/* If needed, reallocates the array for the added space. Returns true if the
 * array was reallocated. Used internally. */
SIDEF b32 si_arrayMakeSpaceFor(siArrayAny* buffer, isize addLen);

#endif /* SI_NO_ARRAY */

#ifndef SI_NO_STRING
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siString             |
	========================
*/

typedef struct siBuilder {
	siAllocator alloc;

	u8* data;
	isize len;
	isize capacity;
	isize grow;
} siBuilder;

/* Represents a string type. */
typedef struct siString {
	/* TODO */
	isize len;
	/* TODO */
	const u8* data;
} siString;

/* Defines a 'siString' optional type. */
si_optional_define(siString);


/* TODO */
#define for_eachStr(charName, string) for_eachStrEx(charName, si__i, string)
/* TODO */
#define for_eachStrEx(charName, indexName, string) __for_eachStrImpl1(charName, indexName, string, __LINE__)

/* TODO */
#define for_eachRevStr(charName, string) for_eachRevStrEx(charName, si__i, string)
/* TODO */
#define for_eachRevStrEx(charName, indexName, string) __for_eachRevStrImpl1(charName, indexName, string, __LINE__)


/* string - cstring
 * Makes a string from a static NULL-terminated C-string. */
#define SI_STR(string) SI_STR_LEN(string, countof_str(string))
/* string - STR | len - isize
 * Makes a string from a length-specified C-string. */
#define SI_STR_LEN(string, len) (siString){len, (u8*)string}

/* string - char*
 * Creates a string from a NULL-terminated C-string. */
#define SI_CSTR(string) SI_STR_LEN((char*)string, si_cstrLen(string))
/* string - cstring
 * Creates a compile-time constant string from a static NULL-terminated C-string. */
#define SI_STRC(string) {countof_str(string), (u8*)string}

/* TODO */
#define SI_STR_EMPTY SI_STR_LEN("", 0)
/* TODO */
#define SI_STR_NIL SI_STR_LEN(nil, 0)

/* TODO */
typedef i32 siRune;


/* string - siString | index - isize
 * Returns a character at the specified index of the string. */
#define si_stringGet(string, index) si_arrayGet(string, index, u8)

/* Allocates a duplicated string. */
SIDEF siString si_stringCopy(siString from, siAllocator alloc);
/* TODO */
SIDEF siString si_stringCopyFromCstr(cstring from, siAllocator alloc);
/* Allocates a NULL-terminated C-string from a string. */
SIDEF char* si_stringCopyToCstr(siString from, siAllocator alloc);
/* Allocates a NULL-terminated C-string with a specified capacity from a string. */
SIDEF char* si_stringCloneToCstrEx(siString from, isize capacity, siAllocator alloc);


/* Gets the string's first character. */
SIDEF i32 si_stringAtFront(siString string);
/* Gets the string's last character. */
SIDEF i32 si_stringAtBack(siString string);
/* Gets a pointer to the string's first character. */
SIDEF const u8* si_stringBegin(siString string);
/* Gets pointer to the string's past-the-end element. */
SIDEF const u8* si_stringEnd(siString string);


/* Returns a substring from the specified start to the specified index end. */
SIDEF siString si_substr(siString string, isize start, isize end);
/* Returns a substring from the specified start to the end. */
SIDEF siString si_substrEnd(siString string, isize start);
/* Returns a substring from index 0 to the specified end. */
SIDEF siString si_substrStart(siString string, isize end);
/* Returns a substring from the specified start with a length. */
SIDEF siString si_substrLen(siString string, isize start, isize length);


/* Searches for the specified substring in the string (from the beginning). If
 * found, the value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFind(siString string, siString subStr);
/* Searches for the specified byte in the string (from the beginning). If found,
 * the value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFindByte(siString string, u8 byte);
/* TODO */
SIDEF isize si_stringFindRune(siString string, siRune rune);

/* Searches for the specified substring in the string (from the end). If found,
 * the value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFindLast(siString string, siString subStr);
/* Searches for the specified byte in the string (from the end). If found, the
 * value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFindLastByte(siString string, u8 byte);
/* TODO */
SIDEF isize si_stringFindLastRune(siString string, siRune rune);

/* Returns the total occurences of the specified substring in the main string. */
SIDEF isize si_stringFindCount(siString string, siString subStr);


/* Compares two strings and returns true if they're equal. */
SIDEF b32 si_stringEqual(siString lhs, siString rhs);
/* Compares two strings. Returns either: A) '0' if both strings are equal;
 * B) '-1' if the largest differentiating byte is 'lhs' string's; C) '1' otherwise. */
SIDEF i32 si_stringCompare(siString lhs, siString rhs);

/* Joins all specified strings into one and writes it into an allocated string. */
SIDEF siString si_stringJoin(siBuffer(siString) strings, siString separator,
		siAllocator alloc);

/* Cuts every front and back occurences of the provided cut set in the string. */
SIDEF siString si_stringTrim(siString string, siString cutSet);
/* Cuts every front occurences of the provided cut set in the string. */
SIDEF siString si_stringTrimLeft(siString string, siString cutSet);
/* Cuts back occurences of the provided cut set in the string. */
SIDEF siString si_stringTrimRight(siString string, siString cutSet);
/* Removes any leading or trailing spaces and newlines in the string. */
SIDEF siString si_stringStrip(siString string);


/* TODO */
SIDEF siBuilder si_builderMake(isize capacity, siAllocator alloc);
/* TODO */
SIDEF siBuilder si_builderMakeLen(isize len, isize capacity, siAllocator alloc);
/* TODO */
SIDEF siBuilder si_builderMakeNone(siAllocator alloc);

/* TODO */
SIDEF char* si_builderToCstr(siBuilder b);
/* TODO */
SIDEF siString si_builderToStr(siBuilder b);

/* TODO */
SIDEF b32 si_builderWriteByte(siBuilder* b, u8 byte);
/* TODO */
SIDEF b32 si_builderWriteBytes(siBuilder* b, const void* bytes, isize len);
/* TODO */
SIDEF b32 si_builderWriteStr(siBuilder* b, siString str);
/* TODO */
SIDEF b32 si_builderWriteRune(siBuilder* b, siRune character);

/* TODO */
SIDEF b32 si_builderWriteStrQuoted(siBuilder* b, siString str);
/* TODO */
SIDEF b32 si_builderWriteStrQuotedEx(siBuilder* b, siString str, u8 quote);
/* TODO */
SIDEF b32 si_builderWriteStrQuotedRune(siBuilder* b, siString str, siRune quoteStart,
	siRune quoteEnd);

/* TODO */
SIDEF b32 si_builderWriteInt(siBuilder* b, i64 num);
/* TODO */
SIDEF b32 si_builderWriteIntEx(siBuilder* b, i64 num, i32 base);
/* TODO */
SIDEF b32 si_builderWriteUInt(siBuilder* b, u64 num);
/* TODO */
SIDEF b32 si_builderWriteUIntEx(siBuilder* b, u64 num, i32 base);
/* TODO */
SIDEF b32 si_builderWriteFloat(siBuilder* b, f64 num);
/* TODO */
SIDEF b32 si_builderWriteFloatEx(siBuilder* b, f64 num, i32 base, i32 afterPoint);

/* TODO */
SIDEF void si_builderClear(siBuilder* b);
/* TODO */
SIDEF void si_builderPopByte(siBuilder* b);
/* TODO */
SIDEF void si_builderPopRune(siBuilder* b);

/* TODO */
SIDEF b32 si_builderMakeSpaceFor(siBuilder* b, isize addLen);


/* TODO */
SIDEF siString si_stringInsert(siString string, siString subStr, isize index,
		siAllocator alloc);
/* TODO */
SIDEF siString si_stringRemoveAll(siString string, siString subStr, siAllocator alloc);
/* TODO */
siString si_stringRemove(siString string, siString subStr, i32 amount, siAllocator alloc);
/* TODO */
SIDEF siString si_stringReplaceAll(siString string, siString strOld, siString strNew,
		siAllocator alloc);
/* TODO */
SIDEF siString si_stringReplace(siString string, siString strOld, siString strNew,
		isize amount, siAllocator alloc);
/* Splits the string into multiple _substrings_ based on the separator. Returns
 * nil if the specified delimiter wasn't found in the string.
 *
 * NOTE: The elements of the array are all substrings, not newly allocated strings.
 * This means that if the original string gets freed, the elements would then
 * point to a freed memory region. */
SIDEF siBuffer(siString) si_stringSplit(siString string, siString delimiter,
	siAllocator alloc);
/* Splits the string into a specified amount _substrings_ based on the separator.
 * Returns nil if the specified delimiter wasn't found in the string.
 *
 * NOTE: The elements of the array are all substrings, not newly allocated strings.
 * This means that if the original string gets freed, the elements would then
 * point to a freed memory region. */
SIDEF siBuffer(siString) si_stringSplitEx(siString string, siString delimiter,
	isize amount, siAllocator alloc);
/* Reverses the string. */
SIDEF siString si_stringReverse(siString string, siAllocator alloc);


#ifndef SI_NO_MEMORY

/* Copies a string into the specified pointer (see 'si_memcopy' for more detail). */
SIDEF isize si_memcopyStr(void* dst, siString src);
/* Moves a string into the specified pointer (see 'si_memmove' for more detail). */
SIDEF isize si_memmoveStr(void* dst, siString src);
/* Compares a string with the specified pointer (see 'si_memcompare' for more detail). */
SIDEF i32 si_memcompareStr(const void* dst, siString src);

/* Copies a string into the specified pointer (see 'si_memcopy_s' for more detail). */
SIDEF isize si_memcopyStr_s(siBufferAny dst, siString src);
/* Moves a string into the specified pointer (see 'si_memmove_s' for more detail). */
SIDEF isize si_memmoveStr_s(siBufferAny dst, siString src);

#endif

/* An integer to character lookup table (eg. SI_NUM_TO_CHAR_TABLE[0] would return
 * '0', wheare as an index of 10 would return 'A'/'a'). */
extern const u8* SI_NUM_TO_CHAR_TABLE;



/* Converts the string's letters to uppercase. */
SIDEF siString si_stringUpper(siString string, siAllocator alloc);
/* Converts the string's letters to lowercase. */
SIDEF siString si_stringLower(siString string, siAllocator alloc);
/* Capitalizes each string's word. */
SIDEF void si_stringTitle(siString string);
/* Capitalizes the first word, while every other word is decapitalized. */
SIDEF void si_stringCapitalize(siString string);

/* Sets the lookup table state to use upper/lower characters when converting an
 * integer to a string. If 'upper' is set to true, converting '15' to base 16
 * would result in "FF", otherwise "ff". By default this is set to true. */
SIDEF void si_numChangeTable(b32 upper);

/* Converts a string into a base 10 unsigned integer. The string can only contain
 * up to 20 characters. No error reporting is done at all. */
SIDEF u64 si_stringToUInt(siString string);
/* Converts a string into a base 10 unsigned integer. An error code is written to
 * the specified pointer (check 'si_stringToUIntBase' for the error codes). */
SIDEF u64 si_stringToUIntEx(siString string, i32* outRes);

/* Converts a string into a base 10 unsigned integer. Setting a '-1' base tells
 * the function to automatically find the base from the string's prefixes (eg. '0b').
 * An error code is written to the specified pointer. The possible error codes
 * are:
 * '-2' - the string length is bigger than 20 characters,
 * '-1' - the string length is zero,
 * '<0...len - 1>' - an invalid base character was encountered at that index (eg.
 * finding an alphabet character in a base 10 string).
 * 'INT32_MAX' - no errors were encountered. */
SIDEF u64 si_stringToUIntBase(siString string, i32 base, i32* outRes);

/* Converts a string into a base 10 signed integer. The string can only contain
 * up to 20 characters. No error reporting is done at all. */
SIDEF i64 si_stringToInt(siString string);
/* Converts a string into a base 10 signed integer. An error code is written to
 * the specified pointer (check 'si_stringToUIntBase' for the error codes). */
SIDEF i64 si_stringToIntEx(siString string, i32* outRes);
/* Converts a string into a base 10 unsigned integer. Setting a '-1' base tells
 * the function to automatically find the base from the string's prefixes (eg. '0b').
 * An error code is written to the specified pointer (check 'si_stringToUIntBase'
 * for the error codes). */
SIDEF i64 si_stringToIntBase(siString string, i32 base, i32* outRes);


/* TODO */
SIDEF siString si_stringFromUInt(u64 num, siBuffer(u8) out);
/* TODO */
SIDEF siString si_stringFromUIntEx(u64 num, i32 base, siBuffer(u8) out);
/* TODO */
SIDEF siString si_stringFromInt(i64 num, siBuffer(u8) out);
/* TODO */
SIDEF siString si_stringFromIntEx(i64 num, i32 base, siBuffer(u8) out);


/* Allocates a string from a float with 6 digits after the decimal point in base 10. */
SIDEF siString si_stringFromFloat(f64 num, siBuffer(u8) out);
/* Allocates a string from a float with the given amount of digits after the
 * decimal point in the specified base. */
SIDEF siString si_stringFromFloatEx(f64 num, i32 base, i32 afterPoint, siBuffer(u8) out);

/* Returns a 'true'/'false' string from a boolean. */
SIDEF siString si_stringFromBool(b32 boolean);
/* Returns either 'true', 'false' or UINT32_MAX from a string. 'UINT32_MAX'
 * is returned if the given string cannot be converted into a boolean. */
SIDEF b32 si_stringToBool(siString string);

/* Checks if the string has the specified prefix. */
SIDEF b32 si_stringHasPrefix(siString string, siString prefix);
/* Returns the amount of matching front characters between the specified strings. */
SIDEF isize si_stringPrefixLen(siString string, siString prefix);

/* Checks if the string has the specified suffix. */
SIDEF b32 si_stringHasSuffix(siString string, siString suffix);
/* Returns the amount of matching back characters between the specified strings. */
SIDEF isize si_stringSuffixLen(siString string, siString suffix);

/* Allocates a formatted string that represents the contents of an array based
 * on the 'fmt' string. For example, if 'fmt = "<%x, %hhu, %#b>' and
 * 'array = (u8){255, 64, 4}, the string would become '{<0xFF, 64, 0b100>}'. */
SIDEF siString si_stringFromBuffer(siBufferAny buffer, cstring fmt, siBuffer(u8) out);



#if SI_SYSTEM_IS_WINDOWS
	/* UTF-16 encoded character. */
	typedef u16 siOsChar;
#else
	/* UTF-8 encoded character. */
	typedef char siOsChar;
#endif

/* TODO */
typedef siOsChar* siOsString;
/* TODO */
typedef struct siOsString_2x { siOsString v[2]; isize len[2]; } siOsString_2x;


/* TODO */
SIDEF siOsString si_stringToOsStr(siString str, siBuffer(siOsChar) out);
/* TODO */
SIDEF siOsString si_stringToOsStrEx(siString str, siBuffer(siOsChar) out, isize* copied);


#if 1

#define __for_eachStrImpl1(charName, indexName, string, __line) __for_eachStrImpl2(charName, indexName, string, __line)
#define __for_eachStrImpl2(charName, indexName, string, __line) \
	SI_ASSERT_NOT_NIL(string.data); \
	siUtf32Char si__tmp##__line; \
	for (isize indexName = 0; si__forEachStr(string.data, &charName, &si__tmp##__line, indexName) < (string).len; indexName += (si__tmp##__line).len)

#define __for_eachRevStrImpl1(charName, indexName, string, __line) __for_eachRevStrImpl2(charName, indexName, string, __line)
#define __for_eachRevStrImpl2(charName, indexName, string, __line) \
	SI_ASSERT_NOT_NIL(string.data); \
	siUtf32Char si__tmp##__line; \
	si__forEachRevStr(string.data, &charName, &si__tmp##__line, string.len - 1); \
	for (isize indexName = string.len - 1 - (si__tmp##__line).len; indexName >= 0; indexName -= si__forEachRevStr(string.data, &charName, &si__tmp##__line, indexName))

#endif

#endif /* SI_NO_STRING */

#ifndef SI_NO_UNICODE
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	============================
	| UTF-8, UTF-16 and UTF-32 |
	============================
*/


/* Denotes that the string is UTF-8 encoded. */
typedef siString siUtf8String;
/* Denotes that the string is UTF-16 encoded. */
typedef siBuffer(u16) siUtf16String;


/* A UTF-32 encoded '�' character for reporting invalid states. */
#define SI_UNICODE_INVALID_UTF32 (siUtf32Char){0xFFFD, 3}
/* A UTF-8 encoded '�' character for reporting invalid states. */
#define SI_UNICODE_INVALID_UTF8 (siUtf8Char){{0xEF, 0xBF, 0xBD}, 3}


typedef struct siUtf8Char {
	/* The codepoint (UTF-8 encoded). */
	u8 codepoint[4];
	/* Length of 'codepoint' in bytes. */
	i32 len;
} siUtf8Char;

typedef struct siUtf32Char {
	/* The codepoint (UTF-32 encoded). */
	siRune codepoint;
	/* Length of the character in UTF-8. */
	i32 len;
} siUtf32Char;


/* Decodes the given UTF-8 character into UTF-32 and returns a 'siUtf32Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf32Char){SI_UNICODE_INVALID, 0}. */
SIDEF siUtf32Char si_utf8Decode(const u8* character);
/* Encodes the specified UTF-32 character into UTF-8 and returns a 'siUtf8Char'
 * structure. This function does not check if the UTF-32 is invalid. */
SIDEF siUtf8Char si_utf8Encode(siRune codepoint);


/* Encodes a UTF-8 string into a UTF-16 string _without_ a NULL-terminator. */
SIDEF siUtf16String si_utf8ToUtf16Str(siUtf8String str, siBuffer(u16) out);
/* Encodes a UTF-8 string into a UTF-16 string with an option to include a NULL-terminated
 * byte.
 * NOTE: If a NULL-terminated byte is requested, the returned string will have a
 * capacity of 'length + 1'. _Use 'string.capacity' to advance the buffer's offset,
 * not the length._ */
SIDEF siUtf16String si_utf8ToUtf16StrEx(siUtf8String str, b32 nullTerm, siBuffer(u16) out);

/* Encodes a UTF-16 string into a UTF-8 string _without_ a NULL-terminator. */
SIDEF siUtf8String si_utf16ToUtf8Str(siUtf16String str, siBuffer(u8) out);
/* Encodes a UTF-16 string into a UTF-8 string with an option to include a NULL-terminated
 * byte.
 * NOTE: If a NULL-terminated byte is requested, the returned string will have a
 * capacity of 'length + 1'. _Use 'string.capacity' to advance the buffer's offset,
 * not the length._ */
SIDEF siUtf8String si_utf16ToUtf8StrEx(siUtf16String str, b32 nullTerm, siBuffer(u8) out);
/* Returns how many bytes a UTF-16 string would occupy in UTF-8. */
SIDEF isize si_utf16ToUtf8StrLen(siUtf16String string);


/* Decodes the given UTF-16 character into UTF-32 and returns a 'siUtf32Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf32Char){SI_UNICODE_INVALID, 0}. */
SIDEF siUtf32Char si_utf16Decode(const u16 character[2]);
/* Decodes the given UTF-16 character into UTF-8 and returns a 'siUtf8Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf8Char){SI_UNICODE_INVALID, 0}. */
SIDEF siUtf8Char si_utf16Encode(const u16 character[2]);


#if 1

SIDEF siUtf32Char si__stringLastRune(siString str);
SIDEF isize si__forEachStr(const u8* data, siRune* rune, siUtf32Char* tmp, isize i);
SIDEF isize si__forEachRevStr(const u8* data, siRune* rune, siUtf32Char* tmp, isize i);

#endif

#endif /* SI_NO_UNICODE */

#ifndef SI_NO_CHAR
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siRune/siChar        |
	========================
*/

/* Maximum amount of allowed ASCII characters. */
#define SI_ASCII_MAX 0x7F
/* Maximum amount of allowed UTF-32 characters. */
#define SI_RUNE_MAX 0x00010FFF

/* Returns the lowercased version of the given rune. */
SIDEF siRune si_runeLower(siRune rune);
/* Returns the uppercased version of the given rune. */
SIDEF siRune si_runeUpper(siRune rune);

/* Checks if the specified rune is lowercased. */
SIDEF b32 si_runeIsLower(siRune rune);
/* Checks if the specified rune is uppercased. */
SIDEF b32 si_runeIsUpper(siRune rune);
/* Returns true if the given rune is a space. */
SIDEF b32 si_runeIsSpace(siRune rune);
/* Returns true if the given rune is from '0' to '9'. */
SIDEF b32 si_runeIsDigit(siRune rune);
/* Returns true if the given rune is a hex digit (between '0'..'9'; 'a'...'f'; 'A'...'F'). */
SIDEF b32 si_runeIsHex(siRune rune);
/* Returns true if the given rune is a letter. */
SIDEF b32 si_runeIsAlpha(siRune rune);
/* Returns true if the given rune is a letter or a number. */
SIDEF b32 si_runeIsAlphanumeric(siRune rune);
/* Checks if the specified rune is a punctuation character. */
SIDEF b32 si_runeIsPunctuation(siRune rune);
/* Checks if the specified rune is a control character. */
SIDEF b32 si_runeIsControl(siRune rune);
/* Checks if the specified rune is printable. */
SIDEF b32 si_runeIsPrintable(siRune rune);
/* Checks if the specified rune is graphical. */
SIDEF b32 si_runeIsGraphical(siRune rune);
/* Checks if the specified rune is NOT a letter, '@', '#' or '$'. */
SIDEF b32 si_runeIsDelimiter(siRune rune);

/* Converts a digit to an actual integer ('3' -> 3). Returns -1 otherwise. */
SIDEF i32 si_runeDigitToInt(siRune rune);
/* Converts a hex digit into an actual integer ('F' -> 15). Returns -1 otherwise. */
SIDEF i32 si_runeHexToInt(siRune rune);



/* Returns the lowercased version of the given character. */
SIDEF char si_charLower(char c);
/* Returns the uppercased version of the given character. */
SIDEF char si_charUpper(char c);

/* Checks if the specified character is lowercased. */
SIDEF b32 si_charIsLower(char c);
/* Checks if the specified character is uppercased. */
SIDEF b32 si_charIsUpper(char c);
/* Returns true if the given character is a space. */
SIDEF b32 si_charIsSpace(char c);
/* Returns true if the given character is from '0' to '9'. */
SIDEF b32 si_charIsDigit(char c);
/* Returns true if the given character is a hex digit (between '0'..'9'; 'a'...'f'; 'A'...'F'). */
SIDEF b32 si_charIsHex(char c);
/* Returns true if the given character is in the ASCII alphabet. */
SIDEF b32 si_charIsAlpha(char c);
/* Returns true if the given character is in the ASCII alphabet OR is a number. */
SIDEF b32 si_charIsAlphanumeric(char c);
/* Checks if the specified character is a punctuation character. */
SIDEF b32 si_charIsPunctuation(char c);
/* Checks if the specified character is a control character. */
SIDEF b32 si_charIsControl(char c);
/* Checks if the specified character is printable. */
SIDEF b32 si_charIsPrintable(char c);
/* Checks if the specified character is graphical. */
SIDEF b32 si_charIsGraphical(char c);
/* Checks if the specified character is NOT alphanumeric as well as not an '@',
 * '#' or '$' character. */
SIDEF b32 si_charIsDelimiter(char c);

/* Converts '0'...'9' to an actual integer ('3' -> 3). */
SIDEF i32 si_charDigitToInt(char c);
/* Converts a hex digit into an actual integer ('F' -> 15). */
SIDEF i32 si_charHexToInt(char c);


#endif /* SI_NO_CHAR */

#ifndef SI_NO_HASHTABLE
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siHashTable          |
	========================
*/


typedef struct siHashEntry {
	/* Key of the value. */
	u64 key;
	/* Pointer to the value. */
	void* value;
	/* Points to the next valid item. */
	struct siHashEntry* next;
} siHashEntry;

typedef siArray(siHashEntry) siHashTable;

/* type - TYPE
 * Denotes that this is a 'siHashTable' variable. */
#define siHt(type) siHashTable



/* Creates a hash table using the given names and data. */
SIDEF siHashTable si_hashtableMake(const void** keyArray, isize keyLen, void** dataArray,
		isize len, siAllocator alloc);
/* Reserves a 'capacity' amount of items for the hash table. */
SIDEF siHashTable si_hashtableMakeReserve(isize capacity, siAllocator alloc);


/* Returns the key entry's value pointer from the hash table. If not found, nil
 * is returned. */
SIDEF void* si_hashtableGet(siHashTable ht, const void* key, isize keyLen);
/* Returns the key entry's value pointer from the hash table. If not found, nil
 * is returned. */
SIDEF void* si_hashtableGetWithHash(siHashTable ht, u64 hash);
/* ht - siHashTable* | key - cstring | type - TYPE
 * Returns the key entry's value as the specified type. If the item does not exist,
 * this will cause a crash, as the return of 'si_hashtableGet' will be nil. */
#define si_hashtableGetItem(ht, key, type) (*((type*)si_hashtableGet(ht, key)))


/* Adds a 'key' entry to the hash table and returns the entry pointer, regardless
 * if it's a new or pre-existing one. 'outSuccess' is set to true if the hash
 * didn't exist before the set, otherwise it's set to 'false' and the entry's
 * value's gets overwritten either way (unless disabled via a macro). 'outSuccess'
 * can be nullable.*/
SIDEF siHashEntry* si_hashtableSet(siHashTable* ht, const void* key, isize keyLen,
		void* valuePtr, b32* outSuccess);
SIDEF siHashEntry* si_hashtableSetWithHash(siHashTable* ht, u64 hash, void* valuePtr,
		b32* outSuccess);

#endif /* SI_NO_HASHTABLE */

#ifndef SI_NO_SYSTEM
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siSystem             |
	========================
*/

#ifndef SIDEF_WINDOWS
	#if SI_SYSTEM_IS_WINDOWS
		#define SIDEF_WINDOWS
	#else
		#define SIDEF_WINDOWS siNoreturn
	#endif
#endif

#ifndef SIDEF_UNIX
	#if SI_SYSTEM_IS_UNIX
		#define SIDEF_UNIX
	#else
		#define SIDEF_UNIX siNoreturn
	#endif
#endif

SI_ENUM(i32, siErrorSystem) {
	/* No error. */
	siErrorSystem_None = 0,

	/* Invalid value. */
	siErrorSystem_Invalid,
	/* Invalid file name. */
	siErrorSystem_InvalidFilename,
	/* File/directory already exists. */
	siErrorSystem_Exists,
	/* File/directory doesn't exist. */
	siErrorSystem_NotExists,
	/* Insufficient permissions. */
	siErrorSystem_Permission,
	/* Not enough memory in the memory card. */
	siErrorSystem_NoMemory,
	/* Failed to truncate the file. */
	siErrorSystem_TruncationFail,
	/* System has insufficient resources to complete the action. */
	siErrorSystem_Unavailable,
	/* A thread deadlock occurred. */
	siErrorSystem_Deadlock,

	/* An unspecified error occurred. */
	siErrorSystem_Generic,
	/* Total amount of valid errors. */
	siErrorSystem_Length,

	/* Sili reserves its errors to span from 0 to (INT32_MAX / 2). Every other
	 * value won't be touched by sili. */
	SI_ERROR_SYSTEM_END = (INT32_MAX / 2),
};

SI_ENUM(i32, siWindowsVersion) {
	siWindowsVersion_XP = 1,
	siWindowsVersion_Vista,
	siWindowsVersion_7,
	siWindowsVersion_8,
	siWindowsVersion_8_1,
	siWindowsVersion_10,
	siWindowsVersion_11,
};

SI_ENUM(i32, siUnixDE) {
	siUnixDE_KDE = 1,
	siUnixDE_GNOME,
	siUnixDE_Xfce,
};


/* TODO */
#define SI_ERROR_SYS() SI_ERROR_SYS_EX(si_systemGetError())
/* TODO */
#define SI_ERROR_SYS_EX(code) SI_ERROR_EX(code, si_systemErrorLog, nil);
/* TODO */
#define SI_ERROR_SYS_CHECK(condition, .../*action*/) \
	SI_ERROR_CHECK_EX(condition, si_systemGetError(), si_systemErrorLog, nil, __VA_ARGS__)
/* TODO */
#define SI_ERROR_SYS_CHECK_RET(condition) SI_ERROR_CHECK_EX_RET(condition, si_systemGetError(), si_systemErrorLog, nil)
/* TODO */
#define SI_OPTION_SYS_CHECK(condition, type) SI_OPTION_CHECK_EX(condition, si_systemGetError(), si_systemErrorLog, nil, type)


/* TODO */
SIDEF siErrorSystem si_systemGetError(void);

/* Returns the name of a system error. */
SIDEF siString si_systemErrorName(siErrorSystem err);
/* Returns the description of a system error. */
SIDEF siString si_systemErrorDesc(siErrorSystem err);


/* Ends the program with a to be returned code. */
siNoreturn SIDEF void si_exit(i32 code);


/* Sets an environment variable for the current process. */
SIDEF b32 si_envVarSet(siString name, siString value);
/* Unsets an environment variable for the current process. */
SIDEF b32 si_envVarUnset(siString name);

/* Gets the data length of the specified environment variable. */
SIDEF isize si_envVarGetLength(siString name);
/* Gets an environment variable for the current process. If the specified variable
 * name doesn't exist, the returned string's data is set to nil. */
SIDEF siString si_envVarGetData(siString name, siBuffer(u8) out);


/* Returns the current running Windows OS version. Returns '0' if the version is
 * older than XP. */
SIDEF SIDEF_WINDOWS siWindowsVersion si_windowsGetVersion(void);

/* Returns true if the current Unix system is running Wayland. */
SIDEF SIDEF_UNIX b32 si_unixIsWayland(void);
/* Returns true if the current Unix system is running X11. */
SIDEF SIDEF_UNIX b32 si_unixIsX11(void);

/* Returns the current running desktop environment on the system. If the found
 * desktop environment isn't recognized by Sili, 0 is returned. */
SIDEF SIDEF_UNIX siUnixDE si_unixGetDE(void);

/* TODO */
SIDEF SI_ERROR_PROC(si_systemErrorLog);

#endif /* SI_NO_SYSTEM */

#ifndef SI_NO_IO
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	|  Input and Output    |
	========================
*/

SI_ENUM(u32, siFileMode) {
	siFileMode_Read = SI_BIT(0),
	siFileMode_Write = SI_BIT(1),
	siFileMode_Append = SI_BIT(2),
	siFileMode_Plus = SI_BIT(3),

	siFileMode_All = siFileMode_Read | siFileMode_Write | siFileMode_Append | siFileMode_Plus
};

typedef struct siFile {
	/* */
	siError error;

	/* OS handle of the file. */
	isize handle;

	/* Size of the read file. */
	isize size;
} siFile;

SI_ENUM(i32, siSeekWhere) {
	siSeekWhere_Begin = 0, /* Sets the pointer from the beginning of the file. */
	siSeekWhere_Current = 1, /* Sets the pointer from the current offset. */
	siSeekWhere_End = 2 /* Sets the pointer from the EOF offset. */
};

SI_ENUM(i32, siStdFile) {
	siStdFile_Input,
	siStdFile_Output,
	siStdFile_Error,

	siStdFile_Count
};

/* Returns the standard input file. */
#define si_stdin  si_fileGetStdFile(siStdFile_Input)
/* Returns the standard output file. */
#define si_stdout si_fileGetStdFile(siStdFile_Output)
/* Returns the standard error file. */
#define si_stderr si_fileGetStdFile(siStdFile_Error)


#if SI_SYSTEM_IS_WINDOWS
	const u8 SI_PATH_SEPARATOR = '\\';
#else
	const u8 SI_PATH_SEPARATOR = '/';
#endif

/*
	========================
	|  siPath              |
	========================
*/


/* Checks if the specified path exists. */
SIDEF b32 si_pathExists(siString path);
SIDEF b32 si_pathExistsOS(siOsString path);
/* Copies the specified source path to the destination. Returns the size of the
 * file if the copy was succesful. */
SIDEF siResult(isize) si_pathCopy(siString pathSrc, siString pathDst);
/* Copies every file and folder from the specified source to the given destination.
 * Returns the amount of files copied, otherwise an error if the function failed. */
SIDEF siResult(i32) si_pathItemsCopy(siString pathSrc, siString pathDst);
/* Moves the specified source path to the given detination. Returns an error if failed. */
SIDEF siError si_pathMove(siString pathSrc, siString pathDst);
/* Renames the specified path to the given new path. Returns an error if failed. */
SIDEF siError si_pathRename(siString path, siString newPath);
/* Creates a new folder at the specified path. Returns an error if failed. */
SIDEF siError si_pathCreateFolder(siString path);
/* Removes the specified path, including the files and folders within in. Returns
 * an error if failed. */
SIDEF siError si_pathRemove(siString path);
/* Creates a hard link of the specified path. Returns an error if failed. */
SIDEF siError si_pathCreateHardLink(siString path, siString linkPath);
/* Creates a soft link of the specified path. Returns an error if failed. */
SIDEF siError si_pathCreateSoftLink(siString path, siString linkPath);

/* Returns a string view of the specified path's base name. */
SIDEF siString si_pathBaseName(siString path);
/* Returns a string view of the specified path's extension. */
SIDEF siString si_pathExtension(siString path);
/* Returns a string view of the specified path's unrooted path. */
SIDEF siString si_pathUnrooted(siString path);
/* Finds the full, rooted path of the specified path and creates a string from
 * it. Returns an error if finding the full path failed. */
SIDEF siResult(siString) si_pathGetFullName(siString path, siAllocator alloc);

/* Returns a static string of the OS's default temporary path. */
SIDEF siString si_pathGetTmp(void);

/* Returns the specified path's last write time in UNIX time. Zero is returned
 * if the time couldn't be found. */
SIDEF u64 si_pathLastWriteTime(siString path);

/* Writes the contents of the specified path to the out parameter. Returns an
 * error, if failed. */
SIDEF siError si_pathReadContentsBuf(siString path, siBuffer(u8)* out);

/* Checks if the specified path is absolute. */
SIDEF b32 si_pathIsAbsolute(siString path);
/* Checks if the specified path is relative. */
SIDEF b32 si_pathIsRelative(siString path);

/* TODO */
SIDEF isize si_pathToOS(siString path, siOsChar* out, isize capacity);
/* TODO */
SIDEF siOsString_2x si_pathToOSMul(siString first, siString second, siOsChar* out,
		isize capacity);



/*
	========================
	|  siFile              |
	========================
*/

/* Gets the pointer to the specified standard file (stdin, stdout or stderr). */
SIDEF siFile* si_fileGetStdFile(siStdFile type);

/* Creates a file. */
SIDEF siFile si_fileCreate(siString path);
/* Opens a file that exists. */
SIDEF siFile si_fileOpen(siString path);
/* The file is opened via a specified mode. */
SIDEF siFile si_fileOpenMode(siString path, siFileMode mode);

/* Returns the latest size of the file. */
SIDEF isize si_fileSize(siFile file);
/* Updates the size of the file in the structure. */
SIDEF void si_fileSizeUpdate(siFile* file);

/* Allocates the specified amount of bytes, reads said amount (if possible) from
 * the current file offset and writes it to the buffer before returning it. */
SIDEF siResult(siBuffer(u8)) si_fileRead(siFile file, isize len, siAllocator alloc);
SIDEF siError si_fileReadBuf(siFile file, isize len, siBuffer(u8)* out);
/* Allocates the specified amount of bytes, reads said amount (if possible) from
 * the specified offset and writes it to the buffer before returning it. */
SIDEF siResult(siBuffer(u8)) si_fileReadAt(siFile file, isize offset, isize len,
		siAllocator alloc);
SIDEF siError si_fileReadAtBuf(siFile file, isize offset, isize len, siBuffer(u8)* out);

/* Allocates the specified amount of bytes, reads said amount (if possible) from
 * the specified offset and writes it to the buffer before returning it. The
 * function doesn't check for if the buffer has enough capacity.*/
SIDEF siError si_fileReadUnsafe(siFile file, isize offset, isize len, siBuffer(u8)* out);

/* Allocates 'file.size' bytes, reads said amount (if possible) from the file's
 * beginning and writes it to the buffer before returning it. _File seek offset
 * does not get changed when calling the function._ */
SIDEF siString si_fileReadContents(siFile file, siAllocator alloc);
SIDEF siError si_fileReadContentsBuf(siFile file, siBuffer(u8)* out);

/* Allocates 'file.size' bytes, reads said amount (if possible) from the file's
 * beginning and then splits the string into an array of string view lines.
 * _File seek offset does not get changed when calling the function._ */
SIDEF siBuffer(siString) si_fileReadlines(siFile file, siAllocator alloc);


/* TODO */
SIDEF isize si_fileWrite(siFile* file, siBuffer(u8) data);
/* TODO */
SIDEF isize si_fileWriteAt(siFile* file, siBuffer(u8) data, isize offset);
/* TODO */
SIDEF isize si_fileWriteByte(siFile* file, u8 byte);
/* TODO */
SIDEF isize si_fileWritePtr(siFile* file, const void* ptr, isize len);
/* TODO */
SIDEF isize si_fileWriteStr(siFile* file, siString str);


/* Returns the current offset of the file stream. */
SIDEF isize si_fileTell(siFile file);
/* Seeks the file stream offset to the specified offset using the given method.
 * Returns the amount of bytes that were read. */
SIDEF isize si_fileSeek(siFile file, isize offset, siSeekWhere method);
/* Seeks to the front of the file. Returns 'true' if the operation went through. */
SIDEF b32 si_fileSeekFront(siFile file);
/* Seeks to the back of the file. Returns 'true' if the operation went through. */
SIDEF b32 si_fileSeekBack(siFile file);

/* Truncates the file to the specified size and returns 'true' if it succeded. */
SIDEF b32 si_fileTruncate(siFile* file, isize size);
/* Returns the last time the file was written. */
SIDEF u64 si_fileLastWriteTime(siFile file);

/* Closes the file. */
SIDEF void si_fileClose(siFile* file);

/*
	========================
	|  siDirectory         |
	========================
*/
SI_ENUM(i32, siIoType) {
	/* === Cross-platform === */
	siIoType_File = 1,
	siIoType_Directory,
	siIoType_Link,

	/* === Unix only === */
	siIoType_Socket,
	siIoType_Device,
	siIoType_Block,
	siIoType_Fifo,
};

typedef struct siDirectoryEntry {
	siString path;
	siIoType type;
} siDirectoryEntry;

typedef struct siDirectory {
	siError error;
	void* handle;
	isize directoryLen;
	u8 buffer[SI_PATH_MAX];
} siDirectory;


/* Opens a directory and creates a directory stream. */
SIDEF siDirectory si_directoryOpen(siString path);

/* Polls the next file, folder or link inside the directory. If one is found,
 * information about it will be written into the specified out pointer and 'true'
 * is returned. If there is nothing else left to inquiry, the function automatically
 * closes the stream and returns false.
 *
 * NOTE 1: If an error occurred, 'false' is returned, the stream is closed and an
 * error is written into 'dir->error'.
 * NOTE 2: If you decide to end the polling process early, make sure to call
 * 'si_directoryClose'. */
SIDEF b32 si_directoryPollEntry(siDirectory* dir, siDirectoryEntry* out);
/* Polls the next file, folder or link inside the directory. If one is found,
 * information about it will be written into the specified out pointer and 'true'
 * is returned. If there is nothing else left to inquiry, the function automatically
 * closes the stream and returns false. You can also specify if you want the
 * returned path to contain the base directory path as well.
 *
 * NOTE 1: If an error occurred, 'false' is returned, the stream is closed and an
 * error is written into 'dir->error'.
 * NOTE 2: If you decide to end the polling process early, make sure to call
 * 'si_directoryClose'. */
SIDEF b32 si_directoryPollEntryEx(siDirectory* dir, b32 fullPath, siDirectoryEntry* out);

/* Closes the directory stream. */
SIDEF void si_directoryClose(siDirectory* dir);


#endif /* SI_NO_IO */

#ifndef SI_NO_THREAD
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siThread             |
	========================
*/

/* name - NAME
 * Defines a valid thread function prototype.*/
#define SI_THREAD_PROC(name) void* name(void* data)
/* Represents a thread function. */
typedef SI_THREAD_PROC(siThreadFunction);


SI_ENUM(u32, siThreadState) {
	siThreadState_Closed = 0,
	siThreadState_Initialized,
	siThreadState_Running,
};

typedef struct siThread {
	#if SI_SYSTEM_IS_WINDOWS
		/* OS-specific thread ID. */
		HANDLE id;
	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE || SI_SYSTEM_EMSCRIPTEN
		/* OS-specific thread ID. */
		pthread_t id;
	#endif

	/* Pointer to the function. */
	siThreadFunction* func;
	/* The functions' argument. */
	void* arg;

	/* Dictates how much stack size gets allocated for th thread. Setting this
	 * to 0 makes the system give the default amount of stack size to the thread. */
	usize stackSize;

	/* TODO */
	volatile siThreadState state;

	/* The raw pointer return of the function. */
	void* returnValue;
} siThread;


/* thread - siThread | type - TYPE
 * Casts 'thread.returnValue' correctly to match the given type instead of it being
 * void*. */
#define si_threadGetReturn(thread, type) si_transmute(type, (thread).returnValue, void*)


/* Prepares a thread data strucutre. */
SIDEF siThread si_threadMake(siThreadFunction function, void* arg);
/* Prepares a thread data strucutre wth the option to specify the stack size.
 * By default this is set to zero.*/
SIDEF siThread si_threadMakeEx(siThreadFunction function, void* arg, usize stackSize);
/* Prepares a thread data structure, writes it to 'out' and runs the thread. */
SIDEF siError si_threadMakeAndRun(siThreadFunction function, void* arg, siThread* out);

/* Runs the thread. */
SIDEF siError si_threadRun(siThread* thread);
/* Suspends the parent thread until the specified thread terminates. */
SIDEF siError si_threadJoin(siThread* thread);

/* Destroys the thread. */
SIDEF siError si_threadDestroy(siThread* thread);

/* TODO */
SIDEF b32 si_threadPrioritySet(siThread t, i32 priority);

#endif /* SI_NO_THREAD */

#ifndef SI_NO_TIME
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siTime               |
	========================
*/

typedef struct {
	/* Years (1677-2262). */
	i32 years;
	/* Months (1-12). */
	i32 months;
	/* Days (1-31). */
	i32 days;

	/* Hours (0-23). */
	i32 hours;
	/* Minutes (0-59). */
	i32 minutes;
	/* Seconds (0-59). */
	i32 seconds;
	/* Nanoseconds (0-999999999)*/
	i32 nanoseconds;
} siTimeCalendar;


/* An array of full month names. Used for 'si_timeToString'.  */
extern siString* SI_NAMES_MONTHS_FULL;
/* An array of short month names. Used for 'si_timeToString'.  */
extern siString* SI_NAMES_MONTHS_SHRT;
/* An array of full week day names. First element is "Sunday". Used for 'si_timeToString'.  */
extern siString* SI_NAMES_DAYS_FULL;
/* An array of short week day names. First element is "Sun". Used for 'si_timeToString'.  */
extern siString* SI_NAMES_DAYS_SHRT;
/* An array of AM/PM names. First two elements are the uppercased versions, the
 * last two are lowercased. Used for 'si_timeToString'. */
extern siString* SI_NAMES_AM_PM;


/* One microsecond in nanoseconds */
#define SI_CLOCKS_MICRO (1000)
/* One milisecond in nanoseconds */
#define SI_CLOCKS_MILI  (1000000)
/* One second in nanoseconds */
#define SI_CLOCKS_SECS  (1000000000)

/* time - UINT
 * Converts Windows time into sili's Unix time. */
#define si_timeToUnix(time) (((time) - 116444736000000000) * 100)


/* Reads the current value of the processor’s time-stamp counter.
 * NOTE: Only natively works for AMD64, i386, ARM64, WASI and PPC CPUs. On other CPUs
 * the function relies on OS functions like 'gettimeofday'. */
SIDEF u64 si_RDTSC(void);
/* Reads the current value of the processor’s time-stamp counter and writes the
 * current processor ID to the pointer. */
SIDEF u64 si_RDTSCP(i32* proc);
/* Returns the current clock (in nanoseconds). */
SIDEF u64 si_clock(void);

/* Starts a timestamp locally. */
SIDEF void si_timeStampStart(void);
/* Prints the time since the start. */
SIDEF void si_timeStampPrintSince(void);
/* TODO */
SIDEF u64 si_timeStampStartEx(void);
/* TODO */
SIDEF void si_timeStampPrintSinceEx(u64 t);

/* Makes the CPU sleep for a certain amount of miliseconds. */
SIDEF void si_sleep(i32 miliseconds);

/* Returns the number of seconds since 1970-01-01 UTC-0. */
SIDEF i64 si_timeNowUTC(void);
/* Returns the number of seconds since 1970-01-01 local time.*/
SIDEF i64 si_timeNowLocal(void);

/* Checks if the specified year is a leap year. */
SIDEF b32 si_timeYearIsLeap(i32 year);
/* Returns an integer from 0 to 6 (Sunday to Saturday) that represents a week day
 * based on the specified date. */
SIDEF i32 si_timeGetDayOfWeek(i32 year, i32 month, i32 day);

/* Converts raw Unix time into human-readable time components. */
SIDEF siTimeCalendar si_timeToCalendar(i64 time);

/* Creates a string in the specified buffer from the specified format.
 * Specifiers:
 * - 'yyyy'/'yy' - full year/the two last digits
 * - 'M'/'MM' - month digit without/with padding, 'MMM'/'MMMM' - shortened/full month name,
 * - 'd'/'dd' - day digit without/with padding, 'DDD'/'DDDD' - shortened/full day name,
 * - 'h'/'hh' - hours without/with padding, 'm'/'mm' - minutes without/with padding,
 * - 's'/'ss' - seconds without/with padding, 'n'/'nn' - nanoseconds without/with padding.
 * - 'AP/ap' - uppercased/lowercased am/pm display. */
SIDEF siString si_timeToString(siTimeCalendar calendar, siString fmt, siBuffer(u8) out);

#endif /* SI_NO_TIME */

#ifndef SI_NO_BIT
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siBit                |
	========================
*/

SI_ENUM(usize, siBitType) {
	SI_BIT_ZERO,
	SI_BIT_ONE
};


/* variable - VARIABLE | flag - UINT | condition - EXPRESSION
 * TODO */
#define SI_BIT_SET(variable, flag, condition) \
	(condition) ? ((variable) |= (flag)) : ((variable) &= ~(flag))

#if SI_ENDIAN_IS_LITTLE
	/* little - EXPRESSION | big - EXPRESSION
	* Returns the specified 'little' value. */
	#define SI_ENDIAN_VALUE(little, big) little
#else
	/* little - EXPRESSION | big - EXPRESSION
	* Returns the specified 'big' value. */
	#define SI_ENDIAN_VALUE(little, big) end
#endif

/* Returns how many 1 bits are in an 8-bit number. */
i32 si_numCountBitsU8(u8 num);
/* Returns how many 1 bits are in a 32-bit number. */
i32 si_numCountBitsU32(u32 num);
/* Returns how many 1 bits are in a 64-bit number. */
i32 si_numCountBitsU64(u64 num);

#if SI_STANDARD_CHECK_MIN(C, C11)
	#define si_numCountBits(num) \
		_Generic((num), \
			u8  : si_numCountBitsU8(num), \
			u16 : si_numCountBitsU32(num), \
			u32 : si_numCountBitsU32(num), \
			u64 : si_numCountBitsU64(num), \
			default: si_numCountBitsU64(num) \
		)(X)
#endif

/* type - TYPE | num - UINT
 * Returns the amount of leading zeros in a number. */
#define si_numLeadingZeros(type, num) si_numLeadingZerosEx(num, si_sizeof(type) * 8)
/* type - TYPE | num - UINT
 * Returns the amount of leading ones in a number. */
#define si_numLeadingOnes(type, num) si_numLeadingOnesEx(num, si_sizeof(type) * 8)

/* type - TYPE | num - UINT | bitType - siBitType
 * Returns the amount of trailing bits of the specified bit type (0 or 1). */
#define si_numTrailingBit(type, num, bitType) (type)si_numTrailingBitEx(num, si_sizeof(type) * 8, bitType)
/* type - TYPE | num - UINT | bits - usize
 * Rotates the bits of the number left by 'bits' amount. */
#define si_numRotateLeft(type, num, bits) (type)si_numRotateLeftEx(num, si_sizeof(type) * 8, bits)
/* type - TYPE | num - UINT | bits - usize
 * Rotates the bits of the number right by 'bits' amount. */
#define si_numRotateRight(type, num, bits) (type)si_numRotateRightEx(num, si_sizeof(type) * 8, bits)
/* type - TYPE | num - UINT
 * Reverses the bits of the number. */
#define si_numReverseBits(type, num) (type)si_numReverseBitsEx(num, si_sizeof(type) * 8 )
/* allocator - siAllocator* | type - TYPE | num - UINT
 * Creates a 'siArray(u8)' from the specified number and writes it into the allocator. */
#define si_numToBytes(type, num, allocator) si_numToBytesEx(num, si_sizeof(type), allocator)
/* TODO */
SIDEF u64 si_numFromBytes(siBuffer(u8) bytes);


/* Swaps the endianess of a 16-bit number. */
SIDEF u16 si_swap16(u16 x);
/* Swaps the endianess of a 32-bit number. */
SIDEF u32 si_swap32(u32 x);
/* Swaps the endianess of a 64-bit number. */
SIDEF u64 si_swap64(u64 x);

/* Swaps the endianess of a 16-bit number on non-BE architectures. */
SIDEF u16 si_swap16be(u16 x);
/* Swaps the endianess of a 32-bit number on non-BE architectures. */
SIDEF u32 si_swap32be(u32 x);
/* Swaps the endianess of a 64-bit number on non-BE architectures. */
SIDEF u64 si_swap64be(u64 x);

/* Swaps the endianess of a 16-bit number on non-LE architectures. */
SIDEF u16 si_swap16le(u16 x);
/* Swaps the endianess of a 32-bit number on non-LE architectures. */
SIDEF u32 si_swap32le(u32 x);
/* Swaps the endianess of a 64-bit number on non-LE architectures. */
SIDEF u64 si_swap64le(u64 x);


/* Returns the length of a base 10 unsigned number. */
SIDEF isize si_numLen(u64 num);
/* Returns the length of a specified base unsigned number. */
SIDEF isize si_numLenEx(u64 num, i32 base);
/* Returns the length of a base 10 signed number. */
SIDEF isize si_numLenInt(i64 num);
/* Returns the length of a specified base signed number. */
SIDEF isize si_numLenIntEx(i64 num, i32 base);


/* type - TYPE | a - TYPE | b - TYPE | res - TYPE*
 * Returns true if the addition of two integers resulted in an overflow. The addition
 * result is written to the given pointer. */
#define si_checkAdd(type, a, b, res) SI_CHECK_ARITHMETIC_FUNC(type, Add, a, b, res)
/* type - TYPE | a - TYPE | b - TYPE | res - TYPE*
 * Returns true if the subtraction of two integers resulted in an overflow. The
 * subtraction result is written to the given pointer. */
#define si_checkSub(type, a, b, res) SI_CHECK_ARITHMETIC_FUNC(type, Sub, a, b, res)
/* type - TYPE | a - TYPE | b - TYPE | res - TYPE*
 * Returns true if the multiplication of two integers resulted in an overflow.
 * The mulitplication result is written to the given pointer. */
#define si_checkMul(type, a, b, res) SI_CHECK_ARITHMETIC_FUNC(type, Mul, a, b, res)



#if 1

SIDEF u32 si_numLeadingZerosEx(u64 num, u32 totalBits);
SIDEF u32 si_numLeadingOnesEx(u64 num, u32 totalBits);
SIDEF u32 si_numTrailingBitEx(u64 num, usize totalBits, siBitType bit);

u64 si_numRotateLeftEx(u64 num, usize num_sizeof, usize bits);
u64 si_numRotateRightEx(u64 num, usize num_sizeof, usize n);
u64 si_numReverseBitsEx(u64 num, usize num_sizeof);

siBuffer(u8) si_numToBytesEx(u64 num, isize num_sizeof, siAllocator alloc);


#define SI_CHECK_ARITHMETIC_FUNC(type, func, a, b, res) si__check##func##_##type(a, b, res)

#define SI_CHECK_ARITHMETIC_DEC(type, func, def, body) \
	def b32 si__check##func##_##type(type a, type b, type* res) body \

#define SI_CHECK_ARITHMETIC_DEC_ALL(func, def, body) \
	SI_CHECK_ARITHMETIC_DEC(i8,    func, SIDEF, ;) \
	SI_CHECK_ARITHMETIC_DEC(i16,   func, SIDEF, ;) \
	SI_CHECK_ARITHMETIC_DEC(i32,   func, SIDEF, ;) \
	SI_CHECK_ARITHMETIC_DEC(i64,   func, SIDEF, ;) \
	SI_CHECK_ARITHMETIC_DEC(isize, func, SIDEF, ;) \
	SI_CHECK_ARITHMETIC_DEC(u8,    func, SIDEF, ;) \
	SI_CHECK_ARITHMETIC_DEC(u16,   func, SIDEF, ;) \
	SI_CHECK_ARITHMETIC_DEC(u32,   func, SIDEF, ;) \
	SI_CHECK_ARITHMETIC_DEC(u64,   func, SIDEF, ;) \
	SI_CHECK_ARITHMETIC_DEC(usize, func, SIDEF, ;)


SI_CHECK_ARITHMETIC_DEC_ALL(Add, SIDEF, ;)
SI_CHECK_ARITHMETIC_DEC_ALL(Sub, SIDEF, ;)
SI_CHECK_ARITHMETIC_DEC_ALL(Mul, SIDEF, ;)

#undef SI_CHECK_ARITHMETIC_DEC_ALL

#endif

#endif /* SI_NO_BIT */

#ifndef SI_NO_CPU
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siCpu                |
	========================
*/

/* Executes the 'CPUID' assembly instruction on x86 family of CPUs. */
SIDEF void si_CPUID(u32 ID, u32 registers[4]);

/* Returns the CPU's clock speed in mHz. The result of the function gets cached. */
SIDEF i32 si_cpuClockSpeed(void);
/* Returns the amount of CPU processors there are. */
SIDEF i32 si_cpuProcessorCount(void);


#endif /* SI_NO_CPU */

#ifndef SI_NO_PRINT
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| si_print             |
	========================
*/

/* */
//SIDEF siString si_aprintf(cstring str, siAllocator* alloc, ...);



/* Writes a NULL-terminated C-string to the standard output. Returns the amount
 * of written bytes. */
SIDEF isize si_print(cstring str);
/* Writes a NULL-terminated C-string with a newline to the standard output. Returns
 * the amount of written bytes. */
SIDEF isize si_printLn(cstring str);
/* Writes a NULL-terminated formatted C-string to the standard output. Returns
 * the amount of written bytes. */
SIDEF isize si_printf(cstring fmt, ...);
SIDEF isize si_printfVa(cstring fmt, va_list va);
/* Writes a NULL-terminated formatted C-string with a newline to the standard
 * output. Returns the amount of written bytes. */
SIDEF isize si_printfLn(cstring fmt, ...);
SIDEF isize si_printfLnVa(cstring fmt, va_list va);


/* Writes a NULL-terminated C-string to a file. Returns the amount of written
 * bytes. */
SIDEF isize si_fprint(siFile* file, cstring str);
/* Writes a NULL-terminated C-string with a newline to a file. Returns the amount
 * of written bytes. */
SIDEF isize si_fprintLn(siFile* file, cstring str);
/* Writes a NULL-terminated formatted C-string to a file. Returns the amount of
 * written bytes. */
SIDEF isize si_fprintf(siFile* file, cstring fmt, ...);
SIDEF isize si_fprintfVa(siFile* file, cstring fmt, va_list va);
/* Writes a NULL-terminated formatted C-string with a newline to a file. Returns
 * the amount of written bytes. */
SIDEF isize si_fprintfLn(siFile* file, cstring fmt, ...);
SIDEF isize si_fprintfLnVa(siFile* file, cstring fmt, va_list va);


/* Writes a NULL-terminated formatted C-string to a buffer. */
SIDEF siString si_bprintf(siBuffer(u8) out, cstring fmt, ...);
SIDEF siString si_bprintfVa(siBuffer(u8) out, cstring fmt, va_list va);
/* Writes a NULL-terminated formatted C-string to a buffer. */
SIDEF siString si_bprintfLn(siBuffer(u8) out, cstring fmt, ...);
SIDEF siString si_bprintfLnVa(siBuffer(u8) out, cstring fmt, va_list va);


/* Writes a formatted string to a buffer. */
SIDEF siString si_bprintfStr(siBuffer(u8) out, siString fmt, ...);
SIDEF siString si_bprintfStrVa(siBuffer(u8) out, siString fmt, va_list va);


/* Terminates the program immediately with a formatted C-string message.
 * NOTE: Use the SI_PANIC() macros instead. */
siNoreturn SIDEF void si_panic(cstring conditionStr, siCallerLoc call, cstring message, ...);


SI_ENUM(u8, siPrintColorType) {
	siPrintColorType_3bit = 1,
	siPrintColorType_8bit,
	siPrintColorType_24bit,
};

SI_ENUM(u8, siPrintColor3bit) {
	siPrintColor3bit_Black,
	siPrintColor3bit_Red,
	siPrintColor3bit_Green,
	siPrintColor3bit_Yellow,
	siPrintColor3bit_Blue,
	siPrintColor3bit_Magenta,
	siPrintColor3bit_Cyan,
	siPrintColor3bit_White
};

typedef struct siPrintColor {
	siPrintColorType type;
	union_anonymous (data,
		struct { siPrintColor3bit color; b8 bold; b8 light; } ansi;
		u8 cube;
		u8 rgb[3];
	);
} siPrintColor;

/* color - siPrintColor
 * Creates a regular ANSI/3-bit print color. */
#define si_printColor3bit(color) si_printColor3bitEx(color, 0, 0)
/* color - siPrintColor | bold - b8 | light - b8
 * Creates a ANSI/3-bit print color with the options to make it bold and/or light. */
#define si_printColor3bitEx(color, bold, light) \
	(siPrintColor){siPrintColorType_3bit, {.ansi = {color, bold, light}}}

/* color - u8
 * Creates an 8-bit print color that relies on a 256-color lookup table.*/
#define si_printColor8bit(color) \
	(siPrintColor){siPrintColorType_8bit, {.cube = color}}

/* r - u8 | g - u8 | b - u8
 * Creates a 24-bit "true color" print color based on RGB. */
#define si_printColor24bit(r, g, b) \
	(siPrintColor){siPrintColorType_24bit, {.rgb[0] = r, .rgb[1] = g, .rgb[2] = b}}

/* Checks if the terminal supports displaying 24-bit colors. */
SIDEF b32 si_printHas24bitColor(void);

#endif /* !defined(SI_NO_PRINT) */

#ifndef SI_NO_DLL
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siDll                |
	========================
*/

/* A type denotating that the value is a DLL handle. */
typedef void* siDllHandle;
/* A type denotating that the value is a DLL process/function. */
typedef void* siDllProc;

/* Loads the specified DLL. */
SIDEF siDllHandle si_dllLoad(cstring path);
/* Unloads the specified DLL. */
SIDEF void si_dllUnload(siDllHandle dll);

/* Returns a pointer to the specified processor name. */
SIDEF siDllProc si_dllProcAddress(siDllHandle dll, cstring name);

/* dll - siDllHandle | function - FUNCTION |  type - TYPE
 * Loads the specified function's name as a processor and returns it as an ISO-C
 * friendly function. */
#define si_dllProcAddressFunc(dll, function, type) \
	si_transmute(type, si_dllProcAddress(dll, #function), siDllProc)

#ifdef SI_TYPEOF_USED
/* dll - siDllHandle | function - FUNCTION
 * Loads the specified function's name as a processor and returns it as an ISO-C
 * friendly function. */
	#define si_dllProcAddressFuncType(dll, function) \
		si_dllProcAddressFunc(dll, function, typeof(function))
#endif

#endif /* SI_NO_DLL */

#ifndef SI_NO_MATH
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siMath               |
	========================
*/

#define SI_TAU 6.28318530717958647692528676655900576f
#define SI_PI  3.14159265358979323846264338327950288f

#define SI_TO_RADIANS(degrees) ((degrees) * SI_PI / 180.0f)
#define SI_TO_DEGREES(radians) ((radians) * 180.0f / SI_PI)

/* Returns the smallest of its arguments: either a or b.*/
#define si_min(type, a, b) SI_MATH_FUNC(type, min, a, b)
/* Returns the smallest of its arguments: either a, b or c.*/
#define si_min3(type, a, b, c) si_min(type, si_min(type, a, b), c)

/* Returns the largest of its arguments: either a or b.*/
#define si_max(type, a, b) SI_MATH_FUNC(type, max, a, b)
/* Returns the largest of its arguments: either a, b or c.*/
#define si_max3(type, a, b, c) si_max(type, si_max(type, a, b), c)

/* Returns 'lower' if x is lower than it, 'upper' if x is upper than it or itself
 * if neither. */
#define si_clamp(type, x, lower, upper) si_min(type, upper, si_max(type, x, lower))

/* Returns the absolute value of 'x'. */
#define si_abs(type, x) SI_MATH_FUNC(type, abs, x)

/* Returns a boolean that indicates if 'x' is between 'lower' and 'upper'. */
#define si_between(type, x, lower, upper) SI_MATH_FUNC(type, between, x, lower, upper)

/* Returns the sine of an angle (in radians). */
#define si_sin(type, x) SI_MATH_FUNC(type, sin, x)
/* Returns the cosine of an angle (in radians). */
#define si_cos(type, x) SI_MATH_FUNC(type, cos, x)

/* Calculates the largest integer not greater than 'x'. */
#define si_round(type, x) SI_MATH_FUNC(type, round, x)

/* Calculates the largest integer not greater than 'x'. */
#define si_floor(type, x) SI_MATH_FUNC(type, floor, x)

/* Calculates the smallest integer not less than 'x'. */
#define si_ceil(type, x) SI_MATH_FUNC(type, ceil, x)

/* Raises an unsigned base 10 by the power of the exponent, and returns the result
 * as a 64-bit unsigned int (meaning the exponent can only be from 0 to 19, otherwise
 * the app will crash). */
u64 si_pow10(i32 exponent);
/* Raises a f64 10 by the power of the exponent. The exponent must be in-between
 * -18 to 18, otherwise the app will crash.*/
f64 si_pow10F64(i32 exponent);


/* Checks if the given 32-bit float is NaN. */
SIDEF b32 si_float32IsNan(f32 num);
/* Checks if the given 64-bit float is NaN. */
SIDEF b32 si_float64IsNan(f64 num);

#if SI_STANDARD_CHECK_MIN(C, C11)
	/* num - f32/f64
	 * Checks if the given float is NaN. */
	#define si_floatIsNan(num) \
	    _Generic((num),        \
	        f32 : si_float32IsNan, \
	        f64 : si_float64IsNan \
	    )(num)
#else
	/* num - f32/f64
	 * Checks if the given float is NaN. */
	#define si_floatIsNan(num) \
		(si_sizeof(num) == 4) ? si_float32IsNan((f32)num) : si_float64IsNan((f64)num)

#endif

/* Checks if the given 32-bit float is infinite. '0' - number isn't infinite,
 * '1' - positive infinity, '2' - negative infinity. */
SIDEF i32 si_float32IsInf(f32 num);
/* Checks if the given 64-bit float is infinite. '0' - number isn't infinite,
 * '1' - positive infinity, '2' - negative infinity. */
SIDEF i32 si_float64IsInf(f64 num);

#if SI_STANDARD_CHECK_MIN(C, C11)
	/* num - f32/f64
	 * Checks if the given 32-bit float is infinite. '0' - number isn't infinite,
	 * '1' - positive infinity, '2' - negative infinity. */
	#define si_floatIsInf(num) \
	    _Generic((num),        \
	        f32 : si_float32IsInf, \
	        f64 : si_float64IsInf \
	    )(num)
#else
	/* num - f32/f64
	 * Checks if the given 32-bit float is infinite. '0' - number isn't infinite,
	 * '1' - positive infinity, '2' - negative infinity. */
	#define si_floatIsInf(num) \
		(si_sizeof(num) == 4) ? si_float32IsInf((f32)num) : si_float64IsInf((f64)num)

#endif

#if 1
	#define SI_MATH_FUNC(type, name, ...) si__##name##_##type(__VA_ARGS__)


	#define SI_MATH_FUNC_DECLARE_1X_SIGNED(name, def, body) \
		def i8    si__##name##_i8(i8 a) body \
		def i16   si__##name##_i16(i16 a) body \
		def i32   si__##name##_i32(i32 a) body \
		def i64   si__##name##_i64(i64 a) body \
		def isize si__##name##_isize(isize a) body \
		def f32   si__##name##_f32(f32 a) body \
		def f64   si__##name##_f64(f64 a) body

	#define SI_MATH_FUNC_DECLARE_2X(name, def, body) \
		def u8    si__##name##_u8(u8 a, u8 b) body \
		def i8    si__##name##_i8(i8 a, i8 b) body \
		def u16   si__##name##_u16(u16 a, u16 b) body \
		def i16   si__##name##_i16(i16 a, i16 b) body \
		def u32   si__##name##_u32(u32 a, u32 b) body \
		def i32   si__##name##_i32(i32 a, i32 b) body \
		def u64   si__##name##_u64(u64 a, u64 b) body \
		def i64   si__##name##_i64(i64 a, i64 b) body \
		def usize si__##name##_usize(usize a, usize b) body \
		def isize si__##name##_isize(isize a, isize b) body \
		def f32   si__##name##_f32(f32 a, f32 b) body \
		def f64   si__##name##_f64(f64 a, f64 b) body

	#define SI_MATH_FUNC_DECLARE_3X_B32(name, def, body) \
		def b32 si__##name##_u8(u8 a, u8 b, u8 c) body \
		def b32 si__##name##_i8(i8 a, i8 b, i8 c) body \
		def b32 si__##name##_u16(u16 a, u16 b, u16 c) body \
		def b32 si__##name##_i16(i16 a, i16 b, i16 c) body \
		def b32 si__##name##_u32(u32 a, u32 b, u32 c) body \
		def b32 si__##name##_i32(i32 a, i32 b, i32 c) body \
		def b32 si__##name##_u64(u64 a, u64 b, u64 c) body \
		def b32 si__##name##_i64(i64 a, i64 b, i64 c) body \
		def b32 si__##name##_usize(usize a, usize b, usize c) body \
		def b32 si__##name##_isize(isize a, isize b, isize c) body \
		def b32 si__##name##_f32(f32 a, f32 b, f32 c) body \
		def b32 si__##name##_f64(f64 a, f64 b, f64 c) body

	#define SI_MATH_FUNC_DECLARE_1X_FLOAT(name, def, ...) \
		def f32   si__##name##_f32(f32 a) __VA_ARGS__ \
		def f64   si__##name##_f64(f64 a) __VA_ARGS__


	SI_MATH_FUNC_DECLARE_2X(min, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_2X(max, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_SIGNED(abs, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_3X_B32(between, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_FLOAT(floor, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_FLOAT(ceil, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_FLOAT(round, SIDEF, ;)

	SI_MATH_FUNC_DECLARE_1X_FLOAT(sin, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_FLOAT(cos, SIDEF, ;)

#endif

#endif /* SI_NO_MATH */

#ifndef SI_NO_BENCHMARK
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siBenchmark          |
	========================
*/

/* timesToLoop - usize | function - NAME
 * Runs the function 'timesToLoop' times and prints how long it took to finish. */
#define si_benchmarkRunsPerLoop(timesToLoop, function) \
	do { \
		u64 timeSince = si_RDTSC(); \
		volatile usize i; \
		for (i = 0; i < (timesToLoop); i += 1) { \
			function; \
		} \
		timeSince = si_RDTSC() - timeSince; \
		f64 timeTaken = (f64)timeSince / ((f64)si_cpuClockSpeed() / 1000.0) / (f64)SI_CLOCKS_MILI; \
		si_printf(SI_PERFORMANCE_MSG, #function, timeTaken, timesToLoop / 1000000.0); \
	} while(0)

/* timesToLoop - usize | function - NAME
 * Runs the function 'timesToLoop' for 'ms' miliseconds and prints how many times
 * the function got executed.  */
#define si_benchmarkExecutesPerMs(ms, function) \
	do { \
		u64 counter = 0; \
		u32 miliseconds = (u32)ms; \
		siThread thread; \
		si_threadMakeAndRun(si__benchmarkThread, &miliseconds, &thread); \
		while (thread.state == siThreadState_Running) { \
			function; \
			counter += 1; \
		} \
		si_printf(SI_PERFORMANCE_MSG, #function, (f64)miliseconds, (f64)counter / 1000000.0); \
	} while(0)

/* multiplesOf10 - usize | function - NAME
 * Runs the function through multiples of 10 until it reaches 'multiplesOf10'.
 * Then it prints the benchmarking data for each run, as well as the combined
 * average of all of the runs. */
#define si_benchmarkLoopsAvg(multiplesOf10, function) \
	si_benchmarkLoopsAvgRange(1, multiplesOf10, function)

/* start - usize | end - usize | function - NAME
 * Runs the function through multiples of 10 from 'start' until it reaches 'end'.
 * Then it prints the benchmarking data for each run, as well as the combined
 * average of all of the runs. */
#define si_benchmarkLoopsAvgRange(start, end, function) \
	do { \
		u64 array[20]; \
		usize len = 0; \
		si_benchmarkLoop(function, array, &len, start, end); \
		si_benchmarkLoopsAvgPrint(#function, array, len, (usize[]){start, end}); \
	} while(0)

/* multiplesOf10 - usize | function1 - NAME | function2 - NAME
 * Runs both functions through multiples of 10 until it reaches 'multiplesOf10'.
 * Then it prints the benchmarking data for each functions' runs, as well as the
 * combined average of all of the runs. All of this data then gets compared between
 * function1 vs function2 (ON UNIX: green text - faster, red text - slower). */
#define si_benchmarkLoopsAvgCmp(multiplesOf10, function1, function2) \
	si_benchmarkLoopsAvgCmpRange(1, multiplesOf10, function1, function2)

/* start - usize | end - usize | function1 - NAME | function2 - NAME
 * Runs both functions through multiples of 10 from 'start' until it reaches 'end'.
 * Then it prints the benchmarking data for each functions' runs, as well as the
 * combined average of all of the runs. All of this data then gets compared between
 * function1 vs function2 (ON UNIX: green text - faster, red text - slower). */
#define si_benchmarkLoopsAvgCmpRange(start, end, function1, function2) \
	do { \
		u64 arrays[2][20]; \
		usize len1 = 0, len2 = 0; \
		si_benchmarkLoop(function1, arrays[0], &len1, start, end); \
		si_benchmarkLoop(function2, arrays[1], &len2, start, end); \
		\
		si_benchmarkLoopsAvgCmpPrint((cstring[]){#function1, #function2}, arrays, len1, (usize[]){start, end}); \
	} while(0)

typedef struct { u64 duration; cstring unit; } siBenchmarkLimit;

#define SI_PERFORMANCE_MSG \
	"====== BENCHMARK DATA ======\n" \
	"General:\n" \
		"\tFunction - '%s'\n" \
		"\tDuration - '%f' ms\n" \
		"\tExecution count - '%f' million\n"

#define si_benchmarkLoop(function, array, lenPtr, start, end) \
	do { \
		volatile u64 medianIndex; \
		for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10) { \
			/* NOTE(EimaMei): Having a 'print' statement forces smart compilers
			 * like GCC and Clang to become dumb, by stopping them optimzing the function out. */ \
			si_printf("%n", medianIndex); \
			\
			volatile u64 index; \
			u64 t1, t2; \
			\
			t1 = si_RDTSC(); \
			for (index = 0; index < medianIndex; index++) { (void)function;  } \
			t2 = si_RDTSC(); \
			\
			array[*(lenPtr)] = t2 - t1; \
			*(lenPtr) += 1; \
		} \
	} while (0)

extern const siBenchmarkLimit siBenchLimit[];

const siBenchmarkLimit* si_benchmarkLimitLoop(u64 time);

SIDEF void si_benchmarkLoopsAvgPrint(cstring name, u64 cycles[20], usize len, usize range[2]);
SIDEF void si_benchmarkLoopsAvgCmpPrint(cstring names[2], u64 cycles[2][20], usize len, usize range[2]);
SIDEF void* si__benchmarkThread(void* arg);

#endif /* SI_NO_BENCHMARK */

/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| SI_IMPLEMENTATION    |
	========================
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*/

#ifdef SI_IMPLEMENTATION
	#ifndef SI_NO_MEMORY
		#define SI_IMPLEMENTATION_MEMORY
	#endif
	#ifndef SI_NO_VIRTUAL_MEMORY
		#define SI_IMPLEMENTATION_VIRTUAL_MEMORY
	#endif
	#ifndef SI_NO_ALLOCATOR
		#define SI_IMPLEMENTATION_ALLOCATOR
	#endif
	#ifndef SI_NO_ARRAY
		#define SI_IMPLEMENTATION_ARRAY
	#endif
	#ifndef SI_NO_STRING
		#define SI_IMPLEMENTATION_STRING
	#endif
	#ifndef SI_NO_UNICODE
		#define SI_IMPLEMENTATION_UNICODE
	#endif
	#ifndef SI_NO_CHAR
		#define SI_IMPLEMENTATION_CHAR
	#endif
	#ifndef SI_NO_HASHTABLE
		#define SI_IMPLEMENTATION_HASHTABLE
	#endif
	#ifndef SI_NO_SYSTEM
		#define SI_IMPLEMENTATION_SYSTEM
	#endif
	#ifndef SI_NO_IO
		#define SI_IMPLEMENTATION_IO
	#endif
	#ifndef SI_NO_THREAD
		#define SI_IMPLEMENTATION_THREAD
	#endif
	#ifndef SI_NO_TIME
		#define SI_IMPLEMENTATION_TIME
	#endif
	#ifndef SI_NO_BIT
		#define SI_IMPLEMENTATION_BIT
	#endif
	#ifndef SI_NO_CPU
		#define SI_IMPLEMENTATION_CPU
	#endif
	#ifndef SI_NO_PRINT
		#define SI_IMPLEMENTATION_PRINT
	#endif
	#ifndef SI_NO_DLL
		#define SI_IMPLEMENTATION_DLL
	#endif
	#ifndef SI_NO_MATH
		#define SI_IMPLEMENTATION_MATH
	#endif
	#ifndef SI_NO_BENCHMARK
		#define SI_IMPLEMENTATION_BENCHMARK
	#endif
#endif


#ifndef SI_NO_OPTIONAL

SIDEF
siError si__errorDeclare(i32 error, siErrorProc proc, void* userData, siCallerLoc call) {
	siError res;
	res.code = error;
#ifndef SI_NO_ERROR_STRUCT
	res.time = si_timeNowUTC();
	res.filename = call.filename;
	res.filenameLen = call.filenameLen;
	res.line = call.line;
#endif

#ifndef SI_NO_ERROR_LOGS
	if (proc != nil) {
		b32 status = proc(&res, userData);
		SI_STOPIF(!status, si_exit(-1));
	}
	else {
		siPrintColor red = si_printColor3bitEx(siPrintColor3bit_Red, true, false);
		si_fprintf(
			si_stderr, "%CError at \"%s:%i\"%C: Number '%i'.\n",
			red, res.filename, res.line, res.code
		);
	}
#endif

	return res;
}

#endif /* SI_NO_OPTIONAL */

#ifdef SI_IMPLEMENTATION_MEMORY

inline
b32 si_isPowerOfTwo(isize x) {
	return !(x & (x - 1)) && x > 0;
}

inline
usize si_alignForward(usize num, isize alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	usize align = (usize)alignment;
	return (num + align - 1) & ~(align - 1);
}

inline void* si_pointerAdd(void* ptr, isize bytes)                  { return si_cast(void*, (u8*)ptr + bytes); }
inline void* si_pointerSub(void* ptr, isize bytes)                  { return si_cast(void*, (u8*)ptr - bytes); }
inline const void* si_pointerAddConst(const void* ptr, isize bytes) { return si_cast(void*, (const u8*)ptr + bytes); }
inline const void* si_pointerSubConst(const void* ptr, isize bytes) { return si_cast(void*, (const u8*)ptr - bytes); }
inline isize si_pointerDiff(const void* begin, const void* end)     { return si_cast(isize, (const u8*)end - (const u8*)begin); }
inline b32 si_pointerBetween(const void* ptr, const void* start, const void* end) { return (ptr >= start) && (ptr <= end); }


SIDEF
isize si_memcopy(void* restrict dst, const void* restrict src, isize size) {
	SI_ASSERT_NOT_NIL(dst);
	SI_ASSERT_NOT_NIL(src);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	memcpy(dst, src, (usize)size);
	return size;
#else
	u8* dest = (u8*)dst;
	const u8* source = (const u8*)src;

	for_range (i, 0, size) {
		dest[i] = source[i];
	}

	return size;
#endif
}

SIDEF
isize si_memmove(void* restrict dst, const void* restrict src, isize size) {
	SI_ASSERT_NOT_NIL(dst);
	SI_ASSERT_NOT_NIL(src);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	memmove(dst, src, (usize)size);
	return size;
#else
	u8* dest = (u8*)dst;
	const u8* source = (const u8*)src;

	if (dest < source || (dest >= source + size)) {
		for_range (i, 0, size) {
		   	dest[i] = source[i];
		}
	}
	else {
		isize i;
		for (i = size - 1; i >= 0; i--) {
			dest[i] = source[i];
		}
	}

	return size;
#endif
}

SIDEF
isize si_memset(void* data, u8 value, isize size) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	memset(data, value, (usize)size);
	return size;
#else
	u8* ptr = (u8*)data;

	for_range (i, 0, size) {
		ptr[i] = value;
	}

	return size;
#endif

}

SIDEF
i32 si_memcompare(const void* ptr1, const void* ptr2, isize size) {
	SI_ASSERT_NOT_NIL(ptr1);
	SI_ASSERT_NOT_NIL(ptr2);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	return memcmp(ptr1, ptr2, (usize)size);
#else
	const u8* left = (const u8*)ptr1;
	const u8* right = (const u8*)ptr2;

	for_range (i, 0, size) {
		if (left[i] != right[i]) {
			return (left[i] - right[i]);
		}
	}

	return 0;
#endif
}

SIDEF
void* si_memchr(const void* data, u8 value, isize size) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	return (void*)memchr(data, value, (usize)size);
#else
	const u8* ptr = (const u8*)data;
	for_range (i, 0, size) {
		if (ptr[i] == value) {
			return (void*)&ptr[i];
		}
	}

	return nil;
#endif
}


SIDEF
isize si_cstrLen(cstring string) {
	SI_ASSERT_NOT_NIL(string);

#ifndef SI_NO_CRT
	return (isize)strlen(string);
#else
	isize i = 0;
	while (string[i] != '\0') { i += 1; }

	return i;
#endif

}

inline
isize si_memmoveLeft(void* src, isize length, isize moveBy) {
	return si_memmove((u8*)src - moveBy, src, length);
}
inline
isize si_memmoveRight(void* src, isize length, isize moveBy) {
	return si_memmove((u8*)src + moveBy, src, length);
}

#endif /* SI_IMPLEMENTATION_MEMORY */

#ifdef SI_IMPLEMENTATION_VIRTUAL_MEMORY

SIDEF
siResult(siVirtualMemory) si_vmAlloc(void* address, isize size) {
	SI_ASSERT_NOT_NEG(size);

	siVirtualMemory vm;
	vm.size = size;

#if SI_SYSTEM_IS_WINDOWS
	vm.data = VirtualAlloc(address, (usize)size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	SI_OPTION_SYS_CHECK(vm.data == nil, siVirtualMemory);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	vm.data = mmap(address, (usize)size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	SI_OPTION_SYS_CHECK(vm.data == MAP_FAILED, siVirtualMemory);
#else
	vm.data = nil;
	SI_UNUSED(address);
#endif

	return SI_OPT(siVirtualMemory, vm);
}

SIDEF
siError si_vmFree(siVirtualMemory vm) {
	SI_ASSERT_NOT_NIL(vm.data);

#if SI_SYSTEM_IS_WINDOWS
	MEMORY_BASIC_INFORMATION info;
	while (vm.size > 0) {
		isize res = (isize)VirtualQuery(vm.data, &info, si_sizeof(info));
		SI_ERROR_SYS_CHECK_RET(res == 0);

		if (info.BaseAddress != vm.data || info.AllocationBase != vm.data ||
			info.State != MEM_COMMIT || info.RegionSize > (usize)vm.size) {
			break;
		}

		res = VirtualFree(vm.data, 0, MEM_RELEASE);
		SI_ERROR_SYS_CHECK_RET(res == 0);

		vm.data = si_pointerAdd(vm.data, (isize)info.RegionSize);
		vm.size -= (isize)info.RegionSize;
	}
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	int res = munmap(vm.data, (usize)vm.size);
	SI_ERROR_SYS_CHECK_RET(res != 0);
#endif

	return SI_ERROR_NIL;
}

SIDEF
siError si_vmPurge(siVirtualMemory vm) {
	SI_ASSERT_NOT_NIL(vm.data);

#if SI_SYSTEM_IS_WINDOWS
	void* res = VirtualAlloc(vm.data, (usize)vm.size, MEM_RESET, PAGE_READWRITE);
	SI_ERROR_SYS_CHECK_RET(res == nil);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	int res = madvise(vm.data, (usize)vm.size, MADV_DONTNEED);
	SI_ERROR_SYS_CHECK_RET(res != 0);
#endif

	return SI_ERROR_NIL;
}


#endif /* SI_IMPLEMENTATION_VIRTUAL_MEMORY */

#ifdef SI_IMPLEMENTATION_ALLOCATOR

inline
void* si_alloc(siAllocator alloc, isize bytes) {
	SI_ASSERT_NOT_NEG(bytes);

	siAllocatorFunc func;
	func.type = siAllocatorFunc_Alloc;
	func.data.alloc = (usize)bytes;

	return alloc.proc(func, alloc.data);
}

inline
void si_free(siAllocator alloc, void* ptr) {
	SI_STOPIF(ptr == nil, return);

	siAllocatorFunc func;
	func.type = siAllocatorFunc_Free;
	func.data.free = ptr;

	alloc.proc(func, alloc.data);
}

inline
void si_freeAll(siAllocator alloc) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_FreeAll;

	alloc.proc(func, alloc.data);
}

inline
void* si_realloc(siAllocator alloc, void* ptr, isize sizeOld, isize sizeNew) {
	SI_ASSERT_NOT_NEG(sizeOld);
	SI_ASSERT_NOT_NEG(sizeNew);

	siAllocatorFunc func;
	func.type = siAllocatorFunc_Resize;
	func.data.resize = (siAllocatorDataResize){ptr, (usize)sizeOld, (usize)sizeNew};

	return alloc.proc(func, alloc.data);
}


inline
isize si_allocatorAvailable(siAllocator alloc) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_GetAvailable;

	return (isize)alloc.proc(func, alloc.data);
}



inline
siAllocator si_allocatorHeap(void) {
	siAllocator alloc;
	alloc.proc = si_allocator_heap_proc;
	alloc.data = nil;

	return alloc;
}

SIDEF
void* si_allocator_heap_proc(siAllocatorFunc func, void* data) {
#ifndef SI_NO_CRT
	void* out;
	switch (func.type) {
		case siAllocatorFunc_Alloc: {
			out = malloc(func.data.alloc);
			SI_ASSERT_NOT_NIL(out);
		} break;

		case siAllocatorFunc_Free: {
			free(func.data.free);
			out = nil;
		} break;

		case siAllocatorFunc_FreeAll: {
			SI_PANIC_MSG("Cannot free all for a heap allocator.");
		} break;

		case siAllocatorFunc_Resize: {
			siAllocatorDataResize resize = func.data.resize;
			out = realloc(resize.src, resize.sizeNew);
			SI_ASSERT_NOT_NIL(out);
		} break;

		case siAllocatorFunc_GetAvailable: {
			out = (void*)ISIZE_MAX;
		} break;

		default: SI_PANIC();
	}

	SI_UNUSED(data);
	return out;

#else
	SI_PANIC_MSG("Heap allocator is unsupported without using CRT functions.");
	SI_UNUSED(func); SI_UNUSED(data);
#endif
}

inline
siAllocatorArena si_arenaMake(siAllocator alloc, isize capacity) {
	return si_arenaMakeEx(alloc, capacity, SI_DEFAULT_MEMORY_ALIGNMENT);
}
SIDEF
siAllocatorArena si_arenaMakeEx(siAllocator alloc, isize capacity, i32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	SI_ASSERT_NOT_NEG(capacity);

	siAllocatorArena out = {0};
	out.alloc = alloc;
	out.alignment = alignment;
	out.capacity = capacity;
	out.ptr = si_alloc(out.alloc, out.capacity);

	return out;
}
inline
siAllocatorArena si_arenaMakePtr(void* ptr, isize capacity, i32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	SI_ASSERT_NOT_NEG(capacity);

	siAllocatorArena out = {0};
	out.alignment = alignment;
	out.capacity = capacity;
	out.ptr = ptr;

	return out;
}

inline
siAllocator si_allocatorArena(siAllocatorArena* arena) {
	siAllocator alloc;
	alloc.data = arena;
	alloc.proc = si_allocator_arena_proc;
	return alloc;
}


SIDEF
void si_arenaFree(siAllocatorArena* arena) {
	si_free(arena->alloc, arena->ptr);
	arena->ptr = nil;
	arena->offset = 0;
	arena->capacity = 0;
}

SIDEF
void* si_allocator_arena_proc(siAllocatorFunc func, void* data) {
	siAllocatorArena* alloc = (siAllocatorArena*)data;
	SI_ASSERT_MSG(alloc->ptr != nil, "You cannot use an already freed arena.");

	void* out;
	switch (func.type) {
		case siAllocatorFunc_Alloc: {
			isize bytes = (isize)si_alignForward(func.data.alloc, alloc->alignment);
			out = &alloc->ptr[alloc->offset];

			alloc->offset += bytes;
			if (alloc->offset > alloc->capacity) {
				SI_PANIC_FMT(
					"Exceeded the available memory for an allocation (Tried writing '%zd' bytes "
					"into an already filled allocator with '%zd' bytes. Current offset: '%zd').\n",
					bytes, alloc->capacity, alloc->offset
				);
			}
		} break;

		case siAllocatorFunc_Free: {
			//SI_PANIC_MSG("Unsupported function.");
			out = nil;
		} break;

		case siAllocatorFunc_FreeAll: {
			alloc->offset = 0;
			out = nil;
		} break;

		case siAllocatorFunc_Resize: {
			siAllocatorDataResize resize = func.data.resize;
			SI_STOPIF(resize.sizeOld >= resize.sizeNew, return resize.src);

			func.type = siAllocatorFunc_Alloc;
			func.data.alloc = resize.sizeNew;
			if (resize.src == nil) {
				return si_allocator_arena_proc(func, data);
			}

			out = si_allocator_arena_proc(func, data);
			si_memcopy(out, resize.src, (isize)resize.sizeOld);
		} break;

		case siAllocatorFunc_GetAvailable: {
			out = (void*)(alloc->capacity - alloc->offset);
		} break;

		default: SI_PANIC();
	}

	return out;
}


inline
siAllocatorArenaTmp si_arenaTmpStart(siAllocatorArena* arena) {
	siAllocatorArenaTmp tmp;
	tmp.arena = arena;
	tmp.offset = arena->offset;
	return tmp;
}
inline
void si_arenaTmpEnd(siAllocatorArenaTmp tmp) {
	tmp.arena->offset = tmp.offset;
}


inline
siAllocatorLifo si_lifoMake(siAllocator alloc, isize capacity) {
	return si_lifoMakeEx(alloc, capacity, SI_DEFAULT_MEMORY_ALIGNMENT);
}
SIDEF
siAllocatorLifo si_lifoMakeEx(siAllocator alloc, isize capacity, i32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	SI_ASSERT_NOT_NEG(capacity);

	siAllocatorLifo lifo = {0};
	lifo.alloc = alloc;
	lifo.alignment = alignment;
	lifo.capacity = capacity;
	lifo.ptr = si_alloc(lifo.alloc, lifo.capacity);

	return lifo;
}
inline
siAllocatorLifo si_lifoMakePtr(void* ptr, isize capacity, i32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	SI_ASSERT_NOT_NEG(capacity);

	siAllocatorLifo lifo = {0};
	lifo.alignment = alignment;
	lifo.capacity = capacity;
	lifo.ptr = ptr;

	return lifo;
}

inline
siAllocator si_allocatorLifo(siAllocatorLifo* lifo) {
	siAllocator alloc;
	alloc.data = lifo;
	alloc.proc = si_allocator_lifo_proc;
	return alloc;
}


SIDEF
void si_lifoFree(siAllocatorLifo* lifo) {
	si_free(lifo->alloc, lifo->ptr);
	lifo->ptr = nil;
	lifo->capacity = 0;
}


SIDEF
void* si_allocator_lifo_proc(siAllocatorFunc func, void* data) {
	siAllocatorLifo* lifo = (siAllocatorLifo*)data;
	SI_ASSERT_MSG(lifo->ptr != nil, "You cannot use an already freed LIFO allocator.");

	void* out;
	switch (func.type) {
		case siAllocatorFunc_Alloc: {
			void* ptr = &lifo->ptr[lifo->offset];
			*(isize*)ptr = (isize)lifo->offset;

			isize bytes = (isize)si_alignForward(si_sizeof(isize) + func.data.alloc, lifo->alignment);
			lifo->offset += bytes;

			if (lifo->offset > lifo->capacity) {
				SI_PANIC_FMT(
					"Exceeded the available memory for an allocation (Tried writing '%zd' bytes "
					"into an already filled allocator with '%zd' bytes. Current offset: '%zd').\n",
					bytes, lifo->capacity, lifo->offset
				);
			}

			out = si_pointerAdd(ptr, si_sizeof(isize));
		} break;

		case siAllocatorFunc_Free: {
			SI_ASSERT_MSG(
				si_pointerBetween(func.data.free, lifo->ptr, &lifo->ptr[lifo->offset])
				&& lifo->offset != 0, "Invalid pointer."
			);

			lifo->offset = *(isize*)si_pointerSub(func.data.free, si_sizeof(isize));
			out = nil;
		} break;

		case siAllocatorFunc_FreeAll: {
			lifo->offset = 0;
			out = nil;
		} break;

		case siAllocatorFunc_Resize: {
			out = nil;
			SI_PANIC_MSG("Unsupported function.");
		} break;

		case siAllocatorFunc_GetAvailable: {
			out = (void*)(lifo->capacity - lifo->offset);
		} break;

		default: SI_PANIC();
	}

	return out;
}


inline
siAllocatorPool si_poolMake(siAllocator alloc, isize numChunks, isize chunkSize) {
	return si_poolMakeEx(alloc, numChunks, chunkSize, SI_DEFAULT_MEMORY_ALIGNMENT);
}
SIDEF
siAllocatorPool si_poolMakeEx(siAllocator alloc, isize numChunks, isize chunkSize,
		i32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	SI_ASSERT_NOT_NEG(numChunks);
	SI_ASSERT_NOT_NEG(chunkSize);

	siAllocatorPool pool;
	pool.alloc = alloc;
	pool.alignment = alignment;
	pool.numChunks = numChunks;
	pool.chunkSize = chunkSize;
	pool.ptr = si_alloc(pool.alloc, pool.numChunks * pool.chunkSize);
	pool.head = nil;

	for_range (i, 0, pool.numChunks) {
		void* ptr = &pool.ptr[i * pool.chunkSize];
		siAllocatorPoolFreeNode* node = (siAllocatorPoolFreeNode*)ptr;

		node->next = pool.head;
		pool.head = node;
	}

	return pool;

}

inline
siAllocator si_allocatorPool(siAllocatorPool* pool) {
	siAllocator alloc;
	alloc.data = pool;
	alloc.proc = si_allocator_pool_proc;
	return alloc;
}


SIDEF
void si_poolFree(siAllocatorPool* pool) {
	si_free(pool->alloc, pool->ptr);
	pool->ptr = nil;
	pool->numChunks = 0;
}

SIDEF
void* si_allocator_pool_proc(siAllocatorFunc func, void* data) {
	siAllocatorPool* pool = (siAllocatorPool*)data;
	SI_ASSERT_MSG(pool->ptr != nil, "You cannot use an already freed pool.");

	void* out;
	switch (func.type) {
		case siAllocatorFunc_Alloc: {
			SI_ASSERT_FMT(
				(isize)func.data.alloc <= pool->chunkSize,
				"Allocated object must fit the chunk size (requested allocation: '%zd', chunk size: '%zd')",
				(isize)func.data.alloc, pool->chunkSize
			);

			out = pool->head;
			if (out == nil) {
				SI_PANIC_MSG("Pool allocator has no free memory.");
			}

			pool->head = pool->head->next;
		} break;

		case siAllocatorFunc_Free: {
			siAllocatorPoolFreeNode* node = func.data.free;
			if (!si_pointerBetween(node, pool->ptr, &pool->ptr[pool->numChunks * pool->chunkSize])) {
				SI_PANIC_MSG("Invalid pointer.");
			}

			node->next = pool->head;
			pool->head = node;
			out = nil;
		} break;

		case siAllocatorFunc_FreeAll: {
			for_range (i, 0, pool->numChunks) {
				void* ptr = &pool->ptr[i * pool->chunkSize];
				siAllocatorPoolFreeNode* node = (siAllocatorPoolFreeNode*)ptr;

				node->next = pool->head;
				pool->head = node;
			}
			out = nil;
		} break;

		case siAllocatorFunc_Resize: {
			SI_PANIC_MSG("Unsupported function.");
		} break;

		case siAllocatorFunc_GetAvailable: {
			out = (pool->head != nil) ? (void*)pool->chunkSize : 0;
		} break;

		default: SI_PANIC();
	}

	return out;
}

siNoreturn SIDEF
void si_panic(cstring conditionStr, siCallerLoc call, cstring message, ...) {
	siPrintColor red = si_printColor3bitEx(siPrintColor3bit_Red, true, false);

	si_fprintf(
		si_stderr,
		"%CAssertion \"%s\" at \"%s:%d\"%C: %s%s",
		red, conditionStr, call.filename, call.line, call.function, (message != nil) ? ": " : "\n"
	);


	if (message != nil) {
		va_list va;
		va_start(va, message);
		si_fprintfLnVa(si_stderr, message, va);
		va_end(va);
	}

	SI_DEBUG_TRAP();
}

#endif /* SI_IMPLEMENTATION_ALLOCATOR */

#ifdef SI_IMPLEMENTATION_ARRAY


inline
siBufferAny si_slice(siBufferAny array, isize start, isize end) {
	SI_ASSERT_NOT_NEG(start);
	SI_ASSERT_NOT_NEG(end);
	SI_ASSERT(start < array.len);
	SI_ASSERT(end < array.len);

	siBufferAny buffer;
	buffer.data = si_bufferGet(array, start);
	buffer.len = (end - start) + 1;
	buffer.typeSize = array.typeSize;

	return buffer;
}

inline
siBufferAny si_sliceEnd(siBufferAny array, isize start) {
	return si_slice(array, start, (array.len - 1));
}
inline
siBufferAny si_sliceStart(siBufferAny array, isize end) {
	return si_slice(array, 0, end);
}
inline
siBufferAny si_sliceLen(siBufferAny array, isize start, isize length) {
	return si_slice(array, start, start + length);
}

inline
void* si_bufferGet(siBufferAny buffer, isize index) {
	SI_ASSERT_NOT_NIL(buffer.data);
	SI_ASSERT_NOT_NEG(index);
	SI_ASSERT(index < buffer.len);

	return (u8*)buffer.data + index * buffer.typeSize;
}

inline
void* si__bufferGetType(siBufferAny buffer, isize index, isize typeSizeof) {
	SI_ASSERT(typeSizeof == buffer.typeSize);
	return si_bufferGet(buffer, index);
}

inline
void* si_bufferFront(siBufferAny array) {
	return si_bufferGet(array, 0);
}

inline
void* si_bufferBack(siBufferAny array) {
	return si_bufferGet(array, array.len - 1);
}

inline
void si_bufferSet(siBufferAny buffer, isize index, const void* data) {
	si_memcopy(si_bufferGet(buffer, index), data, buffer.typeSize);
}

/* Copies an element at the specified index into the given pointer and returns
 * true if the index is less than the array's length. Otherwise, 'false' is returned. */
inline
b32 si_bufferAtGet(siBufferAny buffer, isize index, void* out) {
	SI_ASSERT_NOT_NIL(out);

	if (index >= buffer.len) {
		return false;
	}

	si_memcopy(out, si_bufferGet(buffer, index), buffer.typeSize);
	return true;
}

inline
b32 si_bufferAtFront(siBufferAny buffer, void* out) {
	SI_STOPIF(buffer.len == 0, return false);
	si_memcopy(out, buffer.data, buffer.typeSize);

	return true;
}

inline
b32 si_bufferAtBack(siBufferAny buffer, void* out) {
	SI_STOPIF(buffer.len == 0, return false);
	si_memcopy(out, si_bufferGet(buffer, buffer.len - 1), buffer.typeSize);

	return true;
}

SIDEF
isize si_bufferFind(siBufferAny buffer, void* valuePtr) {
	for_range (i, 0, buffer.len) {
		void* dst = si_bufferGet(buffer, i);
		if (si_memcompare(dst, valuePtr, buffer.typeSize) == 0) {
			return i;
		}
	}

	return -1;
}

SIDEF
isize si_bufferFindLast(siBufferAny buffer, void* valuePtr) {
	isize i;
	for (i = buffer.len - 1; i >= 0; i -= 1) {
		void* dst = si_bufferGet(buffer, i);
		if (si_memcompare(dst, valuePtr, buffer.typeSize) == 0) {
			return i;
		}
	}

	return -1;
}

SIDEF
isize si_bufferFindCount(siBufferAny buffer, void* valuePtr) {
	isize occurences = 0;
	for_range (i, 0, buffer.len) {
		void* element = si_bufferGet(buffer, i);
		if (si_memcompare(element, valuePtr, buffer.typeSize) == 0) {
			occurences += 1;
		}
	}

	return occurences;

}

SIDEF
b32 si_bufferEqual(siBufferAny lhs, siBufferAny rhs) {
	SI_STOPIF(lhs.len != rhs.len, return false);
	SI_STOPIF(lhs.data == rhs.data, return true);

	return si_memcompare(lhs.data, rhs.data, si_min(isize, lhs.len, rhs.len)) == 0;
}


SIDEF
siArrayAny si_arrayMakeEx(void* list, isize typeSize, isize count, siAllocator alloc)  {
	siArrayAny array = si_arrayMakeReserve(typeSize, count, count, alloc);
	si_memcopy(array.data, list, typeSize * count);

	return array;
}

inline
siArrayAny si_arrayMakeReserve(isize typeSize, isize length, isize capacity,
		siAllocator alloc) {
	SI_ASSERT_NOT_NEG(typeSize);
	SI_ASSERT_NOT_NEG(length);
	SI_ASSERT_NOT_NEG(capacity);

	siArrayAny array;
	array.alloc = alloc;
	array.typeSize = typeSize;
	array.len = length;
	array.capacity = capacity;
	array.data = si_alloc(alloc, typeSize * capacity);
	array.grow = 0;

	return array;
}

inline
siArrayAny si_arrayCopy(siBufferAny buffer, siAllocator alloc) {
	return si_arrayMakeEx(buffer.data, buffer.typeSize, buffer.len, alloc);
}

inline
void si_arrayFree(siArrayAny array) {
	si_free(array.alloc, array.data);
}


inline void* si_arrayGet(siArrayAny array, isize index) { return si_bufferGet(SI_BUF_ARR(array), index); }
inline void* si_arrayFront(siArrayAny array) { return si_bufferFront(SI_BUF_ARR(array)); }
inline void* si_arrayBack(siArrayAny array) { return si_bufferBack(SI_BUF_ARR(array)); }
inline void si_arraySet(siArrayAny array, isize index, void* data) { si_bufferSet(SI_BUF_ARR(array), index, data); }

inline b32 si_arrayAtGet(siArrayAny array, isize index, void* out) { return si_bufferAtGet(SI_BUF_ARR(array), index, out); }
inline b32 si_arrayAtFront(siArrayAny array, void* out) { return si_bufferAtFront(SI_BUF_ARR(array), out); }
inline b32 si_arrayAtBack(siArrayAny array, void* out) { return si_bufferAtBack(SI_BUF_ARR(array), out); }

inline isize si_arrayFind(siArrayAny array, void* valuePtr) { return si_bufferFind(SI_BUF_ARR(array), valuePtr); }
inline isize si_arrayFindLast(siArrayAny array, void* valuePtr) { return si_bufferFindLast(SI_BUF_ARR(array), valuePtr); }
inline isize si_arrayFindCount(siArrayAny array, void* valuePtr) { return si_bufferFindCount(SI_BUF_ARR(array), valuePtr); }

inline b32 si_arrayEqual(siArrayAny lhs, siArrayAny rhs) { return si_bufferEqual(SI_BUF_ARR(lhs), SI_BUF_ARR(rhs)); }



inline
b32 si_arrayAppend(siArrayAny* array, void* valuePtr) {
	return si_arrayAppendEx(array, valuePtr, 1);
}
SIDEF
b32 si_arrayAppendEx(siArrayAny* array, void* valueArray, isize count) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_NOT_NIL(valueArray);
	SI_ASSERT_NOT_NEG(count);

	isize oldLen = array->len;
	b32 allocated = si_arrayMakeSpaceFor(array, count);

	si_memcopy(si_arrayGet(*array, oldLen), valueArray, count * array->typeSize);
	return allocated;
}

inline
void si_arrayPop(siArrayAny* array) {
	SI_ASSERT_NOT_NIL(array);
	SI_STOPIF(array->len == 0, return);
	array->len -= 1;
}

inline
void si_arrayClear(siArrayAny* array) {
	SI_ASSERT_NOT_NIL(array);
	array->len = 0;
}


inline
b32 si_arrayInsert(siArrayAny* array, isize index, void* value) {
	return si_arrayInsertEx(array, index, value, 1);
}

SIDEF
b32 si_arrayInsertEx(siArrayAny* array, isize index, void* valueArray, isize count) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_NOT_NIL(valueArray);
	SI_ASSERT_NOT_NEG(index);
	SI_ASSERT_NOT_NEG(count);
	SI_ASSERT(index < array->len);
	SI_STOPIF(count == 0, return false);

	isize remainderLen = array->len - (index + count);
	b32 allocated = si_arrayMakeSpaceFor(array, count);

	u8* dst = si_arrayGet(*array, index);
	si_memcopy(si_arrayGet(*array, index + count), dst, remainderLen * array->typeSize);
	si_memcopy(dst, valueArray, count * array->typeSize);

	return allocated;
}

inline
void si_arrayErase(siArrayAny* array, isize index) {
	si_arrayEraseEx(array, index, 1);
}

SIDEF
void si_arrayEraseEx(siArrayAny* array, isize index, isize count) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_NOT_NEG(index);
	SI_ASSERT_NOT_NEG(count);
	SI_ASSERT(index + count <= array->len);
	SI_STOPIF(array == 0, return);

	isize length = array->len - index - count;
	u8* dst = si_arrayGet(*array, index);
	u8* src = si_arrayGet(*array, index + count);
	si_memcopy(dst, src, length * array->typeSize);

	array->len -= count;
}


inline
void si_arrayReplaceAll(siArrayAny array, void* restrict valueOld, void* restrict valueNew) {
	si_arrayReplace(array, valueOld, valueNew, -1);
}

SIDEF
void si_arrayReplace(siArrayAny array, void* restrict valueOld, void* restrict valueNew,
		isize amount) {
	SI_ASSERT_NOT_NIL(valueOld);
	SI_ASSERT_NOT_NIL(valueNew);
	SI_STOPIF(amount == 0, return);


	isize count = 0;
	for_range (i, 0, array.len) {
		void* element = si_arrayGet(array, i);
		if (si_memcompare(element, valueOld, array.typeSize) == 0) {
			si_memcopy(element, valueNew, array.typeSize);
			count += 1;

			if (count == amount) {
				break;
			}
		}
	}
}

SIDEF
void si_arrayReverse(siArrayAny array) {
	SI_ASSERT(array.typeSize <= 1024);

	u8* a = si_arrayFront(array);
	u8* b = si_arrayBack(array);
	u8 tmp[1024];

	isize len = array.len / 2;
	while (len) {
		si_memcopy(tmp, a, array.typeSize);
		si_memcopy(a, b, array.typeSize);
		si_memcopy(b, tmp, array.typeSize);

		a += array.typeSize;
		b -= array.typeSize;

		len -= 1;
	}
}

SIDEF
b32 si_arrayFill(siArrayAny* array, isize index, isize count, void* valuePtr) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_NOT_NIL(valuePtr);
	SI_ASSERT_NOT_NEG(index);
	SI_ASSERT_NOT_NEG(count);

	isize addLen = (index + count) - array->len;
	b32 allocated = (addLen > 0)
		? si_arrayMakeSpaceFor(array, addLen)
		: false;

	u8* dst = si_arrayGet(*array, index);
	for_range (i, 0, count) {
		dst += si_memcopy(dst, valuePtr, array->typeSize);
	}

	return allocated;
}



SIDEF
b32 si_arrayMakeSpaceFor(siArrayAny* array, isize addLen) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_NOT_NIL(array->alloc.proc);

	isize newLength = array->len + addLen;
	if (newLength <= array->capacity) {
		array->len = newLength;
		return false;
	}

	isize newCapacity = (array->grow <= 0)
		? SI_BUFFER_NEW_CAP(array, addLen)
		: array->capacity + addLen + array->grow;
	array->data = si_realloc(
		array->alloc, array->data,
		array->capacity * array->typeSize, newCapacity * array->typeSize
	);
	array->len = newLength;
	array->capacity = newCapacity;

	return true;
}

inline
isize si_memcopy_s(siBufferAny dst, const void* src, isize sizeSrc) {
	isize length = si_min(isize, dst.len, sizeSrc);
	return si_memcopy(dst.data, src, length);
}

inline
isize si_memmove_s(siBufferAny dst, const void* src, isize sizeSrc) {
	isize length = si_min(isize, dst.len, sizeSrc);
	return si_memmove(dst.data, src, length);
}


#endif /* SI_IMPLEMENTATION_ARRAY */

#ifdef SI_IMPLEMENTATION_STRING

inline
siUtf32Char si__stringLastRune(siString str) {
	isize i;
	for (i = str.len - 1; i >= 0; i -= 1) {
		if ((str.data[i] & 0xC0) != 0x80) {
			break;
		}
	}

	return si_utf8Decode(&str.data[i]);
}

inline
isize si__forEachStr(const u8* data, siRune* rune, siUtf32Char* tmp, isize i) {
	*tmp = si_utf8Decode(&data[i]);
	*rune = tmp->codepoint;

	return i;
}

inline
isize si__forEachRevStr(const u8* data, siRune* rune, siUtf32Char* tmp, isize i) {
	isize oldSize = tmp->len;
	*tmp = si__stringLastRune(SI_STR_LEN(data, i));
	*rune = tmp->codepoint;

	return oldSize;
}


inline
siBuilder si_builderMake(isize capacity, siAllocator alloc) {
	return si_builderMakeLen(0, capacity, alloc);
}

inline
siBuilder si_builderMakeLen(isize len, isize capacity, siAllocator alloc) {
	SI_ASSERT_NOT_NEG(len);
	SI_ASSERT_NOT_NEG(capacity);

	siBuilder b;
	b.capacity = capacity;
	b.len = 0;
	b.alloc = alloc;
	b.data = si_allocArray(alloc, u8, capacity);
	b.grow = -1;

	return b;
}

SIDEF
siBuilder si_builderMakeNone(siAllocator alloc) {
	siBuilder b;
	b.capacity = 0;
	b.len = 0;
	b.alloc = alloc;
	b.data = nil;
	b.grow = -1;

	return b;

}

inline
char* si_builderToCstr(siBuilder b) {
	si_builderWriteByte(&b, '\0');
	return (char*)b.data;
}

inline
siString si_builderToStr(siBuilder b) {
	return SI_STR_LEN(b.data, b.len);
}


SIDEF
b32 si_builderMakeSpaceFor(siBuilder* b, isize addLen) {
	SI_ASSERT_NOT_NIL(b);
	SI_ASSERT_NOT_NIL(b->alloc.proc);

	isize newLength = b->len + addLen;
	if (newLength <= b->capacity) {
		SI_ASSERT_NOT_NEG(newLength);

		b->len = newLength;
		return false;
	}

	isize newCapacity = (b->grow <= 0)
		? SI_BUFFER_NEW_CAP(b, addLen)
		: b->capacity + addLen + b->grow;
	b->data = si_realloc(b->alloc, b->data, b->capacity, newCapacity);
	b->len = newLength;
	b->capacity = newCapacity;

	return true;
}

inline
b32 si_builderWriteByte(siBuilder* b, u8 byte) {
	return si_builderWriteBytes(b, &byte, 1);
}

SIDEF
b32 si_builderWriteBytes(siBuilder* b, const void* bytes, isize len) {
	isize oldLen = b->len;
	b32 allocated = si_builderMakeSpaceFor(b, len);

	si_memcopy(&b->data[oldLen], bytes, len);
	return allocated;

}
inline
b32 si_builderWriteStr(siBuilder* b, siString str) {
	return si_builderWriteBytes(b, str.data, str.len);
}
inline
b32 si_builderWriteRune(siBuilder* b, siRune character) {
	siUtf8Char str = si_utf8Encode(character);
	return si_builderWriteBytes(b, str.codepoint, str.len);
}

inline
b32 si_builderWriteStrQuoted(siBuilder* b, siString str) {
	return si_builderWriteStrQuotedEx(b, str, '\"');
}

SIDEF
b32 si_builderWriteStrQuotedEx(siBuilder* b, siString str, u8 quote) {
	u8* data = &b->data[b->len];
	b32 allocated = si_builderMakeSpaceFor(b, 2 + str.len);

	data[0] = quote;
	si_memcopyStr(&data[1], str);
	data[1 + str.len] = quote;

	return allocated;
}

SIDEF
b32 si_builderWriteStrQuotedRune(siBuilder* b, siString str, siRune quoteStart,
		siRune quoteEnd) {
	SI_ASSERT_NOT_NIL(b);

	u8* data = &b->data[b->len];
	siUtf8Char start = si_utf8Encode(quoteStart),
			   end = si_utf8Encode(quoteEnd);
	b32 allocated = si_builderMakeSpaceFor(b, start.len + str.len + end.len);

	isize i = 0;
	i += si_memcopy(&data[i], start.codepoint, start.len);
	i += si_memcopyStr(&data[i], str);
	si_memcopy(&data[i], end.codepoint, end.len);

	return allocated;
}

inline
b32 si_builderWriteInt(siBuilder* b, i64 num) {
	return si_builderWriteIntEx(b, num, 10);
}
SIDEF
b32 si_builderWriteIntEx(siBuilder* b, i64 num, i32 base) {
	isize oldLen = b->len;
	b32 allocated = si_builderMakeSpaceFor(b, si_numLenIntEx(num, base));

	si_stringFromIntEx(num, base, SI_BUF_LEN(&b->data[oldLen], b->len - oldLen));

	return allocated;
}

inline
b32 si_builderWriteUInt(siBuilder* b, u64 num) {
	return si_builderWriteUIntEx(b, num, 10);
}
SIDEF
b32 si_builderWriteUIntEx(siBuilder* b, u64 num, i32 base) {
	isize oldLen = b->len;
	b32 allocated = si_builderMakeSpaceFor(b, si_numLenEx(num, base));

	si_stringFromUIntEx(num, base, SI_BUF_LEN(&b->data[oldLen], b->len - oldLen));

	return allocated;
}

inline
void si_builderClear(siBuilder* b) {
	SI_ASSERT_NOT_NIL(b);
	b->len = 0;
}

inline
void si_builderPopByte(siBuilder* b) {
	SI_ASSERT_NOT_NIL(b);
	b->len -= 1;
}
inline
void si_builderPopRune(siBuilder* b) {
	SI_ASSERT_NOT_NIL(b);

	siUtf32Char str = {0};
	isize i = 0;
	while (i < b->len) {
		str = si_utf8Decode(&b->data[i]);
		i += str.len;
	}
	b->len -= str.len;
}

inline
void si_builderFree(siBuilder* b) {
	SI_ASSERT_NOT_NIL(b);
	SI_ASSERT_NOT_NIL(b->data);

	si_free(b->alloc, b->data);
	b->data = nil;
}



inline
siString si_stringCopy(siString from, siAllocator alloc) {
	u8* str = si_allocArray(alloc, u8, from.len);
	si_memcopyStr(str, from);

	return SI_STR_LEN(str, from.len);
}

inline
siString si_stringCopyFromCstr(cstring from, siAllocator alloc) {
	return si_stringCopy(SI_CSTR(from), alloc);
}

inline
char* si_stringCopyToCstr(siString from, siAllocator alloc) {
	return si_stringCloneToCstrEx(from, from.len, alloc);
}
SIDEF
char* si_stringCloneToCstrEx(siString from, isize capacity, siAllocator alloc) {
	char* str = si_allocArray(alloc, char, capacity + 1);
	si_memcopyStr(str, from);
	str[from.len] = '\0';

	return str;
}


inline
i32 si_stringAtFront(siString string) {
	SI_ASSERT_NOT_NIL(string.data);
	SI_STOPIF(string.len == 0, return -1);

	return si_utf8Decode(string.data).codepoint;
}

SIDEF
i32 si_stringAtBack(siString string) {
	SI_ASSERT_NOT_NIL(string.data);
	SI_STOPIF(string.len == 0, return -1);

	return si__stringLastRune(string).codepoint;
}

inline
const u8* si_stringBegin(siString string) {
	return string.data;
}

inline
const u8* si_stringEnd(siString string) {
	return &string.data[string.len];
}


inline
siString si_substr(siString string, isize start, isize end) {
	SI_ASSERT_NOT_NEG(start);
	SI_ASSERT_NOT_NEG(end);
	SI_ASSERT(start < string.len);
	SI_ASSERT(end < string.len);
	SI_ASSERT(start <= end);

	siString str;
	str.data = &string.data[start];
	str.len = (end - start) + 1;

	return str;
}

inline
siString si_substrEnd(siString string, isize start) {
	return si_substr(string, start, string.len - 1);
}
inline
siString si_substrStart(siString string, isize end) {
	return si_substr(string, 0, end);
}
inline
siString si_substrLen(siString string, isize start, isize length) {
	return si_substr(string, start, start + length);
}

SIDEF
isize si_stringFind(siString string, siString subStr) {
	isize counter = 0;

	for_range (i, 0, string.len) {
		const u8* x = &string.data[i];
		if (*x != subStr.data[counter]) {
			counter = 0;
			continue;
		}

		counter += 1;
		if (counter == subStr.len) {
			isize index = si_pointerDiff(string.data, x);
			return index - (subStr.len - 1);
		}
	}

	return -1;
}

inline
isize si_stringFindByte(siString string, u8 byte) {
	for_range (i, 0, string.len) {
		if (string.data[i] == byte) {
			return i;
		}
	}

	return -1;
}

SIDEF
isize si_stringFindRune(siString string, siRune rune) {
	siRune x;
	for_eachStrEx (x, index, string) {
		if (x == rune) {
			return index;
		}
	}

	return -1;
}

SIDEF
isize si_stringFindLast(siString string, siString subStr) {
	SI_ASSERT_NOT_NIL(string.data);

	isize subStrEnd = subStr.len - 1;
	isize counter = subStrEnd;

	isize i;
	for (i = string.len - 1; i >= 0; i -= 1) {
		if (string.data[i] != subStr.data[counter]) {
			counter = subStrEnd;
			continue;
		}

		counter -= 1;
		if (counter == -1) {
			return i;
		}
	}

	return -1;
}

inline
isize si_stringFindLastByte(siString string, u8 byte) {
	isize i;
	for (i = string.len - 1; i >= 0; i -= 1) {
		if (string.data[i] == byte) {
			return i;
		}
	}

	return -1;
}

SIDEF
isize si_stringFindLastRune(siString string, siRune rune) {
	siRune x;
	for_eachRevStrEx (x, index, string) {
		if (x == rune) {
			return index;
		}
	}

	return -1;
}


SIDEF
isize si_stringFindCount(siString string, siString subStr) {
	isize occurences = 0;
	isize counter = 0;

	for_range (i, 0, string.len) {
		if (string.data[i] != subStr.data[counter]) {
			counter = 0;
			continue;
		}

		counter += 1;
		if (counter == subStr.len) {
			occurences += 1;
			counter = 0;
		}
	}

	return occurences;

}


inline
i32 si_stringCompare(siString lhs, siString rhs) {
	return si_memcompare(lhs.data, rhs.data, si_min(isize, lhs.len, rhs.len));
}

inline
b32 si_stringEqual(siString lhs, siString rhs) {
	SI_STOPIF(lhs.len != rhs.len, return false);
	SI_STOPIF(lhs.data == rhs.data, return true);

	return si_stringCompare(lhs, rhs) == 0;
}



SIDEF
siString si_stringJoin(siBuffer(siString) strings, siString separator, siAllocator alloc) {
	SI_ASSERT_NOT_NIL(strings.data);
	SI_STOPIF(strings.len == 0, return SI_STR_EMPTY);

	isize length = separator.len * (strings.len - 1);
	siString str;
	for_eachBuf (str, strings) {
		length += str.len;
	}

	siString* strs = (siString*)strings.data;
	u8* res = si_allocArray(alloc, u8, length);
	si_memcopyStr(res, strs[0]);

	isize i = strs[0].len;
	for_range (j, 1, strings.len) {
		i += si_memcopyStr(&res[i], separator);
		i += si_memcopyStr(&res[i], strs[j]);
	}

	return SI_STR_LEN(res, length);
}


inline
siString si_stringTrim(siString string, siString cutSet) {
	return si_stringTrimRight(si_stringTrimLeft(string, cutSet), cutSet);
}

SIDEF
siString si_stringTrimLeft(siString string, siString cutSet) {
	const u8* posStart = si_stringBegin(string);
	const u8* posEnd = si_stringEnd(string);

	while (si_stringFindByte(cutSet, *posStart) != -1) {
		posStart += 1;
		SI_STOPIF(posStart >= posEnd, break);
	}

	return SI_STR_LEN(posStart, si_pointerDiff(posStart, posEnd));
}

SIDEF
siString si_stringTrimRight(siString string, siString cutSet) {
	const u8* posStart = si_stringBegin(string);
	const u8* posEnd = si_stringEnd(string) - 1;

	while (posEnd > posStart && si_stringFindByte(cutSet, *posEnd) != -1) {
		posEnd -= 1;
	}

	string.len = si_pointerDiff(posStart, posEnd + 1);
	return string;
}

inline
siString si_stringStrip(siString string) {
	return si_stringTrim(string, SI_STR(" \t\r\n\v\f"));
}


SIDEF
siString si_stringInsert(siString string, siString subStr, isize index, siAllocator alloc) {
	SI_ASSERT_NOT_NEG(index);
	SI_ASSERT(index < string.len);
	SI_STOPIF(subStr.len == 0, return string);

	isize len = string.len + subStr.len;
	u8* res = si_allocArray(alloc, u8, len);

	isize i = 0;
	i += si_memcopyStr(&res[i], si_substrStart(string, index - 1));
	i += si_memcopyStr(&res[i], subStr);
	i += si_memcopyStr(&res[i], si_substrEnd(string, index));

	return SI_STR_LEN(res, len);
}

inline
siString si_stringRemoveAll(siString string, siString subStr, siAllocator alloc) {
	return si_stringRemove(string, subStr, -1, alloc);
}

inline
siString si_stringRemove(siString string, siString subStr, i32 amount, siAllocator alloc) {
	return si_stringReplace(string, subStr, SI_STR(""), amount, alloc);
}


SIDEF
siString si_stringReverse(siString string, siAllocator alloc) {
	isize len = string.len;
	u8* res = si_allocArray(alloc, u8, len);

	isize i = len,
		  j = 0;
	while (i > 0) {
		siUtf32Char x = si_utf8Decode(&string.data[j]);

		i -= x.len;
		j += si_memcopy(&res[i], &string.data[j], x.len);
	}

	return SI_STR_LEN(res, len);
}


inline
siString si_stringReplaceAll(siString string, siString strOld, siString strNew,
		siAllocator alloc) {
	return si_stringReplace(string, strOld, strNew, -1, alloc);
}

SIDEF
siString si_stringReplace(siString string, siString strOld, siString strNew, isize amount,
		siAllocator alloc) {
	SI_STOPIF(strOld.data == strNew.data, return string);

	if (amount < 0) {
		amount = si_stringFindCount(string, strOld);
	}
	SI_STOPIF(amount == 0, return string);

	isize len = string.len + amount * (strNew.len - strOld.len);
	isize lineStart = 0, i = 0;
	u8* res = si_allocArray(alloc, u8, len);

	while (amount) {
		siString subStr = si_substrEnd(string, lineStart);
		subStr.len = si_stringFind(subStr, strOld);

		i += si_memcopyStr(&res[i], subStr);
		i += si_memcopyStr(&res[i], strNew);

		lineStart += subStr.len + strOld.len;
		amount -= 1;
	}
	si_memcopyStr(&res[i], si_substrEnd(string, lineStart));

	return SI_STR_LEN(res, len);
}


inline
siBuffer(siString) si_stringSplit(siString string, siString delimiter, siAllocator alloc) {
	return si_stringSplitEx(string, delimiter, -1, alloc);
}

SIDEF
siBuffer(siString) si_stringSplitEx(siString string, siString delimiter, isize amount,
		siAllocator alloc) {
	if (amount < 0) {
		amount = si_stringFindCount(string, delimiter);
	}
	SI_STOPIF(amount == 0, return SI_BUF_LEN((u8*)nil, 0));

	isize len = amount + 1;
	siArray(siString) res = si_arrayMakeReserve(si_sizeof(siString), len, len, alloc);
	siString* data = si_arrayGetType(res, 0, siString);

	isize lineStart = 0;
	for_range (i, 0, amount) {
		siString subStr = si_substrEnd(string, lineStart);
		subStr.len = si_stringFind(subStr, delimiter);

		data[i] = subStr;
		lineStart += subStr.len + delimiter.len;
	}

	if (string.len == lineStart) {
		res.len -= 1;
		return SI_BUF_ARR(res);
	}
	data[amount] = si_substrEnd(string, lineStart);

	return SI_BUF_ARR(res);
}


#ifdef SI_IMPLEMENTATION_MEMORY

inline isize si_memcopyStr(void* dst, siString src) { return si_memcopy(dst, src.data, src.len); }
inline isize si_memmoveStr(void* dst, siString src) { return si_memmove(dst, src.data, src.len); }
inline i32 si_memcompareStr(const void* dst, siString src) { return si_memcompare(dst, src.data, src.len); }

inline isize si_memcopyStr_s(siBufferAny dst, siString src)  { return si_memcopy_s(dst, src.data, src.len); }
inline isize si_memmoveStr_s(siBufferAny dst, siString src) { return si_memmove_s(dst, src.data, src.len); }

#endif

SIDEF
siString si_stringUpper(siString string, siAllocator alloc) {
	siBuilder b = si_builderMake(string.len, alloc);

	siRune x;
	for_eachStr (x, string) {
		si_builderWriteRune(&b, si_runeUpper(x));
	}

	return si_builderToStr(b);
}

SIDEF
siString si_stringLower(siString string, siAllocator alloc) {
	siBuilder b = si_builderMake(string.len, alloc);

	siRune x;
	for_eachStr (x, string) {
		si_builderWriteRune(&b, si_runeLower(x));
	}

	return si_builderToStr(b);

}


const u8 SI_NUM_TO_CHAR_TABLE_UPPER[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"@$";
const u8 SI_NUM_TO_CHAR_TABLE_LOWER[] =
	"0123456789"
	"abcdefghijklmnopqrstuvwxyz"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"@$";
const u8* SI_NUM_TO_CHAR_TABLE = SI_NUM_TO_CHAR_TABLE_UPPER;

inline
void si_numChangeTable(b32 upper) {
	static const u8* choices[2] = {SI_NUM_TO_CHAR_TABLE_LOWER, SI_NUM_TO_CHAR_TABLE_UPPER};
	SI_NUM_TO_CHAR_TABLE = choices[upper & true];
}


inline
siString si_stringFromUInt(u64 num, siBuffer(u8) out) {
	return si_stringFromUIntEx(num, 10, out);
}
SIDEF
siString si_stringFromUIntEx(u64 num, i32 base, siBuffer(u8) out) {
	SI_ASSERT_NOT_NEG(base);

	isize len = si_min(isize, si_numLenEx(num, base), out.len);
	u8* res = si_bufferGetType(out, 0, u8);

	/* NOTE(EimaMei): We build the string from the back (not the front) so that
	 * we wouldn't have to reverse the string after we make the string. */
	isize i = len - 1;

	do {
		res[i] = SI_NUM_TO_CHAR_TABLE[num % (u32)base];
		num /= (u32)base;
		i -= 1;
	} while (num != 0);

	return SI_STR_LEN(res, len);
}

inline
u64 si_stringToUInt(siString string) {
	i32 tmp;
	return si_stringToUIntEx(string, &tmp);
}

inline
u64 si_stringToUIntEx(siString string, i32* outRes) {
	return si_stringToUIntBase(string, -1, outRes);
}

SIDEF
u64 si_stringToUIntBase(siString string, i32 base, i32* outRes) {
	SI_ASSERT_NOT_NIL(outRes);
	SI_STOPIF(string.len == 0, *outRes = -1; return 0);

	u64 res = 0;
	siRune r;
	for_eachStrEx (r, i, string) {
		if (!si_runeIsSpace(r)) {
			string = si_substrEnd(string, i);
			break;
		}
	}

	if (string.data[0] != '0' || base == 10) {
		SI_STOPIF(string.len > 20, *outRes = -2; return 0);

		for_eachStrEx (r, i, string) {
			if (!si_runeIsDigit(r)) {
				isize oldI = i;

				siUtf32Char tmp;
				while (i < string.len) {
					tmp = si_utf8Decode(&string.data[i]);
					SI_STOPIF(!si_runeIsSpace(tmp.codepoint), break);
					i += tmp.len;
				}

				*outRes = (i >= string.len) ? INT32_MAX : (i32)oldI;
				return res;
			}

			res *= 10;
			res += (u64)(r - '0');
		}


		*outRes = INT32_MAX;
		return res;
	}

	i32 maxDigits;
	if (base > -1) {
		switch (string.data[1]) {
			case 'X': base = 16; string = si_substrEnd(string, 2); maxDigits = 16 + 2; break;
			case 'O': base =  8; string = si_substrEnd(string, 2); maxDigits = 22 + 2; break;
			case 'B': base =  2; string = si_substrEnd(string, 2); maxDigits = 64 + 2; break;
			default:  base =  8; string = si_substrEnd(string, 1); maxDigits = 22 + 1; break;
		}
	}
	else {
		switch (base) {
			case 16: maxDigits = 16; break;
			case 8:  maxDigits = 22; break;
			case 2:  maxDigits = 64; break;
			default: SI_PANIC();
		}
	}
	SI_STOPIF(string.len > maxDigits, *outRes = -2; return res);


	for_eachStrEx (r, i, string) {
		res *= (u32)base;

		i32 value = r - '0';
		if (value < 10) {
			res += (u32)value;
		}
		else if (value != (' ' - '0')) {
			value = si_charUpper((char)value) - 7;
			if (value < base) {
				res += (u32)value;
			}
			else {
				*outRes = (i32)i;
				return res;
			}
		}
		else {
			isize oldI = i;

			siUtf32Char tmp;
			while (i < string.len) {
				tmp = si_utf8Decode(&string.data[i]);
				SI_STOPIF(!si_runeIsSpace(tmp.codepoint), break);
				i += tmp.len;
			}

			*outRes = (i >= string.len) ? INT32_MAX : (i32)oldI;
			return res;
		}
	}

	*outRes = INT32_MAX;
	return res;
}

inline
siString si_stringFromInt(i64 num, siBuffer(u8) out) {
	return si_stringFromIntEx(num, 10, out);
}
SIDEF
siString si_stringFromIntEx(i64 num, i32 base, siBuffer(u8) out) {
	SI_ASSERT_NOT_NEG(base);
	b32 isNegative = (num < 0);

	isize len = si_min(isize, si_numLenIntEx(num, base), out.len);
	u8* res = si_bufferGetType(out, 0, u8);

	/* NOTE(EimaMei): We build the string from the back (not the front) so that
	 * we wouldn't have to reverse the string after we make the string. */
	isize i = len - 1;

	if (isNegative) {
		num = -num;
		res[0] = '-';
	}

	do {
		res[i] = SI_NUM_TO_CHAR_TABLE[(u32)num % (u32)base];
		num /= base;
		i -= 1;
	} while (num > 0);

	return SI_STR_LEN(res, len);
}
inline
siString si_stringFromFloat(f64 num, siBuffer(u8) out) {
	return si_stringFromFloatEx(num, 10, 6, out);
}
SIDEF
siString si_stringFromFloatEx(f64 num, i32 base, i32 afterPoint, siBuffer(u8) out) {
	SI_ASSERT_NOT_NEG(afterPoint);

	{
		i32 isInf = si_float64IsInf(num);
		if (isInf) {
			siString str = (isInf == 1) ? SI_STR("inf") : SI_STR("-inf");
			isize len = si_memcopyStr_s(out, str);
			return SI_STR_LEN(out.data, len);
		}
		else if (si_float64IsNan(num)) {
			isize len = si_memcopyStr_s(out, SI_STR("nan"));
			return SI_STR_LEN(out.data, len);
		}
	}

	i32 isNegative;
	{
		union { f64 f; u64 n; } check = {num};
		isNegative = (check.n & SI_ENDIAN_VALUE(SI_BIT(63), SI_BIT(0))) != 0;
	   	check.n &= ~SI_ENDIAN_VALUE(SI_BIT(63), SI_BIT(0)); /* NOTE(EimaMei): A quick way of changing the minus to plus. */
		num = check.f;
	}


	isize baseLen = 0;
	f64 numWhole = (afterPoint != 0)
		? num
		: si_round(f64, num);
	do {
		numWhole /= base;
		baseLen += 1;
	} while (numWhole > 0.9999999999999999f); /* NOTE(EimaMei): How long can this be?? */


	isize len = isNegative + baseLen + (afterPoint != 0) + afterPoint;
	isize i = 0;
	u8* res = si_bufferGetType(out, 0, u8);

	if (isNegative) {
		res[i] = '-';
		i += 1;
	}

	i32 digit;
	do {
		numWhole *= base;
		digit = (i32)numWhole;
		numWhole -= digit;

		res[i] = SI_NUM_TO_CHAR_TABLE[(u32)digit % (u32)base];
		i += 1;

		baseLen -= 1;
	} while (baseLen > 0);

	if (afterPoint == 0) {
		return SI_STR_LEN(res, len);
	}

	res[i] = '.';
	i += 1;

	num -= si_floor(f64, num);
	while (afterPoint) {
		num *= base;

		i32 numInt = (i32)num;
		res[i] = SI_NUM_TO_CHAR_TABLE[(u32)numInt % (u32)base];
		i += 1;

		afterPoint -= 1;
		num -= (f64)numInt;
	}

	return SI_STR_LEN(res, len);
}


inline
siString si_stringFromBool(b32 boolean) {
	static const siString values[] = {SI_STRC("false"), SI_STRC("true")};
	return values[boolean & true];
}

SIDEF
b32 si_stringToBool(siString string) {
	/* TODO(EimaMei): Make this UTF-8 friendly. */

	SI_ASSERT_NOT_NIL(string.data);
	SI_STOPIF(string.len == 0 || (string.len != 1 && string.len != 4 && string.len != 5), return UINT32_MAX);

	if (string.len == 1) {
		switch (si_charLower(*(char*)string.data)) {
			case '1': case 't': return true;
			case '0': case 'f': return false;
			default: return UINT32_MAX;
		}
	}

	u8 str[5];
	for_range (i, 0, countof(str)) {
		str[i] = string.data[i] | SI_BIT(5);
	}

	u32 val = SI_TO_U32(str);

	if (string.len == 4 && val == SI_TO_U32("true")) {
		return true;
	}
	else if (val == SI_TO_U32("fals") && str[4] == 'e') {
		return false;
	}

	return UINT32_MAX;
}

inline
b32 si_stringHasPrefix(siString string, siString prefix) {
	return string.len >= prefix.len && (si_memcompareStr(string.data, prefix) == 0);
}

SIDEF
isize si_stringPrefixLen(siString string, siString prefix) {
	SI_STOPIF(prefix.len > string.len, return 0);

	isize count = 0;
	for_range (i, 0, string.len) {
		if (string.data[i] == prefix.data[count]) {
			count += 1;
			continue;
		}
		break;
	}

	return count;
}

inline
b32 si_stringHasSuffix(siString string, siString suffix) {
	return string.len >= suffix.len
		&& (si_memcompareStr(&string.data[string.len - suffix.len], suffix) == 0);
}
SIDEF
isize si_stringSuffixLen(siString string, siString suffix) {
	SI_STOPIF(suffix.len > string.len, return 0);

	isize count = suffix.len - 1;
	isize i;
	for (i = string.len - 1; i >= 0; i -= 1) {
		if (string.data[i] == suffix.data[count]) {
			count -= 1;
			continue;
		}
		break;
	}

	return (suffix.len - count) + 1;
}


SIDEF
siString si_stringFromBuffer(siBufferAny buffer, cstring fmt, siBuffer(u8) out) {
	SI_ASSERT_NOT_NIL(fmt);
	SI_ASSERT(out.typeSize == si_sizeof(u8));
	SI_STOPIF(out.len < 2, return SI_STR_EMPTY);

	isize argCount = 0;
	isize fmtLen = 0;
	i32 sizes[256] = {0};
	isize indexes[256];

	{
		indexes[argCount] = 0;
		argCount += 1;

		while (fmt[fmtLen] != '\0') {
			SI_STOPIF(fmt[fmtLen] != '%', fmtLen += 1; continue);

			indexes[argCount] = fmtLen;
			i32* size = &sizes[argCount];
back:
			fmtLen += 1;
			switch (fmt[fmtLen]) {
				case 'C': SI_PANIC();

				case 'h': {
					if (*size == 0) {
						if (fmt[fmtLen + 1]== 'h') {
							fmtLen += 1;
							*size = si_sizeof(u8);
						}
						else {
							*size = si_sizeof(u16);
						}
					}

					goto back;
				}

				case 'l': {
					if (*size == 0) {
						SI_STOPIF(fmt[fmtLen + 1] == 'l', fmtLen += 1);
						*size = si_sizeof(u64);
					}
					goto back;
				}

				case 't': case 'z':
					SI_STOPIF(*size == 0, *size = si_sizeof(usize));
					goto back;

				case 'j':
					SI_STOPIF(*size == 0, *size = 8);
					goto back;

				case 'f': case 'F': case 'a': case 'A': case 'e': case 'E': case 'g': case 'G':
					SI_STOPIF(*size == 0, *size = si_sizeof(f64));
					break;

				case 's': case 'p':
					SI_STOPIF(*size == 0, *size = si_sizeof(void*));
					break;

				case 'x': case 'X': case 'i': case 'O': case 'o': case 'b': case 'B':
				case 'u': case 'd':
					SI_STOPIF(*size == 0, *size = si_sizeof(u32));
					break;

				case 'c':
					SI_STOPIF(*size == 0, *size = si_sizeof(siRune));
					break;

				case 'S':
					SI_STOPIF(*size == 0, *size = si_sizeof(siString));
					break;

				case '%': case 'n':
					*size = 0;
					break;

				case '*': case '.': case '-': case '+': case ' ': case '0':
				case '1': case '2': case '3': case '4': case '5': case '6':
				case '7': case '8': case '9': case '#':
					goto back;

				default: continue;
			}

			argCount += 1;
		}
		SI_ASSERT_MSG(argCount <= countof(sizes), "The format string has surpassed the maximum amount of allowed modifiers.");
	}

	si_bufferSet(out, 0, "{");
	isize length = 1;

	for_range (i, 0, buffer.len) {

		isize baseLen = 0;
		for_range (j, 0, argCount) {
			i32 size = sizes[j];
			isize index = indexes[j];
			isize nextIndex = (j < argCount - 1 ? indexes[j + 1] : fmtLen);

			siString substr = SI_STR_LEN(&fmt[index], nextIndex - index);
			void* base = (u8*)si_bufferGet(buffer, i) + baseLen;
			baseLen += size;

			switch (size) {
				case 0:  length += si_bprintfStr(si_sliceEnd(out, length), substr).len; break;
				case 1:  length += si_bprintfStr(si_sliceEnd(out, length), substr, *(u8*)base).len; break;
				case 2:  length += si_bprintfStr(si_sliceEnd(out, length), substr, *(u16*)base).len; break;
				case 4:  length += si_bprintfStr(si_sliceEnd(out, length), substr, *(u32*)base).len; break;
				case 8:  length += si_bprintfStr(si_sliceEnd(out, length), substr, *(u64*)base).len; break;
				case 16: length += si_bprintfStr(si_sliceEnd(out, length), substr, *(siString*)base).len; break;
				default: SI_PANIC();
			}
		}

		if (i != buffer.len - 1) {
			length += si_memcopyStr_s(si_sliceEnd(out, length), SI_STR(", "));
		}
		SI_STOPIF(length >= out.len, return SI_STR_LEN(out.data, length));
	}

	if (length < out.len) {
		si_bufferSet(out, length, "}");
		length += 1;
	}

	return SI_STR_LEN(out.data, length);
}

inline
siOsString si_stringToOsStr(siString str, siBuffer(siOsChar) out) {
	isize copied;
	return si_stringToOsStrEx(str, out, &copied);
}


SIDEF
siOsString si_stringToOsStrEx(siString str, siBuffer(siOsChar) out, isize* copied) {
	SI_ASSERT(out.typeSize == si_sizeof(siOsChar));
	SI_ASSERT_NOT_NIL(copied);

#ifdef SI_SYSTEM_IS_WINDOWS
	SI_STOPIF(out.len == 0, *copied = -1; return nil);

	siUtf16String res = si_utf8ToUtf16StrEx(str, true, out);
	*copied = res.len;

	return res.data;

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	SI_STOPIF(out.len == 0, *copied = -1; return nil);

	isize len = si_memcopyStr_s(si_sliceStart(out, out.len - 1), str);
	si_bufferSet(out, len, "\0");
	*copied = len + 1;

	return out.data;

#else
	*copied = 0;
	return str.data;
#endif
}



SIDEF
i64 si_stringToInt(siString string) {
	SI_ASSERT_NOT_NIL(string.data);

	switch (*(u8*)string.data) {
		case '-':
			string = si_substrEnd(string, 1);
			return -(i64)si_stringToUInt(string);
		case '+':
			string = si_substrEnd(string, 1);
			break;
	}

	return (i64)si_stringToUInt(string);
}

inline
i64 si_stringToIntEx(siString string, i32* outRes) {
	return si_stringToIntBase(string, -1, outRes);
}

SIDEF
i64 si_stringToIntBase(siString string, i32 base, i32* outRes) {
	SI_ASSERT_NOT_NIL(string.data);

	switch (*(u8*)string.data) {
		case '-':
			string.data = (u8*)string.data + 1;
			string.len -= 1;

			return -(i64)si_stringToUIntBase(string, base, outRes);
		case '+':
			string.data = (u8*)string.data + 1;
			string.len  -= 1;
			break;
	}

	return (i64)si_stringToUIntBase(string, base, outRes);
}

#endif /* SI_IMPLEMENTATION_STRING */

#ifdef SI_IMPLEMENTATION_UNICODE

#define FAILURE 12

SIDEF
siUtf32Char si_utf8Decode(const u8* character) {
	u32 state = 0;
	i32 codepoint;
	const u8* next = character;

	do {
		/* Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de> */
		/* See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details. */
		static const u8 utf8d[] = {
			/* The first part of the table maps bytes to character classes that */
			/* to reduce the size of the transition table and create bitmasks. */
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
			7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
			8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
			10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,
			/* The second part is a transition table that maps a combination */
			/* of a state of the automaton and a character class to a state. */
			0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
			12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
			12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
			12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
			12,36,12,12,12,12,12,12,12,12,12,12,
		 };

		u8 byte = *next;
		u32 type = utf8d[byte];

		codepoint = (state != 0)
			? ((byte & 0x3F) | (codepoint << 6))
			: ((0xFF >> type) & byte);
		state = utf8d[256 + state + type];

		next += 1;
	} while (state != 0 && state != FAILURE);

	if (state == FAILURE) {
		return SI_UNICODE_INVALID_UTF32;
	}

	siUtf32Char res;
	res.codepoint = codepoint;
	res.len = (i32)si_pointerDiff(character, next);

	return res;
}
#undef FAILURE

SIDEF
siUtf8Char si_utf8Encode(i32 codepoint) {
	siUtf8Char res;

	if (codepoint <= 0x7F) {
		res.codepoint[0] = (u8)codepoint;
		res.len = 1;
		return res;
	}
	else if (codepoint <= 0x7FF) {
		res.codepoint[0] = (u8)(0xC0 | (codepoint >> 6));
		res.codepoint[1] = (u8)(0x80 | (codepoint & 0x3F));
		res.len = 2;
	}
	else if (codepoint <= 0xFFFF) {
		if (si_between(i32, codepoint, 0xD800, 0xDFFF)) {
			return SI_UNICODE_INVALID_UTF8;
		}
		res.codepoint[0] = (u8)(0xE0 | (codepoint >> 12));
		res.codepoint[1] = (u8)(0x80 | ((codepoint >> 6) & 0x3F));
		res.codepoint[2] = (u8)(0x80 | (codepoint & 0x3F));
		res.len = 3;
	}
	else if (codepoint <= 0x10FFFF) {
		res.codepoint[0] = (u8)(0xF0 | (codepoint >> 18));
		res.codepoint[1] = (u8)(0x80 | ((codepoint >> 12) & 0x3F));
		res.codepoint[2] = (u8)(0x80 | ((codepoint >> 6) & 0x3F));
		res.codepoint[3] = (u8)(0x80 | (codepoint & 0x3F));
		res.len = 4;
	}
	else return SI_UNICODE_INVALID_UTF8;


	return res;
}

inline
siUtf16String si_utf8ToUtf16Str(siUtf8String str, siBuffer(u16) out) {
	return si_utf8ToUtf16StrEx(str, false, out);
}

SIDEF
siUtf16String si_utf8ToUtf16StrEx(siUtf8String str, b32 nullTerm, siBuffer(u16) out) {
	SI_STOPIF(str.len == 0 || out.len == 0, return SI_BUF_LEN((u16*)nil, 0));

	isize capacity = out.len - (i32)(nullTerm & true);
	u16* data = si_bufferGetType(out, 0, u16);

	isize inpI = 0, outI = 0;
	while (inpI < str.len) {
		siUtf32Char utf32 = si_utf8Decode(&str.data[inpI]);

		i32 codepoint = utf32.codepoint;
		inpI += utf32.len;

		if (codepoint < 0xFFFF) {
			SI_STOPIF(outI + 1 >= capacity, break);

			data[outI] = (u16)codepoint;
			outI += 1;
		}
		else {
			SI_STOPIF(outI + 2 >= capacity, break);

			i32 t = codepoint - 0x10000;
			data[outI + 0] = (u16)((t << 10) + 0xD800),
			data[outI + 1] = (u16)(t + 0xDC00);
			outI += 2;
		}
	}

	if (nullTerm) {
		data[outI] = '\0';
		outI+= 1;
	}

	return SI_BUF_LEN(data, outI);
}

inline
siUtf8String si_utf16ToUtf8Str(siUtf16String str, siBuffer(u8) out) {
	return si_utf16ToUtf8StrEx(str, false, out);
}

SIDEF
siUtf8String si_utf16ToUtf8StrEx(siUtf16String str, b32 nullTerm, siBuffer(u8) out) {
	SI_STOPIF(str.len == 0 || out.len == 0, return SI_STR_EMPTY);

	isize capacity = out.len - (i32)(nullTerm & true);
	isize inpI = 0, outI = 0;
	u8* data = si_bufferGetType(out, 0, u8);

	while (inpI < str.len) {
		siUtf8Char utf8 = si_utf16Encode(si_bufferGet(str, inpI));
		if (outI + utf8.len >= capacity) {
			break;
		}

		outI += si_memcopy(&data[outI], utf8.codepoint, utf8.len);
		inpI += 1 + (utf8.len == 4);
	}

	if (nullTerm) {
		data[outI] = '\0';
		outI += 1;
	}

	return SI_STR_LEN(data, outI);
}

SIDEF
isize si_utf16ToUtf8StrLen(siUtf16String string) {
	SI_ASSERT(string.typeSize == si_sizeof(u16));
	isize count = 0;
	isize i = 0;

	while (i < string.len) {
		siUtf32Char character = si_utf16Decode(si_bufferGet(string, i));
		count += character.len;
		i += 1 + (character.len == 4);
	}

	return count;
}

SIDEF
siUtf32Char si_utf16Decode(const u16 character[2]) {
	siUtf32Char res;
	i32 chr = character[0];

	if (!si_between(i32, chr, 0xD800, 0xDBFF)) {
		res.codepoint = chr;
		res.len = 1 + (chr >= 0x80) + (chr >= 0x800);
	}
	else {
		i32 high = (chr - 0xD800) << 10,
			low = character[1] - 0xDC00;

		res.codepoint = (high | low) + 0x10000;
		res.len = 4;
	}

	return res;
}

SIDEF
siUtf8Char si_utf16Encode(const u16 character[2]) {
	siUtf8Char res;
	u8* data = res.codepoint;
	u32 chr = character[0];

	if (chr <= 0xFF) {
		data[0] = (u8)chr;
		res.len = 1;
	}
	else if (chr <= 0x7FF) {
		data[0] = (u8)(0xC0 | (chr >> 6));
		data[1] = (u8)(0x80 | (chr & 0x3F));
		res.len = 2;
	}
	else if (chr <= 0xD7FF) {
		data[0] = (u8)(0xE0 | (chr >> 12));           /* 1110xxxx */
		data[1] = (u8)(0xE0 | ((chr >> 6) & 0x3F));   /* 10xxxxxx */
		data[2] = (u8)(0xE0 | (chr & 0x3F));          /* 10xxxxxx */
		res.len = 3;
	}
	else if (chr >= 0xD800) {
		u32 high = (chr - 0xD800u) << 10,
			low  = character[1] - 0xDC00u;
		u32 codepoint = (high | low) + 0x10000;

		data[0] = (u8)(0xF0 | (codepoint >> 18));           /* 11110xxx */
		data[1] = (u8)(0x80 | ((codepoint >> 12) & 0x3F));  /* 10xxxxxx */
		data[2] = (u8)(0x80 | ((codepoint >> 6) & 0x3F));   /* 10xxxxxx */
		data[3] = (u8)(0x80 | (codepoint & 0x3F));          /* 10xxxxxx */
		res.len = 4;
	}

	return res;
}

#endif /* SI_IMPLEMENTATION_UNICODE */

#ifdef SI_IMPLEMENTATION_CHAR

#if 1
#define SI__CTRL SI_BIT(0) /* Control characters. */
#define SI__PUNC SI_BIT(1) /* Punctuation characters. */
#define SI__NUME SI_BIT(2) /* Numeric digits. */
#define SI__SPAC SI_BIT(3) /* Space characters. */
#define SI__LETU SI_BIT(4) /* Uppercase letters. */
#define SI__LETL SI_BIT(5) /* Lowercase letters. */
#define SI__HEXA SI_BIT(6) /* Hexadecimal characters. */

#define SI__LTXU (SI__LETU | SI__HEXA)
#define SI__LTXL (SI__LETL | SI__HEXA)
#define SI__CTRS (SI__CTRL | SI__SPAC)
#define SI__NUMX (SI__NUME | SI__HEXA)

const u8 characterTraits[SI_ASCII_MAX + 1] = {
	SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL,

	SI__CTRS, SI__CTRS, SI__CTRS, SI__CTRS, SI__CTRS,

	SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL,
	SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL,

	SI__SPAC,

	SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC,
	SI__PUNC, SI__PUNC, SI__PUNC,

	SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX,

	SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC,

	SI__LTXU, SI__LTXU, SI__LTXU, SI__LTXU, SI__LTXU, SI__LTXU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU,
	SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU,
	SI__LETU, SI__LETU,

	SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC,

	SI__LTXL, SI__LTXL, SI__LTXL, SI__LTXL, SI__LTXL, SI__LTXL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL,
	SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL,
	SI__LETL, SI__LETL,

	SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC,
	SI__CTRL
};

const u16 alphaRanges[] = {
	0x00D8, 0x00F6, 0x00F8, 0x01F5, 0x0250, 0x02A8, 0x038E, 0x03A1, 0x03A3, 0x03CE, 0x03D0, 0x03D6, 0x03E2, 0x03F3, 0x0490,
	0x04C4, 0x0561, 0x0587, 0x05D0, 0x05EA, 0x05F0, 0x05F2, 0x0621, 0x063A, 0x0640, 0x064A, 0x0671, 0x06B7, 0x06BA, 0x06BE,
	0x06C0, 0x06CE, 0x06D0, 0x06D3, 0x0905, 0x0939, 0x0958, 0x0961, 0x0985, 0x098C, 0x098F, 0x0990, 0x0993, 0x09A8, 0x09AA,
	0x09B0, 0x09B6, 0x09B9, 0x09DC, 0x09DD, 0x09DF, 0x09E1, 0x09F0, 0x09F1, 0x0A05, 0x0A0A, 0x0A0F, 0x0A10, 0x0A13, 0x0A28,
	0x0A2A, 0x0A30, 0x0A32, 0x0A33, 0x0A35, 0x0A36, 0x0A38, 0x0A39, 0x0A59, 0x0A5C, 0x0A85, 0x0A8B, 0x0A8F, 0x0A91, 0x0A93,
	0x0AA8, 0x0AAA, 0x0AB0, 0x0AB2, 0x0AB3, 0x0AB5, 0x0AB9, 0x0B05, 0x0B0C, 0x0B0F, 0x0B10, 0x0B13, 0x0B28, 0x0B2A, 0x0B30,
	0x0B32, 0x0B33, 0x0B36, 0x0B39, 0x0B5C, 0x0B5D, 0x0B5F, 0x0B61, 0x0B85, 0x0B8A, 0x0B8E, 0x0B90, 0x0B92, 0x0B95, 0x0B99,
	0x0B9A, 0x0B9E, 0x0B9F, 0x0BA3, 0x0BA4, 0x0BA8, 0x0BAA, 0x0BAE, 0x0BB5, 0x0BB7, 0x0BB9, 0x0C05, 0x0C0C, 0x0C0E, 0x0C10,
	0x0C12, 0x0C28, 0x0C2A, 0x0C33, 0x0C35, 0x0C39, 0x0C60, 0x0C61, 0x0C85, 0x0C8C, 0x0C8E, 0x0C90, 0x0C92, 0x0CA8, 0x0CAA,
	0x0CB3, 0x0CB5, 0x0CB9, 0x0CE0, 0x0CE1, 0x0D05, 0x0D0C, 0x0D0E, 0x0D10, 0x0D12, 0x0D28, 0x0D2A, 0x0D39, 0x0D60, 0x0D61,
	0x0E01, 0x0E30, 0x0E32, 0x0E33, 0x0E40, 0x0E46, 0x0E5A, 0x0E5B, 0x0E81, 0x0E82, 0x0E87, 0x0E88, 0x0E94, 0x0E97, 0x0E99,
	0x0E9F, 0x0EA1, 0x0EA3, 0x0EAA, 0x0EAB, 0x0EAD, 0x0EAE, 0x0EB2, 0x0EB3, 0x0EC0, 0x0EC4, 0x0EDC, 0x0EDD, 0x0F18, 0x0F19,
	0x0F40, 0x0F47, 0x0F49, 0x0F69, 0x10D0, 0x10F6, 0x1100, 0x1159, 0x115F, 0x11A2, 0x11A8, 0x11F9, 0x1E00, 0x1E9B, 0x1F50,
	0x1F57, 0x1F80, 0x1FB4, 0x1FB6, 0x1FBC, 0x1FC2, 0x1FC4, 0x1FC6, 0x1FCC, 0x1FD0, 0x1FD3, 0x1FD6, 0x1FDB, 0x1FE0, 0x1FEC,
	0x1FF2, 0x1FF4, 0x1FF6, 0x1FFC, 0x210A, 0x2113, 0x2115, 0x211D, 0x2120, 0x2122, 0x212A, 0x2131, 0x2133, 0x2138, 0x3041,
	0x3094, 0x30A1, 0x30FA, 0x3105, 0x312C, 0x3131, 0x318E, 0x3192, 0x319F, 0x3260, 0x327B, 0x328A, 0x32B0, 0x32D0, 0x32FE,
	0x3300, 0x3357, 0x3371, 0x3376, 0x337B, 0x3394, 0x3399, 0x339E, 0x33A9, 0x33AD, 0x33B0, 0x33C1, 0x33C3, 0x33C5, 0x33C7,
	0x33D7, 0x33D9, 0x33DD, 0x4E00, 0x9FFF, 0xAC00, 0xD7A3, 0xF900, 0xFB06, 0xFB13, 0xFB17, 0xFB1F, 0xFB28, 0xFB2A, 0xFB36,
	0xFB38, 0xFB3C, 0xFB40, 0xFB41, 0xFB43, 0xFB44, 0xFB46, 0xFBB1, 0xFBD3, 0xFD3D, 0xFD50, 0xFD8F, 0xFD92, 0xFDC7, 0xFDF0,
	0xFDF9, 0xFE70, 0xFE72, 0xFE76, 0xFEFC, 0xFF66, 0xFF6F, 0xFF71, 0xFF9D, 0xFFA0, 0xFFBE, 0xFFC2, 0xFFC7, 0xFFCA, 0xFFCF,
	0xFFD2, 0xFFD7, 0xFFDA, 0xFFDC
};

const u16 alphaSingle[] = {
    0x00AA, 0x00B5, 0x00BA, 0x03DA, 0x03DC, 0x03DE, 0x03E0, 0x06D5, 0x09B2, 0x0A5E, 0x0A8D, 0x0AE0, 0x0B9C, 0x0CDE, 0x0E4F,
	0x0E84, 0x0E8A, 0x0E8D, 0x0EA5, 0x0EA7, 0x0EB0, 0x0EBD, 0x1FBE, 0x207F, 0x20A8, 0x2102, 0x2107, 0x2124, 0x2126, 0x2128,
	0xFB3E, 0xFE74
};

const u16 spaces[] = {
	0x200A, 0x200B, 0x200E, 0x200F, 0x2028, 0x2029, 0x3000, 0x202F, 0x205f, 0x3000, 0xFEFF
};

const u16 lowerRange[] = {
	0x00c0,  0x00d6, 532, 0x00d8,  0x00de, 532, 0x0189,  0x018a, 705, 0x018e,  0x018f, 702, 0x01b1,  0x01b2, 717,
	0x0388,  0x038a, 537, 0x038e,  0x038f, 563, 0x0391,  0x03a1, 532, 0x03a3,  0x03ab, 532, 0x0401,  0x040c, 580,
	0x040e,  0x040f, 580, 0x0410,  0x042f, 532, 0x0531,  0x0556, 548, 0x10a0,  0x10c5, 548, 0x1f08,  0x1f0f, 492,
	0x1f18,  0x1f1d, 492, 0x1f28,  0x1f2f, 492, 0x1f38,  0x1f3f, 492, 0x1f48,  0x1f4d, 492, 0x1f68,  0x1f6f, 492,
	0x1f88,  0x1f8f, 492, 0x1f98,  0x1f9f, 492, 0x1fa8,  0x1faf, 492, 0x1fb8,  0x1fb9, 492, 0x1fba,  0x1fbb, 426,
	0x1fc8,  0x1fcb, 414, 0x1fd8,  0x1fd9, 492, 0x1fda,  0x1fdb, 400, 0x1fe8,  0x1fe9, 492, 0x1fea,  0x1feb, 388,
	0x1ff8,  0x1ff9, 372, 0x1ffa,  0x1ffb, 374, 0x2160,  0x216f, 516, 0x24b6,  0x24cf, 526, 0xff21,  0xff3a, 532,
};

const u16 lowerSingle[] = {
	0x0100, 501, 0x0102, 501, 0x0104, 501, 0x0106, 501, 0x0108, 501, 0x010a, 501, 0x010c, 501, 0x010e, 501, 0x0110, 501,
	0x0112, 501, 0x0114, 501, 0x0116, 501, 0x0118, 501, 0x011a, 501, 0x011c, 501, 0x011e, 501, 0x0120, 501, 0x0122, 501,
	0x0124, 501, 0x0126, 501, 0x0128, 501, 0x012a, 501, 0x012c, 501, 0x012e, 501, 0x0130, 301, 0x0132, 501, 0x0134, 501,
	0x0136, 501, 0x0139, 501, 0x013b, 501, 0x013d, 501, 0x013f, 501, 0x0141, 501, 0x0143, 501, 0x0145, 501, 0x0147, 501,
	0x014a, 501, 0x014c, 501, 0x014e, 501, 0x0150, 501, 0x0152, 501, 0x0154, 501, 0x0156, 501, 0x0158, 501, 0x015a, 501,
	0x015c, 501, 0x015e, 501, 0x0160, 501, 0x0162, 501, 0x0164, 501, 0x0166, 501, 0x0168, 501, 0x016a, 501, 0x016c, 501,
	0x016e, 501, 0x0170, 501, 0x0172, 501, 0x0174, 501, 0x0176, 501, 0x0178, 379, 0x0179, 501, 0x017b, 501, 0x017d, 501,
	0x0181, 710, 0x0182, 501, 0x0184, 501, 0x0186, 706, 0x0187, 501, 0x018b, 501, 0x0190, 703, 0x0191, 501, 0x0193, 705,
	0x0194, 707, 0x0196, 711, 0x0197, 709, 0x0198, 501, 0x019c, 711, 0x019d, 713, 0x01a0, 501, 0x01a2, 501, 0x01a4, 501,
	0x01a7, 501, 0x01a9, 718, 0x01ac, 501, 0x01ae, 718, 0x01af, 501, 0x01b3, 501, 0x01b5, 501, 0x01b7, 719, 0x01b8, 501,
	0x01bc, 501, 0x01c4, 502, 0x01c5, 501, 0x01c7, 502, 0x01c8, 501, 0x01ca, 502, 0x01cb, 501, 0x01cd, 501, 0x01cf, 501,
	0x01d1, 501, 0x01d3, 501, 0x01d5, 501, 0x01d7, 501, 0x01d9, 501, 0x01db, 501, 0x01de, 501, 0x01e0, 501, 0x01e2, 501,
	0x01e4, 501, 0x01e6, 501, 0x01e8, 501, 0x01ea, 501, 0x01ec, 501, 0x01ee, 501, 0x01f1, 502, 0x01f2, 501, 0x01f4, 501,
	0x01fa, 501, 0x01fc, 501, 0x01fe, 501, 0x0200, 501, 0x0202, 501, 0x0204, 501, 0x0206, 501, 0x0208, 501, 0x020a, 501,
	0x020c, 501, 0x020e, 501, 0x0210, 501, 0x0212, 501, 0x0214, 501, 0x0216, 501, 0x0386, 538, 0x038c, 564, 0x03e2, 501,
	0x03e4, 501, 0x03e6, 501, 0x03e8, 501, 0x03ea, 501, 0x03ec, 501, 0x03ee, 501, 0x0460, 501, 0x0462, 501, 0x0464, 501,
	0x0466, 501, 0x0468, 501, 0x046a, 501, 0x046c, 501, 0x046e, 501, 0x0470, 501, 0x0472, 501, 0x0474, 501, 0x0476, 501,
	0x0478, 501, 0x047a, 501, 0x047c, 501, 0x047e, 501, 0x0480, 501, 0x0490, 501, 0x0492, 501, 0x0494, 501, 0x0496, 501,
	0x0498, 501, 0x049a, 501, 0x049c, 501, 0x049e, 501, 0x04a0, 501, 0x04a2, 501, 0x04a4, 501, 0x04a6, 501, 0x04a8, 501,
	0x04aa, 501, 0x04ac, 501, 0x04ae, 501, 0x04b0, 501, 0x04b2, 501, 0x04b4, 501, 0x04b6, 501, 0x04b8, 501, 0x04ba, 501,
	0x04bc, 501, 0x04be, 501, 0x04c1, 501, 0x04c3, 501, 0x04c7, 501, 0x04cb, 501, 0x04d0, 501, 0x04d2, 501, 0x04d4, 501,
	0x04d6, 501, 0x04d8, 501, 0x04da, 501, 0x04dc, 501, 0x04de, 501, 0x04e0, 501, 0x04e2, 501, 0x04e4, 501, 0x04e6, 501,
	0x04e8, 501, 0x04ea, 501, 0x04ee, 501, 0x04f0, 501, 0x04f2, 501, 0x04f4, 501, 0x04f8, 501, 0x1e00, 501, 0x1e02, 501,
	0x1e04, 501, 0x1e06, 501, 0x1e08, 501, 0x1e0a, 501, 0x1e0c, 501, 0x1e0e, 501, 0x1e10, 501, 0x1e12, 501, 0x1e14, 501,
	0x1e16, 501, 0x1e18, 501, 0x1e1a, 501, 0x1e1c, 501, 0x1e1e, 501, 0x1e20, 501, 0x1e22, 501, 0x1e24, 501, 0x1e26, 501,
	0x1e28, 501, 0x1e2a, 501, 0x1e2c, 501, 0x1e2e, 501, 0x1e30, 501, 0x1e32, 501, 0x1e34, 501, 0x1e36, 501, 0x1e38, 501,
	0x1e3a, 501, 0x1e3c, 501, 0x1e3e, 501, 0x1e40, 501, 0x1e42, 501, 0x1e44, 501, 0x1e46, 501, 0x1e48, 501, 0x1e4a, 501,
	0x1e4c, 501, 0x1e4e, 501, 0x1e50, 501, 0x1e52, 501, 0x1e54, 501, 0x1e56, 501, 0x1e58, 501, 0x1e5a, 501, 0x1e5c, 501,
	0x1e5e, 501, 0x1e60, 501, 0x1e62, 501, 0x1e64, 501, 0x1e66, 501, 0x1e68, 501, 0x1e6a, 501, 0x1e6c, 501, 0x1e6e, 501,
	0x1e70, 501, 0x1e72, 501, 0x1e74, 501, 0x1e76, 501, 0x1e78, 501, 0x1e7a, 501, 0x1e7c, 501, 0x1e7e, 501, 0x1e80, 501,
	0x1e82, 501, 0x1e84, 501, 0x1e86, 501, 0x1e88, 501, 0x1e8a, 501, 0x1e8c, 501, 0x1e8e, 501, 0x1e90, 501, 0x1e92, 501,
	0x1e94, 501, 0x1ea0, 501, 0x1ea2, 501, 0x1ea4, 501, 0x1ea6, 501, 0x1ea8, 501, 0x1eaa, 501, 0x1eac, 501, 0x1eae, 501,
	0x1eb0, 501, 0x1eb2, 501, 0x1eb4, 501, 0x1eb6, 501, 0x1eb8, 501, 0x1eba, 501, 0x1ebc, 501, 0x1ebe, 501, 0x1ec0, 501,
	0x1ec2, 501, 0x1ec4, 501, 0x1ec6, 501, 0x1ec8, 501, 0x1eca, 501, 0x1ecc, 501, 0x1ece, 501, 0x1ed0, 501, 0x1ed2, 501,
	0x1ed4, 501, 0x1ed6, 501, 0x1ed8, 501, 0x1eda, 501, 0x1edc, 501, 0x1ede, 501, 0x1ee0, 501, 0x1ee2, 501, 0x1ee4, 501,
	0x1ee6, 501, 0x1ee8, 501, 0x1eea, 501, 0x1eec, 501, 0x1eee, 501, 0x1ef0, 501, 0x1ef2, 501, 0x1ef4, 501, 0x1ef6, 501,
	0x1ef8, 501, 0x1f59, 492, 0x1f5b, 492, 0x1f5d, 492, 0x1f5f, 492, 0x1fbc, 491, 0x1fcc, 491, 0x1fec, 493, 0x1ffc, 491,
};

const u16 upperRange[] = {
	0x00e0,  0x00f6, 468, 0x00f8,  0x00fe, 468, 0x0256,  0x0257, 295, 0x0258,  0x0259, 298, 0x028a,  0x028b, 283,
	0x03ad,  0x03af, 463, 0x03b1,  0x03c1, 468, 0x03c3,  0x03cb, 468, 0x03cd,  0x03ce, 437, 0x0430,  0x044f, 468,
	0x0451,  0x045c, 420, 0x045e,  0x045f, 420, 0x0561,  0x0586, 452, 0x1f00,  0x1f07, 508, 0x1f10,  0x1f15, 508,
	0x1f20,  0x1f27, 508, 0x1f30,  0x1f37, 508, 0x1f40,  0x1f45, 508, 0x1f60,  0x1f67, 508, 0x1f70,  0x1f71, 574,
	0x1f72,  0x1f75, 586, 0x1f76,  0x1f77, 600, 0x1f78,  0x1f79, 628, 0x1f7a,  0x1f7b, 612, 0x1f7c,  0x1f7d, 626,
	0x1f80,  0x1f87, 508, 0x1f90,  0x1f97, 508, 0x1fa0,  0x1fa7, 508, 0x1fb0,  0x1fb1, 508, 0x1fd0,  0x1fd1, 508,
	0x1fe0,  0x1fe1, 508, 0x2170,  0x217f, 484, 0x24d0,  0x24e9, 474, 0xff41,  0xff5a, 468,
};

const u16 upperSingle[] = {
	0x00ff, 621, 0x0101, 499, 0x0103, 499, 0x0105, 499, 0x0107, 499, 0x0109, 499, 0x010b, 499, 0x010d, 499, 0x010f, 499,
	0x0111, 499, 0x0113, 499, 0x0115, 499, 0x0117, 499, 0x0119, 499, 0x011b, 499, 0x011d, 499, 0x011f, 499, 0x0121, 499,
	0x0123, 499, 0x0125, 499, 0x0127, 499, 0x0129, 499, 0x012b, 499, 0x012d, 499, 0x012f, 499, 0x0131, 268, 0x0133, 499,
	0x0135, 499, 0x0137, 499, 0x013a, 499, 0x013c, 499, 0x013e, 499, 0x0140, 499, 0x0142, 499, 0x0144, 499, 0x0146, 499,
	0x0148, 499, 0x014b, 499, 0x014d, 499, 0x014f, 499, 0x0151, 499, 0x0153, 499, 0x0155, 499, 0x0157, 499, 0x0159, 499,
	0x015b, 499, 0x015d, 499, 0x015f, 499, 0x0161, 499, 0x0163, 499, 0x0165, 499, 0x0167, 499, 0x0169, 499, 0x016b, 499,
	0x016d, 499, 0x016f, 499, 0x0171, 499, 0x0173, 499, 0x0175, 499, 0x0177, 499, 0x017a, 499, 0x017c, 499, 0x017e, 499,
	0x017f, 200, 0x0183, 499, 0x0185, 499, 0x0188, 499, 0x018c, 499, 0x0192, 499, 0x0199, 499, 0x01a1, 499, 0x01a3, 499,
	0x01a5, 499, 0x01a8, 499, 0x01ad, 499, 0x01b0, 499, 0x01b4, 499, 0x01b6, 499, 0x01b9, 499, 0x01bd, 499, 0x01c5, 499,
	0x01c6, 498, 0x01c8, 499, 0x01c9, 498, 0x01cb, 499, 0x01cc, 498, 0x01ce, 499, 0x01d0, 499, 0x01d2, 499, 0x01d4, 499,
	0x01d6, 499, 0x01d8, 499, 0x01da, 499, 0x01dc, 499, 0x01df, 499, 0x01e1, 499, 0x01e3, 499, 0x01e5, 499, 0x01e7, 499,
	0x01e9, 499, 0x01eb, 499, 0x01ed, 499, 0x01ef, 499, 0x01f2, 499, 0x01f3, 498, 0x01f5, 499, 0x01fb, 499, 0x01fd, 499,
	0x01ff, 499, 0x0201, 499, 0x0203, 499, 0x0205, 499, 0x0207, 499, 0x0209, 499, 0x020b, 499, 0x020d, 499, 0x020f, 499,
	0x0211, 499, 0x0213, 499, 0x0215, 499, 0x0217, 499, 0x0253, 290, 0x0254, 294, 0x025b, 297, 0x0260, 295, 0x0263, 293,
	0x0268, 291, 0x0269, 289, 0x026f, 289, 0x0272, 287, 0x0283, 282, 0x0288, 282, 0x0292, 281, 0x03ac, 462, 0x03cc, 436,
	0x03d0, 438, 0x03d1, 443, 0x03d5, 453, 0x03d6, 446, 0x03e3, 499, 0x03e5, 499, 0x03e7, 499, 0x03e9, 499, 0x03eb, 499,
	0x03ed, 499, 0x03ef, 499, 0x03f0, 414, 0x03f1, 420, 0x0461, 499, 0x0463, 499, 0x0465, 499, 0x0467, 499, 0x0469, 499,
	0x046b, 499, 0x046d, 499, 0x046f, 499, 0x0471, 499, 0x0473, 499, 0x0475, 499, 0x0477, 499, 0x0479, 499, 0x047b, 499,
	0x047d, 499, 0x047f, 499, 0x0481, 499, 0x0491, 499, 0x0493, 499, 0x0495, 499, 0x0497, 499, 0x0499, 499, 0x049b, 499,
	0x049d, 499, 0x049f, 499, 0x04a1, 499, 0x04a3, 499, 0x04a5, 499, 0x04a7, 499, 0x04a9, 499, 0x04ab, 499, 0x04ad, 499,
	0x04af, 499, 0x04b1, 499, 0x04b3, 499, 0x04b5, 499, 0x04b7, 499, 0x04b9, 499, 0x04bb, 499, 0x04bd, 499, 0x04bf, 499,
	0x04c2, 499, 0x04c4, 499, 0x04c8, 499, 0x04cc, 499, 0x04d1, 499, 0x04d3, 499, 0x04d5, 499, 0x04d7, 499, 0x04d9, 499,
	0x04db, 499, 0x04dd, 499, 0x04df, 499, 0x04e1, 499, 0x04e3, 499, 0x04e5, 499, 0x04e7, 499, 0x04e9, 499, 0x04eb, 499,
	0x04ef, 499, 0x04f1, 499, 0x04f3, 499, 0x04f5, 499, 0x04f9, 499, 0x1e01, 499, 0x1e03, 499, 0x1e05, 499, 0x1e07, 499,
	0x1e09, 499, 0x1e0b, 499, 0x1e0d, 499, 0x1e0f, 499, 0x1e11, 499, 0x1e13, 499, 0x1e15, 499, 0x1e17, 499, 0x1e19, 499,
	0x1e1b, 499, 0x1e1d, 499, 0x1e1f, 499, 0x1e21, 499, 0x1e23, 499, 0x1e25, 499, 0x1e27, 499, 0x1e29, 499, 0x1e2b, 499,
	0x1e2d, 499, 0x1e2f, 499, 0x1e31, 499, 0x1e33, 499, 0x1e35, 499, 0x1e37, 499, 0x1e39, 499, 0x1e3b, 499, 0x1e3d, 499,
	0x1e3f, 499, 0x1e41, 499, 0x1e43, 499, 0x1e45, 499, 0x1e47, 499, 0x1e49, 499, 0x1e4b, 499, 0x1e4d, 499, 0x1e4f, 499,
	0x1e51, 499, 0x1e53, 499, 0x1e55, 499, 0x1e57, 499, 0x1e59, 499, 0x1e5b, 499, 0x1e5d, 499, 0x1e5f, 499, 0x1e61, 499,
	0x1e63, 499, 0x1e65, 499, 0x1e67, 499, 0x1e69, 499, 0x1e6b, 499, 0x1e6d, 499, 0x1e6f, 499, 0x1e71, 499, 0x1e73, 499,
	0x1e75, 499, 0x1e77, 499, 0x1e79, 499, 0x1e7b, 499, 0x1e7d, 499, 0x1e7f, 499, 0x1e81, 499, 0x1e83, 499, 0x1e85, 499,
	0x1e87, 499, 0x1e89, 499, 0x1e8b, 499, 0x1e8d, 499, 0x1e8f, 499, 0x1e91, 499, 0x1e93, 499, 0x1e95, 499, 0x1ea1, 499,
	0x1ea3, 499, 0x1ea5, 499, 0x1ea7, 499, 0x1ea9, 499, 0x1eab, 499, 0x1ead, 499, 0x1eaf, 499, 0x1eb1, 499, 0x1eb3, 499,
	0x1eb5, 499, 0x1eb7, 499, 0x1eb9, 499, 0x1ebb, 499, 0x1ebd, 499, 0x1ebf, 499, 0x1ec1, 499, 0x1ec3, 499, 0x1ec5, 499,
	0x1ec7, 499, 0x1ec9, 499, 0x1ecb, 499, 0x1ecd, 499, 0x1ecf, 499, 0x1ed1, 499, 0x1ed3, 499, 0x1ed5, 499, 0x1ed7, 499,
	0x1ed9, 499, 0x1edb, 499, 0x1edd, 499, 0x1edf, 499, 0x1ee1, 499, 0x1ee3, 499, 0x1ee5, 499, 0x1ee7, 499, 0x1ee9, 499,
	0x1eeb, 499, 0x1eed, 499, 0x1eef, 499, 0x1ef1, 499, 0x1ef3, 499, 0x1ef5, 499, 0x1ef7, 499, 0x1ef9, 499, 0x1f51, 508,
	0x1f53, 508, 0x1f55, 508, 0x1f57, 508, 0x1fb3, 509, 0x1fc3, 509, 0x1fe5, 507, 0x1ff3, 509,
};


siIntern
isize si__binarySearch(const u16* array, isize len, siRune target, i32 stride) {
	isize t = 0;
	while (len > 1) {
		len /= 2;
		isize p = t + len * stride;

		if (target >= array[p]) {
			t = p;
		}
	}

	return (len != 0 && target >= array[t]) ? t : -1;
}

#endif

SIDEF
siRune si_runeLower(siRune rune) {
	if (rune <= SI_ASCII_MAX) {
		return si_charLower((char)rune);
	}

	isize i;
	i = si__binarySearch(lowerRange, countof(lowerRange) / 3, rune, 3);
	if (i != -1 && si_between(i32, rune, lowerRange[i], lowerRange[i + 1])) {
		return rune + lowerRange[i + 2] - 500;
	}
	i = si__binarySearch(lowerSingle, countof(lowerSingle), rune, 1);
	if (i != -1 && rune == lowerSingle[i]) {
		return rune + lowerSingle[i + 1] - 500;
	}

	return rune;
}

SIDEF
siRune si_runeUpper(siRune rune) {
	if (rune <= SI_ASCII_MAX) {
		return si_charUpper((char)rune);
	}

	isize i;
	i = si__binarySearch(upperRange, countof(upperRange) / 3, rune, 3);
	if (i != -1 && si_between(i32, rune, upperRange[i], upperRange[i + 1])) {
		return rune + upperRange[i + 2] - 500;
	}
	i = si__binarySearch(upperSingle, countof(upperSingle) / 2, rune, 2);
	if (i != -1 && rune == upperSingle[i]) {
		return rune + upperSingle[i + 1] - 500;
	}

	return rune;
}


SIDEF
b32 si_runeIsLower(siRune rune) {
	if (rune <= SI_ASCII_MAX) {
		return si_charIsLower((char)rune);
	}

	isize i;
	i = si__binarySearch(upperRange, countof(upperRange) / 3, rune, 3);
	if (i != -1 && si_between(i32, rune, upperRange[i], upperRange[i + 1])) {
		return true;
	}
	i = si__binarySearch(upperSingle, countof(upperSingle), rune, 1);
	if (i != -1 && rune == upperSingle[i]) {
		return true;
	}

	return false;
}

SIDEF
b32 si_runeIsUpper(siRune rune) {
	if (rune <= SI_ASCII_MAX) {
		return si_charIsUpper((char)rune);
	}

	isize i;
	i = si__binarySearch(lowerRange, countof(lowerRange) / 3, rune, 3);
	if (i != -1 && si_between(i32, rune, lowerRange[i], lowerRange[i + 1])) {
		return true;
	}
	i = si__binarySearch(lowerSingle, countof(lowerSingle), rune, 1);
	if (i != -1 && rune == lowerSingle[i]) {
		return true;
	}

	return false;
}

SIDEF
b32 si_runeIsSpace(siRune rune) {
	if (rune <= 0x1680) {
		switch (rune) {
			case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
			case 0x85: case 0xA0: case 0x1680:
				return true;
		}
		return false;
	}
	else if (si_between(i32, rune, 0x2000, 0x200B)) {
		return true;
	}

	return si__binarySearch(spaces, countof(spaces), rune, 1) != -1;
}

inline
b32 si_runeIsDigit(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsDigit((char)rune) : false;
}

inline
b32 si_runeIsHex(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsHex((char)rune) : false;
}
SIDEF
b32 si_runeIsAlpha(siRune rune) {
	if (rune <= SI_ASCII_MAX) {
		return si_charIsAlpha((char)rune);
	}
	if (si_runeIsUpper(rune) || si_runeIsLower(rune)) {
		return true;
	}

	isize i;
	i = si__binarySearch(alphaRanges, countof(alphaRanges) / 2, rune, 2);
	if (i != -1 && si_between(i32, rune, alphaRanges[i], alphaRanges[i + 1])) {
		return true;
	}

	i = si__binarySearch(alphaSingle, countof(alphaSingle), rune, 1);
	if (i != -1 && rune == alphaRanges[i]) {
		return true;
	}

	return false;
}
inline
b32 si_runeIsAlphanumeric(siRune rune) {
	return si_runeIsDigit(rune) || si_runeIsAlpha(rune);
}
inline
b32 si_runeIsPunctuation(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsPrintable((char)rune) : false;
}
inline
b32 si_runeIsControl(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsPrintable((char)rune) : false;
}
inline
b32 si_runeIsPrintable(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsPrintable((char)rune) : false;
}
inline
b32 si_runeIsGraphical(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsGraphical((char)rune) : false;
}
inline
b32 si_runeIsDelimiter(siRune rune) {
	return !(si_runeIsAlphanumeric(rune) || rune == '@' || rune == '#' || rune == '$');

}

inline
i32 si_runeDigitToInt(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charDigitToInt((char)rune) : -1;
}
inline
i32 si_runeHexToInt(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charHexToInt((char)rune) : -1;
}


inline
char si_charLower(char c) {
	if (characterTraits[(u8)c] & SI__LETU) {
		return c | (char)SI_BIT(5);
	}
	return c;
}

inline
char si_charUpper(char c) {
	if (characterTraits[(u8)c] & SI__LETL) {
		return c & ~(char)SI_BIT(5);
	}
	return c;
}

inline
b32 si_charIsLower(char c) {
	return (characterTraits[(u8)c] & SI__LETL) != 0;
}

inline
b32 si_charIsUpper(char c) {
	return (characterTraits[(u8)c] & SI__LETU) != 0;
}

inline
b32 si_charIsSpace(char c) {
	return (characterTraits[(u8)c] & SI__SPAC) != 0;
}

inline
b32 si_charIsDigit(char c) {
	return (characterTraits[(u8)c] & SI__NUME) != 0;
}

inline
b32 si_charIsHex(char c) {
	return (characterTraits[(u8)c] & SI__HEXA) != 0;
}

inline
b32 si_charIsAlpha(char c) {
	return (characterTraits[(u8)c] & (SI__LETL | SI__LETU)) != 0;
}

inline
b32 si_charIsAlphanumeric(char c) {
	return (characterTraits[(u8)c] & (SI__LETL | SI__LETU | SI__NUME)) != 0;
}

inline
b32 si_charIsPunctuation(char c) {
	return (characterTraits[(u8)c] & SI__PUNC) != 0;
}

inline
b32 si_charIsControl(char c) {
	return (characterTraits[(u8)c] & SI__CTRL) != 0;
}

inline
b32 si_charIsPrintable(char c) {
	return (characterTraits[(u8)c] & (SI__LETL | SI__LETU | SI__NUME | SI__PUNC | SI__SPAC)) != 0;
}

inline
b32 si_charIsGraphical(char c) {
	return (characterTraits[(u8)c] & (SI__LETL | SI__LETU | SI__NUME | SI__PUNC)) != 0;
}


inline
b32 si_charIsDelimiter(char c) {
	return !(si_charIsAlphanumeric(c) || c == '@' || c == '#' || c == '$');
}

inline
i32 si_charDigitToInt(char c) {
	if (characterTraits[(u8)c] & SI__NUME) {
		return (c - '0');
	}

	return -1;
}
SIDEF
i32 si_charHexToInt(char c) {
	u8 trait = characterTraits[(u8)c];

	if (trait & SI__NUME) {
		return si_charDigitToInt(c);
	}
	else if ((trait & SI__LTXL) == SI__LTXL) {
		return c - 'a' + 10;
	}
	else if ((trait & SI__LTXU) == SI__LTXU) {
		return c - 'A' + 10;
	}

	return -1;
}

#undef SI__CTRL
#undef SI__PUNC
#undef SI__NUME
#undef SI__SPAC
#undef SI__LETU
#undef SI__LETL
#undef SI__HEXA

#undef SI__LTXU
#undef SI__LTXL
#undef SI__CTRS
#undef SI__NUMX

#endif /* SI_IMPLEMENTATION_CHAR */

#ifdef SI_IMPLEMENTATION_HASHTABLE

force_inline
u64 si__hashKey(u8* key, isize len);

#ifndef SI_USE_CUSTOM_HASH_FUNCTION
#define SI__FNV_OFFSET 14695981039346656037UL
#define SI__FNV_PRIME 1099511628211UL

force_inline
u64 si__hashKey(u8* key, isize len) {
	u64 hash = SI__FNV_OFFSET;

	for_range (i, 0, len) {
		hash ^= (u64)key[i];
		hash *= SI__FNV_PRIME;
	}
	return hash;
}
#undef SI__FNV_OFFSET
#undef SI__FNV_PRIME

#endif



SIDEF
siHashTable si_hashtableMake(const void** keyArray, isize keyLen, void** dataArray,
		isize len, siAllocator alloc) {
	SI_ASSERT_NOT_NIL(keyArray);
	SI_ASSERT_NOT_NIL(dataArray);
	SI_ASSERT(keyLen != 0);

	siHashTable table = si_hashtableMakeReserve(len, alloc);
	for_range (i, 0, len) {
		si_hashtableSet(&table, keyArray[i], keyLen, dataArray[i], nil);
	}

	return table;
}

SIDEF
siHashTable si_hashtableMakeReserve(isize capacity, siAllocator alloc) {
	/* NOTE(EimaMei): A power of 2 capacity is enforced so that truncating a hash
	 * into a valid index would only require a 'hash & (capacity - 1)', which
	 * basically takes no time in comparison to the usual modulo operator. */
	SI_ASSERT_MSG(si_isPowerOfTwo(capacity), "The specified capacity must be a power of 2 number.");

	siHashTable table = si_arrayMakeReserve(si_sizeof(siHashEntry), 0, capacity, alloc);
	si_memset(table.data, 0, si_sizeof(siHashEntry) * table.capacity);

	return table;
}


SIDEF
void* si_hashtableGet(siHashTable ht, const void* key, isize keyLen) {
	SI_ASSERT_NOT_NIL(key);
	SI_ASSERT_NOT_NEG(keyLen);
	SI_ASSERT(keyLen != 0);
	SI_STOPIF(ht.len == 0, return nil);

	u64 hash = si__hashKey((u8*)key, keyLen);
	return si_hashtableGetWithHash(ht, hash);
}

SIDEF
void* si_hashtableGetWithHash(siHashTable ht, u64 hash) {
	isize index = (isize)(hash & (usize)(ht.capacity - 1));

	siHashEntry* entry = si_arrayGetType(ht, index, siHashEntry);
	goto skip; /* NOTE(EimaMei):	We skip the 'entry->next' line so that the
									first entry can get checked. */

	while (entry->next != 0) {
		entry = entry->next;
skip:
		if (hash == entry->key) {
			return entry->value;
		}
	}

	return nil;
}


SIDEF
siHashEntry* si_hashtableSet(siHashTable* ht, const void* key, isize keyLen,
		void* valuePtr, b32* outSuccess) {
	SI_ASSERT_NOT_NIL(key);
	SI_ASSERT_NOT_NEG(keyLen);
	SI_ASSERT(keyLen != 0);

	u64 hash = si__hashKey((u8*)key, keyLen);
	return si_hashtableSetWithHash(ht, hash, valuePtr, outSuccess);
}

SIDEF
siHashEntry* si_hashtableSetWithHash(siHashTable* ht, u64 hash, void* valuePtr,
		b32* outSuccess) {
	SI_ASSERT_NOT_NIL(ht);
	SI_ASSERT_MSG(ht->len < ht->capacity, "The capacity of the hash table has been surpassed.");


	isize index = (isize)(hash & (usize)(ht->capacity - 1));
	siHashEntry* entry = si_arrayGetType(*ht, index, siHashEntry);
	siHashEntry* original = entry;

	while (entry->key != 0) {
		if (hash == entry->key) {
			SI_STOPIF(outSuccess != nil, *outSuccess = false);
			#ifndef SI_NO_HASH_OVERWRITE
				entry->value = valuePtr;
			#endif
			return entry;
		}

		entry = entry->next ? entry->next : (entry + 1);
	}
	SI_ASSERT(entry->key == 0);

	entry->key = hash;
	entry->value = valuePtr;

	if (entry != original) {
		while (original->next != 0) { original = original->next; }
		original->next = entry;
	}
	SI_STOPIF(outSuccess != nil, *outSuccess = true);

	ht->len += 1;
	return entry;
}

#endif /* SI_IMPLEMENTATION_HASHTABLE */

#ifdef SI_IMPLEMENTATION_SYSTEM
/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
	========================
	| siSystem             |
	========================
*/

SIDEF
SI_ERROR_PROC(si_systemErrorLog) {
	siPrintColor red = si_printColor3bitEx(siPrintColor3bit_Red, true, false);

#ifndef SI_NO_ERROR_STRUCT
	si_fprintf(
		si_stderr, "%CSystem error at \"%s:%i\"%C: %S: %S\n",
		red, error->filename, error->line,
		si_systemErrorName(error->code), si_systemErrorDesc(error->code)
	);

#else
	si_fprintf(
		si_stderr, "%CSystem error%C: %S: %S\n",
		red, si_systemErrorName(error->code), si_systemErrorDesc(error->code)
	);
#endif

	return true;
	SI_UNUSED(data);
}

inline
siString si_systemErrorName(siErrorSystem error) {
	SI_ASSERT_NOT_NEG(error);
	SI_ASSERT(error < siErrorSystem_Length);

	static const siString names[siErrorSystem_Length] = {
		SI_STRC("siErrorSystem_None"),

		SI_STRC("siErrorSystem_Exists"),
		SI_STRC("siErrorSystem_NotExists"),
		SI_STRC("siErrorSystem_Invalid"),
		SI_STRC("siErrorSystem_InvalidFilename"),
		SI_STRC("siErrorSystem_Permission"),
		SI_STRC("siErrorSystem_NoMemory"),
		SI_STRC("siErrorSystem_TruncationFail"),
		SI_STRC("siErrorSystem_Unavailable"),
		SI_STRC("siErrorSystem_Deadlock"),

		SI_STRC("siErrorSystem_Generic"),
	};

	return names[error];
}

inline
siString si_systemErrorDesc(siErrorSystem error) {
	SI_ASSERT_NOT_NEG(error);
	SI_ASSERT(error < siErrorSystem_Length);

	static const siString descriptions[siErrorSystem_Length] = {
		SI_STRC("No error has been encountered."),

		SI_STRC("File or directory already exists."),
		SI_STRC("File or directory doesn't exist."),
		SI_STRC("Provided value is invalid."),
		SI_STRC("Invalid filename."),
		SI_STRC("User doesn't have sufficient permissions."),
		SI_STRC("Out of memory."),
		SI_STRC("Failed to truncate the file."),
		SI_STRC("System has insufficient resources to complete the action."),
		SI_STRC("A thread deadlock occurred."),

		SI_STRC("An unspecified, platform-specific error occurred."),
	};
	return descriptions[error];
}

#ifdef SI_SYSTEM_IS_WASM

/* TODO(EimaMei): Add mutexes to this. */
__wasi_errno_t si__wasmError;

siIntern
__wasi_errno_t si__wasmGetLastError(void) {
	return si__wasmError;
}

siIntern
void si__wasmSetLastError(__wasi_errno_t error) {
	si__wasmError = error;
}

#endif

SIDEF
siErrorSystem si_systemGetError(void) {
#if SI_SYSTEM_IS_WINDOWS
	switch (GetLastError()) {
		case ERROR_NO_MORE_FILES:
		case ERROR_SUCCESS: return siErrorSystem_None;
		case ERROR_ALREADY_EXISTS: case ERROR_FILE_EXISTS: return siErrorSystem_Exists;
		case ERROR_INVALID_NAME: case ERROR_BAD_PATHNAME: return siErrorSystem_InvalidFilename;
		case ERROR_DIRECTORY: case ERROR_INVALID_ADDRESS: return siErrorSystem_Invalid;
		case ERROR_PATH_NOT_FOUND: case ERROR_FILE_NOT_FOUND: return siErrorSystem_NotExists;
		case ERROR_ACCESS_DENIED: return siErrorSystem_Permission;
		case ERROR_NOT_ENOUGH_MEMORY: return siErrorSystem_NoMemory;
	}
	return siErrorSystem_Generic;

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	switch (errno) {
		case 0: return siErrorSystem_None;
		case EEXIST: return siErrorSystem_Exists;
		case EINVAL: return siErrorSystem_Invalid;
		case EISDIR: case ENAMETOOLONG: return siErrorSystem_InvalidFilename;
		case ENOENT: return siErrorSystem_NotExists;
		case EPERM: case EACCES: return siErrorSystem_Permission;
		case ENOMEM: return siErrorSystem_NoMemory;
		case EAGAIN: return siErrorSystem_Unavailable;
		case EDEADLK: return siErrorSystem_Deadlock;
	}
	return siErrorSystem_Generic;

#elif SI_SYSTEM_IS_WASM
	switch (si__wasmGetLastError()) {
		case __WASI_ERRNO_SUCCESS: return siErrorSystem_None;
		case __WASI_ERRNO_INVAL: case __WASI_ERRNO_BADF: return siErrorSystem_Invalid;
		case __WASI_ERRNO_ISDIR: case __WASI_ERRNO_NAMETOOLONG: return siErrorSystem_InvalidFilename;
		case __WASI_ERRNO_EXIST: return siErrorSystem_Exists;
		case __WASI_ERRNO_NOENT: return siErrorSystem_NotExists;
		case __WASI_ERRNO_PERM: case __WASI_ERRNO_ACCES: return siErrorSystem_Permission;
		case __WASI_ERRNO_NOBUFS: return siErrorSystem_NoMemory;
		case __WASI_ERRNO_AGAIN: case __WASI_ERRNO_BUSY: return siErrorSystem_Unavailable;
		case __WASI_ERRNO_DEADLK: return siErrorSystem_Deadlock;
	}

	return siErrorSystem_Generic;

#else
	return 0;
#endif

}

siNoreturn inline
void si_exit(i32 code) {
#if SI_SYSTEM_IS_WINDOWS
	ExitProcess((u32)code);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	exit(code);
#elif SI_SYSTEM_IS_WASM
	__wasi_proc_exit((u32)code);
#endif
}


SIDEF
b32 si_envVarSet(siString name, siString value) {
	siOsChar stack[SI_PATH_MAX];
	siOsString_2x strs = si_pathToOSMul(name, value, stack, countof(stack));
	siOsString* paths = strs.v;


#if SI_SYSTEM_IS_WINDOWS
	return SetEnvironmentVariableW(paths[0], paths[1]) != 0;
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	return setenv(paths[0], paths[1], 1) == 0;
#else
	return false;
	SI_UNUSED(paths);
#endif
}

SIDEF
b32 si_envVarUnset(siString name) {
	siOsChar path[SI_PATH_MAX];
	si_pathToOS(name, path, countof(path));

#if SI_SYSTEM_IS_WINDOWS
	return SetEnvironmentVariableW(path, nil) != 0;
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	return unsetenv(path) == 0;
#else
	return false;
#endif
}


SIDEF
isize si_envVarGetLength(siString name) {
	siOsChar path[SI_PATH_MAX * 2];
	isize pathLen = si_pathToOS(name, path, countof(path));

#if SI_SYSTEM_IS_WINDOWS
	u32 len = GetEnvironmentVariableW(path, &path[pathLen], si_cast(u32, countof(path) - pathLen));
	SI_STOPIF(len == 0, return 0);

	return si_utf16ToUtf8StrLen(SI_BUF_LEN(&path[pathLen], len));
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	return si_cstrLen(getenv(path));
	SI_UNUSED(pathLen);
#else
	return -1;
	SI_UNUSED(pathLen);
#endif
}

SIDEF
siString si_envVarGetData(siString name, siBuffer(u8) out) {
	SI_ASSERT(out.typeSize == sizeof(u8));

	siOsChar path[SI_PATH_MAX * 2];
	isize pathLen = si_pathToOS(name, path, countof(path));

#if SI_SYSTEM_IS_WINDOWS
	u32 len = GetEnvironmentVariableW(path, &path[pathLen], si_cast(u32, countof(path) - pathLen));
	SI_STOPIF(len == 0, return SI_STR_NIL);

	return si_utf16ToUtf8Str(SI_BUF_LEN(&path[pathLen], len), out);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	char* data = getenv(path);
	SI_STOPIF(data == nil, return SI_STR_NIL);

	isize len = si_memcopyStr_s(out, SI_CSTR(data));

	return SI_STR_LEN(out.data, len);
	SI_UNUSED(pathLen);

#else
	return SI_STR_NIL;
	SI_UNUSED(pathLen);
#endif
}

SIDEF SIDEF_WINDOWS
siWindowsVersion si_windowsGetVersion(void) {
#if SI_SYSTEM_IS_WINDOWS
	OSVERSIONINFOEXW info;
	{
		siDllHandle ntdll = si_dllLoad("ntdll.dll");

		typedef LONG(WINAPI* RtlGetVersionProc)(POSVERSIONINFOEXW);
		RtlGetVersionProc _RtlGetVersion = si_transmute(RtlGetVersionProc, si_dllProcAddress(ntdll, "RtlGetVersion"), siDllProc);

		info.dwOSVersionInfoSize = si_sizeof(OSVERSIONINFOEXW);
		_RtlGetVersion(&info);
		si_dllUnload(ntdll);
	}

	/* Windows 10 and 11. */
	if (info.dwMajorVersion == 10 && info.dwMinorVersion == 0) {
		return (info.dwBuildNumber >= 22000) ? siWindowsVersion_11 : siWindowsVersion_10;
	}

	/* Windows 8.1, 8, 7 and Vista. */
	if (info.dwMajorVersion == 6) {
		return siWindowsVersion_Vista + (i32)info.dwMinorVersion;
	}

	return (info.dwMajorVersion == 5 && (info.dwMinorVersion == 1 || info.dwMinorVersion == 2))
		? siWindowsVersion_XP
		: 0;
#else
	SI_PANIC_MSG("Unavailable function for this platform.");
#endif
}

SIDEF SIDEF_UNIX
b32 si_unixIsWayland(void) {
#if SI_SYSTEM_IS_UNIX
	static b32 isWayland = UINT32_MAX;
	SI_STOPIF(isWayland != UINT32_MAX, return isWayland);

	siString res = si_envVarGetData(SI_STR("WAYLAND_DISPLAY"), SI_BUF_STACK(1));
	isWayland = (res.data != nil);

	return isWayland;
#else
	SI_PANIC_MSG("Unavailable function for this platform.");
#endif
}

inline SIDEF_UNIX
b32 si_unixIsX11(void) {
#if SI_SYSTEM_IS_UNIX
	return !si_unixIsWayland();
#else
	SI_PANIC_MSG("Unavailable function for this platform.");
#endif
}

SIDEF SIDEF_UNIX
siUnixDE si_unixGetDE(void) {
#if SI_SYSTEM_IS_UNIX
	static siUnixDE de = -1;
	SI_STOPIF(de != -1, return de);

	siString res = si_envVarGetData(SI_STR("XDG_CURRENT_DESKTOP"), SI_BUF_STACK(8));
	SI_STOPIF(res.data == nil, return 0);

	/* TODO(EimaMei): Hashtable? */
	if (si_stringEqual(res, SI_STR("KDE"))) { de = siUnixDE_KDE; }
	else if (si_stringEqual(res, SI_STR("GNOME"))) { de = siUnixDE_GNOME; }
	else if (si_stringEqual(res, SI_STR("XFCE"))) { de = siUnixDE_Xfce; }
	else { de = 0; }

	return de;
#else
	SI_PANIC_MSG("Unavailable function for this platform.");
#endif

}

#endif /* SI_IMPLEMENTATION_SYSTEM */

#ifdef SI_IMPLEMENTATION_IO

#if SI_COMPILER_MSVC
	#pragma comment(lib, "shell32")
	#pragma comment(lib, "Advapi32")
#endif

inline
b32 si_pathExists(siString path) {
	siOsChar stack[SI_PATH_MAX];
	si_pathToOS(path, stack, countof(stack));
	return si_pathExistsOS(stack);
}
inline
b32 si_pathExistsOS(siOsString path) {
#if SI_SYSTEM_IS_WINDOWS
	return GetFileAttributesW(path) != INVALID_FILE_ATTRIBUTES;
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	struct stat tmp;
	return stat(path, &tmp) == 0;
#else
	return false;
	SI_UNUSED(path);
#endif
}
SIDEF
siResult(isize) si_pathCopy(siString pathSrc, siString pathDst) {
	siOsChar stack[SI_PATH_MAX * 2];
	siOsString_2x strs = si_pathToOSMul(pathSrc, pathDst, stack, countof(stack));
	siOsString* paths = strs.v;


#if SI_SYSTEM_IS_WINDOWS
	isize size = CopyFileW(paths[0], paths[1], true);
	SI_OPTION_SYS_CHECK(size == 0, isize);

	return SI_OPT(isize, size);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	int src = open(paths[0], O_RDONLY, 0);
	SI_OPTION_SYS_CHECK(src == -1, isize);

	int dst = open(paths[1], O_WRONLY | O_CREAT, 0666);
	SI_OPTION_SYS_CHECK(dst == -1, isize);

	struct stat stat;
	int res = fstat(dst, &stat);
	SI_OPTION_SYS_CHECK(res == -1, isize);

	#if SI_SYSTEM_IS_UNIX
		isize size = sendfile64(dst, src, 0, (usize)stat.st_size);
	#else
		isize size = sendfile(dst, src, 0, &stat.st_size, nil, 0);
	#endif
	SI_OPTION_SYS_CHECK(size == -1, isize);

	close(src);
	close(dst);

	return SI_OPT(isize, size);
#else
	return SI_OPT_NIL(isize);
	SI_UNUSED(paths);

#endif

}
SIDEF
siResult(i32) si_pathItemsCopy(siString pathSrc, siString pathDst) {
	/* TODO(EimaMei): Rework this mess one day. */
	SI_ASSERT(pathDst.len <= SI_PATH_MAX);

	siDirectory dir = si_directoryOpen(pathSrc);
	i32 itemsCopied = 0;

	u8 dst[SI_PATH_MAX];
	u8* dstBuffer;
	siString dstStr = SI_STR_LEN(dst, pathDst.len + 1);

	si_memcopy(dst, pathDst.data, pathDst.len);
	dst[pathDst.len] = SI_PATH_SEPARATOR;
	dstBuffer = &dst[pathDst.len + 1];

	siDirectoryEntry entry;
	while (si_directoryPollEntryEx(&dir, false, &entry)) {
		siString path = si_pathBaseName(entry.path);
		si_memcopyStr(dstBuffer, path);

		if (entry.type == siIoType_Directory) {
			siError err = si_pathCreateFolder(dstStr);
			SI_STOPIF(err.code != 0, return SI_OPT_ERR(i32, err));

			siResult(i32) res = si_pathItemsCopy(entry.path, dstStr);
			SI_STOPIF(!res.hasValue, return res);
			itemsCopied += res.data.value;
			continue;
		}

		si_pathCopy(entry.path, dstStr);
		itemsCopied += 1;
	}

	return SI_OPT(i32, itemsCopied);
}

SIDEF
siError si_pathMove(siString pathSrc, siString pathDst) {
	siOsChar stack[SI_PATH_MAX * 2];
	siOsString_2x strs = si_pathToOSMul(pathSrc, pathDst, stack, countof(stack));
	siOsString* paths = strs.v;

#if SI_SYSTEM_IS_WINDOWS
	i32 res = MoveFileW(paths[0], paths[1]);
	SI_ERROR_SYS_CHECK_RET(res == 0);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	i32 res = link(paths[0], paths[1]);
	SI_ERROR_SYS_CHECK_RET(res != 0);

	res = unlink(paths[0]);
	SI_ERROR_SYS_CHECK_RET(res != 0);
#else
	SI_UNUSED(paths);
#endif

	return SI_ERROR_NIL;
}

inline
siError si_pathRename(siString path, siString newPath) {
	return si_pathMove(path, newPath);
}

SIDEF
siError si_pathCreateFolder(siString path) {
	siOsChar stack[SI_PATH_MAX];
	si_pathToOS(path, stack, countof(stack));

	#if SI_SYSTEM_IS_WINDOWS
		i32 res = CreateDirectoryW(stack, nil);
		SI_ERROR_SYS_CHECK_RET(res == 0);
	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
		i32 res = mkdir(stack, 0777);
		SI_ERROR_SYS_CHECK_RET(res != 0);
	#endif

	return SI_ERROR_NIL;
}

SIDEF
siError si_pathRemove(siString path) {
	/* TODO(EimaMei): Check the performance of this, rewrite it. */
	siOsChar stack[SI_PATH_MAX];
	si_pathToOS(path, stack, countof(stack));

	#if SI_SYSTEM_IS_WINDOWS
		u32 attrs = GetFileAttributesW(stack);
		SI_ERROR_SYS_CHECK_RET(attrs == INVALID_FILE_ATTRIBUTES);

		i32 res;
		if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
			siDirectoryEntry entry;
			siDirectory dir = si_directoryOpen(path);
			while (si_directoryPollEntry(&dir, &entry)) {
				siError err = si_pathRemove(entry.path);
				SI_STOPIF(err.code != 0, return err);
			}

			res = RemoveDirectoryW(stack);
		}
		else {
			res = DeleteFileW(stack);
		}
		SI_ERROR_SYS_CHECK_RET(res == 0);

	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
		struct stat tmp;
		i32 res = stat(stack, &tmp);
		SI_ERROR_SYS_CHECK_RET(res != 0);

		if (S_ISDIR(tmp.st_mode)) {
			siDirectoryEntry entry;
			siDirectory dir = si_directoryOpen(path);
			while (si_directoryPollEntry(&dir, &entry)) {
				siError err = si_pathRemove(entry.path);
				SI_STOPIF(err.code != 0, return err);
			}

			res = rmdir(stack);
		}
		else {
			res = unlink(stack);
		}
		SI_ERROR_SYS_CHECK_RET(res != 0);

	#endif

	return SI_ERROR_NIL;
}

SIDEF
siError si_pathCreateHardLink(siString path, siString pathLink) {
	siOsChar stack[SI_PATH_MAX * 2];
	siOsString_2x strs = si_pathToOSMul(path, pathLink, stack, countof(stack));
	siOsString* paths = strs.v;

#if SI_SYSTEM_IS_WINDOWS
	i32 res = CreateHardLinkW(paths[1], paths[0], nil);
	SI_ERROR_SYS_CHECK_RET(res == 0);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	i32 res = link(paths[0], paths[1]);
	SI_ERROR_SYS_CHECK_RET(res != 0);
#else
	SI_UNUSED(paths);
#endif

	return SI_ERROR_NIL;
}

SIDEF
siError si_pathCreateSoftLink(siString path, siString pathLink) {
	siOsChar stack[SI_PATH_MAX * 2];
	siOsString_2x strs = si_pathToOSMul(path, pathLink, stack, countof(stack));
	siOsString* paths = strs.v;

#if SI_SYSTEM_IS_WINDOWS
	u32 attrs = GetFileAttributesW(paths[0]);
	SI_ERROR_SYS_CHECK_RET(attrs == INVALID_FILE_ATTRIBUTES);

	i32 res = CreateSymbolicLinkW(paths[1], paths[0], attrs & FILE_ATTRIBUTE_DIRECTORY);
	SI_ERROR_SYS_CHECK_RET(res == 0);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	i32 res = symlink(paths[0], paths[1]);
	SI_ERROR_SYS_CHECK_RET(res != 0);
#else
	SI_UNUSED(paths);

#endif

	return SI_ERROR_NIL;
}

inline
siString si_pathBaseName(siString path) {
	SI_ASSERT(path.len <= SI_PATH_MAX);

	isize i;
	for (i = path.len - 1; i >= 0; i -= 1) {
		if (path.data[i] == SI_PATH_SEPARATOR) {
			return si_substrEnd(path, i);
		}
	}
	return path;
}

SIDEF
siString si_pathUnrooted(siString path) {
	#if SI_SYSTEM_IS_WINDOWS
		isize offset = 3 * si_pathIsAbsolute(path);
	#else
		isize offset = (isize)si_pathIsAbsolute(path);
	#endif
	path = si_substrEnd(path, offset);

	for_range (i, 0, path.len) {
		if (path.data[i] == SI_PATH_SEPARATOR) {
			return si_substrEnd(path, i);
		}
	}
	return path;
}

SIDEF
siString si_pathExtension(siString path) {
	SI_ASSERT(path.len <= SI_PATH_MAX);

	isize i;
	for (i = path.len - 1; i >= 0; i -= 1) {
		if (path.data[i] == '.') {
			return si_substrStart(path, i + 1);
		}
		else if (path.data[i] == SI_PATH_SEPARATOR) {
			break;
		}
	}

	return SI_STR("");
}

SIDEF
siResult(siString) si_pathGetFullName(siString path, siAllocator alloc) {
	siOsChar stack[SI_PATH_MAX * 2];
	isize pathLen = si_pathToOS(path, stack, countof(stack));

#if SI_SYSTEM_IS_WINDOWS
	SI_OPTION_SYS_CHECK(!si_pathExistsOS(stack), siString);
	DWORD dif = si_cast(u32, countof(stack) - pathLen);

	isize len = (isize)GetFullPathNameW(stack, dif, &stack[pathLen], nil);
	SI_OPTION_SYS_CHECK(len == 0, siString);

	siUtf16String str = SI_BUF_LEN(&stack[pathLen], len);
	len = si_utf16ToUtf8StrLen(str);

	siString res = si_utf16ToUtf8Str(str, SI_BUF_ALLOC(u8, len, alloc));
	return SI_OPT(siString, res);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	siOsChar* out = &stack[pathLen];
	out = realpath(stack, out);
	SI_OPTION_SYS_CHECK(out == nil, siString);

	return SI_OPT(siString, si_stringCopyFromCstr(out, alloc));
#else
	return SI_OPT_NIL(siString);
	SI_UNUSED(pathLen); SI_UNUSED(alloc);

#endif
}


inline
b32 si_pathIsAbsolute(siString path) {
	SI_ASSERT(path.len <= SI_PATH_MAX);
	u8* data = (u8*)path.data;

#if SI_SYSTEM_IS_WINDOWS
	return (
		si_charIsAlpha((char)data[0])
		&& data[1] == ':'
		&& data[2] == SI_PATH_SEPARATOR
		&& data[3] != '\0'
	);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	return (data[0] == SI_PATH_SEPARATOR && path.len != 1);
#else
	return false;
	SI_UNUSED(data);
#endif
}

inline
b32 si_pathIsRelative(siString path) {
	return !si_pathIsAbsolute(path);
}

SIDEF
isize si_pathToOS(siString path, siOsChar* out, isize capacity) {
	SI_ASSERT(path.len <= SI_PATH_MAX);
	SI_ASSERT_NOT_NIL(out);
	SI_ASSERT_NOT_NEG(capacity);
	SI_STOPIF(capacity == 0, return 0);

	isize copied;

#if SI_SYSTEM_IS_WINDOWS
	if (path.len > 260 && si_pathIsAbsolute(path) && capacity > 4) {
		out[0] = '\\';
		out[1] = '\\';
		out[2] = '?';
		out[3] = '\\';

		si_stringToOsStrEx(path, SI_BUF_LEN(&out[4], capacity - 4), &copied);
		return copied + 4;
	}
#endif

	si_stringToOsStrEx(path, SI_BUF_LEN(out, capacity), &copied);
	return copied;
}

inline
siOsString_2x si_pathToOSMul(siString first, siString second, siOsChar* out, isize capacity) {
	siOsString_2x res;
	res.v[0] = out;
	res.len[0] = si_pathToOS(first, out, capacity);

	res.v[1] = &out[res.len[0]];
	res.len[1] = si_pathToOS(second, res.v[1], capacity - res.len[0]);

	return res;
}


SIDEF
u64 si_pathLastWriteTime(siString path) {
	siOsChar stack[SI_PATH_MAX];
	si_pathToOS(path, stack, countof(stack));

#if SI_SYSTEM_IS_WINDOWS
	FILETIME lastWriteTime = {0};
	WIN32_FILE_ATTRIBUTE_DATA data = {0};

	if (GetFileAttributesExW(stack, GetFileExInfoStandard, &data)) {
		lastWriteTime = data.ftLastWriteTime;
	}

	ULARGE_INTEGER res;
	res.LowPart = lastWriteTime.dwLowDateTime;
	res.HighPart = lastWriteTime.dwHighDateTime;

	return si_timeToUnix(res.QuadPart);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	/* TODO(EimaMei): Check if the output is correct. */
	struct stat fs;
	int res = stat(stack, &fs);
	return (res == 0) ? (u64)fs.st_mtime : 0;

#else
	return 0;

#endif
}

SIDEF
siString si_pathGetTmp(void) {
#if SI_SYSTEM_IS_WINDOWS
	static u8 buffer[256];
	u16 stack[256];

	u32 len = GetTempPathW(countof(stack), stack);
	SI_ASSERT(len != 0); /* NOTE(EimaMei): This musn't fail. */

	return si_utf16ToUtf8Str(SI_BUF_LEN(stack, len), SI_BUF_LEN(buffer, countof(buffer)));

#else
	return SI_STR("/tmp");

#endif
}


inline
siString si_pathReadContents(siString path, siAllocator alloc) {
	siFile file = si_fileOpen(path);
	siString res = si_fileReadContents(file, alloc);
	si_fileClose(&file);

	return res;
}
inline
siError si_pathReadContentsBuf(siString path, siBuffer(u8)* out) {
	siFile file = si_fileOpen(path);
	siError res = si_fileReadContentsBuf(file, out);
	si_fileClose(&file);

	return res;
}

static b32 SI_STD_FILE_SET = false;
static siFile SI_STD_FILE_ARR[siStdFile_Count] = {0};

SIDEF
siFile* si_fileGetStdFile(siStdFile type) {
	SI_ASSERT_MSG(si_between(i32, type, siStdFile_Input, siStdFile_Error), "Invalid standard file type.");

	#define SET_STD_FILE(index, type) SI_STD_FILE_ARR[index].handle = type

	if (!SI_STD_FILE_SET) {
		#if SI_SYSTEM_IS_WINDOWS
			SET_STD_FILE(0, (isize)GetStdHandle(STD_INPUT_HANDLE));
			SET_STD_FILE(1, (isize)GetStdHandle(STD_OUTPUT_HANDLE));
			SET_STD_FILE(2, (isize)GetStdHandle(STD_ERROR_HANDLE));

			if (IsValidCodePage(CP_UTF8)) {
				SetConsoleOutputCP(CP_UTF8);
			}

			#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
				#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
			#endif

			for_range (i, 1, 2) {
				DWORD mode;
				GetConsoleMode((HANDLE)SI_STD_FILE_ARR[i].handle, &mode);
				mode |= ENABLE_PROCESSED_OUTPUT  | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				SetConsoleMode((HANDLE)SI_STD_FILE_ARR[i].handle, mode);
			}
		#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE || SI_SYSTEM_EMSCRIPTEN
			SET_STD_FILE(0, 0);
			SET_STD_FILE(1, 1);
			SET_STD_FILE(2, 2);

		#else
			SI_PANIC();
		#endif
		SI_STD_FILE_SET = true;
	}
	return &SI_STD_FILE_ARR[type];
	#undef SET_STD_FILE
}


inline
siFile si_fileCreate(siString path) {
	return si_fileOpenMode(path, siFileMode_Write | siFileMode_Plus);
}

inline
siFile si_fileOpen(siString path) {
	return si_fileOpenMode(path, siFileMode_Read | siFileMode_Plus);
}
SIDEF
siFile si_fileOpenMode(siString path, siFileMode mode) {
	SI_ASSERT(path.len <= SI_PATH_MAX);
	SI_ASSERT((mode & ~(u32)siFileMode_All) == 0);

	siFile res = {0};
	res.handle = -1;

#if SI_SYSTEM_IS_WINDOWS
	u32 access, disposition;
	switch (mode) {
		case siFileMode_Read:
			access = GENERIC_READ;
			disposition = OPEN_EXISTING;
			break;
		case siFileMode_Write:
			access = GENERIC_WRITE;
			disposition = CREATE_ALWAYS;
			break;
		case siFileMode_Append:
			access = GENERIC_WRITE;
			disposition = OPEN_ALWAYS;
			break;
		case siFileMode_Read | siFileMode_Plus:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = OPEN_EXISTING;
			break;
		case siFileMode_Write | siFileMode_Plus:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = CREATE_ALWAYS;
			break;
		case siFileMode_Append | siFileMode_Plus:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = OPEN_ALWAYS;
			break;
		default:
			disposition = access = 0;
			SI_PANIC_MSG("Invalid file mode.");
	}

	void* handle;
	{
		siOsChar stack[SI_PATH_MAX];
		si_pathToOS(path, stack, countof(stack));

		handle = CreateFileW(
			stack, access, FILE_SHARE_READ | FILE_SHARE_DELETE, nil,
			disposition, FILE_ATTRIBUTE_NORMAL, nil
		);
	}
	SI_ERROR_SYS_CHECK(handle == INVALID_HANDLE_VALUE, res.error = SI_ERROR_RES; return res);

	if (mode & siFileMode_Append) {
		LARGE_INTEGER offset = {0};
		i32 status = SetFilePointerEx(handle, offset, nil, FILE_END);
		if (!status) {
			CloseHandle(handle);
			res.error = SI_ERROR_SYS();
			return res;
		}
	}
	res.handle = (isize)handle;
	res.size = si_fileSize(res);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	i32 flags;
	switch (mode) {
		case siFileMode_Read:
			flags = O_RDONLY;
			break;
		case siFileMode_Write:
			flags = O_WRONLY | O_CREAT | O_TRUNC;
			break;
		case siFileMode_Append:
			flags = O_WRONLY | O_APPEND | O_CREAT;
			break;
		case siFileMode_Read | siFileMode_Plus:
			flags = O_RDWR;
			break;
		case siFileMode_Write | siFileMode_Plus:
			flags = O_RDWR | O_CREAT | O_TRUNC;
			break;
		case siFileMode_Append | siFileMode_Plus:
			flags = O_RDWR | O_APPEND | O_CREAT;
			break;
		default:
			SI_PANIC_MSG("Invalid file mode.");
			flags = 0;
	}

	isize handle;
	{
		siOsChar stack[SI_PATH_MAX];
		si_pathToOS(path, stack, countof(stack));

		handle = open(stack, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	}
	SI_ERROR_SYS_CHECK(handle == -1, res.error = SI_ERROR_RES; return res);

	res.handle = handle;
	res.size = si_fileSize(res);

#endif

	return res;
}

SIDEF
isize si_fileSize(siFile file) {
	SI_ASSERT_NOT_NEG(file.handle);

#if SI_SYSTEM_IS_WINDOWS
	ULARGE_INTEGER res = {0};

	BY_HANDLE_FILE_INFORMATION data;
	if (GetFileInformationByHandle((HANDLE)file.handle, &data)) {
		res.HighPart = data.nFileSizeHigh;
		res.LowPart = data.nFileSizeLow;
	}

	return (ISIZE_MAX < res.QuadPart)
		? ISIZE_MAX
		: (isize)res.QuadPart;

#else
	isize prevOffset = si_fileTell(file);
	isize len = si_fileSeek(file, 0, siSeekWhere_End);
	si_fileSeek(file, prevOffset, siSeekWhere_Begin);

	return len;

#endif
}

inline
void si_fileSizeUpdate(siFile* file) {
	SI_ASSERT_NOT_NIL(file);
	file->size = si_fileSize(*file);
}

inline
siResult(siBuffer(u8)) si_fileRead(siFile file, isize len, siAllocator alloc) {
	return si_fileReadAt(file, si_fileTell(file), len, alloc);
}
inline
siResult(siBuffer(u8)) si_fileReadAt(siFile file, isize offset, isize len, siAllocator alloc) {
	len = si_min(isize, si_allocatorAvailable(alloc), len);

	siBuffer(u8) buffer = SI_BUF_ALLOC(u8, len, alloc);
	siError err = si_fileReadUnsafe(file, offset, len, &buffer);

	return (err.code == 0)
		? SI_OPT(siBuffer(u8), buffer)
		: SI_OPT_ERR(siBuffer(u8), err);
}

inline
siError si_fileReadBuf(siFile file, isize len, siBuffer(u8)* out) {
	return si_fileReadAtBuf(file, si_fileTell(file), len, out);
}
inline
siError si_fileReadAtBuf(siFile file, isize offset, isize len, siBuffer(u8)* out) {
	return si_fileReadUnsafe(file, offset, si_min(isize, len, out->len), out);
}

SIDEF
siError si_fileReadUnsafe(siFile file, isize offset, isize len, siBuffer(u8)* out) {
	SI_ASSERT_NOT_NEG(file.handle);
	SI_ASSERT_NOT_NIL(out);
	SI_ASSERT_NOT_NEG(len);
	SI_ASSERT_NOT_NIL(out->data);

	si_fileSeek(file, offset, siSeekWhere_Begin);

#if SI_SYSTEM_IS_WINDOWS
	DWORD bytesRead;
	DWORD toRead = (len <= UINT32_MAX)
		? (DWORD)len
		: UINT32_MAX;
	SI_ASSERT_MSG(len <= UINT32_MAX, "TODO: Fix this.");

	i32 res = ReadFile((HANDLE)file.handle, out->data, toRead, &bytesRead, NULL);
	SI_ERROR_SYS_CHECK_RET(res == 0)

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	isize bytesRead = pread((int)file.handle, out->data, (usize)len, offset);
	SI_ERROR_SYS_CHECK_RET(bytesRead == -1)

#else
	isize bytesRead = 0;

#endif

	out->len = bytesRead;
	return SI_ERROR_NIL;
}

SIDEF
siString si_fileReadContents(siFile file, siAllocator alloc) {
	isize len = si_min(isize, si_allocatorAvailable(alloc), file.size);
	siBuffer(u8) buffer = SI_BUF_ALLOC(u8, len, alloc);

	isize oldOffset = si_fileTell(file);
	siError err = si_fileReadUnsafe(file, 0, len, &buffer);
	si_fileSeek(file, oldOffset, siSeekWhere_Begin);

	return (err.code == 0) ? SI_STR_LEN(buffer.data, buffer.len) : SI_STR_EMPTY;
}
SIDEF
siError si_fileReadContentsBuf(siFile file, siBuffer(u8)* out) {
	isize oldOffset = si_fileTell(file);
	siError res = si_fileReadUnsafe(
		file, 0, si_min(isize, out->len, file.size), out
	);
	si_fileSeek(file, oldOffset, siSeekWhere_Begin);

	return res;
}

SIDEF
siBuffer(siString) si_fileReadlines(siFile file, siAllocator alloc) {
	siString str = si_fileReadContents(file, alloc);
	siBuffer(siString) res = si_stringSplit(str, SI_STR("\n"), alloc);
	si_free(alloc, (void*)str.data);

	return res;
}

inline
isize si_fileWrite(siFile* file, siBuffer(u8) data) {
	return si_fileWriteAt(file, data, si_fileTell(*file));
}

inline
isize si_fileWriteAt(siFile* file, siBuffer(u8) content, isize offset) {
	SI_ASSERT_NOT_NIL(file);
	SI_ASSERT_NOT_NEG(file->handle);
	SI_ASSERT(content.typeSize == si_sizeof(u8));

#if SI_SYSTEM_IS_WINDOWS
	si_fileSeek(*file, offset, siSeekWhere_Begin);

	/* TODO(EimaMei): Fix this. */
	DWORD bytesWritten;
	DWORD length = (content.len < UINT32_MAX)
		? (DWORD)content.len
		: UINT32_MAX;
	i32 res = WriteFile((HANDLE)file->handle, content.data, length, &bytesWritten, nil);
	SI_ERROR_SYS_CHECK(res == 0, file->error = SI_ERROR_RES; return -1);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	isize curOffset = si_fileSeek(*file, 0, siSeekWhere_Current);
	isize bytesWritten = (curOffset == offset)
		? write((int)file->handle, content.data, (usize)content.len)
		: pwrite((int)file->handle, content.data, (usize)content.len, offset);
	SI_ERROR_SYS_CHECK(bytesWritten == -1, file->error = SI_ERROR_RES; return -1);

#elif SI_SYSTEM_IS_WASM
	struct __wasi_ciovec_t iov;
	iov.buf = content.data;
	iov.buf_len = (__wasi_size_t)content.len;

	__wasi_size_t written;
	__wasi_errno_t err = __wasi_fd_write((__wasi_fd_t)file->handle, &iov, 1, &written);
	si__wasmSetLastError(err);
	SI_ERROR_SYS_CHECK(err != 0, file->error = SI_ERROR_RES; return -1);


	isize bytesWritten = (isize)written;

#else
	isize bytesWritten = 0;

#endif

	/* TODO(EimaMei): Have a test on this. */
	file->size += (offset - file->size) + bytesWritten;
	return bytesWritten;
}

inline
isize si_fileWriteByte(siFile* file, u8 byte) {
	return si_fileWritePtr(file, &byte, 1);
}


inline
isize si_fileWritePtr(siFile* file, const void* ptr, isize len) {
	return si_fileWrite(file, SI_BUF_LEN((u8*)ptr, len));

}

inline
isize si_fileWriteStr(siFile* file, siString str) {
	return si_fileWritePtr(file, str.data, str.len);
}



inline
isize si_fileTell(siFile file) {
	return si_fileSeek(file, 0, siSeekWhere_Current);
}

inline
isize si_fileSeek(siFile file, isize offset, siSeekWhere method) {
	SI_ASSERT_NOT_NEG(file.handle);

#if SI_SYSTEM_IS_WINDOWS
	LARGE_INTEGER res;
	res.QuadPart = offset;

	SetFilePointerEx((HANDLE)file.handle, res, &res, (u32)method);
	return (ISIZE_MAX < res.QuadPart)
		? ISIZE_MAX
		: (isize)res.QuadPart;

#elif SI_SYSTEM_IS_APPLE
	return lseek((int)file.handle, offset, method);

#elif SI_SYSTEM_IS_UNIX
	return lseek64((int)file.handle, offset, method);

#else
	return 0;
	SI_UNUSED(offset); SI_UNUSED(method);

#endif
}

inline
b32 si_fileSeekFront(siFile file) {
	return si_fileSeek(file, 0, siSeekWhere_Begin) != 0;
}

inline
b32 si_fileSeekBack(siFile file) {
	return si_fileSeek(file, 0, siSeekWhere_End) != 0;
}
SIDEF
b32 si_fileTruncate(siFile* file, isize size) {
	SI_ASSERT_NOT_NIL(file);
	SI_ASSERT_NOT_NEG(file->handle);
	SI_ASSERT_NOT_NEG(size);

#if SI_SYSTEM_IS_WINDOWS
	isize prevOffset = si_fileTell(*file);
	isize res = si_fileSeek(*file, size, siSeekWhere_Begin);
	SI_STOPIF(res == 0, return false);

	res = SetEndOfFile((HANDLE)file->handle);
	SI_ERROR_SYS_CHECK(res == 0, file->error = SI_ERROR_SYS_EX(siErrorSystem_TruncationFail); return false);

	si_fileSeek(*file, prevOffset, siSeekWhere_Begin);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	int res = ftruncate((int)file->handle, size);
	SI_ERROR_SYS_CHECK(res == -1, file->error = SI_ERROR_SYS_EX(siErrorSystem_TruncationFail); return false);
#else
	return false;

#endif

	return true;
}

SIDEF
u64 si_fileLastWriteTime(siFile file) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME lastWriteTime = {0};

	BY_HANDLE_FILE_INFORMATION data;
	if (GetFileInformationByHandle((HANDLE)file.handle, &data)) {
		lastWriteTime = data.ftLastWriteTime;
	}

	ULARGE_INTEGER res;
	res.HighPart = lastWriteTime.dwHighDateTime;
	res.LowPart = lastWriteTime.dwLowDateTime;

	return si_timeToUnix(res.QuadPart);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	struct stat fs;
	int res = fstat((int)file.handle, &fs);
	return (res == 0) ? (u64)fs.st_mtime : 0;
#else
	return 0;
	SI_UNUSED(file);

#endif


}

inline
void si_fileClose(siFile* file) {
	SI_ASSERT_NOT_NIL(file);
	SI_ASSERT_NOT_NEG(file->handle);

#if SI_SYSTEM_IS_WINDOWS
	CloseHandle((HANDLE)file->handle);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	close((int)file->handle);
#endif
}

SIDEF
siDirectory si_directoryOpen(siString path) {
	SI_ASSERT_NOT_NIL(path.data);
	SI_ASSERT(path.len <= SI_PATH_MAX);

	siDirectory dir;
	dir.error = SI_ERROR_NIL;
	dir.handle = nil;
	dir.directoryLen = path.len;
	si_memcopyStr(dir.buffer, path);

	if (si_stringAtBack(path) != SI_PATH_SEPARATOR) {
		dir.buffer[dir.directoryLen] = SI_PATH_SEPARATOR;
		dir.directoryLen += 1;
	}

#if SI_SYSTEM_IS_WINDOWS
	u16 stack[SI_PATH_MAX];
	siUtf16String pathWide = si_utf8ToUtf16Str(
		SI_STR_LEN(dir.buffer, dir.directoryLen),
		SI_BUF_LEN(stack, countof(stack) - 2)
	);

	stack[pathWide.len + 0] = '*';
	stack[pathWide.len + 1] = '\0';

	WIN32_FIND_DATAW tmp;
	HANDLE handle = FindFirstFileW(stack, &tmp);
	SI_ERROR_SYS_CHECK(handle == INVALID_HANDLE_VALUE, dir.error = SI_ERROR_RES; return dir);

	dir.handle = handle;
	SI_DISCARD(FindNextFileW(handle, &tmp));

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	/* NOTE(EimaMei): We do this because opendir only takes NULL-terminated C-strings. */
	dir.buffer[dir.directoryLen] = '\0';

	dir.handle = opendir((char*)dir.buffer);
	SI_ERROR_SYS_CHECK(dir.handle == nil, dir.error = SI_ERROR_RES; return dir);

#endif

	return dir;
}

inline
b32 si_directoryPollEntry(siDirectory* dir, siDirectoryEntry* out) {
	return si_directoryPollEntryEx(dir, true, out);
}

SIDEF
b32 si_directoryPollEntryEx(siDirectory* dir, b32 fullPath, siDirectoryEntry* out) {
	SI_ASSERT_NOT_NIL(dir);
	SI_ASSERT_NOT_NIL(out);
	SI_ASSERT_NOT_NIL(dir->handle);

#if SI_SYSTEM_IS_WINDOWS
	WIN32_FIND_DATAW file;
	if (FindNextFileW(dir->handle, &file) == 0) {
		siErrorSystem code = si_systemGetError();
		si_directoryClose(dir);

		SI_ERROR_SYS_CHECK(code != 0, dir->error = SI_ERROR_RES; return false);
		return false;
	}

	if ((file.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)) == 0) {
		out->type = siIoType_File;
	}
	else if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		out->type = siIoType_Directory;
	}
	else {
		out->type = siIoType_Link;
	}

	isize len;
	#ifndef SI_NO_CRT
	len = (isize)wcslen(file.cFileName);
	#else
	len = 0;
	while (file.cFileName[len] != '\0') { len += 1; }
	#endif

	out->path = si_utf16ToUtf8Str(
		SI_BUF_LEN(file.cFileName, len),
		SI_BUF_LEN(&dir->buffer[dir->directoryLen], si_sizeof(dir->buffer) - dir->directoryLen)
	);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	struct dirent* dirEntry;
back:
	dirEntry = readdir((DIR*)dir->handle);
	if (dirEntry == nil) {
		errno = 0;
		siErrorSystem code = si_systemGetError();
		si_directoryClose(dir);

		SI_ERROR_SYS_CHECK(code != 0, dir->error = SI_ERROR_RES; return false);
		return false;
	}


	static u8 IO_types[] = {
		[DT_REG]  = siIoType_File,
		[DT_DIR]  = siIoType_Directory,
		[DT_LNK]  = siIoType_Link,
		[DT_SOCK] = siIoType_Socket,
		[DT_CHR]  = siIoType_Device,
		[DT_BLK]  = siIoType_Block,
		[DT_FIFO] = siIoType_Fifo
	};
	out->type = IO_types[dirEntry->d_type];

	isize len = si_cstrLen(dirEntry->d_name);
	if (len <= 2) {
		u16 front = SI_TO_U16(dirEntry->d_name);
		if (front == SI_TO_U16(".\0") || front == SI_TO_U16("..")) {
			goto back;
		}
	}

	u8* data = &dir->buffer[dir->directoryLen];
	out->path.data = data;
	out->path.len = si_memcopy_s(
		SI_BUF_LEN(data, si_sizeof(dir->buffer) - dir->directoryLen),
		dirEntry->d_name, len
	);
#else
	return false;

#endif
	if (fullPath) {
		out->path.data = dir->buffer;
		out->path.len += dir->directoryLen;
	}

	return true;
}

inline
void si_directoryClose(siDirectory* dir) {
	SI_ASSERT_NOT_NIL(dir);
	SI_STOPIF(dir->handle == nil, return);

#if SI_SYSTEM_IS_WINDOWS
	CloseHandle(dir->handle);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	closedir((DIR*)dir->handle);
#endif
	dir->handle = nil;
}


#endif /* SI_IMPLEMENTATION_IO */

#ifdef SI_IMPLEMENTATION_THREAD

#if SI_SYSTEM_IS_WINDOWS

siIntern
DWORD WINAPI si__threadProc(LPVOID arg) {
	siThread* t = (siThread*)arg;
	t->returnValue = t->func(t->arg);
	t->state = siThreadState_Initialized;

	return 0;
}

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE || SI_SYSTEM_EMSCRIPTEN

siIntern
void* si__threadProc(void* arg) {
	#if SI_SYSTEM_EMSCRIPTEN
	si_sleep(1);
	#endif

	siThread* t = (siThread*)arg;
	t->returnValue = t->func(t->arg);
	t->state = siThreadState_Initialized;

	return nil;
}
#endif


inline
siThread si_threadMake(siThreadFunction function, void* arg) {
	return si_threadMakeEx(function, arg, 0);
}

inline
siThread si_threadMakeEx(siThreadFunction function, void* arg, usize stackSize) {
	SI_ASSERT_NOT_NIL(function);

	siThread thread = {0};
	thread.func = function;
	thread.arg = arg;
	thread.stackSize = stackSize;
	thread.state = siThreadState_Initialized;

	return thread;
}

SIDEF
siError si_threadMakeAndRun(siThreadFunction function, void* arg, siThread* out) {
	*out = si_threadMake(function, arg);
	return si_threadRun(out);
}

SIDEF
siError si_threadRun(siThread* thread) {
	SI_ASSERT_NOT_NIL(thread);
	SI_ASSERT(thread->state == siThreadState_Initialized);

	#if SI_SYSTEM_IS_WINDOWS
		thread->id = CreateThread(nil, thread->stackSize, si__threadProc, thread, 0, nil);
		SI_ERROR_SYS_CHECK_RET(thread->id == nil);

	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE || SI_SYSTEM_EMSCRIPTEN
		pthread_attr_t attr;
		void* attrPtr = nil;
		if (thread->stackSize != 0) {
			int res = pthread_attr_init(&attr);
			if (res != 0) {
				pthread_attr_setstacksize(&attr, thread->stackSize);
				attrPtr = &attr;
			}
		}

		pthread_t id;
		{
			int res = pthread_create(&id, attrPtr, si__threadProc, thread);
			SI_ERROR_SYS_CHECK_RET(res != 0);
		}
		thread->id = id;

		if (attrPtr) {
			pthread_attr_destroy(&attr);
		}
	#else
		return SI_ERROR_NIL;
	#endif

	thread->state = siThreadState_Running;
	return SI_ERROR_NIL;
}

SIDEF
siError si_threadJoin(siThread* thread) {
	SI_ASSERT_NOT_NIL(thread);
	SI_ASSERT_NOT_NIL((void*)thread->id);

	#if SI_SYSTEM_IS_WINDOWS
		DWORD res = WaitForSingleObject(thread->id, INFINITE);
		SI_ERROR_SYS_CHECK_RET(res != 0);
	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE || SI_SYSTEM_EMSCRIPTEN
		int res = pthread_join(thread->id, nil);
		SI_ERROR_SYS_CHECK_RET(res != 0);
	#endif

	return SI_ERROR_NIL;
}

SIDEF
siError si_threadDestroy(siThread* thread) {
	SI_ASSERT_NOT_NIL(thread);
	SI_STOPIF(thread->id == 0, return SI_ERROR_NIL);

#if SI_SYSTEM_IS_WINDOWS
	i32 res = CloseHandle(thread->id);
	SI_ERROR_SYS_CHECK_RET(!res);
#endif
	thread->id = 0;
	thread->state = siThreadState_Closed;

	return SI_ERROR_NIL;
}

SIDEF
b32 si_threadPrioritySet(siThread t, i32 priority) {
	/* TODO(EimaMei): Rework this */
	#if SI_SYSTEM_IS_WINDOWS
		isize res = SetThreadPriority(t.id, priority);
		SI_ASSERT_MSG(res != 0, "Something went wrong setting the thread priority.");
		SI_UNUSED(res);
	#elif SI_SYSTEM_IS_UNIX
		usize error_code = pthread_setschedprio(t.id, priority);

		cstring error_msg = nil;
		switch (error_code) {
			case 0:  break;
			case EINVAL:   error_msg = "The value of 'priority' is invalid for the scheduling policy of the specified thread."; break;
			case ENOTSUP:  error_msg = "An attempt was made to set the priority to an unsupported value."; break;
			case EPERM:    error_msg = "The caller does not have the appropriate permission to set the scheduling policy"
						   "of the specified thread OR the implementation does not allow the application to modify the"
							"priority to the value specified."; break;
			case ESRCH:    error_msg = "The value specified by thread does not refer to an existing thread."; break;
			default:       error_msg = "Unknown error code (%li)."; break;
		}
		SI_ASSERT_FMT(error_code == 0, error_msg, error_code);
	#else
		SI_PANIC_MSG("si_threadPrioritySet: Not supported on MacOS.");
		SI_UNUSED(t);
		SI_UNUSED(priority);
	#endif

	return false;
}
#endif /* SI_IMPLEMENTATION_THREAD */

#ifdef SI_IMPLEMENTATION_TIME

siString si__timeMonthNames[] = {
	SI_STRC("January"),
	SI_STRC("February"),
	SI_STRC("March"),
	SI_STRC("April"),
	SI_STRC("May"),
	SI_STRC("June"),
	SI_STRC("July"),
	SI_STRC("August"),
	SI_STRC("September"),
	SI_STRC("October"),
	SI_STRC("November"),
	SI_STRC("December"),
};

siString si__timeMonthNamesShrt[] = {
	SI_STRC("Jan"),
	SI_STRC("Feb"),
	SI_STRC("Mar"),
	SI_STRC("Apr"),
	SI_STRC("May"),
	SI_STRC("Jun"),
	SI_STRC("Jul"),
	SI_STRC("Aug"),
	SI_STRC("Sep"),
	SI_STRC("Oct"),
	SI_STRC("Nov"),
	SI_STRC("Dec"),
};

siString si__timeWeekNames[] = {
	SI_STRC("Sunday"),
	SI_STRC("Monday"),
	SI_STRC("Tuesday"),
	SI_STRC("Wednesday"),
	SI_STRC("Thursday"),
	SI_STRC("Friday"),
	SI_STRC("Saturday"),
};

siString si__timeWeekNamesShrt[] = {
	SI_STRC("Sun"),
	SI_STRC("Mon"),
	SI_STRC("Tue"),
	SI_STRC("Wed"),
	SI_STRC("Thu"),
	SI_STRC("Fri"),
	SI_STRC("Sat"),
};

siString si__timeAM_PM_Names[] = {
	SI_STRC("AM"), SI_STRC("PM"), SI_STRC("am"), SI_STRC("pm")
};

siString* SI_NAMES_MONTHS_FULL = si__timeMonthNames;
siString* SI_NAMES_MONTHS_SHRT = si__timeMonthNamesShrt;
siString* SI_NAMES_DAYS_FULL = si__timeWeekNames;
siString* SI_NAMES_DAYS_SHRT = si__timeWeekNamesShrt;
siString* SI_NAMES_AM_PM = si__timeAM_PM_Names;

inline
u64 si_RDTSC(void) {
	/* NOTE(EimaMei): Credit goes to gb.h for the i386 and PPC code. (https://github.com/gingerBill/gb/blob/master/gb.h#L8682C1-L8715C7). */
#if SI_COMPILER_CHECK_MIN(MSVC, 12, 0, 0)
	return __rdtsc();
#elif !defined(SI_NO_INLINE_ASM)
	#if SI_ARCH_I386
		u64 res;
		si_asm (".byte 0x0f, 0x31", : "=A" (res));
		return res;

	#elif SI_ARCH_AMD64
		u64 res;
		si_asm(
			"rdtsc"           SI_ASM_NL
			"shl rdx, 0x20"   SI_ASM_NL
			"or rax, rdx",
			SI_ASM_OUTPUT("=a"(res))
		);
		return res;

	#elif SI_ARCH_IS_PPC
		u32 high, low, tmp;
		si_asm (
			"0:"            SI_ASM_NL
			"mftbu %0"      SI_ASM_NL
			"mftb %1"       SI_ASM_NL
			"mftbu %2"      SI_ASM_NL
			"cmpw %2, %0"   SI_ASM_NL
			"bne 0b",
			SI_ASM_OUTPUT("=r"(high), "=r"(low), "=r"(tmp))
		);
		return ((u64)high << 32) | low;

	#elif SI_ARCH_ARM64
		u64 res;
		si_asm ("mrs %0, cntvct_el0", SI_ASM_OUTPUT("=r"(res)));
		return res;

	#elif SI_ARCH_IS_WASM
		__wasi_timestamp_t time;
		__wasi_errno_t res = __wasi_clock_time_get(__WASI_CLOCKID_PROCESS_CPUTIME_ID, 1, &time);

		return (res == 0) ? time : 0;

	#elif SI_SYSTEM_IS_WINDOWS
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return (u64)count.QuadPart;

	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
		struct timespec tv = {0};
		clock_gettime(CLOCK_MONOTONIC, &tv);
		return (u64)tv.tv_sec * SI_CLOCKS_SECS + (u64)tv.tv_nsec;

	#else
		return 0;
	#endif

#else
	#if SI_SYSTEM_IS_WINDOWS
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return (u64)count.QuadPart;

	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
		struct timespec tv = {0};
		clock_gettime(CLOCK_MONOTONIC, &tv);
		return (u64)tv.tv_sec * SI_CLOCKS_SECS + (u64)tv.tv_nsec;

	#elif SI_SYSTEM_IS_WASM
		__wasi_timestamp_t time;
		__wasi_errno_t res = __wasi_clock_time_get(__WASI_CLOCKID_PROCESS_CPUTIME_ID, 1, &time);

		return (res == 0) ? time : 0;
	#else
		return 0;
	#endif
#endif /* SI_NO_INLINE_ASM */
}

inline
u64 si_RDTSCP(i32* proc) {
	SI_ASSERT_NOT_NIL(proc);

#if SI_COMPILER_CHECK_MIN(MSVC, 12, 0, 0)
	return __rdtscp((u32*)proc);
#elif !defined(SI_NO_INLINE_ASM)
	#if SI_ARCH_I386
		u64 res;
		si_asm (".byte 0x0f, 0x31", : "=A" (res));
		return res;

	#elif SI_ARCH_AMD64
		u64 res;
		si_asm(
			"rdtscp"          SI_ASM_NL
			"shl rdx, 0x20"   SI_ASM_NL
			"or rax, rdx",
			SI_ASM_OUTPUT("=a"(res), "=c"(*proc))
		);
		return res;

	#elif SI_ARCH_IS_PPC
		u32 aux;

		/* TODO(EimaMei): Check if this is even accurate. */
		si_asm(
			"mfspr %0, 1023",
			SI_ASM_OUTPUT("=r"(aux))
		);
		*proc = (i32)aux;

		return si_RDTSC();

	#elif SI_ARCH_ARM64
		u64 mpidr;

		/* TODO(EimaMei): Check if this is even accurate. */
		si_asm(
			"mrs %0, mpidr_el1",
			SI_ASM_OUTPUT("=r"(mpidr))
		);
		*proc = (i32)(mpidr & 0xFF);

		return si_RDTSC();

	#elif SI_SYSTEM_IS_WINDOWS
		*proc = GetCurrentProcessorNumber();
		return si_RDTSC();

	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
		*proc = sched_getcpu();
		return si_RDTSC();

	#else
		*proc = 0;
		return si_RDTSC();

	#endif

#else
	#if SI_SYSTEM_IS_WINDOWS
		*proc = GetCurrentProcessorNumber();
		return si_RDTSC();

	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
		*proc = sched_getcpu();
		return si_RDTSC();
	#else
		*proc = 0;
		return si_RDTSC();
	#endif
#endif /* SI_NO_INLINE_ASM */
}


inline
u64 si_clock(void) {
	return si_RDTSC() / (u32)si_cpuClockSpeed() * 1000;
}

u64 si__timecountLocal = 0;

inline
void si_timeStampStart(void) {
	si__timecountLocal = si_timeStampStartEx();
}
inline
u64 si_timeStampStartEx(void) {
	return si_RDTSC();
}

inline
void si_timeStampPrintSince(void) {
	si_timeStampPrintSinceEx(si__timecountLocal);
}
SIDEF
void si_timeStampPrintSinceEx(u64 ts) {
	u64 end = si_RDTSC();
	u64 diff = (end - ts) / (u32)si_cpuClockSpeed() * 1000;

	const siBenchmarkLimit* time = si_benchmarkLimitLoop(diff);
	si_printf(
		"si_timeStampPrintSince: TIME: %.2f %s\n",
		(f32)diff / (f32)time->duration, time->unit
	);
}

inline
void si_sleep(i32 miliseconds) {
	SI_ASSERT_NOT_NEG(miliseconds);
	SI_STOPIF(miliseconds == 0, return);

#if SI_SYSTEM_IS_WINDOWS
	Sleep((u32)miliseconds);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	struct timespec ts = {
		miliseconds / 1000,
		(miliseconds % 1000) * SI_CLOCKS_MILI
	};
	nanosleep(&ts, &ts);

#elif SI_SYSTEM_WASI
	__wasi_subscription_t in = {0};
	in.u.tag = __WASI_EVENTTYPE_CLOCK;
	in.u.u.clock.id = __WASI_CLOCKID_MONOTONIC;
	in.u.u.clock.timeout = miliseconds;
	in.u.u.clock.precision = SI_CLOCKS_MILI;

	__wasi_event_t events;
	size_t nevents;

	__wasi_errno_t result = __wasi_poll_oneoff(&in, &events, 1, &nevents);
	SI_UNUSED(result);
#elif SI_SYSTEM_EMSCRIPTEN
	emscripten_sleep(miliseconds);
#endif
}


SIDEF
i64 si_timeNowUTC(void) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME time;
	GetSystemTimePreciseAsFileTime(&time);

	return si_timeToUnix((i64)time.dwHighDateTime << 32 | time.dwLowDateTime);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	struct timespec spec;
	i32 res = clock_gettime(CLOCK_REALTIME, &spec);

	return (res == 0)
		? spec.tv_sec * 1000000000u + spec.tv_nsec
		: 0;

#else
	return 0;

#endif
}

inline
i64 si_timeNowLocal(void) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME utc;
	GetSystemTimePreciseAsFileTime(&utc);
	FILETIME time;
	int res = FileTimeToLocalFileTime(&utc, &time);

	return (res)
		? si_timeToUnix((i64)time.dwHighDateTime << 32 | time.dwLowDateTime)
		: 0;

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	i64 timeNow = si_timeNowUTC();
	if (timezone == 0) {
		tzset();
	}

	return (timezone != 0)
		? timeNow - timezone * 1000000000 + daylight * 3600000000000
		: 0;
#else
	return 0;

#endif
}

inline
b32 si_timeYearIsLeap(i32 year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

i32 si_timeGetDayOfWeek(i32 year, i32 month, i32 day) {
	static i8 t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	if (month < 3 ) {
		year -= 1;
	}
	return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}



// Convert raw Unix time to human-readable components
siTimeCalendar si_timeToCalendar(i64 time) {
	siTimeCalendar calendar;

	i32 daysSinceEpoch = si_cast(i32, time / 86400000000000);

	{ /* Year. */
		const i32 DAYS_IN_400_YEARS = (400 * 365) + 97;
		const i32 DAYS_IN_100_YEARS = (100 * 365) + 24;
		const i32 DAYS_IN_4_YEARS   = (  4 * 365) + 1;

		calendar.years = 1970;
		calendar.years += 400 * (daysSinceEpoch / DAYS_IN_400_YEARS);
		daysSinceEpoch %= DAYS_IN_400_YEARS;

		calendar.years += 100 * (daysSinceEpoch / DAYS_IN_100_YEARS);
		daysSinceEpoch %= DAYS_IN_100_YEARS;

		calendar.years += 4 * (daysSinceEpoch / DAYS_IN_4_YEARS);
		daysSinceEpoch %= DAYS_IN_4_YEARS;

		calendar.years += daysSinceEpoch / 365;
		daysSinceEpoch %= 365;
	}

	{ /* Month, day. */
		static const i8 daysInMonths[2][12] = {
			{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
			{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
		};

		b32 leap = si_timeYearIsLeap(calendar.years);
		for_rangeEx (, calendar.months, 0, countof(daysInMonths[0])) {
			i32 days = daysInMonths[leap][calendar.months];
			SI_STOPIF(daysSinceEpoch < days, break);

			daysSinceEpoch -= days;
		}
		calendar.months += 1;
		calendar.days = daysSinceEpoch;
	}

	{ /* Hour, minute, second, nanosecond. */
		i64 nanoseconds = time % 86400000000000;

		calendar.hours = si_cast(i32, nanoseconds / 3600000000000);
		nanoseconds %= 3600000000000;

		calendar.minutes = si_cast(i32, nanoseconds / 60000000000);
		nanoseconds %= 60000000000;

		calendar.seconds = si_cast(i32, nanoseconds / 1000000000);
		nanoseconds %= 1000000000;

		calendar.nanoseconds = (i32)nanoseconds;
	}

	return calendar;
}

siIntern
isize si__timeToStr(siString fmt, isize* i, char letter, siBuffer(u8) out, isize len, i32 time) {
	if (*i < fmt.len - 1 && fmt.data[1 + *i] == letter) {
		*i += 1;
		if (time < 10) {
			si_bufferSet(out, len, "0");
			return 1 + si_stringFromUInt((u32)time, si_sliceEnd(out, len + 1)).len;
		}
	}

	return si_stringFromUInt((u32)time, si_sliceEnd(out, len)).len;
}

siIntern
void si__timeTimezone(b32 check, siTimeCalendar* calendar) {
	SI_STOPIF(check == false, return);

	if (calendar->hours != 12 && calendar->hours != 0) {
		calendar->hours %= 12;
	}
	else if (calendar->hours == 0) {
		calendar->hours = 12;
	}
}

SIDEF
siString si_timeToString(siTimeCalendar calendar, siString fmt, siBuffer(u8) out) {
	SI_ASSERT(out.typeSize == si_sizeof(u8));

	b32 AMwasChecked = false;
	i32 ogHour = calendar.hours;
	isize len = 0;

	for_range (i, 0, fmt.len) {
		SI_STOPIF(len >= out.len, break);

		switch (fmt.data[i]) {
			case 'y': {
				isize ogI = i;
				while (i < fmt.len - 1 && fmt.data[i + 1] == 'y' && (i - ogI) != 4) { i += 1; }

				u32 year = (i - ogI != 1) ? (u32)calendar.years : (u32)calendar.years % 100u;
				len += si_stringFromUInt(year, si_sliceEnd(out, len)).len;
			} break;

			case 'M': {
				SI_ASSERT(si_between(i32, calendar.months, 0, 12));

				isize ogI = i;
				while (i < fmt.len - 1 && fmt.data[i + 1] == 'M' && (i - ogI) != 2) { i += 1; }

				switch (i - ogI) {
					case 0: {
						len += si_stringFromUInt((u32)calendar.months, si_sliceEnd(out, len)).len;
					} break;

					case 1: {
						if (calendar.months < 10) {
							si_bufferSet(out, len, "0");
							len += 1;
						}

						len += si_stringFromUInt((u32)calendar.months, si_sliceEnd(out, len)).len;
					} break;
				}
			} break;

			case 'd': {
				isize ogI = i;
				while (i < fmt.len - 1 && fmt.data[i + 1] == 'd' && (i - ogI) != 4) { i += 1; }

				switch (i - ogI) {
					case 0: {
						len += si_stringFromUInt((u32)calendar.days, si_sliceEnd(out, len)).len;
					} break;

					case 1: {
						if (calendar.days < 10) {
							si_bufferSet(out, len, "0");
							len += 1;
						}

						len += si_stringFromUInt((u32)calendar.days, si_sliceEnd(out, len)).len;
					} break;

					case 2: {
						i32 wd = si_timeGetDayOfWeek(calendar.years, calendar.months, calendar.days);
						siString* str = &SI_NAMES_DAYS_SHRT[wd];
						len += si_memcopyStr_s(si_sliceEnd(out, len), *str);
					} break;

					case 4: {
						i32 wd = si_timeGetDayOfWeek(calendar.years, calendar.months, calendar.days);
						siString* str = &SI_NAMES_DAYS_FULL[wd];
						len += si_memcopyStr_s(si_sliceEnd(out, len), *str);
					} break;
				}
			} break;

			case 'h': {
				if (!AMwasChecked) {
					for_range (j, i + 1, fmt.len) {
						if (SI_TO_U16(&fmt.data[j]) == SI_TO_U16("ap") ||
							SI_TO_U16(&fmt.data[j]) == SI_TO_U16("AP")) {
							si__timeTimezone(AMwasChecked, &calendar);
							break;
						}
					}
					AMwasChecked = true;
				}

				len += si__timeToStr(fmt, &i, 'h', out, len, calendar.hours);
			} break;

			case 'm': {
				len += si__timeToStr(fmt, &i, 'm', out, len, calendar.minutes);
			} break;

			case 's': {
				len += si__timeToStr(fmt, &i, 's', out, len, calendar.seconds);
			} break;

			case 'n': {
				len += si__timeToStr(fmt, &i, 'n', out, len, calendar.nanoseconds);
			} break;

			case 'A': {
				SI_STOPIF(i < fmt.len - 1 && fmt.data[1 + i] == 'P', break);
				goto AM_code;
			} break;

			case 'a': {
				SI_STOPIF(i < fmt.len - 1 && fmt.data[1 + i] == 'p', break);
AM_code:
				i += 1;

				siString* str = &SI_NAMES_AM_PM[2 * (fmt.data[i] == 'p') + (ogHour >= 12)];
				len += si_memcopyStr_s(si_sliceEnd(out, len), *str);

				si__timeTimezone(AMwasChecked, &calendar);
				AMwasChecked = true;
			} break;

			default:
				si_bufferSet(out, len, &fmt.data[i]);
				len += 1;
		}
	}

	return SI_STR_LEN(out.data, len);
}

#endif /* SI_IMPLEMENTATION_TIME */

#ifdef SI_IMPLEMENTATION_BIT

SIDEF
i32 si_numCountBitsU8(u8 num) {
	return (i32)(((u64)num * 01001001001L & 042104210421) % 017);
}


SIDEF
i32 si_numCountBitsU32(u32 num) {
	 num -= ((num >> 1) & 0x55555555);
	 num = (num & 0x33333333) + ((num >> 2) & 0x33333333);
	 num = (num + (num >> 4)) & 0x0F0F0F0F;
	 return (i32)((num * 0x01010101) >> 24);
}


SIDEF
i32 si_numCountBitsU64(u64 num) {
	num = num - ((num >> 1) & 0x5555555555555555);
	num = (num & 0x3333333333333333) + ((num >> 2) & 0x3333333333333333);
	num = (num + (num >> 4)) & 0xF0F0F0F0F0F0F0F;
	return (i32)((num * 0x101010101010101) >> 56);
}

SIDEF
u32 si_numLeadingZerosEx(u64 num, u32 totalBits) {
	SI_STOPIF(totalBits >= 64, totalBits = 63);
	u32 count = 0;

	u64 i;
	for (i = SI_BIT(totalBits); i != 0; i >>= 1) {
		if ((num & i) == 0) {
			count += 1;
		}
		else {
			return count;
		}
	}

	return count;
}

SIDEF
u32 si_numLeadingOnesEx(u64 num, u32 totalBits) {
	SI_STOPIF(totalBits >= 64, totalBits = 63);
	u32 count = 0;

	u64 i;
	for (i = SI_BIT(totalBits); i != 0; i >>= 1) {
		if ((num & i) != 0) {
			count += 1;
		}
		else {
			return count;
		}
	}

	return count;
}

SIDEF
u32 si_numTrailingBitEx(u64 num, usize totalBits, siBitType bit) {
	u32 count = 0;

	for_rangeEx (u64, i, 0, totalBits) {
		if ((num & i) == bit) {
			count += 1;
		}
		else {
			return count;
		}
	}

	return count;
}

inline
u64 si_numRotateLeftEx(u64 num, usize totalBits, usize bits) {
	return (num << bits) | (num >> (totalBits - bits));
}

inline
u64 si_numRotateRightEx(u64 num, usize totalBits, usize bits) {
	return (num >> bits) | (num << (totalBits - bits));
}


inline
u64 si_numReverseBitsEx(u64 num, usize totalBits) {
	u64 res = 0;

	for_range (i, 0, totalBits) {
		res <<= 1;
		res |= (num & 1);
		num >>= 1;
	}

	return res;
}

SIDEF
siBuffer(u8) si_numToBytesEx(u64 num, isize num_sizeof, siAllocator alloc) {
	u8* res = si_allocArray(alloc, u8, num_sizeof);

	for_range (i, 0, num_sizeof) {
		res[i] = num & 0xFF;
		num >>= 8;
	}

	return SI_BUF_LEN(res, num_sizeof);
}

SIDEF
u64 si_numFromBytes(siBuffer(u8) bytes) {
	u64 res = 0;
	u8 byte;
	for_eachBuf (byte, bytes) {
		res <<= 8;
		res |= byte;
	}

	return res;
}

inline
u16 si_swap16(u16 x) {
#if SI_COMPILER_CHECK_MIN(GCC, 4, 8, 0) || SI_COMPILER_CHECK_MIN(CLANG, 3, 2, 0)
	return (u16)__builtin_bswap16(x);

#elif SI_COMPILER_CHECK_MIN(MSVC, 14, 0, 0)
	return _byteswap_ushort(x);

#else
	return si_cast(u16, ((x >> 8) & 0xFFu) | ((x & 0xFFu) << 8));

#endif
}
inline
u32 si_swap32(u32 x) {
#if SI_COMPILER_CHECK_MIN(GCC, 4, 3, 0) || SI_COMPILER_CLANG
	return __builtin_bswap32(x);

#elif SI_COMPILER_CHECK_MIN(MSVC, 14, 0, 0)
	return _byteswap_ulong(x);

#else
	return si_cast(
		u32,
		((x & 0xFF000000u) >> 24)  | ((x & 0x00FF0000u) >>  8) |
		((x & 0x0000FF00u) <<  8)  | ((x & 0x000000FFu) << 24)
	);

#endif
}
inline
u64 si_swap64(u64 x) {
#if SI_COMPILER_CHECK_MIN(GCC, 4, 3, 0) || SI_COMPILER_CLANG
	return __builtin_bswap64(x);

#elif SI_COMPILER_CHECK_MIN(MSVC, 14, 0, 0)
	return _byteswap_uint64(x);

#else
   return si_cast(
		u64,
		((x & 0xFF00000000000000ull) >> 56) | ((x & 0x00FF000000000000ull) >> 40) |
		((x & 0x0000FF0000000000ull) >> 24) | ((x & 0x000000FF00000000ull) >>  8) |
		((x & 0x00000000FF000000ull) <<  8) | ((x & 0x0000000000FF0000ull) << 24) |
		((x & 0x000000000000FF00ull) << 40) | ((x & 0x00000000000000FFull) << 56)
   );

#endif
}

#if SI_ENDIAN_IS_LITTLE
inline u16 si_swap16be(u16 x) { return si_swap16(x); }
inline u32 si_swap32be(u32 x) { return si_swap32(x); }
inline u64 si_swap64be(u64 x) { return si_swap64(x); }

inline u16 si_swap16le(u16 x) { return x; }
inline u32 si_swap32le(u32 x) { return x; }
inline u64 si_swap64le(u64 x) { return x; }

#else
inline u16 si_swap16be(u16 x) { return x; }
inline u32 si_swap32be(u32 x) { return x; }
inline u64 si_swap64be(u64 x) { return x; }

inline u16 si_swap16le(u16 x) { return si_swap16(x); }
inline u32 si_swap32le(u32 x) { return si_swap32(x); }
inline u64 si_swap64le(u64 x) { return si_swap64(x); }

#endif


SIDEF
isize si_numLen(u64 num) {
	if (num < 10) return 1;
	if (num < 100) return 2;
	if (num < 1000) return 3;
	if (num < 10000) return 4;
	if (num < 100000) return 5;
	if (num < 1000000) return 6;
	if (num < 10000000) return 7;
	if (num < 100000000) return 8;
	if (num < 1000000000) return 9;
	if (num < 10000000000) return 10;
	if (num < 100000000000) return 11;
	if (num < 1000000000000) return 12;
	if (num < 10000000000000) return 13;
	if (num < 100000000000000) return 14;
	if (num < 1000000000000000) return 15;
	if (num < 10000000000000000) return 16;
	if (num < 100000000000000000) return 17;
	if (num < 1000000000000000000) return 18;
	if (num < 10000000000000000000U) return 19;
	return 20;
}

inline
isize si_numLenEx(u64 num, i32 base) {
	SI_ASSERT_NOT_NEG(base);

	isize count = 0;
	do {
		count += 1;
		num /= (u32)base;
	} while (num != 0);

	return count;
}

inline
isize si_numLenInt(i64 num) {
	return si_numLenIntEx(num, 10);
}

inline
isize si_numLenIntEx(i64 num, i32 base) {
	if (num < 0) {
		num = -num;
		return 1 + si_numLenEx((u64)num, base);
	}
	return si_numLenEx((u64)num, base);
}

#define SI_CHECK_ARITHMETIC_IMPL_ALL_I(func, def, action, ...) \
	SI_CHECK_ARITHMETIC_DEC(i8,    func, SIDEF, {    const i8 max = INT8_MAX;     const i8 min = INT8_MIN;  *res =    (i8)(action); __VA_ARGS__ })  \
	SI_CHECK_ARITHMETIC_DEC(i16,   func, SIDEF, {   const i16 max = INT16_MAX;   const i16 min = INT16_MIN; *res =   (i16)(action); __VA_ARGS__ }) \
	SI_CHECK_ARITHMETIC_DEC(i32,   func, SIDEF, {   const i32 max = INT32_MAX;   const i32 min = INT32_MIN; *res =   (i32)(action); __VA_ARGS__ }) \
	SI_CHECK_ARITHMETIC_DEC(i64,   func, SIDEF, {   const i64 max = INT64_MAX;   const i64 min = INT64_MIN; *res =   (i64)(action); __VA_ARGS__ }) \
	SI_CHECK_ARITHMETIC_DEC(isize, func, SIDEF, { const isize max = ISIZE_MAX; const isize min = ISIZE_MIN; *res = (isize)(action); __VA_ARGS__ })

#define SI_CHECK_ARITHMETIC_IMPL_ALL_U(func, def, action, ...) \
	SI_CHECK_ARITHMETIC_DEC(u8,    func, SIDEF, {    const u8 max = UINT8_MAX;  SI_UNUSED(max); *res =    (u8)(action); __VA_ARGS__ } ) \
	SI_CHECK_ARITHMETIC_DEC(u16,   func, SIDEF, {   const u16 max = UINT16_MAX; SI_UNUSED(max); *res =   (u16)(action); __VA_ARGS__ } ) \
	SI_CHECK_ARITHMETIC_DEC(u32,   func, SIDEF, {   const u32 max = UINT32_MAX; SI_UNUSED(max); *res =   (u32)(action); __VA_ARGS__ } ) \
	SI_CHECK_ARITHMETIC_DEC(u64,   func, SIDEF, {   const u64 max = UINT64_MAX; SI_UNUSED(max); *res =   (u64)(action); __VA_ARGS__ } ) \
	SI_CHECK_ARITHMETIC_DEC(usize, func, SIDEF, { const usize max = USIZE_MAX;  SI_UNUSED(max); *res = (usize)(action); __VA_ARGS__ } ) \


SI_CHECK_ARITHMETIC_IMPL_ALL_U(Add, SIDEF, a + b,
	SI_ASSERT_NOT_NIL(res); \
	return a > *res; \
)
SI_CHECK_ARITHMETIC_IMPL_ALL_I(Add, SIDEF, a + b,
	SI_ASSERT_NOT_NIL(res); \
	if (a >= 0) { \
		return (max - a < b); \
	} \
	return (b < min - a); \
)

SI_CHECK_ARITHMETIC_IMPL_ALL_U(Sub, SIDEF, a - b,
	SI_ASSERT_NOT_NIL(res); \
	return a < *res; \
)
SI_CHECK_ARITHMETIC_IMPL_ALL_I(Sub, SIDEF, a - b,
	SI_ASSERT_NOT_NIL(res); \
	if (b < 0) { \
		return (max + b < a); \
	} \
	return (min + b > a); \
)

SI_CHECK_ARITHMETIC_IMPL_ALL_U(Mul, SIDEF, a * b,
	SI_ASSERT_NOT_NIL(res); \
	return (b > 0 && a > max / b);
)
SI_CHECK_ARITHMETIC_IMPL_ALL_I(Mul, SIDEF, a * b,
	SI_ASSERT_NOT_NIL(res); \
	if (a > 0) { \
		return (b > 0 && a > max / b) \
			|| (b < 0 && b < min / a); \
	} \
	return a < 0 && ( \
		(b > 0 && a < min / b) \
		|| (b < 0 && a < max / b) \
	); \
)

#undef SI_CHECK_ARITHMETIC_IMPL_ALL_U
#undef SI_CHECK_ARITHMETIC_IMPL_ALL_S
#undef SI_CHECK_ARITHMETIC_DEC

#endif /* SI_IMPLEMENTATION_BIT */

#ifdef SI_IMPLEMENTATION_CPU

inline
void si_CPUID(u32 ID, u32 registers[4]) {
#if SI_COMPILER_CHECK_MIN(MSVC, 8, 0, 0)
	__cpuid((i32*)registers, (i32)ID);

#elif !defined(SI_NO_INLINE_ASM) && SI_ARCH_IS_X86
	si_asm(
		"cpuid",
		SI_ASM_OUTPUT(
			"=a"(registers[0]), "=b"(registers[1]),
			"=c"(registers[2]), "=d"(registers[3])
		)
		SI_ASM_INPUT("a"(ID), "c"(0))
	);
#else
	SI_UNUSED(ID); SI_UNUSED(registers);

#endif
}


SIDEF
i32 si_cpuClockSpeed(void) {
	/* TODO(EimaMei): Not sure if this is a good solution. */
	static i32 SI_CPU_FREQ_MHZ = -1;
	SI_STOPIF(SI_CPU_FREQ_MHZ != -1, return SI_CPU_FREQ_MHZ);

	/* NOTE(EimaMei): We can find an accurate clock speed by waiting a whole second
	 * and storing the results, however waing a whole second is too long.
	 *
	 * We can somewhat mitigate this by only waiting 100 ms, convert it into mHz
	 * by dividing _integers_ (this specifically "rounds" up the errors), then
	 * multiply by 10. */
	u64 begin = si_RDTSC();
	si_sleep(100);
	u64 end = si_RDTSC();

	SI_CPU_FREQ_MHZ = (i32)(end - begin) / SI_CLOCKS_MILI * 10;

	return SI_CPU_FREQ_MHZ;
}

SIDEF
i32 si_cpuProcessorCount(void) {
	static i32 procCount = -1;
	SI_STOPIF(procCount != -1, return procCount);

#if SI_SYSTEM_IS_UNIX
	procCount = (i32)sysconf(_SC_NPROCESSORS_ONLN);

#elif SI_SYSTEM_IS_WINDOWS
	DWORD len;
	i32 res = GetLogicalProcessorInformation(nil, &len);
	SI_STOPIF(res != 0 || len == 0, return 0);

	SYSTEM_LOGICAL_PROCESSOR_INFORMATION* processors = si_mallocArray(SYSTEM_LOGICAL_PROCESSOR_INFORMATION, len);
	res = GetLogicalProcessorInformation(&processors[0], &len);
	SI_STOPIF(res == 0, len = 0);

	procCount = 0;
	for_range (i, 0, len) {
		SYSTEM_LOGICAL_PROCESSOR_INFORMATION processor = processors[i];
		if (processor.Relationship == RelationProcessorCore) {
			procCount += si_numCountBitsU64(processor.ProcessorMask);
		}
	}

	si_mfree(processors);

#elif SI_SYSTEM_IS_APPLE
	i64 count;
	usize size = si_sizeof(i64);
	if (sysctlbyname("hw.logicalcpu", &count, &size, nil, 0) == 0 && count > 0) {
		procCount = (i32)count;
	}
	else  {
		procCount = 0;
	}

#elif SI_SYSTEM_IS_WASM
	procCount = 1;

#else
	procCount = 0;

#endif

	return procCount;
}

#endif /* SI_IMPLEMENTATION_CPU */

#ifdef SI_IMPLEMENTATION_PRINT

SIDEF
isize si_print(cstring str) {
	return si_fprint(si_stdout, str);
}
SIDEF
isize si_printLn(cstring str) {
	return si_fprintLn(si_stdout, str);
}

SIDEF
isize si_printf(cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_printfVa(fmt, va);
	va_end(va);

	return res;
}
SIDEF
isize si_printfVa(cstring fmt, va_list va) {
	return si_fprintfVa(si_stdout, fmt, va);
}

SIDEF
isize si_printfLn(cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_printfLnVa(fmt, va);
	va_end(va);

	return res;
}
SIDEF
isize si_printfLnVa(cstring fmt, va_list va) {
	return si_fprintfLnVa(si_stdout, fmt, va);
}



SIDEF
isize si_fprint(siFile* file, cstring str) {
	return si_fileWriteStr(file, SI_CSTR(str));
}

SIDEF
isize si_fprintf(siFile* file, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_fprintfVa(file, fmt, va);
	va_end(va);

	return res;
}

SIDEF
isize si_fprintLn(siFile* file, cstring str) {
	isize n = si_fprint(file, str);
	n += si_fileWriteByte(file, '\n');
	return n;
}
SIDEF
isize si_fprintfVa(siFile* file, cstring fmt, va_list va) {
	return si_fileWriteStr(file, si_bprintfVa(SI_BUF_STACK(SI_KILO(8)), fmt, va));

}

SIDEF
isize si_fprintfLn(siFile* file, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_fprintfLnVa(file, fmt, va);
	va_end(va);

	return res;
}
SIDEF
isize si_fprintfLnVa(siFile* file, cstring fmt, va_list va) {
	return si_fileWriteStr(file, si_bprintfLnVa(SI_BUF_STACK(SI_KILO(8)), fmt, va));
}


SIDEF
siString si_bprintf(siBuffer(u8) out, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	siString res = si_bprintfVa(out, fmt, va);
	va_end(va);

	return res;
}

SIDEF
siString si_bprintfVa(siBuffer(u8) out, cstring fmt, va_list va) {
	return si_bprintfStrVa(out, SI_CSTR(fmt), va);
}


SIDEF
siString si_bprintfStr(siBuffer(u8) out, siString fmt, ...) {
	va_list va;
	va_start(va, fmt);
	siString res = si_bprintfStrVa(out, fmt, va);
	va_end(va);

	return res;

}

struct si__printfInfoStruct {
	u8* data;
	isize index;

	i32 padSize;
	u8 padLetter;

	siString str;
	isize capacity;
};

force_inline
void si__printStrToBuf(struct si__printfInfoStruct* info) {
	u8* base = &info->data[info->index];

	isize len = (info->capacity > info->str.len)
		? info->str.len
		: info->capacity;

	if (info->padSize == 0) {
		si_memcopy(base, info->str.data, len);
	}
	else if (info->padSize < 0) {
		isize padLen = len + info->padSize;
		b32 padNeeded = (padLen < 0);

		si_memcopy(base, info->str.data, len);
		if (padNeeded) {
			isize padding = -padLen;
			si_memset(&base[len], info->padLetter, padding);
			info->index += padding;
			info->capacity -= padding;
		}
		info->padSize = 0;
	}
	else {
		isize padLen = info->padSize - len;
		b32 padNeeded = (padLen > 0);

		if (padNeeded) {
			si_memset(base, info->padLetter, padLen);
			info->index += padLen;
			info->capacity -= padLen;
		}
		si_memcopy(&info->data[info->index], info->str.data, len);
		info->padSize = 0;
	}

	info->index += len;
	info->capacity -= len;
}
force_inline
void si__printStrCpy(struct si__printfInfoStruct* info) {
	isize len = (info->capacity > info->str.len)
		? info->str.len
		: info->capacity;

	info->index += si_memcopy(&info->data[info->index], info->str.data, len);
}


force_inline
isize SI_SET_FMT_PTR(siRune* x, const u8** fmtPtr) {
	siUtf32Char res = si_utf8Decode(*fmtPtr);
	*x = res.codepoint;
	*fmtPtr += res.len;

	return res.len;
}

#define SI_CHECK_AFTERPOINT_INT(info, afterPointIsSet, afterPoint) \
	if (afterPointIsSet) { \
		SI_STOPIF(afterPoint == 0, break); \
		info.padLetter = '0'; \
		info.padSize = afterPoint; \
	} \
	do {} while (0)

SIDEF
siString si_bprintfStrVa(siBuffer(u8) out, siString fmt, va_list va) {
	SI_ASSERT(out.typeSize == si_sizeof(u8));
	SI_STOPIF(out.len == 0, return SI_STR_EMPTY);

	union {
		i32 I32; u32 U32;
		u64 U64; i64 I64; f64 F64;
		usize USIZE; isize ISIZE;
		cstring STR; u8* PTR;
	} vaValue;

	struct si__printfInfoStruct info = {0};
	info.data = out.data;
	info.capacity = out.len;

	siBuffer(u8) stack = SI_BUF_STACK(128);

	siRune x;
	const u8* fmtPtr = fmt.data;
	i32 base = 10;
	b32 colorPresent = false;

	while (info.index < info.capacity) {
		SI_STOPIF(fmtPtr >= &fmt.data[fmt.len], break);

		SI_SET_FMT_PTR(&x, &fmtPtr);
		SI_STOPIF(
			x != '%',
			{ info.data[info.index] = (u8)x; info.index += 1; continue; }
		);

		isize xLen = SI_SET_FMT_PTR(&x, &fmtPtr);
		vaValue.U64 = 0;
		info.padLetter = ' ';

		i32 afterPoint = 6;
		b32 afterPointIsSet = false;
		i32 typeSize = 0;
		i32* ptrToVar = &info.padSize;

GOTO_PRINT_SWITCH:
		switch (x) {
			case 'n': {
				(void)va_arg(va, signed int*);
				break;
			}
			case '%': {
				info.data[info.index] = '%';
				info.index += 1;
				break;
			}

			case 'h': {
				SI_STOPIF(typeSize != 0, continue);
				if (*fmtPtr == 'h') {
					fmtPtr += 1;
					typeSize = 1;
				}
				else {
					typeSize = 2;
				}

				SI_SET_FMT_PTR(&x, &fmtPtr);
				goto GOTO_PRINT_SWITCH;
			}

			case 'l': {
				SI_STOPIF(typeSize != 0, continue);
				SI_STOPIF(*fmtPtr == 'l', fmtPtr += 1);
				SI_SET_FMT_PTR(&x, &fmtPtr);
				typeSize = 8;
				goto GOTO_PRINT_SWITCH;
			}

			case 't': case 'z': {
				SI_STOPIF(typeSize != 0, continue);
				SI_SET_FMT_PTR(&x, &fmtPtr);
				typeSize = si_sizeof(usize);
				goto GOTO_PRINT_SWITCH;
			}

			case 'j': {
				SI_STOPIF(typeSize != 0, continue);
				SI_SET_FMT_PTR(&x, &fmtPtr);
				typeSize = 8;
				goto GOTO_PRINT_SWITCH;
			}

			case '#': {
				const u8* beforeAlt = fmtPtr;
				do {
					SI_SET_FMT_PTR(&x, &fmtPtr);
				}
				while (x != 'x' && x != 'b' && x != 'o' && x != 'X' && x != 'O');

				u8 altForm[2] = {'0', (u8)x | SI_BIT(5)};
				info.str = SI_STR_LEN(altForm, si_sizeof(altForm));

				si__printStrCpy(&info);

				fmtPtr = beforeAlt + 1;
				x = *beforeAlt;

				goto GOTO_PRINT_SWITCH;
			}
			case '*': {
				*ptrToVar = va_arg(va, i32);

				SI_SET_FMT_PTR(&x, &fmtPtr);
				goto GOTO_PRINT_SWITCH;
			}
			case '.': {
				ptrToVar = &afterPoint;
				afterPointIsSet = true;

				SI_SET_FMT_PTR(&x, &fmtPtr);
				goto GOTO_PRINT_SWITCH;
			}
			case '-': case '+': {
				if (si_runeIsDigit(si_utf8Decode(fmtPtr).codepoint)) {
					goto GOTO_PADCALC_SWITCH;
				}
				siFallthrough; /* NOTE(EimaMei): Go to the ' ' case. */
			}
			case ' ': {
				info.str = SI_STR_LEN(&x, si_sizeof(char));
				si__printStrCpy(&info);

				SI_SET_FMT_PTR(&x, &fmtPtr);
				goto GOTO_PRINT_SWITCH;
			 }

			case '0': {
				SI_SET_FMT_PTR(&x, &fmtPtr);
				if (si_runeIsDigit(x)) {
					info.padLetter = '0';
				}
				else {
					*ptrToVar = 0;
				}

				goto GOTO_PRINT_SWITCH;
			}
			case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			case '8': case '9': {
				const u8* start;
GOTO_PADCALC_SWITCH:
				start = fmtPtr - 1;

				while (true) {
					siUtf32Char letter = si_utf8Decode(fmtPtr);
					if (!si_runeIsDigit(letter.codepoint)) {
						break;
					}
					fmtPtr += letter.len;
				}

				isize len = fmtPtr - start;
				i64 count = si_stringToInt(SI_STR_LEN(start, len));
				SI_ASSERT(count <= INT32_MAX);

				*ptrToVar = (i32)count;

				SI_SET_FMT_PTR(&x, &fmtPtr);
				goto GOTO_PRINT_SWITCH;
			}

			case 'x': case 'X':
				base = 16;
				si_numChangeTable(x != 'x');
				goto GOTO_SPECIFIER_U;
			case 'O': case 'o':
				base = 8;
				goto GOTO_SPECIFIER_U;
			case 'b':
				base = 2;
				goto GOTO_SPECIFIER_U;

			case 'u': {
				base = 10;
GOTO_SPECIFIER_U:
				switch (typeSize) {
					case 0: vaValue.U32 = va_arg(va, u32); break;
					case 1: vaValue.U32 = (u8)va_arg(va, u32); break;
					case 2: vaValue.U32 = (u16)va_arg(va, u32); break;
					case 4: vaValue.U32 = va_arg(va, u32); break;
					case 8: vaValue.U64 = va_arg(va, u64); break;
					default: SI_PANIC();
				}
				SI_CHECK_AFTERPOINT_INT(info, afterPointIsSet, afterPoint);

				info.str = si_stringFromUIntEx(vaValue.U64, base, stack);
				si__printStrToBuf(&info);

				si_numChangeTable(true);
			} break;

			case 'i': case 'd': {
				switch (typeSize) {
					case 0: vaValue.I64 = va_arg(va, i32); break;
					case 1: vaValue.I64 = (i8)va_arg(va, i32); break;
					case 2: vaValue.I64 = (i16)va_arg(va, i32); break;
					case 4: vaValue.I64 = va_arg(va, i32); break;
					case 8: vaValue.I64 = va_arg(va, i64); break;
					default: SI_DEBUG_TRAP();
				}
				SI_CHECK_AFTERPOINT_INT(info, afterPointIsSet, afterPoint);

				info.str = si_stringFromInt(vaValue.I64, stack);
				si__printStrToBuf(&info);
				break;
			}

			case 'c': {
				vaValue.I32 = va_arg(va, i32);

				siUtf8Char utf8 = si_utf8Encode(vaValue.I32);
				info.str = SI_STR_LEN(utf8.codepoint, utf8.len);

				si__printStrToBuf(&info);

			} break;

			case 's': {
				vaValue.STR = va_arg(va, char*);

				if (vaValue.STR == nil) {
					info.str = SI_STR("(nil)");
				}
				else {
					isize len = afterPointIsSet
						? afterPoint
						: si_cstrLen(vaValue.STR);
					info.str = SI_STR_LEN(vaValue.STR, len);
				}

				si__printStrToBuf(&info);
			} break;

			case 'A': case 'a': {
				vaValue.F64 = va_arg(va, f64);

				char altForm[2] = {'0', si_cast(char, x + ('X' - 'A'))};
				info.str = SI_STR_LEN(altForm, si_sizeof(altForm));
				si__printStrCpy(&info);

				info.str = si_stringFromFloatEx(vaValue.F64, 16, afterPoint, stack);
				si__printStrToBuf(&info);
			} break;

			case 'F': case 'f': {
				vaValue.F64 = va_arg(va, f64);

				info.str = si_stringFromFloatEx(vaValue.F64, 10, afterPoint, stack);
				si__printStrToBuf(&info);
			} break;

			case 'E': case 'e': {
				vaValue.F64 = va_arg(va, f64);
GOTO_SCIENTIFIC_NOTATION:
				if (si_float64IsNan(vaValue.F64) || si_float64IsInf(vaValue.F64)) {
					info.str = si_stringFromFloatEx(vaValue.F64, 10, 0, stack);
					si__printStrToBuf(&info);
					break;
				}
				i32 exponent = 0;
				while (vaValue.F64 > 10) {
					vaValue.F64 /= 10;
					exponent += 1;
				}

				info.str = si_stringFromFloatEx(vaValue.F64, 10, afterPoint, stack);
				si__printStrToBuf(&info);


				u8 remainder[4];
				remainder[0] = (u8)x;
				remainder[1] = '+';
				if (exponent < 10) {
					remainder[2] = '0';
					remainder[3] = (u8)(exponent + '0');
				}
				else {
					remainder[2] = (u8)((exponent / 10) + '0');
					remainder[3] = (u8)((exponent % 10) + '0');
				}

				info.str = SI_STR_LEN(remainder, si_sizeof(remainder));
				si__printStrCpy(&info);
			} break;

			case 'G': case 'g': {
				vaValue.F64 = va_arg(va, f64);

				if (vaValue.F64 < 0.0001) {
					x = (u8)('E' + (x - 'G'));
					goto GOTO_SCIENTIFIC_NOTATION;
				}

				u64 pow10Val = si_pow10(afterPoint);
				u64 intFloat = (u64)vaValue.F64;

				if (vaValue.F64 > (f64)pow10Val) {
					afterPoint -= (i32)(intFloat / pow10Val);

					x = (u8)('E' + (x - 'G'));
					goto GOTO_SCIENTIFIC_NOTATION;
				}

				f64 remainder = vaValue.F64 - (f64)intFloat;
				f64 maxZeroValue = si_pow10F64(-(i32)afterPoint);

				i32 newAfterPoint = 0;
				while (newAfterPoint < afterPoint) {
					remainder *= 10;
					if (si_between(f64, si_abs(f64, remainder), 0, maxZeroValue)) {
						break;
					}
					f64 digit = remainder + 0.5;
					remainder -= digit;
					newAfterPoint += 1;
				}

				info.str = si_stringFromFloatEx(vaValue.F64, 10, newAfterPoint, stack);
				si__printStrToBuf(&info);
			} break;

			case 'p': {
				info.str = SI_STR("0x");
				si__printStrCpy(&info);

				vaValue.USIZE = (usize)va_arg(va, void*);
				info.str = si_stringFromUIntEx(vaValue.USIZE, 16, stack);
				si__printStrCpy(&info);
			} break;

			case 'S': {
				info.str = va_arg(va, siString);
				si__printStrToBuf(&info);
			} break;

			case 'B': { /* Boolean */
				info.str = si_stringFromBool(va_arg(va, b32));
				si__printStrCpy(&info);
			} break;

			case 'C': {
				if (colorPresent) {
					colorPresent = false;
					info.str = SI_STR("\33[0m");
					si__printStrCpy(&info);
					break;
				}

				siPrintColor clr = va_arg(va, siPrintColor);
				colorPresent = true;

				switch (clr.type) {
					case siPrintColorType_3bit: {
						SI_ASSERT(si_between(u8, clr.data.ansi.color, 0, 7));

						char str[] ="\33[\0;\0\0m";
						str[2] = (!clr.data.ansi.bold) ? '0' : '1';
						str[4] = (!clr.data.ansi.light) ? '3' : '9';
						str[5] = (char)('0' + clr.data.ansi.color);

						info.str = SI_STR(str);
						si__printStrCpy(&info);
					} break;

					case siPrintColorType_8bit: {
						#define _8BIT_STR "\33[38;5;"

						char str[32] = _8BIT_STR;
						siBuffer(char) buf = SI_BUF_LEN(str, countof_str(str));
						isize trueLen = countof_str(_8BIT_STR);

						siString num = si_stringFromInt(
							clr.data.cube, si_sliceEnd(buf, countof_str(_8BIT_STR))
						);
						trueLen += num.len;

						str[trueLen] = 'm';
						trueLen += 1;

						info.str = SI_STR_LEN(str, trueLen);
						si__printStrCpy(&info);

						#undef _8BIT_STR
					} break;

					case siPrintColorType_24bit: {
						#define _24BIT_STR "\33[38;2;"

						char str[64] = _24BIT_STR;
						siBuffer(char) buf = SI_BUF_LEN(str, countof_str(str));
						isize trueLen = countof_str(_24BIT_STR);

						static char divider[countof(clr.data.rgb)] = {';', ';', 'm'};
						for_range (j, 0, countof(clr.data.rgb)) {
							siString num = si_stringFromInt(
								clr.data.rgb[j], si_sliceEnd(buf, trueLen)
							);
							trueLen += num.len;

							str[trueLen] = divider[j];
							trueLen += 1;

						}

						info.str = SI_STR_LEN(str, trueLen);
						si__printStrCpy(&info);

						#undef _24BIT_STR
					} break;

					default: SI_PANIC();
				}
			} break;

			default: {
				u8 str[5] = {'%'};
				si_memcopy(&str[1], fmtPtr - xLen, xLen);
				info.str = SI_STR_LEN(str, 1 + xLen);
				si__printStrCpy(&info);
			}
		}
	}

	return SI_STR_LEN(info.data, info.index);
}

#undef SI_CHECK_AFTERPOINT_INT
#undef SI_SET_FMT_PTR


SIDEF
siString si_bprintfLn(siBuffer(u8) out, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	siString res = si_bprintfLnVa(out, fmt, va);
	va_end(va);

	return res;
}
SIDEF
siString si_bprintfLnVa(siBuffer(u8) out, cstring fmt, va_list va) {
	SI_STOPIF(out.len == 0, return SI_STR_EMPTY);
	siString str = si_bprintfVa(out, fmt, va);
	si_bufferSet(out, str.len, "\n");
	str.len += 1;

	return str;
}


SIDEF
b32 si_printHas24bitColor(void) {
	siString colorterm = si_envVarGetData(SI_STR("COLORTERM"), SI_BUF_STACK(16));

	return si_stringEqual(colorterm, SI_STR("truecolor"));
}


#endif /* SI_IMPLEMENTATION_PRINT */

#ifdef SI_IMPLEMENTATION_DLL

inline
siDllHandle si_dllLoad(cstring path) {
	SI_ASSERT_NOT_NIL(path);
#if SI_SYSTEM_IS_WINDOWS
	return (siDllHandle)LoadLibraryA(path);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	return (siDllHandle)dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
#else
	return nil;
#endif
}

inline
void si_dllUnload(siDllHandle dll) {
	SI_ASSERT_NOT_NIL(dll);
#if SI_SYSTEM_IS_WINDOWS
	FreeLibrary((HMODULE)dll);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	dlclose(dll);
#endif
}

inline
siDllProc si_dllProcAddress(siDllHandle dll, cstring name) {
	SI_ASSERT_NOT_NIL(dll);
	SI_ASSERT_NOT_NIL(name);

#if SI_SYSTEM_IS_WINDOWS
	PROC proc = GetProcAddress((HMODULE)dll, name);
	return si_transmute(siDllProc, *proc, PROC);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	return (siDllProc)dlsym(dll, name);

#else
	return nil;

#endif
}
#endif /* SI_IMPLEMENTATION_DLL */

#ifdef SI_IMPLEMENTATION_MATH

SI_MATH_FUNC_DECLARE_2X       (min,     inline, { return (a < b) ? a : b;  })
SI_MATH_FUNC_DECLARE_2X       (max,     inline, { return (a > b) ? a : b;  })
SI_MATH_FUNC_DECLARE_3X_B32   (between, inline, { return b <= a && a <= c; })

inline i8 si__abs_i8(i8 a)    { return a < 0 ?  (i8)-a : (i8)a; }
inline i16 si__abs_i16(i16 a) { return a < 0 ? (i16)-a : (i16)a; }
inline i32 si__abs_i32(i32 a) { return a < 0 ? -a : a; }
inline i64 si__abs_i64(i64 a) { return a < 0 ? -a : a; }
inline isize si__abs_isize(isize a) { return a < 0 ? -a : a; }
inline f32 si__abs_f32(f32 a) { return a < 0 ? -a : a; }
inline f64 si__abs_f64(f64 a) { return a < 0 ? -a : a; }

inline
f64 si__sin_f64(f64 x) {
	static f64 const x0 = +1.91059300966915117e-31;
	static f64 const x1 = +1.00086760103908896;
	static f64 const x2 = -1.21276126894734565e-2;
	static f64 const x3 = -1.38078780785773762e-1;
	static f64 const x4 = -2.67353392911981221e-2;
	static f64 const x5 = +2.08026600266304389e-2;
	static f64 const x6 = -3.03996055049204407e-3;
	static f64 const x7 = +1.38235642404333740e-4;
	return x0 + x * (x1 + x * (x2 + x * (x3 + x * (x4 + x * (x5 + x * (x6 + x * x7))))));
}

inline
f32 si__sin_f32(f32 x) {
	static f32 const x0 = +1.91059300966915117e-31f;
	static f32 const x1 = +1.00086760103908896f;
	static f32 const x2 = -1.21276126894734565e-2f;
	static f32 const x3 = -1.38078780785773762e-1f;
	static f32 const x4 = -2.67353392911981221e-2f;
	static f32 const x5 = +2.08026600266304389e-2f;
	static f32 const x6 = -3.03996055049204407e-3f;
	static f32 const x7 = +1.38235642404333740e-4f;
	return x0 + x * (x1 + x * (x2 + x * (x3 + x * (x4 + x * (x5 + x * (x6 + x * x7))))));
}

inline
f64 si__cos_f64(f64 x) {
	static f64 const x0 = +1.00238601909309722;
	static f64 const x1 = -3.81919947353040024e-2;
	static f64 const x2 = -3.94382342128062756e-1;
	static f64 const x3 = -1.18134036025221444e-1;
	static f64 const x4 = +1.07123798512170878e-1;
	static f64 const x5 = -1.86637164165180873e-2;
	static f64 const x6 = +9.90140908664079833e-4;
	static f64 const x7 = -5.23022132118824778e-14;
	return x0 + x * (x1 + x * (x2 + x * (x3 + x * (x4 + x * (x5 + x * (x6 + x * x7))))));
}
inline
f32 si__cos_f32(f32 x) {
	static f32 const x0 = +1.00238601909309722f;
	static f32 const x1 = -3.81919947353040024e-2f;
	static f32 const x2 = -3.94382342128062756e-1f;
	static f32 const x3 = -1.18134036025221444e-1f;
	static f32 const x4 = +1.07123798512170878e-1f;
	static f32 const x5 = -1.86637164165180873e-2f;
	static f32 const x6 = +9.90140908664079833e-4f;
	static f32 const x7 = -5.23022132118824778e-14f;
	return x0 + x * (x1 + x * (x2 + x * (x3 + x * (x4 + x * (x5 + x * (x6 + x * x7))))));
}

inline f64 si__round_f64(f64 x) { return (x >= 0.0f) ? si_floor(f64, x + 0.5f) : si_ceil(f64, x - 0.5f); }
inline f32 si__round_f32(f32 x) { return (x >= 0.0f) ? si_floor(f32, x + 0.5f) : si_ceil(f32, x - 0.5f); }

inline f32 si__floor_f32(f32 a) { return (a >= 0.0f) ? (f32)(i64)a : (f32)(i64)(a - 0.9999999999999999f); }
inline f64 si__floor_f64(f64 a) { return (a >= 0.0f) ? (f64)(i64)a : (f64)(i64)(a - 0.9999999999999999f); }

inline f32 si__ceil_f32(f32 a) { return (a < 0) ? (f32)(i64)a : (f32)(i64)(a + 1); }
inline f64 si__ceil_f64(f64 a) { return (a < 0) ? (f64)(i64)a : (f64)(i64)(a + 1); }

SIDEF
u64 si_pow10(i32 exponent) {
	static const u64 allPossibleValues[] = {
		1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
		10000000000ULL, 100000000000ULL, 1000000000000ULL, 10000000000000ULL,
		100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL
	};
	SI_ASSERT_NOT_NEG(exponent);
	SI_ASSERT_MSG(exponent < countof(allPossibleValues), "Exponent must be in range from 0 to 18 to not overflow.");

	return allPossibleValues[exponent];
}

SIDEF
f64 si_pow10F64(i32 exponent) {
	static const f64 allPossibleValues[] = {
		1e-18, 1e-17, 1e-16, 1e-15, 1e-14, 1e-13,
		1e-12, 1e-11, 1e-10, 1e-09, 1e-08, 1e-07,
		1e-06, 1e-05, 1e-04, 1e-03, 1e-02, 1e-01,

		1e+0,

		1e+01, 1e+02, 1e+03, 1e+04, 1e+05, 1e+06,
		1e+07, 1e+08, 1e+09, 1e+10, 1e+11, 1e+12,
		1e+13, 1e+14, 1e+15, 1e+16, 1e+17, 1e+18
	};
	SI_ASSERT_MSG(si_between(i32, exponent, -18, 18), "Exponent must be in range from -18 to 18 to not overflow.");

	return allPossibleValues[exponent + 18];
}

inline
i32 si_float32IsInf(f32 num) {
	union { f32 f; u32 n; } check;
	check.f = num;

	switch (check.n) {
		case 0x7F800000: return 1;
		case 0xFF800000: return 2;
	}

	return 0;
}

inline
b32 si_float32IsNan(f32 num) {
	static const u32 nanV = 0x7FC00000; /* NOTE(EimaMei): Canonical NaN value for 32-bit. */

	union { f32 f; u32 n; } check;
	check.f = num;
	check.n &= ~SI_ENDIAN_VALUE((u32)SI_BIT(31), SI_BIT(0)); /* NOTE(EimaMei): A quick way of changing the minus to plus. */

	return check.n == nanV;
}

inline
i32 si_float64IsInf(f64 num) {
	union { f64 f; u64 n; } check;
	check.f = num;

	switch (check.n) {
		case 0x7FF0000000000000: return 1; /* NOTE(EimaMei): Positive infinity. */
		case 0xFFF0000000000000: return 2; /* NOTE(EimaMei): Negative infinity. */
	}

	return 0;
}

inline
b32 si_float64IsNan(f64 num) {
	static const u64 nanV = 0x7FF8000000000000; /* NOTE(EimaMei): Canonical NaN value for 64-bit. */

	union { f64 f; u64 n; } check;
	check.f = num;
	check.n &= ~SI_ENDIAN_VALUE(SI_BIT(63), SI_BIT(0)); /* NOTE(EimaMei): A quick way of changing the minus to plus. */

	return check.n == nanV;
}

#endif /* SI_IMPLEMENTATION_MATH */

#ifdef SI_IMPLEMENTATION_BENCHMARK

SIDEF
void si_benchmarkLoopsAvgPrint(cstring name, u64 array[20], usize len, usize range[2]) {
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction - '%s'\n"
		"Runs:\n",
		name
	);

	const siBenchmarkLimit* element = nil;
	f64 freq = (f64)si_cpuClockSpeed() / 1000.0;
	isize padRuns = si_numLen(range[1]);

	usize arrayI = 0, runs;
	for (runs = range[0]; runs <= range[1]; runs *= 10) {
		u64 cycles = array[arrayI];
		f64 time = (f64)cycles / freq;

		element = si_benchmarkLimitLoop((u64)time);
		time /= (f64)element->duration;
		si_printf(
			"\t%*zu %s - %C%9.4f%C %s (%lu cycles)\n",
			padRuns, runs, (runs != 1) ? "runs" : "run ",
			si_printColor3bit(siPrintColor3bit_Green), time, element->unit, cycles
		);

		arrayI += 1;
	}

	u64 cyclesTotal = 0;
	for_range (i, 0, len) {
		cyclesTotal += array[i];
	}

	f64 cyclesMedian = (f64)cyclesTotal / (f64)len;
	f64 time = cyclesMedian / freq;

	element = si_benchmarkLimitLoop((u64)time);
	time /= (f64)element->duration;

	isize padCycles = si_numLen((u64)cyclesMedian);
	si_printf(
		"Final result:\n"
			"\tTime average   - %C%*.4f%C %s\n"
			"\tCycles average - %*lu cycles\n",
		si_printColor3bit(siPrintColor3bit_Green),
		padCycles, time, element->unit, padCycles, (u64)cyclesMedian
	);
}

SIDEF
void si_benchmarkLoopsAvgCmpPrint(cstring names[2], u64 arrays[2][20], usize len,
		usize range[2]) {
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction 1 - '%s'\n"
			"\tFunction 2 - '%s'\n"
		"Runs:\n",
		names[0], names[1]
	);
	const siBenchmarkLimit* elements[2];
	f64 freq = (f64)si_cpuClockSpeed() / 1000.0;

	isize padRuns = si_numLen(range[1]);
	isize padCycles[2] = {
		si_numLen(arrays[0][len - 1]), si_numLen(arrays[1][len - 1])
	};

	usize arrayI = 0, runs;
	for (runs = range[0]; runs <= range[1]; runs *= 10) {
		u64 cycles[2] = {arrays[0][arrayI], arrays[1][arrayI]};
		f64 time[2] = {(f64)cycles[0] / freq, (f64)cycles[1] / freq};

		f64 ratio;
		siPrintColor clr[2];
		if (time[0] > time[1]){
			ratio = time[0] / time[1];
			clr[0] = si_printColor3bit(siPrintColor3bit_Red);
			clr[1] = si_printColor3bit(siPrintColor3bit_Green);
		}
		else if (time[0] < time[1]) {
			ratio = time[1] / time[0];
			clr[0] = si_printColor3bit(siPrintColor3bit_Green);
			clr[1] = si_printColor3bit(siPrintColor3bit_Red);
		}
		else {
			ratio = 1.0;
			clr[0] = clr[1] = si_printColor3bit(siPrintColor3bit_Yellow);
		}

		for_range (j, 0, countof(elements)) {
			elements[j] = si_benchmarkLimitLoop((u64)time[j]);
			time[j] /= (f64)elements[j]->duration;
		}

		si_printf(
			"\t%*zu %s - %C%9.4f%C %s vs %C%9.4f%C %s (%4.4f ratio, %*lu vs %*lu cycles)\n",
			padRuns, runs, (runs != 1) ? "runs" : "run ",
			clr[0], time[0], elements[0]->unit, clr[1], time[1], elements[1]->unit,
			ratio, padCycles[0], cycles[0], padCycles[1], cycles[1]
		);

		arrayI += 1;
	}

	u64 cyclesTotal[2] = {0, 0};
	for_range (i, 0, len) {
		for_range (j, 0, 2) {
			cyclesTotal[j] += arrays[j][i];
		}
	}

	f64 cyclesMedian[2] = {
		(f64)cyclesTotal[0] / (f64)len,
		(f64)cyclesTotal[1] / (f64)len
	};
	f64 time[] = {cyclesMedian[0] / freq, cyclesMedian[1] / freq};

	for_range (i, 0, countof(padCycles)) {
		padCycles[i] = si_numLen((u64)cyclesMedian[i]);
	}

	f64 ratio;
	siPrintColor clr[2];
	if (time[0] > time[1]){
		ratio = time[0] / time[1];
		clr[0] = si_printColor3bit(siPrintColor3bit_Red);
		clr[1] = si_printColor3bit(siPrintColor3bit_Green);
	}
	else if (time[0] < time[1]) {
		ratio = time[1] / time[0];
		clr[0] = si_printColor3bit(siPrintColor3bit_Green);
		clr[1] = si_printColor3bit(siPrintColor3bit_Red);
	}
	else {
		ratio = 1.0;
		clr[0] = clr[1] = si_printColor3bit(siPrintColor3bit_Yellow);
	}

	for_range (j, 0, countof(elements)) {
		elements[j] = si_benchmarkLimitLoop((u64)time[j]);
		time[j] /= (f64)elements[j]->duration;
	}

	si_printf(
		"Final result:\n"
			"\tTime average   - %C%*.4f%C %s vs %C%*.4f%C %s (%4.4f ratio)\n"
			"\tCycles average - %*lu cycles vs %*lu cycles\n",
		clr[0], padCycles[0], time[0], elements[0]->unit,
		clr[1], padCycles[1], time[1], elements[1]->unit,
		ratio,
		padCycles[0], (u64)cyclesMedian[0], padCycles[1], (u64)cyclesMedian[1]
	);
}

void* si__benchmarkThread(void* arg) {
	si_sleep((i32)SI_TO_U32(arg));
	return nil;
}

const siBenchmarkLimit siBenchLimit[] = {
	{(u64)0001, "ns"},
	{(u64)1000, "μs"},
	{(u64)1000 * 1000, "ms"},
	{(u64)1000 * 1000 * 1000, " s"},
	{(u64)1000 * 1000 * 1000 * 60, "min"},
	{(u64)1000 * 1000 * 1000 * 60 * 60, " h"},
	{(u64)1000 * 1000 * 1000 * 60 * 60 * 24, " d"},
	{0, nil}
};

const siBenchmarkLimit* si_benchmarkLimitLoop(u64 time) {
	const siBenchmarkLimit* element = siBenchLimit;
	const siBenchmarkLimit* end = &siBenchLimit[countof(siBenchLimit)];

	while (element != end) {
		if (si_between(u64, time, element->duration, (element + 1)->duration)) {
			break;
		}
		element += 1;
	}

	return element;
}

#endif /* SI_IMPLEMENTATION_BENCHMARK */

#if !defined(SI_COMPILER_MSVC) && defined(SI_USE_ATT_SYNTAX)
	#undef si_asm
	#undef SI_ASM_NL
	#define si_asm(asm, ...)  __asm__ __volatile__(asm __VA_ARGS__)
	#define SI_ASM_NL "\n\t"
#endif

#ifdef __cplusplus
}
#endif

#endif /* SI_INCLUDE_SI_H */

#if defined(SI_NO_ASSERTIONS_IN_HEADER) && !defined(SI_NO_ASSERTIONS)
	#undef SI_ASSERT
	#undef SI_ASSERT_MSG
	#undef SI_ASSERT_FMT
	#undef SI_ASSERT_NOT_NIL
	#undef SI_ASSERT_NOT_NEG

	/* condition - EXPRESSION
	 * Terminates the program if the condition is not met. */
	#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
	/* condition - EXPRESSION | message - cstring
	 * Terminates the program with a message if the condition is not met. */
	#define SI_ASSERT_MSG(condition, message) SI_ASSERT_FMT(condition, message, "")
	/* condition - EXPRESSION | message - cstring | ...fmt - VARIADIC
	 * Terminates the program with a formatted message if the condition is not met. */
	#define SI_ASSERT_FMT(condition, message, ... /* fmt */) \
		SI_STOPIF(!(condition), si_panic(#condition, SI_CALLER_LOC, message, __VA_ARGS__))
	/* ptr - void*
	 * Terminates the program if a pointer is nil. */
	#define SI_ASSERT_NOT_NIL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be nil.")
	/* num - INT
	 * Terminates the program if the integer is negative. */
	#define SI_ASSERT_NOT_NEG(num) SI_ASSERT_MSG((num) > 0, #num " must not be negative.")

#endif /* SI_NO_ASSERTIONS_IN_HEADER */

/*
------------------------------------------------------------------------------
Copyright (C) 2023-2024 EimaMei

This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
------------------------------------------------------------------------------
*/
