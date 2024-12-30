/*
sili.h - v0.1.0 - a general-purpose programming library to replace the C/C++ standard libraries.
===========================================================================
	- YOU MUST DEFINE 'SI_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SI_IMPLEMENTATION
		#include "sili.h"

	- All other files should just include the library without the #define macro.

	- If you want to disable certain features, you can do:
		- #define SI_NO_GENERAL
		- #define SI_NO_MEMORY
		- #define SI_NO_OPTIONAL
		- #define SI_NO_ARRAY
		- #define SI_NO_STRING
		- #define SI_NO_UNICODE
		- #define SI_NO_CHAR
		- #define SI_NO_HASHTABLE
		- #define SI_NO_IO
		- #define SI_NO_THREAD
		- #define SI_NO_TIME
		- #define SI_NO_BIT
		- #define SI_NO_SYSTEM
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
		- TYPE - any type name (siString, usize, rawptr).
		- TYPE* - any type name's pointer type (siString*, usize*, rawptr*).
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

	- SI_NO_SILI_PRINTF_STYLE - disables sili's added features for the 'si_printf'
	types of functions, which may break some output compatibility with the old
	'printf'. The list of said features:
		1. Octal numbers being prefixed with '0o' instead of just '0'.
		2. The string versions of booleans (true/false) being printed via '%B'.
		3. Printing 'siString' types via '%S'.
		4. Having color in the terminal using '%C<letter>' specifiers.

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


#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && !defined(_WIN32))
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

#elif defined(__EMSCRIPTEN__) || defined(__wasm) || defined(__wasm32) || defined(__wasm32__) || defined(__wasm__) || defined(__WASM__)
	#define SI_SYSTEM_WASI 1
	#define SI_SYSTEM_STR "WASI"
	#define SI_SYSTEM_IS_WASM 1

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

#elif defined(__riscv) && __riscv_xlen == 32
	#define SI_ARCH_RISC_V32 1
	#define SI_ARCH_STR "RISC-V 32-bit"

	#define SI_ARCH_IS_RISC 1
	#define SI_ARCH_IS_32BIT 1

#elif defined(__riscv) && __riscv_xlen == 64
	#define SI_ARCH_RISC_V64 1
	#define SI_ARCH_STR "RISC-V 64-bit"

	#define SI_ARCH_IS_RISC 1
	#define SI_ARCH_IS_64BIT 1

#else
	#define SI_ARCH_UNKNOWN 1
	#define SI_ARCH_STR "Unknown"

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


#if !SI_SYSTEM_IS_WINDOWS
	#undef _GNU_SOURCE
	#undef _LARGEFILE64_SOURCE
	#undef __USE_POSIX199506

	#define _GNU_SOURCE
	#define _LARGEFILE64_SOURCE
	#define __USE_POSIX199506 1

	#include <stddef.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <stdio.h>

	#include <pthread.h>
	#include <unistd.h>
	#include <dirent.h>
	#include <dlfcn.h>
	#include <ftw.h>

	#include <time.h>
	#include <memory.h>
	#include <errno.h>

	#include <sys/stat.h>
	#include <sys/fcntl.h>
	#include <sys/time.h>

	#undef _GNU_SOURCE
	#undef _LARGEFILE64_SOURCE
	#undef __USE_POSIX199506
#else
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
#endif

#if SI_SYSTEM_IS_APPLE
	#include <sys/socket.h>
	#include <sys/sysctl.h>
#endif

#if SI_SYSTEM_IS_UNIX
	#include <sys/sendfile.h>
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

SI_STATIC_ASSERT(sizeof(u8) == sizeof(i8));
SI_STATIC_ASSERT(sizeof(u16) == sizeof(i16));
SI_STATIC_ASSERT(sizeof(u32) == sizeof(i32));
SI_STATIC_ASSERT(sizeof(u64) == sizeof(i64));
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

#ifndef noreturn
	#if SI_STANDARD_CHECK_MIN(C, C23) || SI_STANDARD_CHECK_MIN(CPP, CPP11)
		/* Specifies that the function will not return anything. */
		#define noreturn [[noreturn]]

	#elif SI_STANDARD_CHECK_MIN(C, C11)
		/* Specifies that the function will not return anything. */
		#define noreturn _Noreturn

	#else
		#if defined(SI_COMPILER_CLANG) || SI_COMPILER_CHECK_MIN(GCC, 2, 5, 0)
			/* Specifies that the function will not return anything. */
			#define noreturn __attribute__((noreturn))

		#elif defined(SI_COMPILER_MSVC)
			/* Specifies that the function will not return anything. */
			#define noreturn __declspec(noreturn)

		#else
			/* Specifies that the function will not return anything. */
			#define noreturn
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
	/* A pointer type pointing to raw data. */
	typedef void* rawptr;
#endif

#ifndef cstring
	/* A const NULL-terminated C-string type. */
	typedef const char* cstring;
#endif

/*
	========================
	| Constants            |
	========================
*/

/* x - INT.
 * Converts kilobytes into bytes (JEDEC). */
#define SI_KILO(x) ((x) * 1024u)
/* x - INT.
 * Converts megabytes into bytes (JEDEC). */
#define SI_MEGA(x) (SI_KILO(x) * 1024u)
/* x - INT.
 * Converts gigabytes into bytes (JEDEC). */
#define SI_GIGA(x) (SI_MEGA(x) * 1024u)
/* x - INT.
 * Converts terabytes into bytes (JEDEC). */
#define SI_TERA(x) (SI_GIGA(x) * 1024u)

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

#ifndef countof
	/* value - ARRAY
	* Gets the static length of the given value (must be an array). */
	#define countof(.../* value */) (sizeof(__VA_ARGS__) / sizeof((__VA_ARGS__)[0]))
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
		#define offsetof(type, element) ((usize)&(((type*)nil)->element))
	#endif
#endif

#ifndef si_offsetof
	/* type - STRUCT TYPE | element - TYPE's member
	* Returns the offset of the specified member. */
	#define si_offsetof(type, element) ((usize)&(((type*)nil)->element))
#endif

#if !defined(alignof) && (SI_STANDARD_CHECK_MAX(C, C17) || SI_STANDARD_CHECK_MAX(CPP, CPP98))
	/* type - TYPE
	* Gets the alignment of a type. */
	#define alignof(type) offsetof(struct { char c; type member; }, member)
#endif

#ifndef si_alignof
	/* type - TYPE
	* Gets the alignment of a type. */
	#define si_alignof(type) si_offsetof(struct { char c; type member; }, member)
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
		SI_STATIC_ASSERT(sizeof(type) == sizeof(valueType));
	    type out;
		memcpy(&out, &value, sizeof(value));
		return out;
	}
}
#endif

/* ptr - rawptr
 * Converts the pointer's value into an u8. */
#define SI_TO_U8(ptr)  (*si_transmute(u8*, (const rawptr)(ptr), const rawptr))
/* ptr - rawptr
 * Converts the pointer's value into an u16. */
#define SI_TO_U16(ptr) (*si_transmute(u16*, (const rawptr)(ptr), const rawptr))
/* ptr - rawptr
 * Converts the pointer's value into an u32. */
#define SI_TO_U32(ptr) (*si_transmute(u32*, (const rawptr)(ptr), const rawptr))
/* ptr - rawptr
 * Converts the pointer's value into an u64. */
#define SI_TO_U64(ptr) (*si_transmute(u64*, (const rawptr)(ptr), const rawptr))



/*
	========================
	| General macros       |
	========================
*/

#ifdef SI_TYPEOF_USED
	/* a - VARIABLE | b - VARIABLE
	 * Swaps the value of 'a' with 'b'; 'b' with 'a'. */
	#define si_swap(a, b) do { typeof((a)) tmp = (a); (a) = (b); (b) = tmp; } while (0)
	/* countvar - NAME | start - INT | end - INT
	 * A loop that loops from 'start' to 'end' and increments 'countVar' each cycle. */
	#define for_range(countVar, start, end) \
		for (typeof(end) countVar = (start); countVar < (end); countVar += 1)

#else
	/* a - VARIABLE | b - VARIABLE
	 * Swaps the value of 'a' with 'b'; 'b' with 'a'. */
	#define si_swap(a, b) do { \
		char tmp[sizeof(a)]; \
		si_memcopy(tmp, &(a), sizeof(a)); \
		si_memcopy_s(&(a), sizeof(a), &(b), sizeof(b)); \
		si_memcopy_s(&(b), sizeof(b), tmp, sizeof(tmp)); \
	} while (0)

	/* countvar - NAME | start - INT | end - INT
	 * A loop that loops from 'start' to 'end' and increments 'countVar' each cycle. */
	#define for_range(countVar, start, end) \
		for (isize countVar = (start); countVar < (isize)(end); countVar += 1)

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

/*
	========================
	| SI_ASSERT            |
	========================
*/

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

noreturn SIDEF void si_panic(cstring conditionStr, cstring file, i32 line,
		cstring func, cstring message, ...);

#ifdef SI_NO_ASSERTIONS
	#undef SI_NO_ASSERTIONS_IN_HEADER
	#define SI_NO_ASSERTIONS_IN_HEADER
#endif

#ifndef SI_NO_ASSERTIONS_IN_HEADER
	/* condition - EXPRESSION
	 * Crashes the app if the condition is not met. */
	#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
	/* condition - EXPRESSION | message - cstring
	 * Crashes the app with a message if the condition is not met. */
	#define SI_ASSERT_MSG(condition, message) SI_ASSERT_FMT(condition, message, "")
	/* condition - EXPRESSION | message - cstring | ...fmt - VARIADIC
	 * Crashes the app with a formatted message if the condition is not met. */
	#define SI_ASSERT_FMT(condition, message, .../* fmt */) \
		SI_STOPIF(!(condition), si_panic(#condition, __FILE__, __LINE__, __func__, message, __VA_ARGS__))
	/* ptr - rawptr
	 * Crashes the app if a pointer is NULL. */
	#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL.")

#else
	#define SI_ASSERT(condition) do { } while(0)
	#define SI_ASSERT_MSG(condition, message) do {} while(0)
	#define SI_ASSERT_FMT(condition, message, ...) do {} while(0)
	#define SI_ASSERT_NOT_NULL(ptr) do { SI_UNUSED(ptr); } while(0)

#endif /* SI_NO_ASSERTIONS_IN_HEADER */

/* Crashes the app immediately. */
#define SI_PANIC() SI_PANIC_MSG(nil)
/* message - cstring
 * Crashes the app immediately with a message. */
#define SI_PANIC_MSG(message) SI_PANIC_FMT(message, "")
/* message - cstring | ...FMT - VARIADIC
 * Crashes the app immediately with a formatted message. */
#define SI_PANIC_FMT(message, ...) si_panic("SI_PANIC()", __FILE__, __LINE__, __func__, message, __VA_ARGS__)

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
	#define SI_DEFAULT_MEMORY_ALIGNMENT (2 * sizeof(rawptr))
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
SIDEF b32 si_isPowerOfTwo(usize x);
/* Aligns the number to the specified alignment. */
SIDEF usize si_alignForward(usize num, usize alignment);


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
SIDEF rawptr si_memcopy(void* restrict dst, const void* restrict src, usize size);
/* Copies the given amount of bytes from the provided source into the specified
 * destination. The memory blocks can overlap each other. */
SIDEF rawptr si_memmove(void* restrict dst, const void* restrict src, usize size);
/* Sets the given amount of bytes from the provided data source to the specified
 * value. */
SIDEF rawptr si_memset(void* data, u8 value, usize size);
/* Compares the given of bytes from two specified pointers and returns either:
 * A) zero, if all bytes match B) less than zero, if the first clashing byte in
 * the first pointer is lower than in the second one C) more than zero, if the
 * first clashing byte in the first pointer is higher than the second one. */
SIDEF i32 si_memcompare(const void* ptr1, const void* ptr2, usize size);
/* Searches the given amount of bytes from the provided data source and returns
 * either a pointer containing the first occurence of the specified value, or a
 * nil pointer if there were no occurences. */
SIDEF rawptr si_memchr(const void* data, u8 value, usize size);

/* Copies a number of bytes from the source buffer into the destination. The number
 * of copied bytes is guaranteed to be less or equal to the destination's size.
 * The memory blocks cannot overlap each other. Returns the amount of bytes that
 * were copied. */
SIDEF usize si_memcopy_s(void* restrict dst, usize sizeDst, const void* restrict src,
		usize sizeSrc);
/* Copies a number of bytes from the source buffer into the destination. The number
 * of copied bytes is guaranteed to be less or equal to the destination's size.
 * The memory blocks can overlap each other. Returns the amount of bytes that
 * were moved. */
SIDEF usize si_memmove_s(void* restrict dst, usize sizeDst, const void* restrict src,
		usize sizeSrc);

/* Moves the specified memory block to the left by the given amount of bytes. */
SIDEF rawptr si_memmoveLeft(void* src, usize size, isize moveBy);
/* Moves the specified memory block to the right by the given amount of bytes. */
SIDEF rawptr si_memmoveRight(void* src, usize size, isize moveBy);


/* Returns the length of a NULL-terminated C-string. */
SIDEF usize si_cstrLen(cstring string);



#endif /* SI_NO_MEMORY */

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
	rawptr src;
	usize sizeOld;
	usize sizeNew;
} siAllocatorDataResize;

typedef struct siAllocatorFunc {
	siAllocatorFuncEnum type;
	union_anonymous(data,
		usize alloc;
		rawptr free;
		siAllocatorDataResize resize;
	);
} siAllocatorFunc;

/* name - NAME
 * Defines a valid allocator function prototype.*/
#define SI_ALLOCATOR_PROC(name) rawptr name(siAllocatorFunc function, rawptr data)
/* Represents an allocator procedure. */
typedef SI_ALLOCATOR_PROC(siAllocatorProc);

/* Represents an allocator type. */
typedef struct siAllocator { siAllocatorProc* proc; rawptr data; } siAllocator;


/* Allocates the specified amount of bytes of storage. */
SIDEF rawptr si_alloc(siAllocator alloc, usize bytes);
/* Reallocates the specified memory block from the given old size to the new. */
SIDEF rawptr si_realloc(siAllocator alloc, rawptr ptr, usize sizeOld, usize sizeNew);
/* Frees a previously allocated memory block from 'si_alloc'. */
SIDEF void si_free(siAllocator alloc, rawptr ptr);
/* Frees all of the previous allocated memory within the specified allocator.
 * NOTE: Not all allocators can support this function. */
SIDEF void si_freeAll(siAllocator alloc);

/* Returns the available space left inside the specified allocator. */
SIDEF usize si_allocatorAvailable(siAllocator alloc);


/* allocator - siAllocator* | type - TYPE
 * Allocates an exact amount of storage to fit the specified type. */
#define si_allocItem(allocator, type) (type*)si_alloc(allocator, sizeof(type))
/* allocator - siAllocator | type - TYPE | count - usize
 * Allocates an exact amount of storage to fit the specified array of types. */
#define si_allocArray(allocator, type, count) (type*)si_alloc(allocator, sizeof(type) * (count))


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
 * 'si_allocatorAvailable' always returns an 'USIZE_MAX'. */
SIDEF SI_ALLOCATOR_PROC(si_allocator_heap_proc);

/* Returns the heap allocator. */
SIDEF siAllocator si_allocatorHeap(void);

#ifndef si_stack
	/* bytes - usize
	 * Stack allocates the specified amount of bytes of storage. */
	#define si_stack(bytes) si_cast(rawptr, (u8[bytes]){0})
#endif

/* TODO */
#define si_stackAlloc(bytes) si_stack(bytes), (bytes)


#ifndef si_malloc
	/* bytes - usize
	 * Heap allocates the specified amount of bytes of storage. */
	#define si_malloc(bytes) si_alloc(si_allocatorHeap(), bytes)
	/* ptr - rawptr
	 * Deallocates a previously allocated memory block from 'si_malloc'. */
	#define si_mfree(ptr) si_free(si_allocatorHeap(), ptr)
#endif

/* type - TYPE
 * Heap allocates an exact amount of storage to fit the specified type. */
#define si_mallocItem(type) (type*)si_malloc(sizeof(type))
/* type - TYPE | count - usize
 * Heap allocates an exact amount of storage to fit the specified array of types. */
#define si_mallocArray(type, count) (type*)si_malloc(sizeof(type) * (count))

/*
	========================
	| siArena              |
	========================
*/

/* Reprents an arena allocator. */
typedef struct siAllocatorArena {
	siAllocator alloc;
	u8* ptr;
	usize offset;
	usize capacity;
	u32 alignment;
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
SIDEF siAllocatorArena si_arenaMake(siAllocator alloc, usize capacity);
SIDEF siAllocatorArena si_arenaMakeEx(siAllocator alloc, usize capacity, u32 alignment);
SIDEF siAllocatorArena si_arenaMakePtr(rawptr ptr, usize capacity, u32 alignment);

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
	usize offset;
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
 * - si_alloc - the allocator reserves 'aligned(sizeof(usize) + requestedSize)'
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
SIDEF siAllocatorLifo si_lifoMake(siAllocator alloc, usize capacity);
SIDEF siAllocatorLifo si_lifoMakeEx(siAllocator alloc, usize capacity, u32 alignment);
SIDEF siAllocatorLifo si_lifoMakePtr(rawptr ptr, usize capacity, u32 alignment);

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
	usize numChunks;
	usize chunkSize;
	usize actualChunkSize;
	u32 alignment;

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
SIDEF siAllocatorPool si_poolMake(siAllocator alloc, usize numChunks, usize chunkSize);
SIDEF siAllocatorPool si_poolMakeEx(siAllocator alloc, usize numChunks, usize chunkSize, u32 alignment);

/* Returns a pool allocator procedure. */
SIDEF siAllocator si_allocatorPool(siAllocatorPool* pool);

/* Destroys a pool allocator. */
SIDEF void si_poolFree(siAllocatorPool* pool);


//SIDEF SI_ALLOCATOR_PROC(si_allocator_scratch_proc);

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
	/* The file where the error was declared. */
	cstring filename;
	/* The time when the error was declared (in UTC+0). */
	i64 time;
#endif
} siError;


/* errVar - <nullable> siError* | errCode - i32
 * Writes error-related information into the error variable. If logging is enabled,
 * the default error log function is executed. */
#define SI_ERROR(errVar, errCode) SI_ERROR_EX(errVar, errCode, si_errorLogDefault)
/* Sets the error code to zero. */
#define SI_ERROR_DEFAULT (siError){.code = 0}

/* cond - b32 | errVar - <nullable> siError* | errCode - i32 | ..action - ACTION
 * If the specified condition evaluates to true, an error is declared and the given
 * action will be executed. Error-related information is written into the
 * error variable. If logging is enabled, the default error log function is executed. */
#define SI_ERROR_CHECK(cond, errVar, errCode, .../*action*/) \
	SI_ERROR_CHECK_EX(cond, errVar, errCode, si_errorLogDefault, __VA_ARGS__)
/* cond - b32 | errVar - <nullable> siError* | errCode - i32 | func - FUNCTION | ..action - ACTION
 * If the specified condition evaluates to true, an error is declared and the given
 * action will be executed. Error-related information is written into the error
 * variable. If logging is enabled, the given error log function is executed. */
#define SI_ERROR_CHECK_EX(cond, errVar , errCode, func, .../*action*/) \
	SI_STOPIF(cond, SI_ERROR_EX(errVar, errCode, func); __VA_ARGS__);


#ifndef SI_NO_ERROR_STRUCT
	/* err - siError* | errCode - i32 | func - FUNCTION
	 * Writes error-related information (error code, time, line number and
	 * filename) into the error variable. If logging is enabled, the given error
	 * log function is executed. */
	#define SI_ERROR_EX(err, errCode, func) \
		do { \
			if (err != nil) { \
				(err)->line = __LINE__; \
				(err)->filename = __FILE__; \
				(err)->code = errCode; \
				(err)->time = si_timeNowUTC(); \
			} \
			SI_ERROR_LOG(err, func); \
		} while (0)

#else
	/* err - siError* | errCode - i32 | function - func
	 * Writes error-related information (error code) into the error variable. If
	 * logging is enabled, the given error log function is executed. */
	#define SI_ERROR_EX(error, errorCode, func) \
		do { \
			if (error != nil) { \
				(error)->code = errorCode;  \
			} \
			SI_ERROR_LOG(si__errorCode, func);
		} while(0)

#endif

#ifndef SI_NO_ERROR_LOGS
	/* err - siError* | func - FUNCTION
	 * Executes the given error log function if error logging is enabled. */
	#define SI_ERROR_LOG(err, func) func(err)

#else
	/* error - siError* | func - FUNCTION
	 * Does nothing since error logging is disabled. */
	#define SI_ERROR_LOG(err, func) do {} while (0)

#endif

/* The default error logging function when an error occurs. */
SIDEF void si_errorLogDefault(siError* error);


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
 * Creates a returnable 'siOptional' value, with the data being set to zero. */
#define SI_OPT_NIL(type) SI_OPT_ERR(type, 0)

/* type - TYPE | errorV - siError
 * Creates a returnable 'siOptional' error value, with the data being set to the
 * error.*/
#define SI_OPT_ERR(type, errorV) (siOption(type)){false, .data = {.error = errorV}}


/* optionalVar - siOptional(TYPE) | defaultValue - EXPRESSION
 * Gets the value of the provided 'siObject' object. However if 'hasValue' is set
 * to false, return 'defaultValue' instead. */
#define si_optionalGetOrDefault(optionalVar, defaultValue) \
	((optionalVar).hasValue ? (optionalVar).data.value : (defaultValue))


#if 1
	#define si__OptionMacro(type) siOption_##type
#endif

#endif /* SI_NO_OPTIONAL */

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

typedef struct siBuffer {
	rawptr data;
	usize len;
	usize capacity;
	usize typeSize;
	siAllocator alloc;
#ifdef SI_USE_BUFFER_GROW
	usize grow;
#endif
} siBuffer;

/* Defines a 'siBuffer' optional type. */
si_optional_define(siBuffer);

/* type - TYPE
 * Represents a buffer with a specific type. */
#define siArray(type) siBuffer

#ifndef SI_BUFFER_NEW_CAP
	/* bufferPtr - siBuffer* | addLen - isize
	 * Determines how much memory should be allocated for the new buffer's capacity.
	 * Default non-grow capacity: '2 * (capacity + addLen)'.
	 * Default grow capacity: 'capacity + addLen + grow'. */
	#ifndef SI_USE_BUFFER_GROW
		#define SI_BUFFER_NEW_CAP(bufferPtr, addLen) (2 * ((bufferPtr)->capacity + addLen))
	#else
		#define SI_BUFFER_NEW_CAP(bufferPtr, addLen) ((bufferPtr)->capacity + addLen + (bufferPtr)->grow)
	#endif
#endif


/* type - TYPE | ...values - ANYTHING
 * Makes an array from the given type and values. */
#define SI_BUF(type, .../* values*/) SI_BUF_LEN(type, ((type[]){__VA_ARGS__}), countof((type[]){__VA_ARGS__}))
/* type - TYPE | buffer - rawptr | length - usize
 * Makes an array from the specified type, pointer and length. Array is assumed
 * to be static (eg. no allocator). Length is assumed to be the capacity as well. */
#define SI_BUF_LEN(type, buffer, length) SI_BUF_ALL(type, buffer, length, {0})
/* type - TYPE | buffer - rawptr | length - usize | alloc - siAllocator
 * Makes an array from the specified type, pointer, length and allocator. Length
 * is assumed to be the capacity as well. */
#define SI_BUF_ALL(type, buffer, length, alloc) \
	(siBuffer){buffer, length, length, sizeof(type), alloc}

/* bytes - usize
 * Stack allocates the specified amount of bytes and creates a siBuffer from it. */
#define SI_BUF_STACK(bytes) SI_BUF_LEN(u8, si_salloc(bytes), bytes)



#ifdef SI_RELEASE_MODE
	/* array - siBuffer | index - usize | type - TYPE
	 * Returns an element at the specified index of an array. No bound or type
	 * checking is done. */
	#define si_arrayGet(array, index, type) (((type*)(array).data)[index])
#else
	/* array - siBuffer | index - usize | type - TYPE
	 * Returns an element at the specified index of an array. Bound and type
	 * checks are done. */
	#define si_arrayGet(array, index, type) (*(type*)si__arrayGet(&(array), index, sizeof(type), __FILE__, __LINE__))
	SIDEF rawptr si__arrayGet(const siBuffer* buffer, usize index, usize sizeof_type, cstring file, i32 line);
#endif

/* array - siBuffer | type - TYPE
 * Returns the last element of a specified typed array. */
#define si_arrayGetLast(array, type) si_arrayGet(array, (array).len - 1, type)

/* array - siBuffer | type - TYPE
 * Returns a pointer to the given array's element. No bound checking is done. */
#define si_arrayGetPtr(array, index) ((u8*)(array).data + (array).typeSize * (index))


/* type - TYPE | name - NAME | array - siBuffer
 * Loops through every element of the specified array. */
#define for_eachArr(type, name, array) \
	SI_ASSERT_MSG(sizeof(type) == (array).typeSize, "Invalid type specified."); \
	for (type* name = &si_arrayGet(array, 0, type); name < (type*)(array).data + (array).len; name += 1) \
/* type - TYPE | name - NAME | array - siBuffer
 * Loops through every element of the specified array in reverse. */
#define for_eachRevArr(type, name, array) \
	SI_ASSERT_MSG(sizeof(type) == (array).typeSize, "Invalid type specified."); \
	for (type* name = &si_arrayGet(array, (array).len - 1, type); name >= &si_arrayGet(array, 0, type); name -= 1)


/* alloc - siAllocator | type - TYPE | ...values - VARIADIC
 * Allocates an array from the specified type and static items. */
#define si_arrayMake(alloc, type, .../* values */) \
	si_arrayMakeEx((type[]){__VA_ARGS__}, sizeof(type), countof((type[]){__VA_ARGS__}), alloc)
/* Allocates 'count * sizeofItem' amount of bytes to make an array and copies the
 * same amount of bytes from the specified list to it, whilst setting the length
 * to 'count'. */
SIDEF siBuffer si_arrayMakeEx(rawptr list, usize sizeofItem, usize count, siAllocator alloc);
/* Allocates 'capacity * sizeofItem' amount of bytes to make an array, whilst
 * setting the length to the specified size. */
SIDEF siBuffer si_arrayMakeReserve(usize sizeofItem, usize length, usize capacity,
		siAllocator alloc);
/* Creates a siArray from another siArray. */
SIDEF siBuffer si_arrayCopy(siBuffer array, siAllocator alloc);

/* Frees the allocated array. */
SIDEF void si_arrayFree(siBuffer array);


/* Copies an element at the specified index into the given pointer and returns
 * true if the index is less than the array's length. Otherwise, 'false' is returned. */
SIDEF b32 si_arrayAt(siBuffer array, usize index, rawptr outPtr);
/* Copies the first element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF b32 si_arrayAtFront(siBuffer array, rawptr outPtr);
/* Copies the last element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF b32 si_arrayAtBack(siBuffer array, rawptr outPtr);
/* Returns a pointer to the first element (&array[0]) in the array. */
SIDEF rawptr si_arrayBegin(siBuffer array);
/* Returns a pointer to the past-the-end element (&array[array.len]) in the array. */
SIDEF rawptr si_arrayEnd(siBuffer array);



/* Searches for the specified pointer's content in the array (from the beginning).
 * If found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_arrayFind(siBuffer array, rawptr valuePtr);
/* Searches for the specified pointer's content in the array (from the end). If
 * found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_arrayFindLast(siBuffer array, rawptr valuePtr);

/* Returns the amount of times the specified pointer's value repeats in the array. */
SIDEF usize si_arrayFindCount(siBuffer array, rawptr valuePtr);


/* Returns true if the two specified arrays are equal in length and contents. */
SIDEF b32 si_arrayEqual(siBuffer lhs, siBuffer rhs);


/* Appends the specified pointer's value to the array. Returns true if the array
 * was reallocated. */
SIDEF b32 si_arrayAppend(siBuffer* array, rawptr valuePtr);
/* Appends the specified list of items to the array. Returns true if the array
 * was reallocated. */
SIDEF b32 si_arrayAppendEx(siBuffer* array, rawptr valueArray, usize count);
/* Erases the last item in the array. */
SIDEF void si_arrayPop(siBuffer* array);
/* Sets the array's length to zero. */
SIDEF void si_arrayClear(siBuffer* array);

/* Inserts the specified pointer's value at the given index of the array. Returns
 * true if the array was reallocated. */
SIDEF b32 si_arrayInsert(siBuffer* array, usize index, rawptr value);
/* Inserts the specified list of items at the given index of the array. Returns
 * true if the array was reallocated. */
SIDEF b32 si_arrayInsertEx(siBuffer* array, usize index, rawptr valueArray, usize count);
/* Erases the specified pointer's value at the given index of the array. */
SIDEF void si_arrayErase(siBuffer* array, usize index);
/* Erases the specified list of items at the given index of the array. */
SIDEF void si_arrayEraseEx(siBuffer* array, usize index, usize count);

/* Reverses the contents of the array. */
SIDEF void si_arrayReverse(siBuffer array);
/* Fills the contents of the array with the specified pointer's value. Returns
 * true if the array was reallocated. */
SIDEF b32 si_arrayFill(siBuffer* array, usize index, usize count, rawptr valuePtr);

/* Replaces all occurences of the firstly specified pointer's value with the
 * secondly specified one. */
SIDEF void si_arrayReplaceAll(siBuffer array, rawptr restrict valueOld,
		rawptr restrict valueNew);
/* Replaces a given amount of the firstly specified pointer's value with the
 * secondly specified one. */
SIDEF void si_arrayReplace(siBuffer array, rawptr restrict valueOld,
		rawptr restrict valueNew, isize amount);

/* If needed, reallocates the array for the added space. Returns true if the
 * array was reallocated. Used internally. */
SIDEF b32 si_arrayMakeSpaceFor(siBuffer* buffer, usize addLen);


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

/* Represents a string type. */
typedef siBuffer siString;
/* Defines a 'siString' optional type. */
si_optional_define(siString);

/* name - NAME |  string- siString
 * Iterates through the provided string and creates a pointer to the current indexed
 * character via the given name. */
#define for_eachStr(name, string) for_eachArr(u8, name, string)
/* name - NAME |  string- siString
 * Iterates through the provided string in reverse and creates a pointer to the
 * current indexed character via the given name. */
#define for_eachRevStr(name, string) for_eachRevArr(u8, name, string)


/* string - cstring
 * Makes a string from a static NULL-terminated C-string. */
#define SI_STR(string) SI_STR_LEN(string, countof_str(string))
/* string - STR | len - usize
 * Makes a string from a length-specified C-string. */
#define SI_STR_LEN(string, len) SI_BUF_LEN(u8, (u8*)string, len)
/* string - char*
 * Creates a string from a NULL-terminated C-string. */
#define SI_CSTR(string) SI_STR_LEN((char*)string, si_cstrLen(string))
/* string - cstring
 * Creates a compile-time constant string from a static NULL-terminated C-string. */
#define SI_STRC(string) {string, countof_str(string), countof_str(string), 1, {0}}
/* TODO */
#define SI_STR_EMPTY SI_STR_LEN("", 0)


/* string - siString | index - usize
 * Returns a character at the specified index of the string. */
#define si_stringGet(string, index) si_arrayGet(string, index, u8)

/* Allocates a string from a NULL-terminated C-string. */
SIDEF siString si_stringMake(cstring string, siAllocator alloc);
/* Allocates a string from a C-string with a specified length. */
SIDEF siString si_stringMakeLen(cstring string, usize len, siAllocator alloc);
/* Allocates a string with a specified capacity, and copies the contents of a
 * C-string with a specified length. */
SIDEF siString si_stringMakeEx(cstring string, usize len, usize capacity,
		siAllocator alloc);
/* Allocates a string with a reserved capacity with a set length. */
SIDEF siString si_stringMakeReserve(usize length, usize capacity, siAllocator alloc);

/* Allocates a duplicated string. */
SIDEF siString si_stringCopy(siString from, siAllocator alloc);
/* Allocates a NULL-terminated C-string from a string. */
SIDEF char* si_stringCopyToCstr(siString from, siAllocator alloc);
/* Allocates a NULL-terminated C-string with a specified capacity from a string. */
SIDEF char* si_stringCloneToCstrEx(siString from, usize capacity, siAllocator alloc);

/* Frees the string */
SIDEF void si_stringFree(siString string);


/* Gets the character at a specified index. Returns '-1' if the index is invalid. */
SIDEF i32 si_stringAt(siString string, usize index);
/* Gets the string's first character. */
SIDEF i32 si_stringAtFront(siString string);
/* Gets the string's last character. */
SIDEF i32 si_stringAtBack(siString string);
/* Gets a pointer to the string's first character. */
SIDEF u8* si_stringBegin(siString string);
/* Gets pointer to the string's past-the-end element. */
SIDEF u8* si_stringEnd(siString string);


/* string - siString | begin - usize | len - usize
 * Returns a substring of the string from the specified beginning with a length. */
#define si_stringSub(string, begin, len) SI_STR_LEN(&si_stringGet(string, begin), len)
/* string - siString | begin - usize
 * Returns a substring of the string from the specified beginning to the end of
 * string. */
#define si_stringSubToEnd(string, begin) si_stringSub(string, begin, (string).len - (begin))


/* Searches for the specified substring in the string (from the beginning). If
 * found, the value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFind(siString string, siString subStr);
/* Searches for the specified byte in the string (from the beginning). If found,
 * the value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFindByte(siString string, u8 byte);
/* Searches for the specified UTF-8 character in the string (from the beginning).
 * If found, the value's string index is returned and the value's UTF-8 character
 * index is written to the given pointer. Otherwise, '-1' is returned while '-1'
 * is written to the given pointer. . */
SIDEF isize si_stringFindUtf8(siString string, const u8* character, isize* outUtf8Index);

/* Searches for the specified substring in the string (from the end). If found,
 * the value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFindLast(siString string, siString subStr);
/* Searches for the specified byte in the string (from the end). If found, the
 * value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFindLastByte(siString string, u8 byte);

/* Returns the total occurences of the specified substring in the main string. */
SIDEF usize si_stringFindCount(siString string, siString subStr);


/* Compares two strings and returns true if they're equal. */
SIDEF b32 si_stringEqual(siString lhs, siString rhs);
/* Compares two strings. Returns either: A) '0' if both strings are equal;
 * B) '-1' if the largest differentiating byte is 'lhs' string's; C) '1' otherwise. */
SIDEF i32 si_stringCompare(siString lhs, siString rhs);

/* Appends a string to the main string. Returns true if the string was reallocated. */
SIDEF b32 si_stringAppend(siString* string, siString other);
/* Appends a sistring to the main string. Returns true if the string was reallocated. */
SIDEF b32 si_stringSet(siString* string, siString str);
/* Joins all specified strings into one and writes it into an allocated string. */
SIDEF siString si_stringJoin(siArray(siString) strings, siString separator,
		siAllocator alloc);

/* Cuts every front and back occurences of the provided cut set in the string. */
SIDEF void si_stringTrim(siString* string, siString cutSet);
/* Cuts every front occurences of the provided cut set in the string. */
SIDEF void si_stringTrimLeft(siString* string, siString cutSet);
/* Cuts back occurences of the provided cut set in the string. */
SIDEF void si_stringTrimRight(siString* string, siString cutSet);
/* Removes any leading or trailing spaces and newlines in the string. */
SIDEF void si_stringStrip(siString* string);

/* Enquotes the string. */
SIDEF b32 si_stringEnquote(siString* string);
/* Adds a character to the string. Returns true if the string was reallocated. */
b32 si_stringPush(siString* string, char other);
/* Pops the last string's character. */
void si_stringPop(siString* string);
/* Sets the string's leangth to zero. */
void si_stringClear(siString* str);

/* Inserts a substring at the specified index of a string. Returns true if the
 * original string was reallocated. */
SIDEF b32 si_stringInsert(siString* string, siString subStr, usize index);
/* Erases a substring from the specified index. */
SIDEF void si_stringErase(siString* string, usize index, usize count);

/* Reverses the string. */
SIDEF void si_stringReverse(siString string);

/* Replaces all occurences of the old string with the new one. Return true if the
 * original string reallocated. */
SIDEF b32 si_stringReplaceAll(siString* string, siString strOld, siString strNew,
		siAllocator alloc);
/* Replaces a specified amount of the old string with the new one. Return true
 * if the original string reallocated. If 'amount' is negative, all occurences
 * get replaced. */
SIDEF b32 si_stringReplace(siString* string, siString strOld, siString strNew,
		isize amount, siAllocator alloc);
/* Splits the string into multiple _substrings_ based on the separator. Returns
 * nil if the specified delimiter wasn't found in the string.
 *
 * NOTE: The elements of the array are all substrings, not newly allocated strings.
 * This means that if the original string gets freed, the elements would then
 * point to a freed memory region. */
SIDEF siArray(siString) si_stringSplit(siString string, siString delimiter,
	siAllocator alloc);
/* Splits the string into a specified amount _substrings_ based on the separator.
 * Returns nil if the specified delimiter wasn't found in the string.
 *
 * NOTE: The elements of the array are all substrings, not newly allocated strings.
 * This means that if the original string gets freed, the elements would then
 * point to a freed memory region. */
SIDEF siArray(siString) si_stringSplitEx(siString string, siString delimiter,
	isize amount, siAllocator alloc);

/* If needed, reallocates the string for the added space. Returns true if the
 * string was reallocated. Used internally. */
SIDEF b32 si_stringMakeSpaceFor(siString* string, usize addLen);

#ifndef SI_NO_MEMORY

/* Copies a string into the specified pointer (see 'si_memcopy' for more detail). */
SIDEF rawptr si_memcopyStr(void* dst, siString src);
/* Moves a string into the specified pointer (see 'si_memmove' for more detail). */
SIDEF rawptr si_memmoveStr(void* dst, siString src);
/* Compares a string with the specified pointer (see 'si_memcompare' for more detail). */
SIDEF i32 si_memcompareStr(const void* dst, siString src);

/* Copies a string into the specified pointer (see 'si_memcopy_s' for more detail). */
SIDEF usize si_memcopyStr_s(void* dst, usize dstSize, siString src);
/* Moves a string into the specified pointer (see 'si_memmove_s' for more detail). */
SIDEF usize si_memmoveStr_s(void* dst, usize dstSize, siString src);

#endif

/* An integer to character lookup table (eg. SI_NUM_TO_CHAR_TABLE[0] would return
 * '0', wheare as an index of 10 would return 'A'/'a'). */
extern const char* SI_NUM_TO_CHAR_TABLE;

/* Capitalizes each string's letter. */
SIDEF void si_stringUpper(siString string);
/* Decapitalizes each string's letter. */
SIDEF void si_stringLower(siString string);
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


/* Allocates a string from an unsigned integer in base 10. */
SIDEF siString si_stringFromUInt(u64 num, siAllocator alloc);
/* Allocates a string from an unsigned integer in the specified base. */
SIDEF siString si_stringFromUIntEx(u64 num, i32 base, siAllocator alloc);
/* Allocates a string from a signed integer in base 10. */
SIDEF siString si_stringFromInt(i64 num, siAllocator alloc);
/* Allocates a string from a signed integer in the specified base. */
SIDEF siString si_stringFromIntEx(i64 num, i32 base, siAllocator alloc);


/* Allocates a string from a float with 6 digits after the decimal point in base 10. */
SIDEF siString si_stringFromFloat(f64 num, siAllocator alloc);
/* Allocates a string from a float with the given amount of digits after the
 * decimal point in the specified base. */
SIDEF siString si_stringFromFloatEx(f64 num, i32 base, u32 afterPoint, siAllocator alloc);

/* Returns a 'true'/'false' string from a boolean. */
SIDEF siString si_stringFromBool(b32 boolean);
/* Returns either 'true', 'false' or UINT32_MAX from a string. 'UINT32_MAX'
 * is returned if the given string cannot be converted into a boolean. */
SIDEF b32 si_stringToBool(siString string);

/* Checks if the string has the specified prefix. */
SIDEF b32 si_stringHasPrefix(siString string, siString prefix);
/* Returns the amount of matching front characters between the specified strings. */
SIDEF usize si_stringPrefixLen(siString string, siString prefix);

/* Checks if the string has the specified suffix. */
SIDEF b32 si_stringHasSuffix(siString string, siString suffix);
/* Returns the amount of matching back characters between the specified strings. */
SIDEF usize si_stringSuffixLen(siString string, siString suffix);

/* Allocates a formatted string that represents the contesnt of an array based
 * on the 'fmt' string. For example, if 'fmt = "<%x, %hhu, %#b>' and
 * 'array = (u8){255, 64, 4}, the string would become '{<0xFF, 64, 0b100>}'. */
SIDEF siString si_stringFromArray(siBuffer array, cstring fmt, u8* out,
		usize capacity);



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
typedef siArray(u16) siUtf16String;


/* A UTF-32 encoded '�' character for reporting invalid states. */
#define SI_UNICODE_INVALID_UTF32 (siUtf32Char){0xFFFD, 3}
/* A UTF-8 encoded '�' character for reporting invalid states. */
#define SI_UNICODE_INVALID_UTF8 (siUtf8Char){{0xEF, 0xBF, 0xBD}, 3}


typedef struct {
	/* The codepoint (UTF-8 encoded). */
	char codepoint[4];
	/* Length of 'codepoint' in bytes. */
	u32 len;
} siUtf8Char;

typedef struct {
	/* The codepoint (UTF-32 encoded). */
	i32 codepoint;
	/* Length of the character in UTF-8. */
	u32 len;
} siUtf32Char;


/* Decodes the given UTF-8 character into UTF-32 and returns a 'siUtf32Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf32Char){SI_UNICODE_INVALID, 0}. */
SIDEF siUtf32Char si_utf8Decode(const u8* character);
/* Encodes the specified UTF-32 character into UTF-8 and returns a 'siUtf8Char'
 * structure. This function does not check if the UTF-32 is invalid. */
SIDEF siUtf8Char si_utf8Encode(i32 codepoint);

/* Returns the amount of characters in an UTF-8 string. For example, an input
 * string of "Ąsotis" would returns 7 from 'si_cstrLen', while 'si_utf8StrLen'
 * would returns 6. */
SIDEF usize si_utf8StrLen(siUtf8String string);
/* Returns a UTF-32 character at the specified _character_ index.
 * Example: 'si_utf8StrAt("リトアニア", 2)' would return {0x30A2, 3}. */
SIDEF siUtf32Char si_utf8StrAt(siUtf8String string, usize charIndex);
/* Returns a pointer to the specified _character_ index.
 * Example: 'si_utf8StrAt("リトアニア", 2)' would return {&str[7], 3}. */
SIDEF siString si_utf8StrAtUTF8(siUtf8String string, usize charIndex);

/* Encodes a UTF-8 string into a UTF-16 string _without_ a NULL-terminator. */
SIDEF siUtf16String si_utf8ToUtf16Str(siUtf8String str, u16* out, usize capacity);
/* Encodes a UTF-8 string into a UTF-16 string with an option to make it
 * NULL-terminated. */
SIDEF siUtf16String si_utf8ToUtf16StrEx(siUtf8String str, b32 nullTerm, u16* out,
		usize capacity);
/* Encodes a UTF-16 string into a UTF-8 string _without_ a NULL-terminator. */
SIDEF siUtf8String si_utf16ToUtf8Str(siUtf16String str, u8* out, usize capacity);
/* Encodes a UTF-16 string into a UTF-8 string with an option to make it
 * NULL-terminated. */
SIDEF siUtf8String si_utf16ToUtf8StrEx(siUtf16String str, b32 nullTerm, u8* out,
		usize capacity);


/* Decodes the given UTF-16 character into UTF-32 and returns a 'siUtf32Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf32Char){SI_UNICODE_INVALID, 0}. */
SIDEF siUtf32Char si_utf16Decode(const u16 character[2]);
/* Decodes the given UTF-16 character into UTF-8 and returns a 'siUtf8Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf8Char){SI_UNICODE_INVALID, 0}. */
SIDEF siUtf8Char si_utf16Encode(const u16 character[2]);

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
	| siChar               |
	========================
*/

/* Maximum amount of allowed ASCII characters. */
#define SI_ASCII_MAX 0x7F

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
	rawptr value;
	/* Points to the next valid item. */
	struct siHashEntry* next;
} siHashEntry;

typedef siArray(siHashEntry) siHashTable;

/* type - TYPE
 * Denotes that this is a 'siHashTable' variable. */
#define siHt(type) siHashTable

/* Creates a hash table using the given names and data. */
siHashTable si_hashtableMake(const rawptr* keyArray, usize keyLen,
		const rawptr* dataArray, usize len, siAllocator alloc);
/* Reserves a 'capacity' amount of items for the hash table. */
siHashTable si_hashtableMakeReserve(usize capacity, siAllocator alloc);
/* Returns the key entry's value pointer from the hash table. If not found, nil
 * is returned. */
rawptr si_hashtableGet(siHashTable ht, const rawptr key, usize keyLen);
/* ht - siHashTable* | key - cstring | type - TYPE
 * Returns the key entry's value as the specified type. If the item does NOT exist,
 * this will 100% crash, as the return of 'si_hashtableGet' will be nil. */
#define si_hashtableGetItem(ht, key, type) (*((type*)si_hashtableGet(ht, key)))
/* Returns the key entry's value pointer from the hash table. If not found, nil
 * is returned. */
rawptr si_hashtableGetWithHash(siHashTable ht, u64 hash);
/* Adds a 'key' entry to the hash table and returns the entry pointer, regardless
 * if it's a new or pre-existing one. 'outSuccess' is set to true if the hash
 * didn't exist before the set, otherwise it's set to 'false' and the entry's
 * value's gets overwritten either way (unless disabled via a macro). 'outSuccess'
 * can be nullable.*/
siHashEntry* si_hashtableSet(siHashTable* ht, const rawptr key, usize keyLen,
		const rawptr valuePtr, b32* outSuccess);
siHashEntry* si_hashtableSetWithHash(siHashTable* ht, u64 hash, const rawptr valuePtr,
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

	/* An unspecified error occurred. */
	siErrorSystem_Generic,
	/* Total amount of valid errors. */
	siErrorSystem_Length,
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


/* Returns the name of a system error. */
SIDEF siString si_systemErrorName(siErrorSystem err);
/* Returns the description of a system error. */
SIDEF siString si_systemErrorDesc(siErrorSystem err);


/* Ends the program with a to be returned code. */
noreturn SIDEF void si_exit(u32 code);

/* Sets an environment variable for the current process. */
SIDEF b32 si_envVarSet(siString name, siString value);
/* Unsets an environment variable for the current process. */
SIDEF b32 si_envVarUnset(siString name);
/* Gets an environment variable for the current process. If the specified variable
 * name doesn't exist, the returned string's data is set to nil. */
SIDEF siString si_envVarGet(siString name, u8* out, usize capacity);


/* Returns the current running Windows OS version. Returns '0' if the version is
 * older than XP, '-1' if this function is executed on a non-Windows OS. */
SIDEF siWindowsVersion si_windowsGetVersion(void);

/* Returns true if the current Unix system is running Wayland. */
SIDEF b32 si_unixIsWayland(void);
/* Returns true if the current Unix system is running X11. */
SIDEF b32 si_unixIsX11(void);

/* Returns the current running desktop environment on the system. If the found
 * desktop environment isn't recognized by Sili, 0 is returned. */
SIDEF siUnixDE si_unixGetDE(void);


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

	#if SI_SYSTEM_IS_WINDOWS
		/* OS-specific handle of the file. */
		rawptr handle;
	#else
		/* OS-specific handle of the file. */
		isize handle;
	#endif
	/* Size of the read file. */
	usize size;
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
	const char SI_PATH_SEPARATOR = '\\';
#else
	const char SI_PATH_SEPARATOR = '/';
#endif

/*
	========================
	|  siPath              |
	========================
*/


/* Checks if the specified path exists. */
SIDEF b32 si_pathExists(siString path);
/* Copies the specified source path to the destination. Returns the size of the
 * file if the copy was succesful. */
SIDEF siResult(usize) si_pathCopy(siString pathSrc, siString pathDst);
/* Copies every file and folder from the specified source to the given destination.
 * Returns the amount of files copied, otherwise an error if the function failed. */
SIDEF siResult(u32) si_pathItemsCopy(siString pathSrc, siString pathDst);
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
SIDEF siError si_pathReadContentsBuf(siString path, siBuffer* out);

/* Checks if the specified path is absolute. */
SIDEF b32 si_pathIsAbsolute(siString path);
/* Checks if the specified path is relative. */
SIDEF b32 si_pathIsRelative(siString path);

/* TODO */
SIDEF siUtf16String si_pathToWin32(siString path, u16* out, usize capacity);
/* TODO */
SIDEF void si_pathToWin32Mul(siString pathSrc, siString pathDst, u16* outBuf, usize capacity,
		siUtf16String out[2]);
/* TODO */
SIDEF char* si_pathToUnix(siString path, u8* out, usize capacity);
/* TODO */
void si_pathToUnixMul(siString pathSrc, siString pathDst, u8* outBuf, usize capacity,
		char* out[2]);



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
SIDEF usize si_fileSize(siFile file);
/* Updates the size of the file in the structure. */
SIDEF void si_fileSizeUpdate(siFile* file);

/* Allocates the specified amount of bytes, reads said amount (if possible) from
 * the current file offset and writes it to the buffer before returning it. */
SIDEF siResult(siBuffer) si_fileRead(siFile file, usize len, siAllocator alloc);
SIDEF siError si_fileReadBuf(siFile file, usize len, siBuffer* out);
/* Allocates the specified amount of bytes, reads said amount (if possible) from
 * the specified offset and writes it to the buffer before returning it. */
SIDEF siResult(siBuffer) si_fileReadAt(siFile file, isize offset, usize len,
		siAllocator alloc);
SIDEF siError si_fileReadAtBuf(siFile file, isize offset, usize len, siBuffer* out);

/* Allocates the specified amount of bytes, reads said amount (if possible) from
 * the specified offset and writes it to the buffer before returning it. The
 * function doesn't check for if the buffer has enough capacity.*/
SIDEF siError si_fileReadUnsafe(siFile file, isize offset, usize len, siBuffer* out);

/* Allocates 'file.size' bytes, reads said amount (if possible) from the file's
 * beginning and writes it to the buffer before returning it. _File seek offset
 * does not get changed when calling the function._ */
SIDEF siString si_fileReadContents(siFile file, siAllocator alloc);
SIDEF siError si_fileReadContentsBuf(siFile file, siString* out);

/* Allocates 'file.size' bytes, reads said amount (if possible) from the file's
 * beginning and then splits the string into an array of string view lines.
 * _File seek offset does not get changed when calling the function._ */
SIDEF siArray(siString) si_fileReadlines(siFile file, siAllocator alloc);

/* Writes a string to the file at the current offset. */
SIDEF isize si_fileWrite(siFile* file, siBuffer string);
/* Writes a string to the file at the specified offset. */
SIDEF isize si_fileWriteAt(siFile* file, siBuffer content, isize offset);

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
SIDEF b32 si_fileTruncate(siFile* file, usize size);
/* Returns the last time the file was written. */
SIDEF u64 si_fileLastWriteTime(siFile file);

/* Closes the file. */
SIDEF void si_fileClose(siFile file);

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
	rawptr handle;
	usize directoryLen;
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
#define SI_THREAD_PROC(name) rawptr name(rawptr data)
/* Represents a thread function. */
typedef SI_THREAD_PROC(siThreadFunction);


SI_ENUM(u32, siThreadState) {
	siThreadState_Closed = 0,
	siThreadState_Initialized,
	siThreadState_Running,
};

typedef struct siThread {
	#if SI_SYSTEM_IS_WINDOWS
		/* Win32 thread ID. */
		HANDLE id;
	#else
		/* POSIX thread ID. */
		pthread_t id;
	#endif

	/* Pointer to the function. */
	siThreadFunction* func;
	/* The functions' argument. */
	rawptr arg;

	/* Dictates how much stack size gets allocated for th thread. Setting this
	 * to 0 makes the system give the default amount of stack size to the thread. */
	usize stackSize;

	/* TODO */
	volatile siThreadState state;

	/* The raw pointer return of the function. */
	rawptr returnValue;
} siThread;


/* thread - siThread | type - TYPE
 * Casts 'thread.returnValue' correctly to match the given type instead of it being
 * rawptr. */
#define si_threadGetReturn(thread, type) si_transmute(type, (thread).returnValue, rawptr)


/* Prepares a thread data strucutre. */
SIDEF siThread si_threadMake(siThreadFunction function, rawptr arg);
/* Prepares a thread data strucutre wth the option to specify the stack size.
 * By default this is set to zero.*/
SIDEF siThread si_threadMakeEx(siThreadFunction function, rawptr arg, usize stackSize);
/* Prepares a thread data structure, writes it to 'out' and runs the thread. */
SIDEF siError si_threadMakeAndRun(siThreadFunction function, rawptr arg, siThread* out);

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


/* How many clocks of 'si_clock()' are equivalent to a second (1s = 1e+9 ns). */
#define SI_CLOCKS_S (1000000000)
/* How many clocks of 'si_clock()' are equivalent to a milisecond (1ms = 1e+6 ns). */
#define SI_CLOCKS_MS (1000000)
/* How many clocks of 'si_clock()' are equivalent to a microsecond (1µs = 1e+3 ns). */
#define SI_CLOCKS_US (1000)

/* time - UINT
 * Converts Windows time into sili's Unix time. */
#define si_timeToUnix(time) (((time) - 116444736000000000) * 100)


/* Reads the current value of the processor’s time-stamp counter.
 * NOTE: Only natively works for AMD64, i386, ARM64 and PPC CPUs. On other CPUs
 * the function relies on OS functions like 'gettimeofday'. */
SIDEF u64 si_RDTSC(void);
/* Returns the current clock. */
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
SIDEF void si_sleep(u32 miliseconds);

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

/* Creates a string in the specified buffer from the format and returns the length
 * of it.
 * Specifiers:
 * - 'yyyy'/'yy' - full year/the two last digits
 * - 'M'/'MM' - month digit without/with padding, 'MMM'/'MMMM' - shortened/full month name,
 * - 'd'/'dd' - day digit without/with padding, 'DDD'/'DDDD' - shortened/full day name,
 * - 'h'/'hh' - hours without/with padding, 'm'/'mm' - minutes without/with padding,
 * - 's'/'ss' - seconds without/with padding, 'n'/'nn' - nanoseconds without/with padding.
 * - 'AP/ap' - uppercased/lowercased am/pm display. */
SIDEF usize si_timeToString(u8* buffer, usize capacity, siTimeCalendar calendar, siString fmt);

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
u32 si_numCountBitsU8(u8 num);
/* Returns how many 1 bits are in a 32-bit number. */
u32 si_numCountBitsU32(u32 num);
/* Returns how many 1 bits are in a 64-bit number. */
u32 si_numCountBitsU64(u64 num);

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
#define si_numLeadingZeros(type, num) si_numLeadingZerosEx(num, sizeof(type) * 8)
/* type - TYPE | num - UINT
 * Returns the amount of leading ones in a number. */
#define si_numLeadingOnes(type, num) si_numLeadingOnesEx(num, sizeof(type) * 8)

/* type - TYPE | num - UINT | bitType - siBitType
 * Returns the amount of trailing bits of the specified bit type (0 or 1). */
#define si_numTrailingBit(type, num, bitType) (type)si_numTrailingBitEx(num, sizeof(type) * 8, bitType)
/* type - TYPE | num - UINT | bits - usize
 * Rotates the bits of the number left by 'bits' amount. */
#define si_numRotateLeft(type, num, bits) (type)si_numRotateLeftEx(num, sizeof(type) * 8, bits)
/* type - TYPE | num - UINT | bits - usize
 * Rotates the bits of the number right by 'bits' amount. */
#define si_numRotateRight(type, num, bits) (type)si_numRotateRightEx(num, sizeof(type) * 8, bits)
/* type - TYPE | num - UINT
 * Reverses the bits of the number. */
#define si_numReverseBits(type, num) (type)si_numReverseBitsEx(num, sizeof(type) * 8 )
/* allocator - siAllocator* | type - TYPE | num - UINT
 * Creates a 'siArray(u8)' from the specified number and writes it into the allocator. */
#define si_numToBytes(type, num, allocator) si_numToBytesEx(num, sizeof(type), allocator)
/* TODO */
SIDEF u64 si_numFromBytes(siArray(u8) bytes);


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
SIDEF u32 si_numLen(u64 num);
/* Returns the length of a specified base unsigned number. */
SIDEF u32 si_numLenEx(u64 num, u32 base);
/* Returns the length of a base 10 signed number. */
SIDEF u32 si_numLenI64(i64 num);
/* Returns the length of a specified base signed number. */
SIDEF u32 si_numLenI64Ex(i64 num, u32 base);


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

siArray(u8) si_numToBytesEx(u64 num, usize num_sizeof, siAllocator alloc);


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
SIDEF u32 si_cpuClockSpeed(void);
/* Returns the amount of CPU processors there are. */
SIDEF u32 si_cpuProcessorCount(void);


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


/* Writes a NULL-terminated formatted C-string to a buffer. Returns the amount
 * of written bytes. */
SIDEF isize si_bprintf(char* out, usize capacity, cstring fmt, ...);
SIDEF isize si_bprintfVa(char* out, usize capacity, cstring fmt, va_list va);
/* Writes a NULL-terminated formatted C-string to a buffer. Returns the amount
 * of written bytes. */
SIDEF isize si_bprintfLn(char* out, usize capacity, cstring fmt, ...);
SIDEF isize si_bprintfLnVa(char* out, usize capacity, cstring fmt, va_list va);


/* Writes a NULL-terminated formatted C-string to a buffer. Returns the amount
 * of written bytes. */
SIDEF siString si_aprintf(siAllocator alloc, cstring fmt, ...);
SIDEF siString si_aprintfVa(siAllocator alloc, cstring fmt, va_list va);




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
typedef rawptr siDllHandle;
/* A type denotating that the value is a DLL process/function. */
typedef rawptr siDllProc;

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
u64 si_pow10(u32 exponent);
/* Raises a f64 10 by the power of the exponent. The exponent must be in-between
 * -18 to 18, otherwise the app will crash.*/
f64 si_pow10F64(i32 exponent);

/* Checks if the given 64-bit float is NaN. */
SIDEF b32 si_float64IsNan(f64 num);
/* Checks if the given 64-bit float is infinite. */
SIDEF u32 si_float64IsInf(f64 num);

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
		for_range (i, 0, timesToLoop) { \
			function; \
		} \
		timeSince = si_RDTSC() - timeSince; \
		f64 timeTaken = (f64)timeSince / ((f64)si_cpuClockSpeed() / 1000.0); /* NOTE(EimaMei): This will take the time in ms, not seconds. */ \
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
		si_threadMakeAndRun(si__BenchmarkThread, &miliseconds, &thread); \
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
		siAllocatorArena aData = si_arenaMakePtr(si_stackAlloc(SI_KILO(4)), SI_DEFAULT_MEMORY_ALIGNMENT); \
		siBenchmarkInfo arr = { \
			#function, \
			si_arrayMakeReserve(sizeof(u64), 0, 20, si_allocatorArena(&aData)) \
		}; \
		\
		si_benchmarkLoop(function, arr.cycles, start, end); \
		si_benchmarkLoopsAvgPrint(arr, (usize[]){start, end}); \
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
		siAllocatorArena si_aData = si_arenaMakePtr(si_stackAlloc(SI_KILO(4)), SI_DEFAULT_MEMORY_ALIGNMENT); \
		siAllocator si_alloc = si_allocatorArena(&si_aData); \
		siBenchmarkInfo si_arr[2] = { \
			{#function1, si_arrayMakeReserve(sizeof(u64), 0, 20, si_alloc)}, \
			{#function2, si_arrayMakeReserve(sizeof(u64), 0, 20, si_alloc)}  \
		};\
		si_benchmarkLoop(function1, si_arr[0].cycles, start, end); \
		si_benchmarkLoop(function2, si_arr[1].cycles, start, end); \
		\
		si_benchmarkLoopsAvgCmpPrint(si_arr, (usize[]){start, end}); \
	} while(0)

typedef struct { u64 duration; cstring unit; } siBenchmarkLimit;

#define SI_PERFORMANCE_MSG \
	"====== BENCHMARK DATA ======\n" \
	"General:\n" \
		"\tFunction - '%s'\n" \
		"\tDuration - '%f' ms\n" \
		"\tExecution count - '%f' million\n"

#define si_benchmarkLoop(function, cycles, start, end) \
	do { \
		usize arrayIndex = 0; \
		u64 medianIndex; \
		for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10) { \
			u64 index; \
			u64 begin = si_RDTSC(); \
			for (index = 0; index < medianIndex; index++) { \
				(void)function; \
			} \
			begin = si_RDTSC() - begin; \
			si_arrayGet(cycles, arrayIndex, u64) = begin; \
			arrayIndex += 1; \
		} \
		cycles.len = arrayIndex; \
	} while (0)

extern const siBenchmarkLimit siBenchLimit[];

const siBenchmarkLimit* si_benchmarkLimitLoop(u64 time);

typedef struct siBenchmarkInfo {
	cstring name;
	siArray(u64) cycles;
} siBenchmarkInfo;


SIDEF void si_benchmarkLoopsAvgPrint(siBenchmarkInfo info, usize range[2]);
SIDEF void si_benchmarkLoopsAvgCmpPrint(siBenchmarkInfo info[2], usize range[2]);
SIDEF rawptr si__BenchmarkThread(rawptr arg);

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
	#ifndef SI_NO_GENERAL
		#define SI_IMPLEMENTATION_GENERAL
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


#ifdef SI_IMPLEMENTATION_GENERAL

inline
rawptr si_alloc(siAllocator alloc, usize bytes) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_Alloc;
	func.data.alloc = bytes;

	return alloc.proc(func, alloc.data);
}

inline
void si_free(siAllocator alloc, rawptr ptr) {
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
rawptr si_realloc(siAllocator alloc, rawptr ptr, usize sizeOld, usize sizeNew) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_Resize;
	func.data.resize = (siAllocatorDataResize){ptr, sizeOld, sizeNew};

	return alloc.proc(func, alloc.data);
}


inline
usize si_allocatorAvailable(siAllocator alloc) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_GetAvailable;

	return (usize)alloc.proc(func, alloc.data);
}



inline
siAllocator si_allocatorHeap(void) {
	siAllocator alloc;
	alloc.proc = si_allocator_heap_proc;
	alloc.data = nil;

	return alloc;
}

SIDEF
rawptr si_allocator_heap_proc(siAllocatorFunc func, rawptr data) {
	rawptr out;
	switch (func.type) {
		case siAllocatorFunc_Alloc: {
			out = malloc(func.data.alloc);
			SI_ASSERT_NOT_NULL(out);
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
			SI_ASSERT_NOT_NULL(out);
		} break;

		case siAllocatorFunc_GetAvailable: {
			out = (rawptr)USIZE_MAX;
		} break;

		default: SI_PANIC();
	}

	SI_UNUSED(data);
	return out;
}

inline
siAllocatorArena si_arenaMake(siAllocator alloc, usize capacity) {
	return si_arenaMakeEx(alloc, capacity, SI_DEFAULT_MEMORY_ALIGNMENT);
}
SIDEF
siAllocatorArena si_arenaMakeEx(siAllocator alloc, usize capacity, u32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));

	siAllocatorArena out;
	out.alloc = alloc;
	out.offset = 0;
	out.alignment = alignment;
	out.capacity = si_alignForward(capacity, alignment);
	out.ptr = si_alloc(out.alloc, out.capacity);

	return out;
}
inline
siAllocatorArena si_arenaMakePtr(rawptr ptr, usize capacity, u32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));

	siAllocatorArena out;
	out.alloc.proc = nil;
	out.alloc.data = nil;
	out.offset = 0;
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
rawptr si_allocator_arena_proc(siAllocatorFunc func, rawptr data) {
	siAllocatorArena* alloc = (siAllocatorArena*)data;
	SI_ASSERT_MSG(alloc->ptr != nil, "You cannot use an already freed arena.");

	rawptr out;
	switch (func.type) {
		case siAllocatorFunc_Alloc: {
			usize bytes = si_alignForward(func.data.alloc, alloc->alignment);
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
			si_memcopy(out, resize.src, resize.sizeOld);
		} break;

		case siAllocatorFunc_GetAvailable: {
			out = (rawptr)(alloc->capacity - alloc->offset);
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
siAllocatorLifo si_lifoMake(siAllocator alloc, usize capacity) {
	return si_lifoMakeEx(alloc, capacity, SI_DEFAULT_MEMORY_ALIGNMENT);
}
SIDEF
siAllocatorLifo si_lifoMakeEx(siAllocator alloc, usize capacity, u32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));

	siAllocatorLifo lifo;
	lifo.alloc = alloc;
	lifo.offset = 0;
	lifo.alignment = alignment;
	lifo.capacity = si_alignForward(capacity, alignment);
	lifo.ptr = si_alloc(lifo.alloc, lifo.capacity);

	return lifo;
}
inline
siAllocatorLifo si_lifoMakePtr(rawptr ptr, usize capacity, u32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));

	siAllocatorLifo lifo;
	lifo.offset = 0;
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
rawptr si_allocator_lifo_proc(siAllocatorFunc func, rawptr data) {
    siAllocatorLifo* lifo = (siAllocatorLifo*)data;
	SI_ASSERT_MSG(lifo->ptr != nil, "You cannot use an already freed LIFO allocator.");

	rawptr out;
	switch (func.type) {
		case siAllocatorFunc_Alloc: {
			rawptr ptr = &lifo->ptr[lifo->offset];
			*(usize*)ptr = lifo->offset;

			usize bytes = si_alignForward(sizeof(usize) + func.data.alloc, lifo->alignment);
			lifo->offset += bytes;

			if (lifo->offset > lifo->capacity) {
				SI_PANIC_FMT(
					"Exceeded the available memory for an allocation (Tried writing '%zd' bytes "
					"into an already filled allocator with '%zd' bytes. Current offset: '%zd').\n",
					bytes, lifo->capacity, lifo->offset
				);
			}

			out = si_pointerAdd(ptr, sizeof(usize));
		} break;

		case siAllocatorFunc_Free: {
			SI_ASSERT_MSG(
				si_pointerBetween(func.data.free, lifo->ptr, &lifo->ptr[lifo->offset])
				&& lifo->offset != 0, "Invalid pointer."
			);

			lifo->offset = *(usize*)si_pointerSub(func.data.free, sizeof(usize));
			out = nil;
		} break;

		case siAllocatorFunc_FreeAll: {
			lifo->offset = 0;
			out = nil;
		} break;

		case siAllocatorFunc_Resize: {
			SI_PANIC_MSG("Unsupported function.");
			out = nil;
		} break;

		case siAllocatorFunc_GetAvailable: {
			out = (rawptr)(lifo->capacity - lifo->offset);
		} break;

		default: SI_PANIC();
	}

	return out;
}


inline
siAllocatorPool si_poolMake(siAllocator alloc, usize numChunks, usize chunkSize) {
	return si_poolMakeEx(alloc, numChunks, chunkSize, SI_DEFAULT_MEMORY_ALIGNMENT);
}
SIDEF
siAllocatorPool si_poolMakeEx(siAllocator alloc, usize numChunks, usize chunkSize,
		u32 alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	SI_ASSERT(numChunks != 0);
	SI_ASSERT(chunkSize != 0);

	siAllocatorPool pool;
	pool.alloc = alloc;
	pool.alignment = alignment;
	pool.numChunks = numChunks;
	pool.chunkSize = chunkSize;
	pool.actualChunkSize = si_alignForward(sizeof(siAllocatorPoolFreeNode) + pool.chunkSize, pool.alignment);
	pool.ptr = si_alloc(pool.alloc, pool.numChunks * pool.actualChunkSize);
	pool.head = nil;

	for_range (i, 0, pool.numChunks) {
		rawptr ptr = &pool.ptr[i * pool.actualChunkSize];
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
rawptr si_allocator_pool_proc(siAllocatorFunc func, rawptr data) {
	siAllocatorPool* pool = (siAllocatorPool*)data;
	SI_ASSERT_MSG(pool->ptr != nil, "You cannot use an already freed pool.");

	rawptr out;
	switch (func.type) {
		case siAllocatorFunc_Alloc: {
			SI_ASSERT_FMT(
				func.data.alloc <= pool->chunkSize,
				"Allocated object must fit the chunk size (requested allocation: '%zd', chunk size: '%zd')",
				func.data.alloc, pool->chunkSize
			);

			out = pool->head;
			if (out == nil) {
				SI_PANIC_MSG("Pool allocator has no free memory.");
			}

			pool->head = pool->head->next;
		} break;

		case siAllocatorFunc_Free: {
			siAllocatorPoolFreeNode* node = func.data.free;
			if (!si_pointerBetween(node, pool->ptr, &pool->ptr[pool->numChunks * pool->actualChunkSize])) {
				SI_PANIC_MSG("Invalid pointer.");
			}

			node->next = pool->head;
			pool->head = node;
			out = nil;
		} break;

		case siAllocatorFunc_FreeAll: {
			for_range (i, 0, pool->numChunks) {
				rawptr ptr = &pool->ptr[i * pool->actualChunkSize];
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
			out = (pool->head != nil) ? (rawptr)pool->chunkSize : 0;
		} break;

		default: SI_PANIC();
	}

	return out;
}

noreturn SIDEF
void si_panic(cstring conditionStr, cstring file, i32 line, cstring func,
		cstring message, ...) {
	siPrintColor red = si_printColor3bitEx(siPrintColor3bit_Red, true, false);

	si_fprintf(
		si_stderr,
		"%CAssertion \"%s\" at \"%s:%d\"%C: %s%s",
		red, conditionStr, file, line, func, (message != nil) ? ": " : "\n"
	);


	if (message != nil) {
		va_list va;
		va_start(va, message);
		si_fprintfVa(si_stderr, message, va);
		si_fprint(si_stderr, "\n");
		va_end(va);
	}

	SI_DEBUG_TRAP();
}

#ifndef SI_NO_OPTIONAL
SIDEF
void si_errorLogDefault(siError* error) {
	siPrintColor red = si_printColor3bitEx(siPrintColor3bit_Red, true, false);
	si_fprintf(
		si_stderr, "%CError at \"%s:%i\"%C: Number '%i'.\n",
		red, error->filename, error->line, error->code
	);
}
#endif /* SI_NO_OPTIONAL */

#endif /* SI_IMPLEMENTATION_GENERAL */

#ifdef SI_IMPLEMENTATION_MEMORY

inline
b32 si_isPowerOfTwo(usize x) {
	return !(x & (x - 1)) && x != 0;
}

inline
usize si_alignForward(usize num, usize alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	return (num + alignment - 1) & ~(alignment - 1);
}

inline void* si_pointerAdd(void* ptr, isize bytes)                  { return si_cast(rawptr, (u8*)ptr + bytes); }
inline void* si_pointerSub(void* ptr, isize bytes)                  { return si_cast(rawptr, (u8*)ptr - bytes); }
inline const void* si_pointerAddConst(const void* ptr, isize bytes) { return si_cast(rawptr, (const u8*)ptr + bytes); }
inline const void* si_pointerSubConst(const void* ptr, isize bytes) { return si_cast(rawptr, (const u8*)ptr - bytes); }
inline isize si_pointerDiff(const void* begin, const void* end)     { return si_cast(isize,  (const u8*)end - (const u8*)begin); }
inline b32 si_pointerBetween(const void* ptr, const void* start, const void* end) { return (ptr >= start) && (ptr <= end); }


SIDEF
rawptr si_memcopy(void* restrict dst, const void* restrict src, usize size) {
	SI_ASSERT_NOT_NULL(dst);
	SI_ASSERT_NOT_NULL(src);

#ifndef SI_NO_CRT
	return memcpy(dst, src, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif
}

SIDEF
rawptr si_memmove(void* restrict dst, const void* restrict src, usize size) {
	SI_ASSERT_NOT_NULL(dst);
	SI_ASSERT_NOT_NULL(src);

#ifndef SI_NO_CRT
	return memmove(dst, src, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif

}

SIDEF
rawptr si_memset(void* data, u8 value, usize size) {
	SI_ASSERT_NOT_NULL(data);

#ifndef SI_NO_CRT
	return memset(data, value, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif

}

SIDEF
i32 si_memcompare(const void* ptr1, const void* ptr2, usize size) {
	SI_ASSERT_NOT_NULL(ptr1);
	SI_ASSERT_NOT_NULL(ptr2);

#ifndef SI_NO_CRT
	return memcmp(ptr1, ptr2, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif
}

SIDEF
rawptr si_memchr(const void* data, u8 value, usize size) {
	SI_ASSERT_NOT_NULL(data);

#ifndef SI_NO_CRT
	return (rawptr)memchr(data, value, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif
}


SIDEF
usize si_cstrLen(cstring string) {
	SI_ASSERT_NOT_NULL(string);

#ifndef SI_NO_CRT
	return strlen(string);
#else
	usize i = 0;
	while (string[i] != '\0') { i += 1; }

	return i;
#endif

}

inline
usize si_memcopy_s(void* restrict dst, usize sizeDst, const void* restrict src,
		usize sizeSrc) {
	usize length = si_min(usize, sizeDst, sizeSrc);
	si_memcopy(dst, src, length);

	return length;
}

inline
usize si_memmove_s(void* restrict dst, usize sizeDst, const void* restrict src,
		usize sizeSrc) {
	usize length = si_min(usize, sizeDst, sizeSrc);
	si_memmove(dst, src, length);

	return length;
}

inline
rawptr si_memmoveLeft(void* src, usize length, isize moveBy) {
	return si_memmove((u8*)src - moveBy, src, length);
}
inline
rawptr si_memmoveRight(void* src, usize length, isize moveBy) {
	return si_memmove((u8*)src + moveBy, src, length);
}

#endif /* SI_IMPLEMENTATION_MEMORY */


#ifdef SI_IMPLEMENTATION_ARRAY

#ifndef SI_RELEASE_MODE
inline
rawptr si__arrayGet(const siBuffer* buffer, usize index, usize sizeof_type,
		cstring file, i32 line) {
	SI_ASSERT_FMT(
		sizeof_type == buffer->typeSize,
		"Invalid type size ('%zd') at '%s:%i' (typeSize = '%zu', ptr = '%p').",
		sizeof_type, file, line, buffer->typeSize, buffer->data
	);
	SI_ASSERT_FMT(
		index < buffer->capacity,
		"Invalid index '%zd' at '%s:%i' (len = '%zu', cap = '%zu', typeSize = '%zu', ptr = '%p').",
		index, file, line, buffer->len, buffer->capacity, buffer->typeSize, buffer->data
	);
	return (u8*)buffer->data + sizeof_type * index;
}
#endif



inline
siBuffer si_arrayMakeEx(rawptr list, usize sizeofItem, usize count, siAllocator alloc)  {
	siBuffer array = si_arrayMakeReserve(sizeofItem, count, count, alloc);
	si_memcopy(array.data, list, sizeofItem * count);

	return array;
}

inline
siBuffer si_arrayMakeReserve(usize sizeofItem, usize length, usize capacity,
		siAllocator alloc) {
	siBuffer array;
	array.alloc = alloc;
	array.typeSize = sizeofItem;
	array.len = length;
	array.capacity = capacity;
	array.data = si_alloc(alloc, sizeofItem * capacity);
#ifdef SI_USE_BUFFER_GROW
	array.grow = SI_USE_BUFFER_GROW;
#endif

	return array;
}

inline
siBuffer si_arrayCopy(siBuffer array, siAllocator alloc) {
	return si_arrayMakeEx(array.data, array.typeSize, array.len, alloc);
}


inline
void si_arrayFree(siBuffer array) {
	si_free(array.alloc, array.data);
}


inline
b32 si_arrayAt(siBuffer array, usize index, rawptr outPtr) {
	SI_STOPIF(index >= array.len, return false);
	si_memcopy(outPtr, si_arrayGetPtr(array, index), array.typeSize);

	return true;
}

inline
b32 si_arrayAtFront(siBuffer array, rawptr outPtr) {
	SI_STOPIF(array.len == 0, return false);
	si_memcopy(outPtr, array.data, array.typeSize);

	return true;
}

inline
b32 si_arrayAtBack(siBuffer array, rawptr outPtr) {
	SI_STOPIF(array.len == 0, return false);
	si_memcopy(outPtr, si_arrayGetPtr(array, array.len - 1), array.typeSize);

	return true;

}

inline
rawptr si_arrayBegin(siBuffer array) {
	return array.data;
}

inline
rawptr si_arrayEnd(siBuffer array) {
	return si_arrayGetPtr(array, array.len);
}


SIDEF
isize si_arrayFind(siBuffer array, rawptr valuePtr) {
    for_range (i, 0, array.len) {
		rawptr dst = si_arrayGetPtr(array, i);
		if (si_memcompare(dst, valuePtr, array.typeSize) == 0) {
			return i;
		}
	}

	return -1;
}

SIDEF
isize si_arrayFindLast(siBuffer array, rawptr valuePtr) {
	isize i;
	for (i = array.len - 1; i != -1; i -= 1) {
		rawptr dst = si_arrayGetPtr(array, i);
		if (si_memcompare(dst, valuePtr, array.typeSize) == 0) {
			return i;
		}
	}

	return -1;
}

SIDEF
usize si_arrayFindCount(siBuffer array, rawptr valuePtr) {
	usize occurences = 0;
	for_range (i, 0, array.len) {
		rawptr element = si_arrayGetPtr(array, i);

		if (si_memcompare(element, valuePtr, array.typeSize) == 0) {
			occurences += 1;
		}
	}

	return occurences;

}


SIDEF
b32 si_arrayEqual(siBuffer lhs, siBuffer rhs) {
	SI_STOPIF(lhs.len != rhs.len, return false);
	SI_STOPIF(lhs.data == rhs.data, return true);

	return si_memcompare(lhs.data, rhs.data, si_min(usize, lhs.len, rhs.len)) == 0;
}


inline
b32 si_arrayAppend(siBuffer* array, rawptr valuePtr) {
	return si_arrayAppendEx(array, valuePtr, 1);
}

SIDEF
b32 si_arrayAppendEx(siBuffer* array, rawptr valueArray, usize count) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_NOT_NULL(valueArray);

	usize offsetOld = array->len * array->typeSize;
	b32 allocated = si_arrayMakeSpaceFor(array, count);

	si_memcopy((u8*)array->data + offsetOld, valueArray, count * array->typeSize);
	return allocated;
}

inline
void si_arrayPop(siBuffer* array) {
	SI_ASSERT_NOT_NULL(array);
	SI_STOPIF(array->len == 0, return);
	array->len -= 1;
}

inline
void si_arrayClear(siBuffer* array) {
	SI_ASSERT_NOT_NULL(array);
	array->len = 0;
}


inline
b32 si_arrayInsert(siBuffer* array, usize index, rawptr value) {
	return si_arrayInsertEx(array, index, value, 1);
}

SIDEF
b32 si_arrayInsertEx(siBuffer* array, usize index, rawptr valueArray, usize count) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_NOT_NULL(valueArray);
	SI_ASSERT(index < array->len);
	SI_STOPIF(count == 0, return false);

	usize remainderLen = array->len - index - count;
	b32 allocated = si_arrayMakeSpaceFor(array, count);

	u8* dst = si_arrayGetPtr(*array, index);
	si_memcopy(si_arrayGetPtr(*array, index + count), dst, remainderLen * array->typeSize);
	si_memcopy(dst, valueArray, count * array->typeSize);

	return allocated;
}

inline
void si_arrayErase(siBuffer* array, usize index) {
	si_arrayEraseEx(array, index, 1);
}

SIDEF
void si_arrayEraseEx(siBuffer* array, usize index, usize count) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT(index + count <= array->len);
	SI_STOPIF(array == 0, return);

	usize length = array->len - index - count;
	u8* dst = si_arrayGetPtr(*array, index);
	u8* src = si_arrayGetPtr(*array, index + count);
	si_memcopy(dst, src, length * array->typeSize);

	array->len -= count;
}


inline
void si_arrayReplaceAll(siBuffer array, rawptr restrict valueOld, rawptr restrict valueNew) {
	si_arrayReplace(array, valueOld, valueNew, -1);
}

SIDEF
void si_arrayReplace(siBuffer array, rawptr restrict valueOld, rawptr restrict valueNew,
		isize amount) {
	SI_ASSERT_NOT_NULL(valueOld);
	SI_ASSERT_NOT_NULL(valueNew);
	SI_STOPIF(amount == 0, return);


	isize count = 0;
	for_range (i, 0, array.len) {
		rawptr element = si_arrayGetPtr(array, i);
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
void si_arrayReverse(siBuffer array) {
	SI_ASSERT(array.typeSize <= 128);

	u8* a = (u8*)array.data;
	u8* b = si_arrayGetPtr(array, array.len - 1);
	u8 tmp[128];

	usize len = array.len / 2;
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
b32 si_arrayFill(siBuffer* array, usize index, usize count, rawptr valuePtr) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_NOT_NULL(valuePtr);

	b32 allocated = false;
	isize addLen = (index + count) - array->len;
	if (addLen > 0) {
		allocated = si_arrayMakeSpaceFor(array, addLen);
	}

	u8* dst = si_arrayGetPtr(*array, index);
	for_range (i, 0, count) {
		si_memcopy(dst, valuePtr, array->typeSize);
		dst += array->typeSize;
	}

	return allocated;
}



SIDEF
b32 si_arrayMakeSpaceFor(siBuffer* buffer, usize addLen) {
	SI_ASSERT_NOT_NULL(buffer);
	SI_ASSERT_NOT_NULL(buffer->alloc.proc);

	usize newLength = buffer->len + addLen;
	if (newLength <= buffer->capacity) {
		buffer->len = newLength;
		return false;
	}

	usize newCapacity = SI_BUFFER_NEW_CAP(buffer, addLen);
	buffer->data = si_realloc(
		buffer->alloc, buffer->data,
		buffer->capacity * buffer->typeSize, newCapacity * buffer->typeSize
	);
	buffer->len = newLength;
	buffer->capacity = newCapacity;

	return true;
}

#endif /* SI_IMPLEMENTATION_ARRAY */

#ifdef SI_IMPLEMENTATION_STRING

inline
siString si_stringMake(cstring string, siAllocator alloc) {
	return si_stringMakeLen(string, si_cstrLen(string), alloc);
}

inline
siString si_stringMakeLen(cstring string, usize len, siAllocator alloc) {
	return si_stringMakeEx(string, len, len, alloc);
}

SIDEF
siString si_stringMakeEx(cstring string, usize len, usize capacity,
		siAllocator alloc) {
	SI_ASSERT_NOT_NULL(string);

	siString str = si_arrayMakeReserve(sizeof(u8), len, capacity, alloc);
	si_memcopy(str.data, string, len);

	return str;
}

inline
siString si_stringMakeReserve(usize length, usize capacity, siAllocator alloc) {
	return si_arrayMakeReserve(sizeof(u8), length, capacity, alloc);
}

inline
siString si_stringCopy(siString from, siAllocator alloc) {
	return si_stringMakeLen((char*)from.data, from.len, alloc);
}

inline
char* si_stringCopyToCstr(siString from, siAllocator alloc) {
	return si_stringCloneToCstrEx(from, from.len, alloc);
}
SIDEF
char* si_stringCloneToCstrEx(siString from, usize capacity, siAllocator alloc) {
	char* str = si_allocArray(alloc, char, capacity + 1);
	si_memcopy(str, from.data, from.len);
	str[from.len] = '\0';

	return str;
}



inline
void si_stringFree(siString string) {
	si_arrayFree(string);
}



inline
i32 si_stringAt(siString string, usize index) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_STOPIF(string.len <= index || string.len == 0, return -1);

	return si_stringGet(string, index);
}

inline
i32 si_stringAtFront(siString string) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_STOPIF(string.len == 0, return -1);

	return si_stringGet(string, 0);
}

SIDEF
i32 si_stringAtBack(siString string) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_STOPIF(string.len == 0, return -1);

	return si_stringGet(string, string.len - 1);
}

inline
u8* si_stringBegin(siString string) {
	return (u8*)string.data;
}

inline
u8* si_stringEnd(siString string) {
	return (u8*)string.data + string.len;
}



SIDEF
isize si_stringFind(siString string, siString subStr) {
	usize counter = 0;
	for_eachStr (letter, string) {
		if (*letter != si_stringGet(subStr, counter)) {
			counter = 0;
			continue;
		}

		counter += 1;
		if (counter == subStr.len) {
			isize index = si_pointerDiff(string.data, letter);
			return index - (subStr.len - 1);
		}
	}

	return -1;
}

inline
isize si_stringFindByte(siString string, u8 byte) {
	for_eachStr (letter, string) {
		if (*letter == byte) {
			return si_pointerDiff(string.data, letter);
		}
	}

	return -1;
}

SIDEF
isize si_stringFindUtf8(siString string, const u8* character, isize* outCharIndex) {
	SI_ASSERT_NOT_NULL(character);
	SI_ASSERT_NOT_NULL(outCharIndex);

	usize i = 0;
	isize utf8I = 0;
	i32 codepoint = si_utf8Decode(character).codepoint;

	while (i < string.len) {
		siUtf32Char x = si_utf8Decode(&si_stringGet(string, i));
		if (x.codepoint == codepoint) {
			*outCharIndex = utf8I;
			return i;
		}

		utf8I += 1;
		i += x.len;
	}

	*outCharIndex = -1;
	return -1;

}


SIDEF
isize si_stringFindLast(siString string, siString subStr) {
	SI_ASSERT_NOT_NULL(string.data);

	usize subStrEnd = subStr.len - 1;
	usize counter = subStrEnd;

	for_eachRevStr (letter, string) {
		if (*letter != si_stringGet(subStr, counter)) {
			counter = subStrEnd;
			continue;
		}

		counter -= 1;
		if (counter == 0) {
			return letter - (u8*)string.data;
		}
	}

	return -1;
}

inline
isize si_stringFindLastByte(siString string, u8 byte) {
	for_eachRevStr (letter, string) {
		if (*letter == byte) {
			return si_pointerDiff(string.data, letter);
		}
	}

	return -1;
}

SIDEF
usize si_stringFindCount(siString string, siString subStr) {
	usize occurences = 0;
	usize counter = 0;
	for_eachStr (letter, string) {
		if (*letter != si_stringGet(subStr, counter)) {
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
	return si_memcompare(lhs.data, rhs.data, si_min(usize, lhs.len, rhs.len));
}

inline
b32 si_stringEqual(siString lhs, siString rhs) {
	SI_STOPIF(lhs.len != rhs.len, return false);
	SI_STOPIF(lhs.data == rhs.data, return true);

	return si_stringCompare(lhs, rhs) == 0;
}



SIDEF
b32 si_stringAppend(siString* string, siString other) {
	SI_ASSERT_NOT_NULL(string);

	usize oldLen = string->len;
	b32 allocated = si_stringMakeSpaceFor(string, other.len);

	si_memcopyStr(&si_stringGet(*string, oldLen), other);
	return allocated;
}

inline
b32 si_stringSet(siString* string, siString str) {
	b32 allocated = si_stringMakeSpaceFor(string, str.len - string->len);
	si_memcopy(string->data, str.data, str.len);

	return allocated;
}

SIDEF
siString si_stringJoin(siArray(siString) strings, siString separator,
		siAllocator alloc) {
	SI_ASSERT_NOT_NULL(strings.data);
	SI_STOPIF(strings.len == 0, return SI_STR_EMPTY);

	usize length = separator.len * (strings.len - 1);
	for_eachArr (siString, string, strings) {
		length += string->len;
	}

	siString res = si_stringMakeReserve(length, length, alloc);

	siString* data = (siString*)strings.data;
	usize i = data[0].len;
	si_memcopy(res.data, data[0].data, data[0].len);

	for_range (j, 1, strings.len) {
		siString str = data[j];

		si_memcopyStr(&si_stringGet(res, i), separator),
			i += separator.len;
		si_memcopyStr(&si_stringGet(res, i), str),
			i += str.len;
	}

	return res;
}


inline
void si_stringTrim(siString* string, siString cutSet) {
	si_stringTrimLeft(string, cutSet);
	si_stringTrimRight(string, cutSet);
}

SIDEF
void si_stringTrimLeft(siString* string, siString cutSet) {
	SI_ASSERT_NOT_NULL(string);

	u8* posStart = si_stringBegin(*string);
	u8* posEnd = si_stringEnd(*string);
	while (posStart < posEnd && si_stringFindByte(cutSet, *posStart) != -1) {
		posStart += 1;
	}


	if (string->data != posStart) {
		string->len = si_pointerDiff(posStart, posEnd);

		if (string->alloc.proc == nil) {
			string->data = posStart;
		}
		else {
			si_memmove(string->data, posStart, string->len);
		}
	}
}

SIDEF
void si_stringTrimRight(siString* string, siString cutSet) {
	SI_ASSERT_NOT_NULL(string);

	u8* posStart = si_stringBegin(*string);
	u8* posEnd = si_stringEnd(*string) - 1;
	while (posEnd > posStart && si_stringFindByte(cutSet, *posEnd) != -1) {
		posEnd -= 1;
	}

	string->len = si_pointerDiff(posStart, posEnd + 1);
}

inline
void si_stringStrip(siString* string) {
	si_stringTrim(string, SI_STR(" \t\r\n\v\f"));
}


SIDEF
b32 si_stringEnquote(siString* string) {
	SI_ASSERT_NOT_NULL(string);
	SI_ASSERT_NOT_NULL(string->data);

	usize oldLen = string->len;
	b32 allocated = si_stringMakeSpaceFor(string, 2);

	u8* data = (u8*)string->data;
	si_memmoveRight(data, oldLen, 1);
	data[0] = '\"';
	data[string->len - 1] = '\"';

	return allocated;
}

inline
b32 si_stringPush(siString* string, char other) {
	return si_stringAppend(string, SI_STR_LEN(&other, 1));
}

inline
void si_stringPop(siString* string) {
	SI_ASSERT_NOT_NULL(string);
	SI_STOPIF(string->len == 0, return);
	string->len -= 1;
}

inline
void si_stringClear(siString* string) {
	SI_ASSERT_NOT_NULL(string);
	string->len = 0;
}


SIDEF
b32 si_stringInsert(siString* string, siString subStr, usize index) {
	SI_ASSERT_NOT_NULL(string);
	SI_ASSERT(index < string->len);
	SI_STOPIF(subStr.len == 0, return false);

	b32 allocated = si_stringMakeSpaceFor(string, subStr.len);
	siString remainder = si_stringSubToEnd(*string, index);

	si_memmoveStr(&si_stringGet(*string, index + subStr.len), remainder);
	si_memcopyStr(remainder.data, subStr);

	return allocated;
}

SIDEF
void si_stringErase(siString* string, usize index, usize count) {
	SI_ASSERT_NOT_NULL(string);
	SI_ASSERT(index + count <= string->len);
	SI_STOPIF(count == 0, return);

	siString remainder = si_stringSubToEnd(*string, index + count);
	si_memmoveStr(&si_stringGet(*string, index), remainder);

	string->len -= count;
}


SIDEF
void si_stringReverse(siString string) {
	SI_STOPIF(string.len == 0, return);

	u8* a = si_stringBegin(string);
	u8* b = si_stringEnd(string) - 1;

	usize len = string.len / 2;
	while (len) {
		si_swap(*a, *b);
		a += 1;
		b -= 1;

		len -= 1;
	}
}


inline
b32 si_stringReplaceAll(siString* string, siString strOld, siString strNew,
		siAllocator alloc) {
	return si_stringReplace(string, strOld, strNew, -1, alloc);
}

SIDEF
b32 si_stringReplace(siString* string, siString strOld, siString strNew, isize amount,
		siAllocator alloc) {
	SI_ASSERT_NOT_NULL(string);
	SI_ASSERT_NOT_NULL(strOld.data);
	SI_ASSERT_NOT_NULL(strNew.data);
	SI_STOPIF(strOld.data == strNew.data, return false);

	if (amount < 0) {
		amount = si_stringFindCount(*string, strOld);
	}
	SI_STOPIF(amount == 0, return false);

	usize len = string->len + amount * (isize)(strNew.len - strOld.len);
	usize lineStart = 0, i = 0;
	siString res = si_stringMakeReserve(len, len, alloc);

	while (amount) {
		siString subStr = si_stringSubToEnd(*string, lineStart);
		subStr.len = si_stringFind(subStr, strOld);

		si_memcopyStr(&si_stringGet(res, i), subStr),
			i += subStr.len;
		si_memcopyStr(&si_stringGet(res, i), strNew),
			i += strNew.len;

		lineStart += subStr.len + strOld.len;
		amount -= 1;
	}
	si_memcopyStr(&si_stringGet(res, i), si_stringSubToEnd(*string, lineStart));

	si_stringFree(*string);
	*string = res;
	return true;
}


inline
siArray(siString) si_stringSplit(siString string, siString delimiter,
		siAllocator alloc) {
	return si_stringSplitEx(string, delimiter, -1, alloc);
}

SIDEF
siArray(siString) si_stringSplitEx(siString string, siString delimiter, isize amount,
		siAllocator alloc) {
	if (amount < 0) {
		amount = si_stringFindCount(string, delimiter);
	}
	SI_STOPIF(amount == 0, return SI_BUF_LEN(u8, nil, 0));


	usize len = amount + 1;
	siArray(siString) res = si_arrayMakeReserve(sizeof(siString), len, len, alloc);

	isize lineStart = 0;
	for_range (i, 0, amount) {
		siString subStr = si_stringSubToEnd(string, lineStart);
		subStr.len = si_stringFind(subStr, delimiter);

		siString* element = &si_arrayGet(res, i, siString);
		*element = subStr;

		lineStart += subStr.len + delimiter.len;
	}

	if (string.len == (usize)lineStart) {
		res.len -= 1;
		return res;
	}
	siString* last = &si_arrayGet(res, amount, siString);
	*last = si_stringSubToEnd(string, lineStart);

	return res;
}

inline
b32 si_stringMakeSpaceFor(siString* string, usize addLen) {
	return si_arrayMakeSpaceFor(string, addLen);
}


#ifdef SI_IMPLEMENTATION_MEMORY

inline rawptr si_memcopyStr(void* dst, siString src) { return si_memcopy(dst, src.data, src.len); }
inline rawptr si_memmoveStr(void* dst, siString src) { return si_memmove(dst, src.data, src.len); }
inline i32 si_memcompareStr(const void* dst, siString src) { return si_memcompare(dst, src.data, src.len); }

inline usize si_memcopyStr_s(void* dst, usize dstSize, siString src) { return si_memcopy_s(dst, dstSize, src.data, src.len); }
inline usize si_memmoveStr_s(void* dst, usize dstSize, siString src) { return si_memmove_s(dst, dstSize, src.data, src.len); }

#endif


inline
void si_stringUpper(siString string) {
	for_eachStr (x, string) {
		*x = si_charUpper(*x);
	}
}

inline
void si_stringLower(siString string) {
	for_eachStr (x, string) {
		*x = si_charLower(*x);
	}
}
inline
void si_stringTitle(siString string) {
	u8* end = si_stringEnd(string);

	for_eachStr (x, string) {
		if (si_charIsSpace(*x)) {
			x += 1;
			while (x < end && si_charIsSpace(*x)) { x += 1; }

			if (x < end) {
				*x = si_charUpper(*x);
			}
		}
		else {
			*x = si_charLower(*x);
		}
	}
}

inline
void si_stringCapitalize(siString string) {
	si_stringLower(string);

	usize i = 0;
	while (si_charIsSpace(si_stringGet(string, i))) { i += 1; }

	u8* data = (u8*)string.data + i;
	*data = si_charUpper(*data);
}


const char SI_NUM_TO_CHAR_TABLE_UPPER[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"@$";
const char SI_NUM_TO_CHAR_TABLE_LOWER[] =
	"0123456789"
	"abcdefghijklmnopqrstuvwxyz"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"@$";
const char* SI_NUM_TO_CHAR_TABLE = SI_NUM_TO_CHAR_TABLE_UPPER;

inline
void si_numChangeTable(b32 upper) {
	static const char* choices[2] = {SI_NUM_TO_CHAR_TABLE_LOWER, SI_NUM_TO_CHAR_TABLE_UPPER};
	SI_NUM_TO_CHAR_TABLE = choices[upper & true];
}


inline
siString si_stringFromUInt(u64 num, siAllocator alloc) {
	return si_stringFromUIntEx(num, 10, alloc);
}
SIDEF
siString si_stringFromUIntEx(u64 num, i32 base, siAllocator alloc) {
	usize len = si_numLenEx(num, base);
	siString res = si_stringMakeReserve(len, len, alloc);

	/* NOTE(EimaMei): We build the string from the back (not the front) so that
	 * we wouldn't have to reverse the string after we make the string. */
	u8* back = si_stringEnd(res) - 1;

	do {
		*back = SI_NUM_TO_CHAR_TABLE[num % base];
		num /= base;
		back -= 1;
	} while (num != 0);

	return res;
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
	SI_ASSERT_NOT_NULL(outRes);
	SI_STOPIF(string.len == 0, *outRes = -1; return 0);

	u64 res = 0;
	usize front = 0;
	while (si_charIsSpace(si_stringGet(string, front))) { front += 1; }

	if (si_stringGet(string, front) != '0' || base == 10) {
		SI_STOPIF(string.len > 20, *outRes = -2; return 0);

		for_range (i, front, string.len) {
			char x = si_stringGet(string, i);
			if (!si_charIsDigit(x)) {
				usize oldI = i;
				while (i < string.len && si_charIsSpace(si_stringGet(string, i))) { i += 1; }

				*outRes = (i >= string.len) ? INT32_MAX : (i32)oldI;
				return res;
			}

			res *= 10;
			res += (x - '0');
		}


		*outRes = INT32_MAX;
		return res;
	}

	u32 maxDigits;

	if (base > -1) {
		front += 1;
		switch (si_stringGet(string, front)) {
			case 'X': base = 16; string = si_stringSubToEnd(string, 2); maxDigits = 16 + 2; front += 2; break;
			case 'O': base =  8; string = si_stringSubToEnd(string, 2); maxDigits = 22 + 2; front += 2; break;
			case 'B': base =  2; string = si_stringSubToEnd(string, 2); maxDigits = 64 + 2; front += 2; break;
			default:  base =  8; string = si_stringSubToEnd(string, 1); maxDigits = 22 + 1; front += 1; break;
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


	for_range (i, front, string.len) {
		res *= base;

		i32 value = si_stringGet(string, i) - '0';
		if (value < 10) {
			res += value;
		}
		else if (value != (' ' - '0')) {
			value = si_charUpper((char)value) - 7;
			if (value < base) {
				res += value;
			}
			else {
				*outRes = (i32)i;
				return res;
			}
		}
		else {
			usize oldI = i;
			while (i < string.len && si_charIsSpace(si_stringGet(string, i))) {
				i += 1;
			}

			*outRes = (i >= string.len) ? INT32_MAX : (i32)oldI;
			return res;
		}
	}

	*outRes = INT32_MAX;
	return res;
}

inline
siString si_stringFromInt(i64 num, siAllocator alloc) {
	return si_stringFromIntEx(num, 10, alloc);
}
SIDEF
siString si_stringFromIntEx(i64 num, i32 base, siAllocator alloc) {
	b32 isNegative = (num < 0);
	usize len = si_numLenI64Ex(num, base);
	siString res = si_stringMakeReserve(len, len, alloc);

	/* NOTE(EimaMei): We build the string from the back (not the front) so that
	 * we wouldn't have to reverse the string after we make the string. */
	u8* back = si_stringEnd(res) - 1;

	if (isNegative) {
		num = -num;
		si_stringGet(res, 0) = '-';
	}

	do {
		*back = SI_NUM_TO_CHAR_TABLE[num % base];
		num /= base;
		back -= 1;
	} while (num > 0);

	return res;
}
inline
siString si_stringFromFloat(f64 num, siAllocator alloc) {
	return si_stringFromFloatEx(num, 10, 6, alloc);
}
SIDEF
siString si_stringFromFloatEx(f64 num, i32 base, u32 afterPoint, siAllocator alloc) {
	{
		u32 isInf = si_float64IsInf(num);
		if (isInf) {
			return (isInf == 1) ? SI_STR("inf") : SI_STR("-inf");
		}
		else if (si_float64IsNan(num)) {
			return SI_STR("nan");
		}
	}

	b32 isNegative;
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


	usize len = isNegative + baseLen + (afterPoint != 0) + afterPoint;
	siString res = si_stringMakeReserve(len, len, alloc);
	u8* ptr = &si_stringGet(res, 0);

	SI_STOPIF(isNegative, *ptr = '-'; ptr += 1);

	u32 digit;
	do {
		numWhole *= base;
		digit = (u32)numWhole;
		numWhole -= digit;

		*ptr = SI_NUM_TO_CHAR_TABLE[digit % base];
		ptr += 1;

		baseLen -= 1;
	} while (baseLen > 0);

	if (afterPoint == 0) {
		return res;
	}

	*ptr = '.';
	ptr += 1;

	f64 numFractional = num - si_floor(f64, num);
	while (afterPoint) {
		numFractional *= base;

		u32 number = (u32)numFractional;
		*ptr = SI_NUM_TO_CHAR_TABLE[number % base];
		ptr += 1;

		afterPoint -= 1;
		numFractional -= (f64)number;
	}

	return res;
}


inline
siString si_stringFromBool(b32 boolean) {
	static const siString values[] = {SI_STRC("false"), SI_STRC("true")};
	return values[boolean & true];
}

SIDEF
b32 si_stringToBool(siString string) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_STOPIF(string.len == 0 || (string.len != 1 && string.len != 4 && string.len != 5), return UINT32_MAX);

	if (string.len == 1) {
		switch (si_charLower(*(u8*)string.data)) {
			case '1': case 't': return true;
			case '0': case 'f': return false;
			default: return UINT32_MAX;
		}
	}

	u8 str[5];
	for_range (i, 0, countof(str)) {
		str[i] = si_stringGet(string, i) | SI_BIT(5);
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
usize si_stringPrefixLen(siString string, siString prefix) {
	SI_STOPIF(prefix.len > string.len, return 0);

	usize count = 0;
	for_eachStr (x, string) {
		if (*x == si_stringGet(prefix, count)) {
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
		&& (si_memcompareStr((u8*)string.data + (string.len - suffix.len), suffix) == 0);
}
SIDEF
usize si_stringSuffixLen(siString string, siString suffix) {
	SI_STOPIF(suffix.len > string.len, return 0);

	usize count = string.len - 1;
	for_eachRevStr (x, string) {
		if (*x == si_stringGet(suffix, count)) {
			count -= 1;
			continue;
		}
		break;
	}

	return string.len - count + 1;
}


SIDEF
siString si_stringFromArray(siBuffer array, cstring fmt, u8* out, usize capacity) {
	SI_ASSERT_NOT_NULL(fmt);
	SI_STOPIF(capacity < 2, return SI_STR_EMPTY);

	u64 args[16];
	u32 argSizes[16];
	usize argCount = 0;

	usize length  = 0;

	{
		usize j = 0;
		while (fmt[j] != '\0') {
			SI_STOPIF(fmt[j] != '%', j += 1; continue);
			j += 1;

go_back:
			switch (fmt[j]) {
				case 'C': SI_PANIC();
				case '%': case 'n': break;

				case 'h':
					j += 1;
					argSizes[argCount] = (fmt[j] == 'h') ? 1 : 2, argCount += 1;
					break;
				case 'j': case 'l': case 'f': case 'F': case 'a': case 'A':
				case 'e': case 'E': case 'g': case 'G':
					argSizes[argCount] = 8; argCount += 1;
					break;
				case 't': case 's': case 'p':
					argSizes[argCount] = sizeof(usize); argCount += 1;
					break;
				case 'x': case 'X': case 'i': case 'O': case 'o': case 'b':
				case 'B': case 'u': case 'd':
					argSizes[argCount] = 4; argCount += 1;
					break;
				case 'c':
					argSizes[argCount] = 1; argCount += 1;
					break;
				case 'S':
					argSizes[argCount] = sizeof(siString); argCount += 1;
					break;
				case '*': case '.': case '-': case '+': case ' ': case '0':
				case '1': case '2': case '3': case '4': case '5': case '6':
				case '7': case '8': case '9': case '#':
					j += 1;
					goto go_back;
			}

			j += 1;
		}
		SI_ASSERT_MSG(argCount <= 16, "The format string can only contain a maximum of 16 modifiers.");
	}

	capacity -= 1;
	out[0] = '{', length += 1;
	for_range (i, 0, array.len) {
		u8* base = (u8*)si_arrayGetPtr(array, i);
		for_range (j, 0, argCount) {
			usize argSize = argSizes[j];

			si_memcopy(&args[j], base, argSize);
			base += argSize;
		}
		length += si_bprintf(
			(char*)&out[length], capacity - length, fmt,
			args[0], args[1],  args[2],  args[3],  args[4],  args[5],  args[6], args[7],
			args[8], args[9], args[10], args[11], args[12], args[13], args[14], args[15]
		) - 1;

		if ((usize)i < array.len - 1) {
			length += si_memcopy_s(&out[length], capacity, ", ", countof_str(", "));
		}
	}
	out[length] = '}';
	length += 1;

	return SI_BUF_LEN(u16, out, length);
}


inline
i64 si_stringToInt(siString string) {
	SI_ASSERT_NOT_NULL(string.data);

	switch (*(u8*)string.data) {
		case '-':
			string.data = (u8*)string.data + 1;
			string.len -= 1;

			return -(i64)si_stringToUInt(string);
		case '+':
			string.data = (u8*)string.data + 1;
			string.len  -= 1;
			break;
	}

	return si_stringToUInt(string);
}

inline
i64 si_stringToIntEx(siString string, i32* outRes) {
	return si_stringToIntBase(string, -1, outRes);
}

inline
i64 si_stringToIntBase(siString string, i32 base, i32* outRes) {
	SI_ASSERT_NOT_NULL(string.data);

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

	return si_stringToUIntBase(string, base, outRes);
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
	res.len = si_cast(u32, next - character);

	return res;
}
#undef FAILURE

SIDEF
siUtf8Char si_utf8Encode(i32 codepoint) {
	siUtf8Char result;

	u32 len = (codepoint >= 0x80) + (codepoint >= 0x800) + (codepoint >= 0x10000);
	if (len == 0) {
		result.codepoint[0] = (char)codepoint;
		result.len = 1;
		return result;
	}

	result.codepoint[0] = si_cast(char, ((0xFF80 >> len) & 0xFF) | (codepoint >> (6 * len)));
	result.len = len + 1;

	for_range (i, 1, result.len) {
		result.codepoint[i] = si_cast(char, 0x80 | ((codepoint >> (6 * (len - i))) & 0x3F));
	}

	return result;
}


SIDEF
usize si_utf8StrLen(siUtf8String string) {
	SI_ASSERT_NOT_NULL(string.data);

	usize offset = 0,
		  len = 0;

	while (offset > string.len) {
		offset += si_utf8Decode(&si_stringGet(string, offset)).len;
		len += 1;
	}

	return len;
}


SIDEF
siUtf32Char si_utf8StrAt(siUtf8String string, usize charIndex) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_ASSERT(charIndex < string.len);
	SI_STOPIF(string.len == 0, return SI_UNICODE_INVALID_UTF32);

	siUtf32Char character = si_utf8Decode((u8*)string.data);
	usize offset = character.len;

	while (charIndex) {
		character = si_utf8Decode(&si_stringGet(string, offset));
		offset = character.len;
		charIndex -= 1;
	}

	return character;
}
SIDEF
siString si_utf8StrAtUTF8(siUtf8String string, usize charIndex) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_ASSERT(charIndex < string.len);
	SI_STOPIF(string.len == 0, return SI_STR_EMPTY);

	siUtf32Char character = si_utf8Decode((u8*)string.data);
	if (charIndex == 0) {
		return SI_STR_LEN(string.data, character.len);

	}

	usize offset = character.len;
	charIndex -= 1;
	do {
		character = si_utf8Decode(&si_stringGet(string, offset));
		offset = character.len;
		charIndex -= 1;
	} while (charIndex);

	u8* characterPtr = &si_stringGet(string, offset);
	character = si_utf8Decode(characterPtr);
	return SI_STR_LEN(characterPtr, character.len);
}


inline
siUtf16String si_utf8ToUtf16Str(siUtf8String str, u16* out, usize capacity) {
	return si_utf8ToUtf16StrEx(str, false, out, capacity);
}

SIDEF
siUtf8String si_utf8ToUtf16StrEx(siUtf8String str, b32 nullTerm, u16* out,
		usize capacity) {
	SI_ASSERT_NOT_NULL(out);
	SI_STOPIF(str.len == 0 || capacity == 0, return SI_STR_EMPTY);

	capacity -= nullTerm;
	usize offsetUTF8 = 0, offsetUTF16 = 0;

	while (offsetUTF8 < str.len && offsetUTF16 < capacity) {
		siUtf32Char utf32 = si_utf8Decode(&si_stringGet(str, offsetUTF8));

		i32 codepoint = utf32.codepoint;
		offsetUTF8 += utf32.len;

		if (codepoint < 0xFFFF) {
			out[offsetUTF16] = (u16)codepoint;
			offsetUTF16 += 1;
		}
		else {
			SI_STOPIF(offsetUTF16 + 2 >= capacity, break);

			i32 t = codepoint - 0x10000,
				high = (t << 10) + 0xD800,
				low = t + 0xDC00;

			out[offsetUTF16 + 0] = (u16)high;
			out[offsetUTF16 + 1] = (u16)low;
			offsetUTF16 += 2;
		}
	}

	if (nullTerm) {
		out[offsetUTF16] = '\0';
		offsetUTF16 += 1;
	}

	return SI_BUF_LEN(u16, out, offsetUTF16);
}

inline
siUtf8String si_utf16ToUtf8Str(siUtf16String str, u8* out, usize capacity) {
	return si_utf16ToUtf8StrEx(str, false, out, capacity);
}

SIDEF
siUtf8String si_utf16ToUtf8StrEx(siUtf16String str, b32 nullTerm, u8* out,
		usize capacity) {
	SI_ASSERT_NOT_NULL(out);
	SI_STOPIF(str.len == 0 || capacity == 0, return SI_STR_EMPTY);

	capacity -= nullTerm;

	usize offsetUTF8 = 0,
		  offsetUTF16 = 0;
	while (offsetUTF16 < str.len && offsetUTF8 < capacity) {
		u32 chr = si_arrayGet(str, offsetUTF16, u16);

		if (chr <= 0xFF) {
			out[offsetUTF8]= (u8)chr;

			offsetUTF8 += 1;
			offsetUTF16 += 1;
		}
		else if (chr <= 0x7FF) {
			SI_STOPIF(offsetUTF8 + 2 >= capacity, break);

			out[offsetUTF8 + 0] = si_cast(u8, 0xC0 | (chr >> 6));
			out[offsetUTF8 + 1] = si_cast(u8, 0x80 | (chr & 0x3F));

			offsetUTF8 += 2;
			offsetUTF16 += 1;
		}
		else if (chr <= 0xD7FF) {
			SI_STOPIF(offsetUTF8 + 3 >= capacity, break);

			out[offsetUTF8 + 0] = si_cast(u8, 0xE0 | (chr >> 12));           /* 1110xxxx */
			out[offsetUTF8 + 1] = si_cast(u8, 0xE0 | ((chr >> 6) & 0x3F));   /* 10xxxxxx */
			out[offsetUTF8 + 2] = si_cast(u8, 0xE0 | (chr & 0x3F));          /* 10xxxxxx */

			offsetUTF8 += 3;
			offsetUTF16 += 1;
		}
		else if (chr >= 0xD800) {
			SI_STOPIF(offsetUTF8 + 4 >= capacity, break);

			u32 high = (chr - 0xD800) << 10,
				low  = si_arrayGet(str, offsetUTF16 + 1, u16) - 0xDC00;
			u32 codepoint = (high | low) + 0x10000;

			out[offsetUTF8 + 0] = si_cast(u8, 0xF0 | (codepoint >> 18));           /* 11110xxx */
			out[offsetUTF8 + 1] = si_cast(u8, 0x80 | ((codepoint >> 12) & 0x3F));  /* 10xxxxxx */
			out[offsetUTF8 + 2] = si_cast(u8, 0x80 | ((codepoint >> 6) & 0x3F));   /* 10xxxxxx */
			out[offsetUTF8 + 3] = si_cast(u8, 0x80 | (codepoint & 0x3F));          /* 10xxxxxx */

			offsetUTF8 += 4;
			offsetUTF16 += 2;
		}
	}

	if (nullTerm) {
		out[offsetUTF8] = '\0';
		offsetUTF8 += 1;
	}

	return SI_STR_LEN(out, offsetUTF8);
}

SIDEF
siUtf32Char si_utf16Decode(const u16 character[2]) {
	siUtf32Char utf32 = SI_UNICODE_INVALID_UTF32;
	i32 chr = character[0];

	if (si_between(i32, chr, 0xD800, 0xDBFF)) {
		i32 high = (chr - 0xD800) << 10,
			low = character[1] - 0xDC00;

		utf32.codepoint = (high | low) + 0x10000;
		utf32.len = 4;
	}
	else {
		utf32.codepoint = chr;
		utf32.len = 1 + (chr >= 0x80) + (chr >= 0x800);
	}

	return utf32;
}

SIDEF
siUtf8Char si_utf16Encode(const u16 character[2]) {
	siUtf32Char utf32 = si_utf16Decode(character);
	return si_utf8Encode(utf32.codepoint);
}
#endif /* SI_IMPLEMENTATION_UNICODE */

#ifdef SI_IMPLEMENTATION_CHAR

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
    SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL,
	SI__CTRL,

	SI__CTRS, SI__CTRS, SI__CTRS, SI__CTRS, SI__CTRS,

	SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL,
	SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL, SI__CTRL,
	SI__CTRL, SI__CTRL,

	SI__SPAC,

	SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC,
	SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC,

	SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX, SI__NUMX,
	SI__NUMX, SI__NUMX,

	SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC,

	SI__LTXU, SI__LTXU, SI__LTXU, SI__LTXU, SI__LTXU, SI__LTXU, SI__LETU, SI__LETU,
	SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU,
	SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU, SI__LETU,
	SI__LETU, SI__LETU,

	SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC,

	SI__LTXL, SI__LTXL, SI__LTXL, SI__LTXL, SI__LTXL, SI__LTXL, SI__LETL, SI__LETL,
	SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL,
	SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL, SI__LETL,
	SI__LETL, SI__LETL,

	SI__PUNC, SI__PUNC, SI__PUNC, SI__PUNC,
	SI__CTRL
};

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
u64 si__hashKey(u8* key, usize len);

#ifndef SI_USE_CUSTOM_HASH_FUNCTION
#define SI__FNV_OFFSET 14695981039346656037UL
#define SI__FNV_PRIME 1099511628211UL

force_inline
u64 si__hashKey(u8* key, usize len) {
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




siHashTable si_hashtableMake(const rawptr* keyArray, usize keyLen,
		const rawptr* dataArray, usize len, siAllocator alloc) {
	SI_ASSERT_NOT_NULL(keyArray);
	SI_ASSERT_NOT_NULL(dataArray);
	SI_ASSERT(keyLen != 0);

	siHashTable table = si_hashtableMakeReserve(len, alloc);
	for_range (i, 0, len) {
		si_hashtableSet(&table, keyArray[i], keyLen, dataArray[i], nil);
	}

	return table;
}

SIDEF
siHashTable si_hashtableMakeReserve(usize capacity, siAllocator alloc) {
	SI_ASSERT(capacity != 0);
	/* NOTE(EimaMei): A power of 2 capacity is enforced so that truncating a hash
	 * into a valid index would only require a 'hash & (capacity - 1)', which
	 * basically takes no time in comparison to the usual modulo operator. */
	SI_ASSERT_MSG((capacity & (capacity - 1)) == 0, "The specified capacity must be a power of 2 number (8, 64, 512, 1024, etc).");

	siHashTable table = si_arrayMakeReserve(sizeof(siHashEntry), 0, capacity, alloc);
	si_memset(table.data, 0, sizeof(siHashEntry) * table.capacity);

	return table;
}

SIDEF
rawptr si_hashtableGet(siHashTable ht, rawptr key, usize keyLen) {
	SI_ASSERT_NOT_NULL(key);
	SI_ASSERT(keyLen != 0);
	SI_STOPIF(ht.len == 0, return nil);

	u64 hash = si__hashKey((u8*)key, keyLen);
	return si_hashtableGetWithHash(ht, hash);
}
SIDEF
rawptr si_hashtableGetWithHash(siHashTable ht, u64 hash) {
	usize index = hash & (ht.capacity - 1);

	siHashEntry* entry = &si_arrayGet(ht, index, siHashEntry);
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
siHashEntry* si_hashtableSet(siHashTable* ht, const rawptr key, usize keyLen,
		const rawptr valuePtr, b32* outSuccess) {
	SI_ASSERT_NOT_NULL(key);
	SI_ASSERT(keyLen != 0);

	u64 hash = si__hashKey((u8*)key, keyLen);
	return si_hashtableSetWithHash(ht, hash, valuePtr, outSuccess);
}
SIDEF
siHashEntry* si_hashtableSetWithHash(siHashTable* ht, u64 hash, const rawptr valuePtr,
		b32* outSuccess) {
	SI_ASSERT_NOT_NULL(ht);
	SI_ASSERT_MSG(ht->len < ht->capacity, "The capacity of the hash table has been surpassed.");


	usize index = hash & (ht->capacity - 1);
	siHashEntry* entry = &si_arrayGet(*ht, index, siHashEntry);
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

siIntern
void si__fileErrorLog(siError* error) {
	siPrintColor red = si_printColor3bitEx(siPrintColor3bit_Red, true, false);

#ifndef SI_NO_ERROR_STRUCT
	si_fprintf(
		si_stderr, "%CFile system error at \"%s:%i\"%C: %S: %S\n",
		red, error->filename, error->line,
		si_systemErrorName(error->code), si_systemErrorDesc(error->code)
	);

#else
	si_fprintf(
		si_stderr, "%CFile system error%C: %S: %S\n",
		red, si_systemErrorName(error->code), si_systemErrorDesc(error->code)
	);

#endif
}

#define siErrorSystem_FULL(condition, error, errorCode, ...) \
	SI_ERROR_CHECK_EX(condition, error, errorCode, si__fileErrorLog, __VA_ARGS__)

#define siErrorSystem_CHECK_EX(condition, error, errorCode, returnValue) \
	siErrorSystem_FULL(condition, error, errorCode, return returnValue)

#define siErrorSystem_CHECK(condition, error, returnValue) \
	siErrorSystem_CHECK_EX(condition, error, si__fileGetOSError(), returnValue)

#define siErrorSystem_CHECK_OPT(condition, type) \
	{ \
		siOption(type) output = {0}; \
		siErrorSystem_CHECK(condition, &output.data.error, output); \
	}


inline
siString si_systemErrorName(siErrorSystem error) {
	SI_ASSERT(error < siErrorSystem_Length);

    static const siString names[] = {
        SI_STRC("siErrorSystem_None"),
        SI_STRC("siErrorSystem_Exists"),
        SI_STRC("siErrorSystem_NotExists"),
        SI_STRC("siErrorSystem_Invalid"),
        SI_STRC("siErrorSystem_InvalidFilename"),
        SI_STRC("siErrorSystem_Permission"),
        SI_STRC("siErrorSystem_NoMemory"),
        SI_STRC("siErrorSystem_TruncationFail"),
		SI_STRC("siErrorSystem_Generic"),
    };
	return names[error];
}

inline
siString si_systemErrorDesc(siErrorSystem error) {
	SI_ASSERT(error < siErrorSystem_Length);

	static const siString descriptions[] = {
        SI_STRC("No error has been encountered."),
        SI_STRC("File or directory already exists."),
        SI_STRC("File or directory doesn't exist."),
        SI_STRC("Provided value is invalid."),
        SI_STRC("Invalid filename."),
        SI_STRC("User doesn't have sufficient permissions."),
        SI_STRC("Out of memory."),
        SI_STRC("Failed to truncate the file."),
        SI_STRC("An unspecified, platform-specific error occurred."),
    };
	return descriptions[error];
}

siIntern
siErrorSystem si__fileGetOSError(void) {
#if SI_SYSTEM_IS_WINDOWS
	switch (GetLastError()) {
		case ERROR_NO_MORE_FILES:
		case ERROR_SUCCESS: return siErrorSystem_None;
		case ERROR_ALREADY_EXISTS:
		case ERROR_FILE_EXISTS: return siErrorSystem_Exists;
		case ERROR_INVALID_NAME:
		case ERROR_BAD_PATHNAME: return siErrorSystem_InvalidFilename;
		case ERROR_DIRECTORY:
		case ERROR_INVALID_ADDRESS: return siErrorSystem_Invalid;
		case ERROR_PATH_NOT_FOUND:
		case ERROR_FILE_NOT_FOUND: return siErrorSystem_NotExists;
		case ERROR_ACCESS_DENIED: return siErrorSystem_Permission;
		case ERROR_NOT_ENOUGH_MEMORY: return siErrorSystem_NoMemory;
	}
	return siErrorSystem_Invalid;
#else
	switch (errno) {
		case 0: return siErrorSystem_None;
		case EEXIST: return siErrorSystem_Exists;
		case EINVAL: return siErrorSystem_Invalid;
		case EISDIR:
		case ENAMETOOLONG: return siErrorSystem_InvalidFilename;
		case ENOENT: return siErrorSystem_NotExists;
		case EPERM:
		case EACCES: return siErrorSystem_Permission;
		case ENOMEM: return siErrorSystem_NoMemory;
	}

	return siErrorSystem_Invalid;
#endif
}

noreturn inline
void si_exit(u32 code) {
#if SI_SYSTEM_IS_WINDOWS
	ExitProcess(code);
#else
	exit(code);
#endif
}


SIDEF
b32 si_envVarSet(siString name, siString value) {
#if SI_SYSTEM_IS_WINDOWS
	siUtf16String paths[2];
	u16 stack[SI_PATH_MAX];
	si_pathToWin32Mul(name, value, stack, countof(stack), paths);

	return SetEnvironmentVariableW((u16*)paths[0].data, (u16*)paths[1].data) != 0;
#else
	char* paths[2];
	u8 stack[SI_PATH_MAX];
	si_pathToUnixMul(name, value, stack, countof(stack), paths);

	return setenv(paths[0], paths[1], 1) == 0;
#endif
}

SIDEF
b32 si_envVarUnset(siString name) {
#if SI_SYSTEM_IS_WINDOWS
	u16 stack[SI_PATH_MAX];
	si_pathToWin32(name, stack, countof(stack));

	return SetEnvironmentVariableW(stack, nil) != 0;
#else
	u8 stack[SI_PATH_MAX];
	return unsetenv(si_pathToUnix(name, stack, countof(stack))) == 0;
#endif
}

#if SI_SYSTEM_IS_UNIX
	typedef char siOsChar;
	typedef char* siOsString;
#else
	#error "d"
#endif


SIDEF usize si_envVarGetLength(siString name);
SIDEF usize si_envVarGetLengthOS(siOsString name);

SIDEF siString si_envVarGetData(siString name, u8* out, usize capacity);
SIDEF siString si_envVarGetDataEx(siString name, u8* out, usize bytesToCopy);
SIDEF siString si_envVarGetDataOS(siOsString name, u8* out, usize bytesToCopy);

inline
usize si_envVarGetLength(siString name) {
	siOsChar stack[SI_PATH_MAX];

}

inline
siString si_envVarGet(siString name, u8* out, usize capacity) {
	return si_envVarGetLen(name, out, si_min(usize, capacity, si_envVarGetLength(name)));
}

SIDEF
siString si_envVarGetEx(siString name, u8* out, usize bytesToCopy) {


siString si_envVarGetEx(siString name, u8* out, usize bytesToCopy) {
	SI_ASSERT_NOT_NULL(out);

#if SI_SYSTEM_IS_WINDOWS
	u16 stack[SI_PATH_MAX];
	siUtf16String nameWide = si_pathToWin32(name, stack, countof(stack));

	u32 len = (u32)(countof(stack) - nameWide.len);
	len = GetEnvironmentVariableW(nameWide.data, &stack[nameWide.len], len);
	SI_STOPIF(len == 0, return SI_STR_EMPTY);

	return si_utf16ToUtf8Str(SI_BUF_LEN(u16, &stack[nameWide.len], len), out, capacity);
#else
	siAllocator stack = si_allocatorMakeStack(SI_KILO(4));
	cstring nameU = si_stringCopyToCstr(name, stack);

	cstring content = getenv(nameU);
	SI_STOPIF(content == nil, return SI_STR_EMPTY);

	usize len = si_cstrLen(content);
	si_memcopy_s(out, capacity, content, len);

	return SI_STR_LEN(content, len);

#endif
}

SIDEF
siWindowsVersion si_windowsGetVersion(void) {
#if SI_SYSTEM_IS_WINDOWS
	OSVERSIONINFOEXW info;
	{
		siDllHandle ntdll = si_dllLoad("ntdll.dll");

		typedef LONG(WINAPI* RtlGetVersionProc)(POSVERSIONINFOEXW);
		RtlGetVersionProc _RtlGetVersion = si_transmute(RtlGetVersionProc, si_dllProcAddress(ntdll, "RtlGetVersion"), siDllProc);

		info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
		_RtlGetVersion(&info);
		si_dllUnload(ntdll);
	}

	/* Windows 10 and 11. */
	if (info.dwMajorVersion == 10 && info.dwMinorVersion == 0) {
		return (info.dwBuildNumber >= 22000) ? siWindowsVersion_11 : siWindowsVersion_10;
	}

	/* Windows 8.1, 8, 7 and Vista. */
	if (info.dwMajorVersion == 6) {
		return siWindowsVersion_Vista + info.dwMinorVersion;
	}

	return (info.dwMajorVersion == 5 && (info.dwMinorVersion == 1 || info.dwMinorVersion == 2))
		? siWindowsVersion_XP
		: 0;
#else
	return -1;
#endif
}

SIDEF
b32 si_unixIsWayland(void) {
#if SI_SYSTEM_IS_UNIX
	static b32 isWayland = UINT32_MAX;
	SI_STOPIF(isWayland != UINT32_MAX, return isWayland);

	u8 buf[1];
	siString res = si_envVarGet(SI_STR("WAYLAND_DISPLAY"), buf, sizeof(buf));
	isWayland = (res.data != nil);

	return isWayland;
#else
	return false;
#endif
}

inline
b32 si_unixIsX11(void) {
#if SI_SYSTEM_IS_UNIX
	return !si_unixIsWayland();
#else
	return false;
#endif
}

SIDEF
siUnixDE si_unixGetDE(void) {
#if SI_SYSTEM_IS_UNIX
	static siUnixDE de = -1;
	SI_STOPIF(de != -1, return de);

	u8 buf[8];
	siString res = si_envVarGet(SI_STR("XDG_CURRENT_DESKTOP"), buf, sizeof(buf));
	SI_STOPIF(res.data == nil, return 0);

	if (si_stringEqual(res, SI_STR("KDE"))) { de = siUnixDE_KDE; }
	else if (si_stringEqual(res, SI_STR("GNOME"))) { de = siUnixDE_GNOME; }
	else if (si_stringEqual(res, SI_STR("XFCE"))) { de = siUnixDE_Xfce; }
	else { de = 0; }

	return de;
#else
	return -1;
#endif

}

#endif /* SI_IMPLEMENTATION_SYSTEM */

#ifdef SI_IMPLEMENTATION_IO

#if SI_COMPILER_MSVC
	#pragma comment(lib, "shell32")
	#pragma comment(lib, "Advapi32")
#endif

SIDEF
b32 si_pathExists(siString path) {
	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[SI_PATH_MAX];
		si_pathToWin32(path, stack, countof(stack));

		return GetFileAttributesW(stack) != INVALID_FILE_ATTRIBUTES;

	#else
		u8 stack[SI_PATH_MAX];
		struct stat tmp;

		return stat(si_pathToUnix(path, stack, countof(stack)), &tmp) == 0;

	#endif
}
SIDEF
siResult(usize) si_pathCopy(siString pathSrc, siString pathDst) {
	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[SI_PATH_MAX * 2];

		siUtf16String paths[2];
		si_pathToWin32Mul(pathSrc, pathDst, stack, countof(stack), paths);

		i32 size = CopyFileW((u16*)paths[0].data, (u16*)paths[1].data, true);
		siErrorSystem_CHECK_OPT(size == 0, usize);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		cstring src = si_stringCopyToCstr(pathSrc, stack),
				dst = si_stringCopyToCstr(pathDst, stack);

		int fileSrc = open(src, O_RDONLY, 0);
		siErrorSystem_CHECK_OPT(fileSrc == -1, usize);

		int fileDst = open(dst, O_WRONLY | O_CREAT, 0666);
		siErrorSystem_CHECK_OPT(fileDst == -1, usize);

		struct stat stat;
		int res = fstat(fileDst, &stat);
		siErrorSystem_CHECK_OPT(res == -1, usize);

		#if SI_SYSTEM_IS_UNIX
			isize size = sendfile64(fileDst, fileSrc, 0, stat.st_size);
		#else
			isize size = sendfile(fileDst, fileSrc, 0, &stat.st_size, NULL, 0);
		#endif
		siErrorSystem_CHECK_OPT(size == -1, usize);

		close(fileSrc);
		close(fileDst);

	#endif

	return SI_OPT(usize, (usize)size);
}
SIDEF
siResult(u32) si_pathItemsCopy(siString pathSrc, siString pathDst) {
	SI_ASSERT(pathDst.len <= SI_PATH_MAX);

	siDirectory dir = si_directoryOpen(pathSrc);
	u32 itemsCopied = 0;

	char dst[SI_PATH_MAX];
	char* dstBuffer;
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
			SI_STOPIF(err.code != 0, return SI_OPT_ERR(u32, err));

			siResult(u32) res = si_pathItemsCopy(entry.path, dstStr);
			SI_STOPIF(!res.hasValue, return res);
			itemsCopied += res.data.value;
			continue;
		}

		si_pathCopy(entry.path, dstStr);
		itemsCopied += 1;
	}

	return SI_OPT(u32, itemsCopied);
}

SIDEF
siError si_pathMove(siString pathSrc, siString pathDst) {
	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[SI_PATH_MAX * 2];

		siUtf16String paths[2];
		si_pathToWin32Mul(pathSrc, pathDst, stack, countof(stack), paths);

		i32 res = MoveFileW((u16*)paths[0].data, (u16*)paths[1].data);
		siErrorSystem_CHECK(res == 0, &error, error);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		cstring src = si_stringCopyToCstr(pathSrc, stack),
				dst = si_stringCopyToCstr(pathDst, stack);

		int res = link(src, dst);
		siErrorSystem_CHECK(res != 0, &error, error);

		res = unlink(src);
		siErrorSystem_CHECK(res != 0, &error, error);

	#endif

	return error;
}

inline
siError si_pathRename(siString path, siString newPath) {
	return si_pathMove(path, newPath);
}

SIDEF
siError si_pathCreateFolder(siString path) {
	SI_ASSERT(path.len <= SI_PATH_MAX);

	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[SI_PATH_MAX];
		si_pathToWin32(path, stack, countof(stack));

		i32 res = CreateDirectoryW(stack, nil);
		siErrorSystem_CHECK(res == 0, &error, error);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		cstring str = si_stringCopyToCstr(path, stack);

		/* NOTE(EimaMei): For whatever reason, 'mkdir' will sometimes return -1
		 * but still create the folder and set 'errno' to 0. What? */
		int res = mkdir(str, 0777);
		siErrorSystem_CHECK(res != 0, &error, error);

	#endif

	return error;
}

#if !SI_SYSTEM_IS_WINDOWS
// TODO(EimaMei): Replace this.

force_inline
i32 si__unlinkCb(cstring path, const struct stat* sb, i32 typeflag, struct FTW* ftwbuf) {
	return remove(path);
	SI_UNUSED(sb); SI_UNUSED(typeflag); SI_UNUSED(ftwbuf);
}
#endif

SIDEF
siError si_pathRemove(siString path) {
	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 pathWide[SI_PATH_MAX];
		si_pathToWin32(path, pathWide, countof(pathWide));

		u32 attrs = GetFileAttributesW(pathWide);
		siErrorSystem_CHECK(attrs == INVALID_FILE_ATTRIBUTES, &error, error);

		b32 res;
		if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
			siDirectoryEntry entry;
			siDirectory dir = si_directoryOpen(path);
			while (si_directoryPollEntry(&dir, &entry)) {
				siError err = si_pathRemove(entry.path);
				SI_STOPIF(err.code != 0, return err);
			}

			res = RemoveDirectoryW(pathWide);
		}
		else {
			res = DeleteFileW(pathWide);
		}
		siErrorSystem_CHECK(res == false, &error, error);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		cstring pathCstr = si_stringCopyToCstr(path, stack);
		int res = nftw(pathCstr, si__unlinkCb, 64, FTW_DEPTH | FTW_PHYS);
		siErrorSystem_CHECK(res == -1, &error, error);

	#endif

	return error;
}

SIDEF
siError si_pathCreateHardLink(siString path, siString pathLink) {
	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[SI_PATH_MAX * 2];

		siUtf16String paths[2];
		si_pathToWin32Mul(path, pathLink, stack, countof(stack), paths);

		i32 res = CreateHardLinkW((u16*)paths[1].data, (u16*)paths[0].data, nil);
		siErrorSystem_CHECK(res == 0, &error, error);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		int res = link(
			si_stringCopyToCstr(path, stack),
			si_stringCopyToCstr(pathLink, stack)
		);
		siErrorSystem_CHECK(res == -1, &error, error);

	#endif

	return error;
}

SIDEF
siError si_pathCreateSoftLink(siString path, siString pathLink) {
	SI_ASSERT(path.len <= SI_PATH_MAX);
	SI_ASSERT(pathLink.len <= SI_PATH_MAX);

	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[SI_PATH_MAX * 2];

		siUtf16String paths[2];
		si_pathToWin32Mul(path, pathLink, stack, countof(stack), paths);

		u32 attrs = GetFileAttributesW((u16*)paths[0].data);
		siErrorSystem_CHECK(attrs == INVALID_FILE_ATTRIBUTES, &error, error);

		i32 res = CreateSymbolicLinkW(
			(u16*)paths[1].data, (u16*)paths[0].data,
			attrs & FILE_ATTRIBUTE_DIRECTORY
		);
		siErrorSystem_CHECK(res == 0, &error, error);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		int res = symlink(
			si_stringCopyToCstr(path, stack),
			si_stringCopyToCstr(pathLink, stack)
		);
		siErrorSystem_CHECK(res == -1, &error, error);

	#endif

	return error;
}

inline
siString si_pathBaseName(siString path) {
	SI_ASSERT(path.len <= SI_PATH_MAX);

	for_eachRevStr (x, path) {
		if (*x == SI_PATH_SEPARATOR) {
			return si_stringSubToEnd(path, si_pointerDiff(path.data, x + 1));
		}
	}
	return path;
}

SIDEF
siString si_pathUnrooted(siString path) {
	#if SI_SYSTEM_IS_WINDOWS
		usize offset = 3 * si_pathIsAbsolute(path);
	#else
		usize offset = si_pathIsAbsolute(path);
	#endif
	path = si_stringSubToEnd(path, offset);

	for_eachStr (x, path) {
		if (*x == SI_PATH_SEPARATOR) {
			return si_stringSubToEnd(path, si_pointerDiff(path.data, x + 1));
		}
	}
	return path;
}

SIDEF
siString si_pathExtension(siString path) {
	SI_ASSERT(path.len <= SI_PATH_MAX);

	for_eachStr (x, path) {
		if (*x == '.') {
			return si_stringSubToEnd(path, si_pointerDiff(path.data, x + 1));
		}
		else if (*x == SI_PATH_SEPARATOR) {
			break;
		}
	}

	return SI_STR("");
}

SIDEF
siResult(siString) si_pathGetFullName(siString path, siAllocator alloc) {
	u8 pathOut[SI_PATH_MAX];

	#if SI_SYSTEM_IS_WINDOWS
		usize len;
		u16 stack[SI_PATH_MAX * 2];

		siUtf16String tmp = si_pathToWin32(path, stack, countof(stack));
		DWORD dif = si_cast(u32, countof(stack) - tmp.len);

		len = GetFullPathNameW(stack, dif, &stack[tmp.len], nil);
		siErrorSystem_CHECK_OPT(len == 0, siString);

		siUtf8String res = si_utf16ToUtf8Str(
			SI_BUF_LEN(u16, &stack[tmp.len], len),
			pathOut, countof(pathOut)
		);
		return SI_OPT(siString, si_stringCopy(res, alloc));

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

		realpath(si_stringCopyToCstr(path, stack), pathOut);
		siErrorSystem_CHECK_OPT(pathOut == nil, siString);

		return SI_OPT(siString, si_stringMake(pathOut, alloc));

	#endif
}


inline
b32 si_pathIsAbsolute(siString path) {
	SI_ASSERT(path.len <= SI_PATH_MAX);
	u8* data = (u8*)path.data;

	#if SI_SYSTEM_IS_WINDOWS
		return (
			si_charIsAlpha(data[0])
			&& data[1] == ':'
			&& data[2] == SI_PATH_SEPARATOR
			&& data[3] != '\0'
		);
	#else
		return (data[0] == SI_PATH_SEPARATOR && path.len != 1);
	#endif
}

inline
b32 si_pathIsRelative(siString path) {
	return !si_pathIsAbsolute(path);
}

SIDEF
siUtf16String si_pathToWin32(siString path, u16* out, usize capacity) {
	SI_ASSERT(path.len <= SI_KILO(4));
	usize i = 0;

	if (path.len > 260 && si_pathIsAbsolute(path)) {
		out[0] = '\\';
		out[1] = '\\';
		out[2] = '?';
		out[3] = '\\';
		i += 4;

		siUtf16String res = si_utf8ToUtf16StrEx(path, true, &out[i], capacity - i);
		res.data = out;
		res.len += i;

		return res;
	}

	return si_utf8ToUtf16StrEx(path, true, out, capacity);
}

inline
void si_pathToWin32Mul(siString pathSrc, siString pathDst, u16* outBuf, usize capacity,
		siUtf16String out[2]) {
	out[0] = si_pathToWin32(pathSrc, outBuf, capacity);
	out[1] = si_pathToWin32(pathDst, &outBuf[out[0].len], capacity - out[0].len);
}


SIDEF
char* si_pathToUnix(siString path, u8* out, usize capacity) {
	SI_ASSERT(path.len <= SI_KILO(4));
	SI_ASSERT(capacity == 0);

	si_memcopy_s(out, capacity - 1, path.data, path.len);
	out[path.len] = '\0';

	return (char*)out;
}

inline
void si_pathToUnixMul(siString pathSrc, siString pathDst, u8* outBuf, usize capacity,
		char* out[2]) {
	out[0] = si_pathToUnix(pathSrc, outBuf, capacity);
	out[1] = si_pathToUnix(pathDst, &outBuf[pathSrc.len + 1], capacity - (pathSrc.len + 1));
}




SIDEF
u64 si_pathLastWriteTime(siString path) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME lastWriteTime = {0};
	WIN32_FILE_ATTRIBUTE_DATA data = {0};

	u16 stack[SI_PATH_MAX];
	si_pathToWin32(path, stack, countof(stack));

	if (GetFileAttributesExW(stack, GetFileExInfoStandard, &data)) {
		lastWriteTime = data.ftLastWriteTime;
	}

	ULARGE_INTEGER res;
	res.LowPart = lastWriteTime.dwLowDateTime;
	res.HighPart = lastWriteTime.dwHighDateTime;

	return si_timeToUnix(res.QuadPart);

#else
	/* TODO(EimaMei): Check if this is correct. */
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
	struct stat fs;
	int res = stat(si_stringCopyToCstr(path, stack), &fs);
	return (res == 0) ? fs.st_mtime : 0;

#endif
}

SIDEF
siString si_pathGetTmp(void) {
#if SI_SYSTEM_IS_WINDOWS
	static u8 buffer[256];
	u16 stack[256];

	u32 len = GetTempPathW(countof(stack), stack);
	SI_ASSERT(len != 0); /* NOTE(EimaMei): This musn't fail. */

	return si_utf16ToUtf8Str(SI_BUF_LEN(u16, stack, len), buffer, countof(buffer));

#else
	return SI_STR("/tmp");

#endif
}


inline
siString si_pathReadContents(siString path, siAllocator alloc) {
	siFile file = si_fileOpen(path);
	siString res = si_fileReadContents(file, alloc);
	si_fileClose(file);

	return res;
}
inline
siError si_pathReadContentsBuf(siString path, siBuffer* out) {
	siFile file = si_fileOpen(path);
	siError res = si_fileReadContentsBuf(file, out);
	si_fileClose(file);

	return res;
}

static b32 SI_STD_FILE_SET = false;
static siFile SI_STD_FILE_ARR[siStdFile_Count] = {0};

SIDEF
siFile* si_fileGetStdFile(siStdFile type) {
	SI_ASSERT_MSG(si_between(i32, type, siStdFile_Input, siStdFile_Error), "Invalid STD file type.");

	#define SET_STD_FILE(index, type) SI_STD_FILE_ARR[index].handle = type

	if (!SI_STD_FILE_SET) {
		#if SI_SYSTEM_IS_WINDOWS
			SET_STD_FILE(0, GetStdHandle(STD_INPUT_HANDLE));
			SET_STD_FILE(1, GetStdHandle(STD_OUTPUT_HANDLE));
			SET_STD_FILE(2, GetStdHandle(STD_ERROR_HANDLE));

			if (IsValidCodePage(CP_UTF8)) {
				SetConsoleOutputCP(CP_UTF8);
			}
		#else
			SET_STD_FILE(0, 0);
			SET_STD_FILE(1, 1);
			SET_STD_FILE(2, 2);
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
	SI_ASSERT((mode & ~siFileMode_All) == 0);

	siFile res;

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

	rawptr handle;
	{
		u16 stack[SI_PATH_MAX];
		si_pathToWin32(path, stack, countof(stack));

		handle = CreateFileW(
			stack, access, FILE_SHARE_READ | FILE_SHARE_DELETE, nil,
			disposition, FILE_ATTRIBUTE_NORMAL, nil
		);
	}
	siErrorSystem_CHECK(handle == INVALID_HANDLE_VALUE, &res.error, res);

	if (mode & siFileMode_Append) {
		LARGE_INTEGER offset = {0};
		i32 status = SetFilePointerEx(handle, offset, nil, FILE_END);
		if (!status) {
			CloseHandle(handle);
			SI_ERROR(&res.error, si__fileGetOSError());
			return res;
		}
	}

#else
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
			flags = 0;
			SI_PANIC_MSG("Invalid file mode.");
	}

	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
	cstring pathCstr = si_stringCopyToCstr(path, stack);
	i64 handle = open(pathCstr, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	siErrorSystem_CHECK(handle == -1, &res.error, res);

#endif
	res.handle = handle;
	res.size = si_fileSize(res);
	res.error = (siError){0};

	return res;
}

SIDEF
usize si_fileSize(siFile file) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);

#if SI_SYSTEM_IS_WINDOWS
	ULARGE_INTEGER res = {0};

	BY_HANDLE_FILE_INFORMATION data;
	if (GetFileInformationByHandle(file.handle, &data)) {
		res.HighPart = data.nFileSizeHigh;
		res.LowPart = data.nFileSizeLow;
	}

	return (USIZE_MAX < res.QuadPart)
		? USIZE_MAX
		: (usize)res.QuadPart;

#else
	isize prevOffset = si_fileTell(file);
	usize len = si_fileSeek(file, 0, siSeekWhere_End);
	si_fileSeek(file, prevOffset, siSeekWhere_Begin);

	return len;

#endif
}

inline
void si_fileSizeUpdate(siFile* file) {
	SI_ASSERT_NOT_NULL(file);
	file->size = si_fileSize(*file);
}

inline
siResult(siBuffer) si_fileRead(siFile file, usize len, siAllocator alloc) {
	return si_fileReadAt(file, si_fileTell(file), len, alloc);
}
inline
siResult(siBuffer) si_fileReadAt(siFile file, isize offset, usize len, siAllocator alloc) {
	len = si_min(usize, si_allocatorAvailable(alloc), len);

	siBuffer buffer = si_arrayMakeReserve(1, len, len, alloc);
	siError err = si_fileReadUnsafe(file, offset, len, &buffer);

	return (err.code == 0)
		? SI_OPT(siBuffer, buffer)
		: SI_OPT_ERR(siBuffer, err);
}

inline
siError si_fileReadBuf(siFile file, usize len, siBuffer* out) {
	return si_fileReadAtBuf(file, si_fileTell(file), len, out);
}
inline
siError si_fileReadAtBuf(siFile file, isize offset, usize len, siBuffer* out) {
	return si_fileReadUnsafe(file, offset, si_min(usize, len, out->capacity), out);
}

SIDEF
siError si_fileReadUnsafe(siFile file, isize offset, usize len, siBuffer* out) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
	SI_ASSERT_NOT_NULL(out);
	SI_ASSERT_NOT_NULL(out->data);

	siError error = SI_ERROR_DEFAULT;
	si_fileSeek(file, offset, siSeekWhere_Begin);

#if SI_SYSTEM_IS_WINDOWS
	DWORD bytesRead;
	DWORD toRead = (len <= UINT32_MAX)
		? (DWORD)len
		: UINT32_MAX;
	SI_ASSERT_MSG(len <= UINT32_MAX, "TODO: Fix this.");

	i32 res = ReadFile(file.handle, out->data, toRead, &bytesRead, NULL);
	siErrorSystem_CHECK(res == 0, &error, error);

#else
	isize bytesRead = pread((int)file.handle, out->data, len, offset);
	siErrorSystem_CHECK(bytesRead == -1, &error, error);

#endif

	out->len = bytesRead;
	return error;
}

SIDEF
siString si_fileReadContents(siFile file, siAllocator alloc) {
	usize len = si_min(usize, si_allocatorAvailable(alloc), file.size);
	siString buffer = si_arrayMakeReserve(1, 0, len, alloc);

	isize oldOffset = si_fileTell(file);
	siError err = si_fileReadUnsafe(file, 0, len, &buffer);
	si_fileSeek(file, oldOffset, siSeekWhere_Begin);

	return (err.code == 0) ? buffer : SI_STR_EMPTY;
}
SIDEF
siError si_fileReadContentsBuf(siFile file, siBuffer* out) {
	isize oldOffset = si_fileTell(file);
	siError res = si_fileReadUnsafe(
		file, 0, si_min(usize, out->capacity, file.size), out
	);
	si_fileSeek(file, oldOffset, siSeekWhere_Begin);

	return res;
}

inline
siArray(siString) si_fileReadlines(siFile file, siAllocator alloc) {
	return si_stringSplit(si_fileReadContents(file, alloc), SI_STR("\n"), alloc);
}


inline
isize si_fileWrite(siFile* file, siBuffer content) {
	return si_fileWriteAt(file, content, si_fileTell(*file));
}
SIDEF
isize si_fileWriteAt(siFile* file, siBuffer content, isize offset) {
	SI_ASSERT_NOT_NULL(file);
	SI_ASSERT_NOT_NULL((rawptr)file->handle);
	SI_ASSERT_NOT_NULL(content.data);
	SI_ASSERT(content.len != USIZE_MAX);

#if SI_SYSTEM_IS_WINDOWS
	si_fileSeek(*file, offset, siSeekWhere_Begin);

	DWORD bytesWritten;
	DWORD length = (content.len < UINT32_MAX)
		? (DWORD)content.len
		: UINT32_MAX;
	b32 res = WriteFile(file->handle, content.data, length, &bytesWritten, NULL);
	siErrorSystem_CHECK(res == 0, &file->error, -1);

#else
	isize curOffset = si_fileSeek(*file, 0, siSeekWhere_Current);
	isize bytesWritten = (curOffset == offset)
		? write((int)file->handle, content.data, content.len)
		: pwrite((int)file->handle, content.data, content.len, offset);
	siErrorSystem_CHECK(bytesWritten == -1, &file->error, -1);

#endif

	file->size += (offset - file->size) + bytesWritten;
	return bytesWritten;
}

inline
isize si_fileTell(siFile file) {
	return si_fileSeek(file, 0, siSeekWhere_Current);
}

inline
isize si_fileSeek(siFile file, isize offset, siSeekWhere method) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);

#if SI_SYSTEM_IS_WINDOWS
	LARGE_INTEGER res;
	res.QuadPart = offset;

	SetFilePointerEx(file.handle, res, &res, method);
	return (ISIZE_MAX < res.QuadPart)
		? ISIZE_MAX
		: (isize)res.QuadPart;

#elif SI_SYSTEM_IS_APPLE
	return lseek((int)file.handle, offset, method);

#else
	return lseek64((int)file.handle, offset, method);

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
b32 si_fileTruncate(siFile* file, usize size) {
	SI_ASSERT_NOT_NULL(file);
	SI_ASSERT_NOT_NULL((rawptr)file->handle);

#if SI_SYSTEM_IS_WINDOWS
	usize prevOffset = si_fileTell(*file);
	isize res = si_fileSeek(*file, size, siSeekWhere_Begin);
	SI_STOPIF(res == 0, return false);

	res = SetEndOfFile(file->handle);
	siErrorSystem_CHECK_EX(res == 0, &file->error, siErrorSystem_TruncationFail, false);

	si_fileSeek(*file, prevOffset, siSeekWhere_Begin);
#else
	int res = ftruncate((int)file->handle, size);
	siErrorSystem_CHECK_EX(res == -1, &file->error, siErrorSystem_TruncationFail, false);
	//si_fileSeek(*file, prevOffset, siSeekWhere_Begin);

#endif

	return true;
}

SIDEF
u64 si_fileLastWriteTime(siFile file) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME lastWriteTime = {0};

	BY_HANDLE_FILE_INFORMATION data;
	if (GetFileInformationByHandle(file.handle, &data)) {
		lastWriteTime = data.ftLastWriteTime;
	}

	ULARGE_INTEGER res;
	res.HighPart = lastWriteTime.dwHighDateTime;
	res.LowPart = lastWriteTime.dwLowDateTime;

	return si_timeToUnix(res.QuadPart);

#else
	struct stat fs;
	int res = fstat((int)file.handle, &fs);
	return (res == 0) ? fs.st_mtime : 0;

#endif


}

inline
void si_fileClose(siFile file) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
#if SI_SYSTEM_IS_WINDOWS
	CloseHandle(file.handle);
#else
	close((int)file.handle);
#endif
}

SIDEF
siDirectory si_directoryOpen(siString path) {
	SI_ASSERT_NOT_NULL(path.data);
	SI_ASSERT(path.len <= SI_PATH_MAX);

	siDirectory dir;
	dir.error = SI_ERROR_DEFAULT;
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
		stack, countof(stack) - 2
	);

	stack[pathWide.len + 0] = '*';
	stack[pathWide.len + 1] = '\0';

	WIN32_FIND_DATAW tmp;
	HANDLE handle = FindFirstFileW(stack, &tmp);
	siErrorSystem_CHECK(handle == INVALID_HANDLE_VALUE, &dir.error, dir);

	dir.handle = handle;
	SI_DISCARD(FindNextFileW(handle, &tmp));

#else
	/* NOTE(EimaMei): We do this because opendir only takes NULL-terminated C-strings. */
	dir.buffer[dir.directoryLen] = '\0';

	dir.handle = opendir((char*)dir.buffer);
	siErrorSystem_CHECK(dir.handle == nil, &dir.error, dir);

	/* NOTE(EimaMei): We skip '.' and '..'. */
	SI_DISCARD(readdir((DIR*)dir.handle));
	SI_DISCARD(readdir((DIR*)dir.handle));

#endif

	return dir;
}

inline
b32 si_directoryPollEntry(siDirectory* dir, siDirectoryEntry* out) {
	return si_directoryPollEntryEx(dir, true, out);
}

SIDEF
b32 si_directoryPollEntryEx(siDirectory* dir, b32 fullPath, siDirectoryEntry* out) {
	SI_ASSERT_NOT_NULL(dir);
	SI_ASSERT_NOT_NULL(out);
	SI_ASSERT_NOT_NULL(dir->handle);

#if SI_SYSTEM_IS_WINDOWS
	WIN32_FIND_DATAW file;
	if (FindNextFileW(dir->handle, &file) == 0) {
		siErrorSystem code = si__fileGetOSError();
		si_directoryClose(dir);

		siErrorSystem_CHECK_EX(code != siErrorSystem_None, &dir->error, code, false);
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

	/* TODO(EimaMei): Replace wcslen with a sili function. */
	out->path = si_utf16ToUtf8Str(
		SI_BUF_LEN(u16, file.cFileName, wcslen(file.cFileName)),
		&dir->buffer[dir->directoryLen], sizeof(dir->buffer) - dir->directoryLen
	);

#else
	struct dirent* dirEntry = readdir((DIR*)dir->handle);
	if (dirEntry == nil) {
		errno = 0;
		siErrorSystem code = si__fileGetOSError();
		si_directoryClose(dir);

		siErrorSystem_CHECK_EX(code != siErrorSystem_None, &dir->error, code, false);
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

	usize len = si_cstrLen(dirEntry->d_name);
	out->path.data = &dir->buffer[dir->directoryLen];
	out->path.len = si_memcopy_s(
		out->path.data, sizeof(dir->buffer) - dir->directoryLen,
		dirEntry->d_name, len
	);

#endif
	usize dirLen = dir->directoryLen * fullPath;
	out->path.data = si_pointerSub(out->path.data, dirLen);
	out->path.len += dirLen;
	out->path.capacity += dirLen;

	return true;
}

inline
void si_directoryClose(siDirectory* dir) {
	SI_ASSERT_NOT_NULL(dir);
	SI_STOPIF(dir->handle == nil, return);

#if SI_SYSTEM_IS_WINDOWS
	CloseHandle(dir->handle);
#else
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

#else

siIntern
rawptr si__threadProc(rawptr arg) {
	siThread* t = (siThread*)arg;
	t->returnValue = t->func(t->arg);
	t->isRunning = false;

	return nil;
}
#endif


inline
siThread si_threadMake(siThreadFunction function, rawptr arg) {
	return si_threadMakeEx(function, arg, 0);
}

inline
siThread si_threadMakeEx(siThreadFunction function, rawptr arg, usize stackSize) {
	SI_ASSERT_NOT_NULL(function);

	siThread thread;
	thread.func = function;
	thread.arg = arg;
	thread.stackSize = stackSize;
	thread.state = siThreadState_Initialized;
	thread.returnValue = nil;

	return thread;
}

inline
siError si_threadMakeAndRun(siThreadFunction function, rawptr arg, siThread* out) {
	*out = si_threadMake(function, arg);
	return si_threadRun(out);
}

SIDEF
siError si_threadRun(siThread* thread) {
	SI_ASSERT_NOT_NULL(thread);
	SI_ASSERT(thread->state == siThreadState_Initialized);

	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		thread->id = CreateThread(nil, thread->stackSize, si__threadProc, thread, 0, nil);
		siErrorSystem_CHECK(thread->id == nil, &error, error);

	#else
		int res;
		pthread_attr_t attr;
		rawptr attrPtr = nil;
		if (t->stackSize != 0) {
			res = pthread_attr_init(&attr);
			SI_ASSERT(res == 0);

			pthread_attr_setstacksize(&attr, t->stackSize);
			res = pthread_create(&t->id, &attr, si__threadProc, t);
			if (res) {
				pthread_attr_destroy(&attr);
			}
		}
		else {
			res = pthread_create(&t->id, nil, si__threadProc, t);
		}
		t->isRunning = (res == 0);

	#endif

	thread->state = siThreadState_Running;
	return error;
}

SIDEF
siError si_threadJoin(siThread* thread) {
	SI_ASSERT_NOT_NULL(thread);
	SI_ASSERT_NOT_NULL((rawptr)thread->id);

	#if SI_SYSTEM_IS_WINDOWS
		DWORD res = WaitForSingleObject(thread->id, INFINITE);
	#else
		int res = pthread_join(thread->id, nil);
	#endif

	siError error = SI_ERROR_DEFAULT;
	siErrorSystem_CHECK(res != 0, &error, error);

	return error;
}

SIDEF
siError si_threadDestroy(siThread* thread) {
	SI_ASSERT_NOT_NULL(thread);
	SI_ASSERT(thread->state != siThreadState_Running);
	SI_STOPIF(thread->id == nil, return SI_ERROR_DEFAULT);

	siError error = SI_ERROR_DEFAULT;

#if SI_SYSTEM_IS_WINDOWS
	b32 res = CloseHandle(thread->id);
	siErrorSystem_CHECK(!res, &error, error);
#endif

	thread->id = nil;
	thread->state = siThreadState_Closed;

	return error;
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

	#elif SI_SYSTEM_IS_WINDOWS
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return count.QuadPart;

	#else
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000000 + tv.tv_usec;

	#endif
#else
	#if SI_SYSTEM_IS_WINDOWS
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return count.QuadPart;

	#else
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000000 + tv.tv_usec;

	#endif
#endif /* SI_NO_INLINE_ASM */
}

inline
u64 si_clock(void) {
	return si_RDTSC() / si_cpuClockSpeed() * 1000;
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
	u64 diff = (end - ts) / si_cpuClockSpeed() * 1000;

	const siBenchmarkLimit* time = si_benchmarkLimitLoop(diff);
	si_printf(
		"si_timeStampPrintSince: TIME: %.2f %s\n",
		(f32)diff / (f32)time->duration, time->unit
	);
}

inline
void si_sleep(u32 miliseconds) {
	SI_STOPIF(miliseconds == 0, return);

#if SI_SYSTEM_IS_WINDOWS
	Sleep((u32)miliseconds);

#else
	struct timespec ts = {
		(time_t)miliseconds / 1000,
		si_cast(time_t, miliseconds % 1000) * 1000000
	};
	nanosleep(&ts, &ts);

#endif
}


SIDEF
i64 si_timeNowUTC(void) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME time;
	GetSystemTimePreciseAsFileTime(&time);

	return si_timeToUnix((u64)time.dwHighDateTime << 32 | time.dwLowDateTime);

#else
    struct timespec spec;
    i32 res = clock_gettime(CLOCK_REALTIME, &spec);

	return (res == 0)
		? spec.tv_sec * 1000000000u + spec.tv_nsec
		: 0;

#endif
}

inline
i64 si_timeNowLocal(void) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME utc;
	GetSystemTimePreciseAsFileTime(&utc);
	FILETIME time;
	b32 res = FileTimeToLocalFileTime(&utc, &time);

	return (res)
		? si_timeToUnix((u64)time.dwHighDateTime << 32 | time.dwLowDateTime)
		: 0;

#else
	i64 timeNow = si_timeNowUTC();
	if (timezone == 0) {
		tzset();
	}

	return (timezone != 0)
		? timeNow - timezone * 1000000000 + daylight * 3600000000000
		: 0;

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

	{
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

	{
		static const i8 daysInMonths[2][12] = {
			{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
			{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
		};

		b32 leap = si_timeYearIsLeap(calendar.years);
		for (calendar.months = 0; calendar.months < (i32)countof(daysInMonths[0]); calendar.months += 1) {
    	    i32 days = daysInMonths[leap][calendar.months];
			SI_STOPIF(daysSinceEpoch < days, break);

    	    daysSinceEpoch -= days;
		}
		calendar.months += 1;
	}

	{
		i64 nanoseconds = time % 86400000000000;
		calendar.days = daysSinceEpoch + 1;

		calendar.hours = si_cast(i32, nanoseconds / (i64)3600000000000);
    	nanoseconds %= (i64)3.6e+12;

		calendar.minutes = si_cast(i32, nanoseconds / 60000000000);
    	nanoseconds %= 60000000000;

		calendar.seconds = si_cast(i32, nanoseconds / 1000000000);
    	nanoseconds %= 1000000000;

		calendar.nanoseconds = (i32)nanoseconds;
	}

	return calendar;
}

/* TODO(EimaMei): Refactor the code to be shorter and easier to understand. */
usize si_timeToString(u8* buffer, usize capacity, siTimeCalendar calendar, siString fmt) {
	SI_ASSERT_NOT_NULL(buffer);

	siAllocatorArena aData = si_arenaMakePtr(buffer, capacity, 1);
	siAllocator alloc = si_allocatorArena(&aData);

	b32 AMwasChecked = false;
	i32 ogHour = calendar.hours;

	for_eachStr (x, fmt) {
		SI_STOPIF(aData.offset >= aData.capacity, break);

		switch (*x) {
			case 'y': {
				u32 count = 1;
				while (x[count] == 'y' && count != 4) { count += 1; }
				x += count - 1;

				(count != 2)
					? si_stringFromUInt(calendar.years, alloc)
					: si_stringFromUInt(calendar.years % 100, alloc);
			} break;

			case 'M': {
				SI_ASSERT(si_between(i32, calendar.months, 0, 12));
				u32 count = 1;
				while (x[count] == 'M' && count != 4) { count += 1; }
				x += count - 1;

				switch (count) {
					case 1:
						si_stringFromUInt(calendar.months, alloc);
						break;

					case 2:
						if (calendar.months < 10) {
							aData.ptr[aData.offset] = '0';
							aData.offset += 1;
						}

						si_stringFromUInt(calendar.months, alloc);
						break;

					case 3: {
						siString* str = &SI_NAMES_MONTHS_SHRT[calendar.months - 1];
						si_memcopyStr(&aData.ptr[aData.offset], *str);
						aData.offset += str->len;
					} break;

					case 4: {
						siString* str = &SI_NAMES_MONTHS_FULL[calendar.months - 1];
						si_memcopyStr(&aData.ptr[aData.offset], *str);
						aData.offset += str->len;
					} break;
				}
			} break;

			case 'd': {
				u32 count = 1;
				while (x[count] == 'd' && count != 4) { count += 1; }
				x += count - 1;

				switch (count) {
					case 1:
						si_stringFromUInt(calendar.days, alloc);
						break;

					case 2:
						if (calendar.days < 10) {
							aData.ptr[aData.offset] = '0';
							aData.offset += 1;
						}

						si_stringFromUInt(calendar.days, alloc);
						break;

					case 3: {
						i32 wd = si_timeGetDayOfWeek(calendar.years, calendar.months, calendar.days);
						siString* str = &SI_NAMES_DAYS_SHRT[wd];
						si_memcopyStr_s(&aData.ptr[aData.offset], aData.capacity - aData.offset, *str);
						aData.offset += str->len;
					} break;

					case 4: {
						i32 wd = si_timeGetDayOfWeek(calendar.years, calendar.months, calendar.days);
						siString* str = &SI_NAMES_DAYS_FULL[wd];
						si_memcopyStr_s(&aData.ptr[aData.offset], aData.capacity - aData.offset, *str);
						aData.offset += str->len;
					} break;
				}
			} break;

			case 'h': {
				if (!AMwasChecked) {
					siString sub = si_stringSubToEnd(fmt, x - (u8*)fmt.data);
					for_eachRevStr (y, sub) {
						if (*y == 'p' || *y == 'P') {
							if (y != sub.data && (y[-1] == 'a' || y[-1] == 'A')) {
								if (calendar.hours != 12 && calendar.hours != 0) {
									calendar.hours %= 12;
								}
								else if (calendar.hours == 0) {
									calendar.hours = 12;
								}
								break;
							}
						}
					}
					AMwasChecked = true;
				}

				if (x != si_stringEnd(fmt) - 1 && x[1] == 'h') {
					x += 1;
					if (calendar.hours < 10) {
						aData.ptr[aData.offset] = '0';
						aData.offset += 1;
					}
				}

				si_stringFromUInt(calendar.hours, alloc);
			} break;

			case 'm': {
				if (x != si_stringEnd(fmt) - 1 && x[1] == 'm') {
					x += 1;
					if (calendar.minutes < 10) {
						aData.ptr[aData.offset] = '0';
						aData.offset += 1;
					}
				}

				si_stringFromUInt(calendar.minutes, alloc);
			} break;

			case 's': {
				if (x != si_stringEnd(fmt) - 1 && x[1] == 's') {
					x += 1;
					if (calendar.seconds < 10) {
						aData.ptr[aData.offset] = '0';
						aData.offset += 1;
					}
				}

				si_stringFromUInt(calendar.seconds, alloc);
			} break;

			case 'n': {
				if (x != si_stringEnd(fmt) - 1 && x[1] == 'n') {
					x += 1;
					if (calendar.nanoseconds < 10) {
						aData.ptr[aData.offset] = '0';
						aData.offset += 1;
					}
				}

				si_stringFromUInt(calendar.nanoseconds, alloc);
			} break;

			case 'a': {
				if (x != si_stringEnd(fmt) - 1 && x[1] == 'p') {
					x += 1;

					siString* str = &SI_NAMES_AM_PM[2 + (ogHour >= 12)];
					si_memcopyStr_s(&aData.ptr[aData.offset], aData.capacity - aData.offset, *str);
					aData.offset += str->len;

					if (!AMwasChecked) {
						if (calendar.hours != 12 && calendar.hours != 0) {
							calendar.hours %= 12;
						}
						else if (calendar.hours == 0) {
							calendar.hours = 12;
						}
						AMwasChecked = true;
					}
					break;
				}
				siFallthrough;
			}

			case 'A': {
				if (x != si_stringEnd(fmt) - 1 && x[1] == 'P') {
					x += 1;

					siString* str = &SI_NAMES_AM_PM[ogHour >= 12];
					si_memcopyStr_s(&aData.ptr[aData.offset], aData.capacity - aData.offset, *str);
					aData.offset += str->len;

					if (!AMwasChecked) {
						if (calendar.hours != 12 && calendar.hours != 0) {
							calendar.hours %= 12;
						}
						else if (calendar.hours == 0) {
							calendar.hours = 12;
						}
						AMwasChecked = true;
					}
					break;
				}
				siFallthrough;
			}


			default:
				aData.ptr[aData.offset] = *x;
				aData.offset += 1;
				break;
		}
	}

	return aData.offset;
}

#endif /* SI_IMPLEMENTATION_TIME */

#ifdef SI_IMPLEMENTATION_BIT

SIDEF
u32 si_numCountBitsU8(u8 num) {
	return si_cast(u32, ((u64)num * 01001001001L & 042104210421) % 017);
}


SIDEF
u32 si_numCountBitsU32(u32 num) {
	 num -= ((num >> 1) & 0x55555555);
	 num = (num & 0x33333333) + ((num >> 2) & 0x33333333);
	 num = (num + (num >> 4)) & 0x0F0F0F0F;
	 return si_cast(u32, (num * 0x01010101) >> 24);
}


SIDEF
u32 si_numCountBitsU64(u64 num) {
	num = num - ((num >> 1) & 0x5555555555555555);
	num = (num & 0x3333333333333333) + ((num >> 2) & 0x3333333333333333);
	num = (num + (num >> 4)) & 0xF0F0F0F0F0F0F0F;
	return si_cast(u32, (num * 0x101010101010101) >> 56);
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

	for_range (i, 0, totalBits) {
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
siArray(u8) si_numToBytesEx(u64 num, usize num_sizeof, siAllocator alloc) {
	siArray(u8) res = si_arrayMakeReserve(1, num_sizeof, num_sizeof, alloc);
	res.len = num_sizeof;

	for_eachRevArr (u8, byte, res) {
		*byte = num & 0xFF;
		num >>= 8;
	}

	return res;
}

SIDEF
u64 si_numFromBytes(siArray(u8) bytes) {
	SI_ASSERT_NOT_NULL(bytes.data);
	u64 res = 0;

	for_eachArr (u8, byte, bytes) {
		res <<= 8;
		res |= *byte;
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
u32 si_numLen(u64 num) {
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
u32 si_numLenEx(u64 num, u32 base) {
	u32 count = 0;
	do {
		count += 1;
		num /= base;
	} while (num != 0);

	return count;
}

inline
u32 si_numLenI64(i64 num) {
	return si_numLenI64Ex(num, 10);
}

inline
u32 si_numLenI64Ex(i64 num, u32 base) {
	u64 isNegative = (num < 0);
	u64 unsignedNum = ((u64)num ^ -isNegative) + isNegative;
	return si_numLenEx(unsignedNum, base) + (u32)isNegative;
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
	SI_ASSERT_NOT_NULL(res); \
	return a > *res; \
)
SI_CHECK_ARITHMETIC_IMPL_ALL_I(Add, SIDEF, a + b,
	SI_ASSERT_NOT_NULL(res); \
	if (a >= 0) { \
		return (max - a < b); \
	} \
	return (b < min - a); \
)

SI_CHECK_ARITHMETIC_IMPL_ALL_U(Sub, SIDEF, a - b,
	SI_ASSERT_NOT_NULL(res); \
	return a < *res; \
)
SI_CHECK_ARITHMETIC_IMPL_ALL_I(Sub, SIDEF, a - b,
	SI_ASSERT_NOT_NULL(res); \
	if (b < 0) { \
		return (max + b < a); \
	} \
	return (min + b > a); \
)

SI_CHECK_ARITHMETIC_IMPL_ALL_U(Mul, SIDEF, a * b,
	SI_ASSERT_NOT_NULL(res); \
	return (b > 0 && a > max / b);
)
SI_CHECK_ARITHMETIC_IMPL_ALL_I(Mul, SIDEF, a * b,
	SI_ASSERT_NOT_NULL(res); \
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
u32 si_cpuClockSpeed(void) {
	static u32 SI_CPU_FREQ_MHZ = UINT32_MAX;
	SI_STOPIF(SI_CPU_FREQ_MHZ != UINT32_MAX, return SI_CPU_FREQ_MHZ);

	u64 begin = si_RDTSC();
	si_sleep(100);
	u64 end = si_RDTSC();

	u32 val = si_cast(u32, end - begin) / 100000;
	SI_CPU_FREQ_MHZ = ((val + 5) / 10) * 10;

	return SI_CPU_FREQ_MHZ;
}

SIDEF
u32 si_cpuProcessorCount(void) {
	static u32 procCount = UINT32_MAX;
	SI_STOPIF(procCount != UINT32_MAX, return procCount);

#if SI_SYSTEM_IS_UNIX
	procCount = (u32)sysconf(_SC_NPROCESSORS_ONLN);

#elif SI_SYSTEM_IS_WINDOWS
	DWORD len;
	i32 res = GetLogicalProcessorInformation(nil, &len);
	SI_STOPIF(res != 0 || GetLastError() != 122 || len == 0, return 0);

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
	usize size = sizeof(i64);
	if (sysctlbyname("hw.logicalcpu", &count, &size, nil, 0) == 0 && count > 0) {
		procCount = (u32)count;
	}
	else  {
		procCount = 0;
	}

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
isize si_printf(cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_printfVa(fmt, va);
	va_end(va);

	return res;
}

inline
isize si_printfVa(cstring fmt, va_list va) {
	return si_fprintfVa(si_stdout, fmt, va);
}


inline
isize si_fprint(siFile* file, cstring str) {
	return si_fileWrite(file, SI_CSTR(str));
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
isize si_fprintfVa(siFile* file, cstring fmt, va_list va) {
	char buffer[SI_KILO(8)];
	isize count = si_bprintfVa(buffer, sizeof(buffer), fmt, va) - 1;

	return si_fileWrite(file, SI_STR_LEN(buffer, (usize)count));
}
SIDEF
isize si_bprintf(char* out, usize capacity, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_bprintfVa(out, capacity, fmt, va);
	va_end(va);

	return res;
}

struct si__printfInfoStruct {
	char* data;
	usize index;

	i32 padSize;
	u8 padLetter;

	siString str;

	usize capacity;
};

force_inline
void si__printStrToBuf(struct si__printfInfoStruct* info) {
	char* base = &info->data[info->index];

	usize len = (info->capacity > info->str.len)
		? info->str.len
		: info->capacity;

	if (info->padSize == 0) {
		si_memcopy(base, info->str.data, len);
	}
	else if (info->padSize < 0) {
		isize padLen = (isize)len + info->padSize;
		b32 padNeeded = (padLen < 0);

		si_memcopy(base, info->str.data, len);
		if (padNeeded) {
			usize padding = (usize)-padLen;
			si_memset(&base[len], info->padLetter, padding);
			info->index += padding;
			info->capacity -= padding;
		}
		info->padSize = 0;
	}
	else {
		isize padLen = info->padSize - (isize)len;
		b32 padNeeded = (padLen > 0);

		if (padNeeded) {
			usize padding = (usize)padLen;
			memset(base, info->padLetter, padding);
			info->index += padding;
			info->capacity -= padding;
		}
		si_memcopy(&info->data[info->index], info->str.data, len);
		info->padSize = 0;
	}

	info->index += len;
	info->capacity -= len;
}
force_inline
void si__printStrCpy(struct si__printfInfoStruct* info) {
	usize len = (info->capacity > info->str.len)
		? info->str.len
		: info->capacity;

	si_memcopy(&info->data[info->index], info->str.data, len);
	info->index += len;
}


#define SI_SET_FMT_PTR(x, ptr) \
	x = *ptr; ptr += 1

#define SI_CHECK_AFTERPOINT_INT(info, afterPointIsSet, afterPoint) \
	if (afterPointIsSet) { \
		SI_STOPIF(afterPoint == 0, break); \
		info.padLetter = '0'; \
		info.padSize = afterPoint; \
	} \
	do {} while (0)

SIDEF
isize si_bprintfVa(char* buffer, usize capacity, cstring fmt, va_list va) {
	SI_STOPIF(capacity == 0, return 0);

	union {
		i32 I32; u32 U32;
		u64 U64; i64 I64; f64 F64;
		usize USIZE; isize ISIZE;
		cstring STR; u8* PTR;
	} vaValue;

	struct si__printfInfoStruct info;
	info.data = buffer;
	info.index = 0;
	info.capacity = capacity;
	info.padSize = 0;

	siAllocatorArena aData = si_arenaMakePtr(si_stackAlloc(128), 1);
	siAllocator stack = si_allocatorArena(&aData);

	char x;
	cstring fmtPtr = fmt;
	i32 base = 10;
	b32 colorPresent = false;

	info.capacity -= 1; /* NOTE(EimaMei): To ensure we have enough space for the NULL-terminator. */
	while (info.index < info.capacity) {
		SI_SET_FMT_PTR(x, fmtPtr);
		SI_STOPIF(x == '\0', break);
		SI_STOPIF(x != '%', { buffer[info.index] = x; info.index += 1; continue; });

		SI_SET_FMT_PTR(x, fmtPtr);
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
				buffer[info.index] = '%';
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

				SI_SET_FMT_PTR(x, fmtPtr);
				goto GOTO_PRINT_SWITCH;
			}
			case 'l': {
				SI_STOPIF(typeSize != 0, continue);
				SI_STOPIF(*fmtPtr == 'l', fmtPtr += 1);
				SI_SET_FMT_PTR(x, fmtPtr);
				typeSize = 8;
				goto GOTO_PRINT_SWITCH;
			}
			case 't': case 'z': {
				SI_STOPIF(typeSize != 0, continue);
				SI_SET_FMT_PTR(x, fmtPtr);
				typeSize = sizeof(usize);
				goto GOTO_PRINT_SWITCH;
			}
			case 'j': {
				SI_STOPIF(typeSize != 0, continue);
				SI_SET_FMT_PTR(x, fmtPtr);
				typeSize = 8;
				goto GOTO_PRINT_SWITCH;
			}
			case '#': {
				cstring beforeAlt = fmtPtr;
				do {
					SI_SET_FMT_PTR(x, fmtPtr);
				}
				while (x != 'x' && x != 'b' && x != 'o' && x != 'X' && x != 'O');

				char altForm[2] = {'0', x | SI_BIT(5)};
#ifndef SI_NO_SILI_PRINTF_STYLE
				info.str = SI_STR_LEN(altForm, sizeof(altForm));
#else
				b32 isOctal = (x == 'o');
				info.str = SI_STR_LEN(altForm, sizeof(altForm) - isOctal);
#endif
				si__printStrCpy(&info);

				fmtPtr = beforeAlt + 1;
				x = *beforeAlt;

				goto GOTO_PRINT_SWITCH;
			}
			case '*': {
				*ptrToVar = va_arg(va, i32);

				SI_SET_FMT_PTR(x, fmtPtr);
				goto GOTO_PRINT_SWITCH;
			}
			case '.': {
				ptrToVar = &afterPoint;
				afterPointIsSet = true;

				SI_SET_FMT_PTR(x, fmtPtr);
				goto GOTO_PRINT_SWITCH;
			}
			case '-': case '+': {
				if (si_charIsDigit(*fmtPtr)) {
					goto GOTO_PADCALC_SWITCH;
				}
				siFallthrough; /* NOTE(EimaMei): Go to the ' ' case. */
			}
			case ' ': {
				info.str = SI_STR_LEN(&x, sizeof(char));
				si__printStrCpy(&info);

				SI_SET_FMT_PTR(x, fmtPtr);
				goto GOTO_PRINT_SWITCH;
			}
			case '0': {
				SI_SET_FMT_PTR(x, fmtPtr);
				if (si_charIsDigit(x)) {
					info.padLetter = '0';
				}
				else {
					*ptrToVar = 0;
				}

				goto GOTO_PRINT_SWITCH;
			}
			case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			case '8': case '9': {
				cstring start;
GOTO_PADCALC_SWITCH:
				start = fmtPtr - 1;

				while (si_charIsDigit(*fmtPtr)) {
					fmtPtr += 1;
				}

				usize len = fmtPtr - start;
				i64 count = si_stringToInt(SI_STR_LEN(start, len));
				SI_ASSERT(count <= INT32_MAX);

				*ptrToVar = (i32)count;

				SI_SET_FMT_PTR(x, fmtPtr);
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
				break;
			}
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

				if (typeSize == 0) {
					info.str = SI_STR_LEN(&vaValue.I32, 1);
				}
				else {
					siUtf8Char utf8 = si_utf8Encode(vaValue.I32);
					info.str = SI_STR_LEN(utf8.codepoint, utf8.len);
				}
				si__printStrToBuf(&info);

			} break;

			case 's': {
				vaValue.STR = va_arg(va, char*);

				if (vaValue.STR == nil) {
					info.str = SI_STR("(nil)");
				}
				else {
					usize len = afterPointIsSet
						? (usize)afterPoint
						: si_cstrLen(vaValue.STR);
					info.str = SI_STR_LEN(vaValue.STR, len);
				}

				si__printStrToBuf(&info);
			} break;


			case 'A': case 'a': {
				vaValue.F64 = va_arg(va, f64);

				char altForm[2] = {'0', si_cast(char, x + ('X' - 'A'))};
				info.str = SI_STR_LEN(altForm, sizeof(altForm));
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
					info.str = si_stringFromFloatEx(vaValue.F64, 10, afterPoint, stack);
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


				char remainder[4];
				remainder[0] = x;
				remainder[1] = '+';
				if (exponent < 10) {
					remainder[2] = '0';
					remainder[3] = (char)(exponent + '0');
				}
				else {
					remainder[2] = (char)((exponent / 10) + '0');
					remainder[3] = (char)((exponent % 10) + '0');
				}

				info.str = SI_STR_LEN(remainder, sizeof(remainder));
				si__printStrCpy(&info);
			} break;

			case 'G': case 'g': {
				vaValue.F64  = va_arg(va, f64);

				if (vaValue.F64 < 0.0001) {
					x = (char)('E' + (x - 'G'));
					goto GOTO_SCIENTIFIC_NOTATION;
				}

				u64 pow10Val = si_pow10(afterPoint);
				u64 intFloat = (u64)vaValue.F64;

				if (vaValue.F64 > (f64)pow10Val) {
					afterPoint -= si_cast(u32, intFloat / pow10Val);

					x = (char)('E' + (x - 'G'));
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

				info.str = si_stringFromFloatEx(vaValue.F64, 10, (u32)newAfterPoint, stack);
				si__printStrToBuf(&info);
			} break;

			case 'p': {
				info.str = SI_STR("0x");
				si__printStrCpy(&info);

				vaValue.USIZE = (usize)va_arg(va, rawptr);
				info.str = si_stringFromUIntEx(vaValue.USIZE, 16, stack);
				si__printStrCpy(&info);
			} break;

#ifndef SI_NO_SILI_PRINTF_STYLE

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
						char str[32] = "\33[38;5;";

						siAllocatorArena tmp = si_arenaMakePtr(str, countof(str), 1);
						tmp.offset = countof_str("\33[38;5;");

						si_stringFromInt(clr.data.cube, si_allocatorArena(&tmp));
						str[tmp.offset] = 'm';

						info.str = SI_STR_LEN(str, tmp.offset + 1);
						si__printStrCpy(&info);
					} break;

					case siPrintColorType_24bit: {
						char str[64] ="\33[38;2;";

						siAllocatorArena tmp = si_arenaMakePtr(str, countof(str), 1);
						tmp.offset = countof_str("\33[38;2;");
						siAllocator tmpA = si_allocatorArena(&tmp);

						static char buf[countof(clr.data.rgb)] = {';', ';', 'm'};
						for_range (j, 0, countof(clr.data.rgb)) {
							 si_stringFromInt(clr.data.rgb[j], tmpA);

							str[tmp.offset] = buf[j];
							tmp.offset += 1;
						}

						info.str = SI_STR_LEN(str, tmp.offset);
						si__printStrCpy(&info);
					} break;

					default: SI_PANIC();
				}
			} break;
#endif
			default: {
				char str[2] = {'%', x};
				info.str = SI_STR_LEN(str, sizeof(str));
				si__printStrCpy(&info);
				break;
			}
		}

		 si_freeAll(stack);
	}
	buffer[info.index] = '\0';
	info.index += 1;

	return (isize)info.index;
}

#undef SI_CHECK_AFTERPOINT_INT
#undef SI_SET_FMT_PTR


inline
isize si_bprintfLn(char* out, usize capacity, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_bprintfLnVa(out, capacity, fmt, va);
	va_end(va);

	return res;
}
SIDEF
isize si_bprintfLnVa(char* out, usize capacity, cstring fmt, va_list va) {
	SI_STOPIF(capacity == 0, return 0);
	isize len = si_bprintfVa(out, capacity - 1, fmt, va);
	out[len] = '\n';
	return len + 1;
}


inline
b32 si_printHas24bitColor(void) {
	u8 buf[16];
	siString colorterm = si_envVarGet(SI_STR("COLORTERM"), buf, sizeof(buf));

	return si_stringEqual(colorterm, SI_STR("truecolor"));
}


#endif /* SI_IMPLEMENTATION_PRINT */

#ifdef SI_IMPLEMENTATION_DLL

inline
siDllHandle si_dllLoad(cstring path) {
	SI_ASSERT_NOT_NULL(path);
#if SI_SYSTEM_IS_WINDOWS
	return (siDllHandle)LoadLibraryA(path);
#else
	return (siDllHandle)dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
#endif
}

inline
void si_dllUnload(siDllHandle dll) {
	SI_ASSERT_NOT_NULL(dll);
#if SI_SYSTEM_IS_WINDOWS
	FreeLibrary((HMODULE)dll);
#else
	dlclose(dll);
#endif
}

inline
siDllProc si_dllProcAddress(siDllHandle dll, cstring name) {
	SI_ASSERT_NOT_NULL(dll);
	SI_ASSERT_NOT_NULL(name);

#if SI_SYSTEM_IS_WINDOWS
	PROC proc = GetProcAddress((HMODULE)dll, name);
	return si_transmute(siDllProc, *proc, PROC);

#else
	return (siDllProc)dlsym(dll, name);

#endif
}
#endif /* SI_IMPLEMENTATION_DLL */

#ifdef SI_IMPLEMENTATION_MATH

SI_MATH_FUNC_DECLARE_2X       (min,     inline, { return (a < b) ? a : b;  })
SI_MATH_FUNC_DECLARE_2X       (max,     inline, { return (a > b) ? a : b;  })
SI_MATH_FUNC_DECLARE_1X_SIGNED(abs,     inline, { return a < 0 ? -a : a;   })
SI_MATH_FUNC_DECLARE_3X_B32   (between, inline, { return b <= a && a <= c; })

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
u64 si_pow10(u32 exponent) {
	static const u64 allPossibleValues[] = {
		1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
		10000000000ULL, 100000000000ULL, 1000000000000ULL, 10000000000000ULL,
		100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL
	};
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
u32 si_float64IsInf(f64 num) {
	union { f64 f; u64 n; } check;
	check.f = num;

	switch (check.n) {
		case 0x7FF0000000000000: return 1;
		case 0xFFF0000000000000: return 2;
	}

	return 0;
}

inline
b32 si_float64IsNan(f64 num) {
	static const u64 nanV = 0x7FF8000000000000;

	union { f64 f; u64 n; } check;
	check.f = num;
	check.n &= ~SI_ENDIAN_VALUE(SI_BIT(63), SI_BIT(0)); /* NOTE(EimaMei): A quick way of changing the minus to plus. */

	return check.n == nanV;
}

#endif /* SI_IMPLEMENTATION_MATH */

#ifdef SI_IMPLEMENTATION_BENCHMARK

SIDEF
void si_benchmarkLoopsAvgPrint(siBenchmarkInfo info, usize range[2]) {
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction - '%s'\n"
		"Runs:\n",
		info.name
	);

	const siBenchmarkLimit* element = nil;
	u64* array = (u64*)info.cycles.data;
	f64 freq = (f64)si_cpuClockSpeed() / 1000.0;

	isize pad_runs = si_numLen(range[1]);
	isize pad_cycles = si_numLen(array[info.cycles.len - 1]);

	usize arrayI = 0, runs;
	for (runs = range[0]; runs <= range[1]; runs *= 10) {
		u64 cycles = array[arrayI];
		f64 time = (f64)cycles / freq;

		element = si_benchmarkLimitLoop((u64)time);
		time /= (f64)element->duration;
		si_printf(
			"\t%*zu %s - %C%9.4f%C %s (%lu cycles)\n",
			pad_runs, runs, (runs != 1) ? "runs" : "run ",
			si_printColor3bit(siPrintColor3bit_Green), time, element->unit, cycles
		);

		arrayI += 1;
	}

	u64 cycles_total = 0;
	for_range (i, 0, info.cycles.len) {
		cycles_total += array[i];
	}

	f64 cycles_median = (f64)cycles_total / (f64)info.cycles.len;
	f64 time = cycles_median / freq;
	pad_cycles = si_numLen((u64)cycles_median);

	element = si_benchmarkLimitLoop((u64)time);
	time /= (f64)element->duration;

	si_printf(
		"Final result:\n"
			"\tTime average   - %C%*.4f%C %s\n"
			"\tCycles average - %*lu cycles\n",
		si_printColor3bit(siPrintColor3bit_Green),
		pad_cycles, time, element->unit, pad_cycles, (u64)cycles_median
	);
}

SIDEF
void si_benchmarkLoopsAvgCmpPrint(siBenchmarkInfo info[2], usize range[2]) {
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction 1 - '%s'\n"
			"\tFunction 2 - '%s'\n"
		"Runs:\n",
		info[0].name, info[1].name
	);
	const siBenchmarkLimit* elements[2];
	u64* arrays[2] = {(u64*)info[0].cycles.data, (u64*)info[1].cycles.data};
	f64 freq = (f64)si_cpuClockSpeed() / 1000.0;

	isize pad_runs = si_numLen(range[1]);
	isize pad_cycles[2] = {
		si_numLen(arrays[0][info->cycles.len - 1]),
		si_numLen(arrays[1][info->cycles.len - 1])
	};

	usize arrayI = 0, runs;
	for (runs = range[0]; runs <= range[1]; runs *= 10) {
		u64 cycles[] = {arrays[0][arrayI], arrays[1][arrayI]};
		f64 time[] = {(f64)cycles[0] / freq, (f64)cycles[1] / freq};

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
			pad_runs, runs, (runs != 1) ? "runs" : "run ",
			clr[0], time[0], elements[0]->unit, clr[1], time[1], elements[1]->unit,
			ratio, pad_cycles[0], cycles[0], pad_cycles[1], cycles[1]
		);

		arrayI += 1;
	}

	u64 cycles_total[2] = {0, 0};
	for_range (i, 0, info->cycles.len) {
		for_range (j, 0, countof(cycles_total)) {
			cycles_total[j] += arrays[j][i];
		}
	}

	f64 cycles_median[2] = {
		(f64)cycles_total[0] / (f64)info->cycles.len,
		(f64)cycles_total[1] / (f64)info->cycles.len
	};
	f64 time[] = {cycles_median[0] / freq, cycles_median[1] / freq};

	for_range (i, 0, countof(pad_cycles)) {
		pad_cycles[i] = si_numLen((u64)cycles_median[i]);
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
		clr[0], pad_cycles[0], time[0], elements[0]->unit,
		clr[1], pad_cycles[1], time[1], elements[1]->unit,
		ratio,
		pad_cycles[0], (u64)cycles_median[0], pad_cycles[1], (u64)cycles_median[1]
	);
}

rawptr si__BenchmarkThread(rawptr arg) {
	si_sleep(SI_TO_U32(arg));
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
	#undef SI_ASSERT_NULL

	/* condition - EXPRESSION
	 * Crashes the app if the condition is not met. */
	#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
	/* condition - EXPRESSION | message - cstring
	 * Crashes the app with a message if the condition is not met. */
	#define SI_ASSERT_MSG(condition, message) SI_ASSERT_FMT(condition, message, "")
	/* condition - EXPRESSION | message - cstring | ...fmt - VARIADIC
	 * Crashes the app with a formatted message if the condition is not met. */
	#define SI_ASSERT_FMT(condition, message, ... /* fmt */) \
		SI_STOPIF(!(condition), si_panic(#condition, __FILE__, __LINE__, __func__, message, __VA_ARGS__))
	/* ptr - rawptr
	 * Crashes the app if a pointer is NULL. */
	#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL.")

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
