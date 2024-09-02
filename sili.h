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

	- SI_RELEASE_MODE - disables certain functionality that decrease performance
	(such as logging, error reporting, assertions, etc). Defining "NDEBUG"
	does the same thing.

	- SI_NO_ASSERTIONS_IN_HEADER - assertion functions inside the library will
	not check for any conditions, passively allowing for both correct and wrong
	states to function instead of crashing when a condition evaluates to false.

	- SI_NO_ASSERTIONS - assertion functions that were defined in the sili header
	will continue to not have their functionality, including outside its original
	file.

	- SI_NO_ERROR_STRUCT - strips the 'siError' structure's members down to just
	the error code member. This gets turned on automatically via 'SI_RELEASE_MODE'.

	- SI_NO_ERROR_LOGS - disables sili's automatic loggin for when an 'SI_ERROR'
	is declared. This gets turned on automatically via 'SI_RELEASE_MODE'.

	- SI_NO_INLINE_ASM - disables any usage of inline assembly in the header.

	- SI_ASM_USE_ATT_SYNTAX - makes 'si_asm()' utilize AT&T syntax instead of
	Intel's for GNU C compiler derivatives. This has no affect on compilers that
	don't have support for AT&T syntax.

	- SI_NO_WINDOWS_H - disables the inclusion of the win32 API inside the header.
	Note that sili will most likely not properly function without a user-implemented
	replacement.

	- SI_NO_TYPEOF - disables the usage of the 'typeof' operator inside the header,
	alongside undefining macros that utilize it.

	- SI_DEFINE_CUSTOM_HASH_FUNCTION - undefines the base implementation of
	'si__hashKey' inside this file, allowing the user to define their own hash
	key function.

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
	- Sili is designed to be a fast, modern, but also an experimental library and
	because of it some unwarranted results may occur or be present when using the
	library, those being:
		1) Features may not work as expected
		2) Functions may not be documented or only contain incomplete documentation
		3) API breaking changes between releases
		4) Little to no security checks for malicious code attempting to explode
		sections of the code

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

/* TODO
 * TODO */
#define SI_COMPILER_HAS_PRAGMA_ONCE (SI_COMPILER_CLANG || SI_COMPILER_MSVC || SI_COMPILER_CHECK_MIN(GCC, 3, 4, 0))

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
		#define SI_STATIC_ASSERT_MSG(condition, msg) \
			extern int (*__si_error_if_negative (void)) \
			[(condition) ? 2 : -1]

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

	#if !defined(SI_NO_WINDOWS_H)
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


#if defined(SI_RELEASE_MODE) || defined(NDEBUG)
	#undef SI_NO_ASSERTIONS
	#undef SI_NO_ERROR_LOGS
	#undef SI_NO_ERROR_STRUCT

	#define SI_NO_ASSERTIONS
	#define SI_NO_ALLOC_DEBUG_INFO
	#define SI_NO_ERROR_LOGS
	#define SI_NO_ERROR_STRUCT
#endif

#if !defined(SI_NO_TYPE_DEFS)
	#include <stdint.h>

	typedef uint8_t   u8;
	typedef  int8_t   i8;
	typedef uint16_t u16;
	typedef  int16_t i16;
	typedef uint32_t u32;
	typedef  int32_t i32;
	typedef uint64_t u64;
	typedef  int64_t i64;

	#if !defined(b8)
		typedef u8   b8; /* NOTE(EimaMei): Don't use this as the primary boolean type. */
		typedef u16 b16;
		typedef u32 b32; /* NOTE(EimaMei): Use this as the default! */
		typedef u64 b64;
		typedef u8 siByte;
	#endif

	typedef size_t      usize;
	typedef ptrdiff_t   isize;

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
	/* TODO
	 * TODO */
	#define union_anonymous(memberName, ...) \
		union { \
			__VA_ARGS__ \
			union { \
				__VA_ARGS__ \
			} memberName; \
		}

#else
	 * TODO */

	#define union_anonymous(memberName, ...) \
		union { \
			__VA_ARGS__ \
		} memberName

#endif


#if !defined(rawptr)
	/* A pointer type pointing to raw data. */
	typedef void* rawptr;
#endif

#if !defined(cstring)
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
 * Shortcut for (1 << x). */
#define SI_BIT(x) (1ULL << (x))
#if !defined(nil)
	#if SI_LANGUAGE_IS_CPP
		/* A nothing value, equivalent to NULL. */
		#define nil nullptr
	#else
		/* A nothing value, equivalent to NULL. */
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
SI_STATIC_ASSERT(countof_str("abcd") == 4);

#if !defined(offsetof)
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

#if defined(SI_TYPEOF_USED)
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

#if SI_STANDARD_CHECK_MAX(C, C99) || (!defined(__STDC_LIB_EXT1__) && !defined(memcpy_s))
	/* dst - rawptr | dstsz - usize | src - rawptr | count - usize
	 * A pre-C11 implementation of memcpy_s without 'errorno_t'. */
	#define memcpy_s(dst, dstsz, src, count) si_memcopy(dst, src, si_min(usize, dstsz, count))
#endif


/*
	========================
	| SI_ASSERT            |
	========================
*/

#ifndef SI_DEBUG_TRAP
	#if defined(SI_COMPILER_MSVC)
	 	#if SI_COMPILER_VERSION < SI_VERSION(7, 0, 0)
			#define SI_DEBUG_TRAP() __asm int 3
		#else
			#define SI_DEBUG_TRAP() __debugbreak()
		#endif
	#elif SI_COMPILER_CHECK_MIN(GCC, 4, 0, 0) || SI_COMPILER_CHECK_MIN(CLANG, 2, 6, 0)
		#define SI_DEBUG_TRAP() __builtin_trap()
	#else
		#define SI_DEBUG_TRAP() exit(1)
	#endif
#endif

noreturn SIDEF void si_panic(cstring conditionStr, cstring file, i32 line,
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
	#define SI_ASSERT_MSG(condition, message) SI_ASSERT_FMT(condition, message, "")
	/* condition - EXPRESSION | message - cstring | ...FMT - VARIADIC
	 * Crashes the app with a formatted message if the condition is not met. */
	#define SI_ASSERT_FMT(condition, message, ...) \
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
	/* TODO
	 * TODO */
	#define SI_ANY_TYPE(.../* VALUE */) SI_ANY(typeof(__VA_ARGS__), __VA_ARGS__)
#endif
/* type - TYPE | ...VALUE - EXPRESSION
 * Creates a 'siAny' object. Said object's pointer is just "&(<VALUE>)". */
#define SI_ANY(type, .../* VALUE */) \
	(siAny){si_transmute(rawptr, &(type){__VA_ARGS__}, type*), sizeof(type)}


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

SI_ENUM(i32, siAllocatorFuncEnum) {
	siAllocatorFunc_Alloc,
	siAllocatorFunc_Resize,
	siAllocatorFunc_Free,
	siAllocatorFunc_FreeAll,
	siAllocatorFunc_Reset,

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
		usize setOffset;
	);
} siAllocatorFunc;

/* TODO
 * TODO */
#define SI_ALLOCATOR_PROC(name) rawptr name(siAllocatorFunc function, rawptr userData)

/* TODO */
typedef SI_ALLOCATOR_PROC(siAllocatorProc);

typedef struct siAllocator {
	siAllocatorProc* proc;
	rawptr userData;
} siAllocator;

typedef struct siAllocatorData {
	siByte* ptr;
	usize offset;
	usize capacity;
} siAllocatorData;

SI_ALLOCATOR_PROC(si_allocator_heap_proc);
SI_ALLOCATOR_PROC(si_allocator_arena_proc);
SI_ALLOCATOR_PROC(si_allocator_stack_proc);

/* Sets the default memory alignment for 'si_alignCeil'. */
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

/* TODO */
SIDEF siAllocator si_allocatorHeap(void);
/* TODO */
SIDEF siAllocator si_allocatorMakeArena(usize capacity, siAllocatorData* inData);
/* TODO */
#define si_allocatorMakeStack(capacity) \
	si__allocatorMakeStack(capacity, si_sallocItem(siAllocatorData), (siByte*)si_salloc(capacity))
/* TODO */
SIDEF siAllocator si_allocatorMakeTmp(rawptr ptr, usize capacity, siAllocatorData* inData);

/* TODO */
SIDEF rawptr si_alloc(siAllocator alloc, usize bytes);
/* TODO */
SIDEF rawptr si_realloc(siAllocator alloc, rawptr ptr, usize sizeOld, usize sizeNew);
/* TODO */
SIDEF void si_free(siAllocator alloc, rawptr ptr);
/* TODO */
SIDEF void si_freeAll(siAllocator alloc);

/* */
SIDEF void si_allocatorReset(siAllocator alloc);
/* */
SIDEF usize si_allocatorGetAvailable(siAllocator alloc);


/* allocator - siAllocator* | type - TYPE
 * Allocates 'sizeof(type)' bytes to the allocator and casts the value. */
#define si_allocItem(allocator, type) (type*)si_alloc(allocator, sizeof(type))
/* allocator - siAllocator* | type - TYPE | count - usize
 * Allocates an array of 'sizeof(type)' bytes to the allocator and casts the value. */
#define si_allocArray(allocator, type, count) (type*)si_alloc(allocator, sizeof(type) * (count))

#ifndef si_malloc
	/* bytes - usize
	 * Allocates the memory into the heap. */
	#define si_malloc(bytes) si_alloc(si_allocatorHeap(), bytes)
	/* TODO
	 * TODO */
	#define si_mfree(ptr) si_free(si_allocatorHeap(), ptr)
#endif

/* TODO
 * TODO */
#define si_mallocItem(type) (type*)si_malloc(sizeof(type))
/* TODO
 * TODO */
#define si_mallocArray(type, count) (type*)si_malloc(sizeof(type) * (count))


#ifndef si_salloc
	/* bytes - usize
	 * Allocates the memory into the stack. */
	#define si_salloc(bytes) si_cast(rawptr, (siByte[bytes]){0})
#endif

/* type - TYPE/EXPRESSION
 * Allocates 'sizeof(type)' bytes to the stack and casts the value. */
#define si_sallocItem(type) (type*)si_salloc(sizeof(type))
/* type - TYPE/EXPRESSION | count - usize
 * Allocates an array of 'sizeof(type)' bytes to the stack and casts the value. */
#define si_sallocArray(type, count) (type*)si_salloc(sizeof(type) * count)


#if 1
	SIDEF siAllocator si__allocatorMakeStack(usize capacity, siAllocatorData* inData, siByte* __stack);
#endif


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
	| siError & siOptional |
	========================
*/

/*
	========================
	|  siError             |
	========================
*/

/* A struct containing information about an error that happened during an operation. */
typedef struct siError {
	/* Error code from the operation. */
	i32 code;

#if !defined(SI_NO_ERROR_STRUCT)
	/* */
	i32 line;
	/* */
	cstring filename;
	/* The time when the error happened (in UTC+0). */
	i64 time;
#endif
} siError;


/* ptr - siError* | errorCode - i32 | TODO
 * Sets the error code, function name and time for the error variable. The
 * specified pointer can be 'nil', however nothing will then be written. */
#define SI_ERROR(ptr, errorCode) SI_ERROR_EX(ptr, errorCode, si_errorLogDefault)
/* TODO
 * TODO */
#define SI_ERROR_DEFAULT (siError){.code = 0}

/* TODO
 * TODO */
#define SI_ERROR_CHECK(condition, ptr, errorCode, returnValue) \
	SI_ERROR_CHECK_EX(condition, ptr, errorCode, returnValue, si_errorLogDefault)
/* TODO
 * TODO */
#define SI_ERROR_CHECK_EX(condition, ptr, errorCode, returnValue, function) \
	SI_STOPIF(condition, SI_ERROR_EX(ptr, errorCode, function); return returnValue);


#if !defined(SI_NO_ERROR_STRUCT)
	/* error - siError* | errorCode - i32 | TODO
	 * Sets the error code, function name and time for the error variable. The
	 * specified pointer can be 'nil', however nothing will then be written. */
	#define SI_ERROR_EX(error, errorCode, function) \
		do { \
			i32 si__errorCode = errorCode; \
			if (error != nil) { \
				(error)->line = __LINE__; \
				(error)->filename = __FILE__; \
				(error)->code = si__errorCode; \
				(error)->time = si_timeNowUTC(); \
			} \
			SI_ERROR_LOG(error, function); \
		} while (0)

#else
	/* TODO
	 * TODO */
	#define SI_ERROR_EX(error, errorCode, function) \
		do { \
			i32 si__errorCode = errorCode; \
			if (error != nil) { \
				(error)->code = si__errorCode;  \
			} \
			SI_ERROR_LOG(si__errorCode, function);
		} while(0)

#endif

#if !defined(SI_NO_ERROR_LOGS)
	/* TODO
	 * TODO */
	#define SI_ERROR_LOG(error, function) function(error)

#else
	/* TODO
	 * TODO */
	#define SI_ERROR_LOG(errorCode, function) do {} while (0)

#endif

/* TODO */
SIDEF void si_errorLogDefault(siError* error);

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
 * no value, additional information won't be written in the structure. */
#define siOption(type) si__OptionMacro(type)
/* type - TYPE
 * Represents an object that may or may not contain a value. If the object has
 * no value, additional information will be written in the structure. */
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
si_optional_define(siByte);


/* TODO | ...VALUE - EXPRESSION
 * Creates a returnable 'siOptional' from the given value. */
#define SI_OPT(type, .../* VALUE */) (siOption(type)){true, .data = {__VA_ARGS__}}

/* TODO
 * TODO */
#define SI_OPT_NIL(type) SI_OPT_ERR(type, 0)

/* TODO
 * TODO */
#define SI_OPT_ERR(type, errorValue) (siOption(type)){false, .data = {.error = errorValue}}


/* optionalVar - siOptional(TYPE) | defaultValue - EXPRESSION
 * Gets the value of the provided 'siObject' object. However if 'hasValue' is set
 * to false, return 'defaultValue' instead. */
#define si_optionalGetOrDefault(optionalVar, defaultValue) \
	((optionalVar).hasValue ? (optionalVar).data.value : (defaultValue))


#if 1
	#define si__OptionMacro(type) siOption_##type
#endif

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

typedef struct siBuffer {
	/* TODO */
	rawptr data;
	/* TODO */
	usize len;
	/* */
	usize capacity;
	/* */
	usize typeSize;
	/* */
	siAllocator alloc;
} siBuffer;

/* */
si_optional_define(siBuffer);

/* TODO
 * TODO */
#define siArray(type) siBuffer

/* TODO
 * TODO */
#define SI_BUF(type, .../* values*/) SI_BUF_LEN(type, ((type[]){__VA_ARGS__}), countof((type[]){__VA_ARGS__}))
/* TODO
 * TODO */
#define SI_BUF_LEN(type, buffer, length) SI_BUF_ALL(type, buffer, length, nil)
/* TODO
 * TODO */
#define SI_BUF_ALL(type, buffer, length, alloc) \
	(siBuffer){(u8*)buffer, length, length, sizeof(type), alloc}

#if defined(SI_RELEASE_MODE)
	/* TODO
	 * TODO */
	#define si_arrayGet(array, index, type) (((type*)(array).data)[index])
#else
	/* TODO
	 * TODO */
	#define si_arrayGet(array, index, type) (*(type*)si__arrayGet(&(array), index, sizeof(type)))
	inline rawptr si__arrayGet(siBuffer* buffer, usize index, usize sizeof_type);

#endif

/* TODO
 * TODO */
#define si_arrayGetPtr(array, index) (siByte*)((u8*)(array).data + (array).typeSize * (index))

/* TODO
 * TODO */
#define for_eachArr(type, name, array) \
	SI_ASSERT(sizeof(type) == (array).typeSize); \
	for (type* name = &si_arrayGet(array, 0, type); name < (type*)(array).data + (array).len; name += 1) \
/* TODO
 * TODO */
#define for_eachRevArr(type, name, array) \
	SI_ASSERT(sizeof(type) == (array).typeSize); \
	for (type* name = &si_arrayGet(array, (array).len - 1, type); name >= &si_arrayGet(array, 0, type); name -= 1)


/* TODO
 * TODO */
#define si_arrayMake(alloc, type, .../* values */) \
	si_arrayMakeEx((type[]){__VA_ARGS__}, sizeof(type), countof((type[]){__VA_ARGS__}), alloc)
/* TODO */
SIDEF siBuffer si_arrayMakeEx(rawptr list, usize sizeofItem, usize count, siAllocator alloc);
/* TODO */
SIDEF siBuffer si_arrayMakeReserve(usize sizeofItem, usize length, usize capacity,
		siAllocator alloc);
/* Creates a siArray from another siArray. */
SIDEF siBuffer si_arrayCopy(siBuffer array, siAllocator alloc);

/* */
SIDEF void si_arrayFree(siBuffer* array);


/* TODO */
SIDEF b32 si_arrayAt(siBuffer array, usize index, rawptr outPtr);
/* TODO */
SIDEF b32 si_arrayAtFront(siBuffer array, rawptr outPtr);
/* Gets the last character of the sili-string. */
SIDEF b32 si_arrayAtBack(siBuffer array, rawptr outPtr);
/* Returns a pointer to the first element in the array. */
SIDEF rawptr si_arrayBegin(siBuffer array);
/* Returns a pointer to the past-the-end element in the array. */
SIDEF rawptr si_arrayEnd(siBuffer array);



/* TODO */
SIDEF isize si_arrayFind(siBuffer array, rawptr valuePtr);
/* TODO */
SIDEF isize si_arrayFindLast(siBuffer array, rawptr valuePtr);

/* */
SIDEF usize si_arrayFindCount(siBuffer array, rawptr valuePtr);


/* TODO */
SIDEF b32 si_arrayEqual(siBuffer lhs, siBuffer rhs);


/* TODO */
SIDEF b32 si_arrayAppend(siBuffer* array, rawptr valuePtr);
/* TODO */
SIDEF b32 si_arrayAppendEx(siBuffer* array, rawptr valueArray, usize count);
/* Erases the last item in the array. */
SIDEF void si_arrayPop(siBuffer* array);
/* TODO */
SIDEF void si_arrayClear(siBuffer* array);

/* TODO */
SIDEF b32 si_arrayInsert(siBuffer* array, usize index, rawptr value);
/* TODO */
SIDEF b32 si_arrayInsertEx(siBuffer* array, usize index, rawptr valueArray, usize count);
/* TODO */
SIDEF void si_arrayErase(siBuffer* array, usize index);
/* TODO */
SIDEF void si_arrayEraseEx(siBuffer* array, usize index, usize count);

/* TODO */
SIDEF void si_arrayReverse(siBuffer array);
/* TODO */
SIDEF b32 si_arrayFill(siBuffer* array, usize index, usize count, rawptr valuePtr);

/* TODO */
SIDEF void si_arrayReplaceAll(siBuffer array, rawptr restrict valueOld,
		rawptr restrict valueNew);
/* TODO */
SIDEF void si_arrayReplace(siBuffer array, rawptr restrict valueOld,
		rawptr restrict valueNew, isize amount);

/* TODO */
SIDEF b32 si_arrayMakeSpaceFor(siBuffer* buffer, isize addLen);


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

/* TODO */
typedef siBuffer siString;
/* TODO */
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
 * Creates a sili-string literal on the stack from a string literal. */
#define SI_STR(string) SI_STR_LEN(string, countof_str(string))
/* string - STR | len - usize
 * Creates a sili-string literal on the stack from a length-specified C-string. */
#define SI_STR_LEN(string, len) SI_BUF_LEN(u8, string, len)
/* string - char*
 * Creates a sili-string on the stack from a NULL-terminated C-string. */
#define SI_CSTR(string) SI_STR_LEN(string, si_cstrLen(string))
/* string - cstring
 * Creates a compile-time constant sItring literal on the stack from a string literal. */
#define SI_STRC(string) {(u8*)string, countof_str(string), countof_str(string), 1, nil}


/* TODO
 * TODO */
#define si_stringGet(string, index) si_arrayGet(string, index, u8)

/* TODO */
SIDEF siString si_stringMake(cstring string, siAllocator alloc);
/* TODO */
SIDEF siString si_stringMakeLen(cstring string, usize len, siAllocator alloc);
/* TODO */
SIDEF siString si_stringMakeEx(cstring string, usize len, usize capacity,
		siAllocator alloc);
/* TODO */
SIDEF siString si_stringMakeReserve(usize length, usize capacity, siAllocator alloc);

/* TODO */
SIDEF siString si_stringCopy(siString from, siAllocator alloc);
/* */
SIDEF char* si_stringCopyToCstr(siString from, siAllocator alloc);
/* TODO */
SIDEF char* si_stringCloneToCstrEx(siString from, usize capacity, siAllocator alloc);


/* TODO */
SIDEF void si_stringFree(siString* string);


/* Gets the character at a specified index in the sili-string. */
SIDEF i32 si_stringAt(siString string, usize index);
/* Gets the first character of the sili-string. */
SIDEF i32 si_stringAtFront(siString string);
/* Gets the last character of the sili-string. */
SIDEF i32 si_stringAtBack(siString string);
/* Returns a pointer to the first character in the string. */
SIDEF u8* si_stringBegin(siString string);
/* Returns a pointer to the past-the-end element in the string. */
SIDEF u8* si_stringEnd(siString string);


/* string - siString | begin - usize | len - usize
 * Returns a substring of the string from the specified beginning with a length. */
#define si_stringSub(string, begin, len) SI_STR_LEN(&si_stringGet(string, begin), len)
/* string - siString | begin - usize
 * Returns a substring of the string from the specified beginning to the end of
 * string. */
#define si_stringSubToEnd(string, begin) si_stringSub(string, begin, (string).len - (begin))


/* TODO */
SIDEF isize si_stringFind(siString string, siString subStr);
/* TODO */
SIDEF isize si_stringFindByte(siString string, siByte byte);
/* TODO */
SIDEF isize si_stringFindUtf8(siString string, const u8* character, isize* utf8AtIndex);

/* TODO */
SIDEF isize si_stringFindLast(siString string, siString subStr);
/* TODO */
SIDEF isize si_stringFindLastByte(siString string, siByte byte);
/* TODO */
/* TODO(EimaMei): si_stringFindLastUtf8 */

/* Returns the amount of occurences of a substring in the sili-string. */
SIDEF usize si_stringFindCount(siString string, siString subStr);


/* Compares two strings and returns an integer, representing which first byte comes
 * first lexicographically. '-1' for 'lhs', `1` for rhs, '0' if they're equal. */
SIDEF i32 si_stringCompare(siString lhs, siString rhs);
/* Compares two strings and returns a boolean, indicating if they're equal or not.  */
SIDEF b32 si_stringEqual(siString lhs, siString rhs);

/* TODO */
SIDEF b32 si_stringAppend(siString* string, siString other);
/* TODO */
SIDEF b32 si_stringSet(siString* string, siString str);
/* TODO */
SIDEF siString si_stringJoin(siArray(siString) strings, siString separator,
		siAllocator alloc);

/* Cuts the front and back occurences of the provided cut set in the sili-string. */
void si_stringTrim(siString* string, siString cutSet);
/* Cuts the front occurences of the provided cut set in the sili-string. */
void si_stringTrimLeft(siString* string, siString cutSet);
/* Cuts the back occurences of the provided cut set in the sili-string. */
void si_stringTrimRight(siString* string, siString cutSet);
/* Removes any leading or trailing spaces and newlines in the string. */
void si_stringStrip(siString* string);

/* Enquotes the given string. */
b32 si_stringEnquote(siString* string);
/* Pushes a character into the sili-string.
 * NOTE: A new string may be allocated. */
b32 si_stringPush(siString* string, char other);
/* Pops the last character of the sili-string.  */
void si_stringPop(siString* string);
/* Clears the string by setting the length to zero. */
void si_stringClear(siString* str);

/* TODO */
SIDEF b32 si_stringInsert(siString* string, siString subStr, usize index);
/* Erases a substring from the specified index. */
void si_stringErase(siString* string, usize index, usize count);

/* Reverses the entirety of the sili-string */
void si_stringReverse(siString string);

/* TODO */
SIDEF b32 si_stringReplaceAll(siString* string, siString strOld, siString strNew,
		siAllocator alloc);
/* TODO */
SIDEF b32 si_stringReplace(siString* string, siString strOld, siString strNew,
		isize amount, siAllocator alloc);
/* Splits the string into multiple parts based on the separator and returns either
 * an allocated array of sub-strings or 'nil' if no delimiter was found. */
SIDEF siArray(siString) si_stringSplit(siString string, siString delimiter,
	siAllocator alloc);
/* Splits the string a specified amount based on the separator and returns either
 * an allocated array of sub-strings or 'nil' if no delimiter was found. */
SIDEF siArray(siString) si_stringSplitEx(siString string, siString delimiter,
	isize amount, siAllocator alloc);

/* TODO */
SIDEF b32 si_stringMakeSpaceFor(siString* string, isize addLen);

#if !defined(SI_NO_MEMORY)

/* TODO */
SIDEF rawptr si_memcopyStr(void* dst, siString src);
/* TODO */
SIDEF rawptr si_memmoveStr(void* dst, siString src);
/* TODO */
SIDEF i32 si_memcompareStr(const void* dst, siString src);

#endif

/* TODO */
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
 * integer to a string. If 'upper' is set to true, converting '15' to base 16
 * would result in "FF", otherwise "ff". By default this is set to true. */
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
SIDEF siString si_stringFromUInt(u64 num, siAllocator alloc);
/* Creates a string from the specified unsigned integer in the given base. The
 * string gets allocated in the specified allocator. */
SIDEF siString si_stringFromUIntEx(u64 num, i32 base, siAllocator alloc);
/* Creates a string from the specified signed integer in base 10. The string
 * gets allocated in the specified allocator. */
SIDEF siString si_stringFromInt(i64 num, siAllocator alloc);
/* Creates a string from the specified signed integer in the given base. The
 * string gets allocated in the specified allocator. */
SIDEF siString si_stringFromIntEx(i64 num, i32 base, siAllocator alloc);


/* Creates a string from a float. By default the output base is base 10 and 6
 * decimals after the decimal point. */
SIDEF siString si_stringFromFloat(f64 num, siAllocator alloc);
/* Creates a string from a float, using the specified base and amount of decimals
 * after the decimals point. String gets allocated unless it's "inf" or "nan". */
SIDEF siString si_stringFromFloatEx(f64 num, i32 base, u32 afterPoint, siAllocator alloc);
/* TODO(EimaMei): f64 si_cstr_to_f64(cstring str); */

/* TODO */
SIDEF siString si_stringFromBool(b32 boolean);
/* TODO */
SIDEF b32 si_stringToBool(siString string);


/* TODO */
SIDEF siString si_stringFromArray(siBuffer array, cstring fmt);


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
typedef siString siUtf8String;
/* Denotes that the string is UTF-16 encoded. */
typedef siBuffer siUtf16String;


/* A UTF-32 encoded '�' character for reporting invalid states. */
#define SI_UTF32_REPLACEMENT_CHARACTER (siUtf32Char){0xFFFD, 3}
/* A UTF-8 encoded '�' character for reporting invalid states. */
#define SI_UTF8_REPLACEMENT_CHARACTER (siUtf8Char){{0xEF, 0xBF, 0xBD}, 3}


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
SIDEF siUtf16String si_utf8ToUtf16Str(u16* outBuffer, usize capacity, siUtf8String str);
/* Encodes a UTF-8 string into a UTF-16 string with an option to make it
 * NULL-terminated. */
SIDEF siUtf16String si_utf8ToUtf16StrEx(u16* outBuffer, usize capacity,
		siUtf8String str, b32 addNullTerminator);
/* Encodes a UTF-16 string into a UTF-8 string _without_ a NULL-terminator. */
SIDEF siUtf8String si_utf16ToUtf8Str(u8* outBuffer, usize capacity, siUtf16String str);
/* Encodes a UTF-16 string into a UTF-8 string with an option to make it
 * NULL-terminated. */
SIDEF siUtf8String si_utf16ToUtf8StrEx(u8* outBuffer, usize capacity, siUtf16String str,
		b32 addNullTerminator);


/* Decodes the given UTF-16 character into UTF-32 and returns a 'siUtf32Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf32Char){SI_UNICODE_INVALID, 0}. */
SIDEF siUtf32Char si_utf16Decode(const u16 character[2]);
/* Decodes the given UTF-16 character into UTF-8 and returns a 'siUtf8Char' structure.
 * If the specified character is invalid, the function returns a '(siUtf8Char){SI_UNICODE_INVALID, 0}. */
SIDEF siUtf8Char si_utf16Encode(const u16 character[2]);

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

#if !defined(SI_NO_HASHTABLE)
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
	siFileMode_Read = SI_BIT(0),
	siFileMode_Write = SI_BIT(1),
	siFileMode_Append = SI_BIT(2),
	siFIleMode_Plus = SI_BIT(3),

	siFileMode_All = siFileMode_Read | siFileMode_Write | siFileMode_Append | siFIleMode_Plus
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

#define si_stdin  si_fileGetStdFile(siStdFile_Input)
#define si_stdout si_fileGetStdFile(siStdFile_Output)
#define si_stderr si_fileGetStdFile(siStdFile_Error)

SI_ENUM(i32, siFileError) {
	/* An unspecified error occurred. */
	siFileError_Generic = -1,
	/* No error. */
	siFileError_None = 0,

	/* Invalid value. */
	siFileError_Invalid,
	/* Invalid file name. */
	siFileError_InvalidFilename,
	/* File/directory already exists. */
	siFileError_Exists,
	/* File/directory doesn't exist. */
	siFileError_NotExists,
	/* Insufficient permissions. */
	siFileError_Permission,
	/* Failed to truncate the file. */
	siFileError_TruncationFail
};

#if SI_SYSTEM_IS_WINDOWS
	const char SI_PATH_SEPARATOR = '\\';
#else
	const char SI_PATH_SEPARATOR = '/';
#endif

/* TODO */
SIDEF siString si_pathFsErrorName(siFileError err);
/* TODO */
SIDEF siString si_pathFsErrorDesc(siFileError err);

/*
	========================
	|  siPath              |
	========================
*/

/* TODO */
SIDEF b32 si_pathExists(siString path);
/* TODO */
SIDEF siResult(usize) si_pathCopy(siString pathSrc, siString pathDst);
/* Copies every file and folder from 'pathSrc' to 'pathDst'. Returns the amount
 * of items that were copied. */
SIDEF u32 si_pathItemsCopy(siString pathSrc, siString pathDst);
/* TODO */
SIDEF siError si_pathMove(siString pathSrc, siString pathDst);
/* TODO */
SIDEF siError si_pathRename(siString nameOld, siString nameNew);
/* TODO */
SIDEF siError si_pathCreateFolder(siString path);
/* TODO */
SIDEF siError si_pathCreateFolderEx(siString path, siFilePermissions perms);
/* TODO */
SIDEF siError si_pathRemove(siString path);
/* TODO */
SIDEF siError si_pathCreateHardLink(siString path, siString linkPath);
/* TODO */
SIDEF siError si_pathCreateSoftLink(siString path, siString linkPath);
/* TODO */
SIDEF siError si_pathEditPermissions(siString path, siFilePermissions newPerms);

/* TODO */
SIDEF siString si_pathBaseName(siString path);
/* TODO */
SIDEF siString si_pathExtension(siString path);
/* TODO */
SIDEF siString si_pathUnrooted(siString path);
/* TODO */
SIDEF siResult(siString) si_pathGetFullName(siString path, siAllocator alloc);
/* TODO */
SIDEF u64 si_pathLastWriteTime(siString path);
/* TODO */
SIDEF siString si_pathGetTmp(void);
/* TODO */
SIDEF siFilePermissions si_pathPermissions(siString path);

/* TODO */
SIDEF siString si_pathReadContents(siString path, siAllocator alloc);
/* TODO */
SIDEF siError si_pathReadContentsBuf(siString path, siBuffer* outBuffer);

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
siFile* si_fileGetStdFile(siStdFile type);

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
SIDEF siResult(siBuffer) si_fileRead(siFile file, usize len, siAllocator alloc);
SIDEF siError si_fileReadBuf(siFile file, usize len, siBuffer* outBuffer);
/* TODO */
SIDEF siResult(siBuffer) si_fileReadAt(siFile file, isize offset, usize len, siAllocator alloc);
SIDEF siError si_fileReadAtBuf(siFile file, isize offset, usize len, siBuffer* outBuffer);

/* TODO */
SIDEF siError si_fileReadUnsafe(siFile file, isize offset, usize len, siBuffer* outBuffer);

/* TODO */
SIDEF siString si_fileReadContents(siFile file, siAllocator alloc);
SIDEF siError si_fileReadContentsBuf(siFile file, siString* outBuffer);

/* Reads the contents of the file, then splits every line and writes them all
 * into a 'siArray(siString)'. */
SIDEF siArray(siString) si_fileReadlines(siFile file, siAllocator alloc);


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
isize si_fileSeek(siFile file, isize offset, siSeekWhere method);
/* Seeks to the front of the file. Returns 'true' if the operation went through. */
b32 si_fileSeekFront(siFile file);
/* Seeks to the back of the file. Returns 'true' if the operation went through. */
b32 si_fileSeekBack(siFile file);

/* Truncates the file to the specified size. Returns 'true' if the operation went
 * through. */
SIDEF b32 si_fileTruncate(siFile* file, usize size);
/* TODO */
SIDEF u64 si_fileLastWriteTime(siFile file);

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
	siError error;
	rawptr handle;
	usize directoryLen;
	char buffer[SI_MAX_PATH_LEN];
} siDirectory;


/* TODO */
SIDEF siDirectory si_directoryOpen(siString path);
/* TODO */
SIDEF b32 si_directoryPollEntry(siDirectory* dir, siDirectoryEntry* outEntry);
/* TODO */
SIDEF b32 si_directoryPollEntryEx(siDirectory* dir, siDirectoryEntry* outEntry, b32 fullPath);


#endif /* SI_NO_IO */

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

/* Represents a pointer to a thread function. */
typedef rawptr (*siFunction)(rawptr);

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


/* Creates a 'siThread' data strucutre and if specified, immediately runs the
 * thread.  If the function fails to create the thread, the returned thread's
 * 'id' member is set to 'nil', as well as 'isRunning'.*/
SIDEF b32 si_threadMake(siFunction function, rawptr arg, b32 runThread,
		siThread* outThread);
/* Creates a 'siThread' data strucutre with the given stack size and if specified,
 * immediately runs the thread. */
SIDEF b32 si_threadMakeEx(siFunction function, rawptr arg, usize stackSize,
		b32 runThread, siThread* outThread);

/* thread - siThread | type - TYPE
 * Casts 'thread.returnValue' correctly to match the given type instead of it being
 * rawptr. */
#define si_threadGetReturn(thread, type) si_transmute(type, (thread).returnValue, rawptr)

/* Runs the thread. */
b32 si_threadRun(siThread* t);
/* Starts the thread while hanging the parent thread until the specified thread
 * is finished. */
b32 si_threadJoin(siThread* t);

/* Destroys the thread. */
b32 si_threadDestroy(siThread* t);

/* Sets the priority of the thread. */
b32 si_threadPrioritySet(siThread t, i32 priority);

#endif /* SI_NO_THREAD */

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
#define SI_CLOCKS_S (1000000000)
/* How many clocks of 'si_clock()' are equivalent to a milisecond (1ms = 1e+6 ns). */
#define SI_CLOCKS_MS (1000000)
/* How many clocks of 'si_clock()' are equivalent to a microsecond (1µs = 1e+3 ns). */
#define SI_CLOCKS_US (1000)

/* An unsigned 64-bit integer type, notifying that it's used for time stamps. */
typedef u64 siTimeStamp;
/* A signed 64-bit integer type, notifying that it's used for UTC-0 time. */
typedef i64 siUtcTime;
/* A signed 64-bit integer type, notifying that it's used for local time. */
typedef i64 siLocalTime;

/* Executes an 'RDTSC' equivalent asm instruction and returns the value in nanoseconds.
 * NOTE: Only natively works for AMD64, i386, ARM64 and PPC CPUs. On other CPUs
 * the function relies on OS functions like 'gettimeofday'. */
SIDEF siTimeStamp si_RDTSC(void);
/* Returns the number of seconds since 1970-01-01 UTC */
SIDEF siUtcTime si_timeNowUTC(void);
/* Returns the number of seconds since 1970-01-01 local time.*/
SIDEF siLocalTime si_timeNowLocal(void);

/* Returns the current clock. */
SIDEF u64 si_clock(void);

/* Starts the timestamp. */
SIDEF siTimeStamp si_timeStampStart(void);
/* time - siTimeStamp
 * Prints the time since the start. */
#define si_timeStampPrintSince(time) si_timeStampPrintSinceEx(time, __FILE__, __LINE__)

/* Makes the CPU sleep for a certain amount of miliseconds. */
SIDEF void si_sleep(u32 miliseconds);


#if 1
	void si_timeStampPrintSinceEx(siTimeStamp t, cstring filename, i32 line);
#endif

#endif /* SI_NO_TIME */

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

/* TODO(EimaMei): Remove this later and rework the functions that utilize it. */
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

/* x - INT
 * Returns true if x is negative. */
#define si_numIsNeg(x) ((x) < 0)

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


#if 1 /* NOTE(EimaMei): The actual header definition for the macros. No reason to use these in practice. */
	u32 si_numLeadingBitEx(u64 num, usize sizeof_num, siBitType bit);
	u32 si_numTrailingBitEx(u64 num, usize number_sizeof, siBitType bit);

	u64 si_numRotateLeftEx(u64 num, usize num_sizeof, usize bits);
	u64 si_numRotateRightEx(u64 num, usize num_sizeof, usize n);
	u64 si_numReverseBitsEx(u64 num, usize num_sizeof);

	siArray(u8) si_numToBytesEx(u64 num, usize num_sizeof, siAllocator alloc);
#endif

#endif /* SI_NO_BIT */

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
 * to 'SI_CPU_FREQ_MHZ'. */
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
	siPrintColorType_3bit = 1,
	siPrintColorType_8bit,
	siPrintColorType_24bit,
};

SI_ENUM(u8, siPrintColorAnsi) {
	siPrintColorAnsi_Black,
	siPrintColorAnsi_Red,
	siPrintColorAnsi_Green,
	siPrintColorAnsi_Yellow,
	siPrintColorAnsi_Blue,
	siPrintColorAnsi_Magenta,
	siPrintColorAnsi_Cyan,
	siPrintColorAnsi_White
};

typedef struct siPrintColor {
	siPrintColorType type;
	union_anonymous (data,
		struct {
			siPrintColorAnsi color;
			b8 bold;
			b8 light;
		} ansi;
		u8 cube;
		u8 rgb[3];
	);
} siPrintColor;

/* TODO
 * TODO */
#define si_printColor3bit(color) si_printColor3bitEx(color, 0, 0)
/* TODO
 * TODO */
#define si_printColor3bitEx(color, bold, light) \
	(siPrintColor){siPrintColorType_3bit, {.ansi = {color, bold, light}}}

/* TODO
 * TODO */
#define si_printColor8bit(color) \
	(siPrintColor){siPrintColorType_8bit, {.cube = color}}

/* TODO
 * TODO */
#define si_printColor24bit(r, g, b) \
	(siPrintColor){siPrintColorType_24bit, {.rgb[0] = r, .rgb[1] = g, .rgb[2] = b}}


/* Writes a NULL-terminated C-string into si_stdout. Returns the amount of
 * written bytes. */
SIDEF isize si_print(cstring str);
/* Writes a NULL-terminated formatted C-string into si_stdout. Returns the amount
 * of written bytes. */
SIDEF isize si_printf(cstring fmt, ...);
SIDEF isize si_printfVa(cstring fmt, va_list va);

/* Writes a NULL-terminated C-string into a specified file. Returns the amount of
 * written bytes. */
SIDEF isize si_fprint(siFile* file, cstring str);
/* Writes a NULL-terminated formatted C-string into a specified file. Returns the
 * amount of written bytes. */
SIDEF isize si_fprintf(siFile* file, cstring fmt, ...);
SIDEF isize si_fprintfVa(siFile* file, cstring fmt, va_list va);

/* TODO */
SIDEF isize si_sprintf(char* buffer, cstring fmt, ...);
SIDEF isize si_sprintfVa(char* buffer, cstring fmt, va_list va);

/* TODO */
SIDEF isize si_snprintf(char* buffer, usize outCapacity, cstring fmt, ...);
SIDEF isize si_snprintfVa(char* buffer, usize outCapacity, cstring fmt, va_list va);

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

/* dll - siDllHandle | function - FUNCTION |  type - TYPE
 * Loads the specified function's name as a processor and returns it as an ISO-C
 * friendly function. */
#define si_dllProcAddressFunc(dll, function, type) \
	si_transmute(type, si_dllProcAddress(dll, #function), siDllProc)

#if defined(SI_TYPEOF_USED)
/* dll - siDllHandle | function - FUNCTION
 * Loads the specified function's name as a processor and returns it as an ISO-C
 * friendly function. */
	#define si_dllProcAddressFuncType(dll, function) \
		si_dllProcAddressFunc(dll, function, typeof(function))
#endif

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

/* TODO */
#define si_sin(x) (sizeof(x) == 4 ? si_sinF32((f32)(x)) : si_sinF64((f64)(x)))
/* TODO */
#define si_cos(x) (sizeof(x) == 4 ? si_cosF32((f32)(x)) : si_cosF64((f64)(x)))

/* Rounds 'x' to the nearest integer. */
#define si_round(x) (sizeof(x) == 4 ? si_roundF32((f32)(x)) : si_roundF64((f64)(x)))

/* Calculates the largest integer not greater than 'x'. */
#define si_floor(x) (sizeof(x) == 4 ? si_floorF32((f32)(x)) : si_floorF64((f64)(x)))

/* Calculates the smallest integer not less than 'x'. */
#define si_ceil(x) (sizeof(x) == 4 ? si_ceilF32((f32)(x)) : si_ceilF64((f64)(x)))

/* Raises an unsigned base 10 by the power of the exponent, and returns the result
 * as a 64-bit unsigned int (meaning the exponent can only be from 0 to 19, otherwise
 * the app will crash). */
u64 si_pow10(u32 exponent);
/* Raises a f64 10 by the power of the exponent. The exponent must be in-between
 * -18 to 18, otherwise the app will crash.*/
f64 si_pow10F64(i32 exponent);

/* TODO */
SIDEF b32 si_float64IsNan(f64 num);
/* TODO */
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

	SI_MATH_FUNC_DECLARE_2X(min, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_2X(max, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_SIGNED(abs, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_3X_B32(between, SIDEF, ;)

	SIDEF f64 si_sinF64(f64 x);
	SIDEF f32 si_sinF32(f32 x);
	SIDEF f64 si_cosF64(f64 x);
	SIDEF f32 si_cosF32(f32 x);
	SIDEF f64 si_roundF64(f64 x);
	SIDEF f32 si_roundF32(f32 x);
	SIDEF f32 si_floorF32(f32 x);
	SIDEF f64 si_floorF64(f64 x);
	SIDEF f64 si_ceilF64(f64 x);
	SIDEF f32 si_ceilF32(f32 x);

#endif

#endif


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

typedef struct siBenchmarkInfo {
	cstring name;
	siArray(u64) cycles;
} siBenchmarkInfo;


/* TODO */
SIDEF void si_benchmarkLoopsAvgPrint(siBenchmarkInfo info, usize range[2]);

/* TODO */
SIDEF void si_benchmarkLoopsAvgCmpPrint(siBenchmarkInfo info[2], usize range[2]);

/* TODO */
rawptr si__BenchmarkThread(rawptr arg);

/* timesToLoop - usize | function - NAME
 * Runs the function 'timesToLoop' times and prints how long it took to finish. */
#define si_benchmarkRunsPerLoop(timesToLoop, function) \
	do { \
		u64 timeSince = si_RDTSC(); \
		for_range (i, 0, timesToLoop) { \
			function; \
		} \
		timeSince = si_RDTSC() - timeSince; \
		f64 timeTaken = (f64)timeSince / (f64)si_cpuClockSpeed() / 1000.0; /* NOTE(EimaMei): This will take the time in ms, not seconds. */ \
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
		si_threadMake(si__BenchmarkThread, &miliseconds, true, &thread); \
		while (thread.isRunning) { \
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
		siAllocator alloc = si_allocatorMakeStack(SI_KILO(4)); \
		siBenchmarkInfo arr = {#function, si_arrayMakeReserve(sizeof(u64), 0, 20, alloc)}; \
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
		siAllocator si_alloc = si_allocatorMakeStack(SI_KILO(4)); \
		siBenchmarkInfo si_arr[2] = { \
			{#function1, si_arrayMakeReserve(sizeof(u64), 0, 20, si_alloc)}, \
			{#function2, si_arrayMakeReserve(sizeof(u64), 0, 20, si_alloc)}  \
		};\
		si_benchmarkLoop(function1, si_arr[0].cycles, start, end); \
		si_benchmarkLoop(function2, si_arr[1].cycles, start, end); \
		\
		si_benchmarkLoopsAvgCmpPrint(si_arr, (usize[]){start, end}); \
	} while(0)

typedef struct { siTimeStamp duration; cstring unit; } siBenchmarkLimit;

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

const siBenchmarkLimit* si_benchmarkLimitLoop(siTimeStamp time);

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

#ifndef SI_NO_PRINT
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

rawptr si_allocator_heap_proc(siAllocatorFunc func, rawptr userData) {
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

		case siAllocatorFunc_Reset: {
			SI_PANIC_MSG("Cannot reset a heap allocator.");
		} break;

		case siAllocatorFunc_GetAvailable: {
			out = (rawptr)USIZE_MAX;
			si_panic("des", "nas", 1, "cring", "sd");
		} break;

		default: SI_PANIC();
	}

	SI_UNUSED(userData);
	return out;
}

rawptr si_allocator_arena_proc(siAllocatorFunc func, rawptr userData) {
	siAllocatorData* alloc = (siAllocatorData*)userData;
	SI_ASSERT_MSG(alloc->ptr != nil, "You cannot use an already freed arena.");
	rawptr out;

	switch (func.type) {
		case siAllocatorFunc_Alloc: {
			usize bytes = si_alignCeil(func.data.alloc);
			out = &alloc->ptr[alloc->offset];

			alloc->offset += bytes;
			if (alloc->offset > alloc->capacity) {
				SI_PANIC_FMT(
					"Exceeded the available memory for an allocation (Tried writing '%zd' bytes "
					"into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
					bytes, alloc->capacity, alloc->offset
				);
			}
		} break;

		case siAllocatorFunc_Free: {
			out = nil;
			//SI_PANIC_MSG("Unimplemented feature.");
		} break;

		case siAllocatorFunc_FreeAll: {
			si_mfree(alloc->ptr);
			si_memset(alloc, 0, sizeof(*alloc));
			out = nil;
		} break;

		case siAllocatorFunc_Resize: {
			siAllocatorDataResize resize = func.data.resize;
			SI_STOPIF(resize.sizeOld >= resize.sizeNew, return resize.src);

			func.type = siAllocatorFunc_Alloc;
			func.data.alloc = resize.sizeNew;
			if (resize.src == nil) {
				return si_allocator_arena_proc(func, userData);
			}

			out = si_allocator_arena_proc(func, userData);
			si_memcopy(out, resize.src, resize.sizeOld);
		} break;

		case siAllocatorFunc_Reset: {
			alloc->offset = 0;
			out = nil;
		} break;

		case siAllocatorFunc_GetAvailable: {
			out = (rawptr)(alloc->capacity - alloc->offset);
		} break;

		default: SI_PANIC();
	}

	return out;
}

rawptr si_allocator_stack_proc(siAllocatorFunc func, rawptr userData) {
	siAllocatorData* alloc = (siAllocatorData*)userData;
	if (func.type == siAllocatorFunc_FreeAll && alloc->ptr) {
		si_memset(alloc, 0, sizeof(*alloc));
		return nil;
	}

	return si_allocator_arena_proc(func, userData);
}

inline
siAllocator si_allocatorHeap(void) {
	siAllocator alloc;
	alloc.proc = si_allocator_heap_proc;
	alloc.userData = nil;

	return alloc;
}


SIDEF
siAllocator si_allocatorMakeArena(usize capacity, siAllocatorData* inData) {
	SI_ASSERT_NOT_NULL(inData);

	capacity = si_alignCeil(capacity);
	inData->ptr = si_mallocArray(siByte, capacity);
	inData->offset = 0;
	inData->capacity = capacity;

	siAllocator alloc;
	alloc.proc = si_allocator_stack_proc;
	alloc.userData = inData;

	return alloc;
}

SIDEF
siAllocator si__allocatorMakeStack(usize capacity, siAllocatorData* inData, siByte* __stack) {
	SI_ASSERT_NOT_NULL(inData);
	inData->ptr = __stack;
	inData->offset = 0;
	inData->capacity = capacity;

	siAllocator alloc;
	alloc.proc = si_allocator_stack_proc;
	alloc.userData = inData;

	return alloc;
}

SIDEF
siAllocator si_allocatorMakeTmp(rawptr ptr, usize capacity, siAllocatorData* inData) {
	SI_ASSERT_NOT_NULL(inData);
	inData->ptr = (siByte*)ptr;
	inData->offset = 0;
	inData->capacity = capacity;

	siAllocator alloc;
	alloc.proc = si_allocator_stack_proc;
	alloc.userData = inData;

	return alloc;
}



inline
rawptr si_alloc(siAllocator alloc, usize bytes) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_Alloc;
	func.data.alloc = bytes;

	return alloc.proc(func, alloc.userData);
}

inline
void si_free(siAllocator alloc, rawptr ptr) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_Free;
	func.data.free = ptr;

	alloc.proc(func, alloc.userData);
}

inline
void si_freeAll(siAllocator alloc) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_FreeAll;

	alloc.proc(func, alloc.userData);
}

inline
rawptr si_realloc(siAllocator alloc, rawptr ptr, usize sizeOld, usize sizeNew) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_Resize;
	func.data.resize = (siAllocatorDataResize){ptr, sizeOld, sizeNew};

	return alloc.proc(func, alloc.userData);
}


inline
void si_allocatorReset(siAllocator alloc) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_Reset;

	alloc.proc(func, alloc.userData);
}

inline
usize si_allocatorGetAvailable(siAllocator alloc) {
	siAllocatorFunc func;
	func.type = siAllocatorFunc_GetAvailable;

	return (usize)alloc.proc(func, alloc.userData);
}




noreturn SIDEF
void si_panic(cstring conditionStr, cstring file, i32 line, cstring func,
		cstring message, ...) {
	siPrintColor red = si_printColor3bitEx(siPrintColorAnsi_Red, true, false);

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

SIDEF
siUtcTime si_timeNowUTC(void) {
	time_t rawtime;
	time(&rawtime);

	return rawtime;
}
inline
siLocalTime si_timeNowLocal(void) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return (i64)ft.dwHighDateTime | ((i64)ft.dwLowDateTime << 32);

#else
	struct timeval tv;
	int res = gettimeofday(&tv, nil);
	SI_STOPIF(res == 1, return 0);

	return tv.tv_sec;
#endif
}

inline
void si_sleep(u32 miliseconds) {
	SI_STOPIF(miliseconds == 0, return);

#if SI_SYSTEM_IS_WINDOWS
	Sleep((u32)miliseconds);

#elif _POSIX_C_SOURCE >= 199309L
	struct timespec ts = {
		(time_t)miliseconds / 1000,
		si_cast(time_t, miliseconds % 1000) * 1000000
	};
	nanosleep(&ts, &ts);

#endif
}


inline
siTimeStamp si_RDTSC(void) {
	/* NOTE(EimaMei): Shoutout to gb.h for the i386 and PPC code! (Link: https://github.com/gingerBill/gb/blob/master/gb.h#L8682C1-L8715C7). */
#if !defined(SI_NO_INLINE_ASM)
	#if SI_COMPILER_CHECK_MIN(MSVC, 12, 0, 0)
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


inline
u64 si_clock(void) {
	return si_RDTSC() / si_cpuClockSpeed() * 1000;
}


inline
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

void si_errorLogDefault(siError* error) {
	siPrintColor red = si_printColor3bitEx(siPrintColorAnsi_Red, true, false);
	si_fprintf(
		si_stderr, "%CError at \"%s:%i\"%C: Number '%i'.\n",
		red, error->filename, error->line, error->code
	);
}

#endif /* SI_IMPLEMENTATION_GENERAL */

#if defined(SI_IMPLEMENTATION_MEMORY) && !defined(SI_NO_MEMORY)


SIDEF
rawptr si_memcopy(void* restrict dst, const void* restrict src, usize size) {
	SI_ASSERT_NOT_NULL(dst);
	SI_ASSERT_NOT_NULL(src);

#if !defined(SI_NO_CRT)
	return memcpy(dst, src, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif
}

SIDEF
rawptr si_memmove(void* dst, const void* src, isize size) {
	SI_ASSERT_NOT_NULL(dst);
	SI_ASSERT_NOT_NULL(src);

#if !defined(SI_NO_CRT)
	return memmove(dst, src, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif

}

SIDEF
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
	return (rawptr)memchr(data, value, size);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif
}


SIDEF
usize si_cstrLen(cstring string) {
	SI_ASSERT_NOT_NULL(string);

#if !defined(SI_NO_CRT)
	return strlen(string);
#else
	SI_PANIC_MSG("This function doesn't have an implementation for SI_NO_CRT.");
#endif

}


#endif /* SI_IMPLEMENTATION_GENERAL */


#if defined(SI_IMPLEMENTATION_ARRAY) && !defined(SI_NO_ARRAY)

#if !defined(SI_RELEASE_MODE)
rawptr si__arrayGet(siBuffer* buffer, usize index, usize sizeof_type) {
	SI_ASSERT_MSG(sizeof_type == buffer->typeSize, "Invalid type size.");
	SI_ASSERT_MSG(index < buffer->capacity, "Invalid index.");
	return (siByte*)buffer->data + sizeof_type * index;
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
	array.data = si_allocArray(alloc, siByte, sizeofItem * capacity);

	return array;
}

inline
siBuffer si_arrayCopy(siBuffer array, siAllocator alloc) {
	return si_arrayMakeEx(array.data, array.typeSize, array.len, alloc);
}


inline
void si_arrayFree(siBuffer* array) {
	si_free(array->alloc, array->data);
	si_memset(array, 0, sizeof(*array));
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

	siByte* dst = si_arrayGetPtr(*array, index);
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
	siByte* dst = si_arrayGetPtr(*array, index);
	siByte* src = si_arrayGetPtr(*array, index + count);
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
	siByte* a = (siByte*)array.data;
	siByte* b = si_arrayGetPtr(array, array.len - 1);

	usize len = array.len / 2;
	siByte* tmp = (siByte*)si_salloc(128);
	SI_ASSERT(array.typeSize <= 128);

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

	siByte* dst = si_arrayGetPtr(*array, index);
	for_range (i, 0, count) {
		si_memcopy(dst, valuePtr, array->typeSize);
		dst += array->typeSize;
	}

	return allocated;
}



SIDEF
b32 si_arrayMakeSpaceFor(siBuffer* buffer, isize addLen) {
	SI_ASSERT_NOT_NULL(buffer);
	SI_ASSERT_NOT_NULL(buffer->alloc.proc);

	isize newLength = (isize)buffer->len + addLen;
	if (newLength <= (isize)buffer->capacity) {
		buffer->len = newLength;
		return false;
	}

	usize newCapacity = (buffer->capacity + addLen) * 2;
	buffer->data = si_realloc(
		buffer->alloc, buffer->data,
		buffer->capacity * buffer->typeSize, newCapacity * buffer->typeSize
	);
	buffer->len = newLength;
	buffer->capacity = newCapacity;

	return true;
}

#endif /* SI_IMPLEMENTATION_ARRAY */

#if defined(SI_IMPLEMENTATION_STRING) && !defined(SI_NO_STRING)



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
void si_stringFree(siString* string) {
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
			usize index = letter - (u8*)string.data;
			return index - (subStr.len - 1);
		}
	}

	return -1;
}

inline
isize si_stringFindByte(siString string, siByte byte) {
	for_eachStr (letter, string) {
		if (*letter == byte) {
			return letter - (u8*)string.data;
		}
	}

	return -1;
}

SIDEF
isize si_stringFindUtf8(siString string, const u8* character, isize* utf8AtIndex) {
	usize i = 0;
	isize utf8I = 0;
	i32 codepoint = si_utf8Decode(character).codepoint;

	while (i < string.len) {
		siUtf32Char x = si_utf8Decode(&si_stringGet(string, i));
		if (x.codepoint == codepoint) {
			*utf8AtIndex = utf8I;
			return i;
		}

		utf8I += 1;
		i += x.len;
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
isize si_stringFindLastByte(siString string, siByte byte) {
	for_eachRevStr (letter, string) {
		if (*letter == byte) {
			return letter - (u8*)string.data;
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
	SI_STOPIF(strings.len == 0, return SI_STR_LEN(nil, 0));

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
		string->len = posEnd - posStart;

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

	string->len = posEnd - posStart + 1;
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
	si_ptrMoveRight(data, oldLen, 1);
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

	si_stringFree(string);
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
	SI_STOPIF(amount == 0, return (siString){0});


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

SIDEF
b32 si_stringMakeSpaceFor(siString* string, isize addLen) {
	SI_ASSERT_NOT_NULL(string);
	SI_ASSERT_NOT_NULL(string->alloc.proc);

	isize newLength = (isize)string->len + addLen;
	if (newLength <= (isize)string->capacity) {
		string->len = newLength;
		return false;
	}

	usize newCapacity = (string->capacity + addLen) * 2;
	string->data = si_realloc(string->alloc, string->data, string->capacity, newCapacity);
	string->len = newLength;
	string->capacity = newCapacity;

	return true;
}


#if defined(SI_IMPLEMENTATION_MEMORY)

inline rawptr si_memcopyStr(void* dst, siString src) { return si_memcopy(dst, src.data, src.len); }
inline rawptr si_memmoveStr(void* dst, siString src) { return si_memmove(dst, src.data, src.len); }
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

	u8* data = (u8*)string.data;
	data[0] = si_charUpper(data[0]);
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
	u8* back = &si_stringGet(res, len - 1);

	do {
		*back = SI_NUM_TO_CHAR_TABLE[num % base];
		num /= base;
		back -= 1;
	} while (num > 0);

	return res;
}
SIDEF
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
	SI_STOPIF(string.len == 0, *outRes = false; return 0);

	/* NOTE(EimaMei): No base specified, meaning it's base-10. */
	i32 front = si_stringAtFront(string);
	if (front != '0' && front != '%') {
		if (string.len > 20) {
			*outRes = false;
			return 0;
		}
		*outRes = true;
		return si_stringToUInt(string);
	}

	u64 res = 0;
	i32 base;

	char x = si_stringGet(string, 1) & ~SI_BIT(5);
	u32 maxDigits;
	switch (x) {
		case '%':
		case 'X': base = 16; string = si_stringSubToEnd(string, 2); maxDigits = 16; break;
		case 'O': base =  8; string = si_stringSubToEnd(string, 2); maxDigits = 22; break;
		case 'B': base =  2; string = si_stringSubToEnd(string, 2); maxDigits = 64; break;
		default:  base =  8; string = si_stringSubToEnd(string, 1); maxDigits = 22; break;
	}
	SI_STOPIF(string.len > maxDigits, *outRes = false; return res);

	for_eachStr (letter, string) {
		res *= base;

		i32 value = *letter - '0';
		if (value < 10) {
			res += value;
		}
		else {
			value = si_charUpper((char)value) - 7;
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
siString si_stringFromInt(i64 num, siAllocator alloc) {
	return si_stringFromIntEx(num, 10, alloc);
}
SIDEF
siString si_stringFromIntEx(i64 num, i32 base, siAllocator alloc) {
	b32 isNegative = si_numIsNeg(num);
	usize len = si_numLenI64Ex(num, base);
	siString res = si_stringMakeReserve(len, len, alloc);

	/* NOTE(EimaMei): We build the string from the back (not the front) so that
	 * we wouldn't have to reverse the string after we make the string. */
	u8* back = &si_stringGet(res, len - 1);

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
		: si_round(num);
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

	f64 numFractional = num - si_floor(num);
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
	static const siString values[] = { SI_STRC("false"), SI_STRC("true") };
	return values[boolean & true];
}

SIDEF
b32 si_stringToBool(siString string) {
	SI_ASSERT_NOT_NULL(string.data);
	SI_STOPIF(string.len == 0 || (string.len != 1 && string.len != 4 && string.len != 5), return UINT32_MAX);

	if (string.len == 1) {
		switch (*(u8*)string.data | SI_BIT(5)) {
			case '1': case 't': return true;
			case '0': case 'f': return false;
			default: return UINT32_MAX;
		}
	}

	char str[5];
	for_range (i, 0, countof(str)) {
		str[i] = si_stringGet(string, i) | SI_BIT(5);
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

SIDEF
siString si_stringFromArray(siBuffer array, cstring fmt) {
	u64 args[16];
	u32 argSizes[16];
	usize argCount = 0;

	#define si__every_arg args[0], args[1], args[2], args[3], args[4], args[5], \
		args[6], args[7], args[8], args[9], args[10], args[11], args[12], args[13], \
		args[14], args[15]

	char buffer[SI_KILO(4)];
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
		SI_ASSERT(argCount <= 16);
	}

	buffer[0] = '{', length += 1;
	for_range (i, 0, array.len) {
		siByte* base = (siByte*)si_arrayGetPtr(array, i);
		for_range (j, 0, argCount) {
			usize argSize = argSizes[j];

			si_memcopy(&args[j], base, argSize);
			base += argSize;
		}
		length += si_snprintf(&buffer[length], sizeof(buffer) - length, fmt, si__every_arg) - 1;

		if ((usize)i < array.len - 1) {
			buffer[length + 0] = ',';
			buffer[length + 1] = ' ';
			length += 2;
		}
	}
	buffer[length] = '}';
	length += 1;

	return SI_STR_LEN(buffer, length);
#undef si__every_arg
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
SIDEF
i64 si_stringToIntEx(siString string, b32* outRes) {
	SI_ASSERT_NOT_NULL(string.data);

	switch (*(u8*)string.data) {
		case '-':
			string.data = (u8*)string.data + 1;
			string.len -= 1;

			return -(i64)si_stringToUIntEx(string, outRes);
		case '+':
			string.data = (u8*)string.data + 1;
			string.len  -= 1;
			break;
	}

	return si_stringToUIntEx(string, outRes);
}

#endif

#if defined(SI_IMPLEMENTATION_UNICODE) && !defined(SI_NO_UNICODE)

#define FAILURE 12

SIDEF
siUtf32Char si_utf8Decode(const u8* character) {
	u32 state = 0, codepoint;
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
		return SI_UTF32_REPLACEMENT_CHARACTER;
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
	SI_STOPIF(string.len == 0, return SI_UTF32_REPLACEMENT_CHARACTER);

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
	SI_STOPIF(string.len == 0, return SI_STR_LEN(nil, 0));

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
siUtf16String si_utf8ToUtf16Str(u16* outBuffer, usize capacity, siUtf8String str) {
	return si_utf8ToUtf16StrEx(outBuffer, capacity, str, false);
}

SIDEF
siUtf16String si_utf8ToUtf16StrEx(u16* outBuffer, usize capacity, siUtf8String str,
		b32 addNullTerminator) {
	SI_ASSERT_NOT_NULL(outBuffer);

	capacity -= addNullTerminator;
	u16* res = outBuffer;
	usize offsetUTF8 = 0, offsetUTF16 = 0;

	while (offsetUTF8 < str.len && offsetUTF16 < capacity) {
		siUtf32Char utf32 = si_utf8Decode(&si_stringGet(str, offsetUTF8));

		i32 codepoint = utf32.codepoint;
		offsetUTF8 += utf32.len;

		if (codepoint < 0xFFFF) {
			res[offsetUTF16] = (u16)codepoint;
			offsetUTF16 += 1;
		}
		else {
			SI_STOPIF(offsetUTF16 + 2 >= capacity, break);

			u16 t = si_cast(u16, codepoint - 0x10000);
			u16 high = si_cast(u16, (t << 10) + 0xD800);
			u16 low = t + 0xDC00;

			res[offsetUTF16 + 0] = high;
			res[offsetUTF16 + 1] = low;
			offsetUTF16 += 2;
		}
	}

	if (addNullTerminator) {
		res[offsetUTF16] = '\0';
		offsetUTF16 += 1;
	}

	return SI_BUF_LEN(u16, res, offsetUTF16);
}

inline
siUtf8String si_utf16ToUtf8Str(u8* outBuffer, usize capacity, siUtf16String str) {
	return si_utf16ToUtf8StrEx(outBuffer, capacity, str, false);
}

SIDEF
siUtf8String si_utf16ToUtf8StrEx(u8* outBuffer, usize capacity, siUtf16String str,
		b32 addNullTerminator) {
	SI_ASSERT_NOT_NULL(outBuffer);
	SI_STOPIF(str.len == 0, return SI_STR_LEN(nil, 0));

	capacity -= addNullTerminator;
	u8* res = outBuffer;
	usize offsetUTF8 = 0, offsetUTF16 = 0;

	while (offsetUTF16 < str.len && offsetUTF8 < capacity) {
		u32 chr = si_arrayGet(str, offsetUTF16, u16);

		if (chr <= 0xFF) {
			res[offsetUTF8]= (u8)chr;

			offsetUTF8 += 1;
			offsetUTF16 += 1;
		}
		else if (chr <= 0x7FF) {
			SI_STOPIF(offsetUTF8 + 2 >= capacity, break);

			res[offsetUTF8 + 0] = si_cast(u8, 0xC0 | (chr >> 6));
			res[offsetUTF8 + 1] = si_cast(u8, 0x80 | (chr & 0x3F));

			offsetUTF8 += 2;
			offsetUTF16 += 1;
		}
		else if (chr <= 0xD7FF) {
			SI_STOPIF(offsetUTF8 + 3 >= capacity, break);

			res[offsetUTF8 + 0] = si_cast(u8, 0xE0 | (chr >> 12));           /* 1110xxxx */
			res[offsetUTF8 + 1] = si_cast(u8, 0xE0 | ((chr >> 6) & 0x3F));   /* 10xxxxxx */
			res[offsetUTF8 + 2] = si_cast(u8, 0xE0 | (chr & 0x3F));          /* 10xxxxxx */

			offsetUTF8 += 3;
			offsetUTF16 += 1;
		}
		else if (chr >= 0xD800) {
			SI_STOPIF(offsetUTF8 + 4 >= capacity, break);

			u16 high = (u16)chr;
			u16 low = si_arrayGet(str, offsetUTF16 + 1, u16);

			u32 tHigh = (high - 0xD800) << 10;
			u32 tLow = (low - 0xDC00);
			u32 codepoint = (tHigh | tLow) + 0x10000;

			res[offsetUTF8 + 0] = si_cast(u8, 0xF0 | (codepoint >> 18));           /* 11110xxx */
			res[offsetUTF8 + 1] = si_cast(u8, 0x80 | ((codepoint >> 12) & 0x3F));  /* 10xxxxxx */
			res[offsetUTF8 + 2] = si_cast(u8, 0x80 | ((codepoint >> 6) & 0x3F));   /* 10xxxxxx */
			res[offsetUTF8 + 3] = si_cast(u8, 0x80 | (codepoint & 0x3F));          /* 10xxxxxx */

			offsetUTF8 += 4;
			offsetUTF16 += 2;
		}
	}

	if (addNullTerminator) {
		res[offsetUTF8] = '\0';
		offsetUTF8 += 1;
	}

	return SI_STR_LEN(res, offsetUTF8);
}

SIDEF
siUtf32Char si_utf16Decode(const u16 character[2]) {
	siUtf32Char utf32;
	u32 chr = character[0];

	if (si_between(u32, chr, 0xD800, 0xDBFF)) {
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
		return c | SI_BIT(5);
	}
	return c;
}

inline
char si_charUpper(char c) {
	if (characterTraits[(u8)c] & SI__LETL) {
		return c & ~SI_BIT(5);
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

	u64 hash = si__hashKey((siByte*)key, keyLen);
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

	u64 hash = si__hashKey((siByte*)key, keyLen);
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

	ht->len += 1;
	return entry;
}

#endif

#if defined(SI_IMPLEMENTATION_IO) && !defined(SI_NO_IO)

#if SI_COMPILER_MSVC
	#pragma comment(lib, "shell32")
	#pragma comment(lib, "Advapi32")
#endif

siIntern
void si__fileErrorLog(siError* error) {
	siPrintColor red = si_printColor3bitEx(siPrintColorAnsi_Red, true, false);

#if !defined(SI_NO_ERROR_STRUCT)
	si_fprintf(
		si_stderr, "%CFile system error at \"%s:%i\"%C: %S: %S\n",
		red, error->filename, error->line,
		si_pathFsErrorName(error->code), si_pathFsErrorDesc(error->code)
	);

#else
	si_fprintf(
		si_stderr, "%CFile system error%C: %S: %S\n",
		red, si_pathFsErrorName(error->code), si_pathFsErrorDesc(error->code)
	);

#endif
}

#define siFileError_CHECK_EX(condition, error, errorCode, returnValue) \
	SI_ERROR_CHECK_EX(condition, error, errorCode, returnValue, si__fileErrorLog)

#define siFileError_CHECK(condition, error, returnValue) \
	SI_ERROR_CHECK_EX(condition, error, si__fileGetOSError(), returnValue, si__fileErrorLog)

#define siFileError_CHECK_OPT(condition, type) \
	{ \
		siOption(type) output = {0}; \
		siFileError_CHECK(condition, &output.data.error, output); \
	}


SIDEF
siString si_pathFsErrorName(siFileError err) {
	switch (err) {
		case siFileError_Generic: return SI_STR("siFileError_Generic");
		case siFileError_None: return SI_STR("siFileError_None");
		case siFileError_Exists: return SI_STR("siFileError_Exists");
		case siFileError_Invalid: return SI_STR("siFileError_Invalid");
		case siFileError_InvalidFilename: return SI_STR("siFileError_InvalidFilename");
		case siFileError_NotExists: return SI_STR("siFileError_NotExists");
		case siFileError_Permission: return SI_STR("siFileError_Permission");
		case siFileError_TruncationFail: return SI_STR("siFileError_TruncationFail");
	}
	return SI_STR("N/A");
}


SIDEF
siString si_pathFsErrorDesc(siFileError err) {
	switch (err) {
		case siFileError_Generic: return SI_STR("An unspecified, platform-specific error occurred.");
		case siFileError_None: return SI_STR("No error has been encountered.");
		case siFileError_Exists: return SI_STR("File or directory already exists.");
		case siFileError_Invalid: return SI_STR("Provided value is invalid.");
		case siFileError_InvalidFilename: return SI_STR("Invalid filename.");
		case siFileError_NotExists: return SI_STR("File or directory doesn't exist.");
		case siFileError_Permission: return SI_STR("User doesn't have suffice permissions.");
		case siFileError_TruncationFail: return SI_STR("Failed to truncate the file.");
	}
	return SI_STR("N/A");
}


siIntern
siFileError si__fileGetOSError(void) {
#if SI_SYSTEM_IS_WINDOWS
	switch (GetLastError()) {
		case ERROR_NO_MORE_FILES:
		case ERROR_SUCCESS: return siFileError_None;
		case ERROR_ALREADY_EXISTS:
		case ERROR_FILE_EXISTS: return siFileError_Exists;
		case ERROR_INVALID_NAME:
		case ERROR_BAD_PATHNAME: return siFileError_InvalidFilename;
		case ERROR_DIRECTORY:
		case ERROR_INVALID_ADDRESS: return siFileError_Invalid;
		case ERROR_PATH_NOT_FOUND:
		case ERROR_FILE_NOT_FOUND: return siFileError_NotExists;
		case ERROR_ACCESS_DENIED: return siFileError_Permission;
	}
	return siFileError_Invalid;
#else
	switch (errno) {
		case 0: return siFileError_None;
		case EEXIST: return siFileError_Exists;
		case EINVAL: return siFileError_Invalid;
		case EISDIR:
		case ENAMETOOLONG: return siFileError_InvalidFilename;
		case ENOENT: return siFileError_NotExists;
		case EPERM:
		case EACCES: return siFileError_Permission;
	}

	return siFileError_Invalid;
#endif
}


SIDEF
b32 si_pathExists(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[512];
		siUtf16String widePath = si_utf8ToUtf16StrEx(stack, countof(stack), path, true);

		return GetFileAttributesW((u16*)widePath.data) != INVALID_FILE_ATTRIBUTES;

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		struct stat tmp;
		return (stat(si_stringCopyToCstr(path, stack), &tmp) == 0);

	#endif
}
SIDEF
siResult(usize) si_pathCopy(siString pathSrc, siString pathDst) {
	SI_ASSERT(pathSrc.len <= SI_MAX_PATH_LEN);
	SI_ASSERT(pathDst.len <= SI_MAX_PATH_LEN);


	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[512];
		siUtf16String src = si_utf8ToUtf16StrEx(&stack[0], countof(stack), pathSrc, true),
					  dst = si_utf8ToUtf16StrEx(&stack[src.len], countof(stack) - src.len, pathDst, true);

		i32 size = CopyFileW((u16*)src.data, (u16*)dst.data, true);
		siFileError_CHECK_OPT(size == 0, usize);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		cstring src = si_stringCopyToCstr(pathSrc, stack),
				dst = si_stringCopyToCstr(pathDst, stack);

		int fileSrc = open(src, O_RDONLY, 0);
		siFileError_CHECK_OPT(fileSrc == -1, usize);

		int fileDst = open(dst, O_WRONLY | O_CREAT, 0666);
		siFileError_CHECK_OPT(fileDst == -1, usize);

		struct stat stat;
		int res = fstat(fileDst, &stat);
		siFileError_CHECK_OPT(res == -1, usize);

		#if SI_SYSTEM_IS_UNIX
			isize size = sendfile64(fileDst, fileSrc, 0, stat.st_size);
		#else
			isize size = sendfile(fileDst, fileSrc, 0, &stat.st_size, NULL, 0);
		#endif
		siFileError_CHECK_OPT(size == -1, usize);

		close(fileSrc);
		close(fileDst);

	#endif

	return SI_OPT(usize, (usize)size);
}
SIDEF
u32 si_pathItemsCopy(siString pathSrc, siString pathDst) {
	SI_ASSERT(pathDst.len <= SI_MAX_PATH_LEN);

	siDirectory dir = si_directoryOpen(pathSrc);
	u32 itemsCopied = 0;

	char dst[SI_MAX_PATH_LEN];
	char* dstBuffer;
	siString dstStr = SI_STR_LEN(dst, pathDst.len + 1);

	si_memcopy(dst, pathDst.data, pathDst.len);
	dst[pathDst.len] = SI_PATH_SEPARATOR;
	dstBuffer = &dst[pathDst.len + 1];

	siDirectoryEntry entry;
	while (si_directoryPollEntryEx(&dir, &entry, false)) {
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
siError si_pathMove(siString pathSrc, siString pathDst) {
	SI_ASSERT(pathSrc.len <= SI_MAX_PATH_LEN);
	SI_ASSERT(pathDst.len <= SI_MAX_PATH_LEN);

	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[512];
		siUtf16String src = si_utf8ToUtf16StrEx(&stack[0], countof(stack), pathSrc, true),
					  dst = si_utf8ToUtf16StrEx(&stack[src.len], countof(stack) - src.len, pathDst, true);

		i32 res = MoveFileW((u16*)src.data, (u16*)dst.data);
		siFileError_CHECK(res == 0, &error, error);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		cstring src = si_stringCopyToCstr(pathSrc, stack),
				dst = si_stringCopyToCstr(pathDst, stack);

		int res = link(src, dst);
		siFileError_CHECK(res != 0, &error, error);

		res = unlink(src);
		siFileError_CHECK(res != 0, &error, error);

	#endif

	return error;
}

inline
siError si_pathRename(siString nameOld, siString nameNew) {
	return si_pathMove(nameOld, nameNew);
}

inline
siError si_pathCreateFolder(siString path) {
	return si_pathCreateFolderEx(path, SI_FS_PERM_ALL);
}

SIDEF
siError si_pathCreateFolderEx(siString path, siFilePermissions perms) {
	SI_ASSERT(perms != -1);
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[512];
		siUtf16String pathWide = si_utf8ToUtf16StrEx(stack, countof(stack), path, true);

		i32 res = CreateDirectoryW((u16*)pathWide.data, nil);
		siFileError_CHECK(res == 0, &error, error);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		cstring str = si_stringCopyToCstr(path, stack);

		/* NOTE(EimaMei): For whatever reason, 'mkdir' will sometimes return -1
		 * but still create the folder and set 'errno' to 0. What? */
		int res = mkdir(str, (mode_t)perms);
		siFileError_CHECK(res != 0, &error, error);

	#endif

	return error;
}

#if !SI_SYSTEM_IS_WINDOWS
force_inline
i32 si__unlinkCb(cstring path, const struct stat* sb, i32 typeflag, struct FTW* ftwbuf) {
	return remove(path);
	SI_UNUSED(sb); SI_UNUSED(typeflag); SI_UNUSED(ftwbuf);
}
#endif

SIDEF
siError si_pathRemove(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[512];
		siUtf16String pathWide = si_utf8ToUtf16StrEx(stack, countof(stack), path, true);

		/* NOTE(EimaMei): Windows requires double zeros for SHFileOperation for some reason... */
		u16* data = pathWide.data;
		data[pathWide.len] = '\0';

		u32 attrs = GetFileAttributesW(data);
		siFileError_CHECK(attrs == INVALID_FILE_ATTRIBUTES, &error, error);

		if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
			SHFILEOPSTRUCTW operation = {
				nil, FO_DELETE, data, nil, FOF_NO_UI, false, 0, nil
			};

			i32 res = SHFileOperationW(&operation);
			siFileError_CHECK_EX(res != 0 || operation.fAnyOperationsAborted, &error, siFileError_Generic, error);
		}
		else {
			i32 res = DeleteFileW(data);
			siFileError_CHECK(res == 0, &error, error);
		}

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		cstring pathCstr = si_stringCopyToCstr(path, stack);
		int res = nftw(pathCstr, si__unlinkCb, 64, FTW_DEPTH | FTW_PHYS);
		siFileError_CHECK(res == -1, &error, error);

	#endif

	return error;
}

SIDEF
siError si_pathCreateHardLink(siString path, siString pathLink) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	SI_ASSERT(pathLink.len <= SI_MAX_PATH_LEN);

	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[512];
		siUtf16String pathWide = si_utf8ToUtf16StrEx(&stack[0], countof(stack), path, true),
					  linkWide = si_utf8ToUtf16StrEx(&stack[pathWide.len], countof(stack) - pathWide.len, pathLink, true);

		i32 res = CreateHardLinkW((u16*)linkWide.data, (u16*)pathWide.data, nil);
		siFileError_CHECK(res == 0, &error, error);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		int res = link(
			si_stringCopyToCstr(path, stack),
			si_stringCopyToCstr(pathLink, stack)
		);
		siFileError_CHECK(res == -1, &error, error);

	#endif

	return error;
}

SIDEF
siError si_pathCreateSoftLink(siString path, siString pathLink) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	SI_ASSERT(pathLink.len <= SI_MAX_PATH_LEN);

	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[512];
		siUtf16String pathWide = si_utf8ToUtf16StrEx(&stack[0], countof(stack), path, true),
					  linkWide = si_utf8ToUtf16StrEx(&stack[pathWide.len], countof(stack) - pathLink.len, pathLink, true);

		u32 attrs = GetFileAttributesW((u16*)pathWide.data);
		siFileError_CHECK(attrs == INVALID_FILE_ATTRIBUTES, &error, error);

		i32 res = CreateSymbolicLinkW(
			(u16*)linkWide.data, (u16*)pathWide.data,
			attrs & FILE_ATTRIBUTE_DIRECTORY
		);
		siFileError_CHECK(res == 0, &error, error);

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		int res = symlink(
			si_stringCopyToCstr(path, stack),
			si_stringCopyToCstr(pathLink, stack)
		);
		siFileError_CHECK(res == -1, &error, error);

	#endif

	return error;
}

SIDEF
siError si_pathEditPermissions(siString path, siFilePermissions newPerms) {
	SI_ASSERT(newPerms != -1);
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

	siError error = SI_ERROR_DEFAULT;

	#if SI_SYSTEM_IS_WINDOWS
		/* TODO(EimaMei): Implement this. */

	#else
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
		int res = chmod(si_stringCopyToCstr(path, stack), (mode_t)newPerms);
		siFileError_CHECK(res == -1, &error, error);

	#endif

	return error;
}


inline
siString si_pathBaseName(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

	for_eachRevStr (x, path) {
		if (*x == SI_PATH_SEPARATOR) {
			return si_stringSubToEnd(path, (x + 1) - (u8*)path.data);
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
			return si_stringSubToEnd(path, (x + 1) - (u8*)path.data);
		}
	}
	return path;
}

SIDEF
siString si_pathExtension(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

	for_eachStr (x, path) {
		if (*x == '.') {
			return si_stringSubToEnd(path, (x + 1) - (u8*)path.data);
		}
		else if (*x == SI_PATH_SEPARATOR) {
			break;
		}
	}

	return SI_STR("");
}

SIDEF
siResult(siString) si_pathGetFullName(siString path, siAllocator alloc) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
	char pathOut[SI_KILO(4)];


	#if SI_SYSTEM_IS_WINDOWS
		u16 stack[512];
		siUtf16String pathWide = si_utf8ToUtf16StrEx(stack, countof(stack), path, true);

		siUtf16String outWide;
		outWide.typeSize = sizeof(u16);
		outWide.data = &stack[pathWide.len];
		outWide.capacity = outWide.len = GetFullPathNameW(
			(u16*)pathWide.data, countof(stack) - (u32)pathWide.len, (u16*)outWide.data, nil
		);

		siFileError_CHECK_OPT(outWide.len == 0, siString);

		siUtf8String res = si_utf16ToUtf8Str((u8*)pathOut, countof(pathOut), outWide);
		return SI_OPT(siString, si_stringCopy(res, alloc));

	#else
		/* NOTE(EimaMei): Technically on Linux you have no issues creating paths
		 * that go over the highest 'PATH_MAX' value (that being 4KB) except most
		 * software doesn't take this into account, so they either fail or crash.
		 * What a trolleybus moment! */
		siAllocator stack = si_allocatorMakeStack(SI_KILO(1));

		realpath(si_stringCopyToCstr(path, stack), pathOut);
		siFileError_CHECK_OPT(pathOut == nil, siString);

		return SI_OPT(siString, si_stringMake(pathOut, alloc));

	#endif
}


inline
b32 si_pathIsAbsolute(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
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
u64 si_pathLastWriteTime(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

#if SI_SYSTEM_IS_WINDOWS
	FILETIME lastWriteTime = {0};
	WIN32_FILE_ATTRIBUTE_DATA data = {0};
	u16 stack[512];

	siUtf16String pathWide = si_utf8ToUtf16StrEx(stack, countof(stack), path, true);

	if (GetFileAttributesExW((u16*)pathWide.data, GetFileExInfoStandard, &data)) {
		lastWriteTime = data.ftLastWriteTime;
	}

	ULARGE_INTEGER res;
	res.LowPart = lastWriteTime.dwLowDateTime;
	res.HighPart = lastWriteTime.dwHighDateTime;

	/* NOTE(EimaMei): Windows time -> Unix time. */
	return (res.QuadPart - 116444736000000000) / 10000000;

#else
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
	struct stat fs;
	int res = stat(si_stringCopyToCstr(path, stack), &fs);
	return (res == 0) ? fs.st_mtime : 0;

#endif
}

SIDEF
siString si_pathGetTmp(void) {
#if SI_SYSTEM_IS_WINDOWS
	static u8 buffer[SI_MAX_PATH_LEN];
	u16 bufferWide[512];

	u32 len = GetTempPathW(countof(bufferWide), bufferWide);
	SI_ASSERT(len != 0); /* NOTE(EimaMei): This musn't fail. */

	return si_utf16ToUtf8Str(buffer, countof(buffer), SI_BUF_LEN(u16, bufferWide, len));

#else
	return SI_STR("/tmp");

#endif
}


SIDEF
siFilePermissions si_pathPermissions(siString path) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);

#if SI_SYSTEM_IS_WINDOWS
	ACCESS_MASK mask;
	{
		u16 stack[512];
		siUtf16String pathWide = si_utf8ToUtf16Str(stack, countof(stack), path);

		SECURITY_INFORMATION info = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION
			| DACL_SECURITY_INFORMATION;

		SECURITY_DESCRIPTOR* desc = si_transmute(SECURITY_DESCRIPTOR*, stack + pathWide.len, u16*);
		u32 length = sizeof(stack) - (u32)pathWide.len * 2;

		i32 res = GetFileSecurityW(
			(u16*)pathWide.data, info, desc, length, nil
		);
		SI_STOPIF(res == 0, return 0);

		res = GetEffectiveRightsFromAclA(desc->Dacl, desc->Owner, &mask);
		SI_STOPIF(res != 0, return 0);
	}

	siFilePermissions perms = SI_FS_PERM_GROUP_EXEC | SI_FS_PERM_OTHERS_EXEC | SI_FS_PERM_OWNER_EXEC;
	if (mask & READ_CONTROL) {
		perms |= SI_FS_PERM_GROUP_READ | SI_FS_PERM_OWNER_READ | SI_FS_PERM_OTHERS_READ;
	}
	if (mask & WRITE_OWNER) {
		perms |= SI_FS_PERM_OWNER_WRITE | SI_FS_PERM_GROUP_WRITE | SI_FS_PERM_OTHERS_WRITE;
	}

	return perms;

#else
	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
	struct stat fs;
	int res = stat(si_stringCopyToCstr(path, stack), &fs);
	return (res == 0) ? (siFilePermissions)fs.st_mode : -1;

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
siError si_pathReadContentsBuf(siString path, siBuffer* outBuffer) {
	siFile file = si_fileOpen(path);
	siError res = si_fileReadContentsBuf(file, outBuffer);
	si_fileClose(file);

	return res;
}

static b32 SI_STD_FILE_SET = false;
static siFile SI_STD_FILE_ARR[siStdFile_Count] = {0};


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
	return si_fileOpenMode(path, siFileMode_Write | siFIleMode_Plus);
}

inline
siFile si_fileOpen(siString path) {
	return si_fileOpenMode(path, siFileMode_Read | siFIleMode_Plus);
}
SIDEF
siFile si_fileOpenMode(siString path, siFileMode mode) {
	SI_ASSERT(path.len <= SI_MAX_PATH_LEN);
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
		case siFileMode_Read | siFIleMode_Plus:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = OPEN_EXISTING;
			break;
		case siFileMode_Write | siFIleMode_Plus:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = CREATE_ALWAYS;
			break;
		case siFileMode_Append | siFIleMode_Plus:
			access = GENERIC_READ | GENERIC_WRITE;
			disposition = OPEN_ALWAYS;
			break;
		default:
			disposition = access = 0;
			SI_PANIC_MSG("Invalid file mode.");
	}

	rawptr handle;
	{
		u16 stack[512];
		siUtf16String pathWide = si_utf8ToUtf16StrEx(stack, countof(stack), path, true);
		handle = CreateFileW(
			(u16*)pathWide.data,
			access, FILE_SHARE_READ | FILE_SHARE_DELETE, nil,
			disposition, FILE_ATTRIBUTE_NORMAL, nil
		);
	}
	siFileError_CHECK(handle == INVALID_HANDLE_VALUE, &res.error, res);

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
		case siFileMode_Read | siFIleMode_Plus:
			flags = O_RDWR;
			break;
		case siFileMode_Write | siFIleMode_Plus:
			flags = O_RDWR | O_CREAT | O_TRUNC;
			break;
		case siFileMode_Append | siFIleMode_Plus:
			flags = O_RDWR | O_APPEND | O_CREAT;
			break;
		default:
			flags = 0;
			SI_PANIC_MSG("Invalid file mode.");
	}

	siAllocator stack = si_allocatorMakeStack(SI_KILO(1));
	cstring pathCstr = si_stringCopyToCstr(path, stack);
	i64 handle = open(pathCstr, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	siFileError_CHECK(handle == -1, &res.error, res);

#endif
	res.handle = handle;
	res.size = si_fileSize(res);

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

SIDEF
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
	len = si_min(usize, si_allocatorGetAvailable(alloc), len);

	siBuffer buffer = si_arrayMakeReserve(1, len, len, alloc);
	siError err = si_fileReadUnsafe(file, offset, len, &buffer);

	return (err.code == 0)
		? SI_OPT(siBuffer, buffer)
		: SI_OPT_ERR(siBuffer, err);
}

inline
siError si_fileReadBuf(siFile file, usize len, siBuffer* outBuffer) {
	return si_fileReadAtBuf(file, si_fileTell(file), len, outBuffer);
}
inline
siError si_fileReadAtBuf(siFile file, isize offset, usize len, siBuffer* outBuffer) {
	return si_fileReadUnsafe(file, offset, si_min(usize, len, outBuffer->capacity), outBuffer);
}

SIDEF
siError si_fileReadUnsafe(siFile file, isize offset, usize len, siBuffer* outBuffer) {
	SI_ASSERT_NOT_NULL((rawptr)file.handle);
	SI_ASSERT_NOT_NULL(outBuffer);
	SI_ASSERT_NOT_NULL(outBuffer->data);

	siError error = SI_ERROR_DEFAULT;
	si_fileSeek(file, offset, siSeekWhere_Begin);

#if SI_SYSTEM_IS_WINDOWS
	DWORD bytesRead;
	DWORD toRead = (len < UINT32_MAX)
		? (DWORD)len
		: UINT32_MAX;

	i32 res = ReadFile(file.handle, outBuffer->data, toRead, &bytesRead, NULL);
	siFileError_CHECK(res == 0, &error, error);

#else
	isize bytesRead = pread((int)file.handle, outBuffer->data, len, offset);
	siFileError_CHECK(bytesRead == -1, &error, error);

#endif

	outBuffer->len = bytesRead;
	return error;
}

SIDEF
siString si_fileReadContents(siFile file, siAllocator alloc) {
	usize len = si_min(usize, si_allocatorGetAvailable(alloc), file.size);
	siString buffer = si_arrayMakeReserve(1, 0, len, alloc);

	isize oldOffset = si_fileTell(file);
	siError err = si_fileReadUnsafe(file, 0, len, &buffer);
	si_fileSeek(file, oldOffset, siSeekWhere_Begin);

	return (err.code == 0) ? buffer : SI_STR_LEN(nil, 0);
}
SIDEF
siError si_fileReadContentsBuf(siFile file, siBuffer* outBuffer) {
	isize oldOffset = si_fileTell(file);
	siError res = si_fileReadUnsafe(
		file, 0, si_min(usize, outBuffer->capacity, file.size), outBuffer
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
	siFileError_CHECK(res == 0, &file->error, -1);

#else
	isize curOffset = si_fileSeek(*file, 0, siSeekWhere_Current);
	isize bytesWritten = (curOffset == offset)
		? write((int)file->handle, content.data, content.len)
		: pwrite((int)file->handle, content.data, content.len, offset);
	siFileError_CHECK(bytesWritten == -1, &file->error, -1);

#endif

	file->size += (offset - file->size) + bytesWritten;
	return bytesWritten;
}

SIDEF
isize si_fileWriteAtLine(siFile* file, siString line, isize index) {
	SI_ASSERT_NOT_NULL(file);
	SI_ASSERT_NOT_NULL(line.data);

	siAllocator alloc = (file->size + line.len <= SI_KILO(8))
		? si_allocatorMakeStack(SI_KILO(8))
		: si_allocatorHeap();

	siString content = si_arrayMakeReserve(1, 0, file->size + line.len, alloc);
	{
		siError err = si_fileReadContentsBuf(*file, &content);
		SI_STOPIF(err.code != 0, return -1);
	}

	b32 isNeg = index < 0;
	isize lineStart = 0;
	{
		index = si_abs(isize, index);
		while (index) {
			siString substr = si_stringSubToEnd(content, lineStart);
			isize len = !isNeg
				? si_stringFindByte(substr, '\n')
				: si_stringFindLastByte(substr, '\n');
			SI_STOPIF(len == -1, return -1);

			lineStart += len + countof_str("\n");
			index -= 1;
		}
	}

	{
		siString substr = si_stringSubToEnd(content, lineStart);
		isize lineEnd = !isNeg
			? si_stringFindByte(substr, '\n')
			: si_stringFindLastByte(substr, '\n');

		isize oldLen;
		if (lineEnd == -1) { /* NOTE(EimaMei): Index is the last line. */
			oldLen = file->size - (lineStart + 1);
		}
		else {
			siString end = si_stringSubToEnd(substr, lineEnd);
			oldLen = end.len;

			si_memmoveStr(
				&si_stringGet(substr, line.len),
				end
			);
		}
		si_memcopyStr(substr.data, line);

		file->size += (isize)line.len - (isize)oldLen;
		content.len = file->size;
	}

	isize len;
	{
		isize oldOffset = si_fileTell(*file);
		len = si_fileWriteAt(file, content, 0);
		si_fileSeek(*file, oldOffset, siSeekWhere_Begin);
	}

	si_freeAll(alloc);
	return len;
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
	siFileError_CHECK_EX(res == 0, &file->error, siFileError_TruncationFail, false);

	si_fileSeek(*file, prevOffset, siSeekWhere_Begin);
#else
	int res = ftruncate((int)file->handle, size);
	siFileError_CHECK_EX(res == -1, &file->error, siFileError_TruncationFail, false);
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

	/* NOTE(EimaMei): Windows time -> Unix time. */
	return (res.QuadPart - 116444736000000000) / 10000000;

#else
	struct stat fs;
	int res = fstat((int)file.handle, &fs);
	return (res == 0) ? fs.st_mtime : 0;

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
siDirectory si_directoryOpen(siString path) {
	SI_ASSERT_NOT_NULL(path.data);
	SI_ASSERT(path.len - 1 <= SI_MAX_PATH_LEN);

	siDirectory dir;
	dir.error = SI_ERROR_DEFAULT;
	dir.handle = nil;
	si_memcopyStr(dir.buffer, path);

	if (si_stringAtBack(path) != SI_PATH_SEPARATOR) {
		dir.buffer[path.len] = SI_PATH_SEPARATOR;
		path.len += 1;
	}
	dir.directoryLen = path.len;

#if SI_SYSTEM_IS_WINDOWS
	u16 stack[512];
	siUtf16String pathWide = si_utf8ToUtf16Str(stack, countof(stack), SI_STR_LEN(dir.buffer, dir.directoryLen));

	u16* dataWide = (u16*)pathWide.data;
	dataWide[pathWide.len + 0] = '*';
	dataWide[pathWide.len + 1] = '\0';

	WIN32_FIND_DATAW tmp;
	HANDLE handle = FindFirstFileW(dataWide, &tmp);
	siFileError_CHECK(handle == INVALID_HANDLE_VALUE, &dir.error, dir);

	dir.handle = handle;
	SI_DISCARD(FindNextFileW(handle, &tmp));

#else
	/* NOTE(EimaMei): We do this because opendir only takes NULL-terminated C-strings. */
	dir.buffer[path.len] = '\0';

	dir.handle = opendir(dir.buffer);
	siFileError_CHECK(dir.handle == nil, &dir.error, dir);

	/* NOTE(EimaMei): We skip '.' and '..'. */
	SI_DISCARD(readdir((DIR*)dir.handle));
	SI_DISCARD(readdir((DIR*)dir.handle));

#endif

	return dir;
}

inline
b32 si_directoryPollEntry(siDirectory* dir, siDirectoryEntry* entry) {
	return si_directoryPollEntryEx(dir, entry, true);
}

SIDEF
b32 si_directoryPollEntryEx(siDirectory* dir, siDirectoryEntry* entry, b32 fullPath) {
	SI_ASSERT_NOT_NULL(dir);
	SI_ASSERT_NOT_NULL(entry);
	SI_ASSERT_NOT_NULL(dir->handle);

#if SI_SYSTEM_IS_WINDOWS
	WIN32_FIND_DATAW file;
	if (FindNextFileW(dir->handle, &file) == 0) {
		siFileError code = si__fileGetOSError();
		CloseHandle(dir->handle);

		siFileError_CHECK_EX(code != siFileError_None, &dir->error, code, false);
		return false;
	}

	usize utf16Len = wcslen(file.cFileName);
	u8* outPtr;
	usize capacity;

	if (fullPath) {
		outPtr = (u8*)&dir->buffer[dir->directoryLen];
		capacity = sizeof(dir->buffer) - dir->directoryLen;
		entry->path.len = dir->directoryLen;
	}
	else {
		outPtr = (u8*)dir->buffer;
		capacity = sizeof(dir->buffer);
		entry->path.len = 0;
	}

	siUtf8String res = si_utf16ToUtf8Str(outPtr, capacity, SI_BUF_LEN(u16, file.cFileName, utf16Len));
	entry->path = SI_STR_LEN(dir->buffer, res.len);


	if ((file.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)) == 0) {
		entry->type = SI_IO_TYPE_FILE;
	}
	else if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		entry->type = SI_IO_TYPE_DIR;
	}
	else {
		entry->type = SI_IO_TYPE_LINK;
	}

#else
	/* NOTE(EimaMei): Sudie, C++ palaikyme! Galbūt busi sugražintas, kai bent jau
	 * ta kalba pagaliau tūrės paskirtus masyvo inicijavimus. */
	static u8 IO_types[] = {
		[DT_REG]  = SI_IO_TYPE_FILE,
		[DT_DIR]  = SI_IO_TYPE_DIR,
		[DT_LNK]  = SI_IO_TYPE_LINK,
		[DT_SOCK] = SI_IO_TYPE_SOCKET,
		[DT_CHR]  = SI_IO_TYPE_DEVICE,
		[DT_BLK]  = SI_IO_TYPE_BLOCK,
		[DT_FIFO] = SI_IO_TYPE_FIFO
	};

	struct dirent* dirEntry = readdir((DIR*)dir->handle);
	if (dirEntry == nil) {
		errno = 0;
		siFileError code = si__fileGetOSError();
		if (code != siFileError_None) {
			SI_ERROR_EX(&dir->error, code, si__fileErrorLog);
		}

		closedir((DIR*)dir->handle);
		return false;
	}

	char* buffer;
	usize len = si_cstrLen(dirEntry->d_name);

	entry->type = IO_types[dirEntry->d_type];
	if (fullPath) {
		buffer = &dir->buffer[dir->directoryLen];
		entry->path.len = len + dir->directoryLen;
	}
	else {
		buffer = dir->buffer;
		entry->path.len = len;
	}
	SI_ASSERT(entry->path.len <= SI_MAX_PATH_LEN);

	si_memcopy(buffer, dirEntry->d_name, len);
	entry->path.data = (siByte*)buffer;

#endif

	return true;
}

#endif /* SI_IO_IMPLEMENTATION */

#if defined(SI_IMPLEMENTATION_THREAD) && !defined(SI_NO_THREAD)

#if SI_SYSTEM_IS_WINDOWS

siIntern
DWORD WINAPI si__threadProc(LPVOID arg) {
	siThread* t = (siThread*)arg;
	t->returnValue = t->func(t->arg);
	t->isRunning = false;

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


SIDEF
b32 si_threadMake(siFunction function, rawptr arg, b32 runThread, siThread* outThread) {
	return si_threadMakeEx(function, arg, 0, runThread, outThread);
}

SIDEF
b32 si_threadMakeEx(siFunction function, rawptr arg, usize stackSize, b32 runThread,
		siThread* outThread) {
	SI_ASSERT_NOT_NULL(function);

	siThread* t = outThread;
	t->func = function;
	t->arg = arg;
	t->stackSize = stackSize;
	t->initialized = true;
	t->returnValue = nil;
	t->isRunning = false;

	if (runThread) {
		return si_threadRun(t);
	}

	return true;
}

SIDEF
b32 si_threadRun(siThread* t) {
	SI_ASSERT_NOT_NULL(t);
	SI_ASSERT(t->initialized);
	SI_ASSERT(!t->isRunning);

	#if SI_SYSTEM_IS_WINDOWS
		t->id = CreateThread(nil, t->stackSize, si__threadProc, t, 0, nil);
		t->isRunning = (t->id != nil);

	#else
		int res;
		if (t->stackSize != 0) {
			pthread_attr_t attr;
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
u32 si_numLeadingBitEx(u64 num, usize totalBits, siBitType bit) {
	u32 count = 0;

	usize i;
	for (i = totalBits - 1; i < USIZE_MAX; i -= 1) {
		if ((num & i) == bit) {
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
	} while (0 < num);

	return count;
}

inline
u32 si_numLenI64(i64 num) {
	return si_numLenI64Ex(num, 10);
}

inline
u32 si_numLenI64Ex(i64 num, u32 base) {
	i64 isNegative = si_numIsNeg(num);
	u64 unsignedNum = (num ^ -isNegative) + isNegative;
	return si_numLenEx(unsignedNum, base) + (u32)isNegative;
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
	SI_CPU_FREQ_MHZ = ((val + 5) / 10) * 10;

	return SI_CPU_FREQ_MHZ;
}

#endif /* SI_IMPLEMENTATION_CPU */

#if defined(SI_IMPLEMENTATION_PRINT) && !defined(SI_NO_PRINT)


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


SIDEF
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
	isize count = si_snprintfVa(buffer, sizeof(buffer), fmt, va) - 1;

	return si_fileWrite(file, SI_STR_LEN(buffer, (usize)count));
}

SIDEF
isize si_sprintf(char* buffer, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_sprintfVa(buffer, fmt, va);
	va_end(va);

	return res;
}
SIDEF
isize si_snprintf(char* buffer, usize outCapacity, cstring fmt, ...) {
	va_list va;
	va_start(va, fmt);
	isize res = si_snprintfVa(buffer, outCapacity, fmt, va);
	va_end(va);

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

	usize len = si_min(isize, info->str.len, (info->capacity - info->str.len));

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
	usize len = si_min(isize, info->str.len, (info->capacity - info->str.len));

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
					default: SI_PANIC();
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
						? afterPoint
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
				i8 exponent = 0;
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
					remainder[3] = exponent + '0';
				}
				else {
					remainder[2] = (exponent / 10) + '0';
					remainder[3] = (exponent % 10) + '0';
				}

				info.str = SI_STR_LEN(remainder, sizeof(remainder));
				si__printStrCpy(&info);
			} break;

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
				siAllocatorData inData;
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
						usize i = countof_str("\33[38;5;");
						siAllocator tmp = si_allocatorMakeTmp(&str[i], countof(str), &inData);

						siString valueStr = si_stringFromInt(clr.data.cube, tmp);
							i += valueStr.len;
						str[i] = 'm',
							i += 1;

						info.str = SI_STR_LEN(str, i);
						si__printStrCpy(&info);
					} break;

					case siPrintColorType_24bit: {
						char str[64] ="\33[38;2;";
						usize i = countof_str("\33[38;2;");

						static char buf[3] = {';', ';', 'm'};
						for_range (j, 0, countof(clr.data.rgb)) {
							siAllocator tmp = si_allocatorMakeTmp(&str[i], countof(str), &inData);
							siString valueStr = si_stringFromInt(clr.data.rgb[j], tmp);
								i += valueStr.len;
							str[i] = buf[j],
								i += 1;
						}

						info.str = SI_STR_LEN(str, i);
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

		si_allocatorReset(stack);
	}
	buffer[info.index] = '\0';
	info.index += 1;

	return info.index;
}

#undef SI_CHECK_AFTERPOINT_INT
#undef SI_SET_FMT_PTR

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
	return si_transmute(siDllProc, *proc, PROC);

#else
	return (siDllProc)dlsym(dll, name);

#endif
}
#endif /* defined(SI_IMPLEMENTATION_DLL) */

#if defined(SI_IMPLEMENTATION_MATH) && !defined(SI_NO_MATH)

SI_MATH_FUNC_DECLARE_2X(min, inline, { return (a < b) ? a : b; })
SI_MATH_FUNC_DECLARE_2X(max, inline, { return (a > b) ? a : b; })
SI_MATH_FUNC_DECLARE_1X_SIGNED(abs, inline, { return a < 0 ? -a : a; })
SI_MATH_FUNC_DECLARE_3X_B32(between, inline, { return b <= a && a <= c; })

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
inline f32 si_floorF32(f32 x) { return (x >= 0.0f) ? si_cast(f32, (i32)x) : si_cast(f32, (i32)(x - 0.9999999999999999f)); }

inline f64 si_ceilF64(f64 x)  { return (x < 0) ? si_cast(f64, (i64)x) : si_cast(f64, (i64)x + 1); }
inline f32 si_ceilF32(f32 x)  { return (x < 0) ? si_cast(f32, (i32)x) : si_cast(f32, (i32)x + 1); }


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

#if defined(SI_IMPLEMENTATION_BENCHMARK) && !defined(SI_NO_BENCHMARK)

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
	f64 freq = (f64)si_cpuClockSpeed() / 1000;

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
			si_printColor3bit(siPrintColorAnsi_Green), time, element->unit, cycles
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
		si_printColor3bit(siPrintColorAnsi_Green),
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
	f64 freq = (f64)si_cpuClockSpeed() / 1000;

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
			clr[0] = si_printColor3bit(siPrintColorAnsi_Red);
			clr[1] = si_printColor3bit(siPrintColorAnsi_Green);
		}
		else if (time[0] < time[1]) {
			ratio = time[1] / time[0];
			clr[0] = si_printColor3bit(siPrintColorAnsi_Green);
			clr[1] = si_printColor3bit(siPrintColorAnsi_Red);
		}
		else {
			ratio = 1.0;
			clr[0] = clr[1] = si_printColor3bit(siPrintColorAnsi_Yellow);
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
		clr[0] = si_printColor3bit(siPrintColorAnsi_Red);
		clr[1] = si_printColor3bit(siPrintColorAnsi_Green);
	}
	else if (time[0] < time[1]) {
		ratio = time[1] / time[0];
		clr[0] = si_printColor3bit(siPrintColorAnsi_Green);
		clr[1] = si_printColor3bit(siPrintColorAnsi_Red);
	}
	else {
		ratio = 1.0;
		clr[0] = clr[1] = si_printColor3bit(siPrintColorAnsi_Yellow);
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

const siBenchmarkLimit* si_benchmarkLimitLoop(siTimeStamp time) {
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
	#define SI_ASSERT_MSG(condition, message) SI_ASSERT_FMT(condition, message, "")
	/* condition - EXPRESSION | message - cstring | ...FMT - VARIADIC
	 * Crashes the app with a formatted message if the condition is not met. */
	#define SI_ASSERT_FMT(condition, message, ...) \
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
