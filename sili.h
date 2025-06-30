/*
sili.h - v0.3.0 - a general-purpose programming library to replace the C/C++ standard libraries.
===========================================================================
	- YOU MUST DEFINE 'SI_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SI_IMPLEMENTATION
		#include "sili.h"

	- All other files should just include the library without the #define macro.

	- A compiler with C23/C++20 standard support or required extensions is expected. 
	Do not expect sili and its libraries to function otherwise.

	- If you want to disable certain features, you can do:
		- #define SI_NO_VARIADIC
		- #define SI_NO_MEMORY
		- #define SI_NO_ALLOCATOR
		- #define SI_NO_ARRAY
		- #define SI_NO_STRING
		- #define SI_NO_OPTIONAL
		- #define SI_NO_UNICODE
		- #define SI_NO_CHAR
		- #define SI_NO_HASHING
		- #define SI_NO_MAP
		- #define SI_NO_TIME
		- #define SI_NO_BIT
		- #define SI_NO_PRINT
		- #define SI_NO_MATH

		- #define SI_NO_SYSTEM
		- #define SI_NO_VIRTUAL_MEMORY
		- #define SI_NO_IO
		- #define SI_NO_PRINT_SYSTEM
		- #define SI_NO_THREAD
		- #define SI_NO_CPU
		- #define SI_NO_DLL
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
		- ENUM - any legal enumerator.
		- VARIADIC - an indefinite amount of legal C expressions ("firstValue", 230, "third").
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

	- SI_NO_ASSERTIONS - all 'SI_ASSERT' functions get disabled entirely. 'SI_PANIC'
	functions still function.

	- SI_NO_ERROR_STRUCT - strips the 'siError' structure's members down to just
	the error code member. This gets turned on automatically via 'SI_RELEASE_MODE'.

	- SI_NO_ERROR_LOGS - disables sili's automatic logging for when a 'SI_ERROR'
	is declared. This gets turned on automatically via 'SI_RELEASE_MODE'.

	- SI_NO_WINDOWS_H - disables the inclusion of the win32 API inside the header.

===========================================================================
CREDITS
	- Ginger Bill's 'gb.h' (https://github.com//gingerBill/gb) - inspired me to
	make the Sili Toolchain, as well as certain features were taken from the
	library and implemented here.

LICENSE
	- This software is licensed under the zlib license (see the LICENSE at the
	bottom of the file).

WARNING
	- Sili and its supplementary libraries are designed to be fast, modern, but
	also experimental. As a result some unwarranted results may occur during use,
	such as:
		1) Features not working as expected;
		2) Functions having no or incompleted documentation;
		3) API breaking changes between releases (especially before v1.0.0 release);
		4) Little to no security checks for malicious code that attempt to exploit
		parts of the code.

*/

#if (_MSC_VER >= 1020) || (__GNUC__ >= 3)
	#pragma once
#endif

#ifndef SI_INCLUDE_SI_H
#define SI_INCLUDE_SI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SI_VERSION_MAJOR
	#define SI_VERSION_MAJOR 0
	#define SI_VERSION_MINOR 4
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
	#define SI_SYSTEM_IS_BSD 1

	#if TARGET_OS_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
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
	#define SI_SYSTEM_IS_BSD 1


#elif defined(__OpenBSD__)
	#define SI_SYSTEM_OPENBSD
	#define SI_SYSTEM_STR "OpenBSD"
	#define SI_SYSTEM_IS_UNIX 1
	#define SI_SYSTEM_IS_BSD 1

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
	#else
		#define SI_LANGUAGE_C 1
		#define SI_LANGUAGE_STR "C"
	#endif

	#define SI_LANGUAGE_IS_C 1
#endif

#if SI_LANGUAGE_IS_C
	#ifndef SI_STANDARD_VERSION
		#if defined(__STDC_VERSION__)
			#define SI_STANDARD_VERSION __STDC_VERSION__
		#else
			#define SI_STANDARD_VERSION 198900L
		#endif
	#endif

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


#ifndef SI_ARCH_STR
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

	#elif defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
		#define SI_ARCH_PPC64 1
		#define SI_ARCH_STR "PowerPC 64-bit"

		#define SI_ARCH_IS_PPC 1
		#define SI_ARCH_IS_64BIT 1

	#elif defined(__powerpc__) || defined(__powerpc) || defined(__POWERPC__) || defined(__ppc__) \
		|| defined(__PPC__) || defined(_M_PPC) || defined(_ARCH_PPC) || defined(__PPCGECKO__) \
		|| defined(__PPCBROADWAY__) || defined(_XENON) || defined(__ppc)
		#define SI_ARCH_PPC32 1
		#define SI_ARCH_STR "PowerPC 32-bit"

		#define SI_ARCH_IS_PPC 1
		#define SI_ARCH_IS_32BIT 1

	#elif defined(__sh__)
		#define SI_ARCH_SUPERH 1
		#define SI_ARCH_STR "SuperH"
		#define SI_ARCH_IS_32BIT 1
		#define SI_ARCH_IS_SUPERH 1

	#else
		#define SI_ARCH_UNKNOWN 1
		#define SI_ARCH_STR "Unknown"
		#define SI_ARCH_IS_32BIT 1 /* Assume that stuff is 32-bit for the sake of it. */

		#warning "Expect errors."
	#endif

#endif /* #ifndef SI_ARCH_STR */

#if !defined(SI_ENDIAN_STR) && (!defined(SI_ENDIAN_IS_LITTLE) || !defined(SI_ENDIAN_IS_BIG))
	#if SI_COMPILER_GCC || SI_COMPILER_CLANG
		#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			#define SI_ENDIAN_IS_LITTLE 1
			#define SI_ENDIAN_STR "Little-endian"

		#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			#define SI_ENDIAN_IS_BIG 1
			#define SI_ENDIAN_STR "Big-endian"

		#else
			#define SI_ENDIAN_UNKNOWN 1
			#define SI_ENDIAN_STR "Unknown"
			#warning "Expect undefined behaviour"

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
			#warning "Expect undefined behaviour"

		#endif
	#endif
#endif


#if defined(SI_EXPORT) || defined(SI_IMPORT)
	#if SI_SYSTEM_IS_WINDOWS
		#if defined(SI_EXPORT)
			#define SIDEF __declspec(dllexport)
		#else
			#define SIDEF __declspec(dllimport)
		#endif
	#elif defined(SI_EXPORT)
		#define SIDEF __attribute__((visibility("default")))
	#endif
#endif

#ifndef SIDEF
	#define SIDEF extern
#endif


#ifndef SI_STATIC_ASSERT_MSG
	#if SI_STANDARD_CHECK_MIN(C, C23) || SI_LANGUAGE_IS_CPP
		/* condition - EXPRESSIONG | msg - cstring
		 * Stops the program from being compiled if the condition isn't met with
		 * an explanation as to why. */
		#define SI_STATIC_ASSERT_MSG(condition, msg) static_assert(condition, msg)
	#else
		/* condition - EXPRESSIONG | msg - cstring
		* Stops the program from being compiled if the condition isn't met with
		* an explanation as to why. */
		#define SI_STATIC_ASSERT_MSG(condition, msg) _Static_assert(condition, msg)
	#endif
#endif

/* condition - EXPRESSIONG
* Stops the program from being compiled if the condition isn't met. */
#define SI_STATIC_ASSERT(condition) SI_STATIC_ASSERT_MSG(condition, "")



#if SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE || SI_SYSTEM_EMSCRIPTEN
	#ifndef _GNU_SOURCE
	#define _GNU_SOURCE 1
	#endif

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

	#ifndef SI_NO_DLL
		#include <dlfcn.h>
	#endif


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

#else
	#if !defined(SI_NO_MEMORY) && !defined(SI_NO_CRT)
		#include <stdlib.h>
		#include <string.h>
	#endif

	#ifndef SI_NO_TIME
		#include <time.h>

		#if SI_SYSTEM_UNKNOWN && SI_STANDARD_CHECK_MIN(C, C11) && !defined(__STDC_NO_THREADS__)
		#include <threads.h>
		#endif
	#endif

	#ifndef SI_NO_SYSTEM
		#include <errno.h>
	#endif

	#ifndef SI_NO_IO
		#include <stdio.h>
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
	#include <wasi/api.h>
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

	typedef u8   b8;
	typedef u16 b16;
	typedef u32 b32;
	typedef u64 b64;

	#if !defined(bool) && SI_STANDARD_CHECK_MAX(C, C17)
	typedef _Bool bool;
	#endif
#endif

SI_STATIC_ASSERT(sizeof(u8)    == sizeof(i8));
SI_STATIC_ASSERT(sizeof(u16)   == sizeof(i16));
SI_STATIC_ASSERT(sizeof(u32)   == sizeof(i32));
SI_STATIC_ASSERT(sizeof(u64)   == sizeof(i64));
SI_STATIC_ASSERT(sizeof(usize) == sizeof(isize));

SI_STATIC_ASSERT(sizeof(bool) == 1);
SI_STATIC_ASSERT(sizeof(u8)   == 1);
SI_STATIC_ASSERT(sizeof(u16)  == 2);
SI_STATIC_ASSERT(sizeof(u32)  == 4);
SI_STATIC_ASSERT(sizeof(u64)  == 8);

#if SI_ARCH_IS_32BIT
	SI_STATIC_ASSERT(sizeof(usize) == 4);
#else
	SI_STATIC_ASSERT(sizeof(usize) == 8);
#endif

SI_STATIC_ASSERT(sizeof(f32) == 4);
SI_STATIC_ASSERT(sizeof(f64) == 8);






SI_STATIC_ASSERT(sizeof(bool) == 1);
SI_STATIC_ASSERT(sizeof(b8)   == 1);
SI_STATIC_ASSERT(sizeof(b16)  == 2);
SI_STATIC_ASSERT(sizeof(b32)  == 4);
SI_STATIC_ASSERT(sizeof(b64)  == 8);


#ifndef cstring
	/* An immutable NULL-terminated C-string type. */
	typedef const char* cstring;
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

#ifndef siFallthrough
	#if SI_STANDARD_CHECK_MIN(C, C23) || SI_STANDARD_CHECK_MIN(CPP, CPP17)
		/* Specifies a fallthrough for the compiler. */
		#define siFallthrough [[fallthrough]]

	#elif SI_COMPILER_GCC || SI_COMPILER_CLANG
		/* Specifies a fallthrough for the compiler. */
		#define siFallthrough __attribute__((fallthrough))

	#else
		/* Specifies a fallthrough for the compiler. */
		#define siFallthrough do {} while (0)

	#endif
#endif


#ifndef restrict
	#if SI_LANGUAGE_IS_CPP
		#if SI_COMPILER_CLANG || SI_COMPILER_GCC || SI_COMPILER_MSVC
			#define restrict __restrict
		#else
			#define restrict
		#endif
	#endif
#endif

#ifndef siNoreturn
	#if SI_STANDARD_CHECK_MIN(C, C23) || SI_LANGUAGE_IS_CPP
		/* Specifies that the function will not return anything. */
		#define siNoreturn [[noreturn]]

	#else
		/* Specifies that the function will not return anything. */
		#define siNoreturn _Noreturn
	#endif
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
/* x - INT.
 * Converts petabytes into bytes (JEDEC). */
#define SI_PETA(x) (SI_TERA(x) * 1024LL)

/* x - UINT
 * Denotes a bit value. */
#define SI_BIT(x) (1ULL << (x))

#ifndef SI_PATH_MAX
	/* The maximum amount of UTF-8 bytes a singular path can hold. */
	#define SI_PATH_MAX SI_KILO(4)
#endif

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
	#define FLOAT32_MIN 1.17549435e-38f
#endif

#ifndef FLOAT32_MAX
	#define FLOAT32_MAX 3.40282347e+38f
#endif

#ifndef FLOAT64_MIN
	#define FLOAT64_MIN 2.2250738585072014e-308
#endif

#ifndef FLOAT64_MAX
	#define FLOAT64_MAX 1.7976931348623157e+308
#endif

#if SI_STANDARD_CHECK_MAX(C, C17)
	#ifndef true
	#define true (bool)(1)
	#endif

	#ifndef false
	#define false (bool)(0)
	#endif
#endif

SI_STATIC_ASSERT(true  == 1);
SI_STATIC_ASSERT(false == 0);

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

#ifndef SI_BITMASK_TYPE
	/* type - TYPE | enumerator - ENUM | name - NAME
	 * Defines a bitmask type from the specified enumerator and given type. The
	 * element count of the enum is assumed to be '<enum>_Len'. The given type
	 * must be large enough to contain all of the enum elements, otherwise the
	 * statement will fail.
	 *
	 * If you use a different name to denote the enum's length, you can redefine
	 * the macro. */
	#define SI_BITMASK_TYPE(type, enumerator, name) \
		SI_BITMASK_TYPE_EX(type, enumerator, enumerator##_Len, name)

#endif

/* type - TYPE | enumerator - ENUM | numOfElements - isize | name - NAME
 * Defines a bitmask type from the specified enumerator, its count of elements
 * and given type. The given type must be large enough to contain all of the
 * enum elements, otherwise the statement will fail. */
#define SI_BITMASK_TYPE_EX(type, enumerator, enumLen, name) \
	typedef type name; \
	SI_STATIC_ASSERT_MSG( \
		enumLen <= si_sizeof(name) * 8, \
		"Bitmask type is too small to fit all enumerator elements."\
	)

/* enumerator - ENUM | ...elements - NAME
 * Creates a bitmask from the given enumerator's elements. All elements are
 * automatically prefixed by 'enumerator_', meaning TODO */
#define SI_BITMASK_MAKE(enumerator, .../* elements */) SI_VARIADIC(si__ba, enumerator, __VA_ARGS__)

/*
	========================
	| Operators            |
	========================
*/


#ifndef typeof
	#if SI_LANGUAGE_IS_CPP
		extern "C++" {
			template<typename T>
			struct remove_reference {
				using type = T;
			};

			template<typename T>
			struct remove_reference<T&> {
				using type = T;
			};

			template<typename T>
			struct remove_reference<T&&> {
				using type = T;
			};

			template<typename T>
			struct decay {
			private:
				using U = typename remove_reference<T>::type;
			public:
				using type = U;
			};

			template<typename T>
			T&& declval(); // Only declaration, no definition needed

			#define CC_TYPEOF_XP(xp) decay<remove_reference<decltype(xp)>::type>::type
			#define CC_TYPEOF_TY(ty) decay<remove_reference<decltype(declval<ty>())>::type>::type

			/* TODO */
			//#define typeof CC_TYPEOF_TY
			#define typeof( expr ) decay<remove_reference<decltype(expr)>::type>::type

		}

	#elif SI_STANDARD_CHECK_MIN(C, C23)
		/* TODO */
		#define typeof typeof
	#elif SI_COMPILER_GCC || SI_COMPILER_CLANG || SI_COMPILER_CHECK_MIN(MSVC, 19, 39, 33428)
		/* TODO */
		#define typeof __typeof__
	#else
		#error
			"Sili has been unable to detect 'typeof' equivalent functionality in "
			"this compiler, rendering the entire library unusable. If your compiler "
			"does contain such a feature or anything similiar to it, define your "
			"own 'typeof', otherwise you must switch to C23 or C++11."
	#endif
#endif

#ifndef si_sizeof
	/* ...expr - EXPRESSION
	 * Returns the _signed_ size of an expression. */
	#define si_sizeof(.../* expr */) ((isize)sizeof(__VA_ARGS__))
#endif

#ifndef countof
	/* ...value - ARRAY
	 * Gets the static length of the given value (must be an array). */
	#define countof(.../*value*/) (isize)(sizeof(__VA_ARGS__) / sizeof((__VA_ARGS__)[0]))
#endif

#ifndef countof_str
	/* ...value - ARRAY
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

#if !defined(alignof) && SI_STANDARD_CHECK_MAX(C, C17)
	/* type - TYPE
	* Gets the alignment of a type. */
	#define alignof(type) _Alignof(type)
#endif

/*
	========================
	| Casting              |
	========================
*/

#ifndef transmute
	#if SI_COMPILER_GCC || SI_COMPILER_CLANG || SI_COMPILER_MSVC
		#if SI_LANGUAGE_IS_C
			/* type - TYPE | value - EXPRESSION | valueType - TYPE
			* Type prunes a value with the specified type. */
			#define transmute(type, value) ((union { typeof(value) in; type out; }){value}.out)
		#else
			/* type - TYPE | value - EXPRESSION | valueType - TYPE
			* Type prunes a value with the specified type. */
			#define transmute(type, value) si_cppTransmute<type, typeof(value)>(value)

			extern "C++" {
				template<typename type, typename valueType>
				type si_cppTransmute(valueType value) {
					union { valueType in; type out; } res = {value};
					return res.out;
				}
			}
		#endif
	#else
		#error "Needs an update."
	#endif
#endif

/*
	========================
	| General macros       |
	========================
*/

/* a - VARIABLE | b - VARIABLE
 * Swaps the value of 'a' with 'b'; 'b' with 'a'. */
#define si_swap(a, b) do { \
	typeof(a) tmp = a; \
	a = b; \
	b = tmp; \
} while (0)

#ifndef for_range
	/* countvar - NAME | start - INT | end - INT
	 * A loop with that goes through 'end' - 'start' cycles whilst incrementing 'countVar'
	 * each time. */
	#define for_range(countVar, start, end) for_rangeEx(typeof(end), countVar, start, end)
#endif

#ifndef for_rangeEx
	/* type - TYPE | countvar - NAME | start - INT | end - INT
	 * A loop with that goes through 'end' - 'start' cycles whilst incrementing 'countVar'
	 * each time. */
	#define for_rangeEx(type, countVar, start, end) \
		for (type countVar = (start); (countVar) < (end); countVar += 1)
	/* countvar - NAME | start - INT | end - INT
	 * A loop with that goes through 'end' - 'start' cycles whilst incrementing 'countVar'
	 * each time. 'countVar' must be declared beforehand. */
	#define for_rangeRef(countVar, start, end) for_rangeEx (, countVar, start, end)
#endif


#if SI_COMPILER_GCC || SI_COMPILER_CLANG
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

/* TODO(EimaMei): Rework this so that there's only a SI_ASSERT. */
#ifndef SI_NO_ASSERTIONS
	/* condition - EXPRESSION
	 * Terminates the program if the condition is not met. */
	#define SI_ASSERT(condition) SI_STOPIF(!(condition), si_panic(SI_STR(#condition)))
	/* condition - EXPRESSION | message - string
	 * Terminates the program with a message if the condition is not met. */
	#define SI_ASSERT_MSG(condition, message) \
		SI_STOPIF(!(condition), si_panic(SI_STR(#condition), SI_STR(message)))
	/* condition - EXPRESSION | message - siString | ...fmt - VARIADIC
	 * Terminates the program with a formatted message if the condition is not met. */
	#define SI_ASSERT_FMT(condition, message, .../* fmt */) \
		SI_STOPIF(!(condition), si_panic(SI_STR(#condition), SI_STR(message), SI_ARGS(__VA_ARGS__)))

	/* ptr - void*
	 * Terminates the program if a pointer is nil. */
	#define SI_ASSERT_NOT_NIL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL.")
	/* num - INT
	 * Terminates the program if the integer is negative. */
	#define SI_ASSERT_NOT_NEG(num) SI_ASSERT_FMT((num) >= 0, SI_STR(#num " must not be negative (%i)."), num)

	/* str - siString
	 * Terminates the program if the string is null or its length is negative. */
	#define SI_ASSERT_STR(str) SI_ASSERT_NOT_NIL((str).data); SI_ASSERT_NOT_NEG((str).len)

	/* buffer - siArrayAny
	 * Terminates the program if the buffer is null or its length is negative. */
	#define SI_ASSERT_ARR(buffer) SI_ASSERT_NOT_NEG((buffer).len)
	/* buffer - siArrayAny | type - TYPE
	 * Terminates the program if the buffer is null or its length is negative
	 * or its types is incorrect. */
	#define SI_ASSERT_ARR_TYPE(buffer, type) \
		SI_ASSERT_ARR(buffer); \
		SI_ASSERT_FMT( \
			(buffer).typeSize == si_sizeof(type), \
			"Specified buffer's type size must be equal to the size of '" #type "' ('%i' vs '%i')", \
			(buffer).typeSize, si_sizeof(type) \
		)

	/* array - siDynamicArrayAny
	 * Terminates the program if the array is null or its length or capacity is
	 * negative. */
	#define SI_ASSERT_DYN_ARR(array) SI_ASSERT_STR(array); SI_ASSERT_NOT_NEG((array).capacity)
	/* array - siDynamicArrayAny | type - TYPE
	 * Terminates the program if the array is null or its length or capacity is
	 * negative or its types is incorrect. */
	#define SI_ASSERT_DYN_ARR_TYPE(array, type) SI_ASSERT_ARR_TYPE(array, type); SI_ASSERT_NOT_NEG((array).capacity)

#else
	#define SI_ASSERT(condition) do { } while(0)
	#define SI_ASSERT_MSG(condition, message) do {} while(0)
	#define SI_ASSERT_FMT(condition, message, ...) do {} while(0)
	#define SI_ASSERT_NOT_NIL(ptr) do { SI_UNUSED(ptr); } while(0)
	#define SI_ASSERT_NOT_NEG(num) do { SI_UNUSED(num); } while(0)
	#define SI_ASSERT_STR(str) do { SI_UNUSED(str); } while(0)
	#define SI_ASSERT_ARR(buffer) SI_ASSERT_STR(buffer)
	#define SI_ASSERT_ARR_TYPE(buffer, type) SI_ASSERT_STR(buffer)
 	#define SI_ASSERT_DYN_ARR(array) SI_ASSERT_STR(array)
	#define SI_ASSERT_DYN_ARR_TYPE(array, type) SI_ASSERT_STR(array)

#endif /* SI_NO_ASSERTIONS */

/* TODO(EimaMei): Rework this so that there's only a SI_PANIC. */
/* Terminates the program immediately. */
#define SI_PANIC() SI_PANIC_MSG("")
/* message - cstring
 * Terminates the program immediately with a message. */
#define SI_PANIC_MSG(message) si_panic(SI_STR("SI_PANIC()"), SI_STR(message))
/* message - cstring | ...FMT - VARIADIC
 * Terminates the program immediately with a formatted message. */
#define SI_PANIC_FMT(message, ...) si_panic(SI_STR("SI_PANIC()"), SI_STR(message), SI_ARGS(__VA_ARGS__))
/* TODO(EimaMei): Find a way to return SI_ARR_EMPTY if __VA_ARGS__ is just blank. */

/* condition - EXPRESSION | ACTION - ANYTHING
 * Checks if the condition is true. If it is, 'action' will be executed. */
#define SI_STOPIF(condition, .../* ACTION */) if (condition) { __VA_ARGS__; } do {} while(0)

/*
	========================
	| C/C++ macros         |
	========================
*/

#if SI_LANGUAGE_IS_C
	#ifndef SI_STRUCT_ZERO
		/* Default zero-initialization value on C. */
		#define SI_STRUCT_ZERO {0}
	#endif

	#ifndef SI_COMP_LIT
		/* type - TYPE | ...value - ANYTHING
		 * Compound literal syntax on C. */
		#define SI_COMP_LIT(type, .../* value */) ((type){__VA_ARGS__})
		/* type - TYPE | ...value - ANYTHING
		 * A zero value compound literal on C. */
		#define SI_TYPE_ZERO(type) ((type)SI_STRUCT_ZERO)
	#endif

	#ifndef SI_PTR
		/* type - TYPE | ..value - ANYTHING
		 * Returns a pointer to the given list of values.  */
		#define SI_PTR(type, ...) ((type[]){__VA_ARGS__})
		/* type - TYPE | ..value - ANYTHING
		 * Returns a pointer to the list of and its length, separated by a comma. */
		#define SI_PTR_WITH_LEN(type, ...) (type[]){__VA_ARGS__}, countof((type[]){__VA_ARGS__})
	#endif

#else
	#ifndef SI_STRUCT_ZERO
		/* Default zero-initialization value on C++. */
		#define SI_STRUCT_ZERO {}
	#endif

	#ifndef SI_COMP_LIT
		/* type - TYPE | ...value - ANYTHING
		 * Compound literal syntax on C++. */
		#define SI_COMP_LIT(type, ...) (type{__VA_ARGS__})
		/* type - TYPE | ...value - ANYTHING
		 * A zero value compound literal on C. */
		#define SI_TYPE_ZERO(type) (type SI_STRUCT_ZERO)
	#endif

	
	/* TODO(EimaMei): This completely fails in C++. Find a fix. Desperately. */
	#ifndef SI_PTR
		/* type - TYPE | ..value - ANYTHING
		 * Returns a pointer to the given list of values.  */
		#define SI_PTR(type, ...) ([&] { \
			type temp = __VA_ARGS__; \
			return temp; \
		}())

		/* type - TYPE | ..value - ANYTHING
		 * Returns a pointer to the list of and its length, separated by a comma. */
		#define SI_PTR_WITH_LEN(type, ...) SI_PTR(type, __VA_ARGS__), ([] { \
			static type temp[] = {__VA_ARGS__}; \
			return countof(temp); \
		}())
	#endif
#endif


#if SI_LANGUAGE_IS_CPP
	/* TODO */
	#define SI_DEFAULT(value) = value
#else
	/* TODO */
	#define SI_DEFAULT(value)
#endif

/*
	========================
	| siCommon             |
	========================
*/

SI_ENUM(i32, siAllocationType) {
	siAllocationType_Alloc,
	siAllocationType_AllocNonZeroed,

	siAllocationType_Resize,
	siAllocationType_ResizeNonZeroed,

	siAllocationType_Free,
	siAllocationType_FreeAll,

	siAllocationType_MemAvailable,
	siAllocationType_GetFeatures,

	siAllocationType_Len,
};
SI_BITMASK_TYPE(u8, siAllocationType, siAllocationType_Flags);

SI_ENUM(i32, siAllocationError) {
	siAllocationError_None,
	siAllocationError_OutOfMem,
	siAllocationError_InvalidPtr,
	siAllocationError_InvalidArg,
	siAllocationError_NotImplemented,
};


/* name - NAME
 * Defines a valid allocator function prototype.
 *
 *
 * PARAMETERS:
 * - siAllocationType type - denotes what allocator action is being requested.
 *
 * - void* ptr - the pointer to the given memory block. This is only valid for
 *   the Resize and Free types. 'ptr' is guaranteed to not be a nil value for
 *   valid types.
 *
 * - isize oldSize - the old, current size of the given memory block. This is
 *   only valid for the Resize types. 'oldSize' is guaranteed to not be a negative
 *   number.
 *
 * - isize newSize - the new requested size for a given or newmemory block. This
 *   is only valid for the Alloc and Resize types. 'newSize' is guaranteed to
 *   not be a negative number and, for optimizations, is also larger than 'oldSize'.
 *
 * - isize align - the used-specific alignment for an allocation. This is only
 *   valid for Alloc and Resize types. 'align' is always guaranteed to be a
 *   power of two number, starting from 1.
 *
 * - siAllocationError* outError - a parameter to denote an error or a succesful
 *   action. Guaranteed to never be nil except for '.GetFuture' as the function
 *   musn't fail. If no error is encountered, the out parameter _must_ be set
 *   to zero by the allocator.
 *
 * - void* allocatorData - a user-specified parameter. Can be nil.
 */
#define SI_ALLOCATOR_PROC(name) \
	void* name(siAllocationType type, void* ptr, isize oldSize, isize newSize, \
			isize align, siAllocationError* outError, void* allocatorData)

/* Represents an allocator procedure. */
typedef SI_ALLOCATOR_PROC(siAllocatorProc);

typedef struct siAllocator {
	siAllocatorProc* proc;
	void* data;
} siAllocator;


typedef struct siArrayAny {
	isize len;
	void* data;
	isize typeSize;

	#if SI_LANGUAGE_IS_CPP
	/* Sets/gets a specific element in the array with array and bound checking. */
	u8& operator[](isize index);
	/* Creates a slice. Same as 'si_slice'. */
	siArrayAny operator()(isize offset1, isize offset2);
	/* Creates a slice. Same as 'si_sliceFrom'. */
	siArrayAny from(isize offset1);
	/* Creates a slice. Same as 'si_sliceTo'. */
	siArrayAny to(isize offset2);
	/* Creates a slice. Same as 'si_sliceLen'. */
	siArrayAny sliceLen(isize offset1, isize len);
	#endif
} siArrayAny;

/* type - TYPE
 * Represents a buffer with a specific type. */
#define siArray(type) siArrayAny


typedef struct siDynamicArrayAny {
	isize len;
	void* data;
	isize typeSize;

	siAllocator alloc;
	isize capacity;
	isize grow;

	#if SI_LANGUAGE_IS_CPP
	/* Sets/gets a specific element in the array with array and bound checking. */
	u8& operator[](isize index) const;
	#endif
} siDynamicArrayAny;

/* type - TYPE
 * Represents a dynamic array with a specific type. */
#define siDynamicArray(type) siDynamicArrayAny


/* A UTF-32 codepoint.*/
typedef i32 siRune;

typedef struct siString {
	isize len;
	const u8* data;

#if SI_LANGUAGE_IS_CPP
	/* Sets/gets a specific element in the array with array and bound checking. */
	u8 operator[](isize index);

	/* Creates a substring. Same as 'si_substr'. */
	siString operator()(isize offset1, isize offset2);
	/* Creates a substring. Same as 'si_substrFrom'. */
	siString from(isize offset1);
	/* Creates a substring. Same as 'si_substrTo'. */
	siString to(isize offset2);
	/* Creates a substring. Same as 'si_substrLen'. */
	siString substrLen(isize offset1, isize length);
#endif
} siString;

typedef struct siCallerLoc {
	siString filename;
	siString function;
	i32 line;
} siCallerLoc;


SI_ENUM(i32, siTypeId) {
	siTypeId_Unknown,

	siTypeId_char,
	siTypeId_u8,
	siTypeId_u16,
	siTypeId_u32,
	siTypeId_u64,

	siTypeId_i8,
	siTypeId_i16,
	siTypeId_i32,
	siTypeId_i64,

	siTypeId_bool,
	siTypeId_b8,
	siTypeId_b16,
	siTypeId_b32,
	siTypeId_b64,

	siTypeId_f32,
	siTypeId_f64,

	siTypeId_cstring,
	siTypeId_cliterals,
	siTypeId_voidptr,

	siTypeId_string,
	siTypeId_array,

	siTypeId_siCallerLoc,
	#ifdef SI_ADDITIONAL_TYPES
	SI_ADDITIONAL_TYPES
	#endif

	siTypeId_Len
};

#if SI_LANGUAGE_IS_C

#if SI_ARCH_IS_64BIT
	#define __SI_TYPEID_SIZE
#else
	#define __SI_TYPEID_SIZE , isize : siTypeId_i32, usize : siTypeId_u32
#endif

#define si_typeid(type) _Generic(SI_TYPE_ZERO(type), \
	default       : siTypeId_Unknown, \
	char          : siTypeId_char, \
	u8            : siTypeId_u8, \
	u16           : siTypeId_u16, \
	u32           : siTypeId_u32, \
	u64           : siTypeId_u64, \
	i8            : siTypeId_i8, \
	i16           : siTypeId_i16, \
	i32           : siTypeId_i32, \
	i64           : siTypeId_i64, \
	bool          : siTypeId_bool, \
	f32           : siTypeId_f32, \
	f64           : siTypeId_f64, \
	char*         : siTypeId_cstring, \
	const char*   : siTypeId_cstring, \
	void*         : siTypeId_voidptr, \
	siString      : siTypeId_string, \
	siArrayAny    : siTypeId_array, \
	siCallerLoc   : siTypeId_siCallerLoc \
	__SI_TYPEID_SIZE)

#else

extern "C++" {
template<typename T>
	struct __siTypeID {
		static const siTypeId id = siTypeId_Unknown;
	};

	template<> struct __siTypeID<u8>  { static const siTypeId id = siTypeId_u8; };
	template<> struct __siTypeID<u16> { static const siTypeId id = siTypeId_u16; };
	template<> struct __siTypeID<u32> { static const siTypeId id = siTypeId_u32; };
	template<> struct __siTypeID<u64> { static const siTypeId id = siTypeId_u64; };

	template<> struct __siTypeID<char> { static const siTypeId id = siTypeId_char; };
	template<> struct __siTypeID<i8>   { static const siTypeId id = siTypeId_i8; };
	template<> struct __siTypeID<i16>  { static const siTypeId id = siTypeId_i16; };
	template<> struct __siTypeID<i32>  { static const siTypeId id = siTypeId_i32; };
	template<> struct __siTypeID<i64>  { static const siTypeId id = siTypeId_i64; };

	template<> struct __siTypeID<bool>     { static const siTypeId id = siTypeId_bool; };
	template<> struct __siTypeID<f32>    { static const siTypeId id = siTypeId_f32; };
	template<> struct __siTypeID<f64>   { static const siTypeId id = siTypeId_f64; };

	template<> struct __siTypeID<char*>    { static const siTypeId id = siTypeId_cstring; };
	template<> struct __siTypeID<void*>    { static const siTypeId id = siTypeId_voidptr; };
	template<> struct __siTypeID<typeof(nullptr)>    { static const siTypeId id = siTypeId_voidptr; };
	template<> struct __siTypeID<siString> { static const siTypeId id = siTypeId_string; };
	template<> struct __siTypeID<siCallerLoc> { static const siTypeId id = siTypeId_siCallerLoc; };
	template<> struct __siTypeID<siArrayAny> { static const siTypeId id = siTypeId_array; };
}

#define si_typeid(Type) __siTypeID<Type>::id

#endif


#ifndef SI_NO_VARIADIC
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
	| siVariadic           |
	========================
*/

/* TODO(EimaMei): Add a rationale behind this macro hell.*/
/* TODO(EimaMei): Explain how to define siVariadic functions and how to use them. */

/* TODO */
#define SI_ARGS(...) SI_ARR_TYPE(SI_PTR(siAny, SI_VARIADIC(si__ma, __VA_ARGS__)), SI_ARG_LEN(__VA_ARGS__), siAny)
/* TODO(EimaMei): Make this prettier. */
#if SI_LANGUAGE_IS_C
/* TODO */
#define SI_DARG_IMPL(name, required, _default, ...) (name)(SI__DARG_IMPL_2(_default, required, SI_ARG_LEN(si__repeat_##required, SI_EXP _default), SI_ARG_LEN(__VA_ARGS__), __VA_ARGS__))
#define SI_DARG_IMPL_WF(required, _default, ...) SI__DARG_IMPL_2(_default, required, SI_ARG_LEN(si__repeat_##required, SI_EXP _default), SI_ARG_LEN(__VA_ARGS__), __VA_ARGS__)
#else
/* TODO | C++ comment */
#define SI_DARG_IMPL(name, required, _default, ...) (name)(__VA_ARGS__)
#endif


/* TODO */
#define SI_ARG_LEN(...) SI__ARG_LEN_2(__VA_OPT__(,) __VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
/* TODO: move this */
#define SI__ARG_LEN_2(_, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, Count, ...) Count
/* TODO */
#define SI_VARIADIC(name, ...) SI__VARIADIC_2(name, SI_ARG_LEN(__VA_ARGS__), __VA_ARGS__)

typedef struct siAny {
	/* TODO */
	siTypeId type;
	/* TODO */
	const void* ptr;
} siAny;



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
#ifndef SI_DEFAULT_ALIGNMENT
	#define SI_DEFAULT_ALIGNMENT (2 * si_sizeof(void*))
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
SIDEF bool si_isPowerOfTwo(isize x);
/* Aligns the number to the specified alignment. */
SIDEF isize si_alignForward(isize num, isize alignment);
SIDEF usize si_alignForwardU(usize num, isize alignment);
/* Rounds up the number to the next power of 2. */
SIDEF isize si_nextPow2(isize num);


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
SIDEF bool si_pointerBetween(const void* ptr, const void* start, const void* end);


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

/* Returns the length of a NULL-terminated C-string. */
SIDEF isize si_cstrLen(cstring str);

#endif /* SI_NO_MEMORY */

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


/* Allocates the specified amount of bytes of storage. Memory _gets_ zeroed out. */
SIDEF
void* si_alloc(
	siAllocator alloc,
	isize bytes,
	siAllocationError* outError SI_DEFAULT(nil)
);
SIDEF
void* si_allocAlign(
	siAllocator alloc,
	isize bytes,
	isize align,
	siAllocationError* outError SI_DEFAULT(nil)
);

/* Allocates the specified amount of bytes of storage. Memory _doesn't_ get zeroed out. */
SIDEF
void* si_allocNonZeroed(
	siAllocator alloc,
	isize bytes,
	siAllocationError* outError SI_DEFAULT(nil)
);
SIDEF
void* si_allocNonZeroedAlign(
	siAllocator alloc, isize bytes, isize align,
	siAllocationError* outError SI_DEFAULT(nil)
);

/* Resizes specified memory block from the given old size to the new. Memory
 * _gets_ zeroed out. */
SIDEF
void* si_realloc(
	siAllocator alloc, void* ptr, isize oldSize, isize newSize,
	siAllocationError* outError SI_DEFAULT(nil)
);
SIDEF
void* si_reallocAlign(
	siAllocator alloc, void* ptr, isize oldSize, isize newSize, isize align,
	siAllocationError* outError SI_DEFAULT(nil)
);

/* Resizes the specified memory block from the given old size to the new. Memory
 * _doesn't_ get zeroed out. */
SIDEF
void* si_reallocNonZeroed(
	siAllocator alloc, void* ptr, isize oldSize, isize newSize,
	siAllocationError* outError SI_DEFAULT(nil)
);
SIDEF void* si_reallocNonZeroedAlign(
	siAllocator alloc, void* ptr, isize oldSize, isize newSize, isize align,
	siAllocationError* outError SI_DEFAULT(nil)
);


/* Frees a previously allocated memory block from 'si_alloc'. */
SIDEF siAllocationError si_free(siAllocator alloc, void* ptr);
/* Frees all of the previous allocated memory within the specified allocator. */
SIDEF siAllocationError si_freeAll(siAllocator alloc);


/* Returns the available space left inside the specified allocator. A minus one
 * is returned if the feature isn't supported. */
SIDEF isize si_allocatorMemAvailable(siAllocator alloc, siAllocationError* outError SI_DEFAULT(nil));

/* Returns a byte, where each 'siAllocationType' bit corresponds to a supporterd
 * feature. */
SIDEF u8 si_allocatorGetFeatures(siAllocator alloc);
/* Returns true if a feature is supported from the 'si_allocatorGetFeatures' byte. */
SIDEF bool si_allocatorHasFeature(u8 features, siAllocationType type);



/* allocator - siAllocator* | type - TYPE
 * Allocates an exact amount of storage to fit the specified type. Memory gets
 * zeroed out. */
#define si_allocItem(allocator, type) (type*)si_alloc(allocator, si_sizeof(type))
#define si_allocItemAlign(allocator, type, align) (type*)si_allocAlign(allocator, si_sizeof(type), align)

/* allocator - siAllocator* | type - TYPE
 * Allocates an exact amount of storage to fit the specified type. */
#define si_allocItemNonZeroed(allocator, type) (type*)si_allocNonZeroed(allocator, si_sizeof(type))
#define si_allocItemNonZeroedAlign(allocator, type, align) (type*)si_allocNonZeroedAlign(allocator, si_sizeof(type), align)

/* allocator - siAllocator | type - TYPE | count - isize
 * Allocates an exact amount of storage to fit the specified array of types.
 * Memory gets zeroed out. */
#define si_allocArray(allocator, type, count) (type*)si_alloc(allocator, si_sizeof(type) * (count))
#define si_allocArrayAlign(allocator, type, count, align) (type*)si_allocAlign(allocator, si_sizeof(type) * (count), align)

/* allocator - siAllocator | type - TYPE | count - isize
 * Allocates an exact amount of storage to fit the specified array of types. */
#define si_allocArrayNonZeroed(allocator, type, count) (type*)si_allocNonZeroed(allocator, si_sizeof(type) * (count))
#define si_allocArrayNonZeroedAlign(allocator, type, count, align) (type*)si_allocNonZeroedAlign(allocator, si_sizeof(type) * (count), align)


/* TODO */
#define si_reallocArray(allocator, type, ptr, oldCount, newCount) (type*)si_realloc(allocator, ptr, si_sizeof(type) * (oldCount), si_sizeof(type) * (newCount))
#define si_reallocArrayAlign(allocator, ptr, type, oldCount, newCount) (type*)si_reallocAlign(allocator, ptr, si_sizeof(type) * (oldCount), si_sizeof(type) * (newCount))

/* TODO */
#define si_reallocArrayNonZeroed(allocator, type, ptr, oldCount, newCount) (type*)si_reallocNonZeroed(allocator, ptr, si_sizeof(type) * (oldCount), si_sizeof(type) * (newCount))
#define si_reallocArrayNonZeroedAlign(allocator, type, ptr, oldCount, newCount) (type*)si_reallocNonZeroedAlign(allocator, ptr, si_sizeof(type) * (oldCount), si_sizeof(type) * (newCount))


/*
 * Heap allocator
 *
 *
 * Description:
 * - An allocator that utilizes 'malloc', 'realloc' and 'free' functions.
 *
 *
 * Functionality:
 * - si_alloc - 'malloc(size), 0, size'. The allocated block gets zeroed out.
 *
 * - si_allocNonZeroed - 'malloc(size)'.
 *
 * - si_realloc - 'realloc(newSize)'. The 'oldSize' argument is ignored. The newly
 *   allocated memory gets zeroed out.
 *
 * - si_reallocNonZeroed - 'realloc(newSize)'. The 'oldSize' argument is ignored.
 *
 * - si_free - 'free(ptr)'.
 *
 * - si_freeAll - UNSUPPORTED.
 *
 * - si_allocatorMemAvailable - UNSUPPORTED.
 *
 *
 * Errors:
 * - siAllocationError_OutOfMem - alloc, allocNonZeroed, realloc, reallocNonZeroed.
 * - siAllocationError_NotImplemented - freeAll, allocatorGetAvailable.
 * */
SIDEF SI_ALLOCATOR_PROC(si_allocatorHeap_proc);

/* Returns the heap allocator. */
SIDEF siAllocator si_allocatorHeap(void);

#ifndef si_stack
	#ifdef SI_LANGUAGE_IS_C
		/* bytes - usize
		 * Stack allocates the specified amount of bytes of storage. */
		#define si_stack(bytes) (void*)((u8[bytes]){0})
	#else
		#if SI_COMPILER_GCC || SI_COMPILER_CLANG
			/* bytes - usize
			 * Stack allocates the specified amount of bytes of storage. */
			#define si_stack(bytes) alloca(bytes)
		#elif SI_COMPILER_MSVC
			/* bytes - usize
			 * Stack allocates the specified amount of bytes of storage. */
			#define si_stack(bytes) alloca_malloca(bytes)
		#else
			#error "Needs an update."
		#endif
	#endif
#endif

/* TODO */
#define si_stackItem(type) (type*)si_stack(si_sizeof(type))
/* bytes - usize
 * Returns the stack allocated memory and the length. Useful when using allocators. */
#define si_stackAlloc(bytes) si_stack(bytes), (bytes)


#ifndef si_malloc
	/* bytes - isize
	 * Heap allocates the specified amount of bytes of storage. */
	#define si_malloc(bytes) si_alloc(si_allocatorHeap(), bytes)
	#define si_mallocNonZeroed(bytes) si_allocNonZeroed(si_allocatorHeap(), bytes)
	/* ptr - void* | newSize - isize
	 * Heap reallocates the specified amount of bytes of storage. */
	#define si_mrealloc(ptr, newSize) si_realloc(si_allocatorHeap(), ptr, 0, newSize)
	#define si_mreallocNonZeroed(ptr, newSize) si_reallocNonZeroed(si_allocatorHeap(), ptr, 0, newSize)
	/* ptr - void*
	 * Deallocates a previously allocated memory block from 'si_malloc'. */
	#define si_mfree(ptr) si_free(si_allocatorHeap(), ptr)

#endif

/* type - TYPE
 * Heap allocates an exact amount of storage to fit the specified type. */
#define si_mallocItem(type) (type*)si_malloc(si_sizeof(type))
#define si_mallocItemNonZeroed(type) (type*)si_mallocNonZeroed(si_sizeof(type))

/* type - TYPE | count - isize
 * Heap allocates an exact amount of storage to fit the specified array of types. */
#define si_mallocArray(type, count) (type*)si_malloc(si_sizeof(type) * (count))
#define si_mallocArrayNonZeroed(type, count) (type*)si_mallocNonZeroed(si_sizeof(type) * (count))

#if 1
/*
	========================
	| siArena              |
	========================
*/

/* Reprents an arena allocator. */
typedef struct siArena {
	siAllocator alloc;
	u8* ptr;
	isize offset;
	isize capacity;
} siArena;

/*
 * Arena allocator
 *
 *
 * Description:
 * - An allocator that allocates one large memory region and linearly assigns
 *   each allocated element to a free section of said region. In practice, if one
 *   has an arena of 128 bytes and allocates 32 bytes, the remaining 96 bytes will
 *   be available for further linear allocations.
 *
 *
 * Functionality:
 * - si_alloc - reserves the requested amount of bytes. The allocation gets zeroed
 *   out.
 *
 * - si_allocNonZeroed - reserves the requested amount of bytes.
 *   out.
 *
 * - si_realloc - reserves a new memory block and copies the old block's data
 *   into it. The newly allocated memory after the copied data gets zeroed out.
 *
 * - si_reallocNonZeroed - reserves a new memory block and copies the old block's
 *   data.
 *
 * - si_free - UNSUPPORTED.
 *
 * - si_freeAll - frees every single allocated memory block. Internal offset is
 * set to zero.
 *
 * - si_allocatorMemAvailable - returns the available memory.
 *
 *
 * Errors:
 * - siAllocationError_OutOfMem - alloc, allocNonZeroed, realloc, reallocNonZeroed.
 * - siAllocationError_NotImplemented - free.
 * */
SIDEF SI_ALLOCATOR_PROC(si_allocatorArena_proc);

/* Creates an arena allocator. */
SIDEF siArena si_arenaMake(siAllocator alloc, isize capacity);
SIDEF siArena si_arenaMakeEx(siAllocator alloc, isize capacity, isize align);
SIDEF siArena si_arenaMakePtr(void* ptr, isize capacity);

/* Returns an arena allocator procedure. */
SIDEF siAllocator si_allocatorArena(siArena* arena);

/* Destroys an arena allocator. */
SIDEF void si_arenaFree(siArena* arena);

/*
 * Temporary (arena) memory allocator
 *
 * Stores the arena's offset at that current moment (basically a savepoint),
 * denoting that the user is currently allocating temporary, very short-lived
 * memory. When the end of the temporary memory is called, the saved offset
 * gets set as the new internal offset of the allocator. */
typedef struct siArenaTmp {
	siArena* arena;
	isize offset;
} siArenaTmp;

/* Denotes the start of temporary memory. */
SIDEF siArenaTmp si_arenaTmpStart(siArena* arena);
/* Denotes the end of temporary memory. */
SIDEF void si_arenaTmpEnd(siArenaTmp tmp);

#endif

#if 1
/*
	========================
	| siLifo               |
	========================
*/

/* Reprents a LIFO stack-based allocator. */
typedef siArena siLifo;

/*
 * Stack-like LIFO (last in, first out) stack-based allocator
 *
 *
 * Description:
 * - The allocator follows stack-based principles where the last allocated element
 *   is also the first to be deallocated. Works exactly like an arena except an
 *   additional size-length offset is requested for each allocation. For example,
 *   an 8-byte allocation becomes a 16-byte one in memory due to the saved offset.
 * - Memory layout: | 32/64-bit offset | memory data |
 *
 *
 * How freeing works:
 * - A very important part to note is that the offsets are *not* relative, meaning
 *   you *are* able to free any pointer. What happens is that the allocator will
 *   free everything _before_ the offset.
 *
 * - As an example, here is an array from 1 to 6:
 *   {1, 2, 3, 4, 5, 6}
 * - If you decide to free the pointer for the 3rd elemnt, the end-result will be:
 *   {1, 2}
 * - The pointer and everything after it got freed. However if the 6th element was
 * freed, then only it would be freed.
 *
 *
 * Functionality:
 * - si_alloc - reserves the requested amount of bytes. The allocation gets zeroed
 * out.
 *
 * - si_allocNonZeroed - reserves the requested amount of bytes.
 *   out.
 *
 * - si_realloc - reserves a new memory block and copies the old block's data
 * into it. The newly allocated memory after the copied data gets zeroed out.
 *
 * - si_reallocNonZeroed - reserves a new memory block and copies the old block's
 * data.
 *
 * - si_free - frees the pointer and everything after it.
 *
 * - si_freeAll - frees every single allocated memory block. Internal offset is
 * set to zero.
 *
 * - si_allocatorMemAvailable - returns the available memory.
 *
 *
 * Errors:
 * - siAllocationError_OutOfMem - alloc, allocNonZeroed, realloc, reallocNonZeroed.
 * - siAllocationError_InvalidPtr - free. */
SIDEF SI_ALLOCATOR_PROC(si_allocatorLifo_proc);

/* Creates a LIFO allocator. */
SIDEF siLifo si_lifoMake(siAllocator alloc, isize capacity);
SIDEF siLifo si_lifoMakeEx(siAllocator alloc, isize capacity, isize align);
SIDEF siLifo si_lifoMakePtr(void* ptr, isize capacity);

/* Returns a LIFO allocator procedure. */
SIDEF siAllocator si_allocatorLifo(siLifo* lifo);

/* Destroy a LIFO allocator. */
SIDEF void si_lifoFree(siLifo* lifo);

#endif

#if 1
/*
	========================
	| siPool               |
	========================
*/

typedef struct siPoolFreeNode {
	struct siPoolFreeNode* next;
} siPoolFreeNode;

typedef struct siPool {
	siAllocator alloc;
	u8* ptr;
	isize numChunks;
	isize chunkSize;

	siPoolFreeNode* head;
} siPool;

/*
 * Pool allocator
 *
 *
 * Description:
 * - The allocator that allocates fixed-size blocks of memory from a pre-allocated
 * pool. Useful for frequent allocations and deallocations of fixed-sized objects.
 *
 *
 * Each allocation's alignment:
 * - The allocator will always silently ignore any user-specified alignment options
 *   and assume that each allocation is aligned to the pool allocator's alignment
 *   value. As an example, requesting an 8-byte aligned allocation of 10 bytes
 *   will not change the underlined size from 10 to 16 - the allocator will only
 *   simply check if the requested size of 8 bytes is less than the aligned block
 *   size.
 *
 *
 *
 * Functionality:
 * - si_alloc - reserves the block size amount of bytes. The allocation gets zeroed
 *   out. If the requested size is larger than the block size, the function fails.
 *
 * - si_allocNonZeroed - reserves the requested amount of bytes.
 *   out. If the requested size is larger than the block size, the function fails.
 *
 * - si_realloc - UNSUPPORTED.
 *
 * - si_reallocNonZeroed - UNSUPPORTED.
 *
 * - si_free - frees the specified block.
 *
 * - si_freeAll - frees every single block.
 *
 * - si_allocatorMemAvailable - returns the block size if there is an available
 * block, otherwise 0 is returned.
 *
 *
 * Errors:
 * - siAllocationError_InvalidArg - alloc, allocNonZeroed.
 * - siAllocationError_OutOfMem - alloc, allocNonZeroed.
 * - siAllocationError_InvalidPtr - free.
 * - siAllocationError_NotImplemented - realloc, reallocNonZeroed. */
SIDEF SI_ALLOCATOR_PROC(si_allocatorPool_proc);

/* Creates a pool allocator. */
SIDEF siPool si_poolMake(siAllocator alloc, isize numChunks, isize chunkSize);
SIDEF siPool si_poolMakeEx(siAllocator alloc, isize numChunks, isize chunkSize,
		isize align);

/* Returns a pool allocator procedure. */
SIDEF siAllocator si_allocatorPool(siPool* pool);

/* Destroys a pool allocator. */
SIDEF void si_poolFree(siPool* pool);

#endif

#if 1
/*
	========================
	| siDynamicArena       |
	========================
*/

typedef struct siDynamicArenaBlock {
	u8* ptr;
	isize offset;
	struct siDynamicArenaBlock* next;
} siDynamicArenaBlock;

typedef struct siDynamicArena {
	siArena arena;
	isize blockSize;
	siDynamicArenaBlock* head;
} siDynamicArena;


/*
 * Dynamic arena allocator
 *
 *
 * Description:
 * - An arena allocator that automatically grows when the starting capacity is
 *   about to get surpassed. This is done by allocating another memory block
 *   from anothr allocator and returning it instead of returning a no memory error.
 *   Internally this is achieved via a single linked-list.
 *
 * - Using a dynamic allocator with a heap allocator as the backend results in
 *   an allocator that will likely never run out of memory or return any errors
 *   unless the requested size is larger than the block size.
 *
 *
 * Functionality:
 * - si_alloc - reserves the block size amount of bytes. The allocation gets zeroed
 *   out. Allocates a new block if the requested size overflows the current capacity.
 *   If the requested size is larger than the block size, the function fails.
 *
 * - si_allocNonZeroed - reserves the requested amount of bytes. Allocates a new
 *   block if the requested size overflows the current capacity. If the requested
 *   size is larger than the block size, the function fails.
*
 * - si_realloc - same as arena's 'si_realloc'.
 *
 * - si_reallocNonZeroed - same as arena's 'si_reallocNonZeroed'.
 *
 * - si_free - UNSUPPORTED
 *
 * - si_freeAll - frees the allocator alongside the blocks.
 *
 * - si_allocatorMemAvailable - returns the available space of the allocator if
 * it's higher than the block size, otherwise the block size gets returned.
 *
 *
 * Errors:
 * - siAllocationError_InvalidArg - alloc, allocNonZeroed, realloc, reallocNonZeroed.
 * - siAllocationError_OutOfMem - alloc, allocNonZeroed, realloc, reallocNonZeroed.
 * - siAllocationError_NotImplemented - free. */
SIDEF SI_ALLOCATOR_PROC(si_allocatorDynamicArena_proc);

/* Creates a dynamic arena allocator. */
SIDEF siDynamicArena si_dynamicArenaMake(siAllocator alloc, isize startingCapacity,
		isize blockSize);
SIDEF siDynamicArena si_dynamicArenaMakeEx(siAllocator alloc, isize startingCapacity,
		isize blockSize, isize align);

/* Returns a dynamic arena allocator procedure. */
SIDEF siAllocator si_allocatorDynamicArena(siDynamicArena* dynamic);

/* Destroys a dynamic arena allocator. */
SIDEF void si_dynamicArenaFree(siDynamicArena* dynamic);

/*
 * Temporary (dynamic arena) memory allocator
 *
 * Stores the arena's offset and the last memory block at that current moment
 * (basically a savepoint), denoting that the user is currently allocating temporary,
 * very short-lived memory. When the end of the temporary memory is called, the
 * saved offset gets set as the new internal offset of the allocator, while every
 * allocated memory block since the start get "freed" (their internal offset is
 * set to zero). */
typedef struct siDynamicArenaTmp {
	siArenaTmp aTmp;
	isize blockOffset;
	siDynamicArenaBlock* block;
} siDynamicArenaTmp;

/* Denotes the start of temporary memory. */
SIDEF siDynamicArenaTmp si_dynamicArenaTmpStart(siDynamicArena* dynamic);
/* Denotes the end of temporary memory. */
SIDEF void si_dynamicArenaTmpEnd(siDynamicArenaTmp tmp);


#endif

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
	| siArray              |
	========================
*/


/* name - NAME | buffer - siArrayAny
 * Loops through the elements of the buffer, writes the element to 'name'. */
#define for_eachArr(name, buffer) for_eachArrEx(name, si__i, buffer)
#define for_eachArrEx(name, indexName, buffer) \
	for (isize indexName = 0; si__forEachBuf(indexName, (siArrayAny*)&buffer, &(name)); indexName += 1)

/* name - NAME | buffer - siArrayAny
 * Loops through the elements of the buffer, writes the element pointer to 'name'. */
#define for_eachRefArr(name, buffer) for_eachRefArrEx(name, si__i, buffer)
#define for_eachRefArrEx(name, indexName, buffer) \
	for (isize indexName = 0; si__forEachRefBuf(indexName, buffer, &(name)); indexName += 1, name += 1)


/* name - NAME | buffer - siArrayAny
 * Loops through the elements of the buffer, writes the element to 'name'. */
#define for_eachRevArr(name, buffer) for_eachRevArrEx(name, si__i, buffer)
#define for_eachRevArrEx(name, indexName, buffer) \
	for (isize indexName = (buffer).len - 1; si__forEachRevBuf(indexName, buffer, &(name)); indexName -= 1)

/* name - NAME | buffer - siArrayAny
 * Loops through the elements of the buffer in reverse, writes the element pointer
 * to 'name'. */
#define for_eachRevRefArr(name, buffer) for_eachRevRefArrEx(name, si__i, buffer)
#define for_eachRevRefArrEx(name, indexName, buffer) \
	for (isize indexName = (buffer).len - 1; si__forEachRevRefBuf(indexName, buffer, &(name)); indexName -= 1)



/* type - TYPE | ...values - ANYTHING
 * Makes an array on the stack from the values. */
#define SI_ARR(type, .../* values */) SI__ARR_IMPL(type, __VA_ARGS__)
/* ptr - void* | len - isize
 * Makes an array on the stack from the specified pointer and its size, length. */
#define SI_ARR_LEN(ptr, len) SI_ARR_EX(ptr, len, si_sizeof(*(ptr)))
/* TODO */
#define SI_ARR_BUF(buffer) SI_ARR_LEN(buffer, si_sizeof(buffer))
/* ptr - void* | len - isize | type - TYPE
 * Makes an array on the stack from the specified pointer, type and length. */
#define SI_ARR_TYPE(ptr, len, type) SI_ARR_EX(ptr, len, si_sizeof(type))
/* ptr - void* | len - isize | typeSize | isize
 * Makes an array on the stack from the specified pointer, type size and length. */
#define SI_ARR_EX(ptr, len, typeSize) SI_COMP_LIT(siArrayAny, len, ptr, typeSize)

/* bytes - usize
 * Stack allocates the specified amount of bytes and creates a siArray from it. */
#define SI_ARR_STACK(bytes) SI_ARR_STACK_EX(u8, bytes)
/* type - TYPE | len - usize
 * Stack allocates the specified amount of typed-elements and creates a siArray from it. */
#define SI_ARR_STACK_EX(type, len) SI_ARR_LEN((type*)si_stack(len * sizeof(type)), len)
/* TODO */
#define SI_ARR_EMPTY(type) SI_ARR_LEN((type*)nil, 0)


/* alloc - siAllocator | type - TYPE | ...values - ANYTHING
* Allocates an array and copies the specified values to it. */
#define si_arrayMake(alloc, type, .../* values */) \
	SI__ARR_ALLOC_IMPL(type, alloc, __VA_ARGS__)
/* type - TYPE | len - isize | alloc - siAllocator
 * Allocates an empty, zeroed array with a specified length. */
#define si_arrayMakeReserve(type, len, alloc) \
	SI_ARR_TYPE(si_allocArray(alloc, type, len), len, type)
/* type - TYPE | len - isize | alloc - siAllocator
 * Allocates an empty, non-zeroed array with a specified length. */
#define si_arrayMakeReserveNonZeroed(type, len, alloc) \
	SI_ARR_TYPE(si_allocArrayNonZeroed(alloc, type, len), len, type)

/* Returns a reference to the array from the specified start and end offsets.
 * eg. si_slice(SI_ARR(i32, 1, 2, 4, 16, 32, 64), 1, 4) -> {2, 4, 16} */
SIDEF siArrayAny si_slice(siArrayAny array, isize offset1, isize offset2);
/* Returns a slice from the specified start to the end.
 * Equivalent to si_slice(arr, offset1, arr.len). */
SIDEF siArrayAny si_sliceFrom(siArrayAny array, isize offset1);
/* Returns a slice from the first element to the specified end.
 * Equivalent to si_slice(arr, 0, offset2). */
SIDEF siArrayAny si_sliceTo(siArrayAny array, isize offset2);
/* Returns a slice with a specified start and length.
 * Equivalent to si_slice(arr, offset1, offset1 + len). */
SIDEF siArrayAny si_sliceLen(siArrayAny array, isize offset1, isize len);


/* Returns a pointer to the specified index. */
SIDEF void* si_arrayGet(siArrayAny array, isize index);
/* Returns a pointer to the first element (&array[0]) in the array. */
SIDEF void* si_arrayFront(siArrayAny array);
/* Returns a pointer to the past-the-end element (&array[array.len]) in the array. */
SIDEF void* si_arrayBack(siArrayAny array);
/* Sets an element to the specified pointer (bound or type checking isn't done). */
SIDEF void* si_arraySet(
	siArrayAny array, isize index, const void* data, 
	isize count SI_DEFAULT(1)
);

/* Copies an element at the specified index into the given pointer and returns
 * true if the index is less than the array's length. Otherwise, 'false' is returned. */
SIDEF bool si_arrayAtGet(siArrayAny array, isize index, void* out);
/* Copies the first element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF bool si_arrayAtFront(siArrayAny array, void* out);
/* Copies the last element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF bool si_arrayAtBack(siArrayAny array, void* out);

/* Searches for the specified pointer's content in the array (from the beginning).
 * If found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_arrayFind(siArrayAny array, const void* data);
/* Searches for the specified pointer's content in the array (from the end). If
 * found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_arrayFindLast(siArrayAny array, const void* data);
/* Returns the amount of times the specified pointer's value is repeated in the array. */
SIDEF isize si_arrayFindCount(siArrayAny array, const void* data);

/* Returns true if the two specified arrays are equal in length and contents. */
SIDEF bool si_arrayEqual(siArrayAny lhs, siArrayAny rhs);

/* Frees the allocated array. */
SIDEF void si_arrayFree(siArrayAny array, siAllocator alloc);


/* array - siArrayAny | index - isize | value - ANYTHING | type - TYPE
 * Sets an element to the given value (bound or type checking isn't done). */
#define si_arraySetItem(array, index, value, type) (type*)si_arraySet(array, index, SI_PTR(type, value))
/* array - siArrayAny | index - isize | type - TYPE | ...values - ANYTHING
 * Sets a list of elements to the given array (bound or type checking isn't done). */
#define si_arraySetArray(array, index, type, .../* values */) (type*)si_arraySet(array, index, SI_PTR_WITH_LEN(type, __VA_ARGS__))

/* array - siArrayAny | value - ANYTHING | type - TYPE
 * Searches for the given value in the array (from the beginning). If found, the
 * value's array index is returned, otherwise '-1' is returned. */
#define si_arrayFindItem(array, value, type) si_arrayFind(array, SI_PTR(type, value))
/* array - siArrayAny | value - ANYTHING | type - TYPE
 * Searches for the given value in the array (from the end). If found, the value's
 * array index is returned, otherwise '-1' is returned. */
#define si_arrayFindLastItem(array, value, type) si_arrayFindLast(array, SI_PTR(type, value))
/* array - siArrayAny | value - ANYTHING | type - TYPE
 * Returns the amount of times the specified value is repeated in the array. */
#define si_arrayFindCountItem(array, value, type) si_arrayFindCount(array, SI_PTR(type, value))


/* TODO(EimaMei): Move these to the memory section. */
/* Copies a number of bytes from the source buffer into the destination. The number
* of copied bytes is guaranteed to be less or equal to the destination's size.
* The memory blocks cannot overlap each other. Returns the amount of bytes that
* were copied. */
SIDEF isize si_memcopy_s(siArrayAny dst, const void* src, isize sizeSrc);
/* Copies a number of bytes from the source buffer into the destination. The number
* of copied bytes is guaranteed to be less or equal to the destination's size.
* The memory blocks can overlap each other. Returns the amount of bytes that
* were moved. */
SIDEF isize si_memmove_s(siArrayAny dst, const void* src, isize sizeSrc);


/* array - siDynamicArrayAny
 * Makes a dynamic array structure out of a regular array. */
#define SI_ARR_DYN(array) SI_ARR_EX((array).data, (array).len, (array).typeSize)

#ifndef SI_DYNAMIC_ARRAY_NEW_CAP
	/* array - siDynamicArrayAny | addLen - isize
	 * Formula used to calculate the new length when memory is needed and 'grow'
	 * isn't specified. 'addlen' is the exact amount of bytes that is required. */
	#define SI_DYNAMIC_ARRAY_NEW_CAP(array, addLen) (2 * ((array)->capacity + addLen))
#endif


/* alloc - siAllocator | type - TYPE | ...values - VARIADIC
 * Allocates an array from the specified type and static items. */
#define si_dynamicArrayMake(alloc, type, .../* values */) \
	SI__DYNARR_ALLOC_IMPL(type, alloc, __VA_ARGS__)
/* type - TYPE | capacity - isize | alloc - siAllocator
 * Allocates an empty, zeroed dynamic array with a specified length and capacity. */
#define si_dynamicArrayMakeReserve(type, capacity, alloc) \
	si_dynamicArrayReserve(si_sizeof(type), capacity, alloc)
 /* type - TYPE | capacity - isize | alloc - siAllocator
 * Allocates an empty, non-zeroed array with a specified length and capacity. */
#define si_dynamicArrayMakeReserveNonZeroed(type, capacity, alloc) \
	si_dynamicArrayReserveNonZeroed(si_sizeof(type), capacity, alloc)

/* Allocates 'count * typeSize' amount of bytes to make an array and copies the
 * same amount of bytes from the specified list to it, whilst setting the length
 * to 'count'. */
SIDEF siDynamicArrayAny si_dynamicArrayMakeEx(void* list, isize typeSize, isize count,
		siAllocator alloc);
/* Allocates 'capacity * typeSize' amount of bytes to make an array, whilst
 * setting the length to the specified size. */
SIDEF siDynamicArrayAny si_dynamicArrayReserve(isize typeSize, isize capacity,
		siAllocator alloc);
SIDEF siDynamicArrayAny si_dynamicArrayReserveNonZeroed(isize typeSize, isize capacity,
		siAllocator alloc);
/* Creates a new dynamic array from a regular array. */
SIDEF siDynamicArrayAny si_dynamicArrayCopy(siArrayAny array, siAllocator alloc);

/* Frees the allocated array. */
SIDEF void si_dynamicArrayFree(siDynamicArrayAny array);


/* Returns a pointer to the specified element. */
SIDEF void* si_dynamicArrayGet(siDynamicArrayAny array, isize index);
/* Returns a pointer to the first element (&array[0]) in the array. */
SIDEF void* si_dynamicArrayFront(siDynamicArrayAny array);
/* Returns a pointer to the past-the-end element (&array[array.len]) in the array. */
SIDEF void* si_dynamicArrayBack(siDynamicArrayAny array);
/* Sets a list of elements to the specified pointer (bound or type checking isn't done). */
SIDEF void* si_dynamicArraySet(
	siDynamicArrayAny array, isize index, const void* data,
	isize count SI_DEFAULT(1)
);

/* Copies an element at the specified index into the given pointer and returns
 * true if the index is less than the array's length. Otherwise, 'false' is returned. */
SIDEF bool si_dynamicArrayAtGet(siDynamicArrayAny array, isize index, void* out);
/* Copies the first element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF bool si_dynamicArrayAtFront(siDynamicArrayAny array, void* out);
/* Copies the last element of the array into the given pointer and returns true
 * if the array's length isn't zero. Otherwise, 'false' is returned. */
SIDEF bool si_dynamicArrayAtBack(siDynamicArrayAny array, void* out);

/* Searches for the specified pointer's content in the array (from the beginning).
 * If found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_dynamicArrayFind(siDynamicArrayAny array, const void* data);
/* Searches for the specified pointer's content in the array (from the end). If
 * found, the value's array index is returned, otherwise '-1' is returned. */
SIDEF isize si_dynamicArrayFindLast(siDynamicArrayAny array, const void* data);
/* Returns the amount of times the specified pointer's value repeats in the array. */
SIDEF isize si_dynamicArrayFindCount(siDynamicArrayAny array, const void* data);

/* Returns true if the two specified arrays are equal in length and contents. */
SIDEF bool si_dynamicArrayEqual(siDynamicArrayAny lhs, siDynamicArrayAny rhs);


/* Appends the specified pointer's value to the array and returns the element's pointer. */
SIDEF void* si_dynamicArrayAppend(
	siDynamicArrayAny* array, const void* data,
	isize count SI_DEFAULT(1)
);
/* Erases the last item in the array. */
SIDEF void si_dynamicArrayPop(siDynamicArrayAny* array);
/* Sets the array's length to zero. */
SIDEF void si_dynamicArrayClear(siDynamicArrayAny* array);

/* Inserts the specified pointer's value at the given index of the array. Returns
 * true if the array was reallocated. */
SIDEF bool si_dynamicArrayInsert(
	siDynamicArrayAny* array, isize index, const void* data,
	isize count SI_DEFAULT(1)
);
/* Erases the specified pointer's value at the given index of the array. */
SIDEF void si_dynamicArrayErase(
	siDynamicArrayAny* array, isize index, isize count
);

/* Reverses the contents of the array. */
SIDEF void si_dynamicArrayReverse(siDynamicArrayAny array);
/* Fills the contents of the array with the specified pointer's value. Returns
 * true if the array was reallocated. */
SIDEF bool si_dynamicArrayFill(siDynamicArrayAny* array, isize index, isize count,
	const void* data);

/* Replaces all occurences of the firstly specified pointer's value with the
 * secondly specified one. */
SIDEF void si_dynamicArrayReplaceAll(siDynamicArrayAny array, void* restrict valueOld,
		void* restrict valueNew);
/* Replaces a given amount of the firstly specified pointer's value with the
 * secondly specified one. */
SIDEF void si_dynamicArrayReplace(siDynamicArrayAny array, void* restrict valueOld,
		void* restrict valueNew, isize amount);

/* If needed, reallocates the array for the added space. Returns true if the
 * array was reallocated. Used internally. */
SIDEF bool si_dynamicArrayMakeSpaceFor(siDynamicArrayAny* array, isize addLen);



/* array - siArrayAny | index - isize | value - ANYTHING | type - TYPE
 * Sets an element to the given value (bound or type checking isn't done). */
#define si_dynamicArraySetItem(array, index, value, type) (type*)si_dynamicArraySet(array, index, SI_PTR(type, value))
/* array - siArrayAny | index - isize | type - TYPE | ...values - ANYTHING
 * Sets a list of elements to the given array (bound or type checking isn't done). */
#define si_dynamicArraySetArray(array, index, type, ...) (type*)si_dynamicArraySetEx(array, index, SI_PTR_WITH_LEN(type, __VA_ARGS__))

/* array - siArrayAny | value - ANYTHING | type - TYPE
 * Searches for the given value in the array (from the beginning).
 * If found, the value's array index is returned, otherwise '-1' is returned. */
#define si_dynamicArrayFindItem(array, value, type) si_dynamicArrayFind(array, SI_PTR(type, value))
/* array - siArrayAny | value - ANYTHING | type - TYPE
 * Searches for the given value in the array (from the end).
 * If found, the value's array index is returned, otherwise '-1' is returned. */
#define si_dynamicArrayFindLastItem(array, value, type) si_dynamicArrayFindLast(array, SI_PTR(type, value))
/* array - siArrayAny | value - ANYTHING | type - TYPE
 * Returns the amount of times the specified value is repeated in the array. */
#define si_dynamicArrayFindCountItem(array, value, type) si_dynamicArrayFindCount(array, SI_PTR(type, value))

/* array - siArrayAny | value - ANYTHING | type - TYPE
 * Appends the given value to the array and returns the element's pointer. */
#define si_dynamicArrayAppendItem(array, value, type) (type*)si_dynamicArrayAppend(array, SI_PTR(type, value))
/* array - siArrayAny | value - ANYTHING | type - TYPE
 * Appends the specified list of items to the array and returns the elements' pointer. */
#define si_dynamicArrayAppendArray(array, type, ...) (type*)si_dynamicArrayAppendEx(array, SI_PTR_WITH_LEN(type, __VA_ARGS__))

/* array - siArrayAny | index - isize | value - ANYTHING | type - TYPE
 * Inserts the given value at the given index of the array. Returns true if the
 * array was reallocated. */
#define si_dynamicArrayInsertItem(array, index, value, type) si_dynamicArrayInsert(array, index, SI_PTR(type, value))
/* array - siArrayAny | index - isize | value - ANYTHING | type - TYPE
 * Inserts the specified list of items at the given index of the array. Returns
 * true if the array was reallocated. */
#define si_dynamicArrayInsertArray(array, index, type, ...) si_dynamicArrayInsertEx(array, index, SI_PTR_WITH_LEN(type, __VA_ARGS__))

/* array - siArrayAny | index, count - isize | value - ANYTHING | type - TYPE
 * Fills the contents of the array with the given value. Returns true if the array
 * was reallocated. */
#define si_dynamicArrayFillItem(array, index, count, value, type) si_dynamicArrayFill(array, index, count, SI_PTR(type, value))

/* array - siArrayAny | valueOld, valueNew - ANYTHING | type - TYPE
 * Replaces all occurences of the given old value with the specified new one. */
#define si_dynamicArrayReplaceAllItem(array, valueOld, valueNew, type) si_dynamicArrayReplaceAll(array, SI_PTR(type, valueOld), SI_PTR(type, valueNew))
/* array - siArrayAny | valueOld, valueNew - ANYTHING | amount - isize | type - TYPE
 * Replaces a given amount of the given old value with the specified new one. */
#define si_dynamicArrayReplaceItem(array, valueOld, valueNew, amount, type) si_dynamicArrayReplace(array, SI_PTR(type, valueOld), SI_PTR(type, valueNew), amount)



#endif /* SI_NO_ARRAY */

#ifndef SI_NO_STREAM

SI_ENUM(i32, siStreamError) {
	/* TODO */
	siStreamError_None = 0,

	/* TODO */
	siStreamError_EOF,
	/* TODO */
	siStreamError_ShortWrite,
	/* TODO */
	siStreamError_NegativeWrite,
	/* TODO */
	siStreamError_InvalidOffset,
	/* TODO */
	siStreamError_BufferFull,

	/* TODO */
	siStreamError_Unknown,
	/* TODO */
	siStreamError_Unimplemented
};

SI_ENUM(i32, siStreamType) {
	siStreamType_Read,
	siStreamType_ReadAt,

	siStreamType_Write,
	siStreamType_WriteAt,

	siStreamType_Seek,
	siStreamType_Size,
	siStreamType_Flush,

	siStreamType_Close,
	siStreamType_Destroy,

	siStreamType_GetFeatures,

	siStreamType_Len,
};

/* TODO */
SI_BITMASK_TYPE(u16, siStreamType, siStreamType_Flags);

SI_ENUM(i32, siSeekWhere) {
	/* Sets the pointer from the beginning of the file. */
	siSeekWhere_Begin = 0,
	/* Sets the pointer from the current offset. */
	siSeekWhere_Current = 1,
	/* Sets the pointer from the EOF offset. */
	siSeekWhere_End = 2
};

/* TODO */
#define SI_STREAM_PROC(name) \
	isize name(siStreamType type, siArray(u8) ptr, isize offset, siSeekWhere seek, siStreamError* outError, void* streamData)
/* Represents a stream procedure. */
typedef SI_STREAM_PROC(siStreamProc);

/* Represents a stream type. */
typedef struct siStream {
	siStreamProc* proc;
	void* data;
} siStream;

/* TODO */
typedef siStream siWriter;
/* TODO */
typedef siStream siReader;


/* TODO */
SIDEF isize si_streamRead(siStream stream, siArray(u8) out, siStreamError* outError SI_DEFAULT(nil));
/* TODO */
SIDEF isize si_streamReadAt(siStream stream, isize offset, siArray(u8) out, siStreamError* outError SI_DEFAULT(nil));

/* TODO */
SIDEF isize si_streamWrite(siStream stream, siArray(u8) ptr, siStreamError* outError SI_DEFAULT(nil));
SIDEF isize si_streamWriteStr(siStream stream, siString str, siStreamError* outError SI_DEFAULT(nil));
SIDEF isize si_streamWriteByte(siStream stream, u8 byte, siStreamError* outError SI_DEFAULT(nil));
SIDEF isize si_streamWriteInt(siStream stream, i64 num, siStreamError* outError SI_DEFAULT(nil));

/* TODO */
SIDEF isize si_streamWriteAt(siStream stream, isize offset, siArray(u8) ptr, siStreamError* outError SI_DEFAULT(nil));
SIDEF isize si_streamWriteAtStr(siStream stream, isize offset, siString str, siStreamError* outError SI_DEFAULT(nil));
SIDEF isize si_streamWriteAtByte(siStream stream, isize offset, u8 byte, siStreamError* outError SI_DEFAULT(nil));
SIDEF isize si_streamWriteAtInt(siStream stream, isize offset, i64 num, siStreamError* outError SI_DEFAULT(nil));

/* TODO */
SIDEF isize si_streamSeek(
	siStream stream, isize offset, siSeekWhere whence,
	siStreamError* outError SI_DEFAULT(nil)
);
/* TODO */
SIDEF isize si_streamSize(siStream stream, siStreamError* outError SI_DEFAULT(nil));
/* TODO */
SIDEF siStreamError si_streamFlush(siStream stream);



typedef struct siIOWriter {
	siWriter stream;
	u8* data;
	isize len;
	isize capacity;
	siAllocator alloc;
} siIOWriter;

/* TODO */
SIDEF SI_STREAM_PROC(si_streamWriter_proc);


/* TODO */
SIDEF siIOWriter si_ioWriterMakeAlloc(
	siStream stream, siAllocator alloc,
	isize capacity SI_DEFAULT(SI_KILO(4)),
	isize align SI_DEFAULT(SI_DEFAULT_ALIGNMENT)
);
/* TODO */
SIDEF siIOWriter si_ioWriterMakePtr(siStream stream, void* ptr, isize capacity);
/* TODO */
SIDEF siIOWriter si_ioWriterMakeArray(siStream stream, siArray(u8) array);

/* TODO */
SIDEF siStream si_streamWriter(siIOWriter* writer);

#endif /* SI_NO_STREAM */

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


/* rune - NAME | str - siString
 * Loops through the runes of the string, writes the rune to 'name'. */
#define for_eachStr(rune, str) for_eachStrEx(rune, si__i, str)
#define for_eachStrEx(rune, indexName, str) \
	for (isize indexName = 0, indexName##len; si__forEachStr(indexName, str, &(rune), &indexName##len); indexName += indexName##len)

/* rune - NAME | str - siString
 * Loops through the runes of the string in reverse, writes the rune to 'name'. */
#define for_eachRevStr(rune, str) for_eachRevStrEx(rune, si__i, str)
#define for_eachRevStrEx(rune, indexName, str) \
	for (isize indexName = (str).len; si__forEachRevStr(str, &(rune), &(indexName)); )

/* TODO(EimaMei): This isn't great, but so is _Generic to be fair. Move this 
 * and make it much more nicer and faster. Same with the C++ version. */
SIDEF siString SI_STR2(siString x);
SIDEF siString SI_STR2_ptr(const siString* x);
SIDEF siString SI_CSTR2(const char* x);

#if SI_LANGUAGE_IS_C
/* TODO */
#define SI_STR(str) (_Generic((str), \
	siString : SI_STR2, \
	siString* : SI_STR2_ptr, \
	const siString* : SI_STR2_ptr, \
	char* : SI_CSTR2, \
	const char* : SI_CSTR2 \
)(str))

#else
extern "C++" {
force_inline siString SI_STR(const siString& str) { return SI_STR2(str); }
force_inline siString SI_STR(siString* str)       { return SI_STR2_ptr(str); }
force_inline siString SI_STR(const siString* str) { return SI_STR2_ptr(const_cast<siString*>(str)); }
force_inline siString SI_STR(char* str)           { return SI_CSTR2(str); }
force_inline siString SI_STR(const char* str)     { return SI_CSTR2(str); }
}
#endif


/* str - STR | len - isize
 * Makes a string from a length-specified C-string. */
#define SI_STR_LEN(str, len) SI_COMP_LIT(siString, len, (const u8*)str)

/* str - char*
 * Creates a string from a NULL-terminated C-string. */
#define SI_CSTR(str) SI_STR_LEN(str, si_cstrLen(str))
/* str - cstring
 * Creates a compile-time constant string from a static NULL-terminated C-string. */
#define SI_STRC(str) {countof_str(str), (u8*)str}

/* Returns an empty string. Difference between this and 'SI_STR_NIL' is that the
 * former str is still valid whilst the later denotes an error. */
#define SI_STR_EMPTY SI_STR_LEN("", 0)
#define SI_STRC_EMPTY {0, (const u8*)""}
/* Returns a nil string. */
#define SI_STR_NIL SI_STR_LEN(nil, 0)
#define SI_STRC_NIL {0, (const u8*)nil}


/* Allocates a duplicated string. */
SIDEF siString si_stringCopy(siString from, siAllocator alloc);

/* Allocates a string from a given null-terminated C-string. */
SIDEF siString si_stringFromCStr(cstring from, siAllocator alloc);
/* Allocates a NULL-terminated C-string from a string. */
SIDEF char* si_stringToCStr(siString from, siAllocator alloc);
/* Allocates a NULL-terminated C-string with a specified capacity from a string. */
SIDEF char* si_stringToCStrEx(siString from, isize capacity, siAllocator alloc);


/* Gets the string's first character. */
SIDEF siRune si_stringAtFront(siString str);
/* Gets the string's last character. */
SIDEF siRune si_stringAtBack(siString str);
/* Gets a pointer to the string's first character. */
SIDEF const u8* si_stringBegin(siString str);
/* Gets pointer to the string's past-the-end element. */
SIDEF const u8* si_stringEnd(siString str);


/* Returns a reference to the string from the specified start and end offsets.
 * eg. si_substr(SI_STR("Hello"), 1, 4) -> "ell" */
SIDEF siString si_substr(siString str, isize offset1, isize offset2);
/* Returns a substring from the specified start to the end.
 * Equivalent to si_substr(arr, offset1, str.len). */
SIDEF siString si_substrFrom(siString str, isize offset1);
/* Returns a substring from the first element to the specified end.
 * Equivalent to si_substr(arr, 0, offset2). */
SIDEF siString si_substrTo(siString str, isize offset2);
/* Returns a substring with a specified start and length.
 * Equivalent to si_substr(arr, offset1, offset1 + len). */
SIDEF siString si_substrLen(siString str, isize offset1, isize len);


/* Searches for the specified substring in the string (from the beginning). If
 * found, the value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFind(siString str, siString subStr);
/* Searches for the specified byte in the string (from the beginning). If found,
 * the value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFindByte(siString str, u8 byte);
/* Searches for the specified rune in the string from the beginning. Returns the
 * index where the rune is, other '-1' if not found. */
SIDEF isize si_stringFindRune(siString str, siRune rune);

/* Searches for the specified substring in the string (from the end). If found,
 * the value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFindLast(siString str, siString subStr);
/* Searches for the specified byte in the string (from the end). If found, the
 * value's string index is returned, otherwise '-1' is returned. */
SIDEF isize si_stringFindLastByte(siString str, u8 byte);
/* Searches for the specified rune in the string from the end. Returns the
 * index where the rune is, other '-1' if not found. */
SIDEF isize si_stringFindLastRune(siString str, siRune rune);

/* Returns the total occurences of the specified substring in the main string. */
SIDEF isize si_stringFindCount(siString str, siString subStr);


/* Compares two strings and returns true if they're equal. */
SIDEF bool si_stringEqual(siString lhs, siString rhs);
/* Compares two strings. Returns either: '0' if both strings are equal; a negative
 * value if the largest differentiating byte is 'lhs'; a positive number otherwise. */
SIDEF i32 si_stringCompare(siString lhs, siString rhs);


/* Cuts every front and back occurences of the provided cut set in the string. */
SIDEF siString si_stringTrim(siString str, siString cutSet);
/* Cuts every front occurences of the provided cut set in the string. */
SIDEF siString si_stringTrimLeft(siString str, siString cutSet);
/* Cuts back occurences of the provided cut set in the string. */
SIDEF siString si_stringTrimRight(siString str, siString cutSet);
/* Removes any leading or trailing spaces and newlines in the string. */
SIDEF siString si_stringStrip(siString str);
/* Removes any leading spaces and newlines in the string. */
SIDEF siString si_stringStripLeft(siString str);
/* Removes any trailing spaces and newlines in the string. */
SIDEF siString si_stringStripRight(siString str);
/* Removes any trailing or leading double quote. */
SIDEF siString si_stringUnquote(siString str);


/* Joins all specified strings into one and writes it into an allocated string. */
SIDEF siString si_stringJoin(siArray(siString) strings, siString separator,
		siAllocator alloc);

/* Allocates a new, big-enough string to insert a substring into the primary
 * string at the specified index. */
SIDEF siString si_stringInsert(siString str, siString subStr, isize index,
		siAllocator alloc);

/* Allocates a string, which has all mentions of the substring removed from the
 * primary string. */
SIDEF siString si_stringRemoveAll(siString str, siString subStr, siAllocator alloc);
/* Allocates a string, which has a specified amount of mentions of the substring
 * removed from the primary string. */
siString si_stringRemove(siString str, siString subStr, i32 amount, siAllocator alloc);

/* Allocates a string, which has all mentions of the old substring replaced with
 * the new one. */
SIDEF siString si_stringReplaceAll(siString str, siString strOld, siString strNew,
		siAllocator alloc);
/* Allocates a string, which has a specified amount of mentions of the old substring
 * replaced with the new one. */
SIDEF siString si_stringReplace(siString str, siString strOld, siString strNew,
		isize amount, siAllocator alloc);


/* Splits the string into _string views_ based on the separator. */
SIDEF siArray(siString) si_stringSplit(siString str, siString delimiter,
	siAllocator alloc);
/* Splits the string into a specified amount _string views_ based on the separator. */
SIDEF siArray(siString) si_stringSplitEx(siString str, siString delimiter,
	isize amount, siAllocator alloc);
/* Splits lines into string views. */
SIDEF siArray(siString) si_stringSplitLines(siString str, siAllocator alloc);

/* Iterates through the given string. If the specified delimiter is found, the
 * substring before it is written into the given out, then the input becomes a
 * substring from the delimiter's end to the string's end, then true is returned.
 * This also happens if the delimiter isn't found but the string isn't empty.
 * False is returned when the string is empty. */
SIDEF bool si_stringSplitIterate(siString* str, siString delimiter, siString* outStr);
/* Same functionality as 'si_stringSplitIterate', however the delimiter is '\n'.
 * The carrier return newline (\r) is also trimmed out. */
SIDEF bool si_stringSplitLinesIterate(siString* str, siString* outStr);

/* Allocates a string, which is the reversed specified string. */
SIDEF siString si_stringReverse(siString str, siAllocator alloc);

/* Converts the string's letters to uppercase. */
SIDEF siString si_stringUpper(siString str, siAllocator alloc);
/* Converts the string's letters to lowercase. */
SIDEF siString si_stringLower(siString str, siAllocator alloc);


SI_ENUM(i32, siNumToStrOption) {
	/* TODO */
	siNumToStrOption_Prefix = SI_BIT(0),
	/* TODO */
	siNumToStrOption_Plus = SI_BIT(1),
};

/* The highest base that a number can be in. */
#define SI_BASE_MAX 32

typedef struct siBuilder {
	siAllocator alloc;

	u8* data;
	isize len;
	isize capacity;
	isize grow;
} siBuilder;

#ifndef SI_BUILDER_NEW_CAP
	/* builder - siBuilder | addLen - isize
	 * Formula used to calculate the new length when memory is needed and 'grow'
	 * isn't specified. 'addlen' is the exact amount of bytes that is required. */
	#define SI_BUILDER_NEW_CAP(builder, addLen) SI_DYNAMIC_ARRAY_NEW_CAP(builder, addLen)
#endif


/* Creates a string builder structure. */
SIDEF siBuilder si_builderMake(siAllocator alloc, isize capacity, isize grow SI_DEFAULT(0));
#define si_builderMake(...) SI_DARG_IMPL(si_builderMake,     2, (0), __VA_ARGS__)

/* Creates a string builder structure with a set starting length. */
SIDEF siBuilder si_builderMakeLen(siAllocator alloc, isize capacity, isize len, isize grow SI_DEFAULT(0));
/* Creates an empty str builder structure. */
SIDEF siBuilder si_builderMakeNone(siAllocator alloc);
/* TODO */
SIDEF siBuilder si_builderFromArray(siArray(u8) array);

/* Returns the builder data as a string. */
SIDEF siString si_builderToStr(siBuilder b);
/* Writes a NULL-terminator byte and returns the builder data as a C-string. */
SIDEF char* si_builderToCstr(siBuilder* b);

/* Writes a byte into the builder. */
SIDEF isize si_builderWriteByte(siBuilder* b, u8 byte);
/* Writes the contents of a pointer into the builder. */
SIDEF isize si_builderWritePtr(siBuilder* b, const void* bytes, isize len);
/* TODO */
SIDEF isize si_builderWriteArray(siBuilder* b, siArray(u8) array);
/* Writes a string into the builder. */
SIDEF isize si_builderWriteStr(siBuilder* b, siString str);
/* Converts a rune into UTF-8 and writes it into the builder. */
SIDEF isize si_builderWriteRune(siBuilder* b, siRune character);

/* Writes an enquoted string. */
SIDEF isize si_builderWriteStrQuoted(siBuilder* b, siString str, u8 quote SI_DEFAULT('\"'));
/* Writes an enquoted string with an option to specify the starting and ending
 * UTF-32 quotation marks. */
SIDEF isize si_builderWriteStrQuotedRune(siBuilder* b, siString str,
	siRune quoteStart, siRune quoteEnd);

/* Converts an integer into a string and writes it to the builder. */
SIDEF isize si_builderWriteInt(
	siBuilder* b, i64 num,
	i32 base SI_DEFAULT(10), siNumToStrOption options SI_DEFAULT(0)
);

/* Converts an unsigned integer into a string and writes it to the builder. */
SIDEF isize si_builderWriteUInt(
	siBuilder* b, u64 num,
	i32 base SI_DEFAULT(10), siNumToStrOption options SI_DEFAULT(0)
);


/* Converts a float into a string and writes it to the builder. */
SIDEF isize si_builderWriteFloat(siBuilder* b, f64 num);
/* Converts a specified-base and afterPoint float into a string and writes it
 * to the builder. */
SIDEF isize si_builderWriteFloatEx(siBuilder* b, f64 num, i32 base,
	i32 afterPoint);

/* Sets the length to zero. */
SIDEF void si_builderClear(siBuilder* b);
/* Decrements the length by one, if possible. */
SIDEF void si_builderPopByte(siBuilder* b);
/* Decrements the length by the last character's length in UTF-8, if possible. */
SIDEF void si_builderPopRune(siBuilder* b);

/* TODO */
SIDEF void si_builderFree(siBuilder* b);


/* Checks if the current builder's capacity can hold the added length. If not,
 * the builder will reallocate the current data buffer, in which case the capacity
 * gets changed. _The length doesn't change_. If '->grow' isn't -1, the capacity
 * gets increase to 'b->capacity + addLen + b->grow', otherwise the new capacity
 * becomes the result of 'SI_BUILDER_NEW_CAP'. Returns an allocator error. */
SIDEF siAllocationError si_builderMakeSpaceFor(siBuilder* b, isize addLen);


/* TODO */
SIDEF siWriter si_builderToWriter(siBuilder* b);



/* Converts a string into a base 10 unsigned integer. If an invalid character is
 * found, its index gets written into the specified pointer; if not then '-1'. */
/* Converts a string into a specified base unsigned integer. Setting a '-1' base
 * the function to automatically find the base from the string's prefix. If an
 * invalid character is found, its index gets written into the specified pointer;
 * if not then '-1'. The prefix list: 0x (16), 0z (12), 0d (10), 0o (8), 0b (2). */
SIDEF u64 si_stringToUInt(
	siString str,
	i32 base SI_DEFAULT(-1), isize* outInvalidIndex SI_DEFAULT(nil)
);

/* Converts a string into a specified base unsigned integer. Setting a '-1' base
 * the function to automatically find the base from the string's prefix. If an
 * invalid character is found, its index gets written into the specified pointer;
 * if not then '-1'. The prefix list: 0x (16), 0z (12), 0d (10), 0o (8), 0b (2). */
SIDEF i64 si_stringToInt(
	siString str,
	i32 base SI_DEFAULT(-1), isize* outInvalidIndex SI_DEFAULT(nil)
);

/* Returns either 'true', 'false' or UINT32_MAX from a string. 'UINT32_MAX'
 * is returned if the given str cannot be converted into a boolean. */
SIDEF b32 si_stringToBool(siString str);

/* TODO */
siString si_stringFromBits(u64 num, isize bitSize, bool isSigned, i32 base,
		siNumToStrOption options, siString table, siArray(u8) out);

/* Makes a string from the integer. */
SIDEF siString si_stringFromInt(
	siArray(u8) out, i64 num,
	i32 base SI_DEFAULT(10), siNumToStrOption options SI_DEFAULT(0)
);

/* Makes a string from the unsigned integer. */
SIDEF siString si_stringFromUInt(
	siArray(u8) out, u64 num,
	i32 base SI_DEFAULT(10), siNumToStrOption options SI_DEFAULT(0)
);

/* Allocates a string from a float with 6 digits after the decimal point in base 10. */
SIDEF siString si_stringFromFloat(f64 num, siArray(u8) out);
/* Allocates a string from a float with the given amount of digits after the
 * decimal point in the specified base. */
SIDEF siString si_stringFromFloatEx(f64 num, i32 base, isize afterPoint, siArray(u8) out);

/* Returns a 'true'/'false' string from a boolean. */
SIDEF siString si_stringFromBool(bool boolean);


/* Checks if the string has the specified prefix. */
SIDEF bool si_stringHasPrefix(siString str, siString prefix);
/* Returns the amount of matching front characters between the specified strings. */
SIDEF isize si_stringPrefixLen(siString str, siString prefix);

/* Checks if the string has the specified suffix. */
SIDEF bool si_stringHasSuffix(siString str, siString suffix);
/* Returns the amount of matching back characters between the specified strings. */
SIDEF isize si_stringSuffixLen(siString str, siString suffix);

/* Allocates a formatted string that represents the contents of an array based
 * on the 'fmt' string. For example, if 'fmt = "<%x, %hhu, %#b>' and
 * 'array = (u8){255, 64, 4}, the string would become '{<0xFF, 64, 0b100>}'. */
SIDEF siString si_stringFromArray(siArrayAny array, cstring fmt, siArray(u8) out);



#if SI_SYSTEM_IS_WINDOWS
	/* UTF-16 encoded character. The string version is NULL-terminated. */
	typedef wchar_t siOsChar;
	SI_STATIC_ASSERT(si_sizeof(wchar_t) == si_sizeof(u16));
#else
	/* UTF-8 encoded character. The string version is NULL-terminated. */
	typedef char siOsChar;
#endif

/* Type of string that the OS specifics for API calls. */
typedef siOsChar* siOsString;
/* An array of two OS strings alongside their lengths. */
typedef struct siOsString_2x { siOsString v[2]; isize len[2]; } siOsString_2x;


/* Makes an OS-native string out of a string. */
SIDEF siOsString si_stringToOsStr(siString str, siArray(siOsChar) out);
/* Makes an OS-native string out of a string with the option to get the length.
 * The specified pointer cannot be nil. */
SIDEF siOsString si_stringToOsStrEx(siString str, siArray(siOsChar) out, isize* copied);



#ifndef SI_NO_MEMORY

/* Copies a string into the specified pointer (see 'si_memcopy' for more detail). */
SIDEF isize si_memcopyStr(void* dst, siString src);
/* Moves a string into the specified pointer (see 'si_memmove' for more detail). */
SIDEF isize si_memmoveStr(void* dst, siString src);
/* Compares a string with the specified pointer (see 'si_memcompare' for more detail). */
SIDEF i32 si_memcompareStr(const void* dst, siString src);

/* Copies a string into the specified pointer (see 'si_memcopy_s' for more detail). */
SIDEF isize si_memcopyStr_s(siArrayAny dst, siString src);
/* Moves a string into the specified pointer (see 'si_memmove_s' for more detail). */
SIDEF isize si_memmoveStr_s(siArrayAny dst, siString src);

#endif

#endif /* SI_NO_STRING */

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

#if SI_COMPILER_GCC || SI_COMPILER_CLANG
	/* TODO */
	#define SI_FUNC __builtin_FUNCTION()
#elif SI_COMPILER_MSVC
	/* TODO */
	#define SI_FUNC  __FUNCTION__
#else
	/* TODO */
	#define SI_FUNC __func__
#endif

/* Creates a 'siCallerLoc' structure from the current filename, line and function. */
#define SI_CALLER_LOC SI_COMP_LIT(siCallerLoc, SI_STR(__FILE__), SI_STR(SI_FUNC), __LINE__)

typedef struct siError {
	i32 code;

#ifndef SI_NO_ERROR_STRUCT
	siCallerLoc location;
	i64 time; /* The time is in UTC+0. */
#endif
} siError;


/* Sets the error code to zero. */
#define SI_ERROR_NIL SI_TYPE_ZERO(siError)


/* name - NAME
 * Defines a valid error logger function prototype. Returning 'false' terminates
 * the app. */
#define SI_ERROR_PROC(name) bool name(siError* error, void* data)
/* Represents an error logger procedure. */
typedef SI_ERROR_PROC(siErrorProc);

/* code - i32
  * Declares an error with a generic error log. */
#define SI_ERROR(code) SI_ERROR_EX(code, nil, nil)
/* code - i32 | function - siErrorProc (nullable) | data - void* (nullable)
 * Declares an error with a specified error log function and pointer data. Specifying
 * nil for the function defaults to the generic error log. */
#define SI_ERROR_EX(code, function, data) si__errorDeclare(code, function, data, SI_CALLER_LOC)

/* condition - bool | code - i32 | ...action - ACTION
 * If the condition is true, an error is declared with a generic error log and
 * the specified actions are executed. The result of 'SI_ERROR_EX' is saved in
 * the 'SI_ERROR_RES' variable. */
#define SI_ERROR_CHECK(condition, code, .../*action*/) \
	SI_ERROR_CHECK_EX(condition, code, nil, nil, __VA_ARGS__)
/* condition - bool | code - i32 | func - siErrorProc (nullable) | data - void* (nullable) | ...action - ACTION
 * If the condition is true, an error is declared with a specified error log and
 * and the specified actions are executed. The result of 'SI_ERROR_EX' is saved
 * in the 'SI_ERROR_RES' variable. Specifying nil for the function defaults to the
 * generic error log. */
#define SI_ERROR_CHECK_EX(condition, code, func, data, .../*action*/) \
	SI_STOPIF(condition, siError SI_ERROR_RES = SI_ERROR_EX(code, func, data); SI_UNUSED(SI_ERROR_RES); __VA_ARGS__);

/* condition - bool | code - i32
 * If the condition is true, an error is declared with a generic error log and
 * gets returned. */
#define SI_ERROR_CHECK_RET(condition, code) SI_ERROR_CHECK_EX_RET(condition, code, nil, nil)
/* condition - bool | code - i32 | func - siErrorProc (nullable) | data - void* (nullable)
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
		bool hasValue; \
		union {\
			type value; \
			siError error; \
		}; \
	} siOption_##type \

/* type - TYPE
 * Defines an optional type using the specified type's pointer type. */
#define si_optional_define_ptr(type) \
	typedef struct siOption_##type##Ptr { \
		bool hasValue; \
		union { \
			type* value; \
			siError error; \
		}; \
	} siOption_##type##Ptr



/* type - TYPE
 * Represents an object that may not contain a value. If so, no additional error
 * information will be written, only '.hasValue' will be set to false. */
#define siOption(type) si__OptionMacro(type)
/* type - TYPE
 * Same as 'siOption' except the value type is type*. */
#define siOptionPtr(type) si__OptionMacroPtr(type)
/* type - TYPE
 * Represents an object that may not contain a value. If so, additional error
 * information will be written with '.hasValue' being set to false. */
#define siResult(type) siOption(type)
/* type - TYPE
 * Same as 'siResult' except the value type is type*. */
#define siResultPtr(type) siOptionPtr(type)



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

si_optional_define(siString);
si_optional_define_ptr(void);
#ifndef SI_NO_ARRAY
si_optional_define(siArrayAny);
si_optional_define(siDynamicArrayAny);
#endif



/* type - TYPE | ...VALUE - EXPRESSION
 * Creates a returnable 'siOptional' value from the given value. */
#define SI_OPT(type, .../* VALUE */) SI_COMP_LIT(siOption(type), true, {__VA_ARGS__})
#define SI_OPT_PTR(type, .../* VALUE */) SI_COMP_LIT(siOptionPtr(type), true, {__VA_ARGS__})
/* type - TYPE
 * Creates a returnable 'siOptional' item that has no value inside.. */
#define SI_OPT_NIL(type) SI_OPT_ERR(type, SI_ERROR_NIL)
/* type - TYPE | errorV - siError
 * Creates a returnable 'siOptional' error value with a designated error. */
#define SI_OPT_ERR(type, errorV) SI_COMP_LIT(siOption(type), false, {.error = errorV})
#define SI_OPT_PTR_ERR(type, errorV) SI_COMP_LIT(siOptionPtr(type), false, {.error = errorV})


/* optionalVar - siOptional(TYPE) | defaultValue - EXPRESSION
 * Gets the value of the provided 'siObject' object. However if 'hasValue' is set
 * to false, return 'defaultValue' instead. */
#define si_optionalGetOrDefault(optionalVar, defaultValue) \
	((optionalVar).hasValue ? (optionalVar).value : (defaultValue))


/* condition - bool | code - i32 | type - TYPE
 * If the condition is true, an error is declared with a generic error log and
 * an error optional value gets returned. */
#define SI_OPTION_CHECK(condition, code, type) SI_OPTION_CHECK_EX(condition, code, nil, nil, type)
/* condition - bool | code - i32 | func - siErrorProc (nullable) | data - void* (nullable) | type - TYPE
 * If the condition is true, an error is declared with a specified error log and
 * an error optional value gets returned. Specifying nil for the function defaults
 * to the generic error log.*/
#define SI_OPTION_CHECK_EX(condition, code, func, param, type) \
	SI_ERROR_CHECK_EX(condition, code, func, param, return SI_OPT_ERR(type, SI_ERROR_RES))


#if 1 /* Ignore */
	#define si__OptionMacro(type) siOption_##type
	#define si__OptionMacroPtr(type) siOption_##type##Ptr
#endif

#endif /* SI_NO_OPTIONAL */

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


/* A UTF-32 encoded '?' character for reporting invalid states. */
#define SI_UTF32_INVALID SI_COMP_LIT(siUtf32Char, 0xFFFD, 3)
/* A UTF-8 encoded '?' character for reporting invalid states. */
#define SI_UTF8_INVALID SI_COMP_LIT(siUtf8Char, {0xEF, 0xBF, 0xBD}, 3)


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


/* Encodes a UTF-8 string into a UTF-16 string _without_ a NULL-terminator. The
 * conversion loops break is a NULL-terminator is reached. */
SIDEF siUtf16String si_utf8ToUtf16Str(siUtf8String str, siArray(u16) out);
/* Encodes a UTF-8 string into a UTF-16 string with an option to include a
 * NULL-terminator. The conversion loop breaks if a NULL-terminator is reached.
 * NOTE: The NULL-terminator is included in the length. */
SIDEF siUtf16String si_utf8ToUtf16StrEx(siUtf8String str, bool nullTerm, siArray(u16) out);

/* Encodes a UTF-16 string into a UTF-8 string _without_ a NULL-terminator. The
 * conversion loop breaks if a NULL-terminator is reached. */
SIDEF siUtf8String si_utf16ToUtf8Str(siUtf16String str, siArray(u8) out);
/* Encodes a UTF-16 string into a UTF-8 string with an option to include a
 * NULL-terminator. The conversion loop breaks if a NULL-terminator is reached.
 * NOTE: The NULL-terminator is included in the length. */
SIDEF siUtf8String si_utf16ToUtf8StrEx(siUtf16String str, bool nullTerm, siArray(u8) out);
/* Returns how many bytes a UTF-16 string would occupy in UTF-8. */
SIDEF isize si_utf16ToUtf8StrLen(siUtf16String str);


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
SIDEF bool si_runeIsLower(siRune rune);
/* Checks if the specified rune is uppercased. */
SIDEF bool si_runeIsUpper(siRune rune);
/* Returns true if the given rune is a space. */
SIDEF bool si_runeIsSpace(siRune rune);
/* Returns true if the given rune is from '0' to '9'. */
SIDEF bool si_runeIsDigit(siRune rune);
/* Returns true if the given rune is a hex digit (between '0'..'9'; 'a'...'f'; 'A'...'F'). */
SIDEF bool si_runeIsHex(siRune rune);
/* Returns true if the given rune is a letter. */
SIDEF bool si_runeIsAlpha(siRune rune);
/* Returns true if the given rune is a letter or a number. */
SIDEF bool si_runeIsAlphanumeric(siRune rune);
/* Checks if the specified rune is a punctuation character. */
SIDEF bool si_runeIsPunctuation(siRune rune);
/* Checks if the specified rune is a control character. */
SIDEF bool si_runeIsControl(siRune rune);
/* Checks if the specified rune is printable. */
SIDEF bool si_runeIsPrintable(siRune rune);
/* Checks if the specified rune is graphical. */
SIDEF bool si_runeIsGraphical(siRune rune);
/* Checks if the specified rune is NOT a letter, '@', '#' or '$'. */
SIDEF bool si_runeIsDelimiter(siRune rune);

/* Converts a digit to an integer ('3' -> 3). Returns -1 otherwise. */
SIDEF i32 si_runeDigitToInt(siRune rune);
/* Converts a base-32 digit into an integer ('F' -> 15). Returns -1 otherwise. */
SIDEF i32 si_runeBase32ToInt(siRune rune);



/* Returns the lowercased version of the given character. */
SIDEF char si_charLower(char c);
/* Returns the uppercased version of the given character. */
SIDEF char si_charUpper(char c);

/* Checks if the specified character is lowercased. */
SIDEF bool si_charIsLower(char c);
/* Checks if the specified character is uppercased. */
SIDEF bool si_charIsUpper(char c);
/* Returns true if the given character is a space. */
SIDEF bool si_charIsSpace(char c);
/* Returns true if the given character is from '0' to '9'. */
SIDEF bool si_charIsDigit(char c);
/* Returns true if the given character is a hex digit (between '0'..'9'; 'a'...'f'; 'A'...'F'). */
SIDEF bool si_charIsHex(char c);
/* Returns true if the given character is in the ASCII alphabet. */
SIDEF bool si_charIsAlpha(char c);
/* Returns true if the given character is in the ASCII alphabet OR is a number. */
SIDEF bool si_charIsAlphanumeric(char c);
/* Checks if the specified character is a punctuation character. */
SIDEF bool si_charIsPunctuation(char c);
/* Checks if the specified character is a control character. */
SIDEF bool si_charIsControl(char c);
/* Checks if the specified character is printable. */
SIDEF bool si_charIsPrintable(char c);
/* Checks if the specified character is graphical. */
SIDEF bool si_charIsGraphical(char c);
/* Checks if the specified character is NOT alphanumeric as well as not an '@',
 * '#' or '$' character. */
SIDEF bool si_charIsDelimiter(char c);

/* Converts '0'...'9' to an actual integer ('3' -> 3). */
SIDEF i32 si_charDigitToInt(char c);
/* Converts a base-32 digit into an integer ('F' -> 15). Returns -1 otherwise. */
SIDEF i32 si_charBase32ToInt(char c);


#endif /* SI_NO_CHAR */

#ifndef SI_NO_HASHING
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
	| siHashing            |
	========================
*/

/* Creates a 32-bit FNV hash. */
SIDEF u32 si_fnv32(const void* data, isize len);
/* Creates a 32-bit FNV-a hash */
SIDEF u32 si_fnv32a(const void* data, isize len);

/* Creates a 64-bit FNV hash. */
SIDEF u64 si_fnv64(const void* data, isize len);
/* Creates a 64-bit FNV-a hash. */
SIDEF u64 si_fnv64a(const void* data, isize len);

/* Creates a 32-bit MurmurHash3 hash. */
SIDEF u32 si_murmur32(const void* data, isize len);
/* Creates a 32-bit MurmurHash3 hash with a custom seed. */
SIDEF u32 si_murmur32Ex(const void* data, isize len, u32 seed);

/* Creates a 64-bit MurmurHash3 hash. */
SIDEF u64 si_murmur64(const void* data, isize len);
/* Creates a 64-bit MurmurHash3 hash with a custom seed. */
SIDEF u64 si_murmur64Ex(const void* data, isize len, u64 seed);

#endif /* SI_NO_HASHING */

#ifndef SI_NO_MAP
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
	| siMap                |
	========================
*/

typedef struct siMapEntry {
	siString key;
	u32 hash;
	u32 next;
} siMapEntry;

typedef struct siMapAny {
	siAllocator alloc;
	isize len;
	isize capacity;
	isize typeSize;
	siMapEntry* entries;
	void* values;
	u32* hashes;
} siMapAny;

/* type - TYPE
 * Represents a map with a specific type. */
#define siMap(type) siMapAny



/* key - siString | map - siMapAny
 * Loops through the elements of the map, writes the key name to 'key'. */
#define for_eachMap(key, map) \
	for (isize si__mapI = 0; si__forEachMap(si__mapI, map, &(key)); si__mapI += 1)
/* key - siString | value - TYPE | map - siMap(TYPE)
 * Loops through the elements of the map, writes the key name to 'key', the key
 * value to 'value'. */
#define for_eachMapEx(key, value, map) \
	for (isize si__mapI = 0; si__forEachMapEx(si__mapI, map, &(key), &(value)); si__mapI += 1)
/* key - siString | value - TYPE | map - siMap(TYPE)
 * Loops through the elements of the map, writes the key name to 'key', the key
 * value's pointer to 'value'. */
#define for_eachRefMap(key, value, map) \
	for (isize si__mapI = 0; si__forEachRefMap(si__mapI, map, &(key), &(value)); si__mapI += 1)



/* alloc - siAllocator | type - TYPE | ...map - struct {siString, key}
 * Creates a map from the specified contents. Map keys are seperated by commas. */
#define si_mapMake(alloc, type, .../* map */) SI__MAP_IMPL(alloc, type, __VA_ARGS__)
/* type - TYPE | capacity - isize | alloc - siAllocator
 * Reserves a map with the specified type and capacity. */
#define si_mapMakeReserve(type, capacity, alloc) si_mapReserve(si_sizeof(type), capacity, alloc)
/* Reserves a map with the specified type size and capacity. */
SIDEF siMapAny si_mapReserve(isize typeSize, isize capacity, siAllocator alloc);


/* Returns the pointer of an existing key's value, other 'nil' is returned. */
SIDEF void* si_mapGet(siMapAny map, siString name);
SIDEF void* si_mapGetHash(siMapAny map, siString name, u32 hash);

/* Sets the specified's key value to the given pointer's value. Returns the
 * set value's pointer inside the map. */
SIDEF void* si_mapSet(siMapAny* map, siString name, const void* value);
SIDEF void* si_mapSetHash(siMapAny* map, siString name, const void* value,
		u32 hash);

/* Removes the specified key from the map. */
SIDEF void si_mapErase(siMapAny* map, siString name);
SIDEF void si_mapEraseHash(siMapAny* map, siString name, u32 hash);

/* Empties the map. */
SIDEF void si_mapClear(siMapAny* map);

/* Frees the allocated memory by the map. */
SIDEF void si_mapFree(siMapAny map);


/* Gets the existing key's pointer value, dereferences it and casts it to the
 * specified type.
 * NOTE 1: The function doesn't check if the given type is correct.
 * NOTE 2: If the key doesn't exist, this function crashes. */
#define si_mapGetItem(map, name, type) *(type*)si_mapGet(map, name)
/* Sets the specified's key value to the given value. */
#define si_mapSetItem(map, name, value, type) (type*)si_mapSet(map, name, SI_PTR(type, value))


#endif /* SI_NO_MAP */

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


/* var - VARIABLE | mask - UINT | condition  - bool
 * Sets/unsets the mask for the variable. */
#define SI_MASK_SET(var, mask, condition) do { \
	if (condition) (var) |=  (mask); \
	else           (var) &= ~(mask); \
} while (0)

#if SI_ENDIAN_IS_LITTLE
	/* little - EXPRESSION | big - EXPRESSION
	* Returns the specified 'little' value. */
	#define SI_ENDIAN_VALUE(little, big) little
#else
	/* little - EXPRESSION | big - EXPRESSION
	* Returns the specified 'big' value. */
	#define SI_ENDIAN_VALUE(little, big) end
#endif


/* x - INT/UINT
 * Returns the number of set bits. */
#define si_countOnes(type, x) SI_BIT_FUNC(type, countOnes, x)
/* x - INT/UINT
 * Returns the number of unset bits. */
#define si_countZeros(type, x) ((si_sizeof(type) * 8) - si_countOnes(type, x))

/* type - TYPE | x - INT/UINT
 * Returns the number of leading set bits until an unset bit or the end is reached. */
#define si_countLeadingOnes(type, x) SI_BIT_FUNC(type, countLeadingOnes, x)
/* type - TYPE | x - INT/UINT
 * Returns the number of leading unset bits until a set bit or the end is reached. */
#define si_countLeadingZeros(type, x) SI_BIT_FUNC(type, countLeadingZeros, x)

/* type - TYPE | x - INT/UINT
 * Returns the number of trailing set bits until an unset bit or the front is reached. */
#define si_countTrailingOnes(type, x) SI_BIT_FUNC(type, countTrailingOnes, x)
/* type - TYPE | x - INT/UINT
 * Returns the number of trailing unset bits until a set bit or the front is reached. */
#define si_countTrailingZeros(type, x) SI_BIT_FUNC(type, countTrailingZeros, x)


/* type - TYPE | x - INT/UINT | bits - i32
 * Rotates the bits left by the specified amount. */
#define si_bitsRotateLeft(type, x, bits) (type)SI_BIT_FUNC(type, bitsRotateLeft, x, bits)
/* type - TYPE | x - INT/UINT | bits - i32
 * Rotates the bits right by 'bits' amount. */
#define si_bitsRotateRight(type, x, bits) (type)SI_BIT_FUNC(type, bitsRotateRight, x, bits)
/* type - TYPE | x - INT/UINT | alloc - siAllocator
 * Reverses the bits. */
#define si_bitsReverse(type, x) (type)SI_BIT_FUNC(type, bitsReverse, x)


/* type - TYPE | x - UINT/INT | alloc - siAllocator
 * Creates an array from the specified number. */
#define si_bytesToArray(type, x, alloc) SI_BIT_FUNC(type, bytesToArray, x, alloc)
/* Creates an integer from an array. */
SIDEF u64 si_bytesFromArray(siArray(u8) bytes);


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


/* Returns the length of a number. */
SIDEF isize si_numLenInt(i64 num);
/* Returns the length of a specified base number. */
SIDEF isize si_numLenIntEx(i64 num, i32 base);
/* Returns the length of an unsigned number. */
SIDEF isize si_numLenUint(u64 num);
/* Returns the length of a specified base unsigned number. */
SIDEF isize si_numLenUintEx(u64 num, i32 base);

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


#endif /* SI_NO_BIT */

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

/* Specifies time since the UNIX epoch with nanosecond precision.
 *
 * The time range:
 * min: 1677-09-21 00:12:44.145224192 +0000 UTC
 * max: 2262-04-11 23:47:16.854775807 +0000 UTC */
typedef i64 siTime;

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
	/* Nanoseconds (0-SI_SECOND - 1)*/
	i32 nanoseconds;
} siTimeCalendar;

SI_ENUM(i32, siTimeUnit) {
	siTimeUnit_Nanosecond = 0,
	siTimeUnit_Microsecond,
	siTimeUnit_Millisecond,
	siTimeUnit_Second,
	siTimeUnit_Minute,
	siTimeUnit_Hour,
	siTimeUnit_Day,
	siTimeUnit_Month,
	siTimeUnit_Year
};

typedef struct {
	siTimeUnit unit;
	siTime threshold;
} siTimeUnitScale;


/* An array of full month names. Used for 'si_timeToString'.  */
SI_EXTERN siString* SI_NAMES_MONTHS_FULL;
/* An array of short month names. Used for 'si_timeToString'.  */
SI_EXTERN siString* SI_NAMES_MONTHS_SHRT;
/* An array of full week day names. First element is "Sunday". Used for 'si_timeToString'.  */
SI_EXTERN siString* SI_NAMES_DAYS_FULL;
/* An array of short week day names. First element is "Sun". Used for 'si_timeToString'.  */
SI_EXTERN siString* SI_NAMES_DAYS_SHRT;
/* An array of AM/PM names. First two elements are the uppercased versions, the
 * last two are lowercased. Used for 'si_timeToString'. */
SI_EXTERN siString* SI_NAMES_AM_PM;

/* An array of time units from nanosecond to year.*/
SI_EXTERN siString* SI_NAMES_TIME_UNITS;

/* One nanosecond in nanoseconds */
#define SI_NANOSECOND (siTime)(1)
/* One microsecond in nanoseconds */
#define SI_MICROSECOND (SI_NANOSECOND * 1000)
/* One milisecond in nanoseconds */
#define SI_MILLISECOND  (SI_MICROSECOND * 1000)
/* One second in nanoseconds */
#define SI_SECOND (SI_MILLISECOND * 1000)
/* One minute in nanoseconds */
#define SI_MINUTE (SI_SECOND * 60)
/* One hour in nanoseconds */
#define SI_HOUR (SI_MINUTE * 60)
/* One day in nanoseconds */
#define SI_DAY (SI_HOUR * 24)



/* x - INT
 * Converts microseconds into nanoseconds. */
#define SI_TIME_MIC(x) ((x) * SI_MICROSECOND)
/* x - INT
 * Converts milliseconds into nanoseconds. */
#define SI_TIME_MIL(x) ((x) * SI_MILLISECOND)
/* x - INT
 * Converts seconds into nanoseconds. */
#define SI_TIME_SEC(x) ((x) * SI_SECOND)
/* x - INT
 * Converts minutes into nanoseconds. */
#define SI_TIME_MIN(x) ((x) * SI_MINUTE)
/* x - INT
 * Converts hours into nanoseconds. */
#define SI_TIME_HRS(x) ((x) * SI_HOUR)
/* x - INT
 * Converts days into nanoseconds. */
#define SI_TIME_DAY(x) ((x) * SI_DAY)



/* Reads the current value of the processor’s time-stamp counter.
 * NOTE: Only natively works for AMD64, i386, ARM64, RISC-V, WASI and PPC architectures.
 * On other architectures or CPUs the function relies on OS functions. */
SIDEF i64 si_RDTSC(void);
/* Reads the current value of the processor’s time-stamp counter and writes the
 * current processor ID to the pointer. */
SIDEF i64 si_RDTSCP(i32* proc);

/* Returns the current clock. */
SIDEF siTime si_clock(void);
/* Starts the current time in nanoseconds. */
#define si_timeStart() si_clock()

/* Prints the time */
SIDEF void si_timePrint(
	siTime time,
	siString label SI_DEFAULT(SI_STR_EMPTY), siCallerLoc call SI_DEFAULT(SI_CALLER_LOC)
);
/* Prints the time since the start. Returns the difference. */
SIDEF siTime si_timePrintSince(
	siTime time,
	siString label SI_DEFAULT(SI_STR_EMPTY), siCallerLoc call SI_DEFAULT(SI_CALLER_LOC)
);

/* Makes the CPU sleep for a certain amount of milliseconds. */
SIDEF void si_sleep(siTime time);

/* Returns the current time in UTC+0 time. */
SIDEF siTime si_timeNowUTC(void);
/* Returns the current time in local time. */
SIDEF siTime si_timeNowLocal(void);

/* Checks if the specified year is a leap year. */
SIDEF bool si_timeYearIsLeap(i32 year);
/* Returns an integer from 0 to 6 (Sunday to Saturday) that represents a week day
 * based on the specified date. */
SIDEF i32 si_timeGetDayOfWeek(i32 year, i32 month, i32 day);

/* Converts time in nanoseconds into human-readable time components. */
SIDEF siTimeCalendar si_timeToCalendar(i64 time);
/* Converts human-readable time components into time in nanoseconds. */
SIDEF siTime si_calendarToTime(siTimeCalendar calendar);

/* Creates a string in the specified buffer from the specified format.
 * Specifiers:
 * - 'yyyy'/'yy' - full year/the two last digits
 * - 'M'/'MM' - month digit without/with padding, 'MMM'/'MMMM' - shortened/full month name,
 * - 'd'/'dd' - day digit without/with padding, 'DDD'/'DDDD' - shortened/full day name,
 * - 'h'/'hh' - hours without/with padding, 'm'/'mm' - minutes without/with padding,
 * - 's'/'ss' - seconds without/with padding, 'n'/'nn' - nanoseconds without/with padding.
 * - 'AP/ap' - uppercased/lowercased am/pm display. */
SIDEF siString si_timeToString(siTimeCalendar calendar, siString fmt, siArray(u8) out);

/* Takes nanoseconds and returns the appriopraite time unit for it. An input of
 * '2.5+e9' nanoseconds would return SI_COMP_LIT(siTimeUnitScale, SI_SECOND, SI_STR("s")).  */
SIDEF siTimeUnitScale si_timeGetUnit(i64 time);
/* Returns a string of the specified unit type. */
SIDEF siString si_timeUnitStr(siTimeUnit type);

#endif /* SI_NO_TIME */

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



/* TODO */
#define si_bprintf(out, fmt, ...) si_bprintfEx(out, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_bprintfEx(
	siArray(u8) out, siString fmt, siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	bool newLine SI_DEFAULT(false)
);
#define si_bprintfEx(...) SI_DARG_IMPL(si_bprintfEx, 3, (true, false), __VA_ARGS__)


/* TODO */
#define si_bprintfLn(out, fmt, ...) si_wprintfLnEx(out, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_bprintfLnEx(
	siArray(u8) out, siString fmt, siArray(siAny) args,
	bool flush SI_DEFAULT(true)
);
#define si_bprintfLnEx(...) SI_DARG_IMPL(si_bprintfLnEx, 3, (true), __VA_ARGS__)



/* TODO */
#define si_sbprintf(out, fmt, ...) si_sbprintfEx(out, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_sbprintfEx(
	siBuilder* b, siString fmt, siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	bool newLine SI_DEFAULT(false)
);
#define si_sbprintfEx(...)     SI_DARG_IMPL(si_sbprintfEx, 3, (true, false), __VA_ARGS__)


/* TODO */
#define si_sbprintfLn(out, fmt, ...) si_sbprintfLnEx(out, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_sbprintfLnEx(
	siBuilder* b, siString fmt, siArray(siAny) args,
	bool flush SI_DEFAULT(true)
);
#define si_sbprintfLnEx(...) SI_DARG_IMPL(si_sbprintfLnEx, 3, (true), __VA_ARGS__)



/* TODO */
#define si_aprintf(allocator, fmt, ...) si_aprintfEx(allocator, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_aprintfEx(
	siAllocator alloc, siString fmt, siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	bool newLine SI_DEFAULT(false)
);
#define si_aprintfEx(...)      SI_DARG_IMPL(si_aprintfEx, 3, (true, false), __VA_ARGS__)


/* TODO */
#define si_aprintfLn(allocator, fmt, ...) si_wprintfLnEx(allocator, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_aprintfLnEx(
	siAllocator allocator, siString fmt, siArray(siAny) args,
	bool flush SI_DEFAULT(true)
);
#define si_aprintfLnEx(...) SI_DARG_IMPL(si_aprintfLnEx, 3, (true), __VA_ARGS__)



/* TODO */
#define si_wprint(writer, ...) si_wprintEx(writer, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_wprintEx(
	siStream writer, siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	siString seperator SI_DEFAULT(SI_STR(" "))
);
#define si_wprintEx(writer, args, ...) (si_wprintEx)(SI_DARG_IMPL_WF(2, (true, SI_STR(" ")), writer, args __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_wprintLn(writer, ...) si_wprintLnEx(writer, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_wprintLnEx(
	siStream writer, siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	siString seperator SI_DEFAULT(SI_STR(" "))
);
#define si_wprintLnEx(writer, args, ...) (si_wprintLnEx)(SI_DARG_IMPL_WF(2, (true, SI_STR(" ")), writer, args __VA_OPT__(,) __VA_ARGS__))



/* TODO */
#define si_wprintf(writer, fmt, ...) si_wprintfEx(writer, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_wprintfEx(
	siStream writer, siString fmt, siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	bool newLine SI_DEFAULT(false)
);
#define si_wprintfEx(writer, fmt, args, ...) (si_wprintfEx)(SI_DARG_IMPL_WF(3, (true, false), writer, fmt, args __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_wprintfLn(writer, fmt, ...) si_wprintfLnEx(writer, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_wprintfLnEx(
	siStream writer, siString fmt, siArray(siAny) args,
	bool flush SI_DEFAULT(true)
);
#define si_wprintfLnEx(writer, fmt, args, ...) (si_wprintfLnEx)(SI_DARG_IMPL_WF(3, (true), writer, fmt, args __VA_OPT__(,) __VA_ARGS__))


SI_ENUM(i32, siFmtInfoState) {
	siFmtInfoState_Minus = SI_BIT(0),
	siFmtInfoState_Plus  = SI_BIT(1),
	siFmtInfoState_Space = SI_BIT(2),
	siFmtInfoState_Zero  = SI_BIT(3),
	siFmtInfoState_Hash  = SI_BIT(4),
	siFmtInfoState_Width = SI_BIT(5),
	siFmtInfoState_Prec  = SI_BIT(6),
	siFmtInfoState_Color = SI_BIT(7)
};

typedef struct siFmtInfo {
	siStream writer;
	siArray(siAny) args;

	siFmtInfoState state;
	isize width;
	isize precision;

	isize n;
	isize vaCount;
} siFmtInfo;



/* TODO */
SIDEF void si_fmtValue(siFmtInfo* info, siAny arg);


/* TODO */
SIDEF void si_fmtStr(siFmtInfo* info, siString str);

/* TODO */
SIDEF void si_fmtCstring(siFmtInfo* info, cstring str);

/* TODO */
SIDEF void si_fmtRune(siFmtInfo* info, siRune rune);


/* TODO */
SIDEF void si_fmtInt(
	siFmtInfo* info, u64 value, isize bitSize,
	i32 base SI_DEFAULT(10),
	bool isSigned SI_DEFAULT(false),
	bool uppercase SI_DEFAULT(true)
);
#define si_fmtInt(...)  SI_DARG_IMPL(si_fmtInt, 3, (10, true, true), __VA_ARGS__)

/* TODO */
SIDEF void si_fmtBool(siFmtInfo* info, bool b);

/* TODO */
SIDEF void si_fmtPtr(siFmtInfo* info, void* ptr);


/* TODO */
SIDEF void si_fmtFloat(
	siFmtInfo* info, f64 num,
	i32 base SI_DEFAULT(10)
);
#define si_fmtFloat(...) SI_DARG_IMPL(si_fmtFloat, 2, (10), __VA_ARGS__)

/* TODO */
SIDEF void si_fmtSci(
	siFmtInfo* info, f64 num,
	bool uppercase SI_DEFAULT(true)
);
#define si_fmtSci(...) SI_DARG_IMPL(si_fmtSci, 2, (true), __VA_ARGS__)


/* TODO */
SIDEF void si_fmtCallerLoc(siFmtInfo* info, siCallerLoc loc);



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

typedef struct siPrintColor3bitStruct {
	siPrintColor3bit color;
	bool bold, light;
} siPrintColor3bitStruct;

typedef struct siPrintColor {
	siPrintColorType type;
	union {
		siPrintColor3bitStruct ansi;
		u8 cube;
		u8 rgb[3];
	};
} siPrintColor;

/* Creates a regular ANSI/3-bit print color. */
SIDEF siPrintColor si_printColor3bit(
	siPrintColor3bit color,
	bool bold SI_DEFAULT(false),
	bool light SI_DEFAULT(false)
);
#define si_printColor3bit(...) SI_DARG_IMPL(si_printColor3bit, 1, (false, false), __VA_ARGS__)

/* Creates an 8-bit print color that relies on a 256-color lookup table.*/
SIDEF siPrintColor si_printColor8bit(u8 color);
/* Creates a 24-bit "true color" print color based on RGB. */
SIDEF siPrintColor si_printColor24bit(u8 r, u8 g, u8 b);

/* Checks if the terminal supports displaying 24-bit colors. */
SIDEF bool si_printHas24bitColor(void);
/* Disables the coloring functionality of '%C' in si_printf. */
SIDEF void si_printEnableColor(bool value);

#endif /* SI_NO_PRINT */

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

/* TODO(EimaMei): Remove this later on. */
/* Calculates the largest integer not greater than 'x'. */
#define si_round(type, x) SI_MATH_FUNC(type, round, x)

/* Calculates the largest integer not greater than 'x'. */
#define si_floor(type, x) SI_MATH_FUNC(type, floor, x)

/* Calculates the smallest integer not less than 'x'. */
#define si_ceil(type, x) SI_MATH_FUNC(type, ceil, x)


/* Checks if the given 32-bit float is NaN. */
SIDEF bool si_float32IsNan(f32 num);
/* Checks if the given 64-bit float is NaN. */
SIDEF bool si_float64IsNan(f64 num);

/* Checks if the given 32-bit float is infinite. '0' - number isn't infinite,
 * '1' - positive infinity, '2' - negative infinity. */
SIDEF i32 si_float32IsInf(f32 num);
/* Checks if the given 64-bit float is infinite. '0' - number isn't infinite,
 * '1' - positive infinity, '2' - negative infinity. */
SIDEF i32 si_float64IsInf(f64 num);

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
		i64 timeSince = si_RDTSC(); \
		volatile usize i; \
		for (i = 0; i < (timesToLoop); i += 1) { \
			function; \
		} \
		timeSince = si_RDTSC() - timeSince; \
		f64 timeTaken = (f64)timeSince / ((f64)si_cpuClockSpeed() / (f64)SI_NANOSECOND) / (f64)SI_MILLISECOND; \
		si_printf(SI_PERFORMANCE_MSG, SI_STR(#function), timeTaken, timesToLoop / 1000000.0); \
	} while(0)

/* time - siTime | function - NAME
 * Runs the function for a specified amount of time and prints the execution count. */
#define si_benchmarkExecutesPerTime(time, function) \
	do { \
		u64 counter = 0; \
		i64 si_time = time; \
		siThread thread; \
		si_threadMakeAndRun(si__benchmarkThread, &si_time, &thread); \
		while (thread.state == siThreadState_Running) { \
			function; \
			counter += 1; \
		} \
		si_printf(SI_PERFORMANCE_MSG, SI_STR(#function), (f64)si_time, (f64)counter / 1000000.0); \
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
		i64 array[20]; \
		isize len = 0; \
		si_benchmarkLoop(function, array, &len, start, end); \
		\
		usize range[] = {start, end}; \
		si_benchmarkLoopsAvgPrint(SI_STR(#function), array, len, range); \
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
		i64 arrays[2][20]; \
		isize len1 = 0, len2 = 0; \
		si_benchmarkLoop(function1, arrays[0], &len1, start, end); \
		si_benchmarkLoop(function2, arrays[1], &len2, start, end); \
		\
		siString funcs[] = {SI_STR(#function1), SI_STR(#function2)}; \
		usize range[] = {start, end}; \
		si_benchmarkLoopsAvgCmpPrint(funcs, arrays, len1, range); \
	} while(0)

#define SI_PERFORMANCE_MSG \
	"====== BENCHMARK DATA ======\n" \
	"General:\n" \
		"\tFunction - '%s'\n" \
		"\tDuration - '%f' ms\n" \
		"\tExecution count - '%f' million\n"

#define si_benchmarkLoop(function, array, lenPtr, start, end) \
	do { \
		u64 medianIndex; \
		for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10) { \
			/* NOTE(EimaMei): Having a 'print' statement forces smart compilers
			 * like GCC and Clang to become dumb, by stopping them optimzing the function out. */ \
			si_printf("%n", medianIndex); \
			\
			u64 index; \
			i64 t1, t2; \
			\
			t1 = si_RDTSC(); \
			for (index = 0; index < medianIndex; index++) { (void)function; } \
			t2 = si_RDTSC(); \
			\
			array[*(lenPtr)] = t2 - t1; \
			*(lenPtr) += 1; \
		} \
	} while (0)


#endif /* SI_NO_BENCHMARK */

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
	siUnixDE_Custom = 0,
	siUnixDE_KDE,
	siUnixDE_GNOME,
	siUnixDE_Xfce,
	siUnixDE_LXQt,
	siUnixDE_LXDE,
	siUnixDE_MATE,
	siUnixDE_Cinnamon,
};


/* Declares a system error in the function. */
#define SI_ERROR_SYS() SI_ERROR_SYS_EX(si_systemGetError())
/* code - i32
 * Declares a specific system error in the function. */
#define SI_ERROR_SYS_EX(code) SI_ERROR_EX(code, si_systemErrorLog, nil);
/* condition - bool | action - ANYTHING
 * If condition evaluates to true, a system error is declared in the function and
 * executes the specified actions. */
#define SI_ERROR_SYS_CHECK(condition, .../*action*/) \
	SI_ERROR_CHECK_EX(condition, si_systemGetError(), si_systemErrorLog, nil, __VA_ARGS__)
/* condition - bool
 * If condition evaluates to true, a system error is declared in the function and
 * then gets returned. */
#define SI_ERROR_SYS_CHECK_RET(condition) SI_ERROR_CHECK_EX_RET(condition, si_systemGetError(), si_systemErrorLog, nil)
/* condition - bool | type - TYPE
 * If condition evaluates to true, an optional system error is declared in the
 * function and then gets returned. */
#define SI_OPTION_SYS_CHECK(condition, type) SI_OPTION_CHECK_EX(condition, si_systemGetError(), si_systemErrorLog, nil, type)


/* Returns the most recent system error. */
SIDEF siErrorSystem si_systemGetError(void);

/* Returns the name of a system error. */
SIDEF siString si_systemErrorName(siErrorSystem err);
/* Returns the description of a system error. */
SIDEF siString si_systemErrorDesc(siErrorSystem err);


/* Ends the program with a to be returned code. */
siNoreturn SIDEF void si_exit(i32 code);


/* Sets an environment variable for the current process. */
SIDEF bool si_envVarSet(siString name, siString value);
/* Unsets an environment variable for the current process. */
SIDEF bool si_envVarUnset(siString name);

/* Gets the data length of the specified environment variable. */
SIDEF isize si_envVarGetLength(siString name);
/* Gets an environment variable for the current process. If the specified variable
 * name doesn't exist, the returned string's data is set to nil. */
SIDEF siString si_envVarGetData(siString name, siArray(u8) out);


/* Returns the current running Windows OS version. Returns '0' if the version is
 * older than XP. */
SIDEF siWindowsVersion si_windowsGetVersion(void);

/* Returns true if the current Unix system is running Wayland. */
SIDEF bool si_unixIsWayland(void);
/* Returns true if the current Unix system is running X11. */
SIDEF bool si_unixIsX11(void);

/* Returns the current running desktop environment on the system. If the found
 * desktop environment isn't recognized by Sili, 0 is returned. */
SIDEF siUnixDE si_unixGetDE(void);

/* Default error procedure when a system error is declared. */
SIDEF SI_ERROR_PROC(si_systemErrorLog);

#endif /* SI_NO_SYSTEM */

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

/* Requests memory from the OS, where the new mapping is specified in the given
 * pointer. If the pointer is nil, the OS choices a page-aligned mapping itself. */
SIDEF siResult(siVirtualMemory) si_vmAlloc(void* address, isize size);
/* Discards the page by marking it as "not in use" for later use. */
SIDEF siError si_vmDiscard(siVirtualMemory vm);
/* Frees the allocated memory by the OS. */
SIDEF siError si_vmFree(siVirtualMemory vm);


#endif /* SI_NO_VIRTUAL_MEMORY */

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

/* TODO */
typedef isize siFile;

SI_ENUM(u32, siFileMode) {
	siFileMode_Read = SI_BIT(0),
	siFileMode_Write = SI_BIT(1),
	siFileMode_Append = SI_BIT(2),
	siFileMode_Plus = SI_BIT(3),

	siFileMode_All = siFileMode_Read | siFileMode_Write | siFileMode_Append | siFileMode_Plus
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


/* A character that denotes a separate folder in a path. On Windows, this is '\',
 * elsewhere - '/' */
SI_EXTERN const u8 SI_PATH_SEPARATOR;

/*
	========================
	|  siPath              |
	========================
*/


/* Checks if the specified path exists. */
SIDEF bool si_pathExists(siString path);
SIDEF bool si_pathExistsOS(siOsString path);

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

/* Returns the file name and extension (/home/user/file.txt -> file.txt). */
SIDEF siString si_pathBaseName(siString path);
/* Returns the 'short' file extension with the dot (file.txt -> .txt, file.tar.gz -> .gz). */
SIDEF siString si_pathExtension(siString path);
/* Returns the file's entire extension with the dot (file.txt -> .txt, file.tar.gz -> tar.gz). */
SIDEF siString si_pathLongExtension(siString path);
/* Returns the path without the short extension (/home/user/file.ar.00 -> file.ar). */
SIDEF siString si_pathStem(siString path);
/* Returns the path without the long extension (/home/user/file.ar.00 -> file). */
SIDEF siString si_pathShortStem(siString path);
/* Returns a string view of the specified path's unrooted path. */
SIDEF siString si_pathUnrooted(siString path);

/* Finds the full, rooted path of the specified path and creates a string from
 * it. Returns an error if finding the full path failed. */
SIDEF siResult(siString) si_pathGetFullName(siString path, siAllocator alloc);
/* Joins the specified strings into one, seperated by the path separator. */
SIDEF siString si_pathJoin(siString path, siString subPath, siAllocator alloc);
SIDEF siString si_pathJoinEx(siArray(siString) strs, siAllocator alloc);

/* Returns the specified path's last write time in UNIX time. Zero is returned
 * if the time couldn't be found. */
SIDEF siTime si_pathLastWriteTime(siString path);

/* TODO */
SIDEF siArray(u8) si_pathReadContents(
	siString path, siAllocator alloc,
	siErrorSystem* outError SI_DEFAULT(nil)
);
/* TODO */
SIDEF siString si_pathReadContentsStr(
	siString path, siAllocator alloc,
	siErrorSystem* outError SI_DEFAULT(nil)
);

/* TODO */
SIDEF siArray(siString) si_pathReadlines(
	siString path, siAllocator alloc,
	siErrorSystem* outError SI_DEFAULT(nil)
);


/* Checks if the specified path is absolute. */
SIDEF bool si_pathIsAbsolute(siString path);
/* Checks if the specified path is relative. */
SIDEF bool si_pathIsRelative(siString path);


/* Returns a static string of the OS's default temporary path. */
SIDEF siString si_pathGetTmp(void);


/* Converts a string into an OS string path. Returns the length. */
SIDEF isize si_pathToOS(siString path, siOsChar* out, isize capacity);
/* Converts two strings into OS string paths. Returns the strings and lengths. */
SIDEF siOsString_2x si_pathToOSMul(siString first, siString second, siOsChar* out,
		isize capacity);



/*
	========================
	|  siFile              |
	========================
*/

/* Gets the pointer to the specified standard file (stdin, stdout or stderr). */
SIDEF siFile si_fileGetStdFile(siStdFile type);



/* Creates a file. */
SIDEF siFile si_fileCreate(siString path, siError* outError SI_DEFAULT(nil));
/* Opens a file that exists. */
SIDEF siFile si_fileOpen(siString path, siError* outError SI_DEFAULT(nil));
/* The file is opened via a specified mode. */
SIDEF siFile si_fileOpenMode(siString path, siFileMode mode, siError* outError SI_DEFAULT(nil));


/* TODO */
SIDEF isize si_fileRead(siFile file, siArray(u8) out, siErrorSystem* outError SI_DEFAULT(nil));
/* TODO */
SIDEF isize si_fileReadAt(
	siFile file, isize offset, siArray(u8) out,
	siErrorSystem* outError SI_DEFAULT(nil)
);

/* TODO */
SIDEF siArray(u8) si_fileReadContents(
	siFile file, siAllocator alloc,
	siErrorSystem* outError SI_DEFAULT(nil)
);
SIDEF siString si_fileReadContentsStr(
	siFile file, siAllocator alloc,
	siErrorSystem* outError SI_DEFAULT(nil)
);

/* Allocates 'file.size' bytes, reads said amount (if possible) from the file's
 * beginning and then splits the string into an array of string view lines.
 * _File seek offset does not get changed when calling the function._ */
SIDEF siArray(siString) si_fileReadlines(siFile file, siAllocator alloc, siErrorSystem* outError SI_DEFAULT(nil));


/* Writes a buffer into the file at the current offset. Returns the written bytes. */
SIDEF isize si_fileWrite(siFile file, siArray(u8) in, siErrorSystem* outError SI_DEFAULT(nil));
/* Writes a buffer into the file at the specified offset. Returns the written bytes. */
SIDEF isize si_fileWriteAt(
	siFile file, isize offset, siArray(u8) in,
	siErrorSystem* outError SI_DEFAULT(nil)
);
/* Writes a byte into the file. Return the written bytes. */
SIDEF isize si_fileWriteByte(siFile file, u8 byte, siErrorSystem* outError SI_DEFAULT(nil));
/* Writes a pointer into the file. Returns the written bytes. */
SIDEF isize si_fileWritePtr(
	siFile file,
	const void* ptr,
	isize len,
	siErrorSystem* outError SI_DEFAULT(nil)
);
/* Writes a string into the file. Returns the written bytes. */
SIDEF isize si_fileWriteStr(siFile file, siString str, siErrorSystem* outError SI_DEFAULT(nil));


/* Returns the current offset of the file stream. */
SIDEF isize si_fileTell(siFile file, siErrorSystem* outError SI_DEFAULT(nil));
/* Seeks the file stream offset to the specified offset using the given method.
 * Returns the amount of bytes that were read. */
SIDEF isize si_fileSeek(
	siFile file,
	isize offset,
	siSeekWhere method,
	siErrorSystem* outError SI_DEFAULT(nil)
);
/* Seeks to the front of the file. */
SIDEF siErrorSystem si_fileSeekFront(siFile file);
/* Seeks to the back of the file. */
SIDEF siErrorSystem si_fileSeekBack(siFile file);

/* TODO */
SIDEF siErrorSystem si_fileFlush(siFile file);
/* Truncates the file to the specified size and returns 'true' if it succeded. */
SIDEF bool si_fileTruncate(siFile file, isize size);

/* Returns the latest size of the file. */
SIDEF isize si_fileSize(siFile file);
/* Returns the last time the file was written. */
SIDEF siTime si_fileLastWriteTime(siFile file);

/* Closes the file. */
SIDEF void si_fileClose(siFile file);


/* TODO */
SIDEF siStream si_fileStream(siFile file);

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

typedef struct siDirectoryIterator {
	siString path;
	siIoType type;
} siDirectoryIterator;

typedef struct siDirectory {
	siError error;
	void* handle;
	isize directoryLen;
	u8 buffer[SI_PATH_MAX];
} siDirectory;


/* Opens a directory and creates a directory stream. */
SIDEF siDirectory si_directoryOpen(siString path);

/* Iterates through the next file, folder or link inside the directory. Information
 * about it is written into the specified 'out' and true is returned, otherwise
 * the _stream automatically gets closed_ and false is returned.
 *
 * NOTE 1: If an error occurred, 'false' is returned, the stream is closed and an
 * error is written into 'dir->error'.
 * NOTE 2: If you decide to end the iteration process early, you mustcall
 * 'si_directoryClose'. */
SIDEF bool si_directoryIterate(siDirectory* dir, siDirectoryIterator* out);

/* Iterates through the next file, folder or link inside the directory. Information
 * about it is written into the specified 'out' and true is returned, otherwise
 * the _stream automatically gets closed_ and false is returned. There is also
 * the option for the returned path to contain the base directory.
 *
 * NOTE 1: If an error occurred, 'false' is returned, the stream is closed and an
 * error is written into 'dir->error'.
 * NOTE 2: If you decide to end the polling process early, make sure to call
 * 'si_directoryClose'. */
SIDEF bool si_directoryIterateEx(siDirectory* dir, bool fullPath, siDirectoryIterator* out);

/* Closes the directory stream. */
SIDEF void si_directoryClose(siDirectory* dir);


#endif /* SI_NO_IO */

#ifndef SI_NO_PRINT_SYSTEM
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
	|  siPrintf_io         |
	========================
*/


/* TODO */
#define si_print(...) si_printEx(SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_printEx(
	siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	siString seperator SI_DEFAULT(SI_STR(" "))
);
#define si_printEx(args, ...) (si_printEx)(SI_DARG_IMPL_WF(1, (true, SI_STR(" ")), args __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_printLn(...) si_printLnEx(SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_printLnEx(
	siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	siString seperator SI_DEFAULT(SI_STR(" "))
);
#define si_printLnEx(args, ...) (si_printLnEx)(SI_DARG_IMPL_WF(1, (true, SI_STR(" ")), args __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_printf(fmt, ...) si_printfEx(fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_printfEx(
	siString fmt, 
	siArray(siAny) args SI_DEFAULT(SI_ARR_EMPTY(siAny)),
	bool flush SI_DEFAULT(true),
	bool newLine SI_DEFAULT(false)
);
#define si_printfEx(fmt, ...) (si_printfEx)(SI_DARG_IMPL_WF(1, (SI_ARR_EMPTY(siAny), true, false), SI_STR(fmt) __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_printfLn(fmt, ...) si_printfLnEx(fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_printfLnEx(
	siString fmt, 
	siArray(siAny) args SI_DEFAULT(SI_ARR_EMPTY(siAny)),
	bool flush SI_DEFAULT(true)
);
#define si_printfLnEx(fmt, ...) (si_printfLnEx)(SI_DARG_IMPL_WF(1, (SI_ARR_EMPTY(siAny), true), SI_STR(fmt) __VA_OPT__(,) __VA_ARGS__))



/* TODO */
/* TODO */
#define si_fprint(file, ...) si_fprintEx(file, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_fprintEx(
	siFile file, siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	siString seperator SI_DEFAULT(SI_STR(" "))
);
#define si_fprintEx(file, args, ...) (si_fprintEx)(SI_DARG_IMPL_WF(2, (true, SI_STR(" ")), file, args __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_fprintLn(file, ...) si_fprintLnEx(file, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_fprintLnEx(
	siFile file, siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	siString seperator SI_DEFAULT(SI_STR(" "))
);
#define si_fprintLnEx(file, args, ...) (si_fprintLnEx)(SI_DARG_IMPL_WF(2, (true, SI_STR(" ")), file, args __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_fprintf(file, fmt, ...) si_fprintfEx(file, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_fprintfEx(
	siFile file, siString fmt,
	siArray(siAny) args SI_DEFAULT(SI_ARR_EMPTY(siAny)),
	bool flush SI_DEFAULT(true),
	bool newLine SI_DEFAULT(false)
);
#define si_fprintfEx(file, fmt, ...) (si_fprintfEx)(SI_DARG_IMPL_WF(2, (SI_ARR_EMPTY(siAny), true, false), file, SI_STR(fmt) __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_fprintfLn(file, fmt, ...) si_fprintfLnEx(file, fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_fprintfLnEx(
	siFile file, siString fmt,
	siArray(siAny) args SI_DEFAULT(SI_ARR_EMPTY(siAny)),
	bool flush SI_DEFAULT(true)
);
#define si_fprintfLnEx(file, fmt, ...) (si_fprintfLnEx)(SI_DARG_IMPL_WF(2, (SI_ARR_EMPTY(siAny), true), file, SI_STR(fmt) __VA_OPT__(,) __VA_ARGS__))



/* TODO */
#define si_eprint(...) si_eprintEx(SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_eprintEx(
	siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	siString seperator SI_DEFAULT(SI_STR(" "))
);
#define si_eprintEx(args, ...) (si_eprintEx)(SI_DARG_IMPL_WF(1, (true, SI_STR(" ")), args __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_eprintLn(...) si_eprintLnEx(SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_eprintLnEx(
	siArray(siAny) args,
	bool flush SI_DEFAULT(true),
	siString seperator SI_DEFAULT(SI_STR(" "))
);
#define si_eprintLnEx(args, ...) (si_eprintLnEx)(SI_DARG_IMPL_WF(1, (true, SI_STR(" ")), args __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_eprintf(fmt, ...) si_eprintfEx(fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_eprintfEx(
	siString fmt, 
	siArray(siAny) args SI_DEFAULT(SI_ARR_EMPTY(siAny)),
	bool flush SI_DEFAULT(true),
	bool newLine SI_DEFAULT(false)
);
#define si_eprintfEx(fmt, ...) (si_eprintfEx)(SI_DARG_IMPL_WF(1, (SI_ARR_EMPTY(siAny), true, false), SI_STR(fmt) __VA_OPT__(,) __VA_ARGS__))


/* TODO */
#define si_eprintfLn(fmt, ...) si_eprintfLnEx(fmt, SI_ARGS(__VA_ARGS__))
/* TODO */
SIDEF isize si_eprintfLnEx(
	siString fmt, 
	siArray(siAny) args SI_DEFAULT(SI_ARR_EMPTY(siAny)),
	bool flush SI_DEFAULT(true)
);
#define si_eprintfLnEx(fmt, ...) (si_eprintfLnEx)(SI_DARG_IMPL_WF(1, (SI_ARR_EMPTY(siAny), true), SI_STR(fmt) __VA_OPT__(,) __VA_ARGS__))



/* TODO */
siNoreturn SIDEF void SI_DEBUG_TRAP(void);
/* TODO */
/* NOTE(?): Use the SI_PANIC() macros instead. */
siNoreturn SIDEF void si_panic(
	siString condition,
	siString fmt SI_DEFAULT(SI_STR_EMPTY),
	siArray(siAny) args SI_DEFAULT(SI_ARR_EMPTY(siAny)),
	siCallerLoc call SI_DEFAULT(SI_CALLER_LOC)
);
#define si_panic(...)          SI_DARG_IMPL(si_panic,          1, (SI_STR_EMPTY, (SI_ARR_EMPTY(siAny)), SI_CALLER_LOC), __VA_ARGS__)


#endif /* SI_NO_PRINT_SYSTEM */

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
		HANDLE id;
	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE || SI_SYSTEM_EMSCRIPTEN
		pthread_t id;
	#else
		isize id;
	#endif

	siThreadFunction* func;
	void* arg;
	usize stackSize;

	volatile siThreadState state;
	void* returnValue;
} siThread;


/* thread - siThread | type - TYPE
 * Casts 'thread.returnValue' correctly to match the given type instead of it being
 * void*. */
#define si_threadGetReturn(thread, type) transmute(type, (thread).returnValue)


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

#endif /* SI_NO_THREAD */

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

/* Returns the CPU's clock speed in Hz. The result of the function gets cached. */
SIDEF i64 si_cpuClockSpeed(void);
/* Returns the amount of CPU processors there are. */
SIDEF i32 si_cpuProcessorCount(void);


#endif /* SI_NO_CPU */

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

/* Denotes a pointer to a loaded dynamic library. */
typedef void* siDllHandle;
/* Denotes a pointer to a loaded procedure symbol. */
typedef void* siDllProc;

/* The file extension used to dentoe a dynamic library. On Windows the value is
 * set to "DLL", MacOS - "dylib", Linux/Unix - "so". */
SI_EXTERN const siString SI_DLL_EXTENSION;

/* Loads the specified dynamic library. */
SIDEF siDllHandle si_dllLoad(siString path);
/* Loads the specified dynamic library with the option to make the symbols 'global'.
 * This would make the loaded symbols accesable to other loaded shared objects.
 * On Windows this has no result. */
SIDEF siDllHandle si_dllLoadEx(siString path, bool globalSymbols);
/* Unloads the specified dynamic library. */
SIDEF void si_dllUnload(siDllHandle dll);

/* Returns a pointer to the specified procedure. */
SIDEF siDllProc si_dllProcAddress(siDllHandle dll, siString name);

/* Returns a string for any encountered errors, related to dynamic libraries. */
SIDEF siString si_dllError(void);


#ifndef siDllProcType
	/* function - FUNCTION
	 * The type format used for 'si_dllProcAddressFunc'. */
	#define siDllProcType(function) si__##function##_Proc
#endif

/* dll - siDllHandle | function - FUNCTION
 * Returns a pointer to the specified processor and casts it to the set function
 * load format type in a ISO-C comapatible way. */
#define si_dllProcAddressFunc(dll, function) si_dllProcAddressFuncEx(dll, SI_STR(#function), siDllProcType(function))
/* dll - siDllHandle | function - siString | type - TYPE
 * Returns a pointer to the specified processor and casts it to the specified
 * type in a ISO-C comapatible way. */
#define si_dllProcAddressFuncEx(dll, function, type) \
	transmute(type, si_dllProcAddress(dll, function), siDllProc)

#endif /* SI_NO_DLL */

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
		#define SI_IMPLEMENTATION_MEMORY 1
	#endif
	#ifndef SI_NO_ALLOCATOR
		#define SI_IMPLEMENTATION_ALLOCATOR 1
	#endif
	#ifndef SI_NO_ARRAY
		#define SI_IMPLEMENTATION_ARRAY 1
	#endif
	#ifndef SI_NO_STREAM
		#define SI_IMPLEMENTATION_STREAM 1
	#endif
	#ifndef SI_NO_STRING
		#define SI_IMPLEMENTATION_STRING 1
	#endif
	#ifndef SI_NO_OPTIONAL
		#define SI_IMPLEMENTATION_OPTIONAL 1
	#endif
	#ifndef SI_NO_UNICODE
		#define SI_IMPLEMENTATION_UNICODE 1
	#endif
	#ifndef SI_NO_CHAR
		#define SI_IMPLEMENTATION_CHAR 1
	#endif
	#ifndef SI_NO_HASHING
		#define SI_IMPLEMENTATION_HASHING 1
	#endif
	#ifndef SI_NO_MAP
		#define SI_IMPLEMENTATION_MAP 1
	#endif
	#ifndef SI_NO_TIME
		#define SI_IMPLEMENTATION_TIME 1
	#endif
	#ifndef SI_NO_PRINT
		#define SI_IMPLEMENTATION_PRINT 1
	#endif
	#ifndef SI_NO_MATH
		#define SI_IMPLEMENTATION_MATH 1
	#endif
	#ifndef SI_NO_BENCHMARK
		#define SI_IMPLEMENTATION_BENCHMARK 1
	#endif

	#ifndef SI_NO_SYSTEM
		#define SI_IMPLEMENTATION_SYSTEM 1
	#endif
	#ifndef SI_NO_VIRTUAL_MEMORY
		#define SI_IMPLEMENTATION_VIRTUAL_MEMORY 1
	#endif
	#ifndef SI_NO_IO
		#define SI_IMPLEMENTATION_IO 1
	#endif
	#ifndef SI_NO_PRINT_SYSTEM
		#define SI_IMPLEMENTATION_PRINT_SYSTEM
	#endif
	#ifndef SI_NO_THREAD
		#define SI_IMPLEMENTATION_THREAD 1
	#endif
	#ifndef SI_NO_BIT
		#define SI_IMPLEMENTATION_BIT 1
	#endif
	#ifndef SI_NO_CPU
		#define SI_IMPLEMENTATION_CPU 1
	#endif
	#ifndef SI_NO_DLL
		#define SI_IMPLEMENTATION_DLL 1
	#endif
#endif

/* Header definitions that have to be defined in the header declarations but look
 * too ugly to be included in the header. This is basically just a short collection
 * of macro hell, fights with C's lack of, well, everything, as well as battles
 * with the insanities of C++ programming. All of this in the name of making these
 * old languages more usable and pretty... Do not enjoy. */
#if 1

#if 1

#define SI__VARIADIC_2(name, len, ...) SI__VARIADIC_3(name, len, __VA_ARGS__)
#define SI__VARIADIC_3(name, len, ...) name##_##len(__VA_ARGS__)

#define si__ma_0()  SI_TYPE_ZERO(siAny)
#define si__ma_1(a) SI_COMP_LIT(siAny, si_typeid(typeof(a)), SI_PTR(typeof(a), a))
#define si__ma_2(a, b) si__ma_1(a), si__ma_1(b)
#define si__ma_3(a, b, c) si__ma_2(a, b), si__ma_1(c)
#define si__ma_4(a, b, c, d) si__ma_3(a, b, c), si__ma_1(d)
#define si__ma_5(a, b, c, d, e) si__ma_4(a, b, c, d), si__ma_1(e)
#define si__ma_6(a, b, c, d, e, f) si__ma_5(a, b, c, d, e), si__ma_1(f)
#define si__ma_7(a, b, c, d, e, f, g) si__ma_6(a, b, c, d, e, f), si__ma_1(g)
#define si__ma_8(a, b, c, d, e, f, g, h) si__ma_7(a, b, c, d, e, f, g), si__ma_1(h)
#define si__ma_9(a, b, c, d, e, f, g, h, i) si__ma_8(a, b, c, d, e, f, g, h), si__ma_1(i)
#define si__ma_10(a, b, c, d, e, f, g, h, i, j) si__ma_9(a, b, c, d, e, f, g, h, i), si__ma_1(j)
#define si__ma_11(a, b, c, d, e, f, g, h, i, j, k) si__ma_10(a, b, c, d, e, f, g, h, i, j), si__ma_1(k)
#define si__ma_12(a, b, c, d, e, f, g, h, i, j, k, l) si__ma_11(a, b, c, d, e, f, g, h, i, j, k), si__ma_1(l)
#define si__ma_13(a, b, c, d, e, f, g, h, i, j, k, l, m) si__ma_12(a, b, c, d, e, f, g, h, i, j, k, l), si__ma_1(m)
#define si__ma_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) si__ma_13(a, b, c, d, e, f, g, h, i, j, k, l, m), si__ma_1(n)
#define si__ma_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) si__ma_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n), si__ma_1(o)
#define si__ma_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) si__ma_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o), si__ma_1(p)

/* TODO(EimaMei): Optimize this. */
#define si__ba_2(a, b) SI_BIT(a##_##b)
#define si__ba_3(a, b, c) si__ba_2(a, b) | si__ba_2(a, c)
#define si__ba_4(a, b, c, d) si__ba_3(a, b, c) | si__ba_2(a, d)
#define si__ba_5(a, b, c, d, e) si__ba_4(a, b, c, d) | si__ba_2(a, e)
#define si__ba_6(a, b, c, d, e, f) si__ba_5(a, b, c, d, e) | si__ba_2(a, f)
#define si__ba_7(a, b, c, d, e, f, g) si__ba_6(a, b, c, d, e, f) | si__ba_2(a, g)
#define si__ba_8(a, b, c, d, e, f, g, h) si__ba_7(a, b, c, d, e, f, g) | si__ba_2(a, h)
#define si__ba_9(a, b, c, d, e, f, g, h, i) si__ba_8(a, b, c, d, e, f, g, h) | si__ba_2(a, i)
#define si__ba_10(a, b, c, d, e, f, g, h, i, j) si__ba_9(a, b, c, d, e, f, g, h, i) | si__ba_2(a, j)
#define si__ba_11(a, b, c, d, e, f, g, h, i, j, k) si__ba_10(a, b, c, d, e, f, g, h, i, j) | si__ba_2(a, k)
#define si__ba_12(a, b, c, d, e, f, g, h, i, j, k, l) si__ba_11(a, b, c, d, e, f, g, h, i, j, k) | si__ba_2(a, l)
#define si__ba_13(a, b, c, d, e, f, g, h, i, j, k, l, m) si__ba_12(a, b, c, d, e, f, g, h, i, j, k, l) | si__ba_2(a, m)
#define si__ba_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) si__ba_13(a, b, c, d, e, f, g, h, i, j, k, l, m) | si__ba_2(a, n)
#define si__ba_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) si__ba_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) | si__ba_2(a, o)
#define si__ba_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) si__ba_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) | si__ba_2(a, p)


#define SI__DEXP_44(...)               __VA_ARGS__
#define SI__DEXP_45(a, ...)            __VA_ARGS__
#define SI__DEXP_46(a, b, ...)         __VA_ARGS__
#define SI__DEXP_47(a, b, c, ...)      __VA_ARGS__
#define SI__DEXP_48(a, b, c, d, ...)   __VA_ARGS__

#define SI__DEXP_33(...)               __VA_ARGS__
#define SI__DEXP_34(a, ...)            __VA_ARGS__
#define SI__DEXP_35(a, b, ...)         __VA_ARGS__
#define SI__DEXP_36(a, b, c, ...)      __VA_ARGS__
#define SI__DEXP_37(a, b, c, d, ...)   __VA_ARGS__

#define SI__DEXP_22(...)               __VA_ARGS__
#define SI__DEXP_23(a, ...)            __VA_ARGS__
#define SI__DEXP_24(a, b, ...)         __VA_ARGS__
#define SI__DEXP_25(a, b, c, ...)      __VA_ARGS__
#define SI__DEXP_26(a, b, c, d, ...)   __VA_ARGS__

#define SI__DEXP_11(...)               __VA_ARGS__
#define SI__DEXP_12(a, ...)            __VA_ARGS__
#define SI__DEXP_13(a, b, ...)         __VA_ARGS__
#define SI__DEXP_14(a, b, c, ...)      __VA_ARGS__
#define SI__DEXP_15(a, b, c, d, ...)   __VA_ARGS__


/* TODO */
#define SI_COMMA_6(_default, required, in, ...) SI__COMMA_EX(_default, required, in, (__VA_ARGS__), __VA_ARGS__, 0, 1, 1, 1, 1, 1, 1)
#define SI_COMMA_5(_default, required, in, ...) SI__COMMA_EX(_default, required, in, (__VA_ARGS__), __VA_ARGS__, 1, 0, 1, 1, 1, 1, 1)
#define SI_COMMA_4(_default, required, in, ...) SI__COMMA_EX(_default, required, in, (__VA_ARGS__), __VA_ARGS__, 1, 1, 0, 1, 1, 1, 1)
#define SI_COMMA_3(_default, required, in, ...) SI__COMMA_EX(_default, required, in, (__VA_ARGS__), __VA_ARGS__, 1, 1, 1, 0, 1, 1, 1)
#define SI_COMMA_2(_default, required, in, ...) SI__COMMA_EX(_default, required, in, (__VA_ARGS__), __VA_ARGS__, 1, 1, 1, 1, 0, 1, 1)
#define SI_COMMA_1(_default, required, in, ...) SI__COMMA_EX(_default, required, in, (__VA_ARGS__), __VA_ARGS__, 1, 1, 1, 0, 1, 0, 1)
#define SI_COMMA_0(_default, required, in, ...) SI__COMMA_EX(_default, required, in, (__VA_ARGS__), __VA_ARGS__, 1, 1, 1, 0, 1, 1, 0)

#define SI__COMMA_EX(_default, required, in, args, _6,_5, _4, _3, _2, _1, Count, ...) SI_IFCOMMA_##Count(args, required, in, _default)

#define SI_IFCOMMA_1(args, required, in, _default) SI_EXP args, SI__DEXP_EX(required, in, SI_EXP _default)
#define SI_IFCOMMA_0(args, required, in, _default) SI_EXP args


#define SI__DEXP_EX(required, in, ...) SI__DEXP_##required##in(__VA_ARGS__)

#define SI__DARG_IMPL_2(_default, required, total, in, ...) SI__DARG_IMPL_3(_default, required, total, in, __VA_ARGS__)
#define SI__DARG_IMPL_3(_default, required, total, in, ...) SI_COMMA_##total(_default, required, in, __VA_ARGS__)

#define SI_EXP(...) __VA_ARGS__

#define si__repeat_1 1
#define si__repeat_2 si__repeat_1, 1
#define si__repeat_3 si__repeat_2, 1
#define si__repeat_4 si__repeat_3, 1
#define si__repeat_5 si__repeat_4, 1
#define si__repeat_6 si__repeat_5, 1

#endif

#if SI_LANGUAGE_IS_C

#ifndef SI_NO_ALLOCATOR

#define si_alloc(...)                 SI_DARG_IMPL(si_alloc,                 2, (nil), __VA_ARGS__)
#define si_allocAlign(...)            SI_DARG_IMPL(si_allocAlign,            3, (nil), __VA_ARGS__)

#define si_allocNonZeroed(...)        SI_DARG_IMPL(si_allocNonZeroed,        2, (nil), __VA_ARGS__)
#define si_allocNonZeroedAlign(...)   SI_DARG_IMPL(si_allocNonZeroedAlign,   3, (nil), __VA_ARGS__)

#define si_realloc(...)               SI_DARG_IMPL(si_realloc,               4, (nil), __VA_ARGS__)
#define si_reallocAlign(...)          SI_DARG_IMPL(si_reallocAlign,          5, (nil), __VA_ARGS__)

#define si_reallocNonZeroed(...)      SI_DARG_IMPL(si_reallocNonZeroed,      4, (nil), __VA_ARGS__)
#define si_reallocNonZeroedAlign(...) SI_DARG_IMPL(si_reallocNonZeroedAlign, 5, (nil), __VA_ARGS__)


#define si_allocatorMemAvailable(...) SI_DARG_IMPL(si_allocatorMemAvailable, 1, (nil), __VA_ARGS__)

#endif

#ifndef SI_NO_PRINT

#ifndef SI_NO_IO

#define si_fileCreate(...)   SI_DARG_IMPL(si_fileCreate,   1, (nil), __VA_ARGS__)
#define si_fileOpen(...)     SI_DARG_IMPL(si_fileOpen,     1, (nil), __VA_ARGS__)
#define si_fileOpenMode(...) SI_DARG_IMPL(si_fileOpenMode, 2, (nil), __VA_ARGS__)

#define si_fileRead(...)     SI_DARG_IMPL(si_fileRead,     2, (nil), __VA_ARGS__)
#define si_fileReadAt(...)   SI_DARG_IMPL(si_fileReadAt, 3, (nil), __VA_ARGS__)


#define si_fileReadContents(...)    SI_DARG_IMPL(si_fileReadContents,    2, (nil), __VA_ARGS__)
#define si_fileReadContentsStr(...) SI_DARG_IMPL(si_fileReadContentsStr, 2, (nil), __VA_ARGS__)
#define si_fileReadlines(...)       SI_DARG_IMPL(si_fileReadlines,       2, (nil), __VA_ARGS__)

#define si_fileWrite(...)     SI_DARG_IMPL(si_fileWrite,     2, (nil), __VA_ARGS__)
#define si_fileWriteAt(...)   SI_DARG_IMPL(si_fileWriteAt,   3, (nil), __VA_ARGS__)
#define si_fileWriteByte(...) SI_DARG_IMPL(si_fileWriteByte, 2, (nil), __VA_ARGS__)
#define si_fileWritePtr(...)  SI_DARG_IMPL(si_fileWritePtr,  3, (nil), __VA_ARGS__)
#define si_fileWriteStr(...)  SI_DARG_IMPL(si_fileWriteStr,  2, (nil), __VA_ARGS__)

#define si_fileTell(...) SI_DARG_IMPL(si_fileTell, 1, (nil), __VA_ARGS__)
#define si_fileSeek(...) SI_DARG_IMPL(si_fileSeek, 3, (nil), __VA_ARGS__)

#define si_pathReadContents(...)    SI_DARG_IMPL(si_pathReadContents,    2, (nil), __VA_ARGS__)
#define si_pathReadContentsStr(...) SI_DARG_IMPL(si_pathReadContentsStr, 2, (nil), __VA_ARGS__)
#define si_pathReadlines(...)       SI_DARG_IMPL(si_pathReadlines,       2, (nil), __VA_ARGS__)


#endif

#endif

#endif


#ifndef SI_NO_ARRAY

#if SI_LANGUAGE_IS_C
	#define SI__ARR_IMPL(type, ...) SI_ARR_LEN(((type[]){__VA_ARGS__}), countof((type[]){__VA_ARGS__}))
	#define SI__ARR_ALLOC_IMPL(type, alloc, ...) si__arrayMake(si_sizeof(type), (type[]){__VA_ARGS__}, countof((type[]){__VA_ARGS__}), alloc)
	#define SI__DYNARR_ALLOC_IMPL(type, alloc, ...) si_dynamicArrayMakeEx((type[]){__VA_ARGS__}, si_sizeof(type), countof((type[]){__VA_ARGS__}), alloc)

#elif SI_LANGUAGE_IS_CPP
	#define SI__ARR_IMPL(type, ...) ([&]() -> siArrayAny { \
		static type temp[] = {__VA_ARGS__}; \
		return {countof(temp), temp, sizeof(type)}; \
	})()

	#define SI__ARR_ALLOC_IMPL(type, alloc, ...) ([&]() -> siArrayAny { \
		static type temp[] = {__VA_ARGS__}; \
		return si__arrayMake(si_sizeof(type), temp, countof(temp), alloc);  \
	})()

	#define SI__DYNARR_ALLOC_IMPL(type, alloc, ...) ([&]() -> siDynamicArrayAny { \
		static type temp[] = {__VA_ARGS__}; \
		return si_dynamicArrayMakeEx(temp, si_sizeof(type), countof(temp), alloc);  \
	})()

#endif


#if SI_LANGUAGE_IS_CPP
u8& siArrayAny::operator[](isize index) {
	return *static_cast<u8*>(si_arrayGet(*this, index));
}
siArrayAny siArrayAny::operator()(isize offset1, isize offset2) {
	return si_slice(*this, offset1, offset2);
}
siArrayAny siArrayAny::from(isize offset1) {
	return si_sliceFrom(*this, offset1);
}
siArrayAny siArrayAny::to(isize offset2) {
	return si_sliceTo(*this, offset2);
}
siArrayAny siArrayAny::sliceLen(isize offset1, isize length) {
	return si_sliceLen(*this, offset1, length);
}

u8& siDynamicArrayAny::operator[](isize index)	const {
	return *static_cast<u8*>(si_dynamicArrayGet(*this, index));
}
#endif

force_inline
bool si__forEachBuf(isize i, siArrayAny* array, void* value) {
	SI_ASSERT_ARR(*array);

	if (i < array->len) {
		si_memcopy(value, si_arrayGet(*array, i), array->typeSize);
		return true;
	}

	return false;
}

force_inline
bool si__forEachRefBuf(isize i, siArrayAny array, void* value) {
	SI_ASSERT_ARR(array);

	if (i < array.len) {
		*(void**)value = si_arrayGet(array, i);
		return true;
	}

	return false;
}

force_inline
bool si__forEachRevBuf(isize i, siArrayAny array, void* value) {
	SI_ASSERT_ARR(array);

	if (i >= 0) {
		si_memcopy(value, si_arrayGet(array, i), array.typeSize);
		return true;
	}

	return false;
}

force_inline
bool si__forEachRevRefBuf(isize i, siArrayAny array, void* value) {
	SI_ASSERT_ARR(array);

	if (i >= 0) {
		*(void**)value = si_arrayGet(array, i);
		return true;
	}

	return false;
}

force_inline
siArrayAny si__arrayMake(isize typeSizeof, void* ptr, isize len, siAllocator alloc) {
	void* dst = si_allocNonZeroed(alloc, typeSizeof * len);
	if (dst == nil) { return SI_ARR_EX(nil, 0, typeSizeof); }

	si_memcopy(dst, ptr, typeSizeof * len);
	return SI_ARR_EX(dst, len, typeSizeof);
}

#endif

#if SI_LANGUAGE_IS_C

#ifndef SI_NO_ARRAY
	#define si_arraySet(...)        SI_DARG_IMPL(si_arraySet,        3, (1), __VA_ARGS__)
	#define si_dynamicArraySet(...)       SI_DARG_IMPL(si_dynamicArraySet, 3, (1), __VA_ARGS__)

	#define si_dynamicArrayAppend(...) SI_DARG_IMPL(si_dynamicArrayAppend,        2, (1), __VA_ARGS__)
	#define si_dynamicArrayInsert(...) SI_DARG_IMPL(si_dynamicArrayInsert,        3, (1), __VA_ARGS__)
#endif

#ifndef SI_NO_STREAM
	#define si_streamWrite(...)     SI_DARG_IMPL(si_streamWrite,     2, (nil), __VA_ARGS__)
	#define si_streamWriteStr(...)  SI_DARG_IMPL(si_streamWriteStr,  2, (nil), __VA_ARGS__)
	#define si_streamWriteByte(...) SI_DARG_IMPL(si_streamWriteByte, 2, (nil), __VA_ARGS__)
	#define si_streamWriteInt(...)  SI_DARG_IMPL(si_streamWriteInt,  2, (nil), __VA_ARGS__)
#endif

#ifndef SI_NO_STRING
	#define si_builderMakeLen(...)        SI_DARG_IMPL(si_builderMakeLen,  3, (0), __VA_ARGS__)

	#define si_builderWriteStrQuoted(...) SI_DARG_IMPL(si_builderWriteStrQuoted,  2, ('\"'), __VA_ARGS__)
	#define si_builderWriteInt(...)   SI_DARG_IMPL(si_builderWriteInt,   2, (10, 0), __VA_ARGS__)
	#define si_builderWriteUInt(...)  SI_DARG_IMPL(si_builderWriteUInt,  2, (10, 0), __VA_ARGS__)

	#define si_stringToUInt(...)    SI_DARG_IMPL(si_stringToUInt,    1, (-1, nil), __VA_ARGS__)
	#define si_stringToInt(...)     SI_DARG_IMPL(si_stringToInt,     1, (-1, nil), __VA_ARGS__)
	#define si_stringFromInt(...)   SI_DARG_IMPL(si_stringFromInt,   2, (10, 0), __VA_ARGS__)
	#define si_stringFromUInt(...)  SI_DARG_IMPL(si_stringFromUInt,  2, (10, 0), __VA_ARGS__)
#endif

#endif

#ifndef SI_NO_UNICODE

SIDEF siUtf32Char si__stringLastRune(siString str);

force_inline
bool si__forEachStr(isize i, siString str, siRune* rune, isize* codepointSize) {
	SI_ASSERT_STR(str);

	if (i < str.len) {
		siUtf32Char res = si_utf8Decode(&str.data[i]);
		*rune = res.codepoint;
		*codepointSize = res.len;
		return true;
	}

	return false;
}

force_inline
bool si__forEachRevStr(siString str, siRune* rune, isize* index) {
	SI_ASSERT_STR(str);
	if (*index <= 0) { return false; }

	siUtf32Char res = si__stringLastRune(si_substrTo(str, *index));
	*rune = res.codepoint;
	*index -= res.len;

	return true;
}

#endif

#ifndef SI_NO_MAP

#ifdef SI_LANGUAGE_IS_C
	#define si__x struct
#else
	#define si__x struct x
#endif

#define SI__MAP_IMPL(alloc, type, ...) \
	si_mapMakeFull( \
		SI_PTR_WITH_LEN(si__x { siString str; type t; }, __VA_ARGS__), \
		si_sizeof(siString) + si_sizeof(type) + (si_sizeof(type) % alignof(siString) == 0 ? 0 : alignof(siString) - (si_sizeof(type) % alignof(siString))), \
		si_sizeof(type), alloc \
	)

force_inline
bool si__forEachMap(isize i, siMapAny map, siString* key) {
	SI_ASSERT_NOT_NIL(map.entries);

	if (i < map.len) {
		*key = map.entries[i].key;
		return true;
	}

	return false;
}

force_inline
bool si__forEachMapEx(isize i, siMapAny map, siString* key, void* value) {
	SI_ASSERT_NOT_NIL(map.entries);
	SI_ASSERT_NOT_NIL(map.values);

	if (i < map.len) {
		*key = map.entries[i].key;
		si_memcopy(value, si_pointerAdd(map.values, i * map.typeSize), map.typeSize);
		return true;
	}

	return false;
}

force_inline
bool si__forEachRefMap(isize i, siMapAny map, siString* key, void* value) {
	SI_ASSERT_NOT_NIL(map.entries);
	SI_ASSERT_NOT_NIL(map.values);

	if (i < map.len) {
		*key = map.entries[i].key;
		*(void**)value = si_pointerAdd(map.values, i * map.typeSize);
		return true;
	}

	return false;
}


SIDEF siMapAny si_mapMakeFull(const void* input, isize len, isize structTypeSize, isize valueTypeSize, siAllocator alloc);

#endif

#ifndef SI_NO_BIT

#define SI_BIT_FUNC(type, name, ...) si__##name##_##type(__VA_ARGS__)

#define SI_BIT_DEC(func, def, body) \
	def isize si__##func##_u8(u8 x) body \
	def isize si__##func##_u16(u16 x) body \
	def isize si__##func##_u32(u32 x) body \
	def isize si__##func##_u64(u64 x) body \
	def isize si__##func##_usize(usize x) body
#define SI_BIT_DEC_TYPE(func, def, body) \
	def u8 si__##func##_u8(u8 x) body \
	def u16 si__##func##_u16(u16 x) body \
	def u32 si__##func##_u32(u32 x) body \
	def u64 si__##func##_u64(u64 x) body \
	def usize si__##func##_usize(usize x) body
#define SI_BIT_DEC_AMOUNT(func, def, body) \
	def u8 si__##func##_u8(u8 x, i32 amount) body \
	def u16 si__##func##_u16(u16 x, i32 amount) body \
	def u32 si__##func##_u32(u32 x, i32 amount) body \
	def u64 si__##func##_u64(u64 x, i32 amount) body \
	def usize si__##func##_usize(usize x, i32 amount) body
#define SI_BIT_DEC_ARRAY(func, def, body) \
	def siArray(u8) si__##func##_u8(u8 x, siAllocator alloc) body \
	def siArray(u8) si__##func##_u16(u16 x, siAllocator alloc) body \
	def siArray(u8) si__##func##_u32(u32 x, siAllocator alloc) body \
	def siArray(u8) si__##func##_u64(u64 x, siAllocator alloc) body \
	def siArray(u8) si__##func##_usize(usize x, siAllocator alloc) body

SI_BIT_DEC(countOnes, SIDEF, ;)

SI_BIT_DEC(countLeadingOnes, SIDEF, ;)
SI_BIT_DEC(countLeadingZeros, SIDEF, ;)
SI_BIT_DEC(countTrailingOnes, SIDEF, ;)
SI_BIT_DEC(countTrailingZeros, SIDEF, ;)

SI_BIT_DEC_AMOUNT(bitsRotateLeft, SIDEF, ;)
SI_BIT_DEC_AMOUNT(bitsRotateRight, SIDEF, ;)
SI_BIT_DEC_TYPE(bitsReverse, SIDEF, ;)
SI_BIT_DEC_ARRAY(bytesToArray, SIDEF, ;)

#undef SI_BIT_DEC_TYPE
#undef SI_BIT_DEC_AMOUNT

#define SI_CHECK_ARITHMETIC_FUNC(type, func, a, b, res) si__check##func##_##type(a, b, res)

#define SI_CHECK_ARITHMETIC_DEC(type, func, def, body) \
	def bool si__check##func##_##type(type a, type b, type* res) body \

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

#ifndef SI_NO_TIME
#define si_timePrint(...) SI_DARG_IMPL(si_timePrint, 1, (SI_STR_EMPTY, SI_CALLER_LOC), __VA_ARGS__)
#define si_timePrintSince(...) SI_DARG_IMPL(si_timePrintSince, 1, (SI_STR_EMPTY, SI_CALLER_LOC), __VA_ARGS__)
#endif

#ifndef SI_NO_MATH
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

	#define SI_MATH_FUNC_DECLARE_3X_bool(name, def, body) \
		def bool si__##name##_u8(u8 a, u8 b, u8 c) body \
		def bool si__##name##_i8(i8 a, i8 b, i8 c) body \
		def bool si__##name##_u16(u16 a, u16 b, u16 c) body \
		def bool si__##name##_i16(i16 a, i16 b, i16 c) body \
		def bool si__##name##_u32(u32 a, u32 b, u32 c) body \
		def bool si__##name##_i32(i32 a, i32 b, i32 c) body \
		def bool si__##name##_u64(u64 a, u64 b, u64 c) body \
		def bool si__##name##_i64(i64 a, i64 b, i64 c) body \
		def bool si__##name##_usize(usize a, usize b, usize c) body \
		def bool si__##name##_isize(isize a, isize b, isize c) body \
		def bool si__##name##_f32(f32 a, f32 b, f32 c) body \
		def bool si__##name##_f64(f64 a, f64 b, f64 c) body

	#define SI_MATH_FUNC_DECLARE_1X_FLOAT(name, def, ...) \
		def f32   si__##name##_f32(f32 a) __VA_ARGS__ \
		def f64   si__##name##_f64(f64 a) __VA_ARGS__


	SI_MATH_FUNC_DECLARE_2X(min, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_2X(max, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_SIGNED(abs, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_3X_bool(between, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_FLOAT(floor, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_FLOAT(ceil, SIDEF, ;)
	SI_MATH_FUNC_DECLARE_1X_FLOAT(round, SIDEF, ;)

	#undef SI_MATH_FUNC_DECLARE_1X_SIGNED
	#undef SI_MATH_FUNC_DECLARE_1X_FLOAT

#endif

#ifndef SI_NO_BENCHMARK
SIDEF void si_benchmarkLoopsAvgPrint(siString name, i64 cycles[20], isize len, usize range[2]);
SIDEF void si_benchmarkLoopsAvgCmpPrint(siString names[2], i64 cycles[2][20], isize len, usize range[2]);
SIDEF void* si__benchmarkThread(void* arg);
#endif

#endif

#ifdef SI_IMPLEMENTATION_MEMORY

inline
bool si_isPowerOfTwo(isize x) {
	return !(x & (x - 1)) && x > 0;
}

inline
isize si_alignForward(isize num, isize alignment) {
	return (isize)si_alignForwardU((usize)num, alignment);
}
inline
usize si_alignForwardU(usize num, isize alignment) {
	SI_ASSERT(si_isPowerOfTwo(alignment));
	usize align = (usize)alignment;
	return (num + align - 1) & ~(align - 1);
}

SIDEF
isize si_nextPow2(isize num) {
	SI_ASSERT_NOT_NEG(num);

	num -= 1;
	num |= num >> 1;
	num |= num >> 2;
	num |= num >> 4;
	num |= num >> 8;
	num |= num >> 16;
	#if SI_ARCH_IS_64BIT
	num |= num >> 32;
	#endif

	return num + 1;
}


inline void* si_pointerAdd(void* ptr, isize bytes)                  { return (void*)((u8*)ptr + bytes); }
inline void* si_pointerSub(void* ptr, isize bytes)                  { return (void*)((u8*)ptr - bytes); }
inline const void* si_pointerAddConst(const void* ptr, isize bytes) { return (void*)((const u8*)ptr + bytes); }
inline const void* si_pointerSubConst(const void* ptr, isize bytes) { return (void*)((const u8*)ptr - bytes); }
inline isize si_pointerDiff(const void* begin, const void* end)     { return (isize)((const u8*)end - (const u8*)begin); }
inline bool si_pointerBetween(const void* ptr, const void* start, const void* end) { return (ptr >= start) && (ptr <= end); }


SIDEF
isize si_memcopy(void* restrict dst, const void* restrict src, isize size) {
	SI_ASSERT_NOT_NIL(dst);
	SI_ASSERT_NOT_NIL(src);
	SI_ASSERT_NOT_NEG(size);

#ifndef SI_NO_CRT
	/* NOTE(EimaMei): C99/C++ standards define 'memcpy' should do nothing if a 
	zero size is requested, meaning we don't have to check that for ourselves. */
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
isize si_cstrLen(cstring str) {
	SI_ASSERT_NOT_NIL(str);

#ifndef SI_NO_CRT
	return (isize)strlen(str);
#else
	isize i = 0;
	while (str[i] != '\0') { i += 1; }

	return i;
#endif

}


#endif /* SI_IMPLEMENTATION_MEMORY */

#ifdef SI_IMPLEMENTATION_ALLOCATOR


inline
void* si_alloc(siAllocator alloc, isize bytes, siAllocationError* outError) {
	return si_allocAlign(alloc, bytes, SI_DEFAULT_ALIGNMENT, outError);
}

inline
void* si_allocAlign(siAllocator alloc, isize bytes, isize align, siAllocationError* outError) {
	SI_ASSERT_NOT_NEG(bytes);
	SI_ASSERT(si_isPowerOfTwo(align));

	siAllocationError tmp;
	if (outError == nil) { outError = &tmp; }

	return alloc.proc(siAllocationType_Alloc, nil, 0, bytes, align, outError, alloc.data);
}


inline
void* si_allocNonZeroed(siAllocator alloc, isize bytes, siAllocationError* outError) {
	return si_allocNonZeroedAlign(alloc, bytes, SI_DEFAULT_ALIGNMENT, outError);
}

inline
void* si_allocNonZeroedAlign(siAllocator alloc, isize bytes, isize align, siAllocationError* outError) {
	SI_ASSERT_NOT_NEG(bytes);
	SI_ASSERT(si_isPowerOfTwo(align));

	siAllocationError tmp;
	if (outError == nil) { outError = &tmp; }

	return alloc.proc(siAllocationType_AllocNonZeroed, nil, 0, bytes, align, outError, alloc.data);
}


inline
siAllocationError si_free(siAllocator alloc, void* ptr) {
	if (ptr == nil) { return siAllocationError_InvalidArg; }

	siAllocationError err;
	alloc.proc(siAllocationType_Free, ptr, 0, 0, 0, &err, alloc.data);

	return err;
}

inline
siAllocationError si_freeAll(siAllocator alloc) {
	siAllocationError err;
	alloc.proc(siAllocationType_FreeAll, nil, 0, 0, 0, &err, alloc.data);

	return err;
}


inline
void* si_realloc(siAllocator alloc, void* ptr, isize oldSize, isize newSize,
		siAllocationError* outError) {
	return si_reallocAlign(alloc, ptr, oldSize, newSize, SI_DEFAULT_ALIGNMENT, outError);
}

inline
void* si_reallocAlign(siAllocator alloc, void* ptr, isize oldSize, isize newSize,
		isize align, siAllocationError* outError) {
	SI_ASSERT_NOT_NIL(ptr);
	SI_ASSERT_NOT_NEG(oldSize);
	SI_ASSERT_NOT_NEG(newSize);
	SI_ASSERT(si_isPowerOfTwo(align));

	siAllocationError tmp;
	if (outError == nil) { outError = &tmp; }

	return alloc.proc(siAllocationType_Resize, ptr, oldSize, newSize, align, outError, alloc.data);
}


inline
void* si_reallocNonZeroed(siAllocator alloc, void* ptr, isize oldSize, isize newSize,
		siAllocationError* outError) {
	return si_reallocNonZeroedAlign(alloc, ptr, oldSize, newSize, SI_DEFAULT_ALIGNMENT, outError);
}

inline
void* si_reallocNonZeroedAlign(siAllocator alloc, void* ptr, isize oldSize, isize newSize,
		isize align, siAllocationError* outError) {
	SI_ASSERT_NOT_NIL(ptr);
	SI_ASSERT_NOT_NEG(oldSize);
	SI_ASSERT_NOT_NEG(newSize);
	SI_ASSERT(si_isPowerOfTwo(align));

	siAllocationError tmp;
	if (outError == nil) { outError = &tmp; }


	return alloc.proc(siAllocationType_ResizeNonZeroed, ptr, oldSize, newSize, align, outError, alloc.data);
}


SIDEF
isize si_allocatorMemAvailable(siAllocator alloc, siAllocationError* outError) {
	siAllocationError tmp;
	if (outError == nil) { outError = &tmp; }

	isize res = (isize)alloc.proc(siAllocationType_MemAvailable, nil, 0, 0, 0, outError, alloc.data);
	return (*outError == 0) ? res : -1;
}

inline
u8 si_allocatorGetFeatures(siAllocator alloc) {
	void* res = alloc.proc(siAllocationType_GetFeatures, nil, 0, 0, 0, nil, alloc.data);
	return transmute(u8, res);
}

inline
bool si_allocatorHasFeature(u8 features, siAllocationType type) {
	SI_ASSERT(si_between(i32, type, 0, siAllocationType_Len - 1));
	return (features & SI_BIT(type)) != 0;
}



inline
siAllocator si_allocatorHeap(void) {
	siAllocator alloc;
	alloc.proc = si_allocatorHeap_proc;
	alloc.data = nil;

	return alloc;
}

#ifndef SI_NO_CRT
SIDEF
SI_ALLOCATOR_PROC(si_allocatorHeap_proc) {
	SI_UNUSED(allocatorData);

	switch (type) {
		case siAllocationType_Alloc: {
			newSize = si_alignForward(newSize, align);

			void* out = malloc((usize)newSize);
			if (out == nil) { *outError = siAllocationError_OutOfMem; return nil; }

			si_memset(out, 0, newSize);
			*outError = siAllocationError_None;

			return out;
		}

		case siAllocationType_AllocNonZeroed: {
			newSize = si_alignForward(newSize, align);
			void* out = malloc((usize)newSize);

			*outError = (out == nil)
				? siAllocationError_OutOfMem
				: siAllocationError_None;
			return out;
		}

		case siAllocationType_Free: {
			free(ptr);
			*outError = 0;
			return nil;
		}

		case siAllocationType_Resize: {
			newSize = si_alignForward(newSize, align);

			void* out = realloc(ptr, (usize)newSize);
			if (out == nil) { *outError = siAllocationError_OutOfMem; return nil; }

			si_memset((u8*)out + oldSize, 0, newSize - oldSize);
			*outError = 0;

			return out;
		}

		case siAllocationType_ResizeNonZeroed: {
			newSize = si_alignForward(newSize, align);
			void* out = realloc(ptr, (usize)newSize);

			*outError = (out == nil)
				? siAllocationError_OutOfMem
				: siAllocationError_None;
			return out;
		}

		case siAllocationType_GetFeatures: {
			u8 features = SI_BITMASK_MAKE(siAllocationType,
				Alloc, AllocNonZeroed, Resize, ResizeNonZeroed, GetFeatures
			);
			return transmute(void*, features);
		}
	}

	*outError = siAllocationError_NotImplemented;
	return nil;
}
#endif

inline
siArena si_arenaMake(siAllocator alloc, isize capacity) {
	return si_arenaMakeEx(alloc, capacity, SI_DEFAULT_ALIGNMENT);
}
SIDEF
siArena si_arenaMakeEx(siAllocator alloc, isize capacity, isize align) {
	SI_ASSERT_NOT_NEG(capacity);

	siArena out = SI_STRUCT_ZERO;
	out.alloc = alloc;
	out.capacity = capacity;
	out.ptr = si_allocArrayAlign(out.alloc, u8, out.capacity, align);

	return out;
}
inline
siArena si_arenaMakePtr(void* ptr, isize capacity) {
	SI_ASSERT_NOT_NEG(capacity);

	siArena out = SI_STRUCT_ZERO;
	out.capacity = capacity;
	out.ptr = (u8*)ptr;

	return out;
}


inline
siAllocator si_allocatorArena(siArena* arena) {
	siAllocator alloc;
	alloc.data = arena;
	alloc.proc = si_allocatorArena_proc;
	return alloc;
}

SIDEF
void si_arenaFree(siArena* arena) {
	si_free(arena->alloc, arena->ptr);
	arena->ptr = nil;
	arena->offset = 0;
	arena->capacity = 0;
}


siIntern
void* si__arenaAlloc(siArena* arena, isize size, isize align, siAllocationError* outError) {
	isize bytes = si_alignForward(size, align);
	isize newOffset = arena->offset + bytes;

	if (newOffset > arena->capacity) { *outError = siAllocationError_OutOfMem; return nil; }

	void* out = &arena->ptr[arena->offset];
	arena->offset = newOffset;

	*outError = 0;
	return out;
}

siIntern
void* si__arenaResize(siArena* arena, void* ptr, isize oldSize, isize newSize,
		isize align, siAllocationError* outError) {
	void* out = si_allocNonZeroedAlign(si_allocatorArena(arena), newSize, align, outError);
	if (out != nil) {
		si_memcopy(out, ptr, oldSize);
	}

	return out;
}

SIDEF
SI_ALLOCATOR_PROC(si_allocatorArena_proc) {
	siArena* arena = (siArena*)allocatorData;
	SI_ASSERT_MSG(arena->ptr != nil, "You cannot use an already freed arena.");

	switch (type) {
		case siAllocationType_Alloc: {
			void* out = si__arenaAlloc(arena, newSize, align, outError);
			if (out) { si_memset(out, 0, newSize); }
			return out;
		}

		case siAllocationType_AllocNonZeroed: {
			return si__arenaAlloc(arena, newSize, align, outError);
		}

		case siAllocationType_FreeAll: {
			arena->offset = 0;
			*outError = 0;
			return nil;
		}

		case siAllocationType_Resize: {
			void* out = si__arenaResize(arena, ptr, oldSize, newSize, align, outError);
			if (out) { si_memset((u8*)out + oldSize, 0, newSize - oldSize); }
			return out;
		}

		case siAllocationType_ResizeNonZeroed: {
			return si__arenaResize(arena, ptr, oldSize, newSize, align, outError);
		}

		case siAllocationType_MemAvailable: {
			*outError = 0;
			return (void*)(arena->capacity - arena->offset);
		} break;

		case siAllocationType_GetFeatures: {
			u8 features = SI_BITMASK_MAKE(siAllocationType,
				Alloc, AllocNonZeroed, FreeAll, Resize, ResizeNonZeroed,
				MemAvailable, GetFeatures
			);

			return transmute(void*, features);
		}
	}

	*outError = siAllocationError_NotImplemented;
	return nil;
}


inline
siArenaTmp si_arenaTmpStart(siArena* arena) {
	siArenaTmp tmp;
	tmp.arena = arena;
	tmp.offset = arena->offset;
	return tmp;
}
inline
void si_arenaTmpEnd(siArenaTmp tmp) {
	tmp.arena->offset = tmp.offset;
}



inline
siLifo si_lifoMake(siAllocator alloc, isize capacity) {
	return si_lifoMakeEx(alloc, capacity, SI_DEFAULT_ALIGNMENT);
}
SIDEF
siLifo si_lifoMakeEx(siAllocator alloc, isize capacity, isize align) {
	SI_ASSERT_NOT_NEG(capacity);

	siLifo lifo = SI_STRUCT_ZERO;
	lifo.alloc = alloc;
	lifo.capacity = capacity;
	lifo.ptr = si_allocArrayNonZeroedAlign(lifo.alloc, u8, lifo.capacity, align);

	return lifo;
}
inline
siLifo si_lifoMakePtr(void* ptr, isize capacity) {
	SI_ASSERT_NOT_NEG(capacity);

	siLifo lifo = SI_STRUCT_ZERO;
	lifo.capacity = capacity;
	lifo.ptr = (u8*)ptr;

	return lifo;
}


inline
siAllocator si_allocatorLifo(siLifo* lifo) {
	siAllocator alloc;
	alloc.data = lifo;
	alloc.proc = si_allocatorLifo_proc;
	return alloc;
}

SIDEF
void si_lifoFree(siLifo* lifo) {
	si_free(lifo->alloc, lifo->ptr);
	lifo->ptr = nil;
	lifo->capacity = 0;
}


siIntern
void* si__lifoAlloc(siLifo* lifo, isize size, isize align, siAllocationError* outError) {
	isize bytes = si_alignForward(si_sizeof(isize) + size, align);
	isize newOffset = lifo->offset + bytes;

	if (newOffset > lifo->capacity) { *outError = siAllocationError_OutOfMem; return nil; }

	void* out = &lifo->ptr[lifo->offset];
	*(isize*)out = lifo->offset;

	lifo->offset = newOffset;
	*outError = 0;

	return si_pointerAdd(out, si_sizeof(isize));
}

SIDEF
SI_ALLOCATOR_PROC(si_allocatorLifo_proc) {
	siLifo* lifo = (siLifo*)allocatorData;
	SI_ASSERT_MSG(lifo->ptr != nil, "You cannot use an already freed LIFO allocator.");

	/* TODO(EimaMei): Remove out. */
	void* out;
	switch (type) {
		case siAllocationType_Alloc: {
			out = si__lifoAlloc(lifo, newSize, align, outError);
			if (out) { si_memset(out, 0, newSize); }
		} break;

		case siAllocationType_AllocNonZeroed: {
			out = si__lifoAlloc(lifo, newSize, align, outError);
		} break;


		case siAllocationType_Free: {
			if (si_pointerBetween(ptr, lifo->ptr, &lifo->ptr[lifo->offset]) ) {
				*outError = siAllocationError_InvalidPtr;
				return nil;
			}

			lifo->offset = *(isize*)si_pointerSub(ptr, si_sizeof(isize));
			out = nil;
		} break;

		case siAllocationType_FreeAll: {
			lifo->offset = 0;
			out = nil;
		} break;

		case siAllocationType_Resize: {
			out = si__arenaResize(lifo, ptr, oldSize, newSize, align, outError);
			if (out && oldSize < newSize) { si_memset((u8*)out + oldSize, 0, newSize - oldSize); }
		} break;

		case siAllocationType_ResizeNonZeroed: {
			out = si__arenaResize(lifo, ptr, oldSize, newSize, align, outError);
		} break;

		case siAllocationType_MemAvailable: {
			out = (void*)(lifo->capacity - lifo->offset);
			*outError = 0;
		} break;

		case siAllocationType_GetFeatures: {
			u8 features = SI_BITMASK_MAKE(siAllocationType,
				Alloc, AllocNonZeroed, Free, FreeAll, Resize, ResizeNonZeroed,
				MemAvailable, GetFeatures
			);

			out = transmute(void*, features);
		} break;
	}

	return out;
}


inline
siPool si_poolMake(siAllocator alloc, isize numChunks, isize chunkSize) {
	return si_poolMakeEx(alloc, numChunks, chunkSize, SI_DEFAULT_ALIGNMENT);
}
SIDEF
siPool si_poolMakeEx(siAllocator alloc, isize numChunks, isize chunkSize, isize align) {
	SI_ASSERT_NOT_NEG(numChunks);
	SI_ASSERT_NOT_NEG(chunkSize);


	siPool pool;
	pool.alloc = alloc;
	pool.numChunks = numChunks;
	pool.chunkSize = chunkSize;
	pool.head = nil;

	isize totalChunkSize = si_sizeof(siPoolFreeNode*) + pool.chunkSize;
	pool.ptr = si_allocArrayNonZeroedAlign(pool.alloc, u8, pool.numChunks * totalChunkSize, align);

	for_range (i, 0, pool.numChunks) {
		void* ptr = &pool.ptr[i * pool.chunkSize];
		siPoolFreeNode* node = (siPoolFreeNode*)ptr;

		node->next = pool.head;
		pool.head = node;
	}

	return pool;

}

inline
siAllocator si_allocatorPool(siPool* pool) {
	siAllocator alloc;
	alloc.data = pool;
	alloc.proc = si_allocatorPool_proc;
	return alloc;
}


SIDEF
void si_poolFree(siPool* pool) {
	si_free(pool->alloc, pool->ptr);
	pool->ptr = nil;
	pool->numChunks = 0;
}


siIntern
void* si__poolAlloc(siPool* pool, isize size, siAllocationError* outError) {
	if (size > pool->chunkSize) {
		*outError = siAllocationError_InvalidArg;
		return nil;
	}

	void* out = pool->head;
	if (out == nil) { *outError = siAllocationError_OutOfMem; return nil; }

	pool->head = pool->head->next;
	return si_pointerAdd(out, si_sizeof(siPoolFreeNode*));
}

SIDEF
SI_ALLOCATOR_PROC(si_allocatorPool_proc) {
	siPool* pool = (siPool*)allocatorData;
	SI_ASSERT_MSG(pool->ptr != nil, "You cannot use an already freed pool.");
	SI_UNUSED(oldSize); SI_UNUSED(align); /* NOTE(EimaMei): Check the allocator's description. */


	switch (type) {
		case siAllocationType_Alloc: {
			void* out = si__poolAlloc(pool, newSize, outError);
			if (out != nil) { si_memset(out, 0, newSize); }
			return out;
		}

		case siAllocationType_AllocNonZeroed: {
			return si__poolAlloc(pool, newSize, outError);
		}

		case siAllocationType_Free: {
			siPoolFreeNode* node = (siPoolFreeNode*)ptr;
			if (!si_pointerBetween(node, pool->ptr, &pool->ptr[pool->numChunks * (pool->chunkSize + si_sizeof(siPoolFreeNode*))])) {
				*outError = siAllocationError_InvalidPtr;
				return nil;
			}

			node->next = pool->head;
			pool->head = node;

			*outError = 0;
			return nil;
		}

		case siAllocationType_FreeAll: {
			for_range (i, 0, pool->numChunks) {
				void* nodePtr = &pool->ptr[i * (pool->chunkSize + si_sizeof(siPoolFreeNode*))];
				siPoolFreeNode* node = (siPoolFreeNode*)nodePtr;

				node->next = pool->head;
				pool->head = node;
			}

			*outError = 0;
			return nil;
		}

		case siAllocationType_MemAvailable: {
			*outError = 0;
			return (pool->head != nil) ? (void*)pool->chunkSize : 0;
		}

		case siAllocationType_GetFeatures: {
			u8 features = SI_BITMASK_MAKE(siAllocationType,
				Alloc, AllocNonZeroed, Free, FreeAll, MemAvailable, GetFeatures
			);

			return transmute(void*, features);
		}
	}

	*outError = siAllocationError_NotImplemented;
	return nil;
}

inline
siDynamicArena si_dynamicArenaMake(siAllocator alloc, isize startingCapacity,
		isize blockSize) {
	return si_dynamicArenaMakeEx(alloc, startingCapacity, blockSize, SI_DEFAULT_ALIGNMENT);
}
inline
siDynamicArena si_dynamicArenaMakeEx(siAllocator alloc, isize startingCapacity,
		isize blockSize, isize align) {
	SI_ASSERT_NOT_NEG(startingCapacity);
	SI_ASSERT_NOT_NEG(blockSize);

	siDynamicArena dynamic;
	dynamic.arena = si_arenaMakeEx(alloc, startingCapacity, align);
	dynamic.blockSize = blockSize;
	dynamic.head = nil;

	return dynamic;

}

inline
siAllocator si_allocatorDynamicArena(siDynamicArena* dynamic) {
	SI_ASSERT_NOT_NIL(dynamic);

	siAllocator alloc;
	alloc.proc = si_allocatorDynamicArena_proc;
	alloc.data = dynamic;

	return alloc;
}

SIDEF
void si_dynamicArenaFree(siDynamicArena* dynamic) {
	si_arenaFree(&dynamic->arena);

	siDynamicArenaBlock* block = dynamic->head;
	while (block) {
		siDynamicArenaBlock* next = block->next;
		si_free(dynamic->arena.alloc, block);
		block = next;
	}
}

SIDEF
siDynamicArenaTmp si_dynamicArenaTmpStart(siDynamicArena* dynamic) {
	SI_ASSERT_NOT_NIL(dynamic);

	siDynamicArenaTmp tmp;
	tmp.aTmp = si_arenaTmpStart(&dynamic->arena);

	if (dynamic->head != nil) {
		siDynamicArenaBlock* block = dynamic->head;
		siDynamicArenaBlock* next = dynamic->head->next;
		while (next) { next = block->next; }

		tmp.blockOffset = block->offset;
		tmp.block = block;
	}
	else {
		tmp.blockOffset = 0;
		tmp.block = nil;
	}

	return tmp;
}

SIDEF
void si_dynamicArenaTmpEnd(siDynamicArenaTmp tmp) {
	si_arenaTmpEnd(tmp.aTmp);
	SI_STOPIF(tmp.block == nil, return);


	siDynamicArenaBlock* block = tmp.block;
	block->offset = tmp.blockOffset;

	block = block->next;
	while (block) {
		block->offset = 0;
		block = block->next;
	}
}


siIntern
void* si__dynamicArenaAlloc(siDynamicArena* dyn, isize size, isize align,
		siAllocationError* outError) {
	siArena* arena = &dyn->arena;
	isize bytes = (isize)si_alignForward(size, align);
	void* out = &arena->ptr[arena->offset];

	if (arena->offset + bytes > arena->capacity) {
		if (bytes > dyn->blockSize) {
			*outError = siAllocationError_InvalidArg;
			return nil;
		}

		siDynamicArenaBlock* head = nil;
		siDynamicArenaBlock* block = dyn->head;
		while (block && block->offset + bytes > dyn->blockSize) {
			head = block;
			block = block->next;
		}

		if (block == nil) {
			void* newBlockPtr = si_allocNonZeroedAlign(
				arena->alloc, si_sizeof(siDynamicArenaBlock) + dyn->blockSize,
				align, outError
			);

			siDynamicArenaBlock* newBlock = (siDynamicArenaBlock*)newBlockPtr;
			if (newBlock == nil) { *outError = siAllocationError_OutOfMem; return nil; }

			if (head != nil) { head->next = newBlock; }
			else { dyn->head = newBlock; }

			block = newBlock;
			block->ptr = (u8*)(newBlock + 1);
			block->offset = 0;
			block->next = nil;
		}

		out = &block->ptr[block->offset];
		block->offset += bytes;
	}
	else {
		arena->offset += bytes;
	}


	*outError = 0;
	return out;
}

siIntern
void* si__dynamicArenaResize(siDynamicArena* arena, void* ptr, isize oldSize,
		isize newSize, isize align, siAllocationError* outError) {
	if (oldSize >= newSize) { return ptr; }

	void* out = si_allocNonZeroedAlign(si_allocatorDynamicArena(arena), newSize, align, outError);
	if (ptr == nil) { return out; }

	si_memcopy(out, ptr, oldSize);
	return out;
}


SIDEF
SI_ALLOCATOR_PROC(si_allocatorDynamicArena_proc) {
	siDynamicArena* dyn = (siDynamicArena*)allocatorData;
	siArena* arena = &dyn->arena;
	SI_ASSERT_MSG(arena->ptr != nil, "You cannot use an already freed arena.");

	/* TODO(EimaMei): Remove out. */
	void* out;
	switch (type) {
		case siAllocationType_Alloc: {
			out = si__dynamicArenaAlloc(dyn, newSize, align, outError);
			if (out != nil) { si_memset(out, 0, newSize); }
		} break;

		case siAllocationType_AllocNonZeroed: {
			out = si__dynamicArenaAlloc(dyn, newSize, align, outError);
		} break;

		case siAllocationType_Free: {
			*outError = siAllocationError_NotImplemented;
			out = nil;
		} break;

		case siAllocationType_FreeAll: {
			arena->offset = 0;

			siDynamicArenaBlock* block = dyn->head;
			while (block) {
				block->offset = 0;
				block = block->next;
			}

			out = nil;
		} break;

		case siAllocationType_Resize: {
			out = si__dynamicArenaResize(dyn, ptr, oldSize, newSize, align, outError);
			if (out) { si_memset((u8*)out + oldSize, 0, newSize - oldSize); }
		} break;

		case siAllocationType_ResizeNonZeroed: {
			out = si__dynamicArenaResize(dyn, ptr, oldSize, newSize, align, outError);
		} break;

		case siAllocationType_MemAvailable: {
			isize len = arena->capacity - arena->offset;
			if (len < dyn->blockSize) { len = dyn->blockSize; }

			out = (void*)len;
			*outError = 0;
		} break;

		case siAllocationType_GetFeatures: {
			siAllocationType_Flags features = SI_BITMASK_MAKE(siAllocationType,
				Alloc, AllocNonZeroed, FreeAll, Resize, ResizeNonZeroed, MemAvailable, GetFeatures
			);

			return transmute(void*, features);
		} break;

	/* TODO(EimaMei): Remove this. */
		default: SI_PANIC();
	}

	return out;
}

#endif /* SI_IMPLEMENTATION_ALLOCATOR */

#ifdef SI_IMPLEMENTATION_ARRAY

inline
siArrayAny si_slice(siArrayAny array, isize offset1, isize offset2) {
	SI_ASSERT_ARR(array);
	SI_ASSERT_NOT_NEG(offset1);
	SI_ASSERT_NOT_NEG(offset2);
	SI_ASSERT(offset1 <= array.len && offset2 <= array.len);
	SI_ASSERT(offset1 <= offset2);

	siArrayAny res;
	res.data = (u8*)array.data + offset1 * array.typeSize;
	res.len = offset2 - offset1;
	res.typeSize = array.typeSize;

	return res;
}

inline
siArrayAny si_sliceFrom(siArrayAny array, isize offset1) {
	return si_slice(array, offset1, array.len);
}
inline
siArrayAny si_sliceTo(siArrayAny array, isize offset2) {
	return si_slice(array, 0, offset2);
}

inline
siArrayAny si_sliceLen(siArrayAny array, isize offset1, isize len) {
	return si_slice(array, offset1, offset1 + len);
}


inline
void* si_arrayGet(siArrayAny array, isize index) {
	SI_ASSERT_ARR(array);
	SI_ASSERT_FMT(index < array.len, "%i < %i", index, array.len);
	SI_ASSERT_NOT_NEG(index);

	return (u8*)array.data + index * array.typeSize;
}

inline
void* si_arrayFront(siArrayAny array) {
	return si_arrayGet(array, 0);
}

inline
void* si_arrayBack(siArrayAny array) {
	return si_arrayGet(array, array.len - 1);
}

inline
void* si_arraySet(siArrayAny array, isize index, const void* data, isize count) {
	void* ptr = si_arrayGet(array, index);
	si_memcopy(ptr, data, array.typeSize * count);

	return ptr;
}

inline
bool si_arrayAtGet(siArrayAny array, isize index, void* out) {
	SI_ASSERT_ARR(array);
	SI_ASSERT_NOT_NIL(out);

	if (index >= array.len) {
		return false;
	}

	si_memcopy(out, si_arrayGet(array, index), array.typeSize);
	return true;
}

inline
bool si_arrayAtFront(siArrayAny array, void* out) {
	SI_ASSERT_ARR(array);
	SI_STOPIF(array.len == 0, return false);
	si_memcopy(out, array.data, array.typeSize);

	return true;
}

inline
bool si_arrayAtBack(siArrayAny array, void* out) {
	SI_ASSERT_ARR(array);
	SI_STOPIF(array.len == 0, return false);
	si_memcopy(out, si_arrayGet(array, array.len - 1), array.typeSize);

	return true;
}

SIDEF
isize si_arrayFind(siArrayAny array, const void* data) {
	for_range (i, 0, array.len) {
		void* dst = si_arrayGet(array, i);
		if (si_memcompare(dst, data, array.typeSize) == 0) {
			return i;
		}
	}

	return -1;
}

SIDEF
isize si_arrayFindLast(siArrayAny array, const void* data) {
	isize i;
	for (i = array.len - 1; i >= 0; i -= 1) {
		void* dst = si_arrayGet(array, i);
		if (si_memcompare(dst, data, array.typeSize) == 0) {
			return i;
		}
	}

	return -1;
}

SIDEF
isize si_arrayFindCount(siArrayAny array, const void* data) {
	isize occurences = 0;
	for_range (i, 0, array.len) {
		void* element = si_arrayGet(array, i);
		if (si_memcompare(element, data, array.typeSize) == 0) {
			occurences += 1;
		}
	}

	return occurences;

}

SIDEF
bool si_arrayEqual(siArrayAny lhs, siArrayAny rhs) {
	SI_STOPIF(lhs.len != rhs.len, return false);
	SI_STOPIF(lhs.data == rhs.data, return true);

	return si_memcompare(lhs.data, rhs.data, si_min(isize, lhs.len, rhs.len)) == 0;
}

inline
void si_arrayFree(siArrayAny array, siAllocator alloc) {
	si_free(alloc, array.data);
}

inline
siDynamicArrayAny si_dynamicArrayMakeEx(void* list, isize typeSize, isize count, siAllocator alloc)  {
	siDynamicArrayAny array = si_dynamicArrayReserveNonZeroed(typeSize, count, alloc);
	if (array.capacity != 0) {
		si_memcopy(array.data, list, typeSize * count);
		array.len = count;
	}

	return array;
}

SIDEF
siDynamicArrayAny si_dynamicArrayReserveNonZeroed(isize typeSize, isize capacity,
		siAllocator alloc) {
	SI_ASSERT_NOT_NEG(typeSize);
	SI_ASSERT_NOT_NEG(capacity);

	void* data = si_allocNonZeroed(alloc, typeSize * capacity);
	if (data == nil) { return SI_TYPE_ZERO(siDynamicArrayAny); }

	siDynamicArrayAny array;
	array.alloc = alloc;
	array.data = data;
	array.typeSize = typeSize;
	array.len = 0;
	array.capacity = capacity;
	array.grow = 0;

	return array;
}

SIDEF
siDynamicArrayAny si_dynamicArrayReserve(isize typeSize, isize capacity, siAllocator alloc) {
	SI_ASSERT_NOT_NEG(typeSize);
	SI_ASSERT_NOT_NEG(capacity);

	void* data = si_alloc(alloc, typeSize * capacity);
	if (data == nil) {
		siDynamicArrayAny array = SI_STRUCT_ZERO;
		return array;
	}

	siDynamicArrayAny array;
	array.alloc = alloc;
	array.data = data;
	array.typeSize = typeSize;
	array.len = 0;
	array.capacity = capacity;
	array.grow = 0;

	return array;
}

inline
siDynamicArrayAny si_dynamicArrayCopy(siArrayAny array, siAllocator alloc) {
	return si_dynamicArrayMakeEx(array.data, array.typeSize, array.len, alloc);
}

inline
void si_dynamicArrayFree(siDynamicArrayAny array) {
	si_free(array.alloc, array.data);
}


inline void* si_dynamicArrayGet(siDynamicArrayAny array, isize index) { return si_arrayGet(SI_ARR_DYN(array), index); }
inline void* si_dynamicArrayFront(siDynamicArrayAny array) { return si_arrayFront(SI_ARR_DYN(array)); }
inline void* si_dynamicArrayBack(siDynamicArrayAny array) { return si_arrayBack(SI_ARR_DYN(array)); }
inline void* si_dynamicArraySet(siDynamicArrayAny array, isize index, const void* data, isize count) { return si_arraySet(SI_ARR_DYN(array), index, data, count); }

inline bool si_dynamicArrayAtGet(siDynamicArrayAny array, isize index, void* out) { return si_arrayAtGet(SI_ARR_DYN(array), index, out); }
inline bool si_dynamicArrayAtFront(siDynamicArrayAny array, void* out) { return si_arrayAtFront(SI_ARR_DYN(array), out); }
inline bool si_dynamicArrayAtBack(siDynamicArrayAny array, void* out) { return si_arrayAtBack(SI_ARR_DYN(array), out); }

inline isize si_dynamicArrayFind(siDynamicArrayAny array, const void* data) { return si_arrayFind(SI_ARR_DYN(array), data); }
inline isize si_dynamicArrayFindLast(siDynamicArrayAny array, const void* data) { return si_arrayFindLast(SI_ARR_DYN(array), data); }
inline isize si_dynamicArrayFindCount(siDynamicArrayAny array, const void* data) { return si_arrayFindCount(SI_ARR_DYN(array), data); }

inline bool si_dynamicArrayEqual(siDynamicArrayAny lhs, siDynamicArrayAny rhs) { return si_arrayEqual(SI_ARR_DYN(lhs), SI_ARR_DYN(rhs)); }



inline
void* si_dynamicArrayAppend(siDynamicArrayAny* array, const void* data, isize count) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_DYN_ARR(*array);
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(count);

	/* TODO(EimaMei): Fix this. */
	isize oldLen = array->len;
	si_dynamicArrayMakeSpaceFor(array, count);

	void* ptr = si_dynamicArrayGet(*array, oldLen);
	si_memcopy(ptr, data, count * array->typeSize);

	return ptr;
}

inline
void si_dynamicArrayPop(siDynamicArrayAny* array) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_DYN_ARR(*array);
	SI_STOPIF(array->len == 0, return);
	array->len -= 1;
}

inline
void si_dynamicArrayClear(siDynamicArrayAny* array) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_DYN_ARR(*array);
	array->len = 0;
}


SIDEF
bool si_dynamicArrayInsert(siDynamicArrayAny* array, isize index, const void* data,
		isize count) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_DYN_ARR(*array);
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(index);
	SI_ASSERT_NOT_NEG(count);
	SI_ASSERT(index < array->len);
	SI_STOPIF(count == 0, return false);

	isize remainderLen = array->len - (index + count);
	bool allocated = si_dynamicArrayMakeSpaceFor(array, count);

	void* dst = si_dynamicArrayGet(*array, index);
	si_memcopy(si_dynamicArrayGet(*array, index + count), dst, remainderLen * array->typeSize);
	si_memcopy(dst, data, count * array->typeSize);

	return allocated;
}

inline
void si_dynamicArrayErase(siDynamicArrayAny* array, isize index, isize count) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_NOT_NEG(index);
	SI_ASSERT_NOT_NEG(count);
	SI_ASSERT(index + count <= array->len);
	SI_STOPIF(array == 0, return);

	isize length = array->len - index - count;
	void* dst = si_dynamicArrayGet(*array, index);
	void* src = si_dynamicArrayGet(*array, index + count);
	si_memcopy(dst, src, length * array->typeSize);

	array->len -= count;
}


inline
void si_dynamicArrayReplaceAll(siDynamicArrayAny array, void* restrict valueOld, void* restrict valueNew) {
	si_dynamicArrayReplace(array, valueOld, valueNew, -1);
}

SIDEF
void si_dynamicArrayReplace(siDynamicArrayAny array, void* restrict valueOld, void* restrict valueNew,
		isize amount) {
	SI_ASSERT_NOT_NIL(valueOld);
	SI_ASSERT_NOT_NIL(valueNew);
	SI_STOPIF(amount == 0, return);


	isize count = 0;
	for_range (i, 0, array.len) {
		void* element = si_dynamicArrayGet(array, i);
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
void si_dynamicArrayReverse(siDynamicArrayAny array) {
	SI_ASSERT_ARR(array);
	SI_ASSERT(array.typeSize <= 1024);

	u8* a = (u8*)si_dynamicArrayFront(array);
	u8* b = (u8*)si_dynamicArrayBack(array);
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
bool si_dynamicArrayFill(siDynamicArrayAny* array, isize index, isize count, const void* data) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_ARR(*array);
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(index);
	SI_ASSERT_NOT_NEG(count);

	isize addLen = (index + count) - array->len;
	bool allocated = (addLen > 0)
		? si_dynamicArrayMakeSpaceFor(array, addLen)
		: false;

	u8* dst = (u8*)si_dynamicArrayGet(*array, index);
	for_range (i, 0, count) {
		dst += si_memcopy(dst, data, array->typeSize);
	}

	return allocated;
}



SIDEF
bool si_dynamicArrayMakeSpaceFor(siDynamicArrayAny* array, isize addLen) {
	SI_ASSERT_NOT_NIL(array);
	SI_ASSERT_NOT_NIL(array->alloc.proc);

	isize newLength = array->len + addLen;
	if (newLength <= array->capacity) {
		array->len = newLength;
		return false;
	}

	isize newCapacity = (array->grow <= 0)
		? SI_DYNAMIC_ARRAY_NEW_CAP(array, addLen)
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
isize si_memcopy_s(siArrayAny dst, const void* src, isize sizeSrc) {
	isize length = si_min(isize, dst.len, sizeSrc);
	return si_memcopy(dst.data, src, length);
}

inline
isize si_memmove_s(siArrayAny dst, const void* src, isize sizeSrc) {
	isize length = si_min(isize, dst.len, sizeSrc);
	return si_memmove(dst.data, src, length);
}


#endif /* SI_IMPLEMENTATION_ARRAY */

#ifdef SI_IMPLEMENTATION_STREAM

inline
isize si_streamRead(siStream stream, siArray(u8) out, siStreamError* outError) {
	return si_streamReadAt(stream, 0, out, outError);
}

inline
isize si_streamReadAt(siStream stream, isize offset, siArray(u8) out, siStreamError* outError) {
	SI_ASSERT_ARR_TYPE(out, u8);

	siStreamError tmp;
	if (outError == nil) { outError = &tmp; }

	return stream.proc(siStreamType_Read, out, offset, siSeekWhere_Begin, outError, stream.data);
}

inline
isize si_streamWrite(siStream stream, siArray(u8) ptr, siStreamError* outError) {
	SI_ASSERT_ARR_TYPE(ptr, u8);

	siStreamError tmp;
	if (outError == nil) { outError = &tmp; }

	return stream.proc(siStreamType_Write, ptr, 0, siSeekWhere_Begin, outError, stream.data);

}

inline
isize si_streamWriteStr(siStream stream, siString str, siStreamError* outError) {
	return si_streamWrite(stream, SI_ARR_LEN((u8*)str.data, str.len), outError);
}

inline
isize si_streamWriteByte(siStream stream, u8 byte, siStreamError* outError) {
	return si_streamWrite(stream, SI_ARR_LEN(&byte, 1), outError);
}

inline
isize si_streamWriteInt(siStream stream, i64 num, siStreamError* outError) {
	return si_streamWriteStr(stream, si_stringFromInt(SI_ARR_STACK(64), num), outError);
}


inline
isize si_streamWriteAt(siStream stream, isize offset, siArray(u8) ptr, siStreamError* outError) {
	SI_ASSERT_NOT_NEG(offset);
	SI_ASSERT_ARR_TYPE(ptr, u8);

	siStreamError tmp;
	if (outError == nil) { outError = &tmp; }

	return stream.proc(siStreamType_WriteAt, ptr, offset, siSeekWhere_Begin, outError, stream.data);
}

inline
isize si_streamWriteAtStr(siStream stream, isize offset, siString str, siStreamError* outError) {
	return si_streamWriteAt(stream, offset, SI_ARR_LEN((u8*)str.data, str.len), outError);
}

inline
isize si_streamWriteAtByte(siStream stream, isize offset, u8 byte, siStreamError* outError) {
	return si_streamWriteAt(stream, offset, SI_ARR_LEN(&byte, 1), outError);
}

inline
isize si_streamWriteAtInt(siStream stream, isize offset, i64 num, siStreamError* outError) {
	return si_streamWriteAtStr(stream, offset, si_stringFromInt(SI_ARR_STACK(64), num), outError);
}

inline
isize si_streamSeek(siStream stream, isize offset, siSeekWhere whence, siStreamError* outError) {
	siStreamError tmp;
	if (outError == nil) { outError = &tmp; }

	return stream.proc(siStreamType_Seek, SI_ARR_LEN((u8*)nil, 0), offset, whence, outError, stream.data);
}

inline
isize si_streamSize(siStream stream, siStreamError* outError) {
	siStreamError tmp;
	if (outError == nil) { outError = &tmp; }

	isize n = stream.proc(siStreamType_Size, SI_ARR_LEN((u8*)nil, 0), 0, siSeekWhere_Begin, outError, stream.data);
	return n;
}



inline
siStreamError si_streamFlush(siStream stream) {
	siStreamError error;
	stream.proc(siStreamType_Flush, SI_ARR_LEN((u8*)nil, 0), 0, siSeekWhere_Begin, &error, stream.data);
	return error;
}


siIntern
siStreamError si__writerFlush(siIOWriter* b) {
	if (b->len == 0) { return siStreamError_None; }

	siStreamError error;
	isize n = si_streamWrite(b->stream, SI_ARR_LEN(b->data, b->len), &error);
	if (error == 0 && n < b->len) {
		error = siStreamError_ShortWrite;
	}

	if (error != 0) {
		if (n > 0 && n < b->len) {
			si_memcopy(b->data, &b->data[n], b->len - n);
		}
		b->len -= n;
	}
	else {
		b->len = 0;
	}

	return error;
}

SIDEF
SI_STREAM_PROC(si_streamWriter_proc) {
	siIOWriter* w = (siIOWriter*)streamData;
	SI_ASSERT_NOT_NIL(w);
	SI_ASSERT_NOT_NIL(w->data);

	switch (type) {
		case siStreamType_Write: {
			isize avail = w->capacity - w->len;

			isize n = 0;
			isize m;

			siStreamError error = 0;
			while (avail < ptr.len && error == 0) {
				if (avail == 0) {
					m = si_streamWrite(w->stream, ptr, &error);
					if (m < 0 && *outError == 0) {
						*outError = siStreamError_NegativeWrite;
						break;
					}
				}
				else {
					m = si_memcopy(&w->data[w->len], ptr.data, avail);
					si__writerFlush(w);
				}

				n += m;
				ptr = si_sliceFrom(ptr, avail);
			}

			m = si_memcopy(&w->data[w->len], ptr.data, ptr.len);
			w->len += m;
			n += m;

			return n;
		}

		case siStreamType_Flush: {
			*outError = si__writerFlush(w);
			return 0;
		}

		case siStreamType_Destroy: {
			if (w->alloc.proc) {
				si_free(w->alloc, w->data);
				w->alloc.proc = nil;
			}
			w->data = nil;
			w->len = 0;
			w->capacity = 0;

			return 0;
		}

		case siStreamType_GetFeatures: {
			return SI_BITMASK_MAKE(siStreamType, Write, Flush, Destroy);
		} break;
	}

	*outError = siStreamError_Unimplemented;
	return 0;
	SI_UNUSED(seek); SI_UNUSED(offset);
}


SIDEF
siIOWriter si_ioWriterMakeAlloc(siStream stream, siAllocator alloc, isize capacity, isize align) {
	u8* data = si_allocArrayNonZeroedAlign(alloc, u8, capacity, align);
	if (data == nil) { return SI_TYPE_ZERO(siIOWriter); }

	siIOWriter writer = SI_TYPE_ZERO(siIOWriter);
	writer.alloc = alloc;
	writer.data = data;
	writer.capacity = capacity;
	writer.stream = stream;

	return writer;
}

SIDEF
siIOWriter si_ioWriterMakePtr(siStream stream, void* ptr, isize capacity) {
	SI_ASSERT_NOT_NIL(ptr);
	SI_ASSERT_NOT_NEG(capacity);

	siIOWriter writer = SI_TYPE_ZERO(siIOWriter);
	writer.data = (u8*)ptr;
	writer.capacity = capacity;
	writer.stream = stream;

	return writer;
}

inline
siIOWriter si_ioWriterMakeArray(siStream stream, siArray(u8) array) {
	SI_ASSERT_ARR_TYPE(array, u8);
	return si_ioWriterMakePtr(stream, array.data, array.len);
}

inline
siStream si_streamWriter(siIOWriter* writer) {
	siStream stream;
	stream.proc = si_streamWriter_proc;
	stream.data = writer;
	return stream;
}

#endif /* SI_IMPLEMENTATION_STREAM */

#ifdef SI_IMPLEMENTATION_STRING

#if SI_LANGUAGE_IS_CPP
u8 siString::operator[](isize index) {
	SI_ASSERT(index < len);
	SI_ASSERT_NOT_NEG(index);
	return this->data[index];
}
siString siString::operator()(isize offset1, isize offset2) {
	return si_substr(*this, offset1, offset2);
}
siString siString::from(isize offset1) {
	return si_substrFrom(*this, offset1);
}
siString siString::to(isize offset2) {
	return si_substrTo(*this, offset2);
}
siString siString::substrLen(isize offset1, isize length) {
	return si_substrLen(*this, offset1, length);
}
#endif

SIDEF
siUtf32Char si__stringLastRune(siString str) {
	SI_ASSERT_MSG(str.len > 0, "This function doesn't check for if the length is zero. Fix your function.");

	u8 lastChar = str.data[str.len - 1];
	if (lastChar < 0x80) {
		return SI_COMP_LIT(siUtf32Char, lastChar, 1);
	}

	isize end = si_max(isize, str.len - 4, 0);
	isize i;
	for (i = str.len - 1; i >= end; i -= 1) {
		if ((str.data[i] & 0xC0) != 0x80) {
			break;
		}
	}

	i = si_max(isize, i, 0);
	return si_utf8Decode(&str.data[i]);
}

siIntern
siString si__stringTrimCr(siString str) {
	isize i = str.len;
	if (i > 0 && str.data[i - 1] == '\r') {
		str.len -= 1;
	}

	return str;
}


inline
siBuilder si_builderMake(siAllocator alloc, isize capacity, isize grow) {
	return si_builderMakeLen(alloc, capacity, 0, grow);
}

SIDEF
siBuilder si_builderMakeLen(siAllocator alloc, isize capacity, isize len, isize grow) {
	SI_ASSERT_NOT_NEG(capacity);
	SI_ASSERT_NOT_NEG(len);
	SI_ASSERT_NOT_NEG(grow);

	u8* data = si_allocArrayNonZeroed(alloc, u8, capacity);
	if (data == nil) { return SI_TYPE_ZERO(siBuilder); }

	siBuilder b;
	b.data = data;
	b.capacity = capacity;
	b.len = len;
	b.alloc = alloc;
	b.grow = grow;

	return b;
}

inline
siBuilder si_builderMakeNone(siAllocator alloc) {
	siBuilder b = SI_STRUCT_ZERO;
	b.alloc = alloc;

	return b;
}


inline
siBuilder si_builderFromArray(siArray(u8) array) {
	SI_ASSERT_ARR_TYPE(array, u8);

	siBuilder b = SI_STRUCT_ZERO;
	b.data = (u8*)array.data;
	b.capacity = array.len;
	return b;
}

inline
siString si_builderToStr(siBuilder b) {
	return SI_STR_LEN(b.data, b.len);
}
inline
char* si_builderToCstr(siBuilder* b) {
	si_builderWriteByte(b, '\0');
	si_builderPopByte(b);
	return (char*)b->data;
}


SIDEF
siAllocationError si_builderMakeSpaceFor(siBuilder* b, isize addLen) {
	SI_ASSERT_NOT_NIL(b);

	isize newLength = b->len + addLen;
	if (newLength <= b->capacity) {
		SI_ASSERT_NOT_NEG(newLength);
		return siAllocationError_None;
	}
	SI_ASSERT_NOT_NIL(b->alloc.proc);

	isize newCapacity = (b->grow <= 0)
		? SI_BUILDER_NEW_CAP(b, addLen)
		: b->capacity + addLen + b->grow;
	SI_ASSERT(newLength <= newCapacity);

	siAllocationError error;
	void* data = si_reallocNonZeroed(b->alloc, b->data, b->capacity, newCapacity, &error);
	if (data != nil) {
		b->data = (u8*)data;
		b->capacity = newCapacity;
	}

	return error;
}

inline
isize si_builderWriteByte(siBuilder* b, u8 byte) {
	return si_builderWritePtr(b, &byte, 1);
}

inline
isize si_builderWriteArray(siBuilder* b, siArray(u8) array) {
	SI_ASSERT_ARR_TYPE(array, u8);
	return si_builderWritePtr(b, array.data, array.len);
}


SIDEF
isize si_builderWritePtr(siBuilder* b, const void* bytes, isize len) {
	siAllocationError res = si_builderMakeSpaceFor(b, len);
	if (res != siAllocationError_None) { return 0; }

	isize count = (res == 0) ? len : (b->capacity - b->len);
	b->len += si_memcopy(&b->data[b->len], bytes, count);

	return count;
}

inline
isize si_builderWriteStr(siBuilder* b, siString str) {
	return si_builderWritePtr(b, str.data, str.len);
}
inline
isize si_builderWriteRune(siBuilder* b, siRune character) {
	siUtf8Char str = si_utf8Encode(character);
	return si_builderWritePtr(b, str.codepoint, str.len);
}

inline
isize si_builderWriteStrQuoted(siBuilder* b, siString str, u8 quote) {
	si_builderMakeSpaceFor(b, 2 + str.len);
	siArray(u8) data = SI_ARR_LEN(&b->data[b->len], b->capacity - b->len);

	isize n = 0;
	n += si_memcopy_s(si_sliceFrom(data, n), &quote, 1);
	n += si_memcopyStr_s(si_sliceFrom(data, n), str);
	n += si_memcopy_s(si_sliceFrom(data, n), &quote, 1);

	b->len += n;
	return n;
}

SIDEF
isize si_builderWriteStrQuotedRune(siBuilder* b, siString str,
		siRune quoteStart, siRune quoteEnd) {
	siUtf8Char start = si_utf8Encode(quoteStart),
			   end = si_utf8Encode(quoteEnd);

	si_builderMakeSpaceFor(b, start.len + str.len + end.len);
	siArray(u8) data = SI_ARR_LEN(&b->data[b->len], b->capacity - b->len);

	isize n = 0;
	n += si_memcopy_s(si_sliceFrom(data, n), start.codepoint, start.len);
	n += si_memcopyStr_s(si_sliceFrom(data, n), str);
	n += si_memcopy_s(si_sliceFrom(data, n), end.codepoint, end.len);

	b->len += n;
	return n;
}


SIDEF
isize si_builderWriteInt(siBuilder* b, i64 num, i32 base, siNumToStrOption options) {
	siString str = si_stringFromInt(SI_ARR_STACK(256), num, base, options);
	return si_builderWriteStr(b, str);
}

SIDEF
isize si_builderWriteUInt(siBuilder* b, u64 num, i32 base, siNumToStrOption options) {
	siString str = si_stringFromUInt(SI_ARR_STACK(256), num, base, options);
	return si_builderWriteStr(b, str);
}


inline
isize si_builderWriteFloat(siBuilder* b, f64 num) {
	return si_builderWriteFloatEx(b, num, 10, 6);
}

SIDEF
isize si_builderWriteFloatEx(siBuilder* b, f64 num, i32 base, i32 afterPoint) {
	siString str = si_stringFromFloatEx(num, base, afterPoint, SI_ARR_STACK(1024));
	return si_builderWriteStr(b, str);
}


inline
void si_builderClear(siBuilder* b) {
	SI_ASSERT_NOT_NIL(b);
	b->len = 0;
}

inline
void si_builderPopByte(siBuilder* b) {
	SI_ASSERT_NOT_NIL(b);
	b->len -= (b->len != 0);
}
inline
void si_builderPopRune(siBuilder* b) {
	SI_ASSERT_NOT_NIL(b);

	if (b->len != 0) {
		siUtf32Char r = si__stringLastRune(SI_STR_LEN(b->data, b->len));
		b->len -= r.len;
	}
}

inline
void si_builderFree(siBuilder* b) {
	SI_ASSERT_NOT_NIL(b);
	SI_ASSERT_NOT_NIL(b->data);

	if (b->alloc.proc) {
		si_free(b->alloc, b->data);
	}
	b->data = nil;
	b->len = 0;
	b->capacity = 0;
}

siIntern
isize si__writerBuilder_proc(siStreamType type, siArrayAny ptr, isize offset,
	siSeekWhere seek, siStreamError* outError, void* streamData) {
		siBuilder* b = (siBuilder*)streamData;
	SI_ASSERT_NOT_NIL(b);
	SI_UNUSED(offset); SI_UNUSED(seek);

	switch (type) {
		case siStreamType_Write: {
			/* TODO(EimaMei): Add errors later on. */
			isize n = si_builderWriteArray(b, ptr);
			*outError = (n == 0) ? siStreamError_Unknown : siStreamError_None;

			return n;
		} break;

		case siStreamType_Size: {
			*outError = 0;
			return b->len;
		} break;

		case siStreamType_Destroy: {
			si_builderFree(b);
			*outError = 0;

			return 0;
		} break;

		case siStreamType_GetFeatures: {
			return SI_BITMASK_MAKE(siStreamType, Write, Size, Destroy);
		} break;
	}

	*outError = siStreamError_Unimplemented;
	return 0;
}

SIDEF
siWriter si_builderToWriter(siBuilder* b) {
	siWriter stream;
	stream.data = b;
	stream.proc = si__writerBuilder_proc;
	return stream;
}

SIDEF siString SI_STR2(const siString x) { return x; }
SIDEF siString SI_STR2_ptr(const siString* x) { return *x; }
SIDEF siString SI_CSTR2(const char* x) { return SI_COMP_LIT(siString, si_cstrLen(x), (const u8*)x); }

inline
siString si_stringCopy(siString from, siAllocator alloc) {
	u8* str = si_allocArrayNonZeroed(alloc, u8, from.len);
	si_memcopyStr(str, from);

	return SI_STR_LEN(str, from.len);
}

inline
siString si_stringFromCStr(cstring from, siAllocator alloc) {
	return si_stringCopy(SI_CSTR(from), alloc);
}

inline
char* si_stringToCStr(siString from, siAllocator alloc) {
	return si_stringToCStrEx(from, from.len, alloc);
}
SIDEF
char* si_stringToCStrEx(siString from, isize capacity, siAllocator alloc) {
	char* str = si_allocArrayNonZeroed(alloc, char, capacity + 1);
	si_memcopyStr(str, from);
	str[from.len] = '\0';

	return str;
}


inline
siRune si_stringAtFront(siString str) {
	SI_ASSERT_NOT_NIL(str.data);
	SI_STOPIF(str.len == 0, return -1);

	return si_utf8Decode(str.data).codepoint;
}

SIDEF
siRune si_stringAtBack(siString str) {
	SI_ASSERT_STR(str);
	SI_STOPIF(str.len == 0, return -1);

	return si__stringLastRune(str).codepoint;
}

inline
const u8* si_stringBegin(siString str) {
	return str.data;
}

inline
const u8* si_stringEnd(siString str) {
	return &str.data[str.len];
}


inline
siString si_substr(siString str, isize offset1, isize offset2) {
	SI_ASSERT_STR(str);
	SI_ASSERT_NOT_NEG(offset1);
	SI_ASSERT_NOT_NEG(offset2);
	SI_ASSERT(offset1 <= str.len && offset2 <= str.len);
	SI_ASSERT(offset1 <= offset2);

	siString res;
	res.data = &str.data[offset1];
	res.len = offset2 - offset1;

	return res;
}

inline
siString si_substrFrom(siString str, isize offset1) {
	return si_substr(str, offset1, str.len);
}
inline
siString si_substrTo(siString str, isize offset2) {
	return si_substr(str, 0, offset2);
}
inline
siString si_substrLen(siString str, isize offset1, isize len) {
	return si_substr(str, offset1, offset1 + len);
}


SIDEF
isize si_stringFind(siString str, siString subStr) {
	isize counter = 0;

	for_range (i, 0, str.len) {
		const u8* x = &str.data[i];
		if (*x != subStr.data[counter]) {
			counter = 0;
			continue;
		}

		counter += 1;
		if (counter == subStr.len) {
			isize index = si_pointerDiff(str.data, x);
			return index - (subStr.len - 1);
		}
	}

	return -1;
}

SIDEF
isize si_stringFindByte(siString str, u8 byte) {
	for_range (i, 0, str.len) {
		if (str.data[i] == byte) {
			return i;
		}
	}

	return -1;
}

SIDEF
isize si_stringFindRune(siString str, siRune rune) {
	siRune x;
	for_eachStrEx (x, index, str) {
		if (x == rune) {
			return index;
		}
	}

	return -1;
}

SIDEF
isize si_stringFindLast(siString str, siString subStr) {
	SI_ASSERT_STR(str);
	SI_ASSERT_STR(subStr);
	SI_STOPIF(subStr.len == 0, return -1);

	isize subStrEnd = subStr.len - 1;
	isize counter = subStrEnd;

	isize i;
	for (i = str.len - 1; i >= 0; i -= 1) {
		if (str.data[i] != subStr.data[counter]) {
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

SIDEF
isize si_stringFindLastByte(siString str, u8 byte) {
	SI_ASSERT_STR(str);

	isize i;
	for (i = str.len - 1; i >= 0; i -= 1) {
		if (str.data[i] == byte) {
			return i;
		}
	}

	return -1;
}

SIDEF
isize si_stringFindLastRune(siString str, siRune rune) {
	SI_ASSERT_STR(str);

	siRune x;
	for_eachRevStrEx (x, index, str) {
		if (x == rune) {
			return index;
		}
	}

	return -1;
}


SIDEF
isize si_stringFindCount(siString str, siString subStr) {
	SI_ASSERT_STR(str);
	SI_ASSERT_STR(subStr);

	isize occurences = 0;
	isize counter = 0;

	for_range (i, 0, str.len) {
		if (str.data[i] != subStr.data[counter]) {
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

SIDEF
bool si_stringEqual(siString lhs, siString rhs) {
	if (lhs.len != rhs.len) { return false; }
	if (lhs.data == rhs.data) { return true; }

	return si_memcompare(lhs.data, rhs.data, lhs.len) == 0;
}



SIDEF
siString si_stringJoin(siArray(siString) arrStr, siString separator, siAllocator alloc) {
	SI_ASSERT_ARR_TYPE(arrStr, siString);
	SI_STOPIF(arrStr.len == 0, return SI_STR_EMPTY);

	isize length = separator.len * (arrStr.len - 1);
	siString str;
	for_eachArr (str, arrStr) {
		length += str.len;
	}

	siString* data = (siString*)arrStr.data;
	u8* res = si_allocArrayNonZeroed(alloc, u8, length);
	si_memcopyStr(res, data[0]);

	isize i = data[0].len;
	for_range (j, 1, arrStr.len) {
		i += si_memcopyStr(&res[i], separator);
		i += si_memcopyStr(&res[i], data[j]);
	}

	return SI_STR_LEN(res, length);
}


inline
siString si_stringTrim(siString str, siString cutSet) {
	return si_stringTrimRight(si_stringTrimLeft(str, cutSet), cutSet);
}

SIDEF
siString si_stringTrimLeft(siString str, siString cutSet) {
	SI_ASSERT_STR(str);
	SI_STOPIF(str.len == 0, return str);

	const u8* posStart = si_stringBegin(str);
	const u8* posEnd = si_stringEnd(str);

	while (si_stringFindByte(cutSet, *posStart) != -1) {
		posStart += 1;
		SI_STOPIF(posStart >= posEnd, break);
	}

	return SI_STR_LEN(posStart, si_pointerDiff(posStart, posEnd));
}

SIDEF
siString si_stringTrimRight(siString str, siString cutSet) {
	const u8* posStart = si_stringBegin(str);
	const u8* posEnd = si_stringEnd(str) - 1;

	while (posEnd > posStart && si_stringFindByte(cutSet, *posEnd) != -1) {
		posEnd -= 1;
	}

	str.len = si_pointerDiff(posStart, posEnd + 1);
	return str;
}

inline
siString si_stringStrip(siString str) {
	return si_stringTrimRight(si_stringTrimLeft(str, SI_STR(" \t\r\n\v\f")), SI_STR(" \t\r\n\v\f"));
}

inline
siString si_stringStripLeft(siString str) {
	return si_stringTrimLeft(str, SI_STR(" \t\r\n\v\f"));
}

inline
siString si_stringStripRight(siString str) {
	return si_stringTrimRight(str, SI_STR(" \t\r\n\v\f"));
}

SIDEF
siString si_stringUnquote(siString str) {
	SI_STOPIF(str.len < 2, return str);
	if (str.data[0] == '\"') { str.data += 1; str.len -= 1; }
	if (str.data[str.len - 1] == '\"') { str.len -= 1; }

	return str;
}



SIDEF
siString si_stringInsert(siString str, siString subStr, isize index, siAllocator alloc) {
	SI_ASSERT_NOT_NEG(index);
	SI_ASSERT(index < str.len);
	SI_STOPIF(subStr.len == 0, return str);

	isize len = str.len + subStr.len;
	u8* res = si_allocArrayNonZeroed(alloc, u8, len);

	isize i = 0;
	i += si_memcopyStr(&res[i], si_substrTo(str, index));
	i += si_memcopyStr(&res[i], subStr);
	i += si_memcopyStr(&res[i], si_substrFrom(str, index));

	return SI_STR_LEN(res, len);
}

inline
siString si_stringRemoveAll(siString str, siString subStr, siAllocator alloc) {
	return si_stringRemove(str, subStr, -1, alloc);
}

inline
siString si_stringRemove(siString str, siString subStr, i32 amount, siAllocator alloc) {
	return si_stringReplace(str, subStr, SI_STR(""), amount, alloc);
}


SIDEF
siString si_stringReverse(siString str, siAllocator alloc) {
	isize len = str.len;
	u8* res = si_allocArrayNonZeroed(alloc, u8, len);

	isize i = len,
		  j = 0;
	while (i > 0) {
		siUtf32Char x = si_utf8Decode(&str.data[j]);

		i -= x.len;
		j += si_memcopy(&res[i], &str.data[j], x.len);
	}

	return SI_STR_LEN(res, len);
}


inline
siString si_stringReplaceAll(siString str, siString strOld, siString strNew,
		siAllocator alloc) {
	return si_stringReplace(str, strOld, strNew, -1, alloc);
}

SIDEF
siString si_stringReplace(siString str, siString strOld, siString strNew, isize amount,
		siAllocator alloc) {
	SI_STOPIF(strOld.data == strNew.data, return str);

	if (amount < 0) {
		amount = si_stringFindCount(str, strOld);
	}
	SI_STOPIF(amount == 0, return str);

	isize len = str.len + amount * (strNew.len - strOld.len);
	isize lineStart = 0, i = 0;
	u8* res = si_allocArrayNonZeroed(alloc, u8, len);

	while (amount) {
		siString subStr = si_substrFrom(str, lineStart);
		subStr.len = si_stringFind(subStr, strOld);

		i += si_memcopyStr(&res[i], subStr);
		i += si_memcopyStr(&res[i], strNew);

		lineStart += subStr.len + strOld.len;
		amount -= 1;
	}
	si_memcopyStr(&res[i], si_substrFrom(str, lineStart));

	return SI_STR_LEN(res, len);
}


inline
siArray(siString) si_stringSplit(siString str, siString delimiter, siAllocator alloc) {
	return si_stringSplitEx(str, delimiter, -1, alloc);
}

SIDEF
siArray(siString) si_stringSplitEx(siString str, siString delimiter, isize amount,
		siAllocator alloc) {
	SI_ASSERT_STR(str);
	SI_ASSERT_STR(delimiter);

	if (amount < 0) {
		amount = si_stringFindCount(str, delimiter);
	}
	SI_STOPIF(amount == 0, return SI_ARR_LEN((u8*)nil, 0));

	isize len = amount + 1;
	siArray(siString) res = si_arrayMakeReserveNonZeroed(siString, len, alloc);
	siString* data = (siString*)res.data;

	isize lineStart = 0;
	for_range (i, 0, amount) {
		siString subStr = si_substrFrom(str, lineStart);
		subStr.len = si_stringFind(subStr, delimiter);

		data[i] = subStr;
		lineStart += subStr.len + delimiter.len;
	}

	if (str.len == lineStart) {
		res.len -= 1;
		return res;
	}
	data[amount] = si_substrFrom(str, lineStart);

	return res;
}

SIDEF
siArray(siString) si_stringSplitLines(siString str, siAllocator alloc) {
	siArray(siString) arr = si_stringSplit(str, SI_STR("\n"), alloc);

	siString* line;
	for_eachRefArr (line, arr) {
		*line = si__stringTrimCr(*line);
	}

	return arr;
}


SIDEF
bool si_stringSplitIterate(siString* str, siString delimiter, siString* outStr) {
	SI_ASSERT_NOT_NIL(str);
	SI_ASSERT_NOT_NIL(outStr);
	SI_ASSERT_STR(*str);
	SI_ASSERT_STR(delimiter);
	SI_STOPIF(delimiter.len == 0, *outStr = SI_STR_EMPTY; return false);

	siString res = *str;
	isize i = si_stringFind(res, delimiter);

	if (i > -1) {
		res.len = i;
		*str = si_substrFrom(*str, i + delimiter.len);
		*outStr = res;
		return true;
	}
	else {
		*outStr = res;
		*str = si_substrFrom(*str, str->len);

		return res.len != 0;
	}
}

SIDEF
bool si_stringSplitLinesIterate(siString* str, siString* outStr) {
	SI_ASSERT_NOT_NIL(outStr);

	siString out;
	bool res = si_stringSplitIterate(str, SI_STR("\n"), &out);

	if (res) {
		out = si__stringTrimCr(out);
	}
	*outStr = out;

	return res;
}



#ifdef SI_IMPLEMENTATION_MEMORY

inline isize si_memcopyStr(void* dst, siString src) { return si_memcopy(dst, src.data, src.len); }
inline isize si_memmoveStr(void* dst, siString src) { return si_memmove(dst, src.data, src.len); }
inline i32 si_memcompareStr(const void* dst, siString src) { return si_memcompare(dst, src.data, src.len); }

inline isize si_memcopyStr_s(siArrayAny dst, siString src)  { return si_memcopy_s(dst, src.data, src.len); }
inline isize si_memmoveStr_s(siArrayAny dst, siString src) { return si_memmove_s(dst, src.data, src.len); }

#endif

SIDEF
siString si_stringUpper(siString str, siAllocator alloc) {
	siBuilder b = si_builderMake(alloc, str.len);

	siRune x;
	for_eachStr (x, str) {
		si_builderWriteRune(&b, si_runeUpper(x));
	}

	return si_builderToStr(b);
}

SIDEF
siString si_stringLower(siString str, siAllocator alloc) {
	siBuilder b = si_builderMake(alloc, str.len);

	siRune x;
	for_eachStr (x, str) {
		si_builderWriteRune(&b, si_runeLower(x));
	}

	return si_builderToStr(b);

}


siString SI_NUM_TABLE_UPPER = SI_STRC(
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUV"
);
siString SI_NUM_TABLE_LOWER = SI_STRC(
	"0123456789"
	"abcdefghijklmnopqrstuv"
);


siIntern
u64 si__bitsAreNegative(u64 num, isize bitSize, bool isSigned, bool* outIsNeg) {
	SI_ASSERT_NOT_NEG(bitSize);
	if (!isSigned) { *outIsNeg = false; return num; }

	switch (bitSize) {
		case  8: *outIsNeg =  (i8)num < 0; return (u64)si_abs( i8,  (i8)num);
		case 16: *outIsNeg = (i16)num < 0; return (u64)si_abs(i16, (i16)num);
		case 32: *outIsNeg = (i32)num < 0; return (u64)si_abs(i32, (i32)num);
		case 64: *outIsNeg = (i64)num < 0; return (u64)si_abs(i64, (i64)num);
		default: SI_PANIC_MSG("Invalid bit size.");
	}
}

SIDEF
siString si_stringFromBits(u64 num, isize bitSize, bool isSigned, i32 base,
		siNumToStrOption options, siString table, siArray(u8) out) {
	SI_ASSERT(si_between(i64, base, 2, SI_BASE_MAX));
	SI_ASSERT_ARR_TYPE(out, u8);

	u8 buf[256];

	bool isNegative;
	num = si__bitsAreNegative(num, bitSize, isSigned, &isNegative);
	u64 base_u = (u64)base;

	isize i = countof(buf) - 1;
	do {
		buf[i] = table.data[num % base_u];
		num /= base_u;
		i -= 1;
	} while (num != 0);

	if (options & siNumToStrOption_Prefix) {
		switch (base) {
			case  2: buf[i] = 'b'; i -= 1; buf[i] = '0'; i -= 1; break;
			case  8: buf[i] = 'o'; i -= 1; buf[i] = '0'; i -= 1; break;
			case 10: buf[i] = 'd'; i -= 1; buf[i] = '0'; i -= 1; break;
			case 12: buf[i] = 'z'; i -= 1; buf[i] = '0'; i -= 1; break;
			case 16: buf[i] = 'x'; i -= 1; buf[i] = '0'; i -= 1; break;
		}
	}


	if (isNegative) {
		buf[i] = '-';
		i -= 1;
	}
	else if (options & siNumToStrOption_Plus) {
		buf[i] = '+';
		i -= 1;
	}

	i += 1;
	isize len = si_memcopy_s(out, &buf[i], countof(buf) - i);
	return SI_STR_LEN(out.data, len);
}


inline
siString si_stringFromInt(siArray(u8) out, i64 num, i32 base, siNumToStrOption options) {
	return si_stringFromBits((u64)num, 64, true, base, options, SI_NUM_TABLE_UPPER, out);
}

inline
siString si_stringFromUInt(siArray(u8) out, u64 num, i32 base, siNumToStrOption options) {
	return si_stringFromBits(num, 64, true, base, options, SI_NUM_TABLE_UPPER, out);
}


SIDEF
u64 si_stringToUInt(siString str, i32 base, isize* outInvalidIndex) {
	SI_ASSERT(base == -1 || si_between(i32, base, 2, SI_BASE_MAX));

	isize baseI = 0;
	siRune r;
	for_eachStrEx (r, i, str) {
		if (!si_runeIsSpace(r)) {
			baseI += i;
			str = si_substrFrom(str, i);
			break;
		}
	}

	bool isNeg;
	if (str.len > 1) {
		switch (str.data[0]) {
			case '-': str = si_substrFrom(str, 1); isNeg = true; baseI += 1; break;
			case '+': str = si_substrFrom(str, 1); isNeg = false; baseI += 1; break;
			default: isNeg = false;
		}
	}
	else {
		isNeg = false;
	}

	if (str.len > 2 && str.data[0] == '0') {
		switch (str.data[1]) {
			case 'x': base = 16; str = si_substrFrom(str, 2); baseI += 2; break;
			case 'z': base = 12; str = si_substrFrom(str, 2); baseI += 2; break;
			case 'd': base = 10; str = si_substrFrom(str, 2); baseI += 2; break;
			case 'o': base =  8; str = si_substrFrom(str, 2); baseI += 2; break;
			case 'b': base =  2; str = si_substrFrom(str, 2); baseI += 2; break;
			default:  base = 10;
		}
	}
	else if (base == -1) {
		base = 10;
	}

	u64 res = 0;
	u64 base_u = (u64)base;
	for_eachStrEx (r, i, str) {
		if (r == '_') {
			continue;
		}
		else if (r == ' ') {
			isize oldI = i + baseI;
			siUtf32Char tmp;
			i += 1;

			while (i < str.len) {
				tmp = si_utf8Decode(&str.data[i]);
				SI_STOPIF(!si_runeIsSpace(tmp.codepoint), break);
				i += tmp.len;
			}

			if (outInvalidIndex) { *outInvalidIndex = (i >= str.len) ? -1 : oldI; }
			if (isNeg) { res = (~res + 1); }
			return res;
		}

		i32 value = si_charBase32ToInt((char)r);
		if (value >= base || value == -1) {
			if (outInvalidIndex) { *outInvalidIndex = i + baseI; }
			if (isNeg) { res = (~res + 1); }
			return res;
		}

		res *= base_u;
		res += (u64)value;
	}

	if (outInvalidIndex) { *outInvalidIndex = -1; }
	if (isNeg) { res = (~res + 1); }
	return res;
}

inline
i64 si_stringToInt(siString str, i32 base, isize* outInvalidIndex) {
	return (i64)si_stringToUInt(str, base, outInvalidIndex);
}

inline
siString si_stringFromFloat(f64 num, siArray(u8) out) {
	return si_stringFromFloatEx(num, 10, 6, out);
}
SIDEF
siString si_stringFromFloatEx(f64 num, i32 base, isize afterPoint, siArray(u8) out) {
	SI_ASSERT_NOT_NEG(afterPoint);
	SI_ASSERT(si_between(i32, base, 2, SI_BASE_MAX));

	/* TODO(EimaMei): Rework this to be faster and to be more like si__stringFromBits. */
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

	/* TODO(EimaMei): Update this with a 'float is negative' function. */
	i32 isNegative;
	{
		union { f64 f; u64 n; } check = {num};
		isNegative = (i32)(check.n >> 63) & 1;
	   	check.n &= ~SI_BIT(63); /* NOTE(EimaMei): A quick way of changing the minus to plus. */
		num = check.f;
	}

	isize baseLen = 0;
	f64 numWhole = (afterPoint != 0)
		? num
		: si_round(f64, num);
	do {
		numWhole /= base;
		baseLen += 1;
	} while (numWhole > 0.9999999999999999); /* NOTE(EimaMei): How long can this be?? */


	isize len = isNegative + baseLen + (afterPoint != 0) + afterPoint;
	isize i = 0;
	u8* res = (u8*)out.data;

	if (isNegative) {
		res[i] = '-';
		i += 1;
	}

	i32 digit;
	do {
		numWhole *= base;
		digit = (i32)numWhole;
		numWhole -= digit;

		res[i] = SI_NUM_TABLE_UPPER.data[(u32)digit % (u32)base];
		i += 1;

		baseLen -= 1;
	} while (baseLen > 0);

	if (afterPoint == 0) {
		return SI_STR_LEN(res, len);
	}

	res[i] = '.';
	i += 1;

	f64 rounder = 5.0 / base;
	for_range (j, 0, afterPoint) { rounder /= base; }

	num += rounder;
	while (afterPoint) {
		num *= base;

		u64 numInt = (u64)num;
		res[i] = SI_NUM_TABLE_UPPER.data[numInt % (u64)base];
		i += 1;

		afterPoint -= 1;
		num -= (f64)numInt;
	}

	return SI_STR_LEN(res, len);
}


inline
siString si_stringFromBool(bool boolean) {
	static const siString values[] = {SI_STRC("false"), SI_STRC("true")};
	return values[boolean & true];
}

SIDEF
b32 si_stringToBool(siString str) {
	SI_ASSERT_STR(str);
	SI_STOPIF(str.len == 0, return UINT32_MAX);

	if (str.len == 1) {
		switch (str.data[0]) {
			case '1': case 't': case 'T': return true;
			case '0': case 'f': case 'F': return false;
			default: return UINT32_MAX;
		}
	}

	if (si_stringEqual(str, SI_STR("true")) || si_stringEqual(str, SI_STR("True"))) {
		return true;
	}
	else if (si_stringEqual(str, SI_STR("false")) || si_stringEqual(str, SI_STR("False"))) {
		return false;
	}

	return UINT32_MAX;
}

inline
bool si_stringHasPrefix(siString str, siString prefix) {
	return str.len >= prefix.len && (si_memcompareStr(str.data, prefix) == 0);
}

SIDEF
isize si_stringPrefixLen(siString str, siString prefix) {
	SI_STOPIF(prefix.len > str.len, return 0);

	isize count = 0;
	for_range (i, 0, str.len) {
		if (str.data[i] == prefix.data[count]) {
			count += 1;
			continue;
		}
		break;
	}

	return count;
}

inline
bool si_stringHasSuffix(siString str, siString suffix) {
	return str.len >= suffix.len
		&& (si_memcompareStr(&str.data[str.len - suffix.len], suffix) == 0);
}
SIDEF
isize si_stringSuffixLen(siString str, siString suffix) {
	SI_ASSERT_STR(str);
	SI_ASSERT_STR(suffix);
	SI_STOPIF(suffix.len > str.len, return 0);

	isize count = suffix.len - 1;
	isize i;
	for (i = str.len - 1; i >= 0; i -= 1) {
		if (str.data[i] == suffix.data[count]) {
			count -= 1;
			continue;
		}
		break;
	}

	return (suffix.len - count) + 1;
}


SIDEF
siString si_stringFromArray(siArrayAny buffer, cstring fmt, siArray(u8) out) {
	/* TODO(EimaMei): rework this. */
#if 0
	SI_ASSERT_ARR(buffer);
	SI_ASSERT_NOT_NIL(fmt);
	SI_ASSERT_ARR_TYPE(out, u8);
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

				case 'z':
					SI_STOPIF(*size == 0, *size = si_sizeof(usize));
					goto back;

				case 'j':
					SI_STOPIF(*size == 0, *size = 8);
					goto back;

				case 'f': case 'F': case 'a': case 'A': case 'e': case 'E': case 'g': case 'G':
					SI_STOPIF(*size == 0, *size = si_sizeof(f64));
					break;

				case 'S': case 'p':
					SI_STOPIF(*size == 0, *size = si_sizeof(void*));
					break;

				case 'x': case 'X': case 'i': case 'O': case 'o': case 'b': case 't':
				case 'u': case 'd':
					SI_STOPIF(*size == 0, *size = si_sizeof(u32));
					break;

				case 'c':
					SI_STOPIF(*size == 0, *size = si_sizeof(siRune));
					break;

				case 's':
					SI_STOPIF(*size == 0, *size = si_sizeof(siString));
					break;

				case 'L':
					SI_STOPIF(*size == 0, *size = si_sizeof(siCallerLoc));
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
		SI_ASSERT_MSG(argCount <= countof(sizes), "The format str has surpassed the maximum amount of allowed modifiers.");
	}

	si_arraySet(out, 0, "{");
	isize length = 1;

	for_range (i, 0, buffer.len) {

		isize baseLen = 0;
		for_range (j, 0, argCount) {
			i32 size = sizes[j];
			isize index = indexes[j];
			isize nextIndex = (j < argCount - 1 ? indexes[j + 1] : fmtLen);

			siString substr = SI_STR_LEN(&fmt[index], nextIndex - index);
			void* base = (u8*)si_arrayGet(buffer, i) + baseLen;
			baseLen += size;

			switch (size) {
				case 0:  length += si_bprintf(si_sliceFrom(out, length), substr).len; break;
				case si_sizeof(u8):  length += si_bprintf(si_sliceFrom(out, length), substr, *(u8*)base).len; break;
				case si_sizeof(u16):  length += si_bprintf(si_sliceFrom(out, length), substr, *(u16*)base).len; break;
				case si_sizeof(u32):  length += si_bprintf(si_sliceFrom(out, length), substr, *(u32*)base).len; break;
				case si_sizeof(u64):  length += si_bprintf(si_sliceFrom(out, length), substr, *(u64*)base).len; break;
				#if SI_ARCH_IS_64BIT
				case si_sizeof(siString): length += si_bprintf(si_sliceFrom(out, length), substr, *(siString*)base).len; break;
				#endif
				case si_sizeof(siCallerLoc): length += si_bprintf(si_sliceFrom(out, length), substr, *(siCallerLoc*)base).len; break;
				default: SI_PANIC();
			}
		}

		if (i != buffer.len - 1) {
			length += si_memcopyStr_s(si_sliceFrom(out, length), SI_STR(", "));
		}
		SI_STOPIF(length >= out.len, return SI_STR_LEN(out.data, length));
	}

	if (length < out.len) {
		si_arraySet(out, length, "}");
		length += 1;
	}

	return SI_STR_LEN(out.data, length);
#endif
}

inline
siOsString si_stringToOsStr(siString str, siArray(siOsChar) out) {
	isize copied;
	return si_stringToOsStrEx(str, out, &copied);
}


SIDEF
siOsString si_stringToOsStrEx(siString str, siArray(siOsChar) out, isize* copied) {
	SI_ASSERT(out.typeSize == si_sizeof(siOsChar));
	SI_ASSERT_NOT_NIL(copied);

#ifdef SI_SYSTEM_IS_WINDOWS
	SI_STOPIF(out.len == 0, *copied = -1; return nil);

	siUtf16String res = si_utf8ToUtf16StrEx(str, true, out);
	*copied = res.len;

	return (siOsString)res.data;

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	SI_STOPIF(out.len == 0, *copied = -1; return nil);

	isize len = si_memcopyStr_s(si_sliceTo(out, out.len - 1), str);
	*((u8*)out.data + len) = '\0';
	*copied = len + 1;

	return (siOsChar*)out.data;

#else
	*copied = 0;
	return (siOsString)str.data;
#endif
}

#endif /* SI_IMPLEMENTATION_STRING */

#ifdef SI_IMPLEMENTATION_OPTIONAL

SIDEF
siError si__errorDeclare(i32 error, siErrorProc proc, void* userData, siCallerLoc call) {
	siError res;
	res.code = error;
#ifndef SI_NO_ERROR_STRUCT
	res.time = si_timeNowUTC();
	res.location = call;
#else
	SI_UNUSED(call);
#endif

#ifndef SI_NO_ERROR_LOGS
	if (proc != nil) {
		bool status = proc(&res, userData);
		SI_STOPIF(!status, si_exit(1));
	}
	else {
		siPrintColor red = si_printColor3bit(siPrintColor3bit_Red, true);
		si_eprintf(
			SI_STR("%CError at \"%L\"%C: Number '%i'.\n"),
			red, res.location, res.code
		);
	}
#else
	SI_UNUSED(proc); SI_UNUSED(userData);

#endif

	return res;
}

#endif /* SI_IMPLEMENTATION_OPTIONAL */

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
		return SI_UTF32_INVALID;
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
			return SI_UTF8_INVALID;
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
	else return SI_UTF8_INVALID;


	return res;
}

inline
siUtf16String si_utf8ToUtf16Str(siUtf8String str, siArray(u16) out) {
	return si_utf8ToUtf16StrEx(str, false, out);
}

SIDEF
siUtf16String si_utf8ToUtf16StrEx(siUtf8String str, bool nullTerm, siArray(u16) out) {
	SI_ASSERT_STR(str);
	SI_ASSERT_ARR_TYPE(out, u16);
	SI_STOPIF(str.len == 0 || out.len == 0, return SI_ARR_LEN((u16*)nil, 0));

	isize capacity = out.len - (i32)(nullTerm & true);
	u16* data = (u16*)out.data;

	isize inpI = 0, outI = 0;
	while (inpI < str.len && outI < capacity) {
		siUtf32Char utf32 = si_utf8Decode(&str.data[inpI]);

		i32 codepoint = utf32.codepoint;
		inpI += utf32.len;

		if (codepoint == 0) {
			break;
		}
		else if (codepoint < 0xFFFF) {
			data[outI] = (u16)codepoint;
			outI += 1;
		}
		else {
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

	return SI_ARR_LEN(data, outI);
}

inline
siUtf8String si_utf16ToUtf8Str(siUtf16String str, siArray(u8) out) {
	return si_utf16ToUtf8StrEx(str, false, out);
}

SIDEF
siUtf8String si_utf16ToUtf8StrEx(siUtf16String str, bool nullTerm, siArray(u8) out) {
	SI_ASSERT_ARR_TYPE(str, u16);
	SI_ASSERT_ARR_TYPE(out, u8);
	SI_STOPIF(str.len == 0 || out.len == 0, return SI_STR_EMPTY);

	isize capacity = out.len - (i32)(nullTerm & true);
	isize inpI = 0, outI = 0;
	u8* data = (u8*)out.data;

	while (inpI < str.len) {
		siUtf8Char utf8 = si_utf16Encode((u16*)si_arrayGet(str, inpI));
		if (outI + utf8.len > capacity || utf8.codepoint[0] == '\0') {
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
isize si_utf16ToUtf8StrLen(siUtf16String str) {
	SI_ASSERT_ARR_TYPE(str, u16);

	isize count = 0;
	isize i = 0;

	while (i < str.len) {
		siUtf32Char character = si_utf16Decode((u16*)si_arrayGet(str, i));
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
	i = si__binarySearch(lowerSingle, countof(lowerSingle) / 2, rune, 2);
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
bool si_runeIsLower(siRune rune) {
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
bool si_runeIsUpper(siRune rune) {
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
bool si_runeIsSpace(siRune rune) {
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
bool si_runeIsDigit(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsDigit((char)rune) : false;
}

inline
bool si_runeIsHex(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsHex((char)rune) : false;
}
SIDEF
bool si_runeIsAlpha(siRune rune) {
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
bool si_runeIsAlphanumeric(siRune rune) {
	return si_runeIsDigit(rune) || si_runeIsAlpha(rune);
}
inline
bool si_runeIsPunctuation(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsPrintable((char)rune) : false;
}
inline
bool si_runeIsControl(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsPrintable((char)rune) : false;
}
inline
bool si_runeIsPrintable(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsPrintable((char)rune) : false;
}
inline
bool si_runeIsGraphical(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charIsGraphical((char)rune) : false;
}
inline
bool si_runeIsDelimiter(siRune rune) {
	return !(si_runeIsAlphanumeric(rune) || rune == '@' || rune == '#' || rune == '$');
}

inline
i32 si_runeDigitToInt(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charDigitToInt((char)rune) : -1;
}
inline
i32 si_runeBase32ToInt(siRune rune) {
	return (rune <= SI_ASCII_MAX) ? si_charBase32ToInt((char)rune) : -1;
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
bool si_charIsLower(char c) {
	return (characterTraits[(u8)c] & SI__LETL) != 0;
}

inline
bool si_charIsUpper(char c) {
	return (characterTraits[(u8)c] & SI__LETU) != 0;
}

inline
bool si_charIsSpace(char c) {
	return (characterTraits[(u8)c] & SI__SPAC) != 0;
}

inline
bool si_charIsDigit(char c) {
	return (characterTraits[(u8)c] & SI__NUME) != 0;
}

inline
bool si_charIsHex(char c) {
	return (characterTraits[(u8)c] & SI__HEXA) != 0;
}

inline
bool si_charIsAlpha(char c) {
	return (characterTraits[(u8)c] & (SI__LETL | SI__LETU)) != 0;
}

inline
bool si_charIsAlphanumeric(char c) {
	return (characterTraits[(u8)c] & (SI__LETL | SI__LETU | SI__NUME)) != 0;
}

inline
bool si_charIsPunctuation(char c) {
	return (characterTraits[(u8)c] & SI__PUNC) != 0;
}

inline
bool si_charIsControl(char c) {
	return (characterTraits[(u8)c] & SI__CTRL) != 0;
}

inline
bool si_charIsPrintable(char c) {
	return (characterTraits[(u8)c] & (SI__LETL | SI__LETU | SI__NUME | SI__PUNC | SI__SPAC)) != 0;
}

inline
bool si_charIsGraphical(char c) {
	return (characterTraits[(u8)c] & (SI__LETL | SI__LETU | SI__NUME | SI__PUNC)) != 0;
}


inline
bool si_charIsDelimiter(char c) {
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
i32 si_charBase32ToInt(char c) {
	u8 trait = characterTraits[(u8)c];

	if (trait & SI__NUME) {
		return si_charDigitToInt(c);
	}
	else if ((trait & SI__LETL) == SI__LETL) {
		return c - 'a' + 10;
	}
	else if ((trait & SI__LETU) == SI__LETU) {
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

#ifdef SI_IMPLEMENTATION_HASHING

SIDEF
u32 si_fnv32(const void* data, isize len) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(len);

	u32 hash = 0x811C9DC5;
	const u8* ptr = (u8*)data;
	for_range (i, 0, len) {
		hash = (hash * 0x01000193) ^ ptr[i];
	}

	return hash;
}

SIDEF
u32 si_fnv32a(const void* data, isize len) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(len);

	u32 hash = 0x811C9DC5;
	const u8* ptr = (u8*)data;
	for_range (i, 0, len) {
		hash = (hash ^ ptr[i]) * 0x01000193;
	}

	return hash;
}

SIDEF
u64 si_fnv64(const void* data, isize len) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(len);

	u64 hash = 0xCBF29CE484222325;
	const u8* ptr = (u8*)data;
	for_range (i, 0, len) {
		hash = (hash * 0x100000001B3) ^ ptr[i];
	}

	return hash;
}

SIDEF
u64 si_fnv64a(const void* data, isize len) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(len);

	u64 hash = 0xCBF29CE484222325;
	const u8* ptr = (u8*)data;
	for_range (i, 0, len) {
		hash = (hash ^ ptr[i]) * 0x100000001B3;
	}

	return hash;
}

inline
u32 si_murmur32(const void* data, isize len) {
	return si_murmur32Ex(data, len, 0x9747B28C);
}


force_inline
u32 si__murmur32Scramble(u32 key) {
	key *= 0xCC9E2D51;
	key = (key << 15) | (key >> 17);
	key *= 0x1B873593;
	return key;
}

SIDEF
u32 si_murmur32Ex(const void* data, isize len, u32 seed) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(len);

	u32 hash = seed;
	const u32* blocks = (const u32*)data;

	for_range (i, 0, len / 4) {
		u32 key = blocks[i];
		hash ^= si__murmur32Scramble(key);
		hash = ((hash << 13) | (hash >> 19)) * 5 + 0xE6546B64;
	}

	u32 key = 0;
	for (isize i = len & 3; i >= 0; i -= 1) {
		key <<= 8;
		key |= ((const u8*)data)[i - 1];
	}

	hash ^= si__murmur32Scramble(key);
	hash ^= (u32)len;
	hash ^= hash >> 16;
	hash *= 0x85EBCA6B;
	hash ^= hash >> 13;
	hash *= 0xC2B2AE35;
	hash ^= hash >> 16;
	return hash;
}

#if SI_ARCH_IS_32BIT
force_inline
void si__murmur64Scramble(u32* h, const u32* d) {
	u32 k = *d;
	k *= 0x5BD1E995;
	k ^= k >> 24;
	k *= 0x5BD1E995;
	*h *= 0x5BD1E995;
	*h ^= k;
}
#endif


SIDEF
u64 si_murmur64(const void* data, isize len) {
	return si_murmur64Ex(data, len, 0x9747B28C);
}
SIDEF
u64 si_murmur64Ex(const void* data, isize len, u64 seed) {
	SI_ASSERT_NOT_NIL(data);
	SI_ASSERT_NOT_NEG(len);

#if SI_ARCH_IS_64BIT
	const u64 m = 0xC6A4A7935BD1E995;
	const i32 r = 47;

	u64 hash = seed ^ ((u64)len * m);


	for_range (i, 0, len / 8) {
		u64 k = ((const u64*)data)[i];

		k *= m;
		k ^= k >> r;
		k *= m;

		hash ^= k;
		hash *= m;
	}

	isize i = (len & 7u) - 1;
	hash ^= (u64)(((const u8*)data)[i] << (i * 8));

	hash ^= hash >> r;
	hash *= m;
	hash ^= hash >> r;

	return hash;
#else

	u32 h[2] = {(u32)seed ^ (u32)len, seed >> 32};

	const u32* d = (const u32*)data;

	while (len >= 8) {
		for_range (i, 0, 2) {
			si__murmur64Scramble(&h[i], d);
			d += 1;
			len -= 4;
		}
	}

	if (len >= 4) {
		si__murmur64Scramble(&h[0], d);
		d += 1;
		len -= 4;
	}

	u32 const m = 0x5BD1E995;
	isize i = len - 1;
	h[0] ^= (u32)(((const u8*)data)[i] << (i * 8));
	h[1] *= m;

	h[0] ^= h[1] >> 18;
	h[0] *= m;
	h[1] ^= h[0] >> 22;
	h[1] *= m;
	h[0] ^= h[1] >> 17;
	h[0] *= m;
	h[1] ^= h[0] >> 19;
	h[1] *= m;

	return ((u64)h[0] << 32) | h[1];
#endif
}

#endif /* SI_IMPLEMENTATION_HASHING */

#ifdef SI_IMPLEMENTATION_MAP


SIDEF
siMapAny si_mapMakeFull(const void* input, isize len, isize structTypeSize, isize valueTypeSize, siAllocator alloc) {
	siMapAny map = si_mapReserve(valueTypeSize, len, alloc);
	if (map.entries == nil) { return map; }

	for_range (i, 0, len) {
		const void* entry = si_pointerAddConst(input, i * structTypeSize);
		si_mapSet(&map, *(siString*)entry, si_pointerAddConst(entry, si_sizeof(siString)));
	}

	return map;
}

#define SI_HASH_NONE UINT32_MAX

SIDEF
siMapAny si_mapReserve(isize typeSize, isize capacity, siAllocator alloc) {
	SI_ASSERT_NOT_NEG(typeSize);
	SI_ASSERT_NOT_NEG(capacity);

	siMapAny map;
	map.alloc = alloc;
	map.len = 0;
	map.capacity = si_nextPow2(capacity);
	map.typeSize = typeSize;

	isize lenEntries = si_alignForward(si_sizeof(*map.entries) * map.capacity, SI_DEFAULT_ALIGNMENT),
		  lenHashes = si_alignForward(si_sizeof(*map.hashes) * map.capacity, SI_DEFAULT_ALIGNMENT);

	void* ptr = si_allocNonZeroed(alloc, lenEntries + lenHashes + map.typeSize * map.capacity);
	if (ptr == nil) { return SI_TYPE_ZERO(siMapAny); }

	map.entries = (siMapEntry*)ptr;
	map.hashes = (u32*)si_pointerAdd(map.entries, lenEntries);
	map.values = (void*)si_pointerAdd(map.hashes, lenHashes);

	for_range (i, 0, map.capacity) {
		map.entries[i].hash = SI_HASH_NONE;
		map.entries[i].next = SI_HASH_NONE;
		map.hashes[i] = SI_HASH_NONE;
	}

	return map;
}

force_inline
u32 si__mapHash(siString name) {
	return si_fnv32a(name.data, name.len) & 0x7FFFFFFF;
}

typedef struct { u32 hashIndex, entryIndex, entryPrev; } __siMapSearch;

siIntern
__siMapSearch si__mapFind(siMapAny map, u32 hash, siString key) {
	__siMapSearch res = {SI_HASH_NONE, SI_HASH_NONE, SI_HASH_NONE};
	SI_STOPIF(map.capacity == 0, return res);

	res.hashIndex = hash & (u32)(map.capacity - 1);
	res.entryIndex = map.hashes[res.hashIndex];

	while (res.entryIndex != SI_HASH_NONE) {
		siMapEntry entry = map.entries[res.entryIndex];
		if (entry.hash == hash && si_stringEqual(entry.key, key)) {
			return res;
		}

		res.entryPrev = res.entryIndex;
		res.entryIndex = entry.next;
	}

	return res;
}


SIDEF
void* si_mapGet(siMapAny map, siString name) {
	return si_mapGetHash(map, name, si__mapHash(name));
}
SIDEF
void* si_mapGetHash(siMapAny map, siString name, u32 hash) {
	__siMapSearch search = si__mapFind(map, hash, name);
	return (search.entryIndex != SI_HASH_NONE)
		? si_pointerAdd(map.values, (isize)search.entryIndex * map.typeSize)
		: nil;
}

SIDEF
void* si_mapSet(siMapAny* map, siString name, const void* value) {
	return si_mapSetHash(map, name, value, si__mapHash(name));
}

SIDEF
void* si_mapSetHash(siMapAny* map, siString name, const void* value,
		u32 hash) {
	SI_ASSERT_NOT_NIL(map);
	SI_ASSERT_STR(name);
	SI_ASSERT_NOT_NIL(value);
	SI_ASSERT_MSG(map->len < map->capacity, "The capacity of the map has been surpassed.");

	__siMapSearch find = si__mapFind(*map, hash, name);

	u32 index;
	if (find.entryIndex != SI_HASH_NONE) {
		index = find.entryIndex;
	}
	else {
		index = (u32)map->len;

		siMapEntry* entry = &map->entries[index];
		entry->key = name;
		entry->hash = hash;
		entry->next = SI_HASH_NONE;
		map->len += 1;

		if (find.entryPrev != SI_HASH_NONE) {
			map->entries[find.entryPrev].next = index;
		}
		else {
			map->hashes[find.hashIndex] = index;
		}
	}

	void* res = si_pointerAdd(map->values, (isize)index * map->typeSize);
	si_memcopy(res, value, map->typeSize);

	return res;
}

SIDEF
void si_mapErase(siMapAny* map, siString name) {
	si_mapEraseHash(map, name, si__mapHash(name));
}

SIDEF
void si_mapEraseHash(siMapAny* map, siString name, u32 hash) {
	SI_ASSERT_NOT_NIL(map);
	SI_ASSERT_STR(name);
	SI_STOPIF(map->capacity == 0, return);

	__siMapSearch find = si__mapFind(*map, hash, name);
	SI_STOPIF(find.entryIndex == SI_HASH_NONE, return);

	if (find.entryPrev == SI_HASH_NONE) {
		map->hashes[find.hashIndex] = map->entries[find.entryIndex].next;
	}
	else {
		map->entries[find.entryPrev].next = map->entries[find.entryIndex].next;
	}

	map->len -= 1;
	if ((isize)find.entryIndex == map->len) {
		return;
	}

	siMapEntry* entryLast = &map->entries[map->len];
	map->entries[find.entryIndex] = *entryLast;

	__siMapSearch last = si__mapFind(*map, entryLast->hash, entryLast->key);
	if (last.entryPrev != SI_HASH_NONE) {
		map->entries[last.entryPrev].next = find.entryIndex;
	}
	else {
		map->hashes[last.hashIndex] = find.entryIndex;
	}
}

SIDEF
void si_mapClear(siMapAny* map) {
	SI_ASSERT_NOT_NIL(map);

	for_range (i, 0, map->len) {
		siMapEntry entry = map->entries[i];
		__siMapSearch find = si__mapFind(*map, entry.hash, entry.key);
		map->hashes[find.entryIndex] = SI_HASH_NONE;
	}
	map->len = 0;
}

inline
void si_mapFree(siMapAny map) {
	si_free(map.alloc, map.entries);
}

#undef SI_HASH_NONE

#endif /* SI_IMPLEMENTATION_MAP */

#ifdef SI_IMPLEMENTATION_BIT

#define SI_BIT_DEC_AMOUNT_IMPL(func, def, body, retVal) \
	def u8 si__##func##_u8(u8 x, i32 amount) { body return (u8)(retVal); } \
	def u16 si__##func##_u16(u16 x, i32 amount) { body return (u16)(retVal); } \
	def u32 si__##func##_u32(u32 x, i32 amount) { body return (u32)(retVal); } \
	def u64 si__##func##_u64(u64 x, i32 amount) { body return (u64)(retVal); } \
	def usize si__##func##_usize(usize x, i32 amount) { body return (usize)(retVal); }

#define SI_BIT_DEC_TYPE_IMPL(func, def, body) \
	def u8 si__##func##_u8(u8 x)    { u64 res = 0; body return (u8)res; } \
	def u16 si__##func##_u16(u16 x) { u64 res = 0; body return (u16)res; } \
	def u32 si__##func##_u32(u32 x) { u64 res = 0; body return (u32)res; } \
	def u64 si__##func##_u64(u64 x) { u64 res = 0; body return (u64)res; } \
	def usize si__##func##_usize(usize x) { usize res = 0; body return (u64)res; }

SI_BIT_DEC(countOnes, inline, {
	isize count = 0;
	while (x) {
		count += (isize)(x & 1);
		x >>= 1;
	}
	return count;
})

SI_BIT_DEC(countLeadingOnes, inline, {
	isize count = 0;

	for (isize i = si_sizeof(x) * 8 - 1; i >= 0; i -= 1) {
		if (x & SI_BIT(i)) { count += 1; }
		else { break; }
	}

	return count;
})

SI_BIT_DEC(countLeadingZeros, inline, {
	isize count = 0;

	for (isize i = si_sizeof(x) * 8 - 1; i >= 0; i -= 1) {
		if ((x & SI_BIT(i)) == 0) { count += 1; }
		else { break; }
	}

	return count;
})

SI_BIT_DEC(countTrailingOnes, inline, {
	isize count = 0;

	while (x & 1) {
		count += 1;
		x >>= 1;
	}

	return count;
})

SI_BIT_DEC(countTrailingZeros, inline, {
	if (x == 0) { return si_sizeof(x) * 8; }
	isize count = 0;

	while ((x & 1) == 0) {
		count += 1;
		x >>= 1;
	}

	return count;
})

SI_BIT_DEC_AMOUNT_IMPL(bitsRotateLeft, inline,
	{
		SI_ASSERT_NOT_NEG(amount);
		amount &= si_sizeof(x) * 8 - 1;
		if (!amount) { return x; }
	},
	(x << amount) | (x >> (si_sizeof(x) * 8 - amount))
)

SI_BIT_DEC_AMOUNT_IMPL(bitsRotateRight, inline,
	{
		SI_ASSERT_NOT_NEG(amount);
		amount &= si_sizeof(x) * 8 - 1;
		if (!amount) { return x; }
	},
	(x >> amount) | (x << (si_sizeof(x) * 8 - amount))
)

SI_BIT_DEC_TYPE_IMPL(bitsReverse, inline, {
	for_range (i, 0, si_sizeof(x) * 8) {
		res <<= 1;
		res |= (x & 1);
		x >>= 1;
	}
})

SI_BIT_DEC_ARRAY(bytesToArray, inline, {
	isize len = si_sizeof(x);
	u8* res = si_allocArrayNonZeroed(alloc, u8, len);

	u64 copy = x;
	for_range (i, 0, len - 1) {
		res[i] = (u8)(copy & 0xFF);
		copy >>= 8;
	}

	return SI_ARR_LEN(res, len);
})

#undef SI_BIT_DEC
#undef SI_BIT_DEC_ARRAY
#undef SI_BIT_DEC_TYPE_IMPL
#undef SI_BIT_DEC_AMOUNT_IMPL

SIDEF
u64 si_bytesFromArray(siArray(u8) bytes) {
	SI_ASSERT_ARR_TYPE(bytes, u8);
	SI_ASSERT(bytes.len <= si_sizeof(u64));

	u64 res = 0;
	u8 byte;
	for_eachArr (byte, bytes) {
		res <<= 8;
		res |= byte;
	}

	return res;
}

inline
u16 si_swap16(u16 x) {
	return (u16)(((x >> 8) & 0xFFu) | ((x & 0xFFu) << 8));
}
inline
u32 si_swap32(u32 x) {
	return (u32)(
		((x & 0xFF000000u) >> 24)  | ((x & 0x00FF0000u) >>  8) |
		((x & 0x0000FF00u) <<  8)  | ((x & 0x000000FFu) << 24)
	);
}
inline
u64 si_swap64(u64 x) {
   return (u64)(
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
isize si_numLenInt(i64 num) {
	isize len;
	if (num < 0) {
		num = -num;
		len = 1;
	}
	else { len = 0; }

	if (num < 10) return 1 + len;
	if (num < 100) return 2 + len;
	if (num < 1000) return 3 + len;
	if (num < 10000) return 4 + len;
	if (num < 100000) return 5 + len;
	if (num < 1000000) return 6 + len;
	if (num < 10000000) return 7 + len;
	if (num < 100000000) return 8 + len;
	if (num < 1000000000) return 9 + len;
	if (num < 10000000000) return 10 + len;
	if (num < 100000000000) return 11 + len;
	if (num < 1000000000000) return 12 + len;
	if (num < 10000000000000) return 13 + len;
	if (num < 100000000000000) return 14 + len;
	if (num < 1000000000000000) return 15 + len;
	if (num < 10000000000000000) return 16 + len;
	if (num < 100000000000000000) return 17 + len;
	if (num < 1000000000000000000) return 18 + len;
	return 19 + len;
}
inline
isize si_numLenIntEx(i64 num, i32 base) {
	if (num < 0) {
		num = -num;
		return 1 + si_numLenUintEx((u64)num, base);
	}
	return si_numLenUintEx((u64)num, base);
}

SIDEF
isize si_numLenUint(u64 num) {
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
isize si_numLenUintEx(u64 num, i32 base) {
	SI_ASSERT_NOT_NEG(base);

	isize count = 0;
	do {
		count += 1;
		num /= (u32)base;
	} while (num != 0);

	return count;
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

#ifdef SI_IMPLEMENTATION_TIME

siString si__timeMonthNames[] = {
	SI_STRC("January"), SI_STRC("February"), SI_STRC("March"), SI_STRC("April"),
	SI_STRC("May"), SI_STRC("June"), SI_STRC("July"), SI_STRC("August"),
	SI_STRC("September"), SI_STRC("October"), SI_STRC("November"), SI_STRC("December"),
};

siString si__timeMonthNamesShrt[] = {
	SI_STRC("Jan"), SI_STRC("Feb"), SI_STRC("Mar"), SI_STRC("Apr"), SI_STRC("May"),
	SI_STRC("Jun"), SI_STRC("Jul"), SI_STRC("Aug"), SI_STRC("Sep"), SI_STRC("Oct"),
	SI_STRC("Nov"), SI_STRC("Dec"),
};

siString si__timeWeekNames[] = {
	SI_STRC("Sunday"), SI_STRC("Monday"), SI_STRC("Tuesday"), SI_STRC("Wednesday"),
	SI_STRC("Thursday"), SI_STRC("Friday"), SI_STRC("Saturday"),
};

siString si__timeWeekNamesShrt[] = {
	SI_STRC("Sun"), SI_STRC("Mon"), SI_STRC("Tue"), SI_STRC("Wed"), SI_STRC("Thu"),
	SI_STRC("Fri"), SI_STRC("Sat"),
};

siString si__timeAM_PM_Names[] = {
	SI_STRC("AM"), SI_STRC("PM"), SI_STRC("am"), SI_STRC("pm")
};


siString si__timeTimeUnits[] = {
	SI_STRC("ns"), SI_STRC("μs"), SI_STRC("ms"), SI_STRC("s"), SI_STRC("min"),
	SI_STRC("h"), SI_STRC("d"), SI_STRC("m"), SI_STRC("y")
};

siString* SI_NAMES_MONTHS_FULL = si__timeMonthNames;
siString* SI_NAMES_MONTHS_SHRT = si__timeMonthNamesShrt;
siString* SI_NAMES_DAYS_FULL = si__timeWeekNames;
siString* SI_NAMES_DAYS_SHRT = si__timeWeekNamesShrt;
siString* SI_NAMES_AM_PM = si__timeAM_PM_Names;
siString* SI_NAMES_TIME_UNITS = si__timeTimeUnits;

#if SI_SYSTEM_IS_WINDOWS
siIntern
siTime si__win32ToSili(i64 time) {
	return (siTime)(((time) - 116444736000000000) * 100);
}

#elif SI_ARCH_IS_WASM
siIntern
i64 si__wasiRdtsc(void) {
	__wasi_timestamp_t time;
	__wasi_errno_t res = __wasi_clock_time_get(__WASI_CLOCKID_PROCESS_CPUTIME_ID, 1, &time);

	return (res == 0) ? (siTime)time : 0;
}
#endif

#ifndef SI_IMPL_RDTSC
inline
i64 si_RDTSC(void) {
#if SI_COMPILER_CHECK_MIN(MSVC, 12, 0, 0)
	#if !SI_SYSTEM_IS_WASM
		return __rdtsc();
	#else
		return si__wasiRdtsc();
	#endif
#elif SI_COMPILER_GCC || SI_COMPILER_CLANG
	/* NOTE(EimaMei): Credit goes to gb.h for the i386 and PPC code. */
	#if SI_ARCH_I386
		i64 res;
		__asm__ volatile (".byte 0x0f, 0x31" : "=A" (res));
		return res;

	#elif SI_ARCH_AMD64
		i64 high, low;
		__asm__ volatile ("rdtsc" : "=a"(low), "=d"(high));
		return (high << 32) | low;

	#elif SI_ARCH_IS_PPC
		i32 high, low, tmp;
		__asm__ volatile (
			"0:\n"
			"\tmftbu %0\n"
			"\tmftb %1\n"
			"\tmftbu %2\n"
			"\tcmpw %2, %0\n"
			"\tbne 0b\n"
			: "=r"(high), "=r"(low), "=r"(tmp)
		);
		return ((i64)high << 32) | low;

	#elif SI_ARCH_ARM64
		i64 res;
		__asm__ volatile ("mrs %0, cntvct_el0" : "=r"(res));
		return res;

	#elif SI_ARCH_IS_RISC
		i64 res = 0;
		__asm__ volatile ("rdcycle %0" : "=r"(res));
		return res;

	#elif SI_ARCH_IS_WASM
		return si__wasiRdtsc();

	#elif SI_SYSTEM_IS_WINDOWS
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return count.QuadPart;

	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
		struct timespec tv = {0};
		clock_gettime(CLOCK_MONOTONIC, &tv);
		return SI_TIME_SEC(tv.tv_sec) + tv.tv_nsec;

#else
		return 0;

	#endif

#else
	#if SI_SYSTEM_IS_WINDOWS
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return count.QuadPart;

	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
		struct timespec tv = {0};
		clock_gettime(CLOCK_MONOTONIC, &tv);
		return SI_TIME_SEC(tv.tv_sec) + tv.tv_nsec;

	#elif SI_ARCH_IS_WASM
		return si__wasiRdtsc();

	#else
		return 0;
	#endif
#endif
}
#endif

inline
i64 si_RDTSCP(i32* proc) {
	SI_ASSERT_NOT_NIL(proc);

#if SI_COMPILER_CHECK_MIN(MSVC, 12, 0, 0)
	#if !SI_SYSTEM_IS_WASM
		return __rdtscp((u32*)proc);
	#else
		*proc = 0;
		return si_RDTSC();
	#endif
#elif SI_COMPILER_GCC || SI_COMPILER_CLANG
	#if SI_ARCH_I386
		i32 high, low;
		__asm__ volatile (
			"rdtscp"
			: "=a"(high), "=d"(low), "=c"(*proc)
		);
		return ((i64)high << 32) | (i64)low;

	#elif SI_ARCH_AMD64
		i64 high, low;
		__asm__ volatile ("rdtscp" : "=a"(low), "=d"(high), "=c"(*proc));
		return (high << 32) | low;

	#elif SI_ARCH_IS_PPC
		u32 aux;

		/* TODO(EimaMei): Check if this is even accurate. */
		__asm__ volatile ("mfspr %0, 1023" : "=r"(aux));
		*proc = (i32)aux;

		return si_RDTSC();

	#elif SI_ARCH_ARM64
		u64 mpidr;

		/* TODO(EimaMei): Check if this is even accurate. */
		__asm__ volatile ("mrs %0, mpidr_el1" : "=r"(mpidr));
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
#endif
}


SIDEF
siTime si_clock(void) {
	u64 clock_hz  = (u64)si_cpuClockSpeed();
	u64 cycles = (u64)si_RDTSC();
	u64 seconds = cycles / clock_hz;
	u64 rem_cycles = cycles % clock_hz;

	return (i64)(seconds * SI_SECOND + (rem_cycles * SI_SECOND) / clock_hz);
}

SIDEF
void si_timePrint(siTime time, siString label, siCallerLoc call) {
	const siTimeUnitScale scale = si_timeGetUnit(time);
	si_printfLn(
		"%s: %L: TIME: %.2f %2s",
		(label.len != 0) ? label : SI_STR("si_timeStampPrintSince"),
		call, (f64)time / (f64)scale.threshold, si_timeUnitStr(scale.unit)
	);
}

SIDEF
siTime si_timePrintSince(siTime timestamp, siString label, siCallerLoc call) {
	siTime end = si_clock();
	siTime diff = (end - timestamp);

	si_timePrint(diff, label, call);
	return diff;
}

SIDEF
void si_sleep(siTime time) {
	SI_ASSERT_NOT_NEG(time);

#if SI_SYSTEM_IS_WINDOWS
	if (time < SI_MILLISECOND) { return; }

	/* TODO(EimaMei): Replace this with a high-precision timer later on. */
	Sleep((u32)time / SI_MILLISECOND);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	if (time == 0) { return; }

	struct timespec ts = {time / SI_SECOND, time % SI_SECOND};
	nanosleep(&ts, &ts);

#elif SI_SYSTEM_WASI
	if (time == 0) { return; }

	__wasi_subscription_t in = SI_STRUCT_ZERO;
	in.u.tag = __WASI_EVENTTYPE_CLOCK;
	in.u.u.clock.id = __WASI_CLOCKID_MONOTONIC;
	in.u.u.clock.timeout = time;
	in.u.u.clock.precision = SI_NANOSECOND;

	__wasi_event_t events;
	size_t nevents;

	__wasi_errno_t result = __wasi_poll_oneoff(&in, &events, 1, &nevents);
	SI_UNUSED(result);
#elif SI_SYSTEM_EMSCRIPTEN
	if (time < SI_MILLISECOND) { return; }
	emscripten_sleep((unsigned int)(time / SI_MILLISECOND));

#elif SI_STANDARD_CHECK_MIN(C, C11) && !defined(__STDC_NO_THREADS__)
	if (time == 0) { return; }

	struct timespec ts = {time / SI_SECOND, time % SI_SECOND};
	thrd_sleep(&ts, nil);

#endif
}


SIDEF
siTime si_timeNowUTC(void) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME time;
	GetSystemTimePreciseAsFileTime(&time);

	return si__win32ToSili((i64)time.dwHighDateTime << 32 | (i64)time.dwLowDateTime);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	struct timespec spec;
	i32 res = clock_gettime(CLOCK_REALTIME, &spec);

	return (res == 0) ? SI_TIME_SEC(spec.tv_sec) + spec.tv_nsec : 0;

#else
	return 0;

#endif
}

inline
siTime si_timeNowLocal(void) {
#if SI_SYSTEM_IS_WINDOWS
	FILETIME utc;
	GetSystemTimePreciseAsFileTime(&utc);
	FILETIME time;
	int res = FileTimeToLocalFileTime(&utc, &time);

	return (res)
		? si__win32ToSili((i64)time.dwHighDateTime << 32 | (i64)time.dwLowDateTime)
		: 0;

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	i64 timeNow = si_timeNowUTC();
	if (timezone == 0) {
		tzset();
	}

	return (timezone != 0)
		? timeNow - SI_TIME_SEC(timezone) + daylight * SI_HOUR
		: 0;
#else
	return 0;

#endif
}

SIDEF
bool si_timeYearIsLeap(i32 year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

SIDEF
i32 si_timeGetDayOfWeek(i32 year, i32 month, i32 day) {
	static i8 t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	if (month < 3 ) {
		year -= 1;
	}
	return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

static i8 SI__LUT_DAYS_IN_MONTH[2][12] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

SIDEF
siTimeCalendar si_timeToCalendar(i64 time) {
	/* TODO(EimaMei): Add leap second support??? */
	siTimeCalendar calendar;

	i32 daysSinceEpoch = (i32)(time / SI_DAY);

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
		i8* daysInMonths = SI__LUT_DAYS_IN_MONTH[si_timeYearIsLeap(calendar.years)];

		daysSinceEpoch = si_abs(i32, daysSinceEpoch);
		for_rangeRef (calendar.months, 0, 12) {
			i32 days = daysInMonths[calendar.months];
			SI_STOPIF(daysSinceEpoch < days, break);

			daysSinceEpoch -= days;
		}
		calendar.months += 1;
		calendar.days = daysSinceEpoch;
	}

	{
		i64 nanoseconds = time % SI_DAY;

		calendar.hours = (i32)(nanoseconds / SI_HOUR);
		nanoseconds %= SI_HOUR;

		calendar.minutes = (i32)(nanoseconds / SI_MINUTE);
		nanoseconds %= SI_MINUTE;

		calendar.seconds = (i32)(nanoseconds / SI_SECOND);
		nanoseconds %= SI_SECOND;

		calendar.nanoseconds = (i32)nanoseconds;
	}

	return calendar;
}


SIDEF
siTime si_calendarToTime(siTimeCalendar calendar) {
	SI_ASSERT(si_between(i32, calendar.years, 1677, 2262));
	SI_ASSERT(si_between(i32, calendar.months, 1, 12));
	SI_ASSERT(si_between(i32, calendar.days, 1, SI__LUT_DAYS_IN_MONTH[si_timeYearIsLeap(calendar.years)][calendar.months]));
	SI_ASSERT(si_between(i32, calendar.hours, 0, 23));
	SI_ASSERT(si_between(i32, calendar.minutes, 0, 59));
	SI_ASSERT(si_between(i32, calendar.seconds, 0, 59));
	SI_ASSERT(si_between(i32, calendar.nanoseconds, 0, SI_SECOND - 1));

	/* TODO(EimaMei): Add leap second support??? */
	siTime res = 0;
	res += calendar.nanoseconds;
	res += calendar.seconds * SI_SECOND;
	res += calendar.minutes * SI_MINUTE;
	res += calendar.hours * SI_HOUR;
	res += calendar.days * SI_DAY;

	i8* daysInMonths = SI__LUT_DAYS_IN_MONTH[si_timeYearIsLeap(calendar.years)];
	for_range (i, 0, calendar.months - 1) {
		res += daysInMonths[i] * SI_DAY;
	}

	isize amountOfYears = si_abs(i32, calendar.years - 1970);
	i32 direction = (calendar.years > 1970) ? 1 : -1;
	i32 year = 1970;
	for_range (i, 0, amountOfYears) {
		res += (365 + si_timeYearIsLeap(year - 1)) * SI_DAY;
		year += direction;
	}

	return res;
}

siIntern
isize si__timeToStr(siString fmt, isize* i, char letter, siArray(u8) out, isize len, i32 time) {
	if (*i < fmt.len - 1 && fmt.data[1 + *i] == letter) {
		*i += 1;
		if (time < 10) {
			si_arraySet(out, len, "0");
			return 1 + si_stringFromInt(si_sliceFrom(out, len + 1), time).len;
		}
	}

	return si_stringFromInt(si_sliceFrom(out, len), time).len;
}

siIntern
void si__timeTimezone(bool check, siTimeCalendar* calendar) {
	SI_STOPIF(check == false, return);

	if (calendar->hours != 12 && calendar->hours != 0) {
		calendar->hours %= 12;
	}
	else if (calendar->hours == 0) {
		calendar->hours = 12;
	}
}

SIDEF
siString si_timeToString(siTimeCalendar calendar, siString fmt, siArray(u8) out) {
	SI_ASSERT(si_between(i32, calendar.years, 1677, 2262));
	SI_ASSERT(si_between(i32, calendar.months, 1, 12));
	SI_ASSERT(si_between(i32, calendar.days, 1, SI__LUT_DAYS_IN_MONTH[si_timeYearIsLeap(calendar.years)][calendar.months]));
	SI_ASSERT(si_between(i32, calendar.hours, 0, 23));
	SI_ASSERT(si_between(i32, calendar.minutes, 0, 59));
	SI_ASSERT(si_between(i32, calendar.seconds, 0, 59));
	SI_ASSERT(si_between(i32, calendar.nanoseconds, 0, SI_SECOND - 1));
	SI_ASSERT_STR(fmt);
	SI_ASSERT_ARR_TYPE(out, u8);

	bool AMwasChecked = false;
	i32 ogHour = calendar.hours;
	isize len = 0;

	for_range (i, 0, fmt.len) {
		SI_STOPIF(len >= out.len, break);

		switch (fmt.data[i]) {
			case 'y': {
				isize ogI = i;
				while (i < fmt.len - 1 && fmt.data[i + 1] == 'y' && (i - ogI) != 4) { i += 1; }

				i32 year = (i - ogI != 1) ? calendar.years : calendar.years % 100;
				len += si_stringFromInt(si_sliceFrom(out, len), year).len;
			} break;

			case 'M': {
				SI_ASSERT(si_between(i32, calendar.months, 0, 12));

				isize ogI = i;
				while (i < fmt.len - 1 && fmt.data[i + 1] == 'M' && (i - ogI) != 2) { i += 1; }

				switch (i - ogI) {
					case 0: {
						len += si_stringFromInt(si_sliceFrom(out, len), calendar.months).len;
					} break;

					case 1: {
						if (calendar.months < 10) {
							si_arraySet(out, len, "0");
							len += 1;
						}

						len += si_stringFromInt(si_sliceFrom(out, len), calendar.months).len;
					} break;
				}
			} break;

			case 'd': {
				isize ogI = i;
				while (i < fmt.len - 1 && fmt.data[i + 1] == 'd' && (i - ogI) != 4) { i += 1; }

				switch (i - ogI) {
					case 0: {
						len += si_stringFromInt(si_sliceFrom(out, len), calendar.days).len;
					} break;

					case 1: {
						if (calendar.days < 10) {
							si_arraySet(out, len, "0");
							len += 1;
						}

						len += si_stringFromInt(si_sliceFrom(out, len), calendar.days).len;
					} break;

					case 2: {
						i32 wd = si_timeGetDayOfWeek(calendar.years, calendar.months, calendar.days);
						siString* str = &SI_NAMES_DAYS_SHRT[wd];
						len += si_memcopyStr_s(si_sliceFrom(out, len), *str);
					} break;

					case 4: {
						i32 wd = si_timeGetDayOfWeek(calendar.years, calendar.months, calendar.days);
						siString* str = &SI_NAMES_DAYS_FULL[wd];
						len += si_memcopyStr_s(si_sliceFrom(out, len), *str);
					} break;
				}
			} break;

			case 'h': {
				if (!AMwasChecked) {
					for_range (j, i + 1, fmt.len) {
						if (j + 1 < fmt.len &&
							((fmt.data[j] == 'a' && fmt.data[j + 1] == 'p')
							|| (fmt.data[j] == 'A' && fmt.data[j + 1] == 'P'))
						) {
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
				len += si_memcopyStr_s(si_sliceFrom(out, len), *str);

				si__timeTimezone(AMwasChecked, &calendar);
				AMwasChecked = true;
			} break;

			default:
				si_arraySet(out, len, &fmt.data[i]);
				len += 1;
		}
	}

	return SI_STR_LEN(out.data, len);
}

SIDEF
siTimeUnitScale si_timeGetUnit(i64 time) {
	static const siTimeUnitScale arr[] = {
		{siTimeUnit_Nanosecond, SI_NANOSECOND},
		{siTimeUnit_Microsecond, SI_MICROSECOND},
		{siTimeUnit_Millisecond, SI_MILLISECOND},
		{siTimeUnit_Second, SI_SECOND},
		{siTimeUnit_Minute, SI_MINUTE},
		{siTimeUnit_Hour, SI_HOUR},
		{siTimeUnit_Day, SI_DAY},
		{siTimeUnit_Month, SI_DAY * 30},
		{siTimeUnit_Year, SI_DAY * 365},
	};
	SI_STOPIF(time <= 0, return SI_COMP_LIT(siTimeUnitScale, -1, 0));

	for_range (i, 1, countof(arr)) {
		siTime converted = time / arr[i].threshold;
		if (converted == 0) {
			return arr[i - 1];
		}
	}

	return arr[countof(arr) - 1];
}

inline
siString si_timeUnitStr(siTimeUnit type) {
	SI_ASSERT(si_between(i32, type, 0, siTimeUnit_Year));
	return SI_NAMES_TIME_UNITS[type];
}


#endif /* SI_IMPLEMENTATION_TIME */

#ifdef SI_IMPLEMENTATION_PRINT

bool SI__COLOR_MODE = true;

inline
isize si_bprintfEx(siArray(u8) out, siString fmt, siArray(siAny) args, bool flush, bool newLine) {
	siBuilder b = si_builderFromArray(out);
	return si_sbprintfEx(&b, fmt, args, flush, newLine);
}

inline
isize si_bprintfLnEx(siArray(u8) out, siString fmt, siArray(siAny) args, bool flush) {
	siBuilder b = si_builderFromArray(out);
	return si_sbprintfLnEx(&b, fmt, args, flush);
}


inline
isize si_sbprintfEx(siBuilder* b, siString fmt, siArray(siAny) args, bool flush,
		bool newLine) {
	return si_wprintfEx(si_builderToWriter(b), fmt, args, flush, newLine);
}

inline
isize si_sbprintfLnEx(siBuilder* b, siString fmt, siArray(siAny) args, bool flush) {
	return si_wprintfLnEx(si_builderToWriter(b), fmt, args, flush);
}


inline
isize si_aprintfEx(siAllocator alloc, siString fmt, siArray(siAny) args, bool flush, bool newLine) {
	siBuilder b = si_builderMakeNone(alloc);
	return si_wprintfEx(si_builderToWriter(&b), fmt, args, flush, newLine);
}

inline
isize si_aprintfLnEx(siAllocator alloc, siString fmt, siArray(siAny) args, bool flush) {
	siBuilder b = si_builderMakeNone(alloc);
	return si_wprintfLnEx(si_builderToWriter(&b), fmt, args, flush);
}


force_inline
siAny si_vaNext(struct siFmtInfo* info) {
	siAny arg = *(siAny*)si_arrayGet(info->args, info->vaCount);
	info->vaCount += 1;

	return arg;
}

#define si_vaNextType(info, type) *(type*)si_vaNextType(info, si_typeid(type))

force_inline
const void* (si_vaNextType)(struct siFmtInfo* info, siTypeId type) {
	siAny arg = si_vaNext(info);
	SI_ASSERT_FMT(arg.type == type, "Expected type '%i', got type '%i'", type, arg.type);
	return arg.ptr;
}


siIntern
void si__fmtWrite(struct siFmtInfo* info, siString str) {
	SI_ASSERT_STR(str);

	if (info->width == 0) {
		info->n += si_streamWriteStr(info->writer, str);
		return;
	}

	u8 pad = (info->state & siFmtInfoState_Zero) ? '0' : ' ';

	if (info->state & siFmtInfoState_Minus && (str.len - info->width) < 0) {
		info->n += si_streamWriteStr(info->writer, str);
		for_range (i, str.len, info->width) {
			info->n += si_streamWriteByte(info->writer, pad);
		}

		info->width = 0;
		return ;
	}
	else if (str.len < info->width) {
		for_range (i, 0, info->width - str.len) {
			info->n += si_streamWriteByte(info->writer, pad);
		}
		info->n += si_streamWriteStr(info->writer, str);

		info->width = 0;
		return ;
	}

	info->n += si_streamWriteStr(info->writer, str);
	info->width = 0;
}

SIDEF
void si_fmtInt(siFmtInfo* info, u64 value, isize bitSize, i32 base, bool isSigned,
		bool uppercase) {
	siNumToStrOption options = 0;
	if (info->state & siFmtInfoState_Plus) { options |= siNumToStrOption_Plus; }
	if (info->state & siFmtInfoState_Hash) { options |= siNumToStrOption_Prefix; }

	siString table = uppercase ? SI_NUM_TABLE_UPPER : SI_NUM_TABLE_LOWER;
	siString str = si_stringFromBits(value, bitSize, isSigned, base, options, table, SI_ARR_STACK(256));

	if (info->state & siFmtInfoState_Prec) {
		if (info->precision == 0) return ;

		for_range (i, str.len, info->precision) {
			info->n += si_streamWriteByte(info->writer, '0');
		}
	}

	si__fmtWrite(info, str);
}

SIDEF
void si_fmtFloat(siFmtInfo* info, f64 num, i32 base) {
	siString str = si_stringFromFloatEx(num, base, info->precision, SI_ARR_STACK(512));
	si__fmtWrite(info, str);
}

SIDEF
void si_fmtRune(siFmtInfo* info, siRune rune) {
	siUtf8Char utf8 = si_utf8Encode(rune);
	si__fmtWrite(info, SI_STR_LEN(utf8.codepoint, utf8.len));
}

siIntern
void si__modifierIntVa(struct siFmtInfo* info, i32 base, bool uppercase) {
	siAny arg = si_vaNext(info);

	isize size;
	u64 value;
	b32 isSigned;
	switch (arg.type) {
		case siTypeId_i8:  value =  *(u8*)arg.ptr; isSigned = true; size =  8; break;
		case siTypeId_i16: value = *(u16*)arg.ptr; isSigned = true; size = 16; break;
		case siTypeId_i32: value = *(u32*)arg.ptr; isSigned = true; size = 32; break;
		case siTypeId_i64: value = *(u64*)arg.ptr; isSigned = true; size = 64; break;

		case siTypeId_u8:  value =  *(u8*)arg.ptr; isSigned = false; size =  8; break;
		case siTypeId_u16: value = *(u16*)arg.ptr; isSigned = false; size = 16; break;
		case siTypeId_u32: value = *(u32*)arg.ptr; isSigned = false; size = 32; break;
		case siTypeId_u64: value = *(u64*)arg.ptr; isSigned = false; size = 64; break;

		default: SI_PANIC_FMT("%i is not a valid type", arg.type);
	}

	si_fmtInt(info, value, size, base, isSigned, uppercase);
}

SIDEF
void si_fmtStr(siFmtInfo* info, siString str) {
	if (str.data == nil) {
		si__fmtWrite(info, SI_STR("(nil)"));
	}
	else if (str.len == 0) {
		si__fmtWrite(info, SI_STR("(empty)"));
	}
	else {
		if (info->state & siFmtInfoState_Prec) {
			str = si_substrTo(str, info->precision);
		}
		si__fmtWrite(info, str);
	}
}

SIDEF
void si_fmtCstring(siFmtInfo* info, cstring cstr) {
	if (cstr == nil) {
		si__fmtWrite(info, SI_STR("(nil)"));
		return ;
	}

	siString str = SI_STR(cstr);
	if (str.len == 0) {
		si__fmtWrite(info, SI_STR("(empty)"));
	}
	else {
		if (info->state & siFmtInfoState_Prec) {
			str = si_substrTo(str, info->precision);
		}
		si__fmtWrite(info, str);
	}
}

SIDEF
void si_fmtBool(siFmtInfo* info, bool b) {
	si__fmtWrite(info, si_stringFromBool(b));
}

SIDEF
void si_fmtPtr(siFmtInfo* info, void* ptr) {
	info->state ^= siFmtInfoState_Hash;
	si_fmtInt(info, (usize)ptr, si_sizeof(usize) * 8, 16, false);
}

SIDEF
void si_fmtSci(siFmtInfo* info, f64 num, bool uppercase) {
	i32 exponent = 0;
	while (num > 10) {
		num /= 10;
		exponent += 1;
	}

	si_fmtFloat(info, num);

	u8 remainder[4];
	remainder[0] = uppercase ? 'E' : 'e';
	remainder[1] = '+';
	if (exponent < 10) {
		remainder[2] = '0';
		remainder[3] = (u8)(exponent + '0');
	}
	else {
		remainder[2] = (u8)((exponent / 10) + '0');
		remainder[3] = (u8)((exponent % 10) + '0');
	}

	info->n += si_streamWriteStr(info->writer, SI_STR_LEN(remainder, 4));
}

SIDEF
void si_fmtCallerLoc(siFmtInfo* info, siCallerLoc loc) {
	info->n += si_streamWriteStr(info->writer, loc.filename);
	info->n += si_streamWriteByte(info->writer, ':');
	info->n += si_streamWriteInt(info->writer, loc.line);
	info->n += si_streamWriteByte(info->writer, ':');
	info->n += si_streamWriteStr(info->writer, loc.function);
}

SIDEF
void si_fmtValue(struct siFmtInfo* info, siAny arg) {
	switch (arg.type) {
		case siTypeId_char:  si_fmtRune(info, *(char*)arg.ptr); break;

		case  siTypeId_u8:   si_fmtInt(info,  *(u8*)arg.ptr,  8, 10, false); break;
		case  siTypeId_u16:  si_fmtInt(info, *(u16*)arg.ptr, 16, 10, false); break;
		case  siTypeId_u32:  si_fmtInt(info, *(u32*)arg.ptr, 32, 10, false); break;
		case  siTypeId_u64:  si_fmtInt(info, *(u64*)arg.ptr, 64, 10, false); break;

		case  siTypeId_i8:   si_fmtInt(info,  *(u8*)arg.ptr,  8); break;
		case siTypeId_i16:   si_fmtInt(info, *(u16*)arg.ptr, 16); break;
		case siTypeId_i32:   si_fmtInt(info, *(u32*)arg.ptr, 32); break;
		case siTypeId_i64:   si_fmtInt(info, *(u64*)arg.ptr, 64); break;

		case siTypeId_f32:   si_fmtFloat(info, (f64)*(f32*)arg.ptr); break;
		case siTypeId_f64:   si_fmtFloat(info, *(f64*)arg.ptr); break;

		case siTypeId_bool:  si_fmtBool(info, *(bool*)arg.ptr); break;

		case siTypeId_string:  si_fmtStr(info, *(siString*)arg.ptr); break;
		case siTypeId_cstring: si_fmtCstring(info,*(char**)arg.ptr); break;
		case siTypeId_voidptr: si_fmtPtr(info, *(void**)arg.ptr); break;

		case siTypeId_siCallerLoc: si_fmtCallerLoc(info, *(siCallerLoc*)arg.ptr); break;

		default: SI_PANIC();
	}
}

force_inline
u64 si__pow10(i32 exponent) {
	static const u64 allPossibleValues[] = {
		1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
		10000000000ULL, 100000000000ULL, 1000000000000ULL, 10000000000000ULL,
		100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL
	};
	SI_ASSERT_NOT_NEG(exponent);
	SI_ASSERT_MSG(exponent < countof(allPossibleValues), "Exponent must be in range from 0 to 18 to not overflow.");

	return allPossibleValues[exponent];
}

force_inline
f64 si__pow10F64(i32 exponent) {
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

SIDEF
isize si_wprintfEx(siStream writer, siString fmt, siArray(siAny) args, bool flush,
		bool newLine) {
	SI_ASSERT_STR(fmt);
	SI_ASSERT_ARR_TYPE(args, siAny);
	SI_ASSERT(args.len <= 16);

	isize end = fmt.len;
	isize i = 0;

	siFmtInfo info = SI_TYPE_ZERO(siFmtInfo);
	info.writer = writer;
	info.args = args;

	while (i < end) {
		isize prevI = i;
		while (i < end && fmt.data[i] != '%') { i += 1; }

		if (i > prevI) {
			info.n += si_streamWriteStr(info.writer, si_substr(fmt, prevI, i));
		}

		if (i >= end) {
			break;
		}

		i += 1;
		if (i < end && fmt.data[i] == '%') {
			info.n += si_streamWriteStr(info.writer, si_substrLen(fmt, i, 2));
			i += 2;
			continue;
		}

		if (info.vaCount > args.len) {
			continue;
		}

		info.state &= siFmtInfoState_Color;
		loop: switch (fmt.data[i]) {
			case '+': info.state |= siFmtInfoState_Plus;  i += 1; goto loop;
			case '-': info.state |= siFmtInfoState_Minus; i += 1; goto loop;
			case '#': info.state |= siFmtInfoState_Hash;  i += 1; goto loop;
			case ' ': info.state |= siFmtInfoState_Space; i += 1; goto loop;
			case '.': info.state |= siFmtInfoState_Prec;  i += 1; goto loop;
			case '0': 
				  if (info.state & siFmtInfoState_Prec) { goto case_zero; }
					  info.state |= siFmtInfoState_Zero;  i += 1; goto loop;

			case '*': {
				siAny arg = si_vaNext(&info);

				u64 value;
				switch (arg.type) {
					case siTypeId_i8:  case siTypeId_u8:  value =  *(u8*)arg.ptr; break;
					case siTypeId_i16: case siTypeId_u16: value = *(u16*)arg.ptr; break;
					case siTypeId_i32: case siTypeId_u32: value = *(u32*)arg.ptr; break; 
					case siTypeId_i64: case siTypeId_u64: value = *(u64*)arg.ptr; break;
					default: SI_PANIC();
				}
				
				if (info.state & siFmtInfoState_Prec) { info.precision = (isize)value; }
				else { info.width = (isize)value; }

				i += 1;
				goto loop;
			}

			case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			case '8': case '9': case_zero: {
				isize countEnd;
				i64 count = si_stringToInt(si_substrFrom(fmt, i), 10, &countEnd);
				if (count > ISIZE_MAX) { count = ISIZE_MAX; }
				SI_ASSERT(countEnd != -1);

				if (info.state & siFmtInfoState_Prec) { info.precision = count; }
				else { info.width = count; }

				i += countEnd;
				goto loop;
			}

			case 'v': si_fmtValue(&info, si_vaNext(&info)); break;

			case 'x': si__modifierIntVa(&info, 16, false); break;
			case 'X': si__modifierIntVa(&info, 16, true); break;
			case 'i':
			case 'd': si__modifierIntVa(&info, 10, true); break;
			case 'z': si__modifierIntVa(&info, 12, true); break;
			case 'o': si__modifierIntVa(&info, 8, true); break;
			case 'b': si__modifierIntVa(&info, 2, true); break;

			case 'r':
			case 'c': {
				siAny arg = si_vaNext(&info);

				i32 rune;
				switch (arg.type) {
					case siTypeId_char: case siTypeId_i8:  case siTypeId_u8:  rune =  *(i8*)arg.ptr; break;
					case siTypeId_i16: case siTypeId_u16: rune = *(i16*)arg.ptr; break;
					case siTypeId_i32:                    rune = *(i32*)arg.ptr; break;
					default: SI_PANIC_FMT("Expected a rune, got type '%i'", arg.type);
				}

				si_fmtRune(&info, rune);
			} break;

			case 's': {
				siAny arg = si_vaNext(&info);

				switch (arg.type) {
					case siTypeId_string: si_fmtStr(&info, *(siString*)arg.ptr); break;
					case siTypeId_cstring: si_fmtCstring(&info, *(char**)arg.ptr); break;
					default: SI_PANIC_FMT("Expected a string, got type '%i'", arg.type);
				}
			} break;

			case 'q': {
				siAny arg = si_vaNext(&info);

				siString str;
				switch (arg.type) {
					case siTypeId_string: {
						siString in = *(siString*)arg.ptr;
						str = (in.data) ? in : SI_STR("(nil)");
					} break;

					case siTypeId_cstring: {
						char* in = *(char**)arg.ptr;
						str = (in) ? SI_CSTR(in) : SI_STR("(nil)");
					} break;

					default: SI_PANIC();
				}

				info.n += si_streamWriteByte(info.writer, '\"');
				si__fmtWrite(&info, str);
				info.n += si_streamWriteByte(info.writer, '\"');
			} break;


			case 'F': case 'f': {
				siAny arg = si_vaNext(&info);
				f64 num;
				switch (arg.type) {
					case siTypeId_f32: num =  (f64)(*(f32*)arg.ptr); break;
					case siTypeId_f64: num = *(f64*)arg.ptr; break;
					default: SI_PANIC();
				}

				si_fmtFloat(&info, num);
			} break;

			/* TODO(EimaMei): Hex float support. */

			case 'E': case 'e': {
				siAny arg = si_vaNext(&info);

				f64 num;
				switch (arg.type) {
					case siTypeId_f32: num =  (f64)(*(f32*)arg.ptr); break;
					case siTypeId_f64: num = *(f64*)arg.ptr; break;
					default: SI_PANIC();
				}

				si_fmtSci(&info, num, (fmt.data[i] == 'E'));
			} break;

			case 'G': case 'g': {
				siAny arg = si_vaNext(&info);

				f64 num;
				switch (arg.type) {
					case siTypeId_f32: num =  (f64)*(f32*)arg.ptr; break;
					case siTypeId_f64: num = *(f64*)arg.ptr; break;
					default: SI_PANIC();
				}

				if (si_float64IsNan(num) || si_float64IsInf(num)) {
					si_fmtFloat(&info, num);
					break;
				}

				if (num < 0.0001) {
					si_fmtSci(&info, num, (fmt.data[i] == 'G'));
					break;
				}

				u64 pow10Val = si__pow10(info.precision);
				u64 intFloat = (u64)num;

				if (num > (f64)pow10Val) {
					info.precision -= (isize)(intFloat / pow10Val);
					si_fmtSci(&info, num, (fmt.data[i] == 'G'));
					break;
				}

				f64 remainder = num - (f64)intFloat;
				f64 maxZeroValue = si__pow10F64(-(i32)info.precision);

				i32 newAfterPoint = 0;
				while (newAfterPoint < info.precision) {
					remainder *= 10;
					if (si_between(f64, si_abs(f64, remainder), 0, maxZeroValue)) {
						break;
					}
					f64 digit = remainder + 0.5;
					remainder -= digit;
					newAfterPoint += 1;
				}

				si_fmtFloat(&info, num);
			} break;

			case 'p': si_fmtPtr(&info, si_vaNextType(&info, void*)); break;
			case 't': si_fmtBool(&info, si_vaNextType(&info, bool)); break;
			case 'L': si_fmtCallerLoc(&info, si_vaNextType(&info, siCallerLoc)); break;

			case 'C': {
				if (info.state & siFmtInfoState_Color) {
					info.state &= ~siFmtInfoState_Color;
					info.n += si_streamWriteStr(info.writer, SI_STR("\33[0m"));
					break;
				}

				siPrintColor clr = si_vaNextType(&info, siPrintColor);
				if (!SI__COLOR_MODE) { break; }

				info.state |= siFmtInfoState_Color;
				switch (clr.type) {
					case siPrintColorType_3bit: {
						SI_ASSERT(si_between(u8, clr.ansi.color, 0, 7));

						char str[] ="\33[\0;\0\0m";
						str[2] = (!clr.ansi.bold) ? '0' : '1';
						str[4] = (!clr.ansi.light) ? '3' : '9';
						str[5] = (char)('0' + clr.ansi.color);

						info.n += si_streamWriteStr(info.writer, SI_STR(str));
					} break;

					case siPrintColorType_8bit: {
						#define _8BIT_STR "\33[38;5;"

						char str[32] = _8BIT_STR;
						siArray(char) buf = SI_ARR_LEN(str, countof_str(str));
						isize trueLen = countof_str(_8BIT_STR);

						siString num = si_stringFromInt(
							si_sliceFrom(buf, countof_str(_8BIT_STR)), clr.cube
						);
						trueLen += num.len;

						str[trueLen] = 'm';
						trueLen += 1;

						info.n += si_streamWriteStr(info.writer, SI_STR(str));

						#undef _8BIT_STR
					} break;

					case siPrintColorType_24bit: {
						#define _24BIT_STR "\33[38;2;"

						char str[64] = _24BIT_STR;
						siArray(char) buf = SI_ARR_LEN(str, countof_str(str));
						isize trueLen = countof_str(_24BIT_STR);

						static char divider[countof(clr.rgb)] = {';', ';', 'm'};
						for_range (j, 0, countof(clr.rgb)) {
							siString num = si_stringFromInt(
								si_sliceFrom(buf, trueLen), clr.rgb[j]
							);
							trueLen += num.len;

							str[trueLen] = divider[j];
							trueLen += 1;

						}

						info.n += si_streamWriteStr(info.writer, SI_STR(str));

						#undef _24BIT_STR
					} break;

					default: SI_PANIC_MSG("Invalid color type. Most likely a non-color value was given.");
				}
			} break;
		}

		i += 1;
	}

	if (info.vaCount < args.len) {
		if (info.n != 0) { 
			info.n += si_streamWriteByte(info.writer, ' '); 
		}
		info.state = 0;

		for_range (j, info.vaCount, args.len - 1) {
			si_fmtValue(&info, si_vaNext(&info));
			info.n += si_streamWriteStr(info.writer, SI_STR(", "));
		}
		si_fmtValue(&info, si_vaNext(&info));
	}

	if (newLine) {
		info.n += si_streamWriteByte(writer, '\n');
	}

	if (flush) {
		si_streamFlush(writer);
	}

	return info.n;
}


inline
isize si_wprintfLnEx(siStream writer, siString fmt, siArray(siAny) args, bool flush) {
	return si_wprintfEx(writer, fmt, args, flush, true);
}

SIDEF
isize si_wprintEx(siStream writer, siArray(siAny) args, bool flush, siString seperator) {
	SI_ASSERT_ARR_TYPE(args, siAny);
	SI_ASSERT(args.len <= 16);
	SI_ASSERT_STR(seperator);

	siFmtInfo info = SI_TYPE_ZERO(siFmtInfo);
	info.writer = writer;

	siAny arg;
	for_eachArrEx (arg, i, args) {
		if (i != 0) {
			info.n += si_streamWriteStr(info.writer, seperator);
		}
		si_fmtValue(&info, arg);
	}

	if (flush) {
		si_streamFlush(info.writer);
	}

	return info.n;
}

SIDEF
isize si_wprintLnEx(siStream writer, siArray(siAny) args, bool flush, siString seperator) {
	isize n = si_wprintEx(writer, args, false, seperator);
	n += si_streamWriteByte(writer, '\n');

	if (flush) {
		si_streamFlush(writer);
	}

	return n;
}


inline
siPrintColor (si_printColor3bit)(siPrintColor3bit color, bool bold, bool light) {
	siPrintColor res;
	res.type = siPrintColorType_3bit;
	res.ansi = SI_COMP_LIT(siPrintColor3bitStruct, color, bold, light);
	return res;
}

inline
siPrintColor si_printColor8bit(u8 color) {
	siPrintColor res;
	res.type = siPrintColorType_8bit;
	res.cube = color;
	return res;
}

SIDEF
siPrintColor si_printColor24bit(u8 r, u8 g, u8 b) {
	siPrintColor res;
	res.type = siPrintColorType_24bit;
	res.rgb[0] = r;
	res.rgb[1] = g;
	res.rgb[2] = b;
	return res;
}


inline
void si_printEnableColor(bool value) {
	SI__COLOR_MODE = value & 1;
}

#endif /* SI_IMPLEMENTATION_PRINT */

#ifdef SI_IMPLEMENTATION_MATH

SI_MATH_FUNC_DECLARE_2X       (min,     inline, { return (a < b) ? a : b;  })
SI_MATH_FUNC_DECLARE_2X       (max,     inline, { return (a > b) ? a : b;  })
SI_MATH_FUNC_DECLARE_3X_bool   (between, inline, { return b <= a && a <= c; })

#undef SI_MATH_FUNC_DECLARE_2X
#undef SI_MATH_FUNC_DECLARE_3X_bool

inline i8 si__abs_i8(i8 a)    { return a < 0 ?  (i8)-a : (i8)a; }
inline i16 si__abs_i16(i16 a) { return a < 0 ? (i16)-a : (i16)a; }
inline i32 si__abs_i32(i32 a) { return a < 0 ? -a : a; }
inline i64 si__abs_i64(i64 a) { return a < 0 ? -a : a; }
inline isize si__abs_isize(isize a) { return a < 0 ? -a : a; }
inline f32 si__abs_f32(f32 a) { return a < 0 ? -a : a; }
inline f64 si__abs_f64(f64 a) { return a < 0 ? -a : a; }

inline f64 si__round_f64(f64 x) { return (x >= 0.0) ? si_floor(f64, x + 0.5) : si_ceil(f64, x - 0.5); }
inline f32 si__round_f32(f32 x) { return (x >= 0.0f) ? si_floor(f32, x + 0.5f) : si_ceil(f32, x - 0.5f); }

inline f32 si__floor_f32(f32 a) { return (f32)((a >= 0.0f) ? (i64)a : (i64)(a - 0.9999999999999999f)); }
inline f64 si__floor_f64(f64 a) { return (f64)((a >= 0.0) ? (i64)a : (i64)(a - 0.9999999999999999)); }

inline f32 si__ceil_f32(f32 a) { return (f32)((a < 0) ? (i64)a : (i64)a + 1); }
inline f64 si__ceil_f64(f64 a) { return (f64)((a < 0) ? (i64)a : (i64)a + 1); }

inline
i32 si_float32IsInf(f32 num) {
	switch (transmute(u32, num)) {
		case 0x7F800000: return 1;
		case 0xFF800000: return 2;
	}

	return 0;
}

inline
bool si_float32IsNan(f32 num) {
	static const u32 nanV = 0x7FC00000; /* NOTE(EimaMei): Canonical NaN value for 32-bit. */

	u32 n = transmute(u32, num);
	n &= ~SI_BIT(31); /* NOTE(EimaMei): A quick way of changing the minus to plus. */

	return (n == nanV);
}

inline
i32 si_float64IsInf(f64 num) {
	switch (transmute(u64, num)) {
		case 0x7FF0000000000000: return 1; /* NOTE(EimaMei): Positive infinity. */
		case 0xFFF0000000000000: return 2; /* NOTE(EimaMei): Negative infinity. */
	}

	return 0;
}

inline
bool si_float64IsNan(f64 num) {
	static const u64 nanV = 0x7FF8000000000000; /* NOTE(EimaMei): Canonical NaN value for 64-bit. */

	u64 n = transmute(u64, num);
	n &= ~SI_BIT(63); /* NOTE(EimaMei): A quick way of changing the minus to plus. */

	return (n == nanV);
}

#endif /* SI_IMPLEMENTATION_MATH */

#ifdef SI_IMPLEMENTATION_BENCHMARK

SIDEF
void si_benchmarkLoopsAvgPrint(siString name, i64 array[20], isize len, usize range[2]) {
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction - '%s'\n"
		"Runs:\n",
		name
	);

	f64 freq = (f64)si_cpuClockSpeed() / (f64)SI_NANOSECOND;
	isize padRuns = si_numLenUint(range[1]);

	usize arrayI = 0, runs;
	for (runs = range[0]; runs <= range[1]; runs *= 10) {
		i64 cycles = array[arrayI];
		f64 time = (f64)cycles / freq;

		siTimeUnitScale unit = si_timeGetUnit((i64)time);
		time /= (f64)unit.threshold;
		si_printf(
			"\t%*zu %s - %C%9.4f%C %2s (%lu cycles)\n",
			padRuns, runs, (runs != 1) ? SI_STR("runs") : SI_STR("run "),
			si_printColor3bit(siPrintColor3bit_Green), time, si_timeUnitStr(unit.unit), cycles
		);

		arrayI += 1;
	}

	i64 cyclesTotal = 0;
	for_range (i, 0, len) {
		cyclesTotal += array[i];
	}

	f64 cyclesMedian = (f64)cyclesTotal / (f64)len;
	f64 time = cyclesMedian / freq;

	siTimeUnitScale unit = si_timeGetUnit((i64)time);
	time /= (f64)unit.threshold;

	isize padCycles = si_numLenUint((u64)cyclesMedian);
	si_printf(
		"Final result:\n"
			"\tTime average   - %C%*.4f%C %2s\n"
			"\tCycles average - %*lu cycles\n",
		si_printColor3bit(siPrintColor3bit_Green),
		padCycles, time, si_timeUnitStr(unit.unit), padCycles, (u64)cyclesMedian
	);
}

SIDEF
void si_benchmarkLoopsAvgCmpPrint(siString names[2], i64 arrays[2][20], isize len,
		usize range[2]) {
	si_printf(
		"====== BENCHMARK DATA ======\n"
		"General:\n"
			"\tFunction 1 - '%s'\n"
			"\tFunction 2 - '%s'\n"
		"Runs:\n",
		names[0], names[1]
	);
	siTimeUnitScale units[2];
	f64 freq = (f64)si_cpuClockSpeed() / (f64)SI_NANOSECOND;

	isize padRuns = si_numLenUint(range[1]);
	isize padCycles[2] = {
		si_numLenUint((u64)arrays[0][len - 1]), si_numLenUint((u64)arrays[1][len - 1])
	};

	usize arrayI = 0, runs;
	for (runs = range[0]; runs <= range[1]; runs *= 10) {
		i64 cycles[2] = {arrays[0][arrayI], arrays[1][arrayI]};
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

		for_range (j, 0, countof(units)) {
			units[j] = si_timeGetUnit((i64)time[j]);
			time[j] /= (f64)units[j].threshold;
		}

		si_printf(
			"\t%*zu %s - %C%9.4f%C %2s vs %C%9.4f%C %2s (%4.4f ratio, %*li vs %*li cycles)\n",
			padRuns, runs, (runs != 1) ? SI_STR("runs") : SI_STR("run "),
			clr[0], time[0], si_timeUnitStr(units[0].unit), clr[1], time[1], si_timeUnitStr(units[1].unit),
			ratio, padCycles[0], cycles[0], padCycles[1], cycles[1]
		);

		arrayI += 1;
	}

	i64 cyclesTotal[2] = {0, 0};
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
		padCycles[i] = si_numLenUint((u64)cyclesMedian[i]);
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

	for_range (j, 0, countof(units)) {
		units[j] = si_timeGetUnit((i64)time[j]);
		time[j] /= (f64)units[j].threshold;
	}

	si_printfLn(
		"Final result:\n"
			"\tTime average   - %C%*.4f%C %s vs %C%*.4f%C %s (%4.4f ratio)\n"
			"\tCycles average - %*lu cycles vs %*lu cycles",
		clr[0], padCycles[0], time[0], si_timeUnitStr(units[0].unit),
		clr[1], padCycles[1], time[1], si_timeUnitStr(units[1].unit),
		ratio,
		padCycles[0], (u64)cyclesMedian[0], padCycles[1], (u64)cyclesMedian[1]
	);
}

void* si__benchmarkThread(void* arg) {
	si_sleep(*(siTime*)arg);
	return nil;
}


#endif /* SI_IMPLEMENTATION_BENCHMARK */

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
	siPrintColor red = si_printColor3bit(siPrintColor3bit_Red, true);

#ifndef SI_NO_ERROR_STRUCT
	si_fprintf(
		si_stderr, SI_STR("%CSystem error at \"%L\"%C: %s: %s\n"),
		red, error->location, si_systemErrorName(error->code), si_systemErrorDesc(error->code)
	);

#else
	si_fprintf(
		si_stderr, SI_STR("%CSystem error%C: %s: %s\n"),
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

		SI_STRC("siErrorSystem_Invalid"),
		SI_STRC("siErrorSystem_InvalidFilename"),
		SI_STRC("siErrorSystem_Exists"),
		SI_STRC("siErrorSystem_NotExists"),
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

		SI_STRC("Provided value is invalid."),
		SI_STRC("Invalid filename."),
		SI_STRC("File or directory already exists."),
		SI_STRC("File or directory doesn't exist."),
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
		case ERROR_ACCESS_DENIED: return siErrorSystem_Permission;

		case ERROR_NO_MORE_FILES:
		case ERROR_SUCCESS:
			return siErrorSystem_None;

		case ERROR_ALREADY_EXISTS:
		case ERROR_FILE_EXISTS:
			return siErrorSystem_Exists;

		case ERROR_INVALID_NAME:
		case ERROR_BAD_PATHNAME:
			return siErrorSystem_InvalidFilename;

		case ERROR_BAD_FORMAT:
		case ERROR_INVALID_DATA:
		case ERROR_DIRECTORY:
		case ERROR_INVALID_ADDRESS:
			return siErrorSystem_Invalid;

		case ERROR_PATH_NOT_FOUND:
		case ERROR_FILE_NOT_FOUND:
			return siErrorSystem_NotExists;

		case ERROR_OUTOFMEMORY:
		case ERROR_NOT_ENOUGH_MEMORY:
			return siErrorSystem_NoMemory;
	}

	return siErrorSystem_Generic;

#elif SI_SYSTEM_IS_WASM
	switch (si__wasmGetLastError()) {
		case __WASI_ERRNO_SUCCESS: return siErrorSystem_None;
		case __WASI_ERRNO_EXIST: return siErrorSystem_Exists;
		case __WASI_ERRNO_NOENT: return siErrorSystem_NotExists;
		case __WASI_ERRNO_NOBUFS: return siErrorSystem_NoMemory;
		case __WASI_ERRNO_DEADLK: return siErrorSystem_Deadlock;

		case __WASI_ERRNO_INVAL:
		case __WASI_ERRNO_BADF:
			return siErrorSystem_Invalid;

		case __WASI_ERRNO_ISDIR:
		case __WASI_ERRNO_NAMETOOLONG:
			return siErrorSystem_InvalidFilename;

		case __WASI_ERRNO_PERM:
		case __WASI_ERRNO_ACCES:
			return siErrorSystem_Permission;

		case __WASI_ERRNO_AGAIN:
		case __WASI_ERRNO_BUSY:
			return siErrorSystem_Unavailable;
	}

	return siErrorSystem_Generic;

#else
	switch (errno) {
		case 0:       return siErrorSystem_None;
		case EEXIST:  return siErrorSystem_Exists;
		case EINVAL:  return siErrorSystem_Invalid;
		case ENOMEM:  return siErrorSystem_NoMemory;
		case EAGAIN:  return siErrorSystem_Unavailable;
		case EDEADLK: return siErrorSystem_Deadlock;
		case ENOENT:  return siErrorSystem_NotExists;

		case EISDIR:
		case ENAMETOOLONG:
			return siErrorSystem_InvalidFilename;

		case EPERM:
		case EACCES:
			return siErrorSystem_Permission;
	}

	return siErrorSystem_Generic;

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
bool si_envVarSet(siString name, siString value) {
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
bool si_envVarUnset(siString name) {
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
	u32 len = GetEnvironmentVariableW(path, &path[pathLen], (u32)(countof(path) - pathLen));
	SI_STOPIF(len == 0, return 0);

	return si_utf16ToUtf8StrLen(SI_ARR_LEN(&path[pathLen], len));
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	return si_cstrLen(getenv(path));
	SI_UNUSED(pathLen);
#else
	return -1;
	SI_UNUSED(pathLen);
#endif
}

SIDEF
siString si_envVarGetData(siString name, siArray(u8) out) {
	SI_ASSERT(out.typeSize == sizeof(u8));

	siOsChar path[SI_PATH_MAX * 2];
	isize pathLen = si_pathToOS(name, path, countof(path));

#if SI_SYSTEM_IS_WINDOWS
	u32 len = GetEnvironmentVariableW(path, &path[pathLen], (u32)(countof(path) - pathLen));
	SI_STOPIF(len == 0, return SI_STR_NIL);

	return si_utf16ToUtf8Str(SI_ARR_LEN(&path[pathLen], len), out);
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

SIDEF
siWindowsVersion si_windowsGetVersion(void) {
#if SI_SYSTEM_IS_WINDOWS
	OSVERSIONINFOEXW info = SI_STRUCT_ZERO;
	{
		siDllHandle ntdll = si_dllLoad(SI_STR("ntdll.dll"));

		typedef LONG(WINAPI* siDllProcType(RtlGetVersion))(POSVERSIONINFOEXW);
		siDllProcType(RtlGetVersion) _RtlGetVersion = si_dllProcAddressFunc(ntdll, RtlGetVersion);

		info.dwOSVersionInfoSize = si_sizeof(OSVERSIONINFOEXW);
		_RtlGetVersion(&info);
		si_dllUnload(ntdll);
	}

	/* Windows 10 and 11. */
	if (info.dwMajorVersion == 10 && info.dwMinorVersion == 0) {
		return (info.dwBuildNumber >= 22000) ? siWindowsVersion_11 : siWindowsVersion_10;
	}
	/* Windows 8.1, 8, 7 and Vista. */
	else if (info.dwMajorVersion == 6) {
		return siWindowsVersion_Vista + (i32)info.dwMinorVersion;
	}

	return (info.dwMajorVersion == 5 && (info.dwMinorVersion == 1 || info.dwMinorVersion == 2))
		? siWindowsVersion_XP
		: 0;

#else
	return -1;
#endif
}

SIDEF
bool si_unixIsWayland(void) {
#if SI_SYSTEM_IS_UNIX
	static b8 isWayland = UINT8_MAX;
	SI_STOPIF(isWayland != UINT8_MAX, return (bool)isWayland);

	siString res = si_envVarGetData(SI_STR("WAYLAND_DISPLAY"), SI_ARR_STACK(1));
	isWayland = (res.data != nil);

	return isWayland;
#else
	return false;
#endif
}

inline
bool si_unixIsX11(void) {
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

	siString res = si_envVarGetData(SI_STR("XDG_CURRENT_DESKTOP"), SI_ARR_STACK(16));
	SI_STOPIF(res.data == nil, return 0);

	siArena arena = si_arenaMakePtr(si_stackAlloc(SI_KILO(1)));
	siMap(i32) map = si_mapMake(
		si_allocatorArena(&arena), i32,
		{SI_STR("KDE"), siUnixDE_KDE}, {SI_STR("GNOME"), siUnixDE_GNOME},
		{SI_STR("XFCE"), siUnixDE_Xfce}, {SI_STR("LXQt"), siUnixDE_LXQt},
		{SI_STR("LXDE"), siUnixDE_LXDE}, {SI_STR("MATE"), siUnixDE_MATE},
		{SI_STR("Cinnamon"), siUnixDE_Cinnamon}
	);

	i32* result = (i32*)si_mapGet(map, res);
	de = result ? *result : siUnixDE_Custom;

	return de;
#else
	return -1;
#endif

}

#endif /* SI_IMPLEMENTATION_SYSTEM */

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
siError si_vmDiscard(siVirtualMemory vm) {
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

#ifdef SI_IMPLEMENTATION_IO

#if SI_COMPILER_MSVC
	#pragma comment(lib, "shell32")
	#pragma comment(lib, "Advapi32")
#endif

#if SI_SYSTEM_IS_WINDOWS
	const u8 SI_PATH_SEPARATOR = '\\';
#else
	const u8 SI_PATH_SEPARATOR = '/';
#endif

inline
bool si_pathExists(siString path) {
	siOsChar stack[SI_PATH_MAX];
	si_pathToOS(path, stack, countof(stack));
	return si_pathExistsOS(stack);
}
inline
bool si_pathExistsOS(siOsString path) {
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
	int res = fstat(src, &stat);
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

	siDirectoryIterator entry;
	while (si_directoryIterateEx(&dir, false, &entry)) {
		siString path = si_pathBaseName(entry.path);
		si_memcopyStr(dstBuffer, path);

		if (entry.type == siIoType_Directory) {
			siError err = si_pathCreateFolder(dstStr);
			SI_STOPIF(err.code != 0, return SI_OPT_ERR(i32, err));

			siResult(i32) res = si_pathItemsCopy(entry.path, dstStr);
			SI_STOPIF(!res.hasValue, return res);
			itemsCopied += res.value;
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
			siDirectoryIterator entry;
			siDirectory dir = si_directoryOpen(path);
			while (si_directoryIterate(&dir, &entry)) {
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
			siDirectoryIterator entry;
			siDirectory dir = si_directoryOpen(path);
			while (si_directoryIterate(&dir, &entry)) {
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
	SI_ASSERT_STR(path);
	SI_ASSERT(path.len <= SI_PATH_MAX);

	isize i;
	for (i = path.len - 1; i >= 0; i -= 1) {
		if (path.data[i] == SI_PATH_SEPARATOR) {
			return si_substrFrom(path, i + 1);
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

	for_range (i, offset, path.len) {
		if (path.data[i] == SI_PATH_SEPARATOR) {
			return si_substrFrom(path, i);
		}
	}
	return si_substrFrom(path, offset);
}

SIDEF
siString si_pathExtension(siString path) {
	SI_ASSERT_STR(path);
	SI_ASSERT(path.len <= SI_PATH_MAX);

	isize i = si_stringFindLastByte(path, '.');
	return (i != -1) ? si_substrFrom(path, i) : SI_STR_EMPTY;

}

SIDEF
siString si_pathLongExtension(siString path) {
	SI_ASSERT_STR(path);
	SI_ASSERT(path.len <= SI_PATH_MAX);

	isize i = si_stringFindLastByte(path, SI_PATH_SEPARATOR);
	if (i != -1) {
		path = si_substrFrom(path, i + 1);
	}

	i = si_stringFindByte(path, '.');
	return (i != -1) ? si_substrFrom(path, i) : SI_STR_EMPTY;
}


SIDEF
siString si_pathStem(siString path) {
	SI_ASSERT_STR(path);
	SI_ASSERT(path.len <= SI_PATH_MAX);

	isize i = si_stringFindLastByte(path, SI_PATH_SEPARATOR);
	if (i != -1) {
		path = si_substrFrom(path, i + 1);
	}

	i = si_stringFindLastByte(path, '.');
	return (i > 0) ? si_substrTo(path, i) : SI_STR_EMPTY;
}

SIDEF
siString si_pathShortStem(siString path) {
	SI_ASSERT_STR(path);
	SI_ASSERT(path.len <= SI_PATH_MAX);

	isize i = si_stringFindLastByte(path, SI_PATH_SEPARATOR);
	if (i != -1) {
		path = si_substrFrom(path, i + 1);
	}

	i = si_stringFindByte(path, '.');
	return (i > 0) ? si_substrTo(path, i) : SI_STR_EMPTY;
}



SIDEF
siResult(siString) si_pathGetFullName(siString path, siAllocator alloc) {
	siOsChar stack[SI_PATH_MAX * 2];
	isize pathLen = si_pathToOS(path, stack, countof(stack));

#if SI_SYSTEM_IS_WINDOWS
	SI_OPTION_SYS_CHECK(!si_pathExistsOS(stack), siString);
	DWORD dif = (u32)(countof(stack) - pathLen);

	isize len = (isize)GetFullPathNameW(stack, dif, &stack[pathLen], nil);
	SI_OPTION_SYS_CHECK(len == 0, siString);

	siUtf16String str = SI_ARR_LEN(&stack[pathLen], len);
	isize utf8Len = si_utf16ToUtf8StrLen(str);

	siString res = si_utf16ToUtf8Str(str, si_arrayMakeReserve(u8, utf8Len, alloc));
	return SI_OPT(siString, res);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	siOsChar* out = &stack[pathLen];
	out = realpath(stack, out);
	SI_OPTION_SYS_CHECK(out == nil, siString);

	return SI_OPT(siString, si_stringFromCStr(out, alloc));
#else
	return SI_OPT_NIL(siString);
	SI_UNUSED(pathLen); SI_UNUSED(alloc);

#endif
}

inline
siString si_pathJoin(siString path, siString subPath, siAllocator alloc) {
	return si_pathJoinEx(SI_ARR(siString, path, subPath), alloc);
}

inline
siString si_pathJoinEx(siArray(siString) strs, siAllocator alloc) {
	return si_stringJoin(strs, SI_STR_LEN(&SI_PATH_SEPARATOR, 1), alloc);
}

inline
bool si_pathIsAbsolute(siString path) {
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
bool si_pathIsRelative(siString path) {
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

		si_stringToOsStrEx(path, SI_ARR_LEN(&out[4], capacity - 4), &copied);
		return copied + 4;
	}
#endif

	si_stringToOsStrEx(path, SI_ARR_LEN(out, capacity), &copied);
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
siTime si_pathLastWriteTime(siString path) {
	siOsChar stack[SI_PATH_MAX];
	si_pathToOS(path, stack, countof(stack));

#if SI_SYSTEM_IS_WINDOWS
	WIN32_FILE_ATTRIBUTE_DATA data;

	i32 res = GetFileAttributesExW(stack, GetFileExInfoStandard, &data);
	if (!res) { return 0; }

	FILETIME time = data.ftLastWriteTime;
	return si__win32ToSili((i64)time.dwHighDateTime << 32 | (i64)time.dwLowDateTime);

#elif SI_SYSTEM_IS_UNIX
	struct stat fs;
	int res = stat(stack, &fs);
	return (res == 0) ? SI_TIME_SEC(fs.st_mtim.tv_sec) + fs.st_mtim.tv_nsec : 0;

#elif SI_SYSTEM_IS_BSD
	struct stat fs;
	int res = stat(stack, &fs);
	return (res == 0) ? SI_TIME_SEC(fs.st_mtimespec.tv_sec) + fs.st_mtimespec.tv_nsec : 0;


#else
	return 0;

#endif
}

SIDEF
siString si_pathGetTmp(void) {
#if SI_SYSTEM_IS_WINDOWS
	static u8 buffer[256];
	siOsChar stack[256];

	u32 len = GetTempPathW(countof(stack), stack);
	SI_ASSERT(len != 0); /* NOTE(EimaMei): This musn't fail. */

	return si_utf16ToUtf8Str(SI_ARR_LEN(stack, len), SI_ARR_LEN(buffer, countof(buffer)));

#else
	return SI_STR("/tmp");

#endif
}


inline
siArray(u8) si_pathReadContents(siString path, siAllocator alloc, siErrorSystem* outError) {
	siFile file = si_fileOpen(path);
	if (file == -1) { return SI_ARR_EMPTY(u8); }

	siArray(u8) res = si_fileReadContents(file, alloc, outError);
	si_fileClose(file);

	return res;
}

inline
siString si_pathReadContentsStr(siString path, siAllocator alloc, siErrorSystem* outError) {
	siArray(u8) res = si_pathReadContents(path, alloc, outError);
	return SI_STR_LEN(res.data, res.len);
}


inline
siArray(siString) si_pathReadlines(siString path, siAllocator alloc, siErrorSystem* outError) {
	siString str = si_pathReadContentsStr(path, alloc, outError);
	siArray(siString) res = si_stringSplitLines(str, alloc);

	return res;
}



SIDEF
siFile si_fileGetStdFile(siStdFile type) {
	SI_ASSERT_MSG(si_between(i32, type, siStdFile_Input, siStdFile_Error), "Invalid standard file type.");

	static bool SI_STD_FILE_SET = false;
	static siFile SI_STD_FILE_ARR[siStdFile_Count];
	if (SI_STD_FILE_SET) {
		return SI_STD_FILE_ARR[type];
	}

	#if SI_SYSTEM_IS_WINDOWS
		SI_STD_FILE_ARR[0] = (isize)GetStdHandle(STD_INPUT_HANDLE));
		SI_STD_FILE_ARR[1] = (isize)GetStdHandle(STD_OUTPUT_HANDLE));
		SI_STD_FILE_ARR[2] = (isize)GetStdHandle(STD_ERROR_HANDLE));

		/* TODO(EimaMei): All fallback code that writes UTF-16 instead of UTF-8. */
		if (IsValidCodePage(CP_UTF8)) {
			SetConsoleOutputCP(CP_UTF8);
		}

		/* TODO(EimaMei): All code that disables colormode if this isn't an option. */
		#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
			#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
		#endif

		for_range (i, 1, 2) {
			DWORD mode;
			GetConsoleMode((HANDLE)SI_STD_FILE_ARR[i].handle, &mode);
			mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode((HANDLE)SI_STD_FILE_ARR[i].handle, mode);
		}
	#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE || SI_SYSTEM_EMSCRIPTEN
		for_range (i, 0, 3) { SI_STD_FILE_ARR[i] = i; }

	#else
		SI_STD_FILE_ARR[0] = (isize)stdin;
		SI_STD_FILE_ARR[1] = (isize)stdout;
		SI_STD_FILE_ARR[2] = (isize)stderr;
	#endif

	SI_STD_FILE_SET = true;
	return SI_STD_FILE_ARR[type];
}


inline
siFile si_fileCreate(siString path, siError* outError) {
	return si_fileOpenMode(path, siFileMode_Write | siFileMode_Plus, outError);
}

inline
siFile si_fileOpen(siString path, siError* outError) {
	return si_fileOpenMode(path, siFileMode_Read | siFileMode_Plus, outError);
}
SIDEF
siFile si_fileOpenMode(siString path, siFileMode mode, siError* outError) {
	SI_ASSERT(path.len <= SI_PATH_MAX);
	SI_ASSERT((mode & ~(u32)siFileMode_All) == 0);

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
		LARGE_INTEGER offset = SI_STRUCT_ZERO;

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
	}

	isize handle;
	{
		siOsChar stack[SI_PATH_MAX];
		si_pathToOS(path, stack, countof(stack));

		handle = open(stack, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	}
	SI_ERROR_SYS_CHECK(handle == -1, if (outError != nil) { *outError = SI_ERROR_RES; });

#else
	cstring mode_str;
	switch (mode) {
	case siFileMode_Read:                     mode_str = "rb";  break;
	case siFileMode_Write:                    mode_str = "wb";   break;
	case siFileMode_Append:                   mode_str = "ab";   break;
	case siFileMode_Read | siFileMode_Plus:   mode_str = "rb+"; break;
	case siFileMode_Write | siFileMode_Plus:  mode_str = "wb+";  break;
	case siFileMode_Append | siFileMode_Plus: mode_str = "ab+";  break;
	default: SI_PANIC_MSG("Invalid file mode.");
	}

	isize handle;
	{
		siOsChar stack[SI_PATH_MAX];
		siArena arena = si_arenaMakePtr(stack, si_sizeof(stack));

		handle = (isize)fopen(si_stringToCStr(path, si_allocatorArena(&arena)), mode_str);
	}
	SI_ERROR_SYS_CHECK(handle == 0, handle = -1; if (outError != nil) { *outError = SI_ERROR_RES; });


#endif

	return handle;
}

SIDEF
isize si_fileSize(siFile file) {
	SI_ASSERT_NOT_NEG(file);

#if SI_SYSTEM_IS_WINDOWS
	ULARGE_INTEGER res = SI_STRUCT_ZERO;

	BY_HANDLE_FILE_INFORMATION data;
	if (GetFileInformationByHandle((HANDLE)file.handle, &data)) {
		res.HighPart = data.nFileSizeHigh;
		res.LowPart = data.nFileSizeLow;
	}

	return (ISIZE_MAX < res.QuadPart) ? ISIZE_MAX : (isize)res.QuadPart;

#else
	isize prevOffset = si_fileTell(file);
	isize len = si_fileSeek(file, 0, siSeekWhere_End);
	si_fileSeek(file, prevOffset, siSeekWhere_Begin);

	return len;

#endif
}

inline
isize si_fileRead(siFile file, siArray(u8) out, siErrorSystem* outError) {
	return si_fileReadAt(file, si_fileTell(file), out, outError);
}

SIDEF
isize si_fileReadAt(siFile file, isize offset, siArray(u8) out, siErrorSystem* outError) {
	SI_ASSERT_NOT_NEG(file);
	SI_ASSERT_NOT_NEG(offset);
	SI_ASSERT_ARR_TYPE(out, u8);

	isize bytesRead;

#if SI_SYSTEM_IS_WINDOWS
	si_fileSeek(file, offset, siSeekWhere_Begin);

	DWORD read;
	i32 res = ReadFile(
		(HANDLE)file.handle, out, (len > UINT32_MAX) ? UINT32_MAX : (u32)len,
		&read, nil
	);
	SI_OPTION_SYS_CHECK(res == 0, siArray(u8));

	bytesRead = read;

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	bytesRead = pread((int)file, out.data, (usize)out.len, offset);
	if (bytesRead == -1 && outError) { *outError = si_systemGetError(); }

#else
	si_fileSeek(file, offset, siSeekWhere_Begin);

	bytesRead = (isize)fread(out.data, sizeof(u8), (usize)out.len, (FILE*)file);
	if (bytesRead < out.len && outError) { bytesRead = -1; *outError = si_systemGetError(); }

#endif

	return bytesRead;
}

SIDEF
siArray(u8) si_fileReadContents(siFile file, siAllocator alloc, siErrorSystem* outError) {
	isize len = si_fileSize(file);
	u8* data = si_allocArrayNonZeroed(alloc, u8, len);
	if (data == nil) { return SI_ARR_EMPTY(u8); }

	isize oldOffset = si_fileTell(file);
	len = si_fileReadAt(file, 0, SI_ARR_LEN(data, len), outError);
	si_fileSeek(file, oldOffset, siSeekWhere_Begin);

	return (len != -1) ? SI_ARR_LEN(data, len) : SI_ARR_EMPTY(u8);
}

SIDEF
siString si_fileReadContentsStr(siFile file, siAllocator alloc, siErrorSystem* outError) {
	siArray(u8) res = si_fileReadContents(file, alloc, outError);
	return SI_STR_LEN(res.data, res.len);
}

SIDEF
siArray(siString) si_fileReadlines(siFile file, siAllocator alloc, siErrorSystem* outError) {
	siArray(u8) str = si_fileReadContents(file, alloc, outError);
	siArray(siString) res = si_stringSplitLines(SI_STR_LEN(str.data, str.len), alloc);

	return res;
}

inline
isize si_fileWrite(siFile file, siArray(u8) in, siErrorSystem* outError) {
	SI_ASSERT_NOT_NEG(file);
	SI_ASSERT_ARR_TYPE(in, u8);

#if SI_SYSTEM_IS_WINDOWS
	si_fileSeek(*file, offset, siSeekWhere_Begin);

	/* TODO(EimaMei): Fix this. */
	DWORD count;
	i32 res = WriteFile(
		(HANDLE)file->handle, content.data, (content.len > UINT32_MAX) ? UINT32_MAX : (u32)content.len,
		&count, nil
	);
	SI_ERROR_SYS_CHECK(res == 0, file->error = SI_ERROR_RES; return -1);
	bytesWritten = count;

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	isize n = write((int)file, in.data, (usize)in.len);
	if (n == -1 && outError) { *outError = si_systemGetError(); }

	return n;

#elif SI_SYSTEM_IS_WASM
	struct __wasi_ciovec_t iov;
	iov.buf = content.data;
	iov.buf_len = (__wasi_size_t)content.len;

	__wasi_size_t count;
	__wasi_errno_t err = __wasi_fd_write((__wasi_fd_t)file->handle, &iov, 1, &count);
	si__wasmSetLastError(err);
	SI_ERROR_SYS_CHECK(err != 0, file->error = SI_ERROR_RES; return -1);

	n = (isize)count;

#else
	isize count = (isize)fwrite(in.data, sizeof(u8), (usize)in.len, (FILE*)file);
	if (count < in.len && outError) { *outError = si_systemGetError(); }

	return count;
#endif
}

SIDEF
isize si_fileWriteAt(siFile file, isize offset, siArray(u8) in, siErrorSystem* outError) {
	SI_ASSERT_NOT_NEG(file);
	SI_ASSERT_NOT_NEG(offset);
	SI_ASSERT_ARR_TYPE(in, u8);

#if SI_SYSTEM_IS_WINDOWS
	si_fileSeek(*file, offset, siSeekWhere_Begin);

	/* TODO(EimaMei): Fix this. */
	DWORD count;
	i32 res = WriteFile(
		(HANDLE)file->handle, content.data, (content.len > UINT32_MAX) ? UINT32_MAX : (u32)content.len,
		&count, nil
	);
	SI_ERROR_SYS_CHECK(res == 0, file->error = SI_ERROR_RES; return -1);
	bytesWritten = count;

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	isize curOffset = si_fileTell(file, outError);
	/* NOTE(EimaMei): Should we notify that a seek error occurred? */

	isize n = (curOffset == offset)
		? write((int)file, in.data, (usize)in.len)
		: pwrite((int)file, in.data, (usize)in.len, offset);
	if (n == -1 && outError) { *outError = si_systemGetError(); }

	return n;
#elif SI_SYSTEM_IS_WASM
	/* TODO(EimaMei): Add offset support. */
	struct __wasi_ciovec_t iov;
	iov.buf = content.data;
	iov.buf_len = (__wasi_size_t)content.len;

	__wasi_size_t count;
	__wasi_errno_t err = __wasi_fd_write((__wasi_fd_t)file->handle, &iov, 1, &count);
	si__wasmSetLastError(err);
	SI_ERROR_SYS_CHECK(err != 0, file->error = SI_ERROR_RES; return -1);

	n = (isize)count;

#else
	si_fileSeek(file, offset, siSeekWhere_Begin);
	return si_fileWrite(file, in, outError);


#endif
}

inline
isize si_fileWriteByte(siFile file, u8 byte, siErrorSystem* outError) {
	return si_fileWritePtr(file, &byte, 1, outError);
}


inline
isize si_fileWritePtr(siFile file, const void* ptr, isize len, siErrorSystem* outError) {
	return si_fileWrite(file, SI_ARR_LEN((u8*)ptr, len), outError);

}

inline
isize si_fileWriteStr(siFile file, siString str, siErrorSystem* outError) {
	return si_fileWritePtr(file, str.data, str.len, outError);
}



inline
isize si_fileTell(siFile file, siErrorSystem* outError) {
	return si_fileSeek(file, 0, siSeekWhere_Current, outError);
}

inline
isize si_fileSeek(siFile file, isize offset, siSeekWhere method, siErrorSystem* outError) {
	SI_ASSERT_NOT_NEG(file);

	isize n;
#if SI_SYSTEM_IS_WINDOWS
	LARGE_INTEGER res;
	res.QuadPart = offset;

	SetFilePointerEx((HANDLE)file.handle, res, &res, (u32)method);
	return (ISIZE_MAX < res.QuadPart)
		? ISIZE_MAX
		: (isize)res.QuadPart;

#elif SI_SYSTEM_IS_APPLE
	n = lseek((int)file.handle, offset, method);
	if (n == -1 && outError) { *outError = si_systemGetError(); }

#elif SI_SYSTEM_IS_UNIX
	n = lseek64((int)file, offset, method);
	if (n == -1 && outError) { *outError = si_systemGetError(); }

#else
	static const int POS_LUT[3] = {SEEK_SET, SEEK_CUR, SEEK_END};
	n = fseek((FILE*)file, offset, POS_LUT[method]);
	if (n == -1 && outError) { *outError = si_systemGetError(); }
	n = ftell((FILE*)file);
	if (n == -1 && outError) { *outError = si_systemGetError(); }

#endif

	return n;
}

inline
siErrorSystem si_fileSeekFront(siFile file) {
	siErrorSystem error = 0;
	si_fileSeek(file, 0, siSeekWhere_Begin, &error);
	return error;
}

inline
siErrorSystem si_fileSeekBack(siFile file) {
	siErrorSystem error = 0;
	si_fileSeek(file, 0, siSeekWhere_End, &error);
	return error;
}


SIDEF
siErrorSystem si_fileFlush(siFile file) {
	SI_ASSERT_NOT_NEG(file);

#if SI_SYSTEM_IS_WINDOWS
	FlushFileBuffers
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	int res = fsync((int)file);
	if (res == -1) { return si_systemGetError(); }

#else
	isize res = fflush((FILE*)file);
	if (res == -1) { return si_systemGetError(); }
#endif

	return 0;
}

SIDEF
bool si_fileTruncate(siFile file, isize size) {
	SI_ASSERT_NOT_NEG(file);
	SI_ASSERT_NOT_NEG(size);

#if SI_SYSTEM_IS_WINDOWS
	isize prevOffset = si_fileTell(*file);
	isize res = si_fileSeek(*file, size, siSeekWhere_Begin);
	SI_STOPIF(res == 0, return false);

	res = SetEndOfFile((HANDLE)file->handle);
	SI_ERROR_SYS_CHECK(res == 0, file->error = SI_ERROR_SYS_EX(siErrorSystem_TruncationFail); return false);

	si_fileSeek(*file, prevOffset, siSeekWhere_Begin);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	int res = ftruncate((int)file, size);
	if (res == -1) { return false; }
#else
	return false;

#endif

	return true;
}

SIDEF
siTime si_fileLastWriteTime(siFile file) {
	SI_ASSERT_NOT_NEG(file);

#if SI_SYSTEM_IS_WINDOWS
	BY_HANDLE_FILE_INFORMATION data;

	i32 res = GetFileInformationByHandle((HANDLE)file.handle, &data);
	if (!res) { return 0; }

	FILETIME time = data.ftLastWriteTime;
	return si__win32ToSili((i64)time.dwHighDateTime << 32 | (i64)time.dwLowDateTime);

#elif SI_SYSTEM_IS_UNIX
	struct stat fs;
	int res = fstat((int)file, &fs);
	return (res == 0) ? SI_TIME_SEC(fs.st_mtim.tv_sec) + fs.st_mtim.tv_nsec : 0;

#elif SI_SYSTEM_IS_BSD
	struct stat fs;
	int res = fstat((int)file.handle, &fs);
	return (res == 0) ? SI_TIME_SEC(fs.st_mtimespec.tv_sec) + fs.st_mtimespec.tv_nsec : 0;

#else
	return 0;
	SI_UNUSED(file);

#endif


}

inline
void si_fileClose(siFile file) {
	SI_ASSERT_NOT_NEG(file);

#if SI_SYSTEM_IS_WINDOWS
	CloseHandle((HANDLE)file->handle);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	close((int)file);
#else
	fclose((FILE*)file);
#endif
}


siIntern
SI_STREAM_PROC(si__streamFile_proc) {
	siFile file = (siFile)streamData;
	SI_ASSERT_NOT_NEG(file);

	switch (type) {
		case siStreamType_Read: {
			siErrorSystem error = 0;
			isize n = si_fileRead(file, ptr, &error);
			*outError = (error != 0) ? siStreamError_Unknown : siStreamError_None;

			return n;
		} break;

		case siStreamType_ReadAt: {
			siErrorSystem error = 0;
			isize n = si_fileReadAt(file, offset, ptr, &error);
			*outError = (error != 0) ? siStreamError_Unknown : siStreamError_None;

			return n;
		} break;

		case siStreamType_Write: {
			siErrorSystem error = 0;
			isize n = si_fileWrite(file, ptr, &error);
			*outError = (error != 0) ? siStreamError_Unknown : siStreamError_None;

			return n;
		} break;

		case siStreamType_WriteAt: {
			siErrorSystem error;
			isize n = si_fileWriteAt(file, offset, ptr, &error);
			*outError = (error != 0) ? siStreamError_Unknown : siStreamError_None;

			return n;
		} break;

		case siStreamType_Seek: {
			siErrorSystem error;
			isize n = si_fileSeek(file, offset, seek, &error);
			*outError = (error != 0) ? siStreamError_Unknown : siStreamError_None;

			return n;
		} break;

		case siStreamType_Size: {
			isize n = si_fileSize(file);
			*outError = (n != 0) ? siStreamError_Unknown : siStreamError_None;
			return n;
		} break;

		case siStreamType_Flush: {
			siErrorSystem error = si_fileFlush(file);
			*outError = (error != 0) ? siStreamError_Unknown : siStreamError_None;

			return 0;
		} break;

		case siStreamType_Close: {
			si_fileClose(file);
			*outError = 0;

			return 0;
		} break;

		case siStreamType_GetFeatures: {
			return SI_BITMASK_MAKE(siStreamType,
				Read, ReadAt, Write, WriteAt, Seek, Size, Flush, Close
			);
		} break;
	}

	*outError = siStreamError_Unimplemented;
	return 0;
}


SIDEF
siStream si_fileStream(siFile file) {
	siStream stream;
	stream.data = (void*)file;
	stream.proc = si__streamFile_proc;
	return stream;
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
	siOsChar stack[SI_PATH_MAX];
	siUtf16String pathWide = si_utf8ToUtf16Str(
		SI_STR_LEN(dir.buffer, dir.directoryLen),
		SI_ARR_LEN(stack, countof(stack) - 2)
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
bool si_directoryIterate(siDirectory* dir, siDirectoryIterator* out) {
	return si_directoryIterateEx(dir, true, out);
}

SIDEF
bool si_directoryIterateEx(siDirectory* dir, bool fullPath, siDirectoryIterator* out) {
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

	siString data = si_utf16ToUtf8Str(
		SI_ARR_LEN(file.cFileName, SI_PATH_MAX),
		SI_ARR_LEN(&dir->buffer[dir->directoryLen], si_sizeof(dir->buffer) - dir->directoryLen)
	);

	out->path = (fullPath)
		? SI_STR_LEN(dir->buffer, dir->directoryLen + data.len)
		: data;

	return true;


#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	struct dirent* dirEntry = readdir((DIR*)dir->handle);
	if (dirEntry == nil) {
		errno = 0;
		siErrorSystem code = si_systemGetError();
		si_directoryClose(dir);

		SI_ERROR_SYS_CHECK(code != 0, dir->error = SI_ERROR_RES; return false);
		return false;
	}

	static const u8 IO_types[15] = {
		0,
		siIoType_Fifo,
		siIoType_Device, 0,
		siIoType_Directory, 0,
		siIoType_Block, 0,
		siIoType_File, 0,
		siIoType_Link, 0,
		siIoType_Socket, 0, 0
	};
	out->type = IO_types[dirEntry->d_type];

	isize len = si_cstrLen(dirEntry->d_name);
	switch (len) {
		case 1:
			if (dirEntry->d_name[0] == '.') { return si_directoryIterateEx(dir, fullPath, out); }
			break;
		case 2:
			if (dirEntry->d_name[0] == '.' && dirEntry->d_name[1] == '.') { return si_directoryIterateEx(dir, fullPath, out); }
			break;
	}

	u8* data = &dir->buffer[dir->directoryLen];
	len = si_memcopy_s(
		SI_ARR_LEN(data, si_sizeof(dir->buffer) - dir->directoryLen),
		dirEntry->d_name, len
	);

	out->path = (fullPath)
		? SI_STR_LEN(dir->buffer, dir->directoryLen + len)
		: SI_STR_LEN(data, len);

	return true;
#else
	return false;

#endif
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

#ifdef SI_IMPLEMENTATION_PRINT_SYSTEM

inline
isize si_printEx(siArray(siAny) args, bool flush, siString seperator) {
	return si_fprintEx(si_stdout, args, flush, seperator);
}

inline
isize si_printLnEx(siArray(siAny) args, bool flush, siString seperator) {
	return si_fprintLnEx(si_stdout, args, flush, seperator);
}

inline
isize (si_printfEx)(siString fmt, siArray(siAny) args, bool flush, bool newLine) {
	return si_fprintfEx(si_stdout, fmt, args, flush, newLine);
}

inline
isize (si_printfLnEx)(siString fmt, siArray(siAny) args, bool flush) {
	return si_fprintfLnEx(si_stdout, fmt, args, flush);
}

inline
isize (si_fprint)(siFile file, siString str) {
	return si_fileWriteStr(file, str);
}

SIDEF
isize si_fprintEx(siFile file, siArray(siAny) args, bool flush, siString seperator) {
	u8 buf[1024];

	siIOWriter writer = si_ioWriterMakePtr(si_fileStream(file), buf, si_sizeof(buf));
	siStream stream = si_streamWriter(&writer);

	return si_wprintEx(stream, args, flush, seperator);
}

SIDEF
isize si_fprintLnEx(siFile file, siArray(siAny) args, bool flush, siString seperator) {
	u8 buf[1024];

	siIOWriter writer = si_ioWriterMakePtr(si_fileStream(file), buf, si_sizeof(buf));
	siStream stream = si_streamWriter(&writer);

	return si_wprintLnEx(stream, args, flush, seperator);
}

SIDEF
isize (si_fprintfEx)(siFile file, siString fmt, siArray(siAny) args, bool flush,
	  bool newLine) {
	u8 buf[1024];

	siIOWriter writer = si_ioWriterMakePtr(si_fileStream(file), buf, si_sizeof(buf));
	siStream stream = si_streamWriter(&writer);

	return si_wprintfEx(stream, fmt, args, flush, newLine);
}

inline
isize (si_fprintfLnEx)(siFile file, siString fmt, siArray(siAny) args, bool flush) {
	return si_fprintfEx(file, fmt, args, flush, true);
}


inline
isize si_eprintEx(siArray(siAny) args, bool flush, siString seperator) {
	return si_fprintEx(si_stderr, args, flush, seperator);
}

inline
isize si_eprintLnEx(siArray(siAny) args, bool flush, siString seperator) {
	return si_fprintEx(si_stderr, args, flush, seperator);
}

inline
isize (si_eprintfEx)(siString fmt, siArray(siAny) args, bool flush, bool newLine) {
	return si_fprintfEx(si_stderr, fmt, args, flush, newLine);
}

inline
isize (si_eprintfLnEx)(siString fmt, siArray(siAny) args, bool flush) {
	return si_fprintfLnEx(si_stderr, fmt, args, flush);
}


SIDEF
bool si_printHas24bitColor(void) {
	siString colorterm = si_envVarGetData(SI_STR("COLORTERM"), SI_ARR_STACK(16));
	return si_stringEqual(colorterm, SI_STR("truecolor"));
}

#ifndef SI_IMPL_DEBUG_TRAP
siNoreturn SIDEF
void SI_DEBUG_TRAP(void) {
	#ifdef SI_COMPILER_MSVC
	 	#if SI_COMPILER_VERSION < SI_VERSION(7, 0, 0)
			__asm int 3;
		#else
			__debugbreak();
		#endif
	#elif SI_COMPILER_GCC || SI_COMPILER_CLANG
		__builtin_trap();
	#else
		si_exit(1);
	#endif
}
#endif

siNoreturn SIDEF
void si_panic(siString condition, siString fmt, siArray(siAny) args, siCallerLoc call) {
	siPrintColor red = si_printColor3bit(siPrintColor3bit_Red, true);

	u8 buffer[SI_KILO(1)];
	siIOWriter iowriter = si_ioWriterMakePtr(si_fileStream(si_stderr), buffer, si_sizeof(buffer));
	siWriter stream = si_streamWriter(&iowriter);

	bool noMsg = (fmt.len == 0);
	si_wprintfEx(stream, SI_STR("%CAssertion \"%s\" at \"%L\"%C"), SI_ARGS(red, condition, call), noMsg, noMsg);

	if (!noMsg) {
		si_streamWriteStr(stream, SI_STR(": "));
		si_wprintfLnEx(stream, fmt, args);
	}

	SI_DEBUG_TRAP();
}

#endif /* SI_IMPLEMENTATION_PRINT_SYSTEM */

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
	si_sleep(SI_TIME_MIL(1));
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

	siThread thread = SI_STRUCT_ZERO;
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
		pthread_attr_t* attrPtr = nil;
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

#endif /* SI_IMPLEMENTATION_THREAD */

#ifdef SI_IMPLEMENTATION_CPU

inline
void si_CPUID(u32 ID, u32 registers[4]) {
	SI_ASSERT_NOT_NIL(registers);

#if SI_ARCH_IS_X86
	#if SI_COMPILER_CHECK_MIN(MSVC, 8, 0, 0)
		__cpuid((i32*)registers, (i32)ID);

	#elif SI_COMPILER_GCC || SI_COMPILER_CLANG
		__asm__ volatile (
			"cpuid"
			: "=a"(registers[0]), "=b"(registers[1]), "=c"(registers[2]), "=d"(registers[3])
			: "a"(ID), "c"(0)
		);
	#endif
#endif

	SI_UNUSED(ID); SI_UNUSED(registers);
}


SIDEF
i64 si_cpuClockSpeed(void) {
	/* TODO(EimaMei): Add OS-Specific functions, add an option to specify processor. */
	static i64 SI_CPU_FREQ_HZ = -1;
	SI_STOPIF(SI_CPU_FREQ_HZ != -1, return SI_CPU_FREQ_HZ);

	/* NOTE(EimaMei): We can find a pretty accurate clock speed by waiting a
	 * whole second and storing the results, however waing a whole second is
	 * too long. We can somewhat mitigate this by only waiting 100 ms, then
	 * multiplying the final result by 10. */
	siTime begin = si_RDTSC();
	si_sleep(SI_TIME_MIL(100));
	siTime end = si_RDTSC();

	SI_ASSERT(begin != end);
	SI_CPU_FREQ_HZ = (i64)((end - begin)) * 10;

	return SI_CPU_FREQ_HZ;
}

SIDEF
i32 si_cpuProcessorCount(void) {
	static i32 procCount = -1;
	SI_STOPIF(procCount != -1, return procCount);

#if SI_SYSTEM_IS_UNIX
	procCount = (i32)sysconf(_SC_NPROCESSORS_ONLN);

#elif SI_SYSTEM_IS_WINDOWS
	i32 len = 0;
	i32 res = GetLogicalProcessorInformation(nil, (DWORD*)&len);
	SI_STOPIF(res != 0 || len <= 0, return 0);

	SYSTEM_LOGICAL_PROCESSOR_INFORMATION* processors = si_mallocArray(SYSTEM_LOGICAL_PROCESSOR_INFORMATION, len);
	res = GetLogicalProcessorInformation(&processors[0], (DWORD*)&len);
	SI_STOPIF(res == 0, len = 0);

	procCount = 0;
	for_range (i, 0, len) {
		SYSTEM_LOGICAL_PROCESSOR_INFORMATION processor = processors[i];
		if (processor.Relationship == RelationProcessorCore) {
			procCount += (i32)si_countOnes(u64, processor.ProcessorMask);
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
		procCount = (i32)sysconf(_SC_NPROCESSORS_ONLN);
	}

#elif SI_SYSTEM_IS_WASM
	procCount = 1;

#else
	procCount = 0;

#endif

	return procCount;
}

#endif /* SI_IMPLEMENTATION_CPU */

#ifdef SI_IMPLEMENTATION_DLL

#if SI_SYSTEM_IS_WINDOWS
	const siString SI_DLL_EXTENSION = SI_STRC("dll");
#elif SI_SYSTEM_IS_APPLE
	const siString SI_DLL_EXTENSION = SI_STRC("dylib");
#elif SI_SYSTEM_IS_UNIX
	const siString SI_DLL_EXTENSION = SI_STRC("so");
#else
	const siString SI_DLL_EXTENSION = SI_STRC("");
#endif


inline
siDllHandle si_dllLoad(siString path) {
	return si_dllLoadEx(path, false);
}

SIDEF
siDllHandle si_dllLoadEx(siString path, bool globalSymbols) {
#if SI_SYSTEM_IS_WINDOWS
	siOsChar src[SI_PATH_MAX];
	si_pathToOS(path, src, countof(src));

	return (siDllHandle)LoadLibraryW(src);
	SI_UNUSED(globalSymbols);
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	siOsChar src[SI_PATH_MAX];
	si_pathToOS(path, src, countof(src));

	int flags = RTLD_NOW;
	flags |= (globalSymbols) ? RTLD_GLOBAL : RTLD_LOCAL;

	return (siDllHandle)dlopen(src, flags);
#else
	return nil;
	SI_UNUSED(globalSymbols);
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

SIDEF
siDllProc si_dllProcAddress(siDllHandle dll, siString name) {
	SI_ASSERT_NOT_NIL(dll);
	SI_ASSERT(name.len <= SI_PATH_MAX);

#if SI_SYSTEM_IS_WINDOWS
	char src[SI_PATH_MAX];
	isize len = si_memcopyStr(src, name);
	src[len] = '\0';

	PROC proc = GetProcAddress((HMODULE)dll, src);
	return transmute(siDllProc, *proc, PROC);

#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	siArray(siOsChar) src = SI_ARR_STACK_EX(siOsChar, SI_PATH_MAX);

	return dlsym(dll, si_stringToOsStr(name, src));

#else
	return nil;

#endif
}

SIDEF
siString si_dllError(void) {
#if SI_SYSTEM_IS_WINDOWS
	/* TODO(EimaMei): Possibly use 'FormatMessage' instead. */
	i32 err = si_systemGetError();
	return (err) ? si_systemErrorName(err) : SI_STR_EMPTY;
#elif SI_SYSTEM_IS_UNIX || SI_SYSTEM_IS_APPLE
	char* error = dlerror();
	return (error) ? SI_CSTR(error) : SI_STR_EMPTY;
#else
	return SI_STR_EMPTY;
#endif
}

#endif /* SI_IMPLEMENTATION_DLL */



#ifdef __cplusplus
}
#endif

#endif /* SI_INCLUDE_SI_H */

/*
------------------------------------------------------------------------------
Copyright (C) 2023-2025 EimaMei

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
