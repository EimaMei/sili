/*
sili.h - v0.1.0 - a cross-platform standard library for modern C99 programming
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

	- SI_NO_ASSERTIONS_IN_HEADER - 'SI_ASSERT()' and its other variations do not
	check for the assertion inside of the 'sili.h' file, meaning any given expression
	from sili functions will evaluate to nothing and not crash the app if the
	assertion equals false. May increase the performance of the program, but should
	only be enabled for release builds, as any errors in the code will then not get
	caught and will most likely crash the app somewhere else further down.

	- SI_NO_ASSERTIONS - same as 'SI_NO_ASSERTIONS' except all of the assertion
	functions continue to be unusable outside of 'sili.h', disabling the functionality
	entirely.


	- SI_NO_ALLOC_DEBUG_INFO - 'si_malloc()' and similar functions take
	'__FILE__' and '__LINE__' arguments under the hood for debugging. For performance
	reasons, you can disable this.

	- SI_RELEASE_MODE - same as defining both 'SI_NO_ASSERTIONS' and 'SI_NO_ALLOC_DEBUG_INFO'.

	- SI_NO_ERROR_STRUCT - by default 'siErrorInfo' that contains an i32 error code,
	u64 time in UTC+0 and a cstring of the last function that edited the structure.
	Defining this macro makes 'siErrorInfo' only contain an error code.

	- SI_NO_INLINE_ASM - disables any usage of inline ASM in "sili.h".

	- SI_ASM_USE_ATT_SYNTAX - makes 'si_asm()' understand AT&T syntax, as by default
	Intel syntax is used.

	- SI_NO_WINDOWS_H - disables the inclusion of <windows.h> inside "sili.h."
	Will cause 'undefined' compiler errors in some parts of the code without a
	proper replacement.

	- SI_NO_TYPEOF - disables the usage of '__typeof__' inside of the library.
	NOTE: Might break certain features in the library.

	- SI_DEFINE_CUSTOM_HASH_FUNCTION - undefines the base implementation of
	'si__hashKey' inside this file, creating the option of defining a custom
	implementation.

	- SI_NO_HASH_OVERWRITE - disables the ovewrite of a pre-existing entry's
	value when using 'si_hashtableSet/WithHash'.

	- SI_NO_SILI_PRINTF_STYLE - disables sili's added features for the 'si_printf'
	types of functions, which may break some output compatibility with the old
	'printf' (albeit most features use non-printf standard specifiers). The list
	of said features:
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
	- Sili is designed to be as a fast, modern, but also experimental STL alternative,
	and because of it some unwarranted results may occur or be present when using
	the library, those being:
		1) Features may not work as expected
		2) Functions may not be documented or only contain incomplete documentation
		3) API breaking changes between releases.
		4) Little to no security checks for malicious code attempting to explode
		sections of the code.

*/

#if (_MSC_VER >= 1020) || ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
	#pragma once
#endif

#ifndef SI_INCLUDE_SI_H
#define SI_INCLUDE_SI_H

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef SI_VERSION_MAJOR
	#define SI_VERSION_MAJOR 0
	#define SI_VERSION_MINOR 1
	#define SI_VERSION_PATCH 0
#endif
#define SI_VERSION_CURRENT SI_VERSION(SI_VERSION_MAJOR, SI_VERSION_MINOR, SI_VERSION_PATCH)

/* major - UINT | minor - UINT | patch - UINT
 * Combines major, minor, and patch version numbers into a single integer. */
#define SI_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)

#ifndef SI_EXTERN
	#if defined(__cplusplus)
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

#elif defined(__clang__)
	#define SI_COMPILER_CLANG 1
	#define SI_COMPILER_STR "Clang"
	#define SI_COMPILER_TYPEOF_SUPPORTS 1

	#define SI_COMPILER_VERSION	SI_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)

#elif defined(_MSC_VER)
	#define SI_COMPILER_MSVC 1
	#define SI_COMPILER_STR "MSVC"

	/* NOTE(EimaMei): what the actual hell, Microsoft?????????????????? */
	#if _MSC_VER == 800
		#define SSI_COMPILER_VERSION SI_VERSION(1, 0, 0)
	#elif _MSC_VER == 900
		#define SSI_COMPILER_VERSION SI_VERSION(3, 0, 0)

	/* 4.0 to 12.0 */
	#elif _MSC_VER < 1800
		#define SI_COMPILER_VERSION SI_VERSION(_MSC_VER - 1000) / 100 + 4, (_MSC_VER % 200) / 10)

	#elif _MSC_VER < 1910
		#define SI_COMPILER_VERSION SI_VERSION(14, 0, 0)
	#elif _MSC_VER == 1910
		#define SI_COMPILER_VERSION SI_VERSION(15, 0, 0)
	#elif _MSC_VER == 1911
		#define SI_COMPILER_VERSION SI_VERSION(15, 3, 0)

	/* 15.5 to 15.9 */
	#elif _MSC_VER < 1920
		#define SI_COMPILER_VERSION SI_VERSION(15, 5 + (_MSC_VER - 1912), 0)

	/* 16.0 to 16.7 */
	#elif _MSC_VER < 1928
		#define SI_COMPILER_VERSION SI_VERSION(16, _MSC_VER - 1928, 0)
	/* 16.8 */
	#elif _MSC_FULL_VER == 192829333
		#define SI_COMPILER_VERSION SI_VERSION(16, 8, 0)

	/* 16.9 */
	#elif _MSC_FULL_VER == 192829919
		#define SI_COMPILER_VERSION SI_VERSION(16, 9, 0)

	#elif _MSC_FULL_VER == 192930040
		#define SI_COMPILER_VERSION SI_VERSION(16, 10, 0)
	#elif _MSC_FULL_VER == 192930133
		#define SI_COMPILER_VERSION SI_VERSION(16, 11, 0)

	/* 17.0 to 17.2 */
	#elif _MSC_VER <= 1930
		#define SI_COMPILER_VERSION SI_VERSION(17, _MSC_VER - 1930, 0)
	#endif

#else
	#define SI_COMPILER_UNKNOWN 1
	#define SI_COMPILER_STR "Unknown"

	#define SI_COMPILER_VERSION SI_VERSION(0, 0, 0)

#endif


#if defined(__cplusplus)
	#if defined(__OBJC__)
		#define SI_LANGUAGE_OBJ_CPP 1
		#define SI_LANGUAGE_STR "Objective-C++"
	#else
		#define SI_LANGUAGE_CPP 1
		#define SI_LANGUAGE_STR "C++"
	#endif

	#define SI_LANGUAGE_IS_CPP 1
#else
	#if defined(__OBJC__)
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

#if defined(__STDC__)
	#if SI_LANGUAGE_IS_C
		#if defined(__STDC_VERSION__)
			#define SI_STANDARD_VERSION __STDC_VERSION__
		#else
			#define SI_STANDARD_VERSION 198900L
		#endif

		#define SI_STANDARD_C89 198900L
		#define SI_STANDARD_C94 199409L
		#define SI_STANDARD_C99 199901l
		#define SI_STANDARD_C11 201112L
		#define SI_STANDARD_C17 201710L
		#define SI_STANDARD_C23 202300L
	#else
		#define SI_STANDARD_VERSION __cplusplus

		#define SI_STANDARD_CPP98 199711L
		#define SI_STANDARD_CPP11 201103L
		#define SI_STANDARD_CPP14 201402L
		#define SI_STANDARD_CPP17 201703L
		#define SI_STANDARD_CPP20 202002L
		#define SI_STANDARD_CPP23 202302L
	#endif
#endif

#if defined(__i386__) || defined(__i386) || defined(i386) || defined(__IA32__) || defined(__X86__) || defined(_M_I386) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || defined(__386)
	#define SI_ARCH_I386 1
	#define SI_ARCH_STR "i386"

	#define SI_ARCH_IS_X86 1
	#define SI_ARCH_IS_32BIT 1

#elif defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || defined(__x86_64)
	#define SI_ARCH_AMD64 1
	#define SI_ARCH_STR "amd64"

	#define SI_ARCH_IS_X86 1
	#define SI_ARCH_IS_64BIT 1

#elif defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
	#define SI_ARCH_PPC64 1
	#define SI_ARCH_STR "PowerPC64"

	#define SI_ARCH_IS_PPC 1
	#define SI_ARCH_IS_64BIT 1

#elif defined(__powerpc__) || defined(__powerpc) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_M_PPC) || defined(_ARCH_PPC) || defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || defined(_XENON) || defined(__ppc)
	#define SI_ARCH_PPC32 1
	#define SI_ARCH_STR "PowerPC"

	#define SI_ARCH_IS_PPC 1
	#define SI_ARCH_IS_32BIT 1

#elif defined(__aarch64__) || defined(_M_ARM64)
	#define SI_ARCH_ARM64 1
	#define SI_ARCH_STR "ARM64"

	#define SI_ARCH_IS_ARM 1
	#define SI_ARCH_IS_64BIT 1

#elif defined(__arm__) || defined(__thumb__) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) || defined(__arm)
	#define SI_ARCH_ARM32 1
	#define SI_ARCH_STR "ARM32"

	#define SI_ARCH_IS_ARM 1
	#define SI_ARCH_IS_32BIT 1

#elif defined(__riscv) && __riscv_xlen == 32
	#define SI_ARCH_RISC_V32 1
	#define SI_ARCH_STR "RISC-V 32-bit"

	#define SI_ARCH_IS_RISC 1
	#define SI_ARCH_IS_32BIT 1

#elif defined(__riscv) && __riscv_xlen = 64
	#define SI_ARCH_RISC_V64 1
	#define SI_ARCH_STR "RISC-V 64-bit"

	#define SI_ARCH_IS_RISC 1
	#define SI_ARCH_IS_64BIT 1

#else
	#define SI_ARCH_UNKNOWN 1
	#define SI_ARCH "Unknown"
#endif

/* Support for non little and big endian environments is not officially supported. */
#if 'ABCD' == 0x41424344UL
	#define SI_ENDIAN_IS_LITTLE 1
	#define SI_ENDIAN_STR "Little-endian"

#elif 'ABCD' == 0x44434241UL
	#define SI_ENDIAN_IS_BIG 1
	#define SI_ENDIAN_STR "Big-endian"

#elif 'ABCD' == 0x42414443UL
	#define SI_ENDIAN_IS_PDP 1
	#define SI_ENDIAN_STR "PDP-endian"

#else
	#define SI_ENDIAN_UNKNOWN 1
	#define SI_ENDIAN_STR "Unknown"

#endif

#if !defined(SI_ARCH_IS_64BIT) && !defined(SI_ARCH_IS_32BIT)
	#include <stdint.h>

	#if UINTPTR_MAX == 0xFFFFFFFF
		#define SI_ARCH_IS_32BIT 1
	#elif UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF
		#define SI_ARCH_IS_64BIT 1
	#else
		/* Support for non 64/32-bit environments is not officially supported. */
	#endif
#endif


#ifndef SI_STATIC_ASSERT_MSG
	#if SI_LANGUAGE_IS_C && SI_STANDARD_VERSION >= SI_STANDARD_C11
		#define SI_STATIC_ASSERT_MSG(condition, msg) _Static_assert(condition, msg)
	#elif SI_LANGUAGE_IS_CPP && SI_STANDARD_VERSION >= SI_STANDARD_CPP11
		#define SI_STATIC_ASSERT_MSG(condition, msg) static_assert(condition, msg)
	#else
		#define SI_STATIC_ASSERT_MSG(condition, msg) \
			extern int (*__si_error_if_negative (void)) \
			[(condition) ? 2 : -1]
	#endif
#endif
#define SI_STATIC_ASSERT(condition) SI_STATIC_ASSERT_MSG(condition, "")


#if SI_SYSTEM_IS_WINDOWS
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
#endif

#if SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	#if !defined(_GNU_SOURCE)
		#define _GNU_SOURCE
	#endif
	#if !defined(_LARGEFILE64_SOURCE)
		#define _LARGEFILE64_SOURCE
	#endif
	#if !defined(__USE_POSIX199506)
		#define __USE_POSIX199506 1
	#endif
#endif

#if !SI_SYSTEM_IS_WINDOWS
	#include <time.h>
	#include <stdlib.h>
	#include <memory.h>
	#include <stdarg.h>
	#include <stddef.h>
	#include <errno.h>
	#include <pthread.h>
	#include <unistd.h>
	#include <dirent.h>
	#include <dlfcn.h>
	#include <ftw.h>

	#include <sys/stat.h>
	#include <sys/fcntl.h>
	#include <sys/time.h>

	#include <stdio.h>
#else
	#if !defined(SI_NO_WINDOWS_H)
		#define NOMINMAX            1
		#define WIN32_LEAN_AND_MEAN 1
		#define WIN32_MEAN_AND_LEAN 1
		#define VC_EXTRALEAN        1
		#include <windows.h>
		#include <aclapi.h>
		#undef NOMINMAX
		#undef WIN32_LEAN_AND_MEAN
		#undef WIN32_MEAN_AND_LEAN
		#undef VC_EXTRALEAN
	#endif

	#include <malloc.h>
	#include <intrin.h>
	#include <time.h>
#endif

#if SI_SYSTEM_IS_APPLE
	#include <sys/socket.h>
#endif

#if SI_SYSTEM_IS_UNIX
	#include <sys/sendfile.h>
#endif


#if defined(SI_RELEASE_MODE)
	#undef SI_NO_ASSERTIONS
	#undef SI_NO_ALLOC_DEBUG_INFO

	#define SI_NO_ASSERTIONS
	#define SI_NO_ALLOC_DEBUG_INFO
#endif

#if !defined(SI_NO_TYPE_DEFS)

#if SI_SYSTEM_IS_WINDOWS && SI_COMPILER_MSVC
	#if SI_COMPILER_VERSION < SI_COMPILER(7, 0, 0)
		typedef unsigned char     u8;
		typedef   signed char     i8;
		typedef unsigned short   u16;
		typedef   signed short   i16;
		typedef unsigned int     u32;
		typedef   signed int     i32;
	#else
		typedef unsigned __int8   u8;
		typedef   signed __int8   i8;
		typedef unsigned __int16 u16;
		typedef   signed __int16 i16;
		typedef unsigned __int32 u32;
		typedef   signed __int32 i32;
	#endif

	typedef unsigned __int64 u64;
	typedef   signed __int64 i64;

	#define INT8_MIN     ((i8)_I8_MIN)
	#define INT8_MAX     _I8_MAX
	#define INT16_MIN    ((i16)_I16_MIN)
	#define INT16_MAX    _I16_MAX
	#define INT32_MIN    ((i32)_I32_MIN)
	#define INT32_MAX    _I32_MAX
	#define INT64_MIN    ((i64)_I64_MIN)
	#define INT64_MAX    _I64_MAX
	#define UINT8_MAX    _UI8_MAX
	#define UINT16_MAX   _UI16_MAX
	#define UINT32_MAX   _UI32_MAX
	#define UINT64_MAX   _UI64_MAX
#else
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
#endif

#if !defined(b8)
	typedef u8   b8; /* NOTE(EimaMei): Don't use this as the primary boolean type. */
	typedef u16 b16;
	typedef u32 b32; /* NOTE(EimaMei): Use this as the default! */
	typedef u64 b64;
	typedef u8 siByte;
#endif

typedef size_t      usize;
typedef ptrdiff_t   isize;
typedef usize  uintptr;
typedef isize  intptr;

typedef float  f32;
typedef double f64;

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
SI_STATIC_ASSERT(sizeof(usize) == sizeof(size_t));


SI_STATIC_ASSERT(sizeof(f32) == 4);
SI_STATIC_ASSERT(sizeof(f64) == 8);

#if SI_ARCH_IS_64BIT
	#ifndef USIZE_MAX
		#define USIZE_MAX UINT64_MAX
	#endif
	#ifndef USIZE_MIN
		#define USIZE_MIN 0
	#endif
	#ifndef ISIZE_MAX
		#define ISIZE_MAX INT64_MAX
	#endif
	#ifndef ISIZE_MIN
		#define ISIZE_MIN INT64_MIN
	#endif
#else
	#ifndef USIZE_MAX
		#define USIZE_MAX UINT32_MAX
	#endif
	#ifndef USIZE_MIN
		#define USIZE_MIN 0
	#endif
	#ifndef ISIZE_MAX
		#define ISIZE_MAX INT32_MAX
	#endif
	#ifndef ISIZE_MIN
		#define ISIZE_MIN INT32_MIN
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



#if !SI_LANGUAGE_IS_CPP && SI_STANDARD_VERSION <= SI_STANDARD_C17
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


#if SI_LANGUAGE_IS_C
	#if SI_COMPILER_MSVC && SI_COMPILER_VERSION <= SI_VERSION(11, 0, 0)
		#define inline __inline
	#elif !defined(__STDC_VERSION__)
		#define inline __inline__
	#endif
#endif

#ifndef force_inline
	#if SI_COMPILER_MSVC
		#define force_inline __forceinline
	#elif SI_COMPILER_GCC || SI_COMPILER_CLANG
		#define force_inline static inline __attribute__((always_inline))
	#else
		#define force_inline static inline
	#endif
#endif

#ifndef siIntern
	#define siIntern static
#endif

#ifndef siNullable
	/* Denotes if the pointer can be 'nil'. Function arguments that are pointers
	 * without this notifier get assert checked if they're null inside sili.h. */
	#define siNullable
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



#if (SI_LANGUAGE_IS_CPP && SI_STANDARD_VERSION >= SI_STANDARD_CPP17) || (SI_LANGUAGE_IS_C && SI_STANDARD_VERSION > SI_STANDARD_C17)
	/* Specifies a fallthrough for the compiler. */
	#define siFallthrough [[fallthrough]]
#elif SI_COMPILER_GCC || SI_COMPILER_CLANG
	/* Specifies a fallthrough for the compiler. */
	#define siFallthrough __attribute__((fallthrough))
#else
	/* Specifies a fallthrough for the compiler. */
	#define siFallthrough do {} while (0)
#endif


#if (SI_LANGUAGE_IS_C && SI_STANDARD_VERSION < SI_STANDARD_C99) || SI_LANGUAGE_IS_CPP
	#if SI_COMPILER_CLANG || (SI_COMPILER_GCC && SI_COMPILER_VERSION >= SI_VERSION(3, 1, 0))
		#define restrict __restrict
	#elif SI_COMPILER_MSVC && SI_COMPILER_VERSION >= SI_VERSION(8, 0, 0)
		#define restrict __restrict
	#else
		#ifndef restrict
			#define restrict
		#endif
	#endif
#endif


#if !defined(rawptr)
	/* A pointer type pointing to raw data. */
	typedef void* rawptr;
#endif

#if !defined(cstring)
	/* A const NULL-terminated C-string type. */
	typedef const char* cstring;
#endif

	/* A struct containing information about an error that happened during an operation. */
typedef struct siErrorInfo {
	/* Error code from the operation. */
	i32 code;

#if !defined(SI_NO_ERROR_STRUCT)
	/* The function where the error occurred. */
	cstring function;
	/* The time when the error happened (in UTC+0). */
	i64 time;
#endif
} siErrorInfo;

/*
	========================
	| Constants            |
	========================
*/

/* x - INT.
 * Converts kilobytes into bytes. */
#define SI_KILO(x) ((usize)(x) * 1024)
/* x - INT.
 * Converts megabytes into bytes. */
#define SI_MEGA(x) (SI_KILO(x) * 1024)
/* x - INT.
 * Converts gigabytes into bytes. */
#define SI_GIGA(x) (SI_MEGA(x) * 1024)
/* x - INT.
 * Converts terabytes into bytes. */
#define SI_TERA(x) (SI_GIGA(x) * 1024)

/* x - UINT
 * Shortcut for (1 << x). */
#define SI_BIT(x) ((usize)1 << si_cast(usize, x))

#if !defined(nil)
	/* A nothing value, equivalent to NULL. */
	#define nil ((void*)0)
#endif

SI_STATIC_ASSERT(sizeof(nil) == sizeof(void*));

/*
	========================
	| Declaration macros   |
	========================
*/

/* type - TYPE | name - NAME
 * Defines the enum with the given integer type. */
#define SI_ENUM(type, name) typedef type name; enum name
/* varName - NAME | multipleArgs - (TYPE1, TYPE2...)
 * Macro to define a function pointer. */
#define SI_FUNC_PTR(varName, multipleArgs) (*varName)multipleArgs
/* ...x - ANYTHING
* Silences the 'unused identifier' warning for the specified expression. */
#define SI_UNUSED(.../* x */) (void)(__VA_ARGS__)
/* ...x - ANYTHING
* Discards the function's return and silences the 'unused return value' warning. */
#define SI_DISCARD(.../* x */) (void)!(__VA_ARGS__)

/*
	========================
	| Casting              |
	========================
*/

/* type - TYPE | value - EXPRESSION
 * Casts a value to the type provided. */
#define si_cast(type, value) ((type)((value)))
#if defined(SI_TYPEOF_USED)
/* type - TYPE | value - EXPRESSION
 * Type prunes a value with the specified type ('typeof' is used to get value's type). */
#define si_transmute(type, value) si_transmuteEx(type, value, typeof(value))
#endif
/* type - TYPE | value - EXPRESSION | valueType - TYPE
 * Type prunes a value with the specified type. */
#define si_transmuteEx(type, value, valueType) ((union { valueType in; type out; }){value}.out)

/* ptr - rawptr
 * Converts the pointer's value into an u8. */
#define SI_TO_U8(ptr)  (*si_transmuteEx(u8*, (const rawptr)(ptr), const rawptr))
/* ptr - rawptr
 * Converts the pointer's value into an u16. */
#define SI_TO_U16(ptr) (*si_transmuteEx(u16*, (const rawptr)(ptr), const rawptr))
/* ptr - rawptr
 * Converts the pointer's value into an u32. */
#define SI_TO_U32(ptr) (*si_transmuteEx(u32*, (const rawptr)(ptr), const rawptr))
/* ptr - rawptr
 * Converts the pointer's value into an u64. */
#define SI_TO_U64(ptr) (*si_transmuteEx(u64*, (const rawptr)(ptr), const rawptr))


/*
	========================
	| Unary operators      |
	========================
*/
#if !defined(typeof) && !defined(SI_NO_TYPEOF)
	#if defined(SI_LANGUAGE_C) && SI_STANDARD_VERSION >= SI_STANDARD_C17
		#define SI_TYPEOF_USED 1

	#elif SI_COMPILER_TYPEOF_SUPPORTS
		/* ...VALUE - TYPE/EXPRESSION
		* Gets the value's type and expresses it as a regular type. */
		#define typeof(.../* VALUE */) __typeof__(__VA_ARGS__)
		#define SI_TYPEOF_USED 1

	#elif defined(SI_LANGUAGE_CPP) && SI_STANDARD_VERSION >= SI_STANDARD_CPP11
		/* ...VALUE - TYPE/EXPRESSION
		* Gets the value's type and expresses it as a regular type. */
		#define typeof(.../* VALUE */) decltype(__VA_ARGS__)
		#define SI_TYPEOF_USED 1

	#endif
#elif defined(typeof) && !defined(SI_NO_TYPEOF) && SI_COMPILER_TYPEOF_SUPPORTS
	#define SI_TYPEOF_USED 1
#endif

#if !defined(countof)
	/* value - ARRAY
	* Gets the static length of the given value (must be an array). */
	#define countof(.../* value */) (sizeof(__VA_ARGS__) / sizeof((__VA_ARGS__)[0]))
#endif
#if !defined(countof_str)
	/* value - ARRAY
	* Gets the static length of the given string while excluding the NULL terminator. */
	#define countof_str(.../* value */) (countof(__VA_ARGS__) - 1)
#endif
SI_STATIC_ASSERT(countof("abcd") == 5);
SI_STATIC_ASSERT(countof((i32[]){1, 2, 3, 4, 5, 6, 7, 8}) == 8);
SI_STATIC_ASSERT(countof_str("abcd") == 4);

#if !defined(offsetof)
	#if SI_COMPILER_GCC || SI_COMPILER_CLANG
		/* type - STRUCT TYPE | element - TYPE's member
		* Gets the offset of the provided member using '__builtin_offsetof'. */
		#define offsetof(type, element)  __builtin_offsetof(type, element)
	#else
		/* type - STRUCT TYPE | element - TYPE's member
		* Gets the offset of the provided member using 'si_offsetof'. */
		#define offsetof(type, element) si_offsetof(type, element)
	#endif
#endif

#if !defined(si_offsetof)
	/* type - STRUCT TYPE | element - TYPE's member
	* Gets the offset of the provided member. Doesn't work as a compile-time macro. */
	#define si_offsetof(type, element) ((usize)&(((type*)nil)->element))
#endif

#if (!defined(SI_LANGUAGE_CPP) && !defined(alignof)) || (defined(SI_LANGUAGE_CPP) && SI_STANDARD_VERSION < SI_STANDARD_CPP11)
	/* type - TYPE
	* Gets the alignment of a type. */
	#define alignof(type) offsetof(struct { char c; type member; }, member)
#endif

#if !defined(si_alignof)
	/* type - TYPE
	* Gets the alignment of a type. Doesn't work as a compile-time macro. */
	#define si_alignof(type) si_offsetof(struct { char c; type member; }, member)
#endif



/*
	========================
	| General macros       |
	========================
*/

/* bytes - usize
 * Allocates the memory into the stack. */
#define si_salloc(bytes) si_cast(rawptr, (siByte[bytes]){0})
/* type - TYPE/EXPRESSION
 * Allocates 'sizeof(type)' bytes to the stack and casts the value. */
#define si_sallocItem(type) (type*)si_salloc(sizeof(type))
/* type - TYPE/EXPRESSION | count - usize
 * Allocates an array of 'sizeof(type)' bytes to the stack and casts the value. */
#define si_sallocArray(type, count) (type*)si_salloc(sizeof(type) * count)
/* variable - VAR
 * Allocates 'sizeof(variable)' bytes to the stack and copies the value of
 * 'variable' to the allocated memory. */
#define si_sallocCopy(variable) \
	si_memcopy( \
		si_salloc(sizeof(variable)),  \
		&(variable) , \
		sizeof(variable) \
	)
/* allocator - siAllocator* | type - TYPE
 * Allocates 'sizeof(type)' bytes to the allocator and casts the value. */
#define si_mallocItem(allocator, type) (type*)si_malloc(allocator, sizeof(type))
/* allocator - siAllocator* | type - TYPE | count - usize
 * Allocates an array of 'sizeof(type)' bytes to the allocator and casts the value. */
#define si_mallocArray(allocator, type, count) (type*)si_malloc(allocator, sizeof(type) * (count))
/* allocator - siAllocator* | variable - VAR
 * Allocates 'sizeof(variable)' bytes to the allocator and copies the value of
 * 'variable' to the allocated memory. */
#define si_mallocCopy(allocator, variable) \
	si_memcopy( \
		si_malloc(allocator, sizeof(variable)),  \
		&(variable) , \
		sizeof(variable) \
	)
/* type - TYPE | ...VALUES - <EXPRESSION>, [EXPRESSION] ...
 * Declares a static buffer. */
#define si_buf(type, .../* VALUES */) (type[]){__VA_ARGS__}

#if defined(SI_TYPEOF_USED)
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
	si_memcopy(tmp, &a, sizeof(a)); \
	si_memcopy(&a, &b, sizeof(a)); \
	si_memcopy(&b, tmp, sizeof(b)); \
} while (0)

/* countvar - NAME | start - INT | end - INT
 * A loop that loops from 'start' to 'end' and increments 'countVar' each cycle. */
#define for_range(countVar, start, end) \
	for (usize countVar = (start); countVar < (usize)(end); countVar += 1)

#endif

#if SI_COMPILER_GCC || SI_COMPILER_CLANG
	/* x - CONDITION
	 * Denotes that this statement is more likely to be expected. */
	#define SI_LIKELY(x)   (__builtin_expect(!!(x), 1))
	/* x - CONDITION
	 * Denotes that this statement is less likely to be expected. */
	#define SI_UNLIKELY(x) (__builtin_expect(!!(x), 0))
#elif SI_COMPILER_MSVC
	/* x - CONDITION
	 * Denotes that this statement is more likely to be expected. */
	#define SI_LIKELY(x) (__assume(x), 1)
	/* x - CONDITION
	 * Denotes that this statement is less likely to be expected. */
	#define SI_UNLIKELY(x) (__assume(x), 0)
#else	/* x - CONDITION
	 * Denotes that this statement is more likely to be expected. */
	#define SI_LIKELY(x) (x)
	/* x - CONDITION
	 * Denotes that this statement is less likely to be expected. */
	#define SI_UNLIKELY(x) (x)
#endif

#if (SI_STANDARD_VERSION < SI_STANDARD_C11 || !defined(__STDC_LIB_EXT1__)) && !defined(memcpy_s)
/* dst - rawptr | dstsz - usize | src - rawptr | count - usize
 * A pre-C11 implementation of memcpy_s without 'errorno_t'. */
#define memcpy_s(dst, dstsz, src, count) si_memcopy(dst, src, si_min(dstsz, count))
#endif


/*
	========================
	| SI_ASSERT            |
	========================
*/
#ifndef SI_DEBUG_TRAP
	#if SI_COMPILER_MSVC
	 	#if SI_COMPILER_VERSION < SI_VERSION(7, 0, 0)
			#define SI_DEBUG_TRAP() __asm int 3
		#else
			#define SI_DEBUG_TRAP() __debugbreak()
		#endif
	#elif SI_COMPILER_GCC || SI_COMPILER_CLANG
		#define SI_DEBUG_TRAP() __builtin_trap()
	#else
		#define SI_DEBUG_TRAP() exit(1)
	#endif
#endif

usize si_assertEx(b32 condition, cstring conditionStr, cstring file, i32 line,
		cstring func, cstring message, ...);

#if defined(SI_NO_ASSERTIONS)
	#undef SI_NO_ASSERTIONS_IN_HEADER
	#define SI_NO_ASSERTIONS_IN_HEADER
#endif

#if !defined(SI_NO_ASSERTIONS_IN_HEADER)
/* condition - EXPRESSION
 * Crashes the app if the condition is not met. */
#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
/* condition - EXPRESSION | message - cstring
 * Crashes the app with a message if the condition is not met. */
#define SI_ASSERT_MSG(condition, message) si_assertEx(condition, #condition, __FILE__, __LINE__, __func__, message, "")
/* condition - EXPRESSION | message - cstring | ...FMT - VARIADIC
 * Crashes the app with a formatted message if the condition is not met. */
#define SI_ASSERT_FMT(condition, message, ...) si_assertEx(condition, #condition, __FILE__, __LINE__, __func__, message, __VA_ARGS__)
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
#define SI_PANIC() si_assertEx(false, "SI_PANIC()", __FILE__, __LINE__, __func__, nil)
/* message - cstring
 * Crashes the app immediately with a message. */
#define SI_PANIC_MSG(message) si_assertEx(false, "SI_PANIC_MSG()", __FILE__, __LINE__, __func__, message, "")
/* message - cstring | ...FMT - VARIADIC
 * Crashes the app immediately with a formatted message. */
#define SI_PANIC_FMT(message, ...) si_assertEx(false, "SI_PANIC_FMT()", __FILE__, __LINE__, __func__, message, __VA_ARGS__)

/* condition - EXPRESSION | ACTION - ANYTHING
 * Checks if the condition is true. If it is, execute 'action'. */
#define SI_STOPIF(condition, .../* ACTION */) if (condition) { __VA_ARGS__; } do {} while(0)


/*
	========================
	| siAny                |
	========================
*/

typedef struct siAny {
	/* Pointer to the data. */
	rawptr ptr;
	/* Size of the data */
	usize typeSize;
} siAny;

#if defined(SI_TYPEOF_USED)
/* ...VALUE - EXPRESSION
 * Creates a 'siAny' object. Said object's pointer is just "&(<VALUE>)". */
#define si_anyMake(.../* VALUE */) \
	(siAny){ \
		((rawptr)&((struct { typeof(__VA_ARGS__) in; }){__VA_ARGS__}.in)), \
		sizeof(typeof(__VA_ARGS__)) \
	}
#endif
/* type - TYPE | ...VALUE - EXPRESSION
 * Creates a 'siAny' object. Said object's pointer is just "&(<VALUE>)". */
#define si_anyMakeType(type, .../* VALUE */) \
	(siAny){ \
		((rawptr)&((struct { type in; }){__VA_ARGS__}.in)), \
		sizeof(type) \
	}


/*
	========================
	| siFunction           |
	========================
*/
/* Contains a function pointer for siThread. */
typedef rawptr SI_FUNC_PTR(siFunction, (rawptr));

#if defined(SI_TYPEOF_USED)
/* func - NAME
 * A hasty macro solution for inputting a function into a thread without a warning. */
#define siFunc(func) SI_FUNC_PTR_CHANGE(func, siFunction)
#else
/* func - NAME
 * A hasty macro solution for inputting a function into a thread that will result
 * in a warning. */
#define siFunc(func) (siFunction)(func)
#endif

#if defined(SI_TYPEOF_USED)
/* func1 - NAME | newType - TYPE
 * Changes the type of the 1st specified function to newly specified one in an ISO C
 * portable fashion. */
#define SI_FUNC_PTR_CHANGE(func1, newType) \
	SI_FUNC_PTR_CHANGE_EX(func1, typeof(&func1), newType)
#endif
/* func1 - NAME | ogType - TYPE | newType - TYPE
 * Changes the type of the 1st specified function to second one's in an ISO C
 * portable fashion. */
#define SI_FUNC_PTR_CHANGE_EX(func1, ogType, newType) \
	(((union { ogType a; newType b;}){func1}).b)


/*
	========================
	| Other                |
	========================
*/

/* A struct denoting the version. */
typedef struct { i32 major, minor; } siVersion;

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
#define SI_RECT(x, y, width, height) ((siRect){(f32)x, (f32)y, (f32)width, (f32)height})
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
#define SI_VEC4_R(rect) SI_VEC4((rect).x, (rect).y, (rect).width, (rect).height)
/* vec1 - siVec2 | vec2 - siVec2
 * Macro to define a 4D vector from two 2D vectors. */
#define SI_VEC4_2V(vec1, vec2) SI_VEC4((vec1).x, (vec1).y, (vec2).x, (vec2).y)


#ifndef SI_MAX_PATH_LEN
	#define SI_MAX_PATH_LEN 260
#endif

/* src - rawptr | srcLen - usize | moveBy - isize
 * Moves the memory by the specified amount to the right. */
#define si_ptrMoveRight(src, srcLen, moveBy) si_memcopy((siByte*)(src) - (moveBy), src, srcLen)
/* src - rawptr | srcLen - usize | moveBy - isize
 * Moves the memory by the specified amount to the left. */
#define si_ptrMoveLeft(src, srcLen, moveBy) si_memcopy((siByte*)(src) + (moveBy), src, srcLen)

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
	| SI_DEFINE            |
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

/* A structure containing the starting pointer of the data, maximum length and
 * the current index of the data. */
typedef struct siAllocator {
	siByte* ptr;
	usize offset;
	usize capacity;
	b32 isStack;
} siAllocator;


/* Sets memory alignment default for 'si_alignCeil'. Change it if need be. */
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


/* Aligns 'n' by SI_DEFAULT_MEMORY_ALIGNMENT. */
SIDEF usize si_alignCeil(usize n);
/* Aligns 'n' by 'alignment'. */
SIDEF usize si_alignCeilEx(usize n, u32 alignment);


/* Creates an allocator from the heap with N amounts of bytes. */
SIDEF siAllocator si_allocatorMake(usize bytes);
/* bytes - usize
 * Creates an allocator from the stack with N amounts of bytes. */
#define si_allocatorMakeStack(bytes) (siAllocator){si_salloc(bytes), 0, bytes, true}
/*  Creates a temporary allocator. */
SIDEF siAllocator si_allocatorMakeTmp(rawptr pointer, usize capacity);
/* TODO
 * TODO */
#define si_allocatorMakeAny(sizeMax, sizeRequested) \
	(((sizeRequested) <= (sizeMax)) ? si_allocatorMakeStack(sizeMax) : si_allocatorMake(sizeRequested))

/* */
SIDEF usize si_allocatorAvailable(siAllocator alloc);
/* */
SIDEF rawptr si_allocatorCurPtr(siAllocator alloc);

/* TODO */
SIDEF b32 si_allocatorResize(siAllocator* alloc, usize newSize);
/* TODO */
SIDEF void si_allocatorReset(siAllocator* alloc);
/* TODO */
SIDEF void si_allocatorResetFrom(siAllocator* alloc, usize offset);
/* TODO */
SIDEF void si_allocatorResetSub(siAllocator* alloc, usize amount);
/* TODO */
SIDEF void si_allocatorPush(siAllocator* alloc, siByte byte);
/* TODO */
SIDEF void si_allocatorFree(siAllocator* alloc);

#if defined(SI_NO_ALLOC_DEBUG_INFO)
/* Allocates 'bytes' amount of memory from the allocator. */
rawptr si_malloc(siAllocator* alloc, usize bytes);
/* Allocates 'bytes' multipled by 'num' amount of memory from the allocator. */
rawptr si_calloc(siAllocator* alloc, usize num, usize bytes);
/* Equivalent to doing 'si_malloc' and then 'si_memcopy'. */
rawptr si_realloc(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize);

#else
/* alloc - siAllocator* | bytes - usize
 * Allocates 'bytes' amount of memory from the allocator. */
#define si_malloc(alloc, bytes) si_mallocEx(alloc, bytes, __FILE__, __LINE__)
/* alloc - siAllocator* | num - usize | bytes - usize
 * Allocates 'bytes' multipled by 'num' amount of memory from the allocator. */
#define si_calloc(alloc, num, bytes) si_callocEx(alloc, num, bytes, __FILE__, __LINE__)
/* alloc - siAllocator* | ptr - rawptr | oldSize - usize | newSize - usize
 * Equivalent to doing 'si_malloc' and then 'si_memcopy'. */
#define si_realloc(alloc, ptr, oldSize, newSize) si_reallocEx(alloc, ptr, oldSize, newSize, __FILE__, __LINE__)

#if 1
	rawptr si_mallocEx(siAllocator* alloc, usize bytes, cstring filename, i32 line);
	rawptr si_callocEx(siAllocator* alloc, usize num, usize bytes, cstring filename, i32 line);
	rawptr si_reallocEx(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize,
			cstring filename, i32 line);
#endif
#endif /* SI_NO_ALLOC_DEBUG_INFO */


#if !defined(SI_NO_MEMORY)
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

/* Copies a sum amount of bytes from the provided source into the specified
 * destination. Both pointers musn't overlap. */
SIDEF rawptr si_memcopy(void* restrict dst, const void* restrict src, usize size);
/* Copies a sum amount of bytes from the provided source into the specified
 * destination. Both pointers can overlap each other. */
SIDEF rawptr si_memmove(void* dst, const void* src, isize size);
/* Sets a give amount of bytes from the provided data source to the specified
 * value. */
SIDEF rawptr si_memset(void* data, u8 value, usize size);
/* Compares a sum amount of bytes from two specified pointers and returns either:
 * A) zero, if all bytes match B) less than zero, if the first clashing byte in
 * the first pointer is lower than in the second one C) more than zero, if the
 * first clashing byte in the first pointer is higher than the second one. */
SIDEF i32 si_memcompare(const void* ptr1, const void* ptr2, usize size);
/* Searches a given amount of bytes from the provided data source and returns
 * either a pointer containing the first occurence of the specified value, or a
 * nil pointer if there were no occurences. */
SIDEF rawptr si_memchr(const void* data, u8 value, usize size);
/* Searches a given amount of bytes from the provided data source and returns
 * either a pointer containing the last occurence of the value, or a nil pointer
 * if there were no occurences. */
SIDEF rawptr si_memrchr(const void* data, u8 value, usize size);

/* Returns the length of a NULL-terminated C-string. */
SIDEF usize si_cstrLen(cstring string);


#endif /* SI_NO_MEMORY */

#if !defined(SI_NO_OPTIONAL)
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
	| siOptional           |
	========================
*/
siIntern struct { b32 hasValue; } si__internOptionalNull = {0};

/* type - TYPE
 * Specifies the return of a 'siOptional(type)'. */
#define siOptionalRet(type) rawptr
/* type - TYPE
 * Denotes a 'siOptional' type. */
#define siOptional(type) struct { b32 hasValue; type value; }*
/* A fully empty 'siOptional' variable. */
const rawptr SI_OPTIONAL_NULL = (const rawptr)&si__internOptionalNull;


#if defined(SI_TYPEOF_USED)
/* ...VALUE - EXPRESSION
 * Creates a returnable 'siOptional' from the given value. */
#define si_optionalMake(.../* VALUE */) \
	si_optionalMakeEx(typeof(__VA_ARGS__), __VA_ARGS__)
/* allocator - siAllocator*
 * Creates an empty 'siOptional' from the specified value and sets the 'hasValue'
 * member to false. */
#define si_optionalMakeEmpty(allocator, type, .../* VALUE */) \
	si_optionalMakeEmpty(allocator, typeof(__VA_ARGS__))

#endif
/* allocator - siAllocator* | type - TYPE | ...VALUE - EXPRESSION
 * Creates a 'siOptional' from the given value, writes it into the allocator and
 * returns a pointer to the written data.. */
#define si_optionalMakeEx(type, .../* VALUE */) \
	si__internOptionalMake( \
		si_anyMakeType(type, __VA_ARGS__), \
		si_offsetof(struct { b32 hasValue; type value; }, value) \
	)
/* allocator - siAllocator* | type - TYPE
 * Creates an empty 'siOptional' from the specified type and sets the 'hasValue'
 * member to false. */
#define si_optionalMakeEmptyEx(allocator, type) \
	si__internOptionalMakeEmptyEx( \
		si_mallocItem(allocator, struct { b32 hasValue; type value; }) \
	)

/* optionalVar - siOptional(TYPE) | defaultValue - EXPRESSION
 * Gets the value of the provided 'siObject' object. However if 'hasValue' is set
 * to false, return 'defaultValue' instead. */
#define si_optionalGetOrDefault(optionalVar, defaultValue) \
	((optionalVar)->hasValue ? (optionalVar)->value : (defaultValue))


/* optionalVar - siOptional(TYPE)
 * Resets the optional variable. */
#define si_optionalReset(optionalVar) \
	do { \
		memset(&(optionalVar)->value, 0, sizeof (optionalVar)->value); \
		(optionalVar)->hasValue = false; \
	} while(0)

#endif /* SI_NO_OPTIONAL */

#if !defined(SI_NO_ARRAY)
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

/* The header for 'siArray'. */
typedef struct siArrayHeader {
	/* The allocator used for creating the array. Used for reallocations, frees, etc. */
	siAllocator* allocator;
	/* The current amount of items. */
	usize len;
	/* The maximum possible count of maximum items. */
	usize capacity;
	/* Size for each item. */
	usize typeSize;
} siArrayHeader;

/* type - TYPE
 * Denotes a 'siArray' variable. */
#define siArray(type) type*
/* array - siArray(TYPE)
 * Gets the siArray's header. */
#define SI_ARRAY_HEADER(array) ((siArrayHeader*)array - 1)



typedef struct siBuffer {
	/* TODO */
	siByte* data;
	/* TODO */
	usize len;
} siBuffer;


/* TODO
 * TODO */
#define siBufferTest(type) type*

/* TODO
 * TODO */
#define SI_BUF(type, .../* values*/) SI_BUF_LEN(si_buf(type, __VA_ARGS__), countof(si_buf(type, __VA_ARGS__)))
#define SI_BUF_TEST(type, .../* values*/) \
	( \
	 (struct { usize len; type x[countof(si_buf(type, __VA_ARGS__))]; })(\
		 countof(si_buf(type, __VA_ARGS__)), \
		 si_buf(type, __VA_ARGS__) \
	).x \
	)

/* TODO
 * TODO */
#define SI_BUF_LEN(buffer, length) (siBuffer){(siByte*)buffer, length}
/* TODO
 * TODO */
#define SI_CSTR(string) SI_STR_LEN(string, si_cstrLen(string))



/* type - TYPE | name - NAME | array - siArray(TYPE)
 * Iterates through a 'siArray' and creates a 'type*' element via a specified name. */
#define for_eachArr(type, name, array) \
	for (type* name = array; name != (type*)si_arrayBack(array); name += 1 )
/* type - TYPE | name - NAME | array - siArray(TYPE)
 * Iterates through a 'siArray' in reverse and creates a 'type*' element via a specified
 * name. */
#define for_eachRevArr(type, name, array) \
	for (type* name = (type*)si_arrayBack(array); name >= array; name -= 1 )


/* array - siArray(TYPE) | index - usize
 * Gets a specific item's pointer and return it as a 'siByte*' for pointer arithmetic. */
#define si_arrayGetPtr(array, index) \
	(((siByte*)(array)) + SI_ARRAY_HEADER(array)->typeSize * (index))

/* allocator - siAllocator* | ...BUFFER - STATIC BUFFER
 * Creates a siArray from the provided static buffer. */
#define si_arrayMake(allocator, ...) \
	si_arrayMakeList(allocator, __VA_ARGS__, sizeof(*(__VA_ARGS__)), countof((__VA_ARGS__)))
/* Creates a siArray from the provided list. */
rawptr si_arrayMakeList(siAllocator* alloc, rawptr list, usize sizeofItem, usize count);
/* Reserves a 'count' amount of items in the array. */
rawptr si_arrayMakeReserve(siAllocator* alloc, usize sizeofItem, usize count);
/* Creates a siArray from another siArray. */
rawptr si_arrayCopy(siAllocator* alloc, siArray(void) array);

/* Gets the current length of the siArray. */
usize si_arrayLen(rawptr array);
/* Gets the item capacity of the siArray. */
usize si_arrayCapacity(rawptr array);
/* Gets the siArray's type size. */
usize si_arrayTypeSize(rawptr array);
/* Gets the array's allocator. */
siAllocator* si_arrayAllocator(rawptr array);
/* Returns the amount of bytes that the array takes up in memory whilst not
 * counting the header. Equivalent to arrayCapacity * arrayTypeSize. */
usize si_arrayTotalSize(rawptr array);
/* Returns true if 'array' is nil or the length is zero. */
b32 si_arrayEmpty(siNullable rawptr array);

/* Returns a pointer of the item. If the index is out of the bounds, the return
 * is nil. */
rawptr si_arrayAt(rawptr array, usize index);
/* Returns a pointer of the front item. Just syntax sugar, same as just returning
 * array. */
rawptr si_arrayFront(rawptr array);
/* Returns a pointer of the back item. */
rawptr si_arrayBack(rawptr array);

/* arrayPtr - siArray(TYPE)* | type - TYPE | | ...VALUE - EXPRESSION
 * Appends the given value to the array. If the array's capacity is reached, the
 * array is reallocated by (currentCapacity * 2). */
#define si_arrayAppend(arrayPtr, type, .../* VALUE */) si__internArrayAppend(arrayPtr, si_anyMakeType(type, __VA_ARGS__))
/* arrayPtr - siArray(TYPE)* | type - TYPE | | ...VALUE - EXPRESSION
 * Wrapper macro for si_arrayAppend. */
#define si_arrayPush(arrayPtr, type, .../* VALUE */) si_arrayAppend(arrayPtr, type, __VA_ARGS__)
/* Erases the last item in the array. */
void si_arrayPop(rawptr arrayPtr);
/* arrayPtr - siArray(TYPE)* | type - TYPE | | newItem - EXPRESSION | index - usize
 * Inserts the specified item at the provided index. */
#define si_arrayInsert(arrayPtr, type, index, ...) si__internArrayInsert(arrayPtr, index, si_anyMakeType(type, __VA_ARGS__))
/* Erases the specified index. */
void si_arrayErase(rawptr arrayPtr, usize index);
/* Erases the given index alongside the amount of elements specified afterwards. */
void si_arrayEraseCount(rawptr arrayPtr, usize index, usize count);
/* arrayPtr - siArray(TYPE)* | type - TYPE | | ...VALUE - EXPRESSION
 * Removes every item in the array that is equal to the value. */
#define si_arrayRemoveItem(arrayPtr, type, .../* VALUE */) si__internArrayRemoveItem(arrayPtr, si_anyMakeType(type, __VA_ARGS__))
/* arrayPtr - siArray(TYPE)* | type - TYPE | index - usize | count - usize | ...VALUE - EXPRESSION
 * 'Fills the array with the specified amount of the given value at an index. */
#define si_arrayFill(arrayPtr, type, index, count, .../* VALUE */ ) si__internArrayFill(arrayPtr, index, count, si_anyMakeType(type, __VA_ARGS__))

/* array - siArray(TYPE) | ...VALUE - EXPRESSION
 * Returns an optional value of the first mention of the specified value, starting
  from the start. */
#define si_arrayFind(array, type, ...) si__internArrayFind(array, 0, si_arrayLen(array), si_anyMakeType(type, __VA_ARGS__))
/* array - siArray(TYPE) | start - usize | end - usize | ...VALUE - EXPRESSION
 * Returns an optional value of the first mention of the specified value, starting
  from the specified start to the specified end. */
#define si_arrayFindEx(array, type, start, end, ...) si__internArrayFind(array, start, end, si_anyMakeType(type, __VA_ARGS__))
/* array - siArray(TYPE) | type - TYPE | ...VALUE - EXPRESSION
 * Returns an optional value of the last mention of the specified value, starting
  from the back. */
#define si_arrayRFind(array, type, ...) si__internArrayRFind(array, si_arrayLen(array) - 1, 0, si_anyMakeType(type, __VA_ARGS__))
/* array - siArray(TYPE) | type - TYPE | | start - usize | end - usize | ...VALUE - EXPRESSION
 * Returns an optional value of the last mention of the specified value, starting
  from the specified start to the specified end. */
#define si_arrayRFindEx(array, type, start, end, ...) si__internArrayRFind(array, start, end, si_anyMakeType(type, __VA_ARGS__))
/* array - siArray(TYPE) | type - TYPE | | oldValue - EXPRESSION | ...newValue - EXPRESSION
 * Replaces every mention of the old specified value with the newly specified one. */
#define si_arrayReplace(arrayPtr, type, oldValue, .../* newValue */) si__internArrayReplace(array, si_anyMakeType(type, oldValue), si_anyMakeType(type, __VA_ARGS__))
/* Reverses the contents of the array. */
void si_arrayReverse(rawptr array);

/* Sums the contents of the array as an unsigned number. */
u64 si_arraySumU64(siArray(void) array);
/* Sums the contents of the array as a signed number. */
i64 si_arraySumI64(siArray(void) array);
/* Sums the contents of the array as a float number. */
f64 si_arraySumF64(siArray(void) array);

/* Clears the contents of the array to zero. */
void si_arrayClear(siArray(void) array);
/* Checks if two arrays equal. */
b32 si_arrayEqual(siArray(void) left, siArray(void) right);

#if 1 /* NOTE(EimaMei): The actual implementations. Shouldn't be used in practice really. */
siOptionalRet(usize) si__internArrayFind(rawptr array, usize start, usize end,
		siAny value);
siOptionalRet(usize) si__internArrayRFind(rawptr array, usize start, usize end,
		siAny value);
rawptr si__internArrayAppend(rawptr arrayPtr, siAny value);
void si__internArrayInsert(rawptr arrayPtr, usize index, siAny value);
void si__internArrayReplace(rawptr arrayPtr, siAny oldValue, siAny newValue);
void si__internArrayRemoveItem(rawptr arrayPtr, siAny item);
void si__internArrayFill(rawptr arrayPtr, usize index, usize count, siAny item);
#endif

#endif /* SI_NO_ARRAY */

#if !defined(SI_NO_STRING)
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

typedef siBuffer siString;

/* name - NAME |  string- siString
 * Iterates through the provided string and creates a pointer to the current indexed
 * character via the given name. */
#define for_eachStr(name, string) \
	for (u8* name = string.data; name < &(string).data[(string).len]; name += 1)
/* name - NAME |  string- siString
 * Iterates through the provided string in reverse and creates a pointer to the
 * current indexed character via the given name. */
#define for_eachRevStr(name, string) \
	for (u8* name = &(string).data[(string).len] - 1; name >= string.data; name -= 1)


/* string - cstring
 * Creates a sili-string literal on the stack from a string literal. */
#define SI_STR(string) SI_STR_LEN(string, countof_str(string))
/* string - STR | len - usize
 * Creates a sili-string literal on the stack from a length-specified C-string. */
#define SI_STR_LEN(string, len) (siString){(u8*)string, len}
/* string - STR
 * Creates a sili-string on the stack from a NULL-terminated C-string. */
#define SI_CSTR(string) SI_STR_LEN(string, si_cstrLen(string))


/* TODO */
SIDEF siString si_stringMake(siString from, siAllocator* alloc);
/* TODO */
SIDEF siString si_stringMakeCstr(cstring from, siAllocator* alloc);
/* TODO */
SIDEF siString si_stringMakeReserve(usize len, siAllocator* alloc);

/* TODO */
SIDEF char* si_stringCloneToCstr(siString from, siAllocator* alloc);
/* TODO */
SIDEF char* si_stringCloneToCstrEx(siString from, usize capacity, siAllocator* alloc);

/* Gets the character at a specified index in the sili-string. */
SIDEF siOptionalRet(char) si_stringAt(siString string, usize index);
/* Gets the first character of the sili-string. */
SIDEF siOptionalRet(char) si_stringAtFront(siString string);
/* Gets the last character of the sili-string. */
SIDEF siOptionalRet(char) si_stringAtBack(siString string);
/* Returns the front pointer of the string (aka &str.data[0]). Asserts if the string
 * length is zero on release mode. */
SIDEF u8* si_stringFront(siString string);
/* Returns the back pointer of the string (aka &str.data[str.len]). Asserts if
 * the string length is zero on release mode. */
SIDEF u8* si_stringBack(siString string);


/* string - siString | begin - usize | len - usize
 * Returns a substring of the string from the specified beginning with a length. */
#define si_stringSub(string, begin, len) SI_STR_LEN(&(string).data[begin], len)
/* string - siString | begin - usize
 * Returns a substring of the string from the specified beginning to the end of
 * string. */
#define si_stringSubToEnd(string, begin) SI_STR_LEN(&(string).data[begin], (string).len - (begin))

/* string - siString
 * Returns true if the sili-string's length is set. */
#define si_stringEmpty(string) ((string).len == 0)

/* TODO */
SIDEF isize si_stringFind(siString string, siString subStr);
/* TODO */
SIDEF isize si_stringFindByte(siString string, siByte byte);
/* TODO */
SIDEF isize si_stringFindUtf8(siString string, char character[4], isize* utf8AtIndex);
/* TODO */
SIDEF isize si_stringFindLast(siString string, siString subStr);
/* TODO */
SIDEF isize si_stringFindLastByte(siString string, siByte byte);
/* Returns the amount of occurences of a substring in the sili-string. */
SIDEF usize si_stringFindCount(siString string, siString subStr);

/* Compares two strings and returns an integer, representing which first byte comes
 * first lexicographically. '-1' for 'lhs', `1` for rhs, '0' if they're equal. */
i32 si_stringCompare(siString lhs, siString rhs);
/* Compares two strings and returns a boolean, indicating if they're equal or not.  */
b32 si_stringEqual(siString lhs, siString rhs);


/* Sets the value of the specified sili-string with a provided one. If the provided
 * sili-string's length is larger than the primary's, the data gets allocated,
 * otherwise it's only copied onto the primary.
 * NOTE: A new string may be allocated. */
void si_stringSet(siString* string, siString str, siAllocator* alloc);
/* ... */
siString si_stringJoin(siString* stringArr, usize len, siString separator,
		siAllocator* alloc);
/* Replaces all occurences of the first specified substring with the given second
 * substring. Returns a boolean indicating if anything was changed/allocated.
 * NOTE: A new string may be allocated. */
b32 si_stringReplaceAll(siString* string, siString oldStr, siString newStr,
		siAllocator* alloc);
/* Replaces a specified amount of occurences of the first specified substring with
 * the given second substring. Returns a boolean indicating if anything was
 * changed/allocated. If the specified amount is negative, every occurence gets
 * replace.
 * NOTE: A new string may be allocated. */
b32 si_stringReplace(siString* string, siString oldStr, siString newStr, isize amount,
		siAllocator* alloc);
/* Cuts the front and back occurences of the provided cut set in the sili-string. */
void si_stringTrim(siString* string, siString cutSet);
/* Cuts the front occurences of the provided cut set in the sili-string. */
void si_stringTrimLeft(siString* string, siString cutSet);
/* Cuts the back occurences of the provided cut set in the sili-string. */
void si_stringTrimRight(siString* string, siString cutSet);
/* Removes any leading or trailing spaces and newlines in the string. */
void si_stringStrip(siString* string);
/* Enquotes the given string. */
void si_stringEnquote(siString* string, siAllocator* alloc);

/* Appends the specified sili-string with the given one.
 * NOTE: A new string may be allocated. */
void si_stringAppend(siString* string, siString other, siAllocator* alloc);
/* Pushes a character into the sili-string.
 * NOTE: A new string may be allocated. */
void si_stringPush(siString* string, char other, siAllocator* alloc);
/* Pops the last character of the sili-string.  */
void si_stringPop(siString* string);
/* Clears the string by setting the length to zero. */
void si_stringClear(siString* str);

/* Inserts a null-terimnated C-string to the siString at the specified index. */
void si_stringInsert(siString* string, cstring cstr, usize index);
/* Inserts a C-string with specified length to the siString at the specified index. */
void si_stringInsertLen(siString* string, cstring cstr, usize cstrLen, usize index);
/* Inserts a C-string with specified length to the siString at the specified index,
 * as erases the index itself if 'eraseIndex' is set to true. */
void si_stringInsertEx(siString* string, cstring cstr, usize cstrLen, usize index, b32 eraseIndex);
/* Erases a substring from the specified index. */
void si_stringErase(siString str, usize index, usize eraseLen);
/* Removes every occurrence of the provided null-terminated C-string. */
void si_stringRemoveCstr(siString str, cstring cstr);

/* Splits the string into multiple parts based on the separator and returns either
 * an allocated array of sub-strings or 'nil' if no delimiter was found. */
SIDEF siArray(siString) si_stringSplit(siString string, siString delimiter,
	siAllocator* alloc);
/* Splits the string a specified amount based on the separator and returns either
 * an allocated array of sub-strings or 'nil' if no delimiter was found. */
SIDEF siArray(siString) si_stringSplitEx(siString string, siString delimiter,
	isize amount, siAllocator* alloc);


/* Reverses the entirety of the sili-string */
void si_stringReverse(siString string);
/* Internal function. Allocates more space to the string. */
void si_stringMakeSpaceFor(siString* string, siAllocator* alloc, usize addLen);

#if !defined(SI_NO_MEMORY)

/* TODO */
SIDEF rawptr si_memcopyStr(void* dst, siString src);
/* TODO */
SIDEF i32 si_memcompareStr(const void* dst, siString src);

#endif

#endif /* SI_NO_STRING */

#if !defined(SI_NO_UNICODE)
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
typedef cstring siUtf8String;
/* Denotes that the string is UTF-16 encoded. */
typedef u16* siUtf16String;


/* Denotes that the UTF-8 character is invalid. */
#define SI_UNICODE_INVALID (-1)


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
siUtf32Char si_utf8Decode(const char character[4]);
/* Encodes the specified UTF-32 character into UTF-8 and returns a 'siUtf8Char'
 * structure. This function does not check if the UTF-32 is invalid. */
siUtf8Char si_utf8Encode(i32 codepoint);

/* Returns the amount of characters in an UTF-8 string. For example, an input
 * string of "Ąsotis" would returns 7 from 'si_cstrLen', while 'si_utf8StrLen'
 * would returns 6. */
usize si_utf8StrLen(siUtf8String str);
/* Returns a UTF-32 codepoint and length from the specified index. For example,
 * 'si_utf8StrAt("リトアニア", 2)' will return (siUtf32Char){0x30A2, 3}. */
siUtf32Char si_utf8StrAt(siUtf8String str, usize charIndex);
/* TODO(EimaMei): Add a version that returns siUtf8Char. */

/* Encodes a NULL-terminated UTF-8 string into UTF-16. */
siUtf16String si_utf8ToUtf16Str(siAllocator* alloc, siUtf8String str, siNullable usize* strLenOut);
/* Encodes a length-specified UTF-8 string into UTF-16. */
siUtf16String si_utf8ToUtf16StrEx(siAllocator* alloc, siUtf8String str, usize strLen,
		siNullable usize* strLenOut);
/* Encodes a NULL-terminated UTF-16 string into UTF-8. */
siUtf8String si_utf16ToUtf8Str(siAllocator* alloc, siUtf16String str, siNullable usize* strLenOut);


/* Decodes the given UTF-16 character into UTF-32 and returns a 'siUtf32Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf32Char){SI_UNICODE_INVALID, 0}. */
siUtf32Char si_utf16Decode(const u16 character[2]);
/* Decodes the given UTF-16 character into UTF-8 and returns a 'siUtf8Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf8Char){SI_UNICODE_INVALID, 0}. */
siUtf8Char si_utf16Encode(const u16 character[2]);

#endif /* SI_NO_UNICODE */

#if !defined(SI_NO_CHAR)
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

/* Returns the lowercased version of the given character. */
SIDEF char si_charLower(char c);
/* TODO */
SIDEF char si_charLowerUnsafe(char c);
/* Returns the uppercased version of the given character. */
SIDEF char si_charUpper(char c);
/* TODO */
char si_charUpperUnsafe(char c);

/* Returns true  if the given character is a space. */
SIDEF b32 si_charIsSpace(char c);
/* Returns true if the given character is from '0' to '9'. */
SIDEF b32 si_charIsDigit(char c);
/* Returns true if the given character is a hex digit (between '0'..'9'; 'a'...'f'; 'A'...'F'). */
SIDEF b32 si_charIsHexDigit(char c);
/* Returns true if the given character is in the English alphabet. */
SIDEF b32 si_charIsAlpha(char c);
/* Returns true if the given character is in the English alphabet OR is a number. */
SIDEF b32 si_charIsAlphanumeric(char c);
/* Converts '0'...'9' to an actual integer ('3' -> 3). */
SIDEF i32 si_charDigitToInt(char c);
/* Converts a hex digit into an actual integer ('F' -> 15). */
SIDEF i32 si_charHexDigitToInt(char c);

#endif /* SI_NO_CHAR */

#if !defined(SI_NO_STRING)
/*
	========================
	|  cstring             |
	========================
*/

extern const char* SI_NUM_TO_CHAR_TABLE;

/* TODO */
SIDEF void si_stringUpper(siString string);
/* TODO */
SIDEF void si_stringLower(siString string);
/* TODO */
SIDEF void si_stringTitle(siString string);
/* TODO */
SIDEF void si_stringCapitalize(siString string);

/* Sets the boolean to use lowercase/uppercase characters when converting an
 * integer to a string. If 'upper' is set to true, 255 (base 16) gets turned to
 * "FF", otherwise it turns to "ff". By default this is set to true. */
void si_numChangeTable(b32 upper);

/* Creates and returns an unsigned 64-bit integer from a string.
 * NOTE: The function doesn't check for any non-number characters. */
u64 si_stringToUInt(siString string);
/* TODO
 * NOTE(1): Results may not work for bases higher than 10.
 * NOTE(2): The function doesn't check for any non-number characters. */
u64 si_stringToUIntEx(siString string, b32* outRes);
/* Creates and returns a signed 64-bit integer from a string.
 * NOTE: The function doesn't check for any non-number characters. */
i64 si_stringToInt(siString string);
/* TODO
 * NOTE(1): Results may not work for bases higher than 10.
 * NOTE(2): The function doesn't check for any non-number characters. */
SIDEF i64 si_stringToIntEx(siString string, b32* outRes);
/* Creates a string from the specified unsigned integer in base 10. The string
 * gets allocated in the specified allocator. */
SIDEF siString si_stringFromUInt(u64 num, siAllocator* alloc);
/* Creates a string from the specified unsigned integer in the given base. The
 * string gets allocated in the specified allocator. */
SIDEF siString si_stringFromUIntEx(u64 num, i32 base, siAllocator* alloc);
/* Creates a string from the specified signed integer in base 10. The string
 * gets allocated in the specified allocator. */
SIDEF siString si_stringFromInt(i64 num, siAllocator* alloc);
/* Creates a string from the specified signed integer in the given base. The
 * string gets allocated in the specified allocator. */
SIDEF siString si_stringFromIntEx(i64 num, i32 base, siAllocator* alloc);


/* Creates a string from a float. By default the output base is base 10 and 6
 * decimals after the decimal point. */
SIDEF siString si_stringFromFloat(f64 num, siAllocator* alloc);
/* Creates a string from a float, using the specified base and amount of decimals
 * after the decimals point. String gets allocated unless it's "inf" or "nan". */
SIDEF siString si_stringFromFloatEx(f64 num, i32 base, u32 afterPoint, siAllocator* alloc);
/* TODO(EimaMei): f64 si_cstr_to_f64(cstring str); */

/* TODO */
SIDEF siString si_stringFromBool(b32 boolean);
/* TODO */
SIDEF b32 si_stringToBool(siString string);

#endif /* SI_NO_STRING */

#if !defined(SI_NO_HASHTABLE)

/* type - TYPE
 * Denotes that this is a 'siHashTable' variable. */
#define siHt(type) siHashTable

typedef struct siHashEntry {
	/* Key of the value. */
	u64 key;
	/* Pointer to the value. */
	rawptr value;
	/* Points to the next valid item. */
	struct siHashEntry* next;
} siHashEntry;

typedef siArray(siHashEntry) siHashTable;

/* Creates a hash table using the given names and data. */
siHashTable si_hashtableMake(siAllocator* alloc, const rawptr* keyArray, usize keyLen,
		const rawptr* dataArray, usize len);
/* Reserves a 'capacity' amount of items for the hash table. */
siHashTable si_hashtableMakeReserve(siAllocator* alloc, usize capacity);
/* Returns the key entry's value pointer from the hash table. If not found, nil
 * is returned. */
rawptr si_hashtableGet(const siHashTable ht, const rawptr key, usize keyLen);
/* ht - siHashTable* | key - cstring | type - TYPE
 * Returns the key entry's value as the specified type. If the item does NOT exist,
 * this will 100% crash, as the return of 'si_hashtableGet' will be nil. */
#define si_hashtableGetItem(ht, key, type) (*((type*)si_hashtableGet(ht, key)))
/* Returns the key entry's value pointer from the hash table. If not found, nil
 * is returned. */
rawptr si_hashtableGetWithHash(const siHashTable ht, u64 hash);
/* Adds a 'key' entry to the hash table and returns the entry pointer, regardless
 * if it's a new or pre-existing one. 'outSuccess' is set to true if the hash
 * didn't exist before the set, otherwise it's set to 'false' and the entry's
 * value's gets overwritten either way (unless disabled via a macro). 'outSuccess'
 * can be nullable.*/
siHashEntry* si_hashtableSet(siHashTable ht, const rawptr key, usize keyLen,
		const rawptr valuePtr, b32* outSuccess);
siHashEntry* si_hashtableSetWithHash(const siHashTable ht, u64 hash, const rawptr valuePtr,
		siNullable b32* outSuccess);

#endif /* SI_NO_HASHTABLE */

#if !defined(SI_NO_IO)
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
	SI_FILE_READ = SI_BIT(0),
	SI_FILE_WRITE = SI_BIT(1),
	SI_FILE_APPEND = SI_BIT(2),
	SI_FILE_PLUS = SI_BIT(3),

	SI_FILE_ALL = SI_FILE_READ | SI_FILE_WRITE | SI_FILE_APPEND | SI_FILE_PLUS
};

typedef struct siFile {
	#if SI_SYSTEM_IS_WINDOWS
		/* OS-specific handle of the file. */
		rawptr handle;
	#else
		/* OS-specific handle of the file. */
		i64 handle;
	#endif
	/* Size of the read file. */
	usize size;
	/* Filename of the file. */
	/* Last time it was written at. */
	u64 lastWriteTime;
} siFile;

SI_ENUM(i32, siFilePermissions) {
	SI_FS_PERM_ERR = -1, /* An error occurred. */
	SI_FS_PERM_NONE = 0, /* No permission bits are set. */

	SI_FS_PERM_OWNER_READ = 0400, /* File owner has read permission. */
	SI_FS_PERM_OWNER_WRITE = 0200, /* File owner has write permission. */
	SI_FS_PERM_OWNER_EXEC = 0100, /* File owner has execute/search permission. */
	SI_FS_PERM_OWNER_ALL = 0700, /* File owner has read, write, and execute/search permissions, equivalent to
									 'SI_FS_PERM_OWNER_READ | SI_FS_PERM_OWNER_WRITE | SI_FS_PERM_OWNER_EXEC'. */
	SI_FS_PERM_GROUP_READ = 040, /* The file's user group has read permission. */
	SI_FS_PERM_GROUP_WRITE = 020, /* The file's user group has write permission. */
	SI_FS_PERM_GROUP_EXEC = 010, /* The file's user group has execute/search permission. */
	SI_FS_PERM_GROUP_ALL = 070, /* The file's user group has read, write, and execute/search permissions, equivalent to
									 'SI_FS_PERM_GROUP_READ | SI_FS_PERM_GROUP_WRITE | SI_FS_PERM_GROUP_EXEC'. */
	SI_FS_PERM_OTHERS_READ = 04, /* Other users have read permission. */
	SI_FS_PERM_OTHERS_WRITE = 02, /* Other users have write permission. */
	SI_FS_PERM_OTHERS_EXEC = 01, /* Other users have execute/search  permission. */
	SI_FS_PERM_OTHERS_ALL = 07, /* Other users have read, write, and execute/search permissions, equivalent to
									 'SI_FS_PERM_OTHERS_READ | SI_FS_PERM_OTHERS_WRITE | SI_FS_PERM_OTHERS_EXEC'. */
	SI_FS_PERM_ALL = 0777, /* All users have read, write, and execute/search permissions. Equivalent to
								'SI_FS_PERM_OWNER_ALL | SI_FS_PERM_GROUP_ALL | SI_FS_PERM_OTHERS_ALL'. */
	/* UNIX ONLY */
	SI_FS_PERM_SET_UID = 04000, /* Set user ID to file owner user ID on execution. */
	SI_FS_PERM_SET_GID = 02000, /* Set group ID to file's user group ID on execution. */
	SI_FS_PERM_STICKY_BIT = 01000, /* Implementation-defined meaning, but POSIX XSI specifies that when set on a directory,
								   only file owners may delete files even if the directory is writeable to others (used with /tmp). */
	SI_FS_PERM_MASK = 07777 /* 	All valid permission bits, equivalent to 'SI_FS_PERM_ALL | SI_FS_PERM_SET_UID |
									SI_FS_PERM_SET_GID | SI_FS_PERM_STICKY_BIT'. */
};

SI_ENUM(i32, siFileMoveMethod) {
	SI_FILE_MOVE_BEGIN = 0, /* Sets the pointer from the beginning of the file. */
	SI_FILE_MOVE_CURRENT = 1, /* Sets the pointer from the current offset. */
	SI_FILE_MOVE_END = 2 /* Sets the pointer from the EOF offset. */
};

SI_ENUM(i32, siStdFileType) {
	SI_STDTYPE_INPUT,
	SI_STDTYPE_OUTPUT,
	SI_STDTYPE_ERROR,
	SI_STDTYPE_COUNT
};

#define SI_STDIN  (si_fileGetStdFile(SI_STDTYPE_INPUT))
#define SI_STDOUT (si_fileGetStdFile(SI_STDTYPE_OUTPUT))
#define SI_STDERR (si_fileGetStdFile(SI_STDTYPE_ERROR))

SI_ENUM(i32, siFileSystemError) {
	SI_FS_ERROR_NONE = 0, /* No error has been encountered. */
	SI_FS_ERROR_INVALID, /* Provided value is invalid. */
	SI_FS_ERROR_INVALID_FILENAME, /* Invalid filename. */
	SI_FS_ERROR_EXISTS, /* File/directory already exists. */
	SI_FS_ERROR_DOESNT_EXIST, /* File/directory doesn't exist. */
	SI_FS_ERROR_PERMISSION, /* User doesn't have permission to read/write. */
	SI_FS_ERROR_TRUNCATE_FAILURE /* Failed to truncate the file. */
};

#if SI_SYSTEM_IS_WINDOWS
	const char SI_PATH_SEPARATOR = '\\';
#else
	const char SI_PATH_SEPARATOR = '/';
#endif

/* Last file system error. On non-release mode, the user gets notified when an
 * error happens. */
extern siErrorInfo SI_FS_ERROR;
/* Returns the string representation of the error code. */
cstring si_pathFsErrorName(siFileSystemError err);
/* Returns the description of the error. */
cstring si_pathFsErrorStr(siFileSystemError err);

/* The amount of bytes that were read. */
extern isize SI_FS_READ_BYTES;

/*
	========================
	|  siPath              |
	========================
*/

/* TODO */
SIDEF b32 si_pathExists(siString path);
/* TODO */
SIDEF isize si_pathCopy(siString existingPath, siString newPath);
/* Copies every file and folder from 'pathSrc' to 'pathDst'. Returns the amount
 * of items that were copied. */
SIDEF u32 si_pathItemsCopy(siString pathSrc, siString pathDst);
/* TODO */
SIDEF b32 si_pathMove(siString existingPath, siString newPath);
/* TODO */
SIDEF b32 si_pathRename(siString oldName, siString newName);
/* TODO */
SIDEF b32 si_pathCreateFolder(siString path);
/* TODO */
SIDEF b32 si_pathCreateFolderEx(siString path, siFilePermissions perms);
/* TODO */
SIDEF b32 si_pathRemove(siString path);
/* TODO */
SIDEF b32 si_pathCreateHardLink(siString existingPath, siString linkPath);
/* TODO */
SIDEF b32 si_pathCreateSoftLink(siString existingPath, siString linkPath);
/* TODO */
SIDEF b32 si_pathEditPermissions(siString path, siFilePermissions newPerms);

/* TODO */
SIDEF siString si_pathBaseName(siString path);
/* TODO */
SIDEF siString si_pathExtension(siString path);
/* TODO */
SIDEF siString si_pathUnrooted(siString path);
/* TODO */
SIDEF siString si_pathGetFullName(siString path, siAllocator* alloc);
/* TODO */
SIDEF u64 si_pathLastWriteTime(siString path);
/* TODO */
SIDEF siString si_pathGetTmp(void);
/* TODO */
SIDEF siFilePermissions si_pathPermissions(siString path);

/* TODO */
SIDEF b32 si_pathIsAbsolute(siString path);
/* */
SIDEF b32 si_pathIsRelative(siString path);

/*
	========================
	|  siFile              |
	========================
*/

/* Gets the pointer to the specified standard file (stdin, stdout or stderr). */
siFile* si_fileGetStdFile(siStdFileType type);

/* TODO */
SIDEF siFile si_fileCreate(siString path);
/* TODO */
SIDEF siFile si_fileOpen(siString path);
/* The file is opened under the user specified mode. */
SIDEF siFile si_fileOpenMode(siString path, siFileMode mode);

/* Returns the size of the file. */
usize si_fileSize(siFile file);
/* Updates the size of the file in the structure. */
void si_fileSizeUpdate(siFile* file);
/* Returns 'true' if the file has been changed since the last write time. */
b32 si_fileHasChanged(siFile file);


/* TODO */
SIDEF siBuffer si_fileRead(siFile file, usize len, siAllocator* alloc);
SIDEF siBuffer si_fileReadBuf(siFile file, usize len, siBuffer buffer);
/* TODO */
SIDEF siBuffer si_fileReadAt(siFile file, isize offset, usize len, siAllocator* alloc);
SIDEF siBuffer si_fileReadAtBuf(siFile file, isize offset, usize len, siBuffer outBuffer);

/* TODO */
SIDEF siBuffer si_fileReadUnsafe(siFile file, isize offset, usize len, rawptr outBuffer);

/* TODO */
SIDEF siString si_fileReadContents(siFile file, siAllocator* alloc);
SIDEF siString si_fileReadContentsBuf(siFile file, siBuffer outBuffer);
/* Reads the contents of the file, then splits every line and writes them all
 * into a 'siArray(siString)'. */
SIDEF siArray(siString) si_fileReadlines(siFile file, siAllocator* alloc);


/* TODO */
SIDEF isize si_fileWrite(siFile* file, siBuffer string);
/* TODO */
SIDEF isize si_fileWriteAt(siFile* file, siBuffer content, isize offset);
/* TODO */
SIDEF isize si_fileWriteAtLine(siFile* file, siString line, isize index);

/* Returns the current offset of the file stream. */
isize si_fileTell(siFile file);
/* Seeks the file stream offset to the specified offset using the given method.
 * Returns the amount of bytes that were read. */
isize si_fileSeek(siFile file, isize offset, siFileMoveMethod method);
/* Seeks to the front of the file. Returns 'true' if the operation went through. */
b32 si_fileSeekFront(siFile file);
/* Seeks to the back of the file. Returns 'true' if the operation went through. */
b32 si_fileSeekBack(siFile file);

/* Truncates the file to the specified size. Returns 'true' if the operation went
 * through. */
b32 si_fileTruncate(siFile* file, usize size);

/* Closes the file. */
void si_fileClose(siFile file);

/*
	========================
	|  siDirectory         |
	========================
*/
SI_ENUM(i32, siIOType) {
	SI_IO_TYPE_ANY = 1,

	/* === Cross-platform === */
	SI_IO_TYPE_FILE,
	SI_IO_TYPE_DIR,
	SI_IO_TYPE_LINK,

	SI_IO_TYPE_BITS = SI_IO_TYPE_FILE | SI_IO_TYPE_DIR | SI_IO_TYPE_LINK,

	/* === THE REST ARE UNIX ONLY. === */
	SI_IO_TYPE_SOCKET,
	SI_IO_TYPE_DEVICE,
	SI_IO_TYPE_BLOCK,
	SI_IO_TYPE_FIFO,

	SI_IO_TYPE_BITS_ALL = SI_IO_TYPE_BITS | SI_IO_TYPE_SOCKET | SI_IO_TYPE_DEVICE | SI_IO_TYPE_BLOCK | SI_IO_TYPE_FIFO
};

/* TODO */
typedef struct siDirectoryEntry {
	siString path;
	siIOType type;
} siDirectoryEntry;

/* TODO */
typedef struct siDirectory {
	rawptr handle;
	usize directoryLen;
	char buffer[SI_MAX_PATH_LEN];
} siDirectory;


/* TODO */
SIDEF siDirectory si_dirOpen(siString path);
/* TODO */
SIDEF b32 si_dirPollEntry(siDirectory dir, siDirectoryEntry* entry);
/* TODO */
SIDEF b32 si_dirPollEntryEx(siDirectory dir, siDirectoryEntry* entry, b32 fullPath);


#endif

#if !defined(SI_NO_THREAD)
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


typedef struct siThread {
	#if SI_SYSTEM_IS_WINDOWS
		/* Win32 thread ID. */
		HANDLE id;
	#else
		/* POSIX thread ID. */
		pthread_t id;
	#endif

	/* Dictates how much stack size gets allocated for th thread. Setting this
	 * to 0 makes the system give the default amount of stack size to the thread. */
	usize stackSize;

	/* A booleaned inidicating if the structure was made through 'si_threadMake'. */
	b32 initialized;
	/* A boolean indicating if the thread is still running. */
	volatile b32 isRunning;

	/* The rawptr return of the function. */
	rawptr returnValue;

	/* Pointer to the function. */
	siFunction func;
	/* The functions' argument. */
	rawptr arg;
} siThread;

/* TODO(EimaMei): Add mutexes and other threading stuff. */


/* function - NAME | arg - siNullable rawptr | runThread - b32 | outThread - siThread*
 * Creates a 'siThread' data strucutre and if specified, immediately runs the
 * thread.  If the function fails to create the thread, the returned thread's
 * 'id' member is set to 'nil', as well as 'isRunning'.*/
#define si_threadMake(function, arg, runThread, outThread) \
	si__internThreadMakeEx(siFunc(function), arg, 0, runThread, outThread)
#define si_threadMakeArr(function, arg, runThread, outThreads, len) \
	for_range (i, 0, len) { \
		si_threadMake(function, arg, runThread, &((outThreads)[i])); \
	} do {} while (0)

/* function - NAME | arg - siNullable rawptr | stackSize - usize | runThread - b32
 * Creates a 'siThread' data strucutre with the given stack size and if specified,
 * immediately runs the thread. */
#define si_threadMakeEx(function, arg, stackSize, runThread, outThread) \
	si__internThreadMakeEx(siFunc(function), arg, stackSize, runThread, outThread)
#define si_threadMakeArrEx(function, arg, stackSize, runThread, outThreads, len) \
	for_range (i, 0, len) { \
		si_threadMakeEx(function, arg, stackSize, runThread, &outThreads[i]); \
	} do {} while (0)


/* thread - siThread | type - TYPE
 * Casts 'thread.returnValue' correctly to match the given type instead of it being
 * rawptr. */
#define si_threadGetReturn(thread, type) (*(type*)(&(thread).returnValue))

/* Runs the thread. */
b32 si_threadRun(siThread* t);
/* Starts the thread while hanging the parent thread until the specified thread
 * is finished. */
b32 si_threadJoin(siThread* t);
/* Starts the threads while hanging the parent thread until the specified thread
 * is finished. */
#define si_threadJoinArr(threads, len) \
	for_range (i, 0, len) { \
		si_threadJoin(&threads[i]); \
	} do {} while (0)


/* Stops the thread mid-execution.
 * NOTE: Only works on Unix. */
b32 si_threadCancel(siThread* t);
/* Destroys the thread. */
b32 si_threadDestroy(siThread* t);

/* Sets the priority of the thread. */
b32 si_threadPrioritySet(siThread t, i32 priority);


#if 1
void si__internThreadMakeEx(siFunction function, siNullable rawptr arg, usize stackSize,
		b32 runThread, siThread* outThread);
#endif

#endif

#if !defined(SI_NO_TIME)
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

/* How many clocks of 'si_clock()' are equivalent to a second (1s = 1e+9 ns). */
#define SI_CLOCKS_PER_SECOND (1000000000)
/* How many clocks of 'si_clock()' are equivalent to a milisecond (1ms = 1e+6 ns). */
#define SI_CLOCKS_PER_MILISECOND (1000000)
/* How many clocks of 'si_clock()' are equivalent to a milisecond (1µs = 1e+3 ns). */
#define SI_CLOCKS_PER_MICROSECOND (1000)

/* An unsigned 64-bit integer type, notifying that it's used for time stamps. */
typedef u64 siTimeStamp;
/* A signed 64-bit integer type, notifying that it's used for UTC-0 time. */
typedef i64 siUtcTime;
/* A signed 64-bit integer type, notifying that it's used for local time. */
typedef i64 siLocalTime;

/* Executes the 'RDTSC' asm instruction and returns the value in nano seconds.
 * NOTE: Only natively works for x86-64, i386, ARM64 and PPC CPUs. On other CPUs
 * the function relies on OS functions like 'gettimeofday'. */
siTimeStamp si_RDTSC(void);
/* Returns the number of seconds since 1970-01-01 UTC */
siUtcTime si_timeNowUTC(void);
/* Returns the number of seconds since 1970-01-01 local time.*/
siLocalTime si_timeNowLocal(void);

/* Returns the current clock. */
u64 si_clock(void);

/* Starts the timestamp. */
siTimeStamp si_timeStampStart(void);
/* Prints the time since the start. */
#define si_timeStampPrintSince(time) si_timeStampPrintSinceEx(time, __FILE__, __LINE__)

/* Makes the CPU sleep for a certain amount of miliseconds. */
void si_sleep(usize miliseconds);


#if 1
	void si_timeStampPrintSinceEx(siTimeStamp t, cstring filename, i32 line);
#endif

#endif

#if !defined(SI_NO_BIT)
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
	/* 0b00000000. */
	SI_BIT_ZERO,
	/* 0b00000001. */
	SI_BIT_ONE
};


/* value - EXPRESSION | flag - UINT
 * TODO */
#define SI_BIT_GET(value, flag) ((value) & (flag))
/* variable - VARIABLE | flag - UINT | condition - EXPRESSION
 * TODO */
#define SI_BIT_SET(variable, flag, condition) \
	(condition) ? ((variable) |= (flag)) : ((variable) &= ~(flag))


/* num - INT
 * Gets the most significant bit of the number.
 * TODO(EimaMei): REWORK */
#define SI_BIT_MSB(num) ((num & SI_BIT(sizeof(typeof(num)) * 8 - 1)) != 0)
/* num - INT
 * Gets the least significant bit of the number. */
#define SI_BIT_LSB(num) ((num & SI_BIT(0)) != 0)

/* bits - INT
 * Shortcut for (bits * 8). */
#define SI_BYTE_TO_BIT(bits) ((bits) * (usize)8)

/* num - INT
 * Gets the upper half of the number */
#define SI_NUM_HIGH_BITS(num) ( \
	(num & ((typeof(num))0xFF << SI_BYTE_TO_BIT(sizeof(typeof(num)) - 1))) \
	>> \
	SI_BYTE_TO_BIT(sizeof(typeof(num)) - 1))

/* num - INT
 * Gets the lower half of the number */
#define SI_NUM_LOW_BITS(num) (num & ((typeof(num))0xFF))

#if SI_ENDIAN_IS_LITTLE
	/* little - EXPRESSION | big - EXPRESSION
	* Returns the specified 'little' value. */
	#define SI_ENDIAN_VALUE(little, big) little
#else
	/* little - EXPRESSION | big - EXPRESSION
	* Returns the specified 'big' value. */
	#define SI_ENDIAN_VALUE(little, big) end
#endif

/* ptr - rawptr | sizeofNum - usize
 * Extracts 'sizeofNum' amount of bytes from a given pointer. Maximum is 8 bytes. */
#define si_numFromPtr(ptr, sizeofNum) \
	(*si_cast(u64*, ptr) & ((sizeofNum < 8) ? (SI_BIT((sizeofNum) * 8) - 1) : UINT64_MAX) )

/* x - INT
 * Returns true if x is negative. */
#define si_numIsNeg(x) ((x) < 0)

/* Returns how many 1 bits are in an 8-bit number. */
usize si_numCountBitsU8(u8 num);
/* Returns how many 1 bits are in a 32-bit number. */
usize si_numCountBitsU32(u32 num);
/* Returns how many 1 bits are in a 64-bit number. */
usize si_numCountBitsU64(u64 num);

#if SI_LANGUAGE_C && SI_STANDARD_VERSION >= SI_STANDARD_C11
	#define si_numCountBits(num) \
		_Generic((num), \
			u8  : si_numCountBitsU8(num), \
			u16 : si_numCountBitsU32(num), \
			u32 : si_numCountBitsU32(num), \
			u64 : si_numCountBitsU64(num), \
			default: si_numCountBitsU64(num) \
		)(X)
#endif

/* type - TYPE | num - UINT | bitType - siBitType
 * Returns the amount of leading bits of the specified bit type (0 or 1). */
#define si_numLeadingBit(type, num, bitType) (type)si_numLeadingBitEx(num, sizeof(type) * 8, bitType)
/* type - TYPE | num - UINT | bitType - siBitType
 * Returns the amount of trailing bits of the specified bit type (0 or 1). */
#define si_numTrailingBit(type, num, bitType) (type)si_numTrailingBitEx(num, sizeof(type) * 8, bitType)
/* type - TYPE | num - UINT | bits - usize
 * Rotates the bits of the number left by 'bits' amount. */
#define si_numRotateLeft(type, num, bits) (type)si_numRotateLeftEx(num, sizeof(type) * 8, bits)
/* type - TYPE | num - UINT | bits - usize
 * Rotates the bits of the number right by 'bits' amount. */
#define si_numRotateRight(type, num, bits) (type)siNumRotateRightEx(num, sizeof(type) * 8, bits)
/* type - TYPE | num - UINT
 * Reverses the bits of the number. */
#define si_numReverseBits(type, num) (type)siNumReverseBitsEx(num, sizeof(type) * 8 )
/* allocator - siAllocator* | type - TYPE | num - UINT
 * Creates a 'siArray(u8)' from the specified number and writes it into the allocator. */
#define si_numToBytes(allocator, type, num) si_numToBytesEx(allocator, num, sizeof(type))
/* Makes a number from the specified siArray(u8). */
u64 si_bytesToNumSiArr(siArray(siByte) arr);
/* Makes a number from a C-array. */
u64 si_bytesToNumArr(siByte* array, usize len);


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
SIDEF usize si_numLen(u64 num);
/* Returns the length of a specified base unsigned number. */
SIDEF usize si_numLenEx(u64 num, u32 base);
/* Returns the length of a base 10 signed number. */
SIDEF usize si_numLenI64(i64 num);
/* Returns the length of a specified base signed number. */
SIDEF usize si_numLenI64Ex(i64 num, u32 base);


#if 1 /* NOTE(EimaMei): The actual header definition for the macros. No reason to use these in practice. */
usize si_numLeadingBitEx(u64 num, usize sizeof_num, siBitType bit);
usize si_numTrailingBitEx(u64 num, usize number_sizeof, siBitType bit);

u64 si_numRotateLeftEx(u64 num, usize num_sizeof, usize bits);
u64 siNumRotateRightEx(u64 num, usize num_sizeof, usize n);
u64 siNumReverseBitsEx(u64 num, usize num_sizeof);

siArray(u8) si_numToBytesEx(siAllocator* alloc, u64 num, usize num_sizeof);
#endif

#endif

#if !defined(SI_NO_CPU)
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
/* The frequency of the CPU in mHz (~3000, ~4200, etc). Set to 0 by default. When
 * 'si_cpuClockSpeed' is ran, the result gets cached to this variable. */
extern u32 SI_CPU_FREQ_MHZ;

/* Returns the CPU's clock speed in mHz. The result of the function gets cached
 * to 'SI_CPU_FREQ_GHZ'. */
u32 si_cpuClockSpeed(void);

#endif /* SI_NO_CPU */

#if !defined(SI_NO_PRINT)
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

SI_ENUM(u8, siPrintColorType) {
	SI_PRINT_COLOR_TYPE_3BIT = 1,
	SI_PRINT_COLOR_TYPE_8BIT,
	SI_PRINT_COLOR_TYPE_24BIT,
};

SI_ENUM(u8, siAnsiColorType) {
	SI_ANSI_BLACK,
	SI_ANSI_RED,
	SI_ANSI_GREEN,
	SI_ANSI_YELLOW,
	SI_ANSI_BLUE,
	SI_ANSI_MAGENTA,
	SI_ANSI_CYAN,
	SI_ANSI_WHITE
};

typedef struct siPrintColor3bit {
	siPrintColorType type;
	siAnsiColorType color;
	b8 bold;
	b8 light;
} siPrintColor3bit;

typedef struct siPrintColor24bit {
	siPrintColorType type;
	u8 r, g, b;
} siPrintColor24bit;
SI_STATIC_ASSERT(sizeof(siPrintColor3bit) == sizeof(siPrintColor24bit));

/* TODO
 * TODO */
#define SI_PRINT_COLOR_3BIT(color, bold, light) \
	(siPrintColor3bit){SI_PRINT_COLOR_TYPE_3BIT, color, bold, light}

/* Writes a NULL-terminated C-string into SI_STDOUT. Returns the amount of
 * written bytes. */
isize si_print(cstring str);
/* Writes a NULL-terminated formatted C-string into SI_STDOUT. Returns the amount
 * of written bytes. */
isize si_printf(cstring fmt, ...);
isize si_printfVa(cstring fmt, va_list va);

/* Writes a NULL-terminated C-string into a specified file. Returns the amount of
 * written bytes. */
isize si_fprint(siFile* file, cstring str);
/* Writes a NULL-terminated formatted C-string into a specified file. Returns the
 * amount of written bytes. */
isize si_fprintf(siFile* file, cstring fmt, ...);
isize si_fprintfVa(siFile* file, cstring fmt, va_list va);

/* TODO */
SIDEF isize si_sprintf(char* buffer, cstring fmt, ...);
SIDEF isize si_sprintfVa(char* buffer, cstring fmt, va_list va);

/* TODO */
SIDEF isize si_snprintf(char* buffer, usize outCapacity, cstring fmt, ...);
SIDEF isize si_snprintfVa(char* buffer, usize outCapacity, cstring fmt, va_list va);
/* TODO */
SIDEF isize si_sprintfAlloc(siAllocator* allocator, char** out, cstring fmt, ...); // TODO(EimaMei): Remove the char** and replace it with siString as the return
SIDEF isize si_sprintfAllocVa(siAllocator* allocator, char** out, cstring fmt, va_list va);


#endif /* !defined(SI_NO_PRINT) */

#if !defined(SI_NO_DLL)
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

/* TODO */
SIDEF siDllHandle si_dllLoad(cstring path);
/* Unloads the specified DLL. */
SIDEF void si_dllUnload(siDllHandle dll);
/* Returns the pointer to the specified processor name of the DLL. */
SIDEF siDllProc si_dllProcAddress(siDllHandle dll, cstring name);

#if defined(SI_TYPEOF_USED)
/* dll - siDllHandle | function - FUNCTION
 * Loads the specified function's name as a processor and returns it as an ISO-C
 * friendly function. */
#define si_dllProcAddressFunc(dll, function) \
	si_dllProcAddressFuncEx(dll, function, typeof(function))
#endif

/* dll - siDllHandle | function - FUNCTION |  type - TYPE
 * Loads the specified function's name as a processor and returns it as an ISO-C
 * friendly function. */
#define si_dllProcAddressFuncEx(dll, function, type) \
	(((union { rawptr a; type b; }){si_dllProcAddress(dll, #function)}).b)


#endif /* !defined(SI_NO_DLL) */

#if !defined(SI_NO_MATH)
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
#define si_min(a, b) ((a) < (b) ? (a) : (b))
/* Returns the smallest of its arguments: either a, b or c.*/
#define si_min3(a, b, c) si_min(si_min(a, b), c)

/* Returns the largest of its arguments: either a or b.*/
#define si_max(a, b) ((a) > (b) ? (a) : (b))
/* Returns the largest of its arguments: either a, b or c.*/
#define si_max3(a, b, c) si_max(si_max(a, b), c)

/* Returns 'lower' if x is lower than it, 'upper' if x is upper than it or itself
 * if neither. */
#define si_clamp(x, lower, upper) si_min(upper, si_max(x, lower))

/* Returns the absolute value of 'x'. */
SIDEF i64 si_abs(i64 x);
SIDEF f64 si_absF64(f64 x);
SIDEF f32 si_absF32(f32 x);
#define si_absf(x) (sizeof(x) == 4 ? si_absF32(x) : si_absF64(x))

/* Returns a boolean that indicates if 'x' is between 'lower' and 'upper'. */
SIDEF b32 si_between(i64 x, i64 lower, i64 upper);
SIDEF b32 si_betweenu(u64 x, u64 lower, u64 upper);
SIDEF b32 si_betweenF64(f64 x, f64 lower, f64 upper);
SIDEF b32 si_betweenF32(f32 x, f32 lower, f32 upper);
#define si_betweenf(x, lower, upper) (sizeof(x) == 4 ? si_betweenF32(x, lower, upper) : si_betweenF64(x, lower, upper))
#define si_betweenPtr(x, lower, upper) si_betweenu((usize)(x), (usize)(lower), (usize)(upper))

SIDEF f64 si_sinF64(f64 x);
SIDEF f32 si_sinF32(f32 x);
#define si_sin(x) (sizeof(x) == 4 ? si_sinF32((f32)(x)) : si_sinF64((f64)(x)))

SIDEF f64 si_cosF64(f64 x);
SIDEF f32 si_cosF32(f32 x);
#define si_cos(x) (sizeof(x) == 4 ? si_cosF32((f32)(x)) : si_cosF64((f64)(x)))

/* Rounds 'x' to the nearest integer. */
SIDEF f64 si_roundF64(f64 x);
SIDEF f32 si_roundF32(f32 x);
#define si_round(x) (sizeof(x) == 4 ? si_roundF32((f32)(x)) : si_roundF64((f64)(x)))

/* Calculates the largest integer not greater than 'x'. */
SIDEF f64 si_floorF64(f64 x);
SIDEF f32 si_floorF32(f32 x);
#define si_floor(x) (sizeof(x) == 4 ? si_floorF32((f32)(x)) : si_floorF64((f64)(x)))

/* Calculates the smallest integer not less than 'x'. */
SIDEF f64 si_ceilF64(f64 x);
SIDEF f32 si_ceilF32(f32 x);
#define si_ceil(x) (sizeof(x) == 4 ? si_ceilF32((f32)(x)) : si_ceilF64((f64)(x)))

/* Raises an unsigned base 10 by the power of the exponent, and returns the result
 * as a 64-bit unsigned int (meaning the exponent can only be from 0 to 19, otherwise
 * the app will crash). */
u64 si_pow10(u32 exponent);
/* Raises a f64 10 by the power of the exponent. The exponent must be in-between
 * -18 to 18, otherwise the app will crash.*/
f64 si_pow10F64(i32 exponent);

/* Returns true if the specified 64-bit float is a NaN. */
b32 si_f64IsNan(f64 num);

/* Rounds up the number to the nearest multiple and returns it. */
i32 si_numRoundNearestMultipleI32(i32 num, i32 multiple);
i64 si_numRoundNearestMultipleI64(i64 num, i32 multiple);
#define si_numRoundNearestMultiple(num, multiple) \
	(sizeof(num) == 4 \
	 ? si_numRoundNearestMultipleI32((i32)(num), multiple) \
	 : si_numRoundNearestMultipleI64((i64)(num), multiple) \
	)
#endif

#if !defined(SI_NO_ARGV)
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
	| siArgv               |
	========================
*/

/* */
SI_ENUM(u32, siOptionErrorType) {
	/* No error was encountered. */
	SI_OPTION_ERROR_NONE = 0,
	/* User-specified option either uses a wrong prefix or doesn't use one at all.
	 * NOTE: 'ctx.error.option' it set to nil. */
	SI_OPTION_ERROR_PREFIX,
	/* User-specified option doesn't exist.
	 * NOTE: 'ctx.error.option' is set to nil. */
	SI_OPTION_ERROR_OPTION,
	/* An option is specified more than once by the user. */
	SI_OPTION_ERROR_MULTIPLE,
	/* User-specified option requires a value, however nothing was provided. */
	SI_OPTION_ERROR_NO_VALUE,
	/* User-specified value does not conform to the option's required type.
	 * Eg. Entering an invalid number like "0xQD" for an integer option. */
	SI_OPTION_ERROR_INVALID_VALUE,
	/* User-specified value isn't in the range/list of valid choices. */
	SI_OPTION_ERROR_INVALID_CHOICE,
	/* User did not use the required separator. */
	SI_OPTION_ERROR_SEPARATOR,
	/* User did not specify a required option. */
	SI_OPTION_ERROR_REQUIRED
};


/* */
SI_ENUM(i32, siOptionType) {
	SI_OPTION_TYPE_STRING = 1,
	SI_OPTION_TYPE_INT,
	SI_OPTION_TYPE_UINT,
	SI_OPTION_TYPE_FLOAT,
	SI_OPTION_TYPE_BOOL,
};

/* */
SI_ENUM(u32, siOptionConfig) {
	SI_OPTION_CONFIG_REQUIRED = SI_BIT(0),
	SI_OPTION_CONFIG_NO_SEPARATOR = SI_BIT(1),
	SI_OPTION_CONFIG_POSITIONAL = SI_BIT(2),
	SI_OPTION_CONFIG_IS_ACTION = SI_BIT(3),

	SI_OPTION_CONFIG_BITS = SI_OPTION_CONFIG_REQUIRED | SI_OPTION_CONFIG_NO_SEPARATOR
							| SI_OPTION_CONFIG_POSITIONAL | SI_OPTION_CONFIG_IS_ACTION,
};
/* */
SI_ENUM(u32, __siInternOptionState) {
	SI__INTERN_OPTION_IS_SET = SI_BIT(0),
	SI__INTERN_OPTION_FLAG = SI_BIT(1),
	SI__INTERN_OPTION_CHOICES = SI_BIT(2),
	SI__INTERN_OPTION_HAS_DEFAULT = SI_BIT(3),
};

/* */
typedef union siOptionValue {
	siString string;
	u64 uinteger;
	i64 integer;
	f64 floatingPoint;
	b32 boolean;
} siOptionValue;


/* */
typedef struct {
	/* */
	siOptionType type;
	/* */
	siOptionConfig config;

	/* */
	siString name;
	siUtf8Char nameShort;
	i32 codepoint;

	/* */
	__siInternOptionState __state;

	/* */
	siString description;
	/* */
	siOptionValue defaultValue;

	union {
		siBuffer list;
		i64 rangeI[2];
		f64 rangeF[2];
	} choices;

	usize outCapacity;
	usize outLen;
	rawptr outPtr;
} siOption;


/* */
typedef struct siOptionError {
	siOptionErrorType type;
	siOption* option;
	siString value;
} siOptionError;

/* */
typedef struct {
	siOption* options;
	usize len;
	usize capacity;

	siString executable;

	siString prefix;
	siString prefixShort;
	siString separator;

	siString description;
	siString usage;

	u32 paddingFront;
	u32 maxOptionLen;
	usize maxColumnLen;

	siOptionError error;
} siOptionContext;


/* */
SIDEF siOptionContext si_argvMake(cstring prefix, siOption* options, usize len);
/* */
SIDEF u32 si_argvParse(siOptionContext* ctx, int argc, char** argv);
/* */
SIDEF void si_argvHelp(siOptionContext ctx);
/* */
SIDEF void si_argvError(siOptionContext ctx);


/* */
SIDEF siOptionContext si_argvContextMake(cstring prefix, siOption* options, usize len);
/* */
SIDEF void si_argvContextSetPrefix(siOptionContext* ctx, cstring prefix);
/* */
SIDEF void si_argvContextSetPrefixShort(siOptionContext* ctx, cstring prefixShort);
/* */
SIDEF void si_argvContextSetSeparator(siOptionContext* ctx, cstring separator);
/* */
SIDEF void si_argvContextSetDescription(siOptionContext* ctx, cstring usage);
/* */
SIDEF void si_argvContextSetUsage(siOptionContext* ctx, cstring usage);

/* */
SIDEF siOption* si_argvOptionMakeStr(siOptionContext* ctx, cstring name,
		siString* outPtr);
/* */
SIDEF siOption* si_argvOptionMakeInt(siOptionContext* ctx, cstring name, i64* outPtr);
/* */
SIDEF siOption* si_argvOptionMakeUint(siOptionContext* ctx, cstring name, u64* outPtr);
/* */
SIDEF siOption* si_argvOptionMakeBool(siOptionContext* ctx, cstring name, b32* outPtr);
/* */
SIDEF siOption* si_argvOptionMakeEx(siOptionContext* ctx, siOptionType type,
		cstring name, rawptr outPtr);
/* */
SIDEF siOption* si_argvFlagMake(siOptionContext* ctx, cstring name, b32 isAction,
		b32* outPtr);

/* */
SIDEF void si_argvOptionSetDescription(siOption* option, cstring description);
/* */
SIDEF void si_argvOptionSetShortName(siOption* option, i32 codepoint);
/* */
SIDEF void si_argvOptionSetConfig(siOption* option, siOptionConfig config);
/* */
SIDEF void si_argvOptionSetChoices(siOption* option, siBuffer choices);
/* */
SIDEF void si_ArgvOptionSetCapacity(siOption* option, usize capacity);
/* smth about the function. */
#define si_argvOptionSetDefault(/*siOption**/ option, /*TYPE*/ type, /*VALUE*/...) si__internArgvOptionSetDefault(option, sizeof(type), &(type){__VA_ARGS__})
/* TODO */
#define si_argvOptionSetRange(/*siOption**/ option, /*TYPE*/ type, /*VALUE*/ value1, \
		/*VALUE*/ value2) si__internArgvOptionSetRange(option, sizeof(type), &(type){value1}, &(type){value2})

#if 1
SIDEF void si__internArgvOptionSetDefault(siOption* option, usize item_sizeof,
		const rawptr ptr);
SIDEF void si__internArgvOptionSetRange(siOption* option, usize item_sizeof,
		const rawptr ptr1, const rawptr ptr2);
#endif

#endif /* SI_NO_ARGV */

#if !defined(SI_NO_BENCHMARK)
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
		clock_t timeSince = si_RDTSC(); \
		for_range (i, 0, timesToLoop) { \
			function; \
		} \
		timeSince = si_RDTSC() - timeSince; \
		u64 timeTaken = timeSince / si_cpuClockSpeed() / 1000; /* NOTE(EimaMei): This will take the time in ms, not seconds. */ \
		si_printf(SI_PERFORMANCE_MSG, #function, timeTaken, (f64)timesToLoop / 1000000.0); \
	} while(0)
/* timesToLoop - usize | function - NAME
 * Runs the function 'timesToLoop' for 'ms' miliseconds and prints how many times
 * the function got executed.  */
#define si_benchmarkExecutesPerMs(ms, function) \
	do { \
		u64 counter = 0; \
		u32 miliseconds = (u32)ms; \
		siThread thread; \
		si_threadMake(si__internBenchmarkThread, &miliseconds, true, &thread); \
		while (thread.isRunning) { \
			function; \
			counter += 1; \
		} \
		si_printf(SI_PERFORMANCE_MSG, #function, miliseconds, (f64)counter / 1000000.0); \
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
		siAllocator alloc = si_allocatorMakeStack(SI_KILO(4)); \
		siArray(u64) cycles = si_arrayMakeReserve(&alloc, sizeof(u64), 20); \
		\
		si_benchmarkLoop(function, cycles, start, end); \
		si_benchmarkLoopsAvgPrint(&alloc, #function, start, end, cycles, (f32)si_cpuClockSpeed() / 1000.f); \
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
		siAllocator alloc = si_allocatorMakeStack(SI_KILO(4)); \
		siArray(u64) cycles[2] = { \
			(u64*)si_arrayMakeReserve(&alloc, sizeof(u64), 20), \
			(u64*)si_arrayMakeReserve(&alloc, sizeof(u64), 20)  \
		};\
		si_benchmarkLoop(function1, cycles[0], start, end); \
		si_benchmarkLoop(function2, cycles[1], start, end); \
		\
		cstring funcnames[2] = {#function1, #function2}; \
		si_benchmarkLoopsAvgCmpPrint(&alloc, funcnames, start, end, cycles); \
	} while(0)

typedef struct { siTimeStamp duration; cstring unit; } siBenchmarkLimit;

#define SI_PERFORMANCE_MSG \
	"====== BENCHMARK DATA ======\n" \
	"General:\n" \
		"\tFunction - '%s'\n" \
		"\tDuration - '%u' ms\n" \
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
			cycles[arrayIndex] = begin; \
			arrayIndex += 1; \
		} \
		SI_ARRAY_HEADER(cycles)->len = arrayIndex; \
	} while (0)

extern const siBenchmarkLimit siBenchLimit[];

const siBenchmarkLimit* si_benchmarkLimitLoop(siTimeStamp time);

#endif

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

#if defined(SI_IMPLEMENTATION)
	#define SI_IMPLEMENTATION_GENERAL
	#define SI_IMPLEMENTATION_MEMORY
	#define SI_IMPLEMENTATION_OPTIONAL
	#define SI_IMPLEMENTATION_ARRAY
	#define SI_IMPLEMENTATION_STRING
	#define SI_IMPLEMENTATION_UNICODE
	#define SI_IMPLEMENTATION_CHAR
	#define SI_IMPLEMENTATION_HASHTABLE
	#define SI_IMPLEMENTATION_IO
	#define SI_IMPLEMENTATION_THREAD
	#define SI_IMPLEMENTATION_BIT
	#define SI_IMPLEMENTATION_CPU
	#define SI_IMPLEMENTATION_PRINT
	#define SI_IMPLEMENTATION_DLL
	#define SI_IMPLEMENTATION_MATH

#ifndef SI_NO_PRINTF
	#define SI_IMPLEMENTATION_ARGV
#endif

	#define SI_IMPLEMENTATION_BENCHMARK
#endif


#if defined(SI_IMPLEMENTATION_GENERAL) && !defined(SI_NO_GENERAL)

inline
usize si_alignCeil(usize num) {
	SI_STOPIF(num % SI_DEFAULT_MEMORY_ALIGNMENT == 0, return num);
	num |= SI_DEFAULT_MEMORY_ALIGNMENT - 1;
	num += 1;

	return num;
}
inline
usize si_alignCeilEx(usize num, u32 alignment) {
	SI_STOPIF(num % alignment == 0, return num);
	num |= alignment - 1;
	num += 1;
	return num;
}

inline
siAllocator si_allocatorMake(usize bytes) {
	siAllocator allocator;
	allocator.capacity = si_alignCeil(bytes);
	allocator.offset = 0;
	allocator.ptr = malloc(allocator.capacity);
	allocator.isStack = false;

	SI_ASSERT_NOT_NULL(allocator.ptr);
	return allocator;
}
inline
siAllocator si_allocatorMakeTmp(rawptr pointer, usize capacity) {
	SI_ASSERT_NOT_NULL(pointer);

	siAllocator allocator;
	allocator.capacity = capacity;
	allocator.offset = 0;
	allocator.ptr = (siByte*)pointer;
	allocator.isStack = true;

	return allocator;
}
inline
void si_allocatorFree(siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(alloc);

	if (!alloc->isStack) {
		free(alloc->ptr);
		*alloc = (siAllocator){0};
	}
	else {
		*alloc = (siAllocator){0};
		alloc->isStack = true;
	}
}

inline
b32 si_allocatorResize(siAllocator* alloc, usize newSize) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_STOPIF(alloc->isStack, return false);

	alloc->ptr = realloc(alloc->ptr, newSize);
	alloc->capacity = newSize;
	alloc->offset = 0;
	SI_ASSERT_NOT_NULL(alloc->ptr);

	return true;
}

inline
void si_allocatorReset(siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(alloc);
	alloc->offset = 0;
}

inline
void si_allocatorResetFrom(siAllocator* alloc, usize offset) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_MSG(offset < alloc->capacity, "Provided offset is too large.");
	alloc->offset = offset;
}
inline
void si_allocatorResetSub(siAllocator* alloc, usize amount) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_MSG(amount <= alloc->capacity && amount <= alloc->offset, "Provided amount is too large.");
	alloc->offset -= amount;

}
inline
usize si_allocatorAvailable(siAllocator alloc) {
	return alloc.capacity - alloc.offset;
}
inline
rawptr si_allocatorCurPtr(siAllocator alloc) {
	return (rawptr)(alloc.ptr + alloc.offset);
}
SIDEF
void si_allocatorPush(siAllocator* alloc, siByte byte) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_FMT(alloc->offset + 1 < alloc->capacity,
		"Exceeded the available memory for an allocation",
		" (Tried writing '%zd' bytes into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
		alloc->capacity, alloc->offset);

	alloc->ptr[alloc->offset] = byte;
	alloc->offset += 1;
}
#if !defined(SI_NO_ALLOC_DEBUG_INFO)
SIDEF
rawptr si_mallocEx(siAllocator* alloc, usize bytes, cstring filename, i32 line) {
	SI_ASSERT_NOT_NULL(alloc);

	usize aligned = si_alignCeil(bytes);
	siByte* res = &alloc->ptr[alloc->offset];

	alloc->offset += aligned;

	if (alloc->offset > alloc->capacity) {
		si_fprintf(SI_STDERR,
			"%s:%i: si_malloc: Exceeded the available memory for an allocation"
			" (Tried writing '%zd' bytes into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
			  filename, line, bytes, alloc->capacity, alloc->offset
		);
		exit(1);
	}

	return res;
}
SIDEF
rawptr si_callocEx(siAllocator* alloc, usize num, usize bytes,
		cstring filename, i32 line) {
	rawptr res = si_mallocEx(alloc, num * bytes, filename, line);
	return memset(res, 0, num * bytes);
}
SIDEF
rawptr si_reallocEx(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize,
		cstring filename, i32 line) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_STOPIF(ptr == nil, return si_malloc(alloc, newSize));

	SI_ASSERT_FMT(
		si_betweenPtr(ptr, alloc->ptr, alloc->ptr + alloc->capacity),
		"%s:%i: Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
		filename, line, ptr, alloc->ptr, alloc->ptr + alloc->capacity
	);
	rawptr out = si_malloc(alloc, newSize);
	si_memcopy(out, ptr, oldSize);
	return out;
}

#else
SIDEF
rawptr si_malloc(siAllocator* alloc, usize bytes) {
	SI_ASSERT_NOT_NULL(alloc);

	siByte* res = alloc->ptr + alloc->offset;
	alloc->offset += bytes;

	SI_ASSERT_FMT(
		alloc->offset < alloc->capacity,
		"Exceeded the available memory for an allocation ((Tried writing '%zd'"
		"bytes into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
		bytes, alloc->capacity, alloc->offset
	);

	return res;
}

SIDEF
rawptr si_calloc(siAllocator* alloc, usize num, usize bytes) {
	rawptr res = si_malloc(alloc, num * bytes);
	return memset(res, 0, num * bytes);
}
SIDEF
rawptr si_realloc(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_STOPIF(ptr == nil, return si_malloc(alloc, newSize));

	SI_ASSERT_FMT(
		si_between((siByte*)ptr, alloc->ptr, alloc->ptr + alloc->capacity),
		"Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
		ptr, alloc->ptr, alloc->ptr + alloc->capacity
	);
	rawptr out = si_malloc(alloc, newSize);
	si_memcopy(out, ptr, oldSize);
	return out;
}
#endif /* SI_NO_ALLOC_DEBUG_INFO */

SIDEF
usize si_assertEx(b32 condition, cstring conditionStr, cstring file, i32 line, cstring func, cstring message, ...) {
	SI_STOPIF(condition, return 0);
	siPrintColor3bit red = SI_PRINT_COLOR_3BIT(SI_ANSI_RED, true, false);
	si_fprintf(
		SI_STDERR,
		"%CAssertion \"%s\" at \"%s:%d\"%C: %s%s",
		&red, conditionStr, file, line, func, (message != nil ? ": " : "\n")
	);

	if (message != nil) {
		va_list va;
		va_start(va, message);
		si_fprintfVa(SI_STDERR, message, va);
		si_fprint(SI_STDERR, "\n");
		va_end(va);
	}

	SI_DEBUG_TRAP();
	return 1;
}

SIDEF
siUtcTime si_timeNowUTC(void) {
	time_t rawtime;
	time(&rawtime);

	return rawtime;
}
SIDEF
siLocalTime si_timeNowLocal(void) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	return (i64)ft.dwHighDateTime | ((i64)ft.dwLowDateTime << 32);
#else
	struct timeval tv;
	i32 res = gettimeofday(&tv, nil);
	SI_STOPIF(res == 1, return 0);

	return tv.tv_sec;
#endif
}

SIDEF
void si_sleep(usize miliseconds) {
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
siTimeStamp si_RDTSC(void) {
	/* NOTE(EimaMei): Shoutout to gb.h for the i386 and PPC code! (Link: https://github.com/gingerBill/gb/blob/master/gb.h#L8682C1-L8715C7). */
#if !defined(SI_NO_INLINE_ASM)
	#if SI_COMPILER_MSVC
		return __rdtsc();
	#elif SI_ARCH_I386
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


SIDEF
u64 si_clock(void) {
	return si_RDTSC() / si_cpuClockSpeed() * 1000;
}


SIDEF
siTimeStamp si_timeStampStart(void) {
	return si_RDTSC();
}
SIDEF
void si_timeStampPrintSinceEx(siTimeStamp ts, cstring filename, i32 line) {
	u64 end = si_RDTSC();
	u64 diff = (end - ts) / si_cpuClockSpeed() * 1000;

	const siBenchmarkLimit* time = si_benchmarkLimitLoop(diff);
	si_printf(
		"si_timeStampPrintSince: %s:%i: TIME: %.2f %s\n",
		filename, line, (f32)diff / (f32)time->duration, time->unit
	);
}


#endif /* SI_IMPLEMENTATION_GENERAL */

#if defined(SI_IMPLEMENTATION_MEMORY) && !defined(SI_NO_MEMORY)


inline
rawptr si_memcopy(void* restrict dst, const void* restrict src, usize size) {
	SI_ASSERT_NOT_NULL(dst);
	SI_ASSERT_NOT_NULL(src);

#if !defined(SI_NO_CRT)
	return memcpy(dst, src, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif
}

inline
rawptr si_memmove(void* dst, const void* src, isize size) {
	SI_ASSERT_NOT_NULL(dst);
	SI_ASSERT_NOT_NULL(src);

#if !defined(SI_NO_CRT)
	return memmove(dst, src, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif

}
inline
rawptr si_memset(void* data, u8 value, usize size) {
	SI_ASSERT_NOT_NULL(data);

#if !defined(SI_NO_CRT)
	return memset(data, value, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif

}

SIDEF
i32 si_memcompare(const void* ptr1, const void* ptr2, usize size) {
	SI_ASSERT_NOT_NULL(ptr1);
	SI_ASSERT_NOT_NULL(ptr2);

#if !defined(SI_NO_CRT)
	return memcmp(ptr1, ptr2, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif
}

SIDEF
rawptr si_memchr(const void* data, u8 value, usize size) {
	SI_ASSERT_NOT_NULL(data);

#if !defined(SI_NO_CRT)
	return memchr(data, value, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif
}

SIDEF
rawptr si_memrchr(const void* data, u8 value, usize size) {
	SI_ASSERT_NOT_NULL(data);

#if !defined(SI_NO_CRT)
	return memrchr(data, value, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif
}


inline
usize si_cstrLen(cstring string) {
	SI_ASSERT_NOT_NULL(string);

#if !defined(SI_NO_CRT)
	return strlen(string);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif

}



#endif /* SI_IMPLEMENTATION_GENERAL */


#if defined(SI_IMPLEMENTATION_OPTIONAL) && !defined(SI_NO_OPTIONAL)

siIntern char si__internOptionalAlloc[256];
siIntern usize si__internOptionalIndex = 0;

SIDEF
rawptr si__internOptionalMake(siAny any, usize offset) {
	SI_ASSERT(sizeof(b32) + offset <= 32);

	siByte* ptr = (siByte*)&si__internOptionalAlloc[si__internOptionalIndex % countof(si__internOptionalAlloc)];
	*(b32*)ptr = true;
	si_memcopy(&ptr[offset], any.ptr, any.typeSize);
	si__internOptionalIndex += offset + any.typeSize;

	return ptr;
}
SIDEF
rawptr si__internOptionalMakeEmptyEx(rawptr res) {
	*(b32*)res = false;
	return res;
}

#endif /* SI_IMPLEMENTATION_OPTIONAL */

#if defined(SI_IMPLEMENTATION_ARRAY) && !defined(SI_NO_ARRAY)

SIDEF
rawptr si_arrayMakeList(siAllocator* alloc, rawptr list, usize sizeofItem, usize count)  {
	rawptr array = si_arrayMakeReserve(alloc, sizeofItem, count);
	si_memcopy(array, list, sizeofItem * count);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	header->len = count;

	return array;
}

SIDEF
rawptr si_arrayCopy(siAllocator* alloc, rawptr array) {
	siArrayHeader* header = SI_ARRAY_HEADER(array);
	return si_arrayMakeList(alloc, array, header->typeSize, header->len);
}
SIDEF
rawptr si_arrayMakeReserve(siAllocator* alloc, usize sizeofItem, usize count) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_STOPIF(count == 0, count = 1);

	siByte* ptr = si_mallocArray(alloc, siByte, sizeof(siArrayHeader) + (sizeofItem * count));
	rawptr array = (rawptr)&ptr[sizeof(siArrayHeader)];

	siArrayHeader* header = (siArrayHeader*)ptr;
	header->len = 0;
	header->capacity = count;
	header->typeSize = sizeofItem;
	header->allocator = alloc;

	return array;
}


SIDEF
usize si_arrayLen(rawptr array) {
	SI_ASSERT_NOT_NULL(array);
	return SI_ARRAY_HEADER(array)->len;
}

SIDEF
usize si_arrayCapacity(rawptr array) {
	SI_ASSERT_NOT_NULL(array);
	return SI_ARRAY_HEADER(array)->capacity;
}

SIDEF
usize si_arrayTypeSize(rawptr array) {
	SI_ASSERT_NOT_NULL(array);
	return SI_ARRAY_HEADER(array)->typeSize;
}

SIDEF
usize si_arrayTotalSize(rawptr array) {
	return si_arrayCapacity(array) * si_arrayTypeSize(array);
}
SIDEF
siAllocator* si_arrayAllocator(rawptr array) {
	SI_ASSERT_NOT_NULL(array);
	return SI_ARRAY_HEADER(array)->allocator;
}

SIDEF
b32 si_arrayEmpty(siNullable rawptr array) {
	return (array == nil || si_arrayLen(array) == 0);
}


SIDEF
rawptr si_arrayAt(rawptr array, usize index) {
	SI_ASSERT_NOT_NULL(array);
	SI_STOPIF(index >= si_arrayLen(array), return nil);

	return si_arrayGetPtr(array, index);
}

SIDEF
rawptr si_arrayFront(rawptr array) {
	SI_ASSERT_NOT_NULL(array);
	return array;
}

SIDEF
rawptr si_arrayBack(rawptr array) {
	SI_ASSERT_NOT_NULL(array);

	usize len = SI_ARRAY_HEADER(array)->len;
	rawptr values[] = {si_arrayGetPtr(array, len - 1), array};
	return values[len == 0];
}

SIDEF
siOptionalRet(usize) si__internArrayFind(rawptr array, usize start, usize end,
		siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_MSG(value.typeSize <= si_arrayTypeSize(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start < end, "Value 'start' is larger than 'end'");

	for_range (i, start, end) {
		siByte* dst = si_arrayGetPtr(array, i);
		if (memcmp(dst, value.ptr, value.typeSize) == 0) {
			return si_optionalMakeEx(usize, i);
		}
	}

	return SI_OPTIONAL_NULL;
}
SIDEF
siOptionalRet(usize) si__internArrayRFind(rawptr array, usize start, usize end,
		siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_NOT_NULL(value.ptr);
	SI_ASSERT_MSG(value.typeSize <= si_arrayTypeSize(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start >= end, "Value 'end' is larger than 'start'");

	usize i;
	for (i = start; i < end - 1; i--) {
		siByte* dst = si_arrayGetPtr(array, i);
		if (memcmp(dst, value.ptr, value.typeSize) == 0) {
			return si_optionalMakeEx(usize, i);
		}
	}

	return SI_OPTIONAL_NULL;
}

SIDEF
void si__internArrayReplace(rawptr array, siAny oldValue, siAny newValue) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_NOT_NULL(oldValue.ptr);
	SI_ASSERT_NOT_NULL(newValue.ptr);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	SI_ASSERT_MSG(
		si_between(newValue.typeSize, oldValue.typeSize, header->typeSize),
		"The given value's sizeof is too large compared to the elements' in the array."
	);

	siOptional(usize) res = si_optionalMakeEx(usize, 0);
	while (true) {
		res = si__internArrayFind(array, res->value, header->len, oldValue);
		if (!res->hasValue) {
			break;
		}

		siByte* dst = si_arrayGetPtr(array, res->value);
		si_memcopy(dst, newValue.ptr, newValue.typeSize);
	}
}
SIDEF
void si_arrayReverse(rawptr arrayPtr) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = si_cast(siByte*, arrayPtr);

	usize len = si_arrayLen(array);
	usize typeSize = si_arrayTypeSize(array);
	SI_ASSERT_MSG(len != 0, "Array is empty");

	siByte* a = array;
	siByte* b = array + (len - 1) * typeSize;
	rawptr tmp[1];
	len /= 2;

	while (len -= 1) {
		si_memcopy(tmp, a, typeSize);
		si_memcopy(a, b, typeSize);
		si_memcopy(b, tmp, typeSize);

		a += typeSize;
		b -= typeSize;
	}
}
SIDEF
void si_arrayPop(rawptr arrayPtr) {
	siByte* array = *si_cast(siByte**, arrayPtr);
	SI_ARRAY_HEADER(array)->len -= 1;
}

SIDEF
void si__internArrayInsert(rawptr arrayPtr, usize index, siAny item) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	SI_ASSERT_NOT_NULL(item.ptr);

	siByte* array = *si_cast(siByte**, arrayPtr);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	usize previousLen = header->len;
	usize beforeIndexLen = previousLen - index;

	SI_ASSERT_FMT(index < previousLen, "Index is higher than the length of the array (array - '%zd', index - '%zd')", previousLen, index);
	header->len += 1;

	if (header->capacity < header->len) {
		rawptr res = si_realloc(
			header->allocator,
			header,
			sizeof(siArrayHeader) + si_arrayTotalSize(array),
			sizeof(siArrayHeader) + 2 * (header->capacity * header->typeSize)
		);

		array = (siByte*)res + sizeof(siArrayHeader);
		*si_cast(siByte**, arrayPtr) = array;

		header = (siArrayHeader*)res;
		header->capacity *= 2;
	}

	siByte* dst = si_arrayGetPtr(array, header->len - beforeIndexLen);
	siByte* src = si_arrayGetPtr(array, index);
	si_memcopy(dst, src, beforeIndexLen * header->typeSize);
	si_memcopy(src, item.ptr, item.typeSize);
}

SIDEF
void si_arrayErase(rawptr arrayPtr, usize index) {
	si_arrayEraseCount(arrayPtr, index, 1);
}
SIDEF
void si_arrayEraseCount(rawptr arrayPtr, usize index, usize count) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	usize len = si_arrayLen(array);
	SI_ASSERT_MSG(index < len, "Index is higher than the length of the string.");
	SI_ASSERT_FMT(index + count <= len, "Index + count is higher than the length of the array (%zd > %zd).", index + count, len);

	siByte* dst = si_arrayGetPtr(array, index);
	siByte* src = si_arrayGetPtr(array, index + count);
	si_memcopy(dst, src, (len - index - count) * si_arrayTypeSize(array));

	SI_ARRAY_HEADER(array)->len -= count;
}
SIDEF
void si__internArrayRemoveItem(rawptr arrayPtr, siAny value) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	SI_ASSERT_NOT_NULL(value.ptr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	SI_ASSERT_MSG(value.typeSize <= si_arrayTypeSize(array), "The given value's sizeof is too large compared to the elements' in the array.");
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	while (header->len != 0) {
		siOptional(usize) res = si__internArrayRFind(array, header->len - 1, 0, value);
		SI_STOPIF(!res->hasValue, break);

		siByte* dst = si_arrayGetPtr(array, res->value);
		siByte* src = si_arrayGetPtr(array, res->value + header->len);
		si_memcopy(dst, src, value.typeSize);

		header->len -= 1;
	}
}
SIDEF
void si__internArrayFill(rawptr arrayPtr, usize index, usize count, siAny value) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	SI_ASSERT_NOT_NULL(value.ptr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	SI_ASSERT_MSG(value.typeSize <= header->typeSize, "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(index < header->capacity, "Index is higher than the array's length.");
	SI_ASSERT_MSG(index + count <= header->capacity, "Index and count combined must be less than the current capacity.");
	usize previousLen = header->len;

	isize newItemCount = (index + count) - header->len;
	newItemCount = si_max(0, newItemCount);
	header->len += newItemCount;

	if (header->capacity < header->len) {
		rawptr res = si_realloc(
			header->allocator,
			header,
			sizeof(siArrayHeader) + si_arrayTotalSize(array),
			sizeof(siArrayHeader) + newItemCount +  2 * header->capacity * header->typeSize
		);
		SI_ASSERT_NOT_NULL(res);

		array = (siByte*)res + sizeof(siArrayHeader);
		*si_cast(siByte**, arrayPtr) = array;

		header = (siArrayHeader*)res;
		header->capacity = header->capacity * 2 + newItemCount;
	}

	for_range(i, index, index + count) {
		siByte* dst = si_arrayGetPtr(array, previousLen + i);
		si_memcopy(dst, value.ptr, header->typeSize);
	}
}

SIDEF
rawptr si__internArrayAppend(rawptr arrayPtr, siAny value) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	SI_ASSERT_NOT_NULL(value.ptr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	SI_ASSERT_MSG(
		value.typeSize <= header->typeSize,
		"The given value's sizeof is too large compared to the elements' in the array."
	);

	usize previousLen = header->len;
	header->len += 1;

	if (header->capacity < header->len) {
		rawptr res = si_realloc(
			header->allocator,
			header,
			sizeof(siArrayHeader) + si_arrayTotalSize(array),
			sizeof(siArrayHeader) + 2 * si_arrayTotalSize(array)
		);

		array = (siByte*)res + sizeof(siArrayHeader);
		*si_cast(siByte**, arrayPtr) = array;

		header = (siArrayHeader*)res;
		header->capacity *= 2;
	}

	return si_memcopy(si_arrayGetPtr(array, previousLen), value.ptr, value.typeSize);
}

SIDEF
void si_arrayClear(siArray(void) arrayPtr) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	rawptr array = si_cast(siByte*, arrayPtr);

	memset(array, 0, si_arrayTotalSize(array));
	SI_ARRAY_HEADER(array)->len = 0;
}

SIDEF
b32 si_arrayEqual(siArray(void) left, siArray(void) right) {
	SI_ASSERT_NOT_NULL(left);
	SI_ASSERT_NOT_NULL(right);

	siArrayHeader* lHeader = SI_ARRAY_HEADER(left);
	siArrayHeader* rHeader = SI_ARRAY_HEADER(right);

	if (lHeader->len != rHeader->len || lHeader->typeSize != rHeader->typeSize) {
		return false;
	}

	for_range (i, 0, lHeader->len) {
		siByte* dst = si_arrayGetPtr(left, i);
		siByte* src = si_arrayGetPtr(right, i);
		if (memcmp(dst, src, lHeader->typeSize) != 0) {
			return false;
		}
	}

	return true;
}


SIDEF
u64 si_arraySumU64(siArray(void) array) {
	SI_ASSERT_NOT_NULL(array);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	u64 res = 0;
	siByte* ptr = (siByte*)array;
	siByte* end = ptr + header->len * header->typeSize;

	while (ptr < end) {
		res += si_numFromPtr(ptr, header->typeSize);
		ptr += header->typeSize;
	}

	return res;
}

SIDEF
i64 si_arraySumI64(siArray(void) array) {
	SI_ASSERT_NOT_NULL(array);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	i64 res = 0;
	siByte* ptr = (siByte*)array;
	siByte* end = ptr + header->len * header->typeSize;

	while (ptr < end) {
		res += (i64)si_numFromPtr(ptr, header->typeSize);
		ptr += header->typeSize;
	}

	return res;
}

SIDEF
f64 si_arraySumF64(siArray(void) array) {
	SI_ASSERT_NOT_NULL(array );
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	f64 res = 0;
	siByte* ptr = (siByte*)array;
	siByte* end = ptr + header->len * header->typeSize;

	while (ptr < end) {
		res += (f64)si_numFromPtr(ptr, header->typeSize);
		ptr += header->typeSize;
	}

	return res;
}
#endif /* SI_IMPLEMENTATION_ARRAY */

#if defined(SI_IMPLEMENTATION_STRING) && !defined(SI_NO_STRING)


inline
siString si_stringMake(siString from, siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(from.data);

	siString str = SI_STR_LEN(si_mallocArray(alloc, u8, from.len), from.len);
	si_memcopy(str.data, from.data, from.len);

	return str;
}

inline
char* si_stringCloneToCstr(siString from, siAllocator* alloc) {
	return si_stringCloneToCstrEx(from, from.len, alloc);
}

inline
char* si_stringCloneToCstrEx(siString from, usize capacity, siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(from.data);

	char* str = si_mallocArray(alloc, char, capacity + 1);
	si_memcopy(str, from.data, from.len);
	str[from.len] = '\0';

	return str;
}


inline
siString si_stringMakeCstr(cstring from, siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(from);

	siString str;
	str.len = si_cstrLen(from);
	str.data = si_mallocArray(alloc, u8, str.len);
	si_memcopy(str.data, from, str.len);

	return str;
}

inline
siString si_stringMakeReserve(usize len, siAllocator* alloc) {
	return SI_STR_LEN(si_mallocArray(alloc, u8, len), len);
}

inline
siOptionalRet(char) si_stringAt(siString string, usize index) {
	SI_ASSERT_NOT_NULL(string.data);

	if (string.len > index || string.len == 0) {
		return SI_OPTIONAL_NULL;
	}

	return si_optionalMakeEx(char, string.data[index]);
}
SIDEF
siOptionalRet(char) si_stringAtFront(siString string) {
	SI_ASSERT_NOT_NULL(string.data);

	if (string.len == 0) {
		return SI_OPTIONAL_NULL;
	}

	return si_optionalMakeEx(char, *string.data);
}

SIDEF
siOptionalRet(char) si_stringAtBack(siString string) {
	SI_ASSERT_NOT_NULL(string.data);

	if (string.len == 0) {
		return SI_OPTIONAL_NULL;
	}

	return si_optionalMakeEx(char, string.data[string.len]);
}


inline
u8* si_stringFront(siString string) {
	SI_ASSERT_NOT_NULL(string.data);

	return &string.data[0];
}

inline
u8* si_stringBack(siString string) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_STOPIF(string.len == 0, return string.data);

	return &string.data[string.len - 1];
}


SIDEF
isize si_stringFind(siString string, siString subStr) {
	usize counter = 0;
	for_eachStr (letter, string) {
		if (*letter != subStr.data[counter]) {
			counter = 0;
			continue;
		}

		counter += 1;
		if (counter == subStr.len) {
			usize index = letter - string.data;
			return index - (subStr.len - 1);
		}
	}

	return -1;
}

inline
isize si_stringFindByte(siString string, siByte byte) {
	for_eachStr (letter, string) {
		if (*letter == byte) {
			return letter - string.data;
		}
	}

	return -1;
}

SIDEF
isize si_stringFindUtf8(siString string, char character[4], isize* utf8AtIndex) {
	usize i = 0;
	isize utf8I = 0;
	i32 codepoint = si_utf8Decode(character).codepoint;

	while (i < string.len) {
		siUtf32Char x = si_utf8Decode((char*)&string.data[i]);
		if (x.codepoint == codepoint) {
			*utf8AtIndex = utf8I;
			return i;
		}

		i += x.len, utf8I += 1;
	}

	*utf8AtIndex = -1;
	return -1;

}


SIDEF
isize si_stringFindLast(siString string, siString subStr) {
	SI_ASSERT_NOT_NULL(string.data);

	usize subStrEnd = subStr.len - 1;
	usize counter = subStrEnd;

	for_eachRevStr (letter, string) {
		if (*letter != subStr.data[counter]) {
			counter = subStrEnd;
			continue;
		}

		counter -= 1;
		if (counter == 0) {
			return letter - string.data;
		}
	}

	return -1;
}


inline
isize si_stringFindLastByte(siString string, siByte byte) {
	for_eachRevStr (letter, string) {
		if (*letter == byte) {
			return letter - string.data;
		}
	}

	return -1;
}

SIDEF
usize si_stringFindCount(siString string, siString subStr) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_STOPIF(subStr.len == 0, return 0);

	usize occurences = 0;
	usize counter = 0;
	for_eachStr (letter, string) {
		if (*letter != subStr.data[counter]) {
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
	return si_memcompare(lhs.data, rhs.data, si_min(lhs.len, rhs.len));
}

inline
b32 si_stringEqual(siString lhs, siString rhs) {
	SI_STOPIF(lhs.len != rhs.len, return false);
	SI_STOPIF(lhs.data == rhs.data, return true);

	return si_stringCompare(lhs, rhs) == 0;
}

inline
void si_stringSet(siString* string, siString str, siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(string);

	if (string->len < str.len) {
		si_stringMakeSpaceFor(string, alloc, str.len - string->len);
	}

	si_memcopy(string->data, str.data, str.len);
	string->len = str.len;
}

SIDEF
siString si_stringJoin(siString* stringArr, usize len, siString separator,
		siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(stringArr);
	SI_STOPIF(len == 0, return SI_STR_LEN(nil, 0));

	usize length = separator.len * (len - 1);
	for_range (i, 0, len) {
		length += stringArr[i].len;
	}

	siString res = si_stringMakeReserve(length, alloc);

	usize i = stringArr[0].len;
	memcpy(res.data, stringArr[0].data, stringArr[0].len);
	for_range (j, 1, len) {
		siString str = stringArr[j];
		memcpy(&res.data[i], separator.data, separator.len), i += separator.len;
		memcpy(&res.data[i], str.data, str.len), i += str.len;
	}

	return res;
}

inline
b32 si_stringReplaceAll(siString* string, siString oldStr, siString newStr,
		siAllocator* alloc) {
	return si_stringReplace(string, oldStr, newStr, -1, alloc);
}
SIDEF
b32 si_stringReplace(siString* string, siString oldStr, siString newStr, isize amount,
		siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(string);
	SI_ASSERT_NOT_NULL(newStr.data);

	if (oldStr.data == newStr.data) {
		return false;
	}
	if (amount < 0) {
		amount = si_stringFindCount(*string, oldStr);
	}
	if (amount == 0) {
		return false;
	}

	isize dif = newStr.len - oldStr.len;
	siString res = si_stringMakeReserve(string->len + amount * dif, alloc);
	usize lineStart = 0, indexRes = 0;

	while (amount) {
		siString substr = si_stringSubToEnd(*string, lineStart);
		isize len = si_stringFind(substr, oldStr);

		si_memcopy(&res.data[indexRes], substr.data, len), indexRes += len;
		si_memcopy(&res.data[indexRes], newStr.data, newStr.len), indexRes += newStr.len;

		lineStart += len + oldStr.len;

		amount -= 1;
	}

	si_memcopy(&res.data[indexRes], &string->data[lineStart], string->len - lineStart);

	*string = res;
	return true;
}

SIDEF
void si_stringTrimRight(siString* string, siString cutSet) {
	SI_ASSERT_NOT_NULL(string);

	u8* posEnd = &string->data[string->len];
	while (posEnd > string->data && si_memchr(cutSet.data, *posEnd, cutSet.len)) {
		posEnd -= 1;
	}

	string->len = (posEnd - string->data) + 1;
}


SIDEF
void si_stringTrimLeft(siString* string, siString cutSet) {
	SI_ASSERT_NOT_NULL(string);

	u8* posStart = string->data;
	u8* posEnd = &string->data[string->len];

	while (posStart < posEnd && si_memchr(cutSet.data, *posStart, cutSet.len)) {
		posStart += 1;
	}

	string->len = (posEnd - posStart) + 1;

	if (string->data != posStart) {
		si_memmove(string->data, posStart, string->len);
	}
}

inline
void si_stringTrim(siString* string, siString cutSet) {
	si_stringTrimRight(string, cutSet);
	si_stringTrimLeft(string, cutSet);
}

SIDEF
void si_stringEnquote(siString* string, siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(string);
	SI_ASSERT_NOT_NULL(string->data);

	usize oldLen = string->len;
	si_stringMakeSpaceFor(string, alloc, 2);

	si_ptrMoveRight(&string->data[0], oldLen, 1);
	string->data[0] = '\"';
	string->data[string->len - 1] = '\"';
}


SIDEF
void si_stringAppend(siString* string, siString other, siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(string);
	SI_ASSERT_NOT_NULL(other.data);

	usize oldLen = string->len;
	si_stringMakeSpaceFor(string, alloc, other.len);
	si_memcopy(&string->data[oldLen], other.data, other.len);
}

inline
void si_stringPush(siString* string, char other, siAllocator* alloc) {
	si_stringAppend(string, SI_STR_LEN(&other, 1), alloc);
}

inline
void si_stringPop(siString* string) {
	SI_ASSERT_NOT_NULL(string);
	string->len -= (string->len != 0);
}

#if 0
SIDEF
void si_stringInsert(siString* string, cstring cstr, usize index) {
	si_stringInsertEx(str, cstr, si_cstrLen(cstr), index, false);
}

SIDEF
void si_stringInsertLen(siString* string, cstring cstr, usize cstrLen, usize index) {
	si_stringInsertEx(str, cstr, cstrLen, index, false);
}
SIDEF
void si_stringInsertEx(siString* string, cstring cstr, usize cstrLen, usize index, b32 eraseIndex) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize previousLen = header->len;
	usize beforeIndexLen = previousLen - index;

	SI_ASSERT_FMT(index < previousLen, "Index is higher than the length of the string (strLen - '%zd', index - '%zd')", previousLen, index);
	header->len += cstrLen - eraseIndex;

	if (header->capacity < header->len) {
		si_stringMakeSpaceFor(str, header->len - header->capacity);
		header = SI_STRING_HEADER(*str);
	}
	siString curStr = *str;

	char* ptr = (char*)si_memcopy(curStr + header->len - beforeIndexLen, curStr + index, beforeIndexLen);
	si_memcopy(curStr + index, cstr, cstrLen);
	ptr[beforeIndexLen] = '\0';
}
SIDEF
void si_stringErase(siString string, usize index, usize eraseLen) {
	SI_ASSERT_NOT_NULL(str);

	usize strLen = si_stringLen(str);
	SI_ASSERT_FMT(index < strLen, "Index is higher than the length of the string. (strLen - '%zd', index - '%zd')", strLen, index);

	usize afterIndexLen = index + eraseLen;

	char* ptr = (char*)si_memcopy(
		str + index,
		str + afterIndexLen,
		strLen - afterIndexLen
	);
	ptr[strLen - afterIndexLen] = '\0';

	SI_STRING_HEADER(str)->len -= eraseLen;
}
SIDEF
void si_stringRemoveCstr(siString string, cstring cstr) {
	SI_ASSERT_NOT_NULL(str);

	siStringHeader* header = SI_STRING_HEADER(str);
	usize cstrLen = si_cstrLen(cstr);
	while (true) {
		isize index = si_stringRFindEx(str, header->len - 1, 0, cstr, cstrLen);
		SI_STOPIF(index == -1, break);

		usize afterIndexLen = index + cstrLen;

		char* ptr = (char*)si_memcopy(
			str + index,
			str + afterIndexLen,
			header->len - afterIndexLen
		);
		ptr[header->len - afterIndexLen] = '\0';

		header->len -= cstrLen;
	}
}
#endif

inline
void si_stringStrip(siString* string) {
	si_stringTrim(string, SI_STR(" \t\n\v\f\r "));
}
SIDEF
void si_stringReverse(siString string) {
	SI_STOPIF(string.len == 0, return);

	u8* a = &string.data[0];
	u8* b = &string.data[string.len];

	usize len = string.len / 2;
	while (len) {
		si_swap(*a, *b);
		a += 1;
		b -= 1;

		len -= 1;
	}
}

inline
siArray(siString) si_stringSplit(siString string, siString delimiter, siAllocator* alloc) {
	return si_stringSplitEx(string, delimiter, -1, alloc);
}

SIDEF
siArray(siString) si_stringSplitEx(siString string, siString delimiter, isize amount,
		siAllocator* alloc) {
	if (amount < 0) {
		amount = si_stringFindCount(string, delimiter);
	}
	SI_STOPIF(amount == 0, return nil);


	siArray(siString) res = si_arrayMakeReserve(alloc, sizeof(*res), amount + 1);
	SI_ARRAY_HEADER(res)->len = amount + 1;

	isize lineStart = 0;
	for_eachArr (siString, x, res) {
		siString substr = si_stringSubToEnd(string, lineStart);
		isize len = si_stringFind(substr, delimiter);

		*x = SI_STR_LEN(substr.data, len);
		lineStart += len + delimiter.len;
	}
	res[amount] = si_stringSubToEnd(string, lineStart);

	return res;
}

inline
void si_stringClear(siString* string) {
	SI_ASSERT_NOT_NULL(string);
	string->len = 0;
}

SIDEF
void si_stringMakeSpaceFor(siString* string, siAllocator* alloc, usize addLen) {
	SI_ASSERT_NOT_NULL(string);
	SI_ASSERT_NOT_NULL(string->data);
	SI_ASSERT_NOT_NULL(alloc);


	if (si_betweenPtr(string->data, alloc->ptr, alloc->ptr + alloc->capacity)
		&& (uintptr)string->data % SI_DEFAULT_MEMORY_ALIGNMENT == 0) {
		usize remainingLen = si_alignCeil(string->len) - string->len;

		if (addLen <= remainingLen) {
			string->len += addLen;
			return;
		}
	}

	usize sizeNew = 2 * string->len + addLen;
	u8* newData = si_mallocArray(alloc, u8, sizeNew);

	si_memcopy(newData, string->data, string->len);
	string->data = newData;
	string->len = sizeNew;
}


#if defined(SI_IMPLEMENTATION_MEMORY)

inline rawptr si_memcopyStr(void* dst, siString src) { return si_memcopy(dst, src.data, src.len); }
inline i32 si_memcompareStr(const void* dst, siString src) { return si_memcompare(dst, src.data, src.len); }

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
	b32 change = true;
	for_eachStr (x, string) {
		if (si_charIsSpace(*x)) {
			change = true;
		}
		else if (change) {
			*x = si_charUpper(*x);
			change = false;
		}
	}
}

inline
void si_stringCapitalize(siString string) {
	si_stringLower(string);
	string.data[0] = si_charUpper(string.data[0]);
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
siString si_stringFromUInt(u64 num, siAllocator* alloc) {
	return si_stringFromUIntEx(num, 10, alloc);
}
SIDEF
siString si_stringFromUIntEx(u64 num, i32 base, siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(alloc);

	usize len = si_numLenEx(num, base);
	siString res = si_stringMakeReserve(len, alloc);

	/* NOTE(EimaMei): We build the string from the back (not the front) so that
	 * we wouldn't have to reverse the string after we make the string. */
	u8* back = &res.data[len];

	do {
		back -= 1;
		*back = SI_NUM_TO_CHAR_TABLE[num % base];
		num /= base;
	} while (num > 0);

	return res;
}
inline
u64 si_stringToUInt(siString string) {
	SI_ASSERT(string.len <= 20);
	u64 res = 0;
	for_eachStr (x, string) {
		res *= 10;
		res += (*x - '0');
	}

	return res;
}
SIDEF
u64 si_stringToUIntEx(siString string, b32* outRes) {
	SI_ASSERT_NOT_NULL(outRes);

	/* NOTE(EimaMei): No base specified, meaning it's base-10. */
	if (string.data[0] != '0') {
		if (string.len > 20) {
			*outRes = false;
			return 0;
		}
		*outRes = true;
		return si_stringToUInt(string);
	}

	u64 res = 0;
	i32 base;

	char x = si_charUpperUnsafe(string.data[1]);
	u32 maxDigits;
	switch (x) {
		case 'X': base = 16; string = si_stringSubToEnd(string, 2); maxDigits = 16; break;
		case 'O': base =  8; string = si_stringSubToEnd(string, 2); maxDigits = 22; break;
		case 'B': base =  2; string = si_stringSubToEnd(string, 2); maxDigits = 64; break;
		default:  base =  8; string = si_stringSubToEnd(string, 1); maxDigits = 22; break;
	}
	SI_STOPIF(string.len > maxDigits, *outRes = false; return res);

	for_eachStr (x, string) {
		res *= base;

		u8 value = *x - '0';
		if (value < 10) {
			res += value;
		}
		else {
			value = si_charUpper(value) - 7;
			if (value < base) {
				res += value;
			}
			else {
				*outRes = false;
				return res;
			}
		}
	}

	*outRes = true;
	return res;
}

inline
siString si_stringFromInt(i64 num, siAllocator* alloc) {
	return si_stringFromIntEx(num, 10, alloc);
}
SIDEF
siString si_stringFromIntEx(i64 num, i32 base, siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(alloc);

	b32 isNegative = si_numIsNeg(num);
	usize len = si_numLenI64Ex(num, base);
	siString res = si_stringMakeReserve(len, alloc);

	/* NOTE(EimaMei): We build the string from the back (not the front) so that
	 * we wouldn't have to reverse the string after we make the string. */
	u8* back = &res.data[len];

	if (isNegative) {
		num = -num;
		res.data[0] = '-';
	}

	do {
		back -= 1;
		*back = SI_NUM_TO_CHAR_TABLE[num % base];
		num /= base;
	} while (num > 0);

	return res;
}
inline
siString si_stringFromFloat(f64 num, siAllocator* alloc) {
	return si_stringFromFloatEx(num, 10, 6, alloc);
}
SIDEF
siString si_stringFromFloatEx(f64 num, i32 base, u32 afterPoint, siAllocator* alloc) {
	SI_ASSERT_NOT_NULL(alloc);


	b32 isNegative;
	{
		/* NOTE(EimaMei): For a quick 'n dirty way of checking if the specified
		 * float are special numbers (for this Nan and infinity), we just union
		 * cast f64 value into an u64 integer and for a regular comparison. Note
		 * that later in the code the sign bit gets always set to zero, meaning
		 * we only need to do 2 checks instead of 4 for each signed version. */
		static u64 infinity = 0x7FF0000000000000;
		static u64 nanV = 0x7FF8000000000000;

		union { f64 f; u64 n; } check = {num};
		isNegative = (check.n & SI_ENDIAN_VALUE(SI_BIT(63), SI_BIT(0))) != 0;
	   	check.n &= ~SI_ENDIAN_VALUE(SI_BIT(63), SI_BIT(0)); /* NOTE(EimaMei): A quick way of changing the minus to plus. */
		num = check.f;

		if (check.n == infinity) {
			return (!isNegative)
				? SI_STR("inf")
				: SI_STR("-inf");
		}
		else if (check.n == nanV) {
			return SI_STR("nan");
		}
	}

	isize baseLen = 0;
	f64 numWhole = (afterPoint != 0)
		? num
		: si_round(num);
	do {
		numWhole /= base;
		baseLen += 1;
	} while (numWhole > 0.9999999999999999f); /* NOTE(EimaMei): How long can this be?? */


	usize len = isNegative + baseLen + (afterPoint != 0) + afterPoint;
	siString res = si_stringMakeReserve(len, alloc);
	u8* ptr = &res.data[0];

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

	f64 numFractional = num - si_floor(num);
	while (afterPoint) {
		numFractional *= base;

		u32 num = (u32)numFractional;
		*ptr = SI_NUM_TO_CHAR_TABLE[num % base];
		ptr += 1;

		afterPoint -= 1;
		numFractional -= (f64)num;
	}

	return res;
}


inline
siString si_stringFromBool(b32 boolean) {
	static siString values[] = {SI_STR("false"), SI_STR("true")};
	return values[boolean & true];
}

b32 si_stringToBool(siString string) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_STOPIF(string.len == 0 || (string.len != 1 && string.len != 4 && string.len != 5), return UINT32_MAX);

	if (string.len == 1) {
		switch (si_charLowerUnsafe(string.data[0])) {
			case '1': case 't': return true;
			case '0': case 'f': return false;
			default: return UINT32_MAX;
		}
	}

	char str[5];
	for_eachStr (x, string) {
		str[x - string.data] = si_charLowerUnsafe(*x);
	}

	u32 val = SI_TO_U32(str);
	if (val == SI_TO_U32("true") && string.len == 4) {
		return true;
	}
	else if (val == SI_TO_U32("fals") && str[4] == 'e') {
		return false;
	}

	return UINT32_MAX;
}

inline
i64 si_stringToInt(siString string) {
	SI_ASSERT_NOT_NULL(string.data);

	switch (string.data[0]) {
		case '-':
			string.data += 1;
			string.len -= 1;

			return -si_stringToUInt(string);
		case '+':
			string.data += 1;
			string.len  -= 1;
			break;
	}

	return si_stringToUInt(string);
}
SIDEF
i64 si_stringToIntEx(siString string, b32* outRes) {
	SI_ASSERT_NOT_NULL(string.data);

	switch (string.data[0]) {
		case '-':
			string.data += 1;
			string.len -= 1;

			return -si_stringToUIntEx(string, outRes);
		case '+':
			string.data += 1;
			string.len  -= 1;
			break;
	}

	return si_stringToUIntEx(string, outRes);
}

#endif

#if defined(SI_IMPLEMENTATION_UNICODE) && !defined(SI_NO_UNICODE)

SIDEF
siUtf32Char si_utf8Decode(const char character[4]) {
	#define FAILURE 12

	u32 state = 0, codepoint;
	u8* next = (u8*)character;

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

	u32 len = si_cast(u32, (const char*)next - character);
	siUtf32Char values[2] = {
		{codepoint, len},
		{SI_UNICODE_INVALID, 0}
	};

	return values[state];
	#undef FAILURE
}
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
usize si_utf8StrLen(siUtf8String str) {
	SI_ASSERT_NOT_NULL(str);

	usize offset = 0;
	usize len = 0;
	while (str[offset] != '\0') {
		offset += si_utf8Decode(&str[offset]).len;
		len += 1;
	}

	return len;
}


SIDEF
siUtf32Char si_utf8StrAt(siUtf8String str, usize charIndex) {
	SI_ASSERT_NOT_NULL(str);

	siUtf32Char character = si_utf8Decode(str);
	usize offset = character.len;

	while (charIndex) {
		character = si_utf8Decode(&str[offset]);
		offset = character.len;
		charIndex -= 1;
	}

	return character;
}


SIDEF
siUtf16String si_utf8ToUtf16Str(siAllocator* alloc, siUtf8String str, usize* strLenOut) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(str);

	siUtf8String utf8Ptr = str;
	siUtf16String res = (siUtf16String)si_allocatorCurPtr(*alloc);
	u16* curPos = (u16*)res;

	while (true) {
		siUtf32Char utf32 = si_utf8Decode(utf8Ptr);
		i32 codepoint = utf32.codepoint;
		SI_ASSERT_MSG(codepoint != SI_UNICODE_INVALID, "Invalid UTF-8 character.");

		SI_STOPIF(codepoint == '\0', break);
		utf8Ptr += utf32.len;

		if (codepoint < 0xFFFF) {
			alloc->offset += 2;
			SI_STOPIF(alloc->offset >= alloc->capacity, goto end);

			*curPos = (u16)codepoint;
			curPos += 1;
		}
		else {
			alloc->offset += 4;
			SI_STOPIF(alloc->offset >= alloc->capacity, goto end);

			u16 t = si_cast(u16, codepoint - 0x10000);
			u16 high = si_cast(u16, (t << 10) + 0xD800);
			u16 low = t + 0xDC00;

			curPos[0] = high;
			curPos[1] = low;
			curPos += 2;
		}
	}
end:
	*curPos = '\0';
	usize len = (char*)curPos - (char*)res;
	alloc->offset += len + 1;

	SI_STOPIF(strLenOut != nil, *strLenOut = len);
	return res;
}
siUtf16String si_utf8ToUtf16StrEx(siAllocator* alloc, siUtf8String str, usize strLen,
		usize* strLenOut) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(str);

	siUtf8String utf8Ptr = str;
	siUtf16String res = (siUtf16String)si_allocatorCurPtr(*alloc);
	u16* curPos = (u16*)res;

	while (strLen) {
		siUtf32Char utf32 = si_utf8Decode(utf8Ptr);
		i32 codepoint = utf32.codepoint;
		SI_ASSERT_MSG(codepoint != SI_UNICODE_INVALID, "Invalid UTF-8 character.");

		SI_STOPIF(codepoint == '\0', break);
		utf8Ptr += utf32.len;

		if (codepoint < 0xFFFF) {
			alloc->offset += 2;
			SI_STOPIF(alloc->offset >= alloc->capacity, goto end);

			*curPos = (u16)codepoint;
			curPos += 1;
		}
		else {
			alloc->offset += 4;
			SI_STOPIF(alloc->offset >= alloc->capacity, goto end);

			u16 t = si_cast(u16, codepoint - 0x10000);
			u16 high = si_cast(u16, (t << 10) + 0xD800);
			u16 low = t + 0xDC00;

			curPos[0] = high;
			curPos[1] = low;
			curPos += 2;
		}

		strLen -= 1;
	}
end:
	*curPos = '\0';
	usize len = (char*)curPos - (char*)res;
	alloc->offset += len + 1;

	SI_STOPIF(strLenOut != nil, *strLenOut = len);
	return res;
}


SIDEF
siUtf8String si_utf16ToUtf8Str(siAllocator* alloc, siUtf16String str, usize* strLenOut) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(str);

	siUtf8String res = (siUtf8String)si_allocatorCurPtr(*alloc);
	char* curPtr = (char*)res;

	while (true) {
		u32 chr = *str;
		str += 1;
		SI_STOPIF(chr == '\0', break);

		if (chr <= 0xFF) {
			alloc->offset += 1;
			SI_STOPIF(alloc->offset >= alloc->capacity, goto end);

			*curPtr = (u8)chr;
			curPtr += 1;
		}
		else if (chr <= 0x7FF) {
			alloc->offset += 2;
			SI_STOPIF(alloc->offset >= alloc->capacity, goto end);

			curPtr[0] = si_cast(u8, 0xC0 | (chr >> 6));            /* 110xxxxx */
			curPtr[1] = si_cast(u8, 0x80 | (chr & 0x3F));          /* 10xxxxxx */
			curPtr += 2;
		}
		else if (chr <= 0xD7FF) {
			alloc->offset += 3;
			SI_STOPIF(alloc->offset >= alloc->capacity, goto end);

			curPtr[0] = si_cast(u8, 0xE0 | (chr >> 12));           /* 1110xxxx */
			curPtr[1] = si_cast(u8, 0xE0 | ((chr >> 6) & 0x3F));   /* 10xxxxxx */
			curPtr[2] = si_cast(u8, 0xE0 | (chr & 0x3F));          /* 10xxxxxx */

			curPtr += 3;
		}
		else if (chr >= 0xD800) {
			u16 high = (u16)chr;
			u16 low = *str;
			str += 1;

			alloc->offset += 4;
			SI_STOPIF(alloc->offset >= alloc->capacity, goto end);

			u32 tHigh = (high - 0xD800) << 10;
			u32 tLow = (low - 0xDC00);
			u32 codepoint = (tHigh | tLow) + 0x10000;

			curPtr[0] = si_cast(u8, 0xF0 | (codepoint >> 18));           /* 11110xxx */
			curPtr[1] = si_cast(u8, 0x80 | ((codepoint >> 12) & 0x3F));  /* 10xxxxxx */
			curPtr[2] = si_cast(u8, 0x80 | ((codepoint >> 6) & 0x3F));   /* 10xxxxxx */
			curPtr[3] = si_cast(u8, 0x80 | (codepoint & 0x3F));          /* 10xxxxxx */

			curPtr += 4;
		}
	}
end:
	*curPtr = '\0';

	usize len = curPtr - res;
	alloc->offset += len + 1;

	SI_STOPIF(strLenOut != nil, *strLenOut = len);

	return res;
}



SIDEF
siUtf32Char si_utf16Decode(const u16 character[2]) {
	siUtf32Char utf32;
	u32 chr = character[0];

	if (si_betweenu(chr, 0xD800, 0xDBFF)) {
		u32 high = chr;
		u32 low = character[1];

		utf32.codepoint = ((high - 0xD800) << 10) + (low - 0xDC00) + 0x10000;
		utf32.len = 3 + (utf32.codepoint >= 0x10000);
	}
	else {
		utf32.codepoint = chr;
		utf32.len = (chr >= 0x80) + (chr >= 0x800);
	}

	return utf32;
}

SIDEF
siUtf8Char si_utf16Encode(const u16 character[2]) {
	siUtf32Char utf32 = si_utf16Decode(character);
	return si_utf8Encode(utf32.codepoint);
}
#endif

#if defined(SI_IMPLEMENTATION_CHAR) && !defined(SI_NO_CHAR)


inline
char si_charLower(char c) {
	if (c >= 'A' && c <= 'Z') {
		return c | SI_BIT(5);
	}
	return c;
}
inline
char si_charLowerUnsafe(char c) {
	return c | SI_BIT(5);
}

inline
char si_charUpper(char c) {
	if (c >= 'a' && c <= 'z') {
		return c & ~SI_BIT(5);
	}
	return c;
}
inline
char si_charUpperUnsafe(char c) {
	return c & ~SI_BIT(5);
}


inline
b32 si_charIsSpace(char c) {
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

inline
b32 si_charIsDigit(char c) {
	return (c >= '0' && c <= '9');
}

inline
b32 si_charIsHexDigit(char c) {
	return (si_charIsDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

inline
b32 si_charIsAlpha(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

inline
b32 si_charIsAlphanumeric(char c) {
	return si_charIsAlpha(c) || si_charIsDigit(c);
}

inline
i32 si_charDigitToInt(char c) {
	SI_ASSERT_MSG(si_charIsDigit(c), "Character is not a digit.");
	return (c - '0');
}
SIDEF
i32 si_charHexDigitToInt(char c) {
	if (si_charIsDigit(c)) {
		return si_charDigitToInt(c);
	}
	else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}

	return -1;
}
#endif

#if defined(SI_IMPLEMENTATION_HASHTABLE) && !defined(SI_NO_HASHTABLE)

force_inline
u64 si__hashKey(siByte* key, usize len);

#if !defined(SI_DEFINE_CUSTOM_HASH_FUNCTION)
#define SI__FNV_OFFSET 14695981039346656037UL
#define SI__FNV_PRIME 1099511628211UL

force_inline
u64 si__hashKey(siByte* key, usize len) {
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




siHashTable si_hashtableMake(siAllocator* alloc, const rawptr* keyArray, usize keyLen,
		const rawptr* dataArray, usize len) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(keyArray);
	SI_ASSERT_NOT_NULL(dataArray);
	SI_ASSERT(keyLen != 0);

	siHashTable table = si_arrayMakeReserve(alloc, sizeof(siHashEntry), len);

	siArrayHeader* arrayHeader = SI_ARRAY_HEADER(table);
	arrayHeader->len = len;

	for_range (i, 0, len) {
		si_hashtableSet(table, keyArray[i], keyLen, dataArray[i], nil);
	}

	return table;
}

SIDEF
siHashTable si_hashtableMakeReserve(siAllocator* alloc, usize capacity) {
	SI_ASSERT(capacity != 0);
	/* NOTE(EimaMei): A power of 2 capacity is enforced so that truncating a hash
	 * into a valid index would only require a 'hash & (capacity - 1)', which
	 * basically takes no time in comparison to the usual modulo operator. */
	SI_ASSERT_MSG((capacity & (capacity - 1)) == 0, "The specified capacity must be a power of 2 number (8, 64, 512, 1024, etc).");

	siHashTable table = si_arrayMakeReserve(alloc, sizeof(siHashEntry), capacity);
	siHashEntry entry;
	entry.key = 0;
	entry.value = nil;
	for_range (i, 0, capacity - 1) {
		entry.next = 0;
		table[i] = entry;
	}
	entry.next = 0;
	table[capacity] = entry;

	return table;
}

SIDEF
rawptr si_hashtableGet(const siHashTable ht, rawptr key, usize keyLen) {
	SI_ASSERT_NOT_NULL(key);
	SI_ASSERT(keyLen != 0);
	SI_STOPIF(si_arrayLen(ht) == 0, return nil);

	u64 hash = si__hashKey(key, keyLen);
	return si_hashtableGetWithHash(ht, hash);
}
SIDEF
rawptr si_hashtableGetWithHash(const siHashTable ht, u64 hash) {
	SI_ASSERT_NOT_NULL(ht);

	siArrayHeader* header = SI_ARRAY_HEADER(ht);
	usize index = hash & (header->capacity - 1);

	siHashEntry* entry = &ht[index];
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
siHashEntry* si_hashtableSet(siHashTable ht, const rawptr key, usize keyLen,
		const rawptr valuePtr, b32* outSuccess) {
	SI_ASSERT_NOT_NULL(ht);
	SI_ASSERT_NOT_NULL(key);
	SI_ASSERT(keyLen != 0);

	u64 hash = si__hashKey(key, keyLen);
	return si_hashtableSetWithHash(ht, hash, valuePtr, outSuccess);
}
SIDEF
siHashEntry* si_hashtableSetWithHash(const siHashTable ht, u64 hash, const rawptr valuePtr,
		b32* outSuccess) {
	SI_ASSERT_NOT_NULL(ht);

	siArrayHeader* header = SI_ARRAY_HEADER(ht);
	SI_ASSERT_MSG(header->len < header->capacity, "The capacity of the hashtable has been surpassed.");

	usize index = hash & (header->capacity - 1);
	siHashEntry* entry = &ht[index];
	siHashEntry* original = entry;

	while (entry->key != 0) {
		if (hash == entry->key) {
			SI_STOPIF(outSuccess != nil, *outSuccess = false);
#if !defined(SI_NO_HASH_OVERWRITE)
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

	header->len += 1;
	return entry;
}

#endif

#if defined(SI_IMPLEMENTATION_IO) && !defined(SI_NO_IO)

siErrorInfo SI_FS_ERROR = {0};


SIDEF
cstring si_pathFsErrorName(siFileSystemError err) {
	switch (err) {
		case SI_FS_ERROR_NONE: return "SI_FS_ERROR_NONE";
		case SI_FS_ERROR_EXISTS: return "SI_FS_ERROR_EXISTS";
		case SI_FS_ERROR_INVALID: return "SI_FS_ERROR_INVALID";
		case SI_FS_ERROR_INVALID_FILENAME: return "SI_FS_ERROR_INVALID_FILENAME";
		case SI_FS_ERROR_DOESNT_EXIST: return "SI_FS_ERROR_DOESNT_EXIST";
		case SI_FS_ERROR_PERMISSION: return "SI_FS_ERROR_PERMISSION";
		case SI_FS_ERROR_TRUNCATE_FAILURE: return "SI_FS_ERROR_TRUNCATE_FAILURE";
	}
	return nil;
}


SIDEF
cstring si_pathFsErrorStr(siFileSystemError err) {
	switch (err) {
		case SI_FS_ERROR_NONE: return "No error has been encountered.";
		case SI_FS_ERROR_EXISTS: return "File or directory already exists.";
		case SI_FS_ERROR_INVALID: return "Provided value is invalid.";
		case SI_FS_ERROR_INVALID_FILENAME: return "Invalid filename.";
		case SI_FS_ERROR_DOESNT_EXIST: return "File or directory doesn't exist.";
		case SI_FS_ERROR_PERMISSION: return "User doesn't have permission to read/write.";
		case SI_FS_ERROR_TRUNCATE_FAILURE: return "Failed to truncate the file.";
	}
	return nil;
}
#if !defined(SI_NO_ERROR_STRUCT)
	#define SI_FS_ERROR_DECLARE_EX(err) \
		{ \
			SI_FS_ERROR.code = err; \
			SI_FS_ERROR.function = __func__; \
			SI_FS_ERROR.time = si_timeNowUTC(); \
			SI_FS_ERROR_LOG(); \
		}
	#define SI_FS_ERROR_DECLARE() \
		SI_FS_ERROR_DECLARE_EX(si__internfileGetOSError())

	#if !defined(SI_NO_FS_ERROR_PRINTS)
		#define SI_FS_ERROR_LOG() \
			si_fprintf( \
				SI_STDERR, \
				"%CRFile system error at \"%s\"%C: %s: %s (err number '%i')\n", \
				SI_FS_ERROR.function, si_pathFsErrorName(SI_FS_ERROR.code), \
				si_pathFsErrorStr(SI_FS_ERROR.code), SI_FS_ERROR.code \
			)
	#else
		#define SI_FS_ERROR_LOG() do {} while (0)
	#endif
#else
	#define SI_FS_ERROR_DECLARE_EX(err) \
		{ \
			SI_FS_ERROR.code = err; \
			SI_FS_ERROR_LOG(); \
		}
	#define SI_FS_ERROR_DECLARE() \
		SI_FS_ERROR_DECLARE_EX(si__internfileGetOSError())

	#if !defined(SI_NO_FS_ERROR_PRINTS)
		#define SI_FS_ERROR_LOG() \
			si_fprintf( \
				SI_STDERR, \
				"%CRFile system error%C: %s: %s (errno '%i')\n", \
				si_pathFsErrorName(SI_FS_ERROR.code), si_pathFsErrorStr(SI_FS_ERROR.code), \
				SI_FS_ERROR \
			)
	#else
		#define SI_FS_ERROR_LOG() do {} while (0)
	#endif
#endif
siIntern inline
siFileSystemError si__internfileGetOSError(void) {
#if SI_SYSTEM_IS_WINDOWS
	switch (GetLastError()) {
		case ERROR_SUCCESS: return SI_FS_ERROR_NONE;
		case ERROR_ALREADY_EXISTS:
		case ERROR_FILE_EXISTS: return SI_FS_ERROR_EXISTS;
		case ERROR_INVALID_NAME:
		case ERROR_BAD_PATHNAME: return SI_FS_ERROR_INVALID_FILENAME;
		case ERROR_DIRECTORY:
		case ERROR_INVALID_ADDRESS: return SI_FS_ERROR_INVALID;
		case ERROR_PATH_NOT_FOUND:
		case ERROR_FILE_NOT_FOUND: return SI_FS_ERROR_DOESNT_EXIST;
		case ERROR_ACCESS_DENIED: return SI_FS_ERROR_DOESNT_EXIST;
	}
	return SI_FS_ERROR_INVALID;
#else
	switch (errno) {
		case EEXIST: return SI_FS_ERROR_EXISTS;
		case EINVAL: return SI_FS_ERROR_INVALID;
		case EISDIR:
		case ENAMETOOLONG: return SI_FS_ERROR_INVALID_FILENAME;
		case ENOENT: return SI_FS_ERROR_DOESNT_EXIST;
		case EPERM:
		case EACCES: return SI_FS_ERROR_PERMISSION;
	}

	return SI_FS_ERROR_NONE;
#endif
}


SIDEF
b32 si_pathExists(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

	#if SI_SYSTEM_IS_WINDOWS
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, nil);
		DWORD file_attrib = GetFileAttributesW(widePath);
		return file_attrib != INVALID_FILE_ATTRIBUTES;
	#else
		struct stat tmp;
		return (stat(si_stringCloneToCstr(path, &stack), &tmp) == 0);
	#endif
}
SIDEF
isize si_pathCopy(siString existingPath, siString newPath) {
	SI_ASSERT(existingPath.len <= SI_MAX_PATH_LEN);
	SI_ASSERT(newPath.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

	#if SI_SYSTEM_IS_WINDOWS
		siUtf16String wideExisting = si_utf8ToUtf16Str(stack, existingPath, nil);
		siUtf16String wideNew = si_utf8ToUtf16Str(stack, newPath, nil);

		b32 res = return (CopyFileW(wideExisting, wideNew, true) != 0);
	#else
		cstring srcPath = si_stringCloneToCstr(existingPath, &stack),
				dstPath = si_stringCloneToCstr(newPath, &stack);

		i32 existingFile = open(srcPath, O_RDONLY, 0);
		SI_STOPIF(existingFile == -1, { SI_FS_ERROR_DECLARE(); return false; });

		i32 newFile = open(dstPath, O_WRONLY | O_CREAT, 0666);
		SI_STOPIF(newFile == -1, { SI_FS_ERROR_DECLARE(); return false; });

		struct stat statExisting;
		i32 res = fstat(existingFile, &statExisting);
		SI_STOPIF(res == -1, { SI_FS_ERROR_DECLARE(); return false; });

		#if SI_SYSTEM_IS_UNIX
			isize size = sendfile64(newFile, existingFile, 0, statExisting.st_size);
		#else
			isize size = sendfile(newFile, existingFile, 0, &statExisting.st_size, NULL, 0);
		#endif
		SI_STOPIF(size == -1, { SI_FS_ERROR_DECLARE(); return false; });

		close(newFile);
		close(existingFile);

		return size;
	#endif
}
SIDEF
u32 si_pathItemsCopy(siString pathSrc, siString pathDst) {
	siDirectory dir = si_dirOpen(pathSrc);
	u32 itemsCopied = 0;

	char dst[SI_MAX_PATH_LEN];
	char* dstBuffer;
	siString dstStr = SI_STR_LEN(dst, pathDst.len + 1);

	si_memcopy(dst, pathDst.data, pathDst.len);
	dst[pathDst.len] = SI_PATH_SEPARATOR;
	dstBuffer = &dst[pathDst.len + 1];

	siDirectoryEntry entry;
	while (si_dirPollEntryEx(dir, &entry, false)) {
		siString path = si_pathBaseName(entry.path);
		si_memcopyStr(dstBuffer, path);

		if (entry.type == SI_IO_TYPE_DIR) {
			si_pathCreateFolder(dstStr);
			itemsCopied += si_pathItemsCopy(entry.path, dstStr);
			continue;
		}

		si_pathCopy(entry.path, dstStr);
		itemsCopied += 1;
	}

	return itemsCopied;
}

SIDEF
b32 si_pathMove(siString existingPath, siString newPath) {
	SI_ASSERT(existingPath.len <= SI_MAX_PATH_LEN);
	SI_ASSERT(newPath.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

	#if SI_SYSTEM_IS_WINDOWS
		siUtf16String wideExisting = si_utf8ToUtf16Str(stack, existingPath, nil);
		siUtf16String wideNew = si_utf8ToUtf16Str(stack, newPath, nil);

		return (MoveFileW(wideExisting, wideNew) != 0);
	#else
		cstring src = si_stringCloneToCstr(existingPath, &stack),
				dst = si_stringCloneToCstr(newPath, &stack);
		i32 res = link(src, dst);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });
		res = unlink(src);
		SI_STOPIF(res != 0, { SI_FS_ERROR_DECLARE(); return false; });

		return true;
	#endif
}

inline
b32 si_pathRename(siString oldName, siString newName) {
	return si_pathMove(oldName, newName);
}

inline
b32 si_pathCreateFolder(siString path) {
	return si_pathCreateFolderEx(path, SI_FS_PERM_ALL);
}

SIDEF
b32 si_pathCreateFolderEx(siString path, siFilePermissions perms) {
	SI_ASSERT(perms != -1);
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

	b32 res;
	#if SI_SYSTEM_IS_WINDOWS
		siUtf16String  pathWide = si_utf8ToUtf16Str(stack, path, nil);

		res = CreateDirectoryW(widePath, nil);
		SI_UNUSED(perms);
	#else
		/* NOTE(EimaMei): For whatever reason, 'mkdir' will sometimes return -1
		 * but still create the folder and set 'errno' to 0. What? */
		cstring str = si_stringCloneToCstr(path, &stack);
		res = (mkdir(str, perms) == 0);
	#endif

	SI_STOPIF(!res, { SI_FS_ERROR_DECLARE(); return false; });
	return true;
}

#if !SI_SYSTEM_IS_WINDOWS
force_inline
i32 si__unlinkCb(cstring path, const struct stat* sb, i32 typeflag, struct FTW* ftwbuf) {
	return remove(path);
	SI_UNUSED(sb); SI_UNUSED(typeflag); SI_UNUSED(ftwbuf);
}
#endif

SIDEF
b32 si_pathRemove(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

	b32 res;
	#if SI_SYSTEM_IS_WINDOWS
		siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
		usize len;
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, &len);

		b32 isDir = si_stringRFindStopAtEx(
			(siString)path,
			len / 2, 0,
			".", 1,
			SI_PATH_SEPARATOR
		) == -1;

		if (!isDir) {
			res = return DeleteFileW(widePath);
		}
		else {
			return RemoveDirectoryW(widePath);
		}
	#else
		cstring pathCstr = si_stringCloneToCstr(path, &stack);
		res = nftw(pathCstr, si__unlinkCb, 64, FTW_DEPTH | FTW_PHYS) != -1;
	#endif

	SI_STOPIF(!res, { SI_FS_ERROR_DECLARE(); return false; });
	return true;
}

SIDEF
b32 si_pathCreateHardLink(siString existingPath, siString linkPath) {
	SI_ASSERT(existingPath.len <= SI_MAX_PATH_LEN);
	SI_ASSERT(linkPath.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));


	#if SI_SYSTEM_IS_WINDOWS
		siAllocator* stack = si_allocatorMake(SI_KILO(2));
		siUtf16String wideExisting = si_utf8ToUtf16Str(stack, existingPath, nil);
		siUtf16String wideLink = si_utf8ToUtf16Str(stack, linkPath, nil);

		return CreateHardLinkW(wideLink, wideExisting, nil) != 0;
	#else
		int res = link(
			si_stringCloneToCstr(existingPath, &stack),
			si_stringCloneToCstr(linkPath, &stack)
		);
		SI_STOPIF(res == -1, { SI_FS_ERROR_DECLARE(); return false; });
		return true;
	#endif

}

SIDEF
b32 si_pathCreateSoftLink(siString existingPath, siString linkPath) {
	SI_ASSERT(existingPath.len <= SI_MAX_PATH_LEN);
	SI_ASSERT(linkPath.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

	#if SI_SYSTEM_IS_WINDOWS
		siAllocator* stack = si_allocatorMake(SI_KILO(2));
		usize existingLen;

		siUtf16String wideExisting = si_utf8ToUtf16Str(stack, existingPath, &existingLen);
		siUtf16String wideLink = si_utf8ToUtf16Str(stack, linkPath, nil);

		b32 isDir = si_stringRFindStopAtEx(
			(siString)existingPath,
			existingLen / 2, 0,
			".", 1,
			SI_PATH_SEPARATOR
		) == -1;

		return CreateSymbolicLinkW(wideLink, wideExisting, isDir) != 0;
	#else
		int res = symlink(
			si_stringCloneToCstr(existingPath, &stack),
			si_stringCloneToCstr(linkPath, &stack)
		);
		SI_STOPIF(res == -1, { SI_FS_ERROR_DECLARE(); return false; });
		return true;
	#endif

}

SIDEF
b32 si_pathEditPermissions(siString path, siFilePermissions newPerms) {
	SI_ASSERT(newPerms != -1);
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

	#if SI_SYSTEM_IS_WINDOWS
		/* TODO(EimaMei): Fix this. */
		SI_UNUSED(newPerms);
		return false;
	#else
		int res = chmod(si_stringCloneToCstr(path, &stack), (mode_t)newPerms);
		SI_STOPIF(res == -1, { SI_FS_ERROR_DECLARE(); return false; });
		return true;
	#endif
}


inline
siString si_pathBaseName(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

	for_eachRevStr (x, path) {
		if (*x == SI_PATH_SEPARATOR) {
			return si_stringSubToEnd(path, (x + 1) - path.data);
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
			return si_stringSubToEnd(path, (x + 1) - path.data);
		}
	}
	return path;
}

SIDEF
siString si_pathExtension(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

	for_eachStr (x, path) {
		if (*x == '.') {
			return si_stringSubToEnd(path, x - path.data + 1);
		}
		else if (*x == SI_PATH_SEPARATOR) {
			break;
		}
	}

	return SI_STR("");
}

SIDEF
siString si_pathGetFullName(siString path, siAllocator* alloc) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMake(SI_KILO(1));

	#if SI_SYSTEM_IS_WINDOWS
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, nil);
		siUtf16String buf = si_mallocArray(stack, u16, MAX_PATH);
		GetFullPathNameW(widePath, MAX_PATH, buf, nil);

		usize len;
		cstring utf8Res = si_utf16ToUtf8Str(stack, buf, &len);

		return si_stringMakeLen(alloc, utf8Res, len);
	#else
		/* NOTE(EimaMei): Technically on Linux you have no issue creating paths
		 * that go over the highest 'PATH_MAX' value (that being 4kb) except most
		 * software doesn't take account this, so they either fail or crash.
		 * What a trolleybus moment! */
		char actualPath[4096];

		realpath(si_stringCloneToCstr(path, &stack), actualPath);
		SI_STOPIF(actualPath == nil, { SI_FS_ERROR_DECLARE(); return SI_STR_LEN(nil, 0); });

		return si_stringMakeCstr(actualPath, alloc);
	#endif
}


inline
b32 si_pathIsAbsolute(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

	#if SI_SYSTEM_IS_WINDOWS
		return (
			si_charIsAlpha(path[0])
			&& path[1] == ':'
			&& path[2] == SI_PATH_SEPARATOR
			&& path[3] != '\0'
		);
	#else
		return (path.data[0] == SI_PATH_SEPARATOR && path.len != 1);
	#endif
}

inline
b32 si_pathIsRelative(siString path) {
	return !si_pathIsAbsolute(path);
}

SIDEF
u64 si_pathLastWriteTime(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMake(SI_KILO(1));


#if SI_SYSTEM_IS_WINDOWS
	FILETIME lastWriteTime = {0};
	WIN32_FILE_ATTRIBUTE_DATA data = {0};

	siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
	siUtf16String wideFilename = si_utf8ToUtf16Str(stack, filename, nil);
	if (GetFileAttributesExW(wideFilename, GetFileExInfoStandard, &data)) {
		lastWriteTime = data.ftLastWriteTime;
	}

	ULARGE_INTEGER res;
	res.LowPart = lastWriteTime.dwLowDateTime;
	res.HighPart = lastWriteTime.dwHighDateTime;
	return res.QuadPart;
#else
	struct stat fs;
	int res = stat(si_stringCloneToCstr(path, &stack), &fs);
	return (res == 0) ? fs.st_mtime : 0;
#endif
}

inline
siString si_pathGetTmp(void) {
#if SI_SYSTEM_IS_WINDOWS
	static char tmp[SI_MAX_PATH_LEN];
	u16 wideTmp[SI_KILO(1)];
	GetTempPathW(SI_KILO(1), wideTmp);

	siAllocator tmpAlloc = si_allocatorMakeTmp(tmp, sizeof(tmp));
	si_utf16ToUtf8Str(&tmpAlloc, wideTmp, nil);

	return tmp;
#else
	return SI_STR("/tmp");
#endif
}


SIDEF
siFilePermissions si_pathPermissions(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	siAllocator stack = si_allocatorMake(SI_KILO(1));

#if SI_SYSTEM_IS_WINDOWS
	ACCESS_MASK mask;
	{
		siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, nil);

		SECURITY_INFORMATION info = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;
		SECURITY_DESCRIPTOR* desc = (SECURITY_DESCRIPTOR*)si_allocatorCurPtr(stack);
		GetFileSecurityW(widePath, info, desc, (DWORD)si_allocatorAvailable(stack), nil);

		GetEffectiveRightsFromAclA(desc->Dacl, desc->Owner, &mask);
	}

	siFilePermissions perms = SI_FS_PERM_GROUP_EXEC | SI_FS_PERM_OTHERS_EXEC | SI_FS_PERM_OWNER_EXEC;
	perms |= ((mask & READ_CONTROL) != 0) * (SI_FS_PERM_GROUP_READ | SI_FS_PERM_OWNER_READ | SI_FS_PERM_OTHERS_READ);
	perms |= ((mask & WRITE_OWNER) != 0) * (SI_FS_PERM_OWNER_WRITE | SI_FS_PERM_GROUP_WRITE | SI_FS_PERM_OTHERS_WRITE);

	return perms;
#else
	struct stat fs;
	int res = stat(si_stringCloneToCstr(path, &stack), &fs);
	return (res == 0) ? fs.st_mode : -1;
#endif
}

static b32 SI_STD_FILE_SET = false;
static siFile SI_STD_FILE_ARR[SI_STDTYPE_COUNT] = {0};


siFile* si_fileGetStdFile(siStdFileType type) {
	SI_ASSERT_MSG(si_between(type, SI_STDTYPE_INPUT, SI_STDTYPE_ERROR), "Invalid STD file type.");

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
	return si_fileOpenMode(path, SI_FILE_WRITE | SI_FILE_PLUS);
}

inline
siFile si_fileOpen(siString path) {
	return si_fileOpenMode(path, SI_FILE_READ | SI_FILE_PLUS);
}
SIDEF
siFile si_fileOpenMode(siString path, siFileMode mode) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	SI_ASSERT((mode & ~SI_FILE_ALL) == 0);
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

	siFile res;
#if SI_SYSTEM_IS_WINDOWS
	u32 access, disposition;
	switch (mode) {
		case SI_FILE_READ:
			access = GENERIC_READ;
			disposition = OPEN_EXISTING;
			break;
		case SI_FILE_WRITE:
			access = GENERIC_WRITE;
			disposition = CREATE_ALWAYS;
			break;
		case SI_FILE_APPEND:
			access = GENERIC_WRITE;
			disposition = OPEN_ALWAYS;
			break;
		case SI_FILE_READ | SI_FILE_PLUS:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = OPEN_EXISTING;
			break;
		case SI_FILE_WRITE | SI_FILE_PLUS:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = CREATE_ALWAYS;
			break;
		case SI_FILE_APPEND | SI_FILE_PLUS:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = OPEN_ALWAYS;
			break;
		default:
			disposition = access = 0;
			SI_PANIC_MSG("Invalid file mode.");
	}

	rawptr handle;
	{
		siUtf16String widePath = si_utf8ToUtf16Str(stack, path, nil);
		handle = CreateFileW(
			widePath,
			access, FILE_SHARE_READ | FILE_SHARE_DELETE, nil,
			disposition, FILE_ATTRIBUTE_NORMAL, nil
		);
	}
	SI_STOPIF(handle == INVALID_HANDLE_VALUE, { SI_FS_ERROR_DECLARE(); return (siFile){0}; });

	if (mode & SI_FILE_APPEND) {
		LARGE_INTEGER offset = {0};
		if (!SetFilePointerEx(handle, offset, nil, FILE_END)) {
			SI_FS_ERROR_DECLARE();
			CloseHandle(handle);
			return (siFile){0};
		}
	}
	res.lastWriteTime = si_pathLastWriteTime(path); // TODO(EimaMei): Optimize out
#else
	i32 flags;
	switch (mode) {
		case SI_FILE_READ:
			flags = O_RDONLY;
			break;
		case SI_FILE_WRITE:
			flags = O_WRONLY | O_CREAT | O_TRUNC;
			break;
		case SI_FILE_APPEND:
			flags = O_WRONLY | O_APPEND | O_CREAT;
			break;
		case SI_FILE_READ | SI_FILE_PLUS:
			flags = O_RDWR;
			break;
		case SI_FILE_WRITE | SI_FILE_PLUS:
			flags = O_RDWR | O_CREAT | O_TRUNC;
			break;
		case SI_FILE_APPEND | SI_FILE_PLUS:
			flags = O_RDWR | O_APPEND | O_CREAT;
			break;
		default:
			flags = 0;
			SI_PANIC_MSG("Invalid file mode.");
	}

	cstring pathCstr = si_stringCloneToCstr(path, &stack);
	i64 handle = open(pathCstr, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	SI_STOPIF(handle == -1, { SI_FS_ERROR_DECLARE(); return (siFile){0}; });

	struct stat fs;
	int statRes = stat(pathCstr, &fs);
	res.lastWriteTime = (statRes == 0) ? fs.st_mtime : 0;
#endif
	res.handle = handle;
	res.size = si_fileSize(res);

	return res;
}

SIDEF
usize si_fileSize(siFile file) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);

#if SI_SYSTEM_IS_WINDOWS
	LARGE_INTEGER res;
	GetFileSizeEx(file.handle, &res);
	return res.QuadPart;
#else
	isize prevOffset = si_fileTell(file);
	usize len = si_fileSeek(file, 0, SI_FILE_MOVE_END);
	si_fileSeek(file, prevOffset, SI_FILE_MOVE_BEGIN);
	return len;
#endif
}

SIDEF
void si_fileSizeUpdate(siFile* file) {
	SI_ASSERT_NOT_NULL(file);
	file->size = si_fileSize(*file);
}

inline
siBuffer si_fileRead(siFile file, usize len, siAllocator* alloc) {
	return si_fileReadAt(file, si_fileTell(file), len, alloc);
}
inline
siBuffer si_fileReadAt(siFile file, isize offset, usize len, siAllocator* alloc) {
	len = si_min(len, si_allocatorAvailable(*alloc));
	return si_fileReadUnsafe(file, offset, len, si_malloc(alloc, len));
}

inline
siBuffer si_fileReadBuf(siFile file, usize len, siBuffer buffer) {
	return si_fileReadAtBuf(file, si_fileTell(file), len, buffer);
}
inline
siBuffer si_fileReadAtBuf(siFile file, isize offset, usize len, siBuffer outBuffer) {
	SI_ASSERT(outBuffer.len != USIZE_MAX);
	return si_fileReadUnsafe(file, offset, si_min(len, outBuffer.len), outBuffer.data);
}

SIDEF
siBuffer si_fileReadUnsafe(siFile file, isize offset, usize len, rawptr outBuffer) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
	SI_ASSERT_NOT_NULL(outBuffer);

	si_fileSeek(file, offset, SI_FILE_MOVE_BEGIN);
#if SI_SYSTEM_IS_WINDOWS
	DWORD bytesRead;
	b32 res = ReadFile(file.handle, outBuffer, (DWORD)len, &bytesRead, NULL);
	SI_STOPIF(!res, { SI_FS_ERROR_DECLARE(); return -1; });
#else
	isize bytesRead = pread((int)file.handle, outBuffer, len, offset);
	SI_STOPIF(bytesRead == -1, { SI_FS_ERROR_DECLARE(); return SI_BUF_LEN(outBuffer, -1); });
#endif

	return SI_BUF_LEN(outBuffer, bytesRead);
}

inline
siString si_fileReadContents(siFile file, siAllocator* alloc) {
	usize len = si_allocatorAvailable(*alloc);
	len = si_min(len, file.size);

	isize oldOffset = si_fileTell(file);
	siString res = si_fileReadUnsafe(file, 0, len, si_malloc(alloc, len));
	si_fileSeek(file, oldOffset, SI_FILE_MOVE_BEGIN);

	return res;
}
inline
siString si_fileReadContentsBuf(siFile file, siBuffer outBuffer) {
	SI_ASSERT(outBuffer.len != USIZE_MAX);

	isize oldOffset = si_fileTell(file);
	siString res = si_fileReadUnsafe(file, 0, si_min(outBuffer.len, file.size), outBuffer.data);
	si_fileSeek(file, oldOffset, SI_FILE_MOVE_BEGIN);

	return res;
}

inline
siArray(siString) si_fileReadlines(siFile file, siAllocator* alloc) {
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
	si_fileSeek(*file, offset, SI_FILE_MOVE_BEGIN);
	DWORD bytesWritten;
	b32 res = WriteFile(file->handle, content, (DWORD)contentLen, &bytesWritten, NULL);

	SI_STOPIF(res == false, { SI_FS_ERROR_DECLARE(); return -1; });
#else
	isize curOffset = si_fileSeek(*file, 0, SI_FILE_MOVE_CURRENT);
	isize bytesWritten = (curOffset == offset)
		? write((int)file->handle, content.data, content.len)
		: pwrite((int)file->handle, content.data, content.len, offset);

	SI_STOPIF(bytesWritten == -1, { SI_FS_ERROR_DECLARE(); return -1; });
#endif

	file->size += (offset - file->size) + bytesWritten;
	return bytesWritten;
}
SIDEF
isize si_fileWriteAtLine(siFile* file, siString line, isize index) {
	SI_ASSERT_NOT_NULL(file);
	SI_ASSERT_NOT_NULL(line.data);

	siAllocator tmp = si_allocatorMakeAny(SI_KILO(8), file->size + line.len);
	siString content = si_fileReadContents(*file, &tmp);

	b32 isNeg = index < 0;
	isize lineStart = 0;

	index = si_abs(index);
	while (index) {
		siString substr = si_stringSubToEnd(content, lineStart);
		isize len = !isNeg
			? si_stringFindByte(substr, '\n')
			: si_stringFindLastByte(substr, '\n');
		SI_STOPIF(len == -1, return -1);

		lineStart += len + countof_str("\n");
		index -= 1;
	}

	siString substr = si_stringSubToEnd(content, lineStart);
	isize lineEnd = !isNeg
		? si_stringFindByte(substr, '\n')
		: si_stringFindLastByte(substr, '\n');

	isize oldLen;
	if (lineEnd == -1) { /* NOTE(EimaMei): Index is the last line. */
		oldLen = file->size - (lineStart + 1);
	}
	else {
		oldLen = lineEnd - (lineStart + 1);

		si_memmove(
			&substr.data[line.len],
			&substr.data[lineEnd],
			file->size - lineEnd
		);
	}
	si_memcopyStr(substr.data, line);
	file->size += (isize)line.len - (isize)oldLen;

	isize oldOffset = si_fileTell(*file);
	isize len = si_fileWriteAt(file, content, 0);
	si_fileSeek(*file, oldOffset, SI_FILE_MOVE_BEGIN);

	si_allocatorFree(&tmp);
	return len;
}


SIDEF
isize si_fileTell(siFile file) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
	isize len = si_fileSeek(file, 0, SI_FILE_MOVE_CURRENT);
	return len;
}

SIDEF
isize si_fileSeek(siFile file, isize offset, siFileMoveMethod method) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);

#if SI_SYSTEM_IS_WINDOWS
	LARGE_INTEGER win32Offset;
	win32Offset.QuadPart = offset;

	SetFilePointerEx(file.handle, win32Offset, &win32Offset, method);
	return win32Offset.QuadPart;

#elif SI_SYSTEM_IS_APPLE
	return  lseek((int)file.handle, offset, method);

#else
	return lseek64((int)file.handle, offset, method);

#endif
}

inline
b32 si_fileSeekFront(siFile file) {
	return si_fileSeek(file, 0, SI_FILE_MOVE_BEGIN) != 0;
}

inline
b32 si_fileSeekBack(siFile file) {
	return si_fileSeek(file, 0, SI_FILE_MOVE_END) != 0;
}
SIDEF
b32 si_fileTruncate(siFile* file, usize size) {
	SI_ASSERT_NOT_NULL(file);
	SI_ASSERT_NOT_NULL((rawptr)file->handle);

#if SI_SYSTEM_IS_WINDOWS
	usize prevOffset = si_fileTell(*file);
	isize res = si_fileSeek(*file, size, SI_FILE_MOVE_BEGIN);
	SI_STOPIF(res == 0, return false);

	res = SetEndOfFile(file->handle);
	SI_STOPIF(res == 0, SI_FS_ERROR_DECLARE_EX(SI_FS_ERROR_TRUNCATE_FAILURE));

	res = si_fileSeek(*file, prevOffset, SI_FILE_MOVE_BEGIN);
	return res != 0;
#else
	i32 res = ftruncate((int)file->handle, size);
	SI_STOPIF(res == -1, SI_FS_ERROR_DECLARE_EX(SI_FS_ERROR_TRUNCATE_FAILURE));
	//si_fileSeek(*file, prevOffset, SI_FILE_MOVE_BEGIN);
	return (res == 0);
#endif
}

SIDEF
void si_fileClose(siFile file) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
#if SI_SYSTEM_IS_WINDOWS
	CloseHandle(file.handle);
#else
	close((int)file.handle);
#endif
}

SIDEF
siDirectory si_dirOpen(siString path) {
	SI_ASSERT_NOT_NULL(path.data);
	SI_ASSERT(path.len - 1 <= SI_MAX_PATH_LEN);

	siDirectory dir;
	dir.directoryLen = path.len;
	si_memcopy(dir.buffer, path.data, path.len);

	if (path.data[path.len - 1] != SI_PATH_SEPARATOR) {
		dir.buffer[path.len] = SI_PATH_SEPARATOR;
		path.len += 1;
	}

#if SI_SYSTEM_IS_WINDOWS
	siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));

	usize len;
	siUtf16String widePath = si_utf8ToUtf16Str(stack, path, &len);
	SI_ASSERT_MSG(len + 4 <= SI_MAX_PATH_LEN, "Path is larger than 260 bytes.");

	// NOTE(EimaMei): Reason why this works is because 'appendText' is right
	// after 'widePath' in memory.
	u16* appendText = si_mallocArray(stack, u16, 2);
	appendText[-1] = '\\';
	appendText[0] = '*';
	appendText[1] = '\0';

	WIN32_FIND_DATAW tmp;
	dir.handle = FindFirstFileW(widePath, &tmp);
	SI_ASSERT(handle != INVALID_HANDLE_VALUE); // TODO(EimaMei): Turėtų būti SI_STOPIF
	FindNextFileW(handle, &tmp);
#else
	/* NOTE(EimaMei): We do this because opendir only takes NULL-terminated C-strings. */
	dir.buffer[path.len] = '\0';

	dir.handle = opendir(dir.buffer);
	SI_STOPIF(dir.handle == nil, { SI_FS_ERROR_DECLARE(); return (siDirectory){0}; });

	/* NOTE(EimaMei): We skip '.' and '..'. */
	SI_DISCARD(readdir(dir.handle));
	SI_DISCARD(readdir(dir.handle));
#endif

	return dir;
}

inline
b32 si_dirPollEntry(siDirectory dir, siDirectoryEntry* entry) {
	return si_dirPollEntryEx(dir, entry, true);
}

SIDEF
b32 si_dirPollEntryEx(siDirectory dir, siDirectoryEntry* entry, b32 fullPath) {
	SI_ASSERT_NOT_NULL(entry);
	SI_ASSERT_NOT_NULL(dir.handle);

#if SI_SYSTEM_IS_WINDOWS
	siAllocator* stack = si_allocatorMakeStack(SI_KILO(1));
	WIN32_FIND_DATAW file;
	if (FindNextFileW(dir.handle, &file) == 0) {
		CloseHandle(dir.handle);
		return false;
	}

	usize len;
	siUtf8String utf8Text = si_utf16ToUtf8Str(stack, file.cFileName, &len);
	entry->len = len + dir.pathLen * fullPath;

	si_memcopy(entry->path, dir.path, dir.pathLen * fullPath);
	si_memcopy(entry->path + dir.pathLen * fullPath, utf8Text, entry->len + 1);
	entry->baseName = entry->path + dir.pathLen * fullPath;

	if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		entry->type = SI_IO_TYPE_DIR;
	}
	else if (file.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
		entry->type = SI_IO_TYPE_LINK;
	}
	else {
		entry->type = SI_IO_TYPE_FILE;
	}

	return true;
#else
	/* NOTE(EimaMei): Sudie, C++ palaikyme! Galbūt tapsi naudojama, kai bent jau
	 * tūrėsi paskirtus masyvo inicijavimus. */
	static u8 IO_types[] = {
		[DT_REG]  = SI_IO_TYPE_FILE,
		[DT_DIR]  = SI_IO_TYPE_DIR,
		[DT_LNK]  = SI_IO_TYPE_LINK,
		[DT_SOCK] = SI_IO_TYPE_SOCKET,
		[DT_CHR]  = SI_IO_TYPE_DEVICE,
		[DT_BLK]  = SI_IO_TYPE_BLOCK,
		[DT_FIFO] = SI_IO_TYPE_FIFO
	};

	struct dirent* dirEntry = readdir((DIR*)dir.handle);
	if (dirEntry == nil) {
		closedir(dir.handle);
		return false;
	}

	char* buffer;
	usize len = si_cstrLen(dirEntry->d_name);

	entry->type = IO_types[dirEntry->d_type];
	if (fullPath) {
		buffer = &dir.buffer[dir.directoryLen];
		entry->path.len = len + dir.directoryLen;
	}
	else {
		buffer = dir.buffer;
		entry->path.len = len;
	}
	SI_ASSERT(entry->path.len <= SI_MAX_PATH_LEN);

	si_memcopy(buffer, dirEntry->d_name, len);
	entry->path.data = (siByte*)buffer;

	return true;
#endif
}

#endif /* SI_IO_IMPLEMENTATION */

#if defined(SI_IMPLEMENTATION_THREAD) && !defined(SI_NO_THREAD)

#if SI_SYSTEM_IS_WINDOWS
siIntern
DWORD WINAPI si__internThreadProc(LPVOID arg) {
	siThread* t = (siThread*)arg;
	t->returnValue = t->func(t->arg);
	t->isRunning = false;

	return 0;
}
#else
siIntern
rawptr si__internThreadProc(rawptr arg) {
	siThread* t = (siThread*)arg;
	t->returnValue = t->func(t->arg);
	t->isRunning = false;

	return nil;
}
#endif


SIDEF
void si__internThreadMakeEx(siFunction function, siNullable rawptr arg, usize stackSize,
		b32 runThread, siThread* outThread) {
	SI_ASSERT_NOT_NULL(function);

	siThread* t = outThread;
	t->func = function;
	t->arg = arg;
	t->stackSize = stackSize;
	t->initialized = true;
	t->returnValue = nil;
	t->isRunning = false;

	if (runThread) {
		si_threadRun(t);
	}
}

SIDEF
b32 si_threadRun(siThread* t) {
	SI_ASSERT_NOT_NULL(t);
	SI_ASSERT(t->initialized);
	SI_ASSERT(!t->isRunning);

	#if SI_SYSTEM_IS_WINDOWS
		t->id = CreateThread(nil, t->stackSize, si__internThreadProc, t, 0, nil);
		t->isRunning = (t->id != nil);
	#else
		int res;
		if (t->stackSize != 0) {
			pthread_attr_t attr;
			res = pthread_attr_init(&attr);
			SI_ASSERT(res == 0);

			pthread_attr_setstacksize(&attr, t->stackSize);
			res = pthread_create(&t->id, &attr, si__internThreadProc, t);
			if (res) {
				pthread_attr_destroy(&attr);
			}
		}
		else {
			res = pthread_create(&t->id, nil, si__internThreadProc, t);
		}
		t->isRunning = (res == 0);

	#endif

	return t->isRunning;
}

SIDEF
b32 si_threadJoin(siThread* t) {
	SI_ASSERT_NOT_NULL(t);
	SI_ASSERT_NOT_NULL((rawptr)t->id);
	SI_ASSERT(t->initialized);

	#if SI_SYSTEM_IS_WINDOWS
		DWORD res = WaitForSingleObject(t->id, INFINITE);
	#else
		int res = pthread_join(t->id, nil);
	#endif

	return res == 0;
}
SIDEF
b32 si_threadCancel(siThread* t) {
	SI_ASSERT_NOT_NULL(t);
	SI_ASSERT_NOT_NULL((rawptr)t->id);
	SI_ASSERT(t->initialized);

	#if SI_SYSTEM_IS_WINDOWS
#ifndef SI_RELEASE_MODE
		si_fprintf(
			SI_STDERR,
			"%CRsi_threadCancel: This feature on Windows is not supported as of sili v%u.%u.%u.%C",
			SI_VERSION_MAJOR, SI_VERSION_MINOR, SI_VERSION_MINOR
		);
#endif
		return false;
	#else
		int res = pthread_cancel(t->id);
		t->isRunning = false;

		return res == 0;
	#endif
}

SIDEF
b32 si_threadDestroy(siThread* t) {
	SI_ASSERT_NOT_NULL(t);
	SI_ASSERT_NOT_NULL((rawptr)t->id);
	SI_ASSERT_MSG(!t->isRunning, "You musn't attempt to destroy a thread while it's still running");
	SI_ASSERT(t->initialized);

	t->initialized = false;
#if SI_SYSTEM_IS_WINDOWS
	b32 res = CloseHandle(t->id);
	if (res) {
		t->id = nil;
	}
	return res;
#else
	t->id = 0;
	return true;
#endif

}

SIDEF
b32 si_threadPrioritySet(siThread t, i32 priority) {
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

#if defined(SI_IMPLEMENTATION_BIT) && !defined(SI_NO_BIT)


SIDEF
usize si_numCountBitsU8(u8 num) {
	return (num * (u64)01001001001L & (u64)042104210421) % 017;
}


SIDEF
usize si_numCountBitsU32(u32 num) {
	 num -= ((num >> 1) & 0x55555555);
	 num = (num & 0x33333333) + ((num >> 2) & 0x33333333);
	 num = (num + (num >> 4)) & 0x0F0F0F0F;
	 return (num * 0x01010101) >> 24;
}


SIDEF
usize si_numCountBitsU64(u64 num) {
	num = num - ((num >> 1) & 0x5555555555555555);
	num = (num & 0x3333333333333333) + ((num >> 2) & 0x3333333333333333);
	num = (num + (num >> 4)) & 0xF0F0F0F0F0F0F0F;
	return (num * 0x101010101010101) >> 56;
}


SIDEF
usize si_numLeadingBitEx(u64 num, usize totalBits, siBitType bit) {
	u64 count = 0;

	usize i;
	for (i = totalBits - 1; i < USIZE_MAX; i -= 1) {
		if (SI_BIT_GET(num, i) == bit) {
			count += 1;
		}
		else {
			return count;
		}
	}

	return count;
}

SIDEF
usize si_numTrailingBitEx(u64 num, usize totalBits, siBitType bit) {
	u64 count = 0;

	for_range (i, 0, totalBits) {
		if (SI_BIT_GET(num, i) == bit) {
			count += 1;
		}
		else {
			return count;
		}
	}

	return count;
}

SIDEF
u64 si_numRotateLeftEx(u64 num, usize totalBits, usize bits) {
	return (num << bits) | (num >> (totalBits - bits));
}

SIDEF
u64 siNumRotateRightEx(u64 num, usize totalBits, usize bits) {
	return (num >> bits) | (num << (totalBits - bits));
}


SIDEF
u64 siNumReverseBitsEx(u64 num, usize totalBits) {
	u64 res = 0;

	for_range (i, 0, totalBits) {
		res <<= 1;
		res |= (num & 1);
		num >>= 1;
	}

	return res;
}

SIDEF
siArray(u8) si_numToBytesEx(siAllocator* alloc, u64 num, usize num_sizeof) {
	siArray(u8) res = (u8*)si_arrayMakeReserve(alloc, sizeof(*res), num_sizeof);
	SI_ARRAY_HEADER(res)->len = num_sizeof;

	for_range (i, 0, num_sizeof) {
		res[i] = (num >> ((num_sizeof - 1 - i) * 8)) & 255;
	}

	return res;
}

SIDEF
u64 si_bytesToNumSiArr(siArray(u8) bytes) {
	SI_ASSERT_NOT_NULL(bytes);
	u64 res = 0;
	usize shift = (si_arrayLen(bytes) - 1) * 8;

	for_range (i, 0, si_arrayLen(bytes)) {
		res |= (u64)bytes[i] << shift;
		shift -= 8;
	}

	return res;
}

SIDEF
u64 si_bytesToNumArr(siByte* array, usize len) {
	SI_ASSERT_NOT_NULL(array);
	usize res = 0;
	usize shift = (len - 1) * 8;

	for_range (i, 0, len) {
		res |= (u64)array[i] << shift;
		shift -= 8;
	}

	return res;
}

inline
u16 si_swap16(u16 x) {
	return si_cast(u16, ((x >> 8) & 0xFFu) | ((x & 0xFFu) << 8));
}
inline
u32 si_swap32(u32 x) {
	return si_cast(
		u32,
		((x & 0xFF000000u) >> 24)  | ((x & 0x00FF0000u) >>  8) |
		((x & 0x0000FF00u) <<  8)  | ((x & 0x000000FFu) << 24)
	);
}
inline
u64 si_swap64(u64 x) {
   return si_cast(
		u64,
		((x & 0xFF00000000000000ull) >> 56) | ((x & 0x00FF000000000000ull) >> 40) |
		((x & 0x0000FF0000000000ull) >> 24) | ((x & 0x000000FF00000000ull) >>  8) |
		((x & 0x00000000FF000000ull) <<  8) | ((x & 0x0000000000FF0000ull) << 24) |
		((x & 0x000000000000FF00ull) << 40) | ((x & 0x00000000000000FFull) << 56)
   );
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
usize si_numLen(u64 num) {
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
usize si_numLenEx(u64 num, u32 base) {
	usize count = 0;
	do {
		count += 1;
		num /= base;
	} while (0 < num);

	return count;
}

inline
usize si_numLenI64(i64 num) {
	return si_numLenI64Ex(num, 10);
}

inline
usize si_numLenI64Ex(i64 num, u32 base) {
	b64 isNegative = si_numIsNeg(num);
	u64 unsignedNum = (num ^ -isNegative) + isNegative;
	return si_numLenEx(unsignedNum, base) + isNegative;
}


#endif

#if defined(SI_IMPLEMENTATION_CPU) && !defined(SI_NO_CPU)

u32 SI_CPU_FREQ_MHZ = 0;


SIDEF
u32 si_cpuClockSpeed(void) {
	SI_STOPIF(SI_CPU_FREQ_MHZ != 0, return SI_CPU_FREQ_MHZ);
	u64 begin = si_RDTSC();
	si_sleep(100);
	u64 end = si_RDTSC();

	u32 val = si_cast(u32, end - begin) / 100000;
	SI_CPU_FREQ_MHZ = (u32)si_numRoundNearestMultiple(val, 10);
	return SI_CPU_FREQ_MHZ;
}

#endif /* SI_IMPLEMENTATION_CPU */

#if defined(SI_IMPLEMENTATION_PRINT) && !defined(SI_NO_PRINT)


inline
isize si_print(cstring str) {
	return si_fprint(SI_STDOUT, str);
}

inline
isize si_printf(cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_printfVa(fmt, va);
	va_end(va);

	return res;
}

inline
isize si_printfVa(cstring fmt, va_list va) {
	return si_fprintfVa(SI_STDOUT, fmt, va);
}


inline
isize si_fprint(siFile* file, cstring str) {
	return si_fileWrite(file, SI_CSTR(str));
}

inline
isize si_fprintf(siFile* file, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_fprintfVa(file, fmt, va);
	va_end(va);

	return res;
}
inline
isize si_fprintfVa(siFile* file, cstring fmt, va_list va) {
	char buffer[SI_KILO(8)];
	isize count = si_snprintfVa(buffer, sizeof(buffer), fmt, va) - 1;
	count = si_fileWrite(file, SI_BUF_LEN(buffer, count));
	return count;
}

inline
isize si_sprintf(char* buffer, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_sprintfVa(buffer, fmt, va);
	va_end(va);

	return res;
}
inline
isize si_snprintf(char* buffer, usize outCapacity, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_snprintfVa(buffer, outCapacity, fmt, va);
	va_end(va);

	return res;
}

inline
isize si_sprintfAlloc(siAllocator* allocator, char** out, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_sprintfAllocVa(allocator, out, fmt, va);
	va_end(va);

	return res;
}
inline
isize si_sprintfAllocVa(siAllocator* allocator, char** out, cstring fmt, va_list va) {
	char* ptr = (char*)si_allocatorCurPtr(*allocator);

	isize res = si_snprintfVa(
		ptr, si_allocatorAvailable(*allocator), fmt, va
	);

	*out = ptr;
	allocator->offset += res;
	return res;
}
SIDEF
isize si_sprintfVa(char* buffer, cstring fmt, va_list va) {
	return si_snprintfVa(buffer, USIZE_MAX, fmt, va);
}

struct si__printfInfoStruct {
	char* data;
	usize index;

	i32 padSize;
	i32 padLetter;

	siString str;

	usize capacity;
};

force_inline
void si__printStrToBuf(struct si__printfInfoStruct* info) {
	char* base = &info->data[info->index];

	usize len = si_min(info->str.len, info->capacity - info->str.len);

	if (info->padSize == 0) {
		si_memcopy(base, info->str.data, len);
	}
	else if (info->padSize < 0) {
		isize padLen = len + info->padSize;
		b32 padNeeded = si_numIsNeg(padLen);

		si_memcopy(base, info->str.data, len);
		if (padNeeded) {
			padLen = -padLen;
			memset(&base[len], info->padLetter, padLen);
			info->index += padLen;
			info->capacity -= padLen;
		}
		info->padSize = 0;
	}
	else {
		isize padLen = info->padSize - len;
		b32 padNeeded = !si_numIsNeg(padLen);

		if (padNeeded) {
			memset(base, info->padLetter, padLen);
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
	usize len = si_min(info->str.len, info->capacity - info->str.len);

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
isize si_snprintfVa(char* buffer, usize capacity, cstring fmt, va_list va) {
	SI_STOPIF(capacity == 0, return 0);

	union {
		i32 I32; u32 U32;
		u64 U64; i64 I64; f64 F64;
		usize USIZE; isize ISIZE;
		cstring STR; siByte* PTR;
	} vaValue;

	struct si__printfInfoStruct info;
	info.data = buffer;
	info.index = 0;
	info.capacity = capacity;
	info.padSize = 0;

	siAllocator stack = si_allocatorMakeStack(128);

	char x;
	cstring fmtPtr = fmt;
	u32 base = 10;
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
#ifndef SI_NO_SILI_PRINTF_STYLE
			case 'C': {
				if (!colorPresent) {
					siPrintColor3bit* x = va_arg(va, rawptr);
					SI_ASSERT(x->type == SI_PRINT_COLOR_TYPE_3BIT);
					colorPresent = true;

					char base[] ="\33[\0;\0\0m";
					base[2] = (!x->bold) ? '0' : '1';
					base[4] = (!x->light) ? '3' : '9';
					base[5] = '0' + x->color;

					info.str = SI_STR(base);
					si__printStrCpy(&info);
				}
				else {
					colorPresent = false;
					info.str = SI_STR("\33[0;0m");
					si__printStrCpy(&info);
				}

				break;
			}
#endif
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
				while (x == 'l' || si_charIsDigit(x) || x == '.' || x == '*');

				char altForm[2] = {'0', si_charLower(x)};
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
				siByte* buf;
GOTO_PADCALC_SWITCH:
				buf = stack.ptr;
				*buf = x;
				buf += 1;

				while (si_charIsDigit(*fmtPtr)) {
					*buf = *fmtPtr;
					buf += 1;
					fmtPtr += 1;
				}

				i64 count = si_stringToInt(SI_STR_LEN(stack.ptr, buf - stack.ptr));
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

				info.str = si_stringFromUIntEx(vaValue.U64, base, &stack);
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
					default: SI_PANIC();
				}
				SI_CHECK_AFTERPOINT_INT(info, afterPointIsSet, afterPoint);

				info.str = si_stringFromInt(vaValue.I64, &stack);
				si__printStrToBuf(&info);
				break;
			}

			case 'c': {
				vaValue.I32 = va_arg(va, i32);

				if (typeSize == 0) {
					info.str = SI_STR_LEN(&vaValue.I32, 1);
				}
				else {
					siUtf8Char x = si_utf8Encode(vaValue.I32);
					info.str = SI_STR_LEN(x.codepoint, x.len);
				}
				si__printStrToBuf(&info);

				break;
			}

			case 's': {
				char* values[2] = {va_arg(va, char*), "(nil)"};
				usize len[2] = {afterPoint * afterPointIsSet, countof_str("(nil)")};

				b32 valueIsNil = values[0] == nil;
				info.str = SI_STR_LEN(values[valueIsNil], len[valueIsNil]);

				if (info.str.len == 0) {
					if (typeSize == 0) {
						info.str.len = si_cstrLen(values[0]);
					}
					else {
						SI_PANIC();
					}
				}

				si__printStrToBuf(&info);
				break;
			}
#ifndef SI_NO_SILI_PRINTF_STYLE
			case 'S': {
				info.str = va_arg(va, siString);
				si__printStrToBuf(&info);
				break;
			}
#endif
			case 'A': case 'a':
				vaValue.F64 = va_arg(va, f64);

				char altForm[2] = {'0', x + ('X' - 'A')};
				info.str = SI_STR_LEN(altForm, sizeof(altForm));
				si__printStrCpy(&info);

				info.str = si_stringFromFloatEx(vaValue.F64, 16, afterPoint, &stack);
				si__printStrToBuf(&info);
				break;

			case 'F': case 'f':
				vaValue.F64 = va_arg(va, f64);

				info.str = si_stringFromFloatEx(vaValue.F64, 10, afterPoint, &stack);
				si__printStrToBuf(&info);
				break;

			case 'E': case 'e': {
				vaValue.F64 = va_arg(va, f64);
GOTO_SCIENTIFIC_NOTATION:
				if (si_f64IsNan(vaValue.F64)) {
					info.str = si_stringFromFloatEx(vaValue.F64, 10, afterPoint, &stack);
					si__printStrToBuf(&info);
					break;
				}
				i8 exponent = 0;
				while (vaValue.F64 > 10) {
					vaValue.F64 /= 10;
					exponent += 1;
				}

				info.str = si_stringFromFloatEx(vaValue.F64, 10, afterPoint, &stack);
				si__printStrToBuf(&info);


				char remainder[4];
				remainder[0] = x;
				remainder[1] = '+';
				if (exponent < 10) {
					remainder[2] = '0';
					remainder[3] = exponent + '0';
				}
				else {
					remainder[2] = (exponent / 10) + '0';
					remainder[3] = (exponent % 10) + '0';
				}

				info.str = SI_STR_LEN(remainder, sizeof(remainder));
				si__printStrCpy(&info);
				break;
			}
			case 'G': case 'g': {
				vaValue.F64  = va_arg(va, f64);

				if (vaValue.F64 < 0.0001) {
					x = 'E' + (x - 'G');
					goto GOTO_SCIENTIFIC_NOTATION;
				}

				u64 pow10Val = si_pow10(afterPoint);
				u64 intFloat = (u64)vaValue.F64;

				if (vaValue.F64 > (f64)pow10Val) {
					afterPoint -= si_cast(u32, intFloat / pow10Val);

					x = 'E' + (x - 'G');
					goto GOTO_SCIENTIFIC_NOTATION;
				}

				f64 remainder = vaValue.F64 - (f64)intFloat;
				f64 maxZeroValue = si_pow10F64(-(i32)afterPoint);

				i32 newAfterPoint = 0;
				while (newAfterPoint < afterPoint) {
					remainder *= 10;
					if (si_between(si_abs(remainder), 0, maxZeroValue)) {
						break;
					}
					f64 digit = remainder + 0.5;
					remainder -= digit;
					newAfterPoint += 1;
				}

				info.str = si_stringFromFloatEx(vaValue.F64, 10, (u32)newAfterPoint, &stack);
				si__printStrToBuf(&info);
				break;
			}
			case 'p': {
				info.str = SI_STR("0x");
				si__printStrCpy(&info);

				vaValue.USIZE = (usize)va_arg(va, rawptr);
				info.str = si_stringFromUIntEx(vaValue.USIZE, 16, &stack);
				si__printStrCpy(&info);
				break;
			}
#ifndef SI_NO_SILI_PRINTF_STYLE
			case 'B':  /* Boolean */
				info.str = si_stringFromBool(va_arg(va, b32));
				si__printStrCpy(&info);
				break;
#endif
			default: {
				char str[2] = {'%', x};
				info.str = SI_STR_LEN(str, sizeof(str));
				si__printStrCpy(&info);
				break;
			}
		}

		si_allocatorReset(&stack);
	}
	buffer[info.index] = '\0';
	info.index += 1;

	#undef SI_SET_FMT_PTR
	#undef SI_CHECK_FOR_PAD
	#undef SI_STR_CPY
	#undef SI_COLOR_CASE

	return info.index;
}

#endif /* SI_IMPLEMENTATION_PRINT */

#if defined(SI_IMPLEMENTATION_DLL) && !defined(SI_NO_DLL)


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
	return SI_FUNC_PTR_CHANGE_EX(*proc, PROC, siDllProc);
#else
	return (siDllProc)dlsym(dll, name);
#endif
}
#endif /* defined(SI_IMPLEMENTATION_DLL) */

#if defined(SI_IMPLEMENTATION_MATH) && !defined(SI_NO_MATH)

inline i64 si_abs(i64 x) { return x < 0 ? -x : x; }
inline f64 si_absF64(f64 x) { return x < 0 ? -x : x; }
inline f32 si_absF32(f32 x) { return x < 0 ? -x : x; }

inline b32 si_between(i64 x, i64 lower, i64 upper) { return lower <= x && x <= upper; }
inline b32 si_betweenu(u64 x, u64 lower, u64 upper) { return lower <= x && x <= upper; }
inline b32 si_betweenF64(f64 x, f64 lower, f64 upper) { return lower <= x && x <= upper; }
inline b32 si_betweenF32(f32 x, f32 lower, f32 upper) { return lower <= x && x <= upper; }

inline
f64 si_sinF64(f64 x) {
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
f32 si_sinF32(f32 x) {
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
f64 si_cosF64(f64 x) {
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
f32 si_cosF32(f32 x) {
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

inline f64 si_roundF64(f64 x) { return (x >= 0.0f) ? si_floorF64(x + 0.5f) : si_ceilF64(x - 0.5f); }
inline f32 si_roundF32(f32 x) { return (x >= 0.0f) ? si_floorF32(x + 0.5f) : si_ceilF32(x - 0.5f); }

inline f64 si_floorF64(f64 x) { return (x >= 0.0f) ? si_cast(f64, (i64)x) : si_cast(f64, (i64)(x - 0.9999999999999999f)); }
inline f32 si_floorF32(f32 x) { return (x >= 0.0f) ? si_cast(f32, (i64)x) : si_cast(f32, (i64)(x - 0.9999999999999999f)); }

inline f64 si_ceilF64(f64 x)  { return (x < 0) ? si_cast(f64, (i64)x) : si_cast(f64, (i64)x + 1); }
inline f32 si_ceilF32(f32 x)  { return (x < 0) ? si_cast(f32, (i32)x) : si_cast(f32, (i64)x + 1); }


SIDEF
u64 si_pow10(u32 exponent) {
	static const u64 allPossibleValues[] = {
		1e+0, 1e+1, 1e+2, 1e+3, 1e+4, 1e+5, 1e+6, 1e+7,
		1e+8, 1e+9, 1e+10, 1e+11, 1e+12, 1e+13, 1e+14, 1e+15,
		1e+16, 1e+17, 1e+18, 1e+19
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
	SI_ASSERT_MSG(si_between(exponent, -18, 18), "Exponent must be in range from 0 to 19 to not overflow.");

	return allPossibleValues[exponent + 18];
}

SIDEF
b32 si_f64IsNan(f64 num) {
	static u64 infinity = 0x7FF0000000000000;
	static u64 nanV = 0x7FF8000000000000;

	union { f64 f; u64 n; } check = {num};
	check.n &= ~SI_ENDIAN_VALUE(SI_BIT(63), SI_BIT(0)); /* NOTE(EimaMei): A quick way of changing the minus to plus. */
	num = check.f;

	return check.n == infinity || check.n == nanV;
}

SIDEF
i32 si_numRoundNearestMultipleI32(i32 num, i32 multiple) {
	return ((num + multiple / 2) / multiple) * multiple;
}
SIDEF
i64 si_numRoundNearestMultipleI64(i64 num, i32 multiple) {
	return ((num + multiple / 2) / multiple) * multiple;
}


#endif /* SI_IMPLEMENTATION_MATH */

#if defined(SI_IMPLEMENTATION_ARGV)


#define SI__OPTION_ERROR(ctx, id, opt, str) \
	ctx->error.type = id; \
	ctx->error.option = opt; \
	ctx->error.value = str; \
	return false

force_inline
b32 si__argvOptionSet(siOptionContext* ctx, siOption* option, siString substr) {
	b32 res;

	switch (option->type) {
		case SI_OPTION_TYPE_STRING: {
			siString* out = (siString*)option->outPtr + option->outLen;
			res = true;

			if ((option->__state & SI__INTERN_OPTION_CHOICES) == 0) {
				*out = substr;
			}
			else {
				b32 found = false;

				for_range (i, 0, option->choices.list.len) {
					siString choice = si_cast(siString*, option->choices.list.data)[i];
					if (si_stringEqual(substr, choice)) {
						*out = choice;
						found = true;
						break;
					}
				}

				if (!found) {
					SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_INVALID_CHOICE, option, substr);
				}
			}
			break;
		}
		case SI_OPTION_TYPE_UINT:
		case SI_OPTION_TYPE_INT: {
			i64* out = (i64*)option->outPtr + option->outLen;
			i64 value = si_stringToIntEx(substr, &res);

			if ((option->__state & SI__INTERN_OPTION_CHOICES) == 0) {
				*out = value;
			}
			else {
				if (si_between(value, option->choices.rangeI[0], option->choices.rangeI[1])) {
					*out = value;
				}
				else {
					SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_INVALID_CHOICE, option, substr);
				}
			}
			break;
		}
		case SI_OPTION_TYPE_BOOL: {
			b32* out = (b32*)option->outPtr + option->outLen;
			*out = si_stringToBool(substr);
			res = (*out != UINT32_MAX);
			break;
		}

		default: res = false;
	}


	if (!res) {
		SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_INVALID_VALUE, option, substr);
	}

	option->outLen += 1;
	option->__state |= SI__INTERN_OPTION_IS_SET;
	return true;
}

force_inline
void si__argvOptionDefaultSet(siOption* option) {
	switch (option->type) {
		case SI_OPTION_TYPE_STRING: {
			siString* out = (siString*)option->outPtr;
			*out = option->defaultValue.string;
			break;
		}
		case SI_OPTION_TYPE_UINT:
		case SI_OPTION_TYPE_INT: {
			i64* out = (i64*)option->outPtr;
			*out = option->defaultValue.integer;
			break;
		}
		case SI_OPTION_TYPE_BOOL: {
			b32* out = (b32*)option->outPtr;
			*out = option->defaultValue.boolean;
			break;
		}
		case SI_OPTION_TYPE_FLOAT: {
			b32* out = (b32*)option->outPtr;
			*out = option->defaultValue.boolean;
			break;
		}
		default: SI_PANIC();
	}

	option->outLen += 1;
	option->__state |= SI__INTERN_OPTION_IS_SET;
}

force_inline
siString si__argvType(siOptionType type) {
	siString buf[] = {SI_STR("string"), SI_STR("integer"), SI_STR("float"), SI_STR("boolean")};
	return buf[type - 1];
}

SIDEF
siOptionContext si_argvMake(cstring prefix, siOption* options, usize len) {
	SI_ASSERT_NOT_NULL(options);

	siOptionContext context;
	context.options = options;
	context.len = 0;
	context.capacity = len;
	context.executable = (siString){0};
	context.separator = SI_STR(" ");
	context.usage = (siString){0};
	context.description = (siString){0};

	context.paddingFront = 4;
	context.maxOptionLen = 35;
	context.maxColumnLen = 120;

	context.prefix = SI_CSTR(prefix);
	context.prefixShort = SI_STR("-");
	SI_ASSERT(context.prefix.len != 0);

	return context;
}
SIDEF
b32 si_argvParse(siOptionContext* ctx, int argc, char** argv) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(argv);

	ctx->executable = SI_CSTR(argv[0]);

	for_range (i, 1, argc) {
		siString value = SI_CSTR(argv[i]);

		b32 regularPrefix = true;
		if (si_memcompareStr(value.data, ctx->prefix) != 0) {
			if (si_memcompareStr(value.data, ctx->prefixShort) != 0) {
				b32 continueLoop = false;

				siOption* option;
				for_range (j, 0, ctx->len) {
					option = &ctx->options[j];
					SI_STOPIF((option->config & SI_OPTION_CONFIG_POSITIONAL) == 0, continue);

					if (option->outLen >= option->outCapacity) {
						continue;
					}

					b32 res = si__argvOptionSet(ctx, option, value);
					SI_STOPIF(!res, return false);
					continueLoop = true;
				}

				SI_STOPIF(continueLoop, continue);
				SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_PREFIX, nil, value);
				return false;
			}
			regularPrefix = false;
			value = si_stringSubToEnd(value, ctx->prefixShort.len);
		}
		else {
			value = si_stringSubToEnd(value, ctx->prefix.len);
		}

		usize optionLen = 0;
		siOption* option;
		for_range (j, 0, ctx->len) {
			option = &ctx->options[j];

			if (regularPrefix) {
				if (si_memcompareStr(value.data, option->name) == 0) {
					optionLen = option->name.len;
					break;
				}
			}
			else {
				siUtf8Char* shrt = &option->nameShort;
				if (shrt->len != 0 && si_memcompare(value.data, shrt->codepoint, shrt->len) == 0) {
					optionLen = shrt->len;
					break;
				}
			}
		}

		if (optionLen != 0) {
			if (option->outLen >= option->outCapacity) {
				SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_MULTIPLE, option, value);
			}
			value = si_stringSubToEnd(value, optionLen);
		}
		else {
			SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_OPTION, nil, value);
		}

		if (option->__state & SI__INTERN_OPTION_FLAG) {
			b32* outPtr = (b32*)option->outPtr + option->outLen;
			*outPtr = true;

			option->outLen += 1;
			option->__state |= SI__INTERN_OPTION_IS_SET;
			SI_STOPIF(option->config & SI_OPTION_CONFIG_IS_ACTION, return option->codepoint);
			continue;
		}

		if ((option->config & SI_OPTION_CONFIG_REQUIRED) == 0) {
			if (i + 1 >= argc || si_memcompareStr(argv[i + 1], ctx->prefix) == 0) {
				si__argvOptionDefaultSet(option);
				SI_STOPIF(option->config & SI_OPTION_CONFIG_IS_ACTION, return option->codepoint);
				continue;
			}
		}


		if (ctx->separator.data[0] == ' ' && ctx->separator.len == 1) {
			if (i + 1 >= argc || si_memcompareStr(argv[i + 1], ctx->prefix) == 0) {
				SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_NO_VALUE, option, value);
			}

			i += 1;
			value = SI_CSTR(argv[i]);

			b32 res = si__argvOptionSet(ctx, option, value);
			SI_STOPIF(!res, return false);
			SI_STOPIF(option->config & SI_OPTION_CONFIG_IS_ACTION, return option->codepoint);
			continue;
		}

		if ((option->config & SI_OPTION_CONFIG_NO_SEPARATOR) == 0
			&& si_memcompareStr(value.data, ctx->separator) != 0
		) {
			value = (value.len != 0) ? value : SI_STR(" ");
			SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_SEPARATOR, option, value);
		}
		value = si_stringSubToEnd(value, ctx->separator.len);

		if (value.data[0] == '\0') {
			SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_NO_VALUE, option, value);
		}

		b32 res = si__argvOptionSet(ctx, option, value);
		SI_STOPIF(!res, return false);
		SI_STOPIF(option->config & SI_OPTION_CONFIG_IS_ACTION, return option->codepoint);
	}

	for_range (i, 0, ctx->len) {
		siOption* option = &ctx->options[i];
		SI_STOPIF(option->__state & SI__INTERN_OPTION_IS_SET, continue);

		if (option->config & SI_OPTION_CONFIG_REQUIRED) {
			SI__OPTION_ERROR(ctx, SI_OPTION_ERROR_REQUIRED, option, (siString){0});
		}

		si__argvOptionDefaultSet(option);
	}

	return true;
}

SIDEF
void si_argvHelp(siOptionContext ctx) {
	if (ctx.usage.data == nil) {
		si_fprintf(
			SI_STDERR,
			"Usage: %S%S",
			ctx.executable, (ctx.len) ? SI_STR(" [options]") : SI_STR("")
		);

		for_range (i, 0, ctx.len) {
			siOption* option = &ctx.options[i];
			SI_STOPIF((option->config & SI_OPTION_CONFIG_POSITIONAL) == 0, continue);

			si_fprintf(
				SI_STDERR,
				(option->config & SI_OPTION_CONFIG_REQUIRED)
					? " %S"
					: " [%S]",
				option->name
			);
		}

		si_fprint(SI_STDERR, "\n");
	}
	else {
		si_fprintf(SI_STDERR, "%S\n", ctx.usage);
	}

	if (ctx.description.len != 0) {
		si_fprintf(SI_STDERR, "%S\n", ctx.description);
	}

	si_fprintf(SI_STDERR, "Options:\n", ctx.usage);

	for_range (i, 0, ctx.len) {
		siOption* option = &ctx.options[i];
		siString type = si__argvType(option->type);
		b32 noSeparator = (option->config & SI_OPTION_CONFIG_NO_SEPARATOR)
			|| option->__state & SI__INTERN_OPTION_FLAG;
		char argLetterStart, argLetterEnd;

		char buf[SI_KILO(4)];
		usize bufI = 0;

		if (option->config & SI_OPTION_CONFIG_REQUIRED)  {
			argLetterStart = '<';
			argLetterEnd = '>';
		}
		else {
			argLetterStart = '[';
			argLetterEnd = ']';
		}

		bufI += si_snprintf(buf, sizeof(buf), "%*s", ctx.paddingFront, "");

		if (option->nameShort.len != 0) {
			bufI += si_snprintf(
				&buf[bufI], sizeof(buf) - bufI,
				"%S%.*s, ",
				ctx.prefixShort, option->nameShort.len, option->nameShort.codepoint
			) - 1;
		}

		bufI += si_snprintf(
			&buf[bufI], sizeof(buf) - bufI,
			"%S%S%S",
			ctx.prefix, option->name, (!noSeparator) ? ctx.separator : SI_STR("")
		) - 1;

		if (option->__state & SI__INTERN_OPTION_CHOICES) {
			switch (option->type) {
				case SI_OPTION_TYPE_STRING: {
					siString* arr = (siString*)option->choices.list.data;
					usize len = option->choices.list.len - 1;

					bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%c", argLetterStart) - 1;
					for_range (j, 0, len) {
						bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%S|", arr[j]) - 1;
					}
					bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%S%c", arr[len], argLetterEnd) - 1;
					break;
				}
				case SI_OPTION_TYPE_INT:
					bufI += si_snprintf(
						&buf[bufI], sizeof(buf) - bufI, "%c%li-%li%c",
						argLetterStart,
						option->choices.rangeI[0], option->choices.rangeI[1],
						argLetterEnd
					) - 1;
					break;
				case SI_OPTION_TYPE_UINT:
					bufI += si_snprintf(
						&buf[bufI], sizeof(buf) - bufI, "%c%lu-%lu%c",
						argLetterStart,
						(u64)option->choices.rangeI[0], (u64)option->choices.rangeI[1],
						argLetterEnd
					) - 1;
					break;
			}
		}
		else if (!noSeparator) {
			bufI += si_snprintf(
				&buf[bufI], sizeof(buf) - bufI, "%c%S%c",
				argLetterStart, type, argLetterEnd
			) - 1;
		}


		if (option->description.len != 0) {
			if (bufI > ctx.maxOptionLen) {
				bufI += si_snprintf(
					&buf[bufI], sizeof(buf) - bufI,
					"\n%*s%*s",
					ctx.paddingFront, "", ctx.maxOptionLen - ctx.paddingFront - 1, ""
				);
			}
			else {
				bufI += si_snprintf(
					&buf[bufI], sizeof(buf) - bufI,
					"%*s",
					ctx.maxOptionLen - bufI, ""
				);
			}

			if (bufI + option->description.len <= ctx.maxColumnLen) {
				bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%S", option->description);
			}
			else {
				siString desc = option->description;
				usize startingLen = 0;
				usize prevLen, prevUtf8Len;
				usize limit = ctx.maxColumnLen - ctx.maxOptionLen;
				isize res;
back:
				prevLen = 0;
				prevUtf8Len = 0;
				while (true) {
					isize utf8Index;
					res = si_stringFindUtf8(desc, " ", &utf8Index);
					SI_STOPIF(res == -1, break);
					SI_STOPIF(utf8Index + prevUtf8Len > limit, break);

					prevLen += res + 1;
					prevUtf8Len += utf8Index + 1;
					desc = si_stringSubToEnd(desc, res + 1);
				}

				if (res == -1) {
					desc = si_stringSubToEnd(option->description, startingLen);
					bufI += si_snprintf(&buf[bufI], sizeof(buf) - bufI, "%S", desc);
				}
				else {
					desc = si_stringSub(option->description, startingLen, prevLen);
					startingLen += prevLen;

					bufI += si_snprintf(
						&buf[bufI],
						sizeof(buf) - bufI, "%S\n%*s%*s", desc,
						ctx.paddingFront, "", ctx.maxOptionLen - ctx.paddingFront - 1, ""
					);

					desc = si_stringSubToEnd(option->description, startingLen);
					goto back;
				}
			}
		}

		if (option->__state & SI__INTERN_OPTION_HAS_DEFAULT) {
			switch (option->type) {
				case SI_OPTION_TYPE_STRING:
					si_fprintf(SI_STDERR, "%.*s (Default: '%S')\n", bufI, buf, option->defaultValue.string);
					break;
				case SI_OPTION_TYPE_INT:
					si_fprintf(SI_STDERR, "%.*s (Default: '%li')\n", bufI, buf, option->defaultValue.integer);
					break;
				case SI_OPTION_TYPE_UINT:
					si_fprintf(SI_STDERR, "%.*s (Default: '%lu')\n", bufI, buf, option->defaultValue.integer);
					break;
				case SI_OPTION_TYPE_BOOL:
					si_fprintf(SI_STDERR, "%.*s (Default: '%B')\n", bufI, buf, option->defaultValue.boolean);
					break;
				case SI_OPTION_TYPE_FLOAT:
					si_fprintf(SI_STDERR, "%.*s (Default: '%f')\n", bufI, buf, option->defaultValue.floatingPoint);
					break;
			}
		}
		else {
			si_fprintf(SI_STDERR, "%.*s\n", bufI, buf);
		}
	}
}

SIDEF
void si_argvError(siOptionContext ctx) {
	siPrintColor3bit red = SI_PRINT_COLOR_3BIT(SI_ANSI_RED, true, false),
					 bold = SI_PRINT_COLOR_3BIT(SI_ANSI_WHITE, true, true);
	siOption* option = ctx.error.option;
	siString type = si__argvType(option->type);

	switch (ctx.error.type) {
		case SI_OPTION_ERROR_PREFIX:
			si_fprintf(
				SI_STDERR,
				"%C%S:%C %Cerror:%C A none valid prefix was used for %C'%S'%C (all options must be prefixed with %C'%S'%C).\n",
				&bold, ctx.executable, &red,
				&bold, ctx.error.value,
				&bold, ctx.prefix
			);
			break;

		case SI_OPTION_ERROR_OPTION:
			si_fprintf(
				SI_STDERR,
				"%C%S:%C %Cerror:%C Unknown option %C'%S%S'%C was specified.\n",
				&bold, ctx.executable, &red,
				&bold, ctx.prefix, ctx.error.value
			);
			break;

		case SI_OPTION_ERROR_MULTIPLE:
			si_fprintf(
				SI_STDERR,
				"%C%S:%C %Cerror:%C Option %C'%S%S%S<%S>'%C was specified more than %C'%lu'%C %S.\n",
				&bold, ctx.executable, &red,
				&bold, ctx.prefix, option->name, ctx.separator, type,
				&bold, option->outCapacity,
				(option->outLen == 1) ? SI_STR("time") : SI_STR("times")
			);
			break;

		case SI_OPTION_ERROR_NO_VALUE:
			si_fprintf(
				SI_STDERR,
				"%C%S:%C %Cerror:%C No value was specified for the %C'%S%S%S<%S>'%C option.\n",
				&bold, ctx.executable, &red,
				&bold, ctx.prefix, option->name, ctx.separator, type
			);
			break;

		case SI_OPTION_ERROR_INVALID_VALUE:
			si_fprintf(
				SI_STDERR,
				"%C%S:%C %Cerror:%C Invalid %S %C'%S'%C was specified for the %C'%S%S%S<%S>'%C option.\n",
				&bold, ctx.executable, &red, type,
				&bold, ctx.error.value,
				&bold, ctx.prefix, option->name, ctx.separator, type
			);
			break;

		case SI_OPTION_ERROR_INVALID_CHOICE: {
			switch (option->type) {
				case SI_OPTION_TYPE_STRING: {
					si_fprintf(
						SI_STDERR,
						"%C%S:%C %Cerror:%C Invalid choice %C'%S'%C was specified for the %C'%S%S%S[",
						&bold, ctx.executable, &red,
						&bold, ctx.error.value,
						&bold, ctx.prefix, option->name, ctx.separator
					);

					siString* arr = (siString*)option->choices.list.data;
					usize len = option->choices.list.len - 1;
					for_range (i, 0, len) {
						siString choice = arr[i];
						si_fprintf(SI_STDERR, "%S|", choice);
					}
					si_fprintf(SI_STDERR, "%C%S]'%C option.\n", &bold, arr[len]);
					break;
				}
				case SI_OPTION_TYPE_INT:
					si_fprintf(
						SI_STDERR,
						"%C%S:%C %Cerror:%C Invalid choice %C'%S'%C was specified for the %C'%S%S%S[%li-%li]'%C option.\n",
						&bold, ctx.executable, &red,
						&bold, ctx.error.value,
						&bold, ctx.prefix, option->name, ctx.separator,
						option->choices.rangeI[0], option->choices.rangeI[1]
					);
					break;
				case SI_OPTION_TYPE_UINT:
					si_fprintf(
						SI_STDERR,
						"%C%S:%C %Cerror:%C Invalid choice %C'%S'%C was specified for the %C'%S%S%S[%lu-%lu]'%C option.\n",
						&bold, ctx.executable, &red,
						&bold, ctx.error.value,
						&bold, ctx.prefix, option->name, ctx.separator,
						(u64)option->choices.rangeI[0], (u64)option->choices.rangeI[1]
					);
					break;
			}
			break;
		}

		case SI_OPTION_ERROR_SEPARATOR:
			si_fprintf(
				SI_STDERR,
				"%C%S:%C %Cerror:%C Invalid separator %C'%S'%C was specified at the %C'%S%S'%C option (it should be %C'%S'%C).\n",
				&bold, ctx.executable, &red,
				&bold, ctx.error.value,
				&bold, ctx.prefix, option->name,
				&bold, ctx.separator
			);
			break;

		case SI_OPTION_ERROR_REQUIRED:
			si_fprintf(
				SI_STDERR,
				"%C%S:%C %Cerror:%C Required option '%C%S%S%S<%S>%C' wasn't specified.\n",
				&bold, ctx.executable, &red,
				&bold, ctx.prefix, option->name, ctx.separator, si__argvType(option->type)
			);
			break;

		default: si_fprint(SI_STDERR, "N/A error\n");
	}
}

inline
void si_argvContextSetPrefix(siOptionContext* ctx, cstring prefix) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(prefix);
	ctx->prefix = SI_CSTR(prefix);
}
inline
void si_argvContextSetPrefixShort(siOptionContext* ctx, cstring prefixShort) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(prefixShort);
	ctx->prefixShort = SI_CSTR(prefixShort);
}
inline
void si_argvContextSetSeparator(siOptionContext* ctx, cstring separator) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(separator);
	ctx->separator = SI_CSTR(separator);
}
inline
void si_argvContextSetDescription(siOptionContext* ctx, cstring description) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(description);
	ctx->description = SI_CSTR(description);
}
inline
void si_argvContextSetUsage(siOptionContext* ctx, cstring usage) {
	SI_ASSERT_NOT_NULL(ctx);
	SI_ASSERT_NOT_NULL(usage);
	ctx->usage = SI_CSTR(usage);
}


inline
siOption* si_argvOptionMakeStr(siOptionContext* ctx, cstring name, siString* outPtr) {
	return si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_STRING, name, outPtr);
}
inline
siOption* si_argvOptionMakeInt(siOptionContext* ctx, cstring name, i64* outPtr) {
	return si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_INT, name, outPtr);
}
inline
siOption* si_argvOptionMakeUint(siOptionContext* ctx, cstring name, u64* outPtr) {
	return si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_UINT, name, outPtr);
}
inline
siOption* si_argvOptionMakeBool(siOptionContext* ctx, cstring name, b32* outPtr) {
	return si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_BOOL, name, outPtr);
}

SIDEF
siOption* si_argvOptionMakeEx(siOptionContext* ctx, siOptionType type, cstring name,
		rawptr outPtr) {
	SI_ASSERT_NOT_NULL(outPtr);
	SI_ASSERT(ctx->len < ctx->capacity);

	siOption* option = &ctx->options[ctx->len];
	option->type = type;
	option->name = SI_CSTR(name);
	option->nameShort = (siUtf8Char){0};
	option->codepoint = 0;
	option->description = (siString){0};
	option->config = 0;
	option->__state = 0;
	option->outPtr = outPtr;
	option->outLen = 0;
	option->outCapacity = 1;

	ctx->len += 1;
	return option;
}

inline
siOption* si_argvFlagMake(siOptionContext* ctx, cstring name, b32 isAction,
		b32* outPtr) {
	siOption* option = si_argvOptionMakeEx(ctx, SI_OPTION_TYPE_BOOL, name, outPtr);
	option->__state = SI__INTERN_OPTION_FLAG;
	option->defaultValue.boolean = false;

	if (isAction) {
		option->config |= SI_OPTION_CONFIG_IS_ACTION;
	}
	return option;
}

inline
void si_argvOptionSetDescription(siOption* option, cstring description) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT_NOT_NULL(description);

	option->description = SI_CSTR(description);
}

inline
void si_argvOptionSetShortName(siOption* option, i32 codepoint) {
	SI_ASSERT_NOT_NULL(option);

	option->nameShort = si_utf8Encode(codepoint);
	option->codepoint = codepoint;
	SI_ASSERT(*option->nameShort.codepoint != SI_UNICODE_INVALID && option->name.len != 0);
}

inline
void si_argvOptionSetConfig(siOption* option, siOptionConfig config) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT_MSG((option->config & ~SI_OPTION_CONFIG_BITS) == 0, "Invalid specified config.");
	SI_ASSERT_MSG((option->__state & SI__INTERN_OPTION_FLAG) == 0, "You cannot set the configurations for a flag.");
	SI_ASSERT_MSG(
		(option->config & SI_OPTION_CONFIG_POSITIONAL) == 0
		|| (option->config & SI_OPTION_CONFIG_IS_ACTION) == 0,
		"You cannot use the 'SI_OPTION_CONFIG_IS_ACTION' config for positionals."
	);
	option->config = config;
}

inline
void si_argvOptionSetChoices(siOption* option, siBuffer choices) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT_NOT_NULL(choices.data);
	option->__state |= SI__INTERN_OPTION_CHOICES;
	option->choices.list = choices;
}

inline
void si_ArgvOptionSetCapacity(siOption* option, usize capacity) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT(capacity != 0);
	option->outCapacity = capacity;
}

inline
void si__internArgvOptionSetDefault(siOption* option, usize item_sizeof, const rawptr ptr) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT_NOT_NULL(ptr);

	switch (option->type) {
		case SI_OPTION_TYPE_STRING:
			SI_ASSERT(item_sizeof == sizeof(siString));
			option->defaultValue.string = *(siString*)ptr;
			break;

		case SI_OPTION_TYPE_UINT:
		case SI_OPTION_TYPE_INT:
			si_memcopy(&option->defaultValue.integer, ptr, item_sizeof);
			break;

		case SI_OPTION_TYPE_FLOAT:
			si_memcopy(&option->defaultValue.floatingPoint, ptr, item_sizeof);
			break;

		case SI_OPTION_TYPE_BOOL:
			si_memcopy(&option->defaultValue.boolean, ptr, item_sizeof);
			option->defaultValue.boolean &= true;
			break;
	}

	option->__state |= SI__INTERN_OPTION_HAS_DEFAULT;
}

inline
void si__internArgvOptionSetRange(siOption* option, usize item_sizeof, const rawptr ptr1,
		const rawptr ptr2) {
	SI_ASSERT_NOT_NULL(option);
	SI_ASSERT(option->type != SI_OPTION_TYPE_STRING && option->type != SI_OPTION_TYPE_BOOL);
	option->__state |= SI__INTERN_OPTION_CHOICES;

	if (option->type == SI_OPTION_TYPE_INT || option->type == SI_OPTION_TYPE_UINT) {
		si_memcopy(&option->choices.rangeI[0], ptr1, item_sizeof);
		si_memcopy(&option->choices.rangeI[1], ptr2, item_sizeof);
	}
	else {
		si_memcopy(&option->choices.rangeF[0], ptr1, item_sizeof);
		si_memcopy(&option->choices.rangeF[1], ptr2, item_sizeof);
	}
}

#undef SI__OPTION_ERROR

#endif /* SI_IMPLEMENTATION_ARGV */

#if defined(SI_IMPLEMENTATION_BENCHMARK) && !defined(SI_NO_BENCHMARK)


void si__internBenchmarkThread(u32* arg) {
	si_sleep(*arg);
}

typedef struct {
	char runsCstr[countof("runs")];
	char spacePad[20];
} siPerformanceMSG;

siIntern
siPerformanceMSG* si_benchmarkGetMsgVars(siAllocator* alloc,
		usize multiplesOf10, usize medianIndex) {

	char runsCstr[] = "runs";
	SI_STOPIF(medianIndex == 1, runsCstr[3] = ' ');

	isize spacePadLen = si_numLen(multiplesOf10) - si_numLen(medianIndex);
	char spacePad[20] = {0};
	memset(spacePad, ' ', spacePadLen);


	siPerformanceMSG* msg = si_mallocItem(alloc, siPerformanceMSG);
	si_memcopy(msg->runsCstr, runsCstr, countof(runsCstr));
	si_memcopy(msg->spacePad, spacePad, spacePadLen);

	return msg;
}

void si_benchmarkLoopsAvgPrint(siAllocator* alloc, cstring funcname,
		usize start, usize end, siArray(u64) cycles, f32 ghzFreq) {
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction - '%s'\n"
		"Runs:\n",
		funcname
	);
	const siBenchmarkLimit* element = nil;

	usize arrayIndex = 0, medianIndex;
	for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10, arrayIndex += 1) {
		u64 amountCycles = cycles[arrayIndex];
		f64 amountTime = (f64)amountCycles / ghzFreq;

		element = si_benchmarkLimitLoop(amountTime);
		amountTime /= (f64)element->duration;

		siPerformanceMSG* msg = si_benchmarkGetMsgVars(alloc, end, medianIndex);
		si_printf(
			"\t%s%zu %s - %9.4f %s (%lu cycles)\n",
			msg->spacePad, medianIndex, msg->runsCstr,
			amountTime, element->unit, amountCycles
		);
	}

	f64 cyclesMedian = (f64)si_arraySumU64(cycles) / (f64)si_arrayLen(cycles);
	f64 timeMedian = cyclesMedian / ghzFreq;
	element = si_benchmarkLimitLoop(timeMedian);
	timeMedian /= (f64)element->duration;

	si_printf(
		"Final result:\n"
			"\tTime average   - %9.4f %s\n"
			"\tCycles average - %lu cycles\n",
		timeMedian, element->unit,
		(u64)cyclesMedian
	);
}

void si_benchmarkLoopsAvgCmpPrint(siAllocator* alloc, cstring funcname[2],
		usize start, usize end, siArray(u64) cycles[2]) {
	f64 ghzFreq = (f64)si_cpuClockSpeed() / 1000;
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction 1 - '%s'\n"
			"\tFunction 2 - '%s'\n"
		"Runs:\n",
		funcname[0], funcname[1]
	);
	const siBenchmarkLimit* element0 = nil, *element1 = nil;

	usize arrayIndex = 0, medianIndex;
	for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10) {
		u64 cycles0 = cycles[0][arrayIndex],
			cycles1 = cycles[1][arrayIndex];
		f64 time_0 = (f64)cycles0 / ghzFreq,
			time_1 = (f64)cycles1 / ghzFreq;

		b32 zeroIsSlower = (time_0 > time_1);
		f64 ratio = zeroIsSlower ? (time_0 / time_1) : (time_1 / time_0);
		i32 clr0 = zeroIsSlower
			? 'R'
			: (ratio == 1.0f) ? 'Y' : 'G';
		i32 clr1 = zeroIsSlower
			? 'G'
			: (ratio == 1.0f) ? 'Y' : 'R';

		element0 = si_benchmarkLimitLoop(time_0);
		element1 = si_benchmarkLimitLoop(time_1);
		time_0 /= (f64)element0->duration;
		time_1 /= (f64)element1->duration;

		siPerformanceMSG* msg = si_benchmarkGetMsgVars(alloc, end, medianIndex);
		si_printf(
			"\t%s%zu %s - %C*%9.4f%C %s vs %C*%9.4f%C %s (%4.4f ratio, %6lu vs %6lu cycles)\n",
			msg->spacePad, medianIndex, msg->runsCstr,
			clr0, time_0, element0->unit, clr1, time_1, element1->unit,
			ratio, cycles0, cycles1
		);

		arrayIndex += 1;
	}

	f64 cyclesMedian0 = (f64)si_arraySumU64(cycles[0]) / (f64)si_arrayLen(cycles[0]),
		cyclesMedian1 = (f64)si_arraySumU64(cycles[1]) / (f64)si_arrayLen(cycles[1]);
	f64 timeMedian0 = cyclesMedian0 / ghzFreq,
		timeMedian1 = cyclesMedian1 / ghzFreq;

	b32 zeroIsSlower = (timeMedian0 > timeMedian1);
	f64 ratio = zeroIsSlower ? (timeMedian0 / timeMedian1) : (timeMedian1 / timeMedian0);
	i32 clr0 = zeroIsSlower ? 'R' : 'G',
			clr1 = zeroIsSlower ? 'G' : 'R';

	element0 = si_benchmarkLimitLoop(timeMedian0);
	element1 = si_benchmarkLimitLoop(timeMedian1);
	timeMedian0 /= (f64)element0->duration;
	timeMedian1 /= (f64)element1->duration;

	si_printf(
		"Final result:\n"
			"\tTime average   - %C*%9.4f%C %s vs %C*%.4f%C %s (%4.4f ratio)\n"
			"\tCycles average - %lu cycles vs %lu cycles\n",
		clr0, timeMedian0, element0->unit,
		clr1, timeMedian1, element1->unit,
		ratio,
		(u64)cyclesMedian0, (u64)cyclesMedian1
	);
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

const siBenchmarkLimit* si_benchmarkLimitLoop(siTimeStamp time) {
	const siBenchmarkLimit* element = siBenchLimit;
	const siBenchmarkLimit* end = &siBenchLimit[countof(siBenchLimit)];

	while (element != end) {
		if (si_betweenu(time, (&element[0])->duration, (&element[1])->duration)) {
			break;
		}
		element += 1;
	}

	return element;
}

#endif


#if !defined(SI_COMPILER_MSVC) && defined(SI_ASM_USE_ATT_SYNTAX)
	#undef si_asm
	#undef SI_ASM_NL
	#define si_asm(asm, ...)  __asm__ __volatile__(asm __VA_ARGS__)
	#define SI_ASM_NL "\n\t"
#endif

#if defined(__cplusplus)
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
	#define SI_ASSERT_MSG(condition, message) si_assertEx(condition, #condition, __FILE__, __LINE__, __func__, message, "")
	/* condition - EXPRESSION | message - cstring | ...FMT - VARIADIC
	 * Crashes the app with a formatted message if the condition is not met. */
	#define SI_ASSERT_FMT(condition, message, ...) si_assertEx(condition, #condition, __FILE__, __LINE__, __func__, message, __VA_ARGS__)
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
