// TODO(EimaMei): Don't forget to remove this.
#define SI_IMPLEMENTATION 1
/*
 * MAJOR TODOS(EimaMei):
 * - Redo siAny.
 * - Rework the beginning text for more info.
 * - Check if all features 100% work on _all_ platforms.
 * - + Add documentation on how functions/macros are documented.
 * - + Update the old examples.
 * - Add new examples.
 * - Rework 'siPair'.
 * - Rework 'siFile' to not use the standard C version for more low-level control.
 * - Add macros to disable other features like align_ceil.
 * - Rename // commments to C89 style.
 * - Check all of the -len functions to see if they actually require strlen.
 * - Complete the other TODOs in this file plus others.
 * - Add replacements for printf and what not.
 * - Add TODOs from sili-ui.
 */

/* README TODOS(EimaMei):
 * - Rework the README.
 * - Add/remove some features from the list.
 * - Add the repo
 * - + Rename sili toolchain to just sili.
 * - + Update the license.
 * - Complete the previous TODOs and improvements.
 * - Update the TODO list.
 */

/*
sili.h - a cross-platform STL for modern C programming
===========================================================================
	- YOU MUST DEFINE 'SI_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SI_IMPLEMENTATION
		#include "sili.h"

	- All other files should just include the library without the #define macro.

    - If you want to disable certain features, you can do:
        - #define SI_NO_ALLOCATOR
		- #define SI_NO_PAIR
		- #define SI_NO_OPTIONAL
		- #define SI_NO_ARRAY
		- #define SI_NO_STRING
		- #define SI_NO_CHAR
        - #define SI_NO_HASHTABLE
		- #define SI_NO_FILE
		- #define SI_NO_THREAD
        - #define SI_NO_BIT
		- #define SI_NO_CPU
        - #define SI_NO_BENCHMARK
	before the SI_IMPLEMENTATION macro, as well as before any other include of
	`sili.h`.

	- NOTE: Disabling certain features may or may not create 'undefined'
	compiler errors at certain spots. Use it at your own risk.

===========================================================================
DOCUMENTATION
    - All functions, constant variables and macros contain a comment with a description
    of what they do above them, as well what they return (if anything). Macros
    specifically use a consistent format because of their lack of typing.
    That being:
        / argumentName - type | otherArgumentName - KEYWORD | ...VALUES - TYPE*
        description of the macro. /
        #define smth(argumentName, otherArgumentName, .../ VALUES/)

    - More often than not a macro's argument will not be a specific type and instead
    some kind of 'text'. Such arguments are noted if their type denotation is
    A FULLY CAPITALIZED KEYWORD. A general list of the keywords, what they mean
    and examples what to input:
        - TYPE - argument is just the type name (siString, usize, rawptr).
        - TYPE* - same as TYPE except it's a pointer (siString*, usize*, rawptr*).
        - INT - argument can be any integer (50, -250LL, ISIZE_MAX).
        - UINT - argument can be any UNSIGNED integer (50, 250LL, USIZE_MAX).
        - EXPRESSION - argument is just some kind of value (60, "hello", SI_RGB(255, 255, 255)).
        - NAME - argument has to be regular text with _no_ enquotes (test, var, len).
        - ANYTHING - argument can be literally anything.

===========================================================================
CREDITS
    - Ginger Bill's 'gb.h' (https://github.com/gingerBill/gb) - inspired me to
    make the Sili Toolchain, as well as certain features were taken from the
    library and implemented here.

LICENSE:
	- This software is licensed under the zlib license (see the LICENSE in the
    bottom of the file).

WARNING
	- This library is _slightly_ experimental and features may not work as
	expected.
	- This also means that many functions are not documented.

*/

#ifndef SI_INCLUDE_SI_H
#define SI_INCLUDE_SI_H

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
	#define SI_EXTERN extern "C"
#else
	#define SI_EXTERN extern
#endif

#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && !defined(_WIN32))
	#define SI_SYSTEM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
    #define SI_SYSTEM_APPLE 1
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR == 1
        #define SI_PLATFORM_IOS 1
    #elif TARGET_OS_IPHONE == 1
        #define SI_PLATFORM_IOS 1
    #elif TARGET_OS_MAC
        #define SI_SYSTEM_OSX 1
    #endif
#elif defined(__unix__)
	#define SI_SYSTEM_UNIX 1

    #if defined(__ANDROID__)
        #define SI_SYSTEM_ANDROID 1
    #elif defined(__EMSCRIPTEN__)
        #define SI_SYSTEM_WASM 1
	#elif defined(__linux__)
		#define SI_SYSTEM_LINUX 1
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#define SI_SYSTEM_FREEBSD 1
    #endif
#elif
#endif

#if defined(_MSC_VER)
	#define SI_COMPILER_MSVC 1
#elif defined(__clang__)
    #define SI_COMPILER_CLANG 1
#elif defined(__GNUC__)
	#define SI_COMPILER_GCC 1
#else
	#warning "Unknown compiler"
#endif

#if defined(__GNUC__)
    #define SI_GNUC_COMPLIANT 1
#endif


#if defined(__cplus__)
	#define SI_LANGUAGE_CPLUS 1
#elif defined(__OBJC__)
    #define SI_LANGUAGE_OBJ_C 1
    #if defined(__cplusplus)
        #define SI_LANGUAGE_CPP 1
        #define SI_LANGUAGE_OBJ_CPP 1
    #endif
#elif defined(__cplusplus)
	#define SI_LANGUAGE_CPP 1
#else
	#define SI_LANGUAGE_C 1
#endif


#if defined(__STDC__)
	#if !defined(SI_LANGUAGE_CPP)
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
        #define SI_STANDARD_CPP23 202311L
	#endif
#endif

#if defined(__STRICT_ANSI__)
	#define SI_STANDARD_ANSI 1
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__) || defined(__amd64__)
	#define SI_CPU_X86 1
	#define SI_CACHE_LINE_SIZE 64

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__) || defined(_ARCH_PPC)
	#define SI_CPU_PPC 1
	#define SI_CACHE_LINE_SIZE 128

#elif defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_ARM) || defined(_M_ARM) || defined(__arm) || defined(__aarch64__)
	#define SI_CPU_ARM 1
    #define SI_CPU_ARM64 __aarch64__
	#define SI_CACHE_LINE_SIZE 64

#elif defined(__mips__) || defined(__mips) || defined(__MIPSEL__) || defined(__mips_isa_rev)
	#define SI_CPU_MIPS 1
	#define SI_CACHE_LINE_SIZE 64

#elif defined(__sparc__) || defined(__sparc)
    #define SI_CPU_SPARC 1
    #define SI_CACHE_LINE_SIZE 64

#elif defined(__riscv)
    #define SI_CPU_RISC_V 1
    #define SI_CACHE_LINE_SIZE __riscv_xlen

#elif defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
    #define SI_CPU_ALPHA 1
    #define SI_CACHE_LINE_SIZE 64 /* NOTE(EimaMei): Sorta correct, sorta not really. Beware. */

#else
	#warning "Unknown CPU Type. Proceeding with caution."
    #define SI_CACHE_LINE_SIZE 0
#endif

#if 1
    #define SI_STATIC_ASSERT2(condition, msg)  typedef char static_assertion_##msg[(!!(condition)) * 2 - 1]
    #define SI_STATIC_ASSERT1(condition, line) SI_STATIC_ASSERT2(condition, line)
#endif
#define SI_STATIC_ASSERT(cond)        SI_STATIC_ASSERT1(cond, __LINE__)


#if defined(SI_SYSTEM_WINDOWS)
	#if !defined(_CRT_SECURE_NO_WARNINGS)
		#define _CRT_SECURE_NO_WARNINGS
	#endif
#endif

#if defined(SI_SYSTEM_UNIX)
	#if !defined(_GNU_SOURCE)
		#define _GNU_SOURCE
	#endif
    #if !defined(_LARGEFILE64_SOURCE)
        #define _LARGEFILE64_SOURCE
    #endif
    #if !defined(__USE_POSIX199309)
        #define __USE_POSIX199309
    #endif
    #if !defined(__USE_XOPEN_EXTENDED)
        #define __USE_XOPEN_EXTENDED
    #endif
#endif


#if !defined(SI_SYSTEM_WINDOWS)
    #include <time.h>
    #include <stdlib.h>
	#include <memory.h>
	#include <stdarg.h>
	#include <stddef.h>
	#include <errno.h>
	#include <pthread.h>
    #include <unistd.h>
    #include <dirent.h>

	#include <sys/stat.h>
	#include <sys/fcntl.h>

	#include <stdio.h>
    #include <stdint.h>
    #include <inttypes.h>
#endif
#if defined(SI_SYSTEM_WINDOWS)
	#if !defined(SI_NO_WINDOWS_H)
		#define NOMINMAX            1
		#define WIN32_LEAN_AND_MEAN 1
		#define WIN32_MEAN_AND_LEAN 1
		#define VC_EXTRALEAN        1
		#include <windows.h>
		#undef NOMINMAX
		#undef WIN32_LEAN_AND_MEAN
		#undef WIN32_MEAN_AND_LEAN
		#undef VC_EXTRALEAN
	#endif

	#include <malloc.h>
	#include <intrin.h>
	#include <stdio.h>
	#include <time.h>
#else

#endif

#if defined(SI_SYSTEM_OSX)
    #include <sys/socket.h>
#endif

#if defined(SI_SYSTEM_UNIX)
    #include <sys/sendfile.h>
#endif


#if UINTPTR_MAX == UINT64_MAX
    #define SI_ARCH_64_BIT 1
#else
    #define SI_ARCH_32_BIT 1
#endif

#define SI_HOST_IS_BIG_ENDIAN      (!*(u8*)&(u16){1})
#define SI_HOST_IS_LITTLE_ENDIAN   (!SI_HOST_IS_BIG_ENDIAN)


#if defined(SI_COMPILER_MSVC)
	#if _MSC_VER < 1300
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
    #define PRIu64       "I64u"

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
	typedef uint8_t   u8;
	typedef  int8_t   i8;
	typedef uint16_t u16;
	typedef  int16_t i16;
	typedef uint32_t u32;
	typedef  int32_t i32;
	typedef uint64_t u64;
	typedef  int64_t i64;
#endif

#if !defined(usize)
	typedef size_t      usize;
	typedef ptrdiff_t   isize;
#endif

#if !defined(uintptr)
	typedef usize  uintptr;
	typedef isize  intptr;
#endif

typedef u8 siByte;

#if defined(SI_ARCH_64_BIT)
	#define USIZE_MAX UINT64_MAX
	#define USIZE_MIN UINT64_MIN
	#define ISIZE_MAX INT64_MAX
	#define ISIZE_MIN INT64_MIN
#else
	#define USIZE_MAX UINT32_MAX
	#define USIZE_MIN UINT64_MIN
    #define ISIZE_MAX UINT32_MAX
    #define ISIZE_MIN UINT32_MIN
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


typedef float  f32;
typedef double f64;

SI_STATIC_ASSERT(sizeof(f32) == 4);
SI_STATIC_ASSERT(sizeof(f64) == 8);

#define SI_F32_MIN (1.17549435e-38f)
#define SI_F32_MAX (3.40282347e+38f)

#define SI_F64_MIN (2.2250738585072014e-308)
#define SI_F64_MAX (1.7976931348623157e+308)


#if !defined(b8)
    typedef u8   b8; /* NOTE(EimaMei): Don't use this as the primary boolean type. */
    typedef u16 b16;
    typedef u32 b32; /* NOTE(EimaMei): Use this as the default! */
    typedef u64 b64;
#endif

#if !defined(SI_LANGUAGE_CPP) && SI_STANDARD_VERSION <= SI_STANDARD_C17
    #ifndef true
        #define true 1
    #endif
    #ifndef false
        #define false 0
    #endif
#endif

SI_STATIC_ASSERT(sizeof(b8)  == 1);
SI_STATIC_ASSERT(sizeof(b16) == 2);
SI_STATIC_ASSERT(sizeof(b32) == 4);
SI_STATIC_ASSERT(sizeof(b64) == 8);

SI_STATIC_ASSERT(true == 1);
SI_STATIC_ASSERT(false == 0);


#if !defined(SI_LANGUAGE_CPP)
	#if defined(_MSC_VER) && _MSC_VER <= 1800
		#define inline __inline
	#elif !defined(__STDC_VERSION__)
		#define inline __inline__
	#else
		#define inline
	#endif
#endif

#ifdef intern
    #undef intern
#endif
#define intern static // TODO(EimaMei): Check the validity of this.

#if defined(SI_COMPILER_MSVC)
    /* asm - ASSEMBLY
     * Inserts inline assembly into the program using MSVC assembly syntax. */
    #define si_asm(... /* asm */) __asm  { __VA_ARGS__ }
#else
    #if defined(SI_CPU_X86) && defined(SI_GNUC_COMPLIANT)
        /* asm - cstring | ...IOR - ASM INPUT, OUTPUT OR REGISTERS
         * Inserts inline assembly into the program using GNU C assembly syntax. */
        #define si_asm(asm, .../* IOR */) \
            __asm__ __volatile__( \
                ".intel_syntax noprefix" SI_ASM_NL \
                asm SI_ASM_NL \
                ".att_syntax" \
                __VA_ARGS__ \
                )

        #define SI_ASM_INPUT(...) : __VA_ARGS__
        #define SI_ASM_OUTPUT(...) : __VA_ARGS__
        #define SI_ASM_REGISTERS(...) : __VA_ARGS__
        #define SI_ASM_NL "\n"
    #else
        /* asm - cstring | ...IOR - ASM INPUT, OUTPUT OR REGISTERS
         * Inserts inline assembly into the program using GNU C assembly syntax. */
        #define si_asm(asm, .../ * IOR */) __asm__ __volatile__(asm __VA_ARGS__)
    #endif
#endif



#if (defined(SI_LANGUAGE_CPP) && (SI_STANDARD_VERSION >= SI_STANDARD_CPP17)) || (!defined (SI_LANGUAGE_CPP) && SI_STANDARD_VERSION > SI_STANDARD_C17)
    /* Specifies a fallthrough for the compiler. */
    #define fallthrough [[fallthrough]]
#elif defined(SI_GNUC_COMPLIANT)
    /* Specifies a fallthrough for the compiler. */
    #define fallthrough __attribute__((fallthrough))
#else
    /* Specifies a fallthrough for the compiler. */
    #define fallthrough do {} while (0)
#endif


#if !(defined(SI_LANGUAGE_C) && SI_STANDARD_VERSION >= SI_STANDARD_C99)
    #if defined(SI_GNUC_COMPLIANT)  && ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
        #define restrict __restrict
    #elif defined(_MSC_VER) && _MSC_VER >= 1400
        #define restrict __restrict
    #else
        #define restrict
    #endif
#endif


#if !defined(rawptr)
    /* A pointer type pointing to raw data. */
	typedef void* rawptr;
#endif

#if !defined(cstring)
    /* A const C-string type. */
	typedef const char* cstring;
#endif

typedef isize siError;

/*
	========================
	| Constants            |
	========================
*/

/* Output is successful. */
#define SI_OKAY  (0)
/* Output is unsuccessful. */
#define SI_ERROR (-1)

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

#if !defined(nil)
    /* A nothing value, equivalent to NULL. */
	#define nil ((void*)0)
#endif

SI_STATIC_ASSERT(sizeof(nil) == sizeof(void*));

/*
	========================
	| Builtin functions    |
	========================
*/
#if !defined(__has_builtin)
    #define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_unreachable)
    #define SI_BUILTIN_UNREACHABLE() __builtin_unreachable()
#else
    #define SI_BUILTSI_BUILTIN_UNREACHABLE() SI_PANIC()
#endif


/*
	========================
	| Declaration macros   |
	========================
*/

/* ...KEYWORDS - C KEYWORDS
 * Syntactic sugar to differentiate the keywords from the return type. */
#define F_TRAITS(.../* KEYWORDS */) __VA_ARGS__

/* type - TYPE | name - NAME
 * Defines the enum with the given integer type. */
#define SI_ENUM(type, name) type name; enum
/* varName - NAME | multipleArgs - (TYPE1, TYPE2...)
 * Macro to define a function pointer. */
#define SI_FUNC_PTR(varName, multipleArgs) (*varName)multipleArgs
/* x - ANYTHING.
 * Silences the unused warnings for a given variable. */
#define SI_UNUSED(x) (void)(x)


/*
	========================
	| Macro related        |
	========================
*/

/* a - ANYTHING | b - ANYTHING
 * Combines 'a' and 'b' into 'ab' for macro shenanigans. */
#define SI_CAT(a, b) a##b


/*
	========================
	| Casting              |
	========================
*/

/* type - TYPE | value - EXPRESSION
 * Casts a value to the type provided. */
#define si_cast(type, value) ((type)((value)))
/* type - TYPE | value - EXPRESSION
 * Bit cast converts the value with the same size types. */
#define si_transmute(type, value) ((union { typeof(value) in; type out; }){value}.out)
/* text - cstring
 * Converts a cstring into an u8. */
#define SI_CSTR_U8(text)  (*si_cast(u8*, text))
/* text - cstring
 * Converts a cstring into an u16. */
#define SI_CSTR_U16(text) (*si_cast(u16*, text))
/* text - cstring
 * Converts a cstring into an u32. */
#define SI_CSTR_U32(text) (*si_cast(u32*, text))
/* text - cstring
 * Converts a cstring into an u64. */
#define SI_CSTR_U64(text) (*si_cast(u64*, text))
/* text - cstring | bytes - usize
 * Converts a cstring into an u64 containing 'bytes' amount of data from the string.
 * Maximum byte count is 8. */
#define SI_CSTR_UBYTE(text, bytes) si_u64FromPtr(text, bytes)


/*
	========================
	| Unary operators      |
	========================
*/
#if !defined(typeof) && !(defined(SI_LANGUAGE_C) && SI_STANDARD_VERSION > SI_STANDARD_C17)
    /* ...VALUE - TYPE/EXPRESSION
    * Gets the value's type and expresses it as just a regular type. */
    #define typeof(.../* VALUE */)  __typeof__(__VA_ARGS__)
#endif

#if !defined(countof)
    /* value - ARRAY
    * Gets the static length of the given value (must be an array). */
    #define countof(value) (sizeof(value) / sizeof((value)[0]))
#endif

#if defined(offsetof)
    #undef offsetof
#endif
#if !defined(offsetof)
    /* type - STRUCT TYPE | element - TYPE's member
    * Get's the offset of the provided member. */
    #define offsetof(type, element) ((isize)&(((type*)nil)->element))
#endif

#if (!defined(SI_LANGUAGE_CPP) && !defined(alignof)) || (defined(SI_LANGUAGE_CPP) && SI_STANDARD_VERSION < SI_STANDARD_CPP11)
    /* type - TYPE
    * Gets the alignment of a type. */
    #define alignof(type) offsetof(struct { char c; type member; }, member)
#endif


/*
	========================
	| General macros       |
	========================
*/

/* msg - char*
 * Printf but without the formatting. */
#define print(msg) puts(msg)
/* bytes - usize
 * Allocates the memory into the stack. */
#define si_salloc(bytes) si_cast(rawptr, (siByte[bytes]){0})
/* type - TYPE/EXPRESSION
 * Allocates 'sizeof(type)' bytes to the stack and casts the value. */
#define si_sallocItem(type) (type*)si_salloc(sizeof(type))
/* type - TYPE/EXPRESSION | count - usize
 * Allocates an array of 'sizeof(type)' bytes to the stack and casts the value. */
#define si_sallocArray(type, count) (type*)si_salloc(sizeof(type) * count)
/* allocator - siAllocator* | type - TYPE
 * Allocates 'sizeof(type)' bytes to the allocator and casts the value. */
#define si_mallocItem(allocator, type) (type*)si_malloc(allocator, sizeof(type))
/* allocator - siAllocator* | type - TYPE | count - usize
 * Allocates an array of 'sizeof(type)' bytes to the allocator and casts the value. */
#define si_mallocArray(allocator, type, count) (type*)si_malloc(allocator, sizeof(type) * (count))
/* allocator - siAllocator* | var - REGULAR POINTER
 * Allocates 'sizeof(variable)' bytes to the allocator and copies the value of
 * 'variable' to the allocated memory. Variable _must_ be a pointer. */
#define si_mallocCopy(allocator, pointer) \
    memcpy( \
        si_mallocItem(allocator, typeof(*(pointer))),  \
        variable, \
        sizeof(typeof(*(pointer))) \
    )
/* type - TYPE | ...VALUES - <EXPRESSION>, [EXPRESSION] ...
 * Declares a static buffer. */
#define si_buf(type, .../* VALUES */) (type[]){__VA_ARGS__}
/* a - VARIABLE | b - VARIABLE
 * Swaps the value of 'a' with 'b'; 'b' with 'a'. */
#define si_swap(a, b) do { typeof((a)) tmp = (a); (a) = (b); (b) = tmp; } while (0)
/* x - INT
 * Gets the absolute value of a number. */
#define si_abs(x) ((x) < 0 ? -(x) : (x))
/* x - EXPRESSION | lower - EXPRESSION | higher - EXPRESSION
 * A check to see if a value is between two other values. */
#define si_between(x, lower, upper) (((lower) <= (x)) && ((x) <= (upper)))
/* Pauses the thread and waits for the user to press enter in the terminal. */
#define si_pause() do { print("Press any key to continue..."); getchar(); } while(0)
/* x - EXPRESSION
 * Checks if value is null. */
#define si_isNull(x) ((x) == nil)
#if 1
    #define for_range_2(countVar, struct_name, start, end) \
        siPointS struct_name = {(isize)start, (isize)end}; \
        for (isize countVar = struct_name.x; countVar < struct_name.y; countVar += 1)
    #define for_range_1(countVar, line, start, end) \
        for_range_2(countVar, SI_CAT(__si_impl_range_, line), start, end)
#endif
/* countvar - NAME | start - INT | end - INT
 * A loop that loops from 'start' to 'end' and increments 'countVar' each cycle. */
#define for_range(countVar, start, end) \
    for_range_1(countVar, __LINE__, start, end)


/*
	========================
	| Endian swap macros   |
	========================
*/

/* x - u16
 * Swaps the endianess of a 16-bit number. */
#define si_swap16(x) \
    ((u16)((((x) >> 8) & 0xFF) | (((x) & 0xFF) << 8)))
/* x - u32
 * Swaps the endianess of a 32-bit number. */
#define si_swap32(x)	\
    ((((x) & 0xFF000000U) >> 24) | (((x) & 0x00FF0000U) >> 8) \
    | (((x) & 0x0000FF00U) << 8) | (((x) & 0x000000FFU) << 24))
/* x - u64
 * Swaps the endianess of a 64-bit number. */
#define si_swap64(x) \
   ((((x) &  (u64)0xFF00000000000000) >> 56) \
   | (((x) & (u64)0x00FF000000000000) >> 40) \
   | (((x) & (u64)0x0000FF0000000000) >> 24) \
   | (((x) & (u64)0x000000FF00000000) >> 8) \
   | (((x) & (u64)0x00000000FF000000) << 8)	\
   | (((x) & (u64)0x0000000000FF0000) << 24) \
   | (((x) & (u64)0x000000000000FF00) << 40) \
   | (((x) & (u64)0x00000000000000FF) << 56))
/* x - u16
 * Changes the endianess of the number to big-endian. Does nothing if the host is big-endian. */
#define si_swap16be(x) ((SI_HOST_IS_LITTLE_ENDIAN) ? si_swap16(x) : x)
/* x - u32
 * Changes the endianess of the number to big-endian. Does nothing if the host is big-endian. */
#define si_swap32be(x) ((SI_HOST_IS_LITTLE_ENDIAN) ? si_swap32(x) : x)
/* x - u64
 * Changes the endianess of the number to big-endian. Does nothing if the host is big-endian. */
#define si_swap64be(x) ((SI_HOST_IS_LITTLE_ENDIAN) ? si_swap64(x) : x)

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
	| Mics/General         |
	========================
*/

/*
	========================
	| SI_ASSERT            |
	========================
*/
usize si_intern_assert(b32 condition, cstring conditionStr, cstring file, i32 line,
        cstring func, cstring message, ...);

/* condition - EXPRESSION
 * Crashes the app if the condition is not met. */
#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
/* condition - EXPRESSION | message - cstring
 * Crashes the app with a message if the condition is not met. */
#define SI_ASSERT_MSG(condition, message) si_intern_assert(condition, #condition, __FILE__, __LINE__, __func__, message, "")
/* condition - EXPRESSION | message - cstring | ...FMT - VARIADIC ARGUMENTS
 * Crashes the app with a formatted message if the condition is not met. */
#define SI_ASSERT_FMT(condition, message, ...) si_intern_assert(condition, #condition, __FILE__, __LINE__, __func__, message, __VA_ARGS__)
/* ptr - rawptr
 * Crashes the app if a pointer is NULL. */
#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL.")
/* path - cstring
 * Crashes the app if the provided path doesn't exist. */
#define SI_ASSERT_PATH_EXISTS(path) SI_ASSERT_FMT(si_pathExists(path), "Path '%s' doesn't exist.", path)
/* Crashes the app immediately. */
#define SI_PANIC() si_intern_assert(false, "SI_PANIC()", __FILE__, __LINE__, __func__, nil)
/* message - cstring
 * Crashes the app immediately with a message. */
#define SI_PANIC_MSG(message) si_intern_assert(false, "SI_PANIC()", __FILE__, __LINE__, __func__, message, "")
/* condition - EXPRESSION | action - ANYTHING
 * Checks if the condition is true. If it is, execute 'action'. */
#define SI_STOPIF(condition, action) if (condition) { action; } do {} while(0)


/*
	========================
	| siAny                |
	========================
*/

/*
 * IMPORTANT NOTE(EimaMei):
 * 'si_anyMake' and 'si_anyMakeStatic' should be used in different cases.
 *
 * 'si_anyMake' _COPIES_ the data into the allocator, while 'si_anyMakeStatic'
 * only stores the pointer of the given value.
 *
 * If you're planning to return a 'siAny' and the value is from the stack, the usage
 * of 'si_anyMake' is mandatory as a pointer to that data outside of the stack
 * will just be invalid data.
 *
 * 'si_anyMakeStatic' can be used and is preferred to be used when the pointer
 * to the data ISN'T invalid. An example would be using 'si_anyMakeStatic' to pass
 * in a temporary 'siAny' to a function.
 *
 * When you can, use 'si_anyMakeStatic' when possible, as it only needs a pointer
 * to the data, while 'si_anyMake' has to save the data in an allocator.
*/

// Contains the pointer to the data and the size of it.
typedef struct {
    // Pointer to the data.
	rawptr ptr;
    // Size of the data
	usize typeSize;
} siAny;

/* allocator - siAllocator* | ...VALUE - EXPRESSION
 * Creates a 'siAny' object. Said object's pointer points to the copied value
 * in the allocator. */
#define si_anyMake(allocator, .../*VALUE */) \
    (siAny){ \
        si_mallocCopy( \
            allocator, \
            (rawptr)&((struct { typeof(__VA_ARGS__) in; }){__VA_ARGS__}.in) \
        ), \
        sizeof(typeof(__VA_ARGS__)) \
    }
/* ...VALUE - EXPRESSION
 * Creates a 'siAny' object. Said object's pointer is just "&(<VALUE>)". */
#define si_anyMakeStatic(.../* VALUE */) \
    (siAny){ \
        (rawptr)&((struct { typeof(__VA_ARGS__) in; }){__VA_ARGS__}.in), \
        sizeof(typeof(__VA_ARGS__)) \
    }
/* type - TYPE | any - siAny
 * Gets the value of the provided 'siAny' object. Crashes the app if the pointer
 * is NULL. */
#define si_anyGet(type, any) \
    ((any.ptr != nil) ? \
     *si_cast(type*, si_cast(siByte (*)[any.typeSize], any.ptr)) :  \
     *(type*)SI_ASSERT_NOT_NULL(any.ptr))


/*
	========================
	| siGeneric/siFatPtr   |
	========================
*/

/* A generic fat pointer header, which stores a length. Usually good enough for
 * most things. */
typedef struct {
    usize len;
} siGenericHeader;

/* x - generic's original pointer
 * A macro to access the generic header. */
#define SI_GENERIC_HEADER(x) ((siGenericHeader*)x - 1)
/* type - TYPE
 * Denotes that this is a generic fat pointer. */
#define siGenericFat(type) type*


/* headerType - STRUCT TYPE | ..CONTENT - pointer to data that'll be copied over
 * Creates a fat pointer from the given header type AND content in the stack.
 * IMPORTANT NOTE(EimaMei): You're responsible for filling out the header, not sili.
*/
#define si_fatPtrMake(headerType, .../*CONTENT*/)  \
    memcpy( \
        si_fatPtrMakeReserve(headerType, typeof(__VA_ARGS__), 1), \
        &(typeof(__VA_ARGS__)){__VA_ARGS__}, \
        sizeof(typeof(__VA_ARGS__)) \
    )

/* headerType - STRUCT HEADER | type - TYPE | count - usize
 * Reserves an array of fat pointers in the stack. */
#define si_fatPtrMakeReserve(headerType, type, count)  \
    ((siByte*)si_salloc((sizeof(type) * count) + sizeof(headerType)) + sizeof(headerType))


/*
	========================
	| siFunction           |
	========================
*/
// Contains a function pointer for siThread.
typedef struct {
   rawptr SI_FUNC_PTR(ptr, (rawptr));
} siFunction;

/* func - NAME
 * A hasty macro solution for inputting a function into a thread. */
#define siFunc(func) (siFunction){(rawptr (*)(rawptr))func}

/*
	========================
	| Other                |
	========================
*/
// An XY point structure. Both are 32-bit integers.
typedef struct { i32 x, y; } siPoint;
// An XY point structure. Both are isize integers.
typedef struct { isize x, y; } siPointS;
// A struct denoting the version.
typedef struct { i32 major, minor; } siVersion;

// An RGBA structure.
typedef struct { u8 r, g, b, a; } siColor;
/* r, g, b, a - u8
 * Macro to define an RGBA color. */
#define SI_RGBA(r, g, b, a) ((siColor){r, g, b, a})
/* r, g, b - u8
 * Macro to define an RGB color. */
#define SI_RGB(r, g, b) ((siColor){r, g, b, 255})

typedef SI_ENUM(u32, siDirection) {
	SI_DIRECTION_LEFT,
	SI_DIRECTION_RIGHT
};

// Moves memory by 'moveBy' amount of bytes depending on the direction.
void si_ptrMoveBy(rawptr src, usize srcLen, isize moveBy, siDirection direction);

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

#if !defined(SI_ALLOCATOR_UNDEFINE)
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
typedef struct {
	siByte* ptr;
	usize maxLen;
	usize offset;
} siAllocator;

/* Creates an allocator from the heap with N amounts of bytes. */
siAllocator* si_allocatorMake(usize bytes);
/* bytes - usize
 * Creates an allocator from the stack with N amounts of bytes. */
#define si_allocatorMakeStack(bytes) \
    si_intern_allocatorMakeStack(bytes, si_salloc(bytes + sizeof(siAllocator)))
//siAllocator* si_intern_allocatorMakeStack(usize bytes, rawptr res);

/* Resizes the amount of available bytes in the allocator. */
void si_allocatorResize(siAllocator* alloc, usize newSize);
/* Resets the allocator to start from the beginning. */
void si_allocatorReset(siAllocator* alloc);
/* Resets the allocator to the given offset. */
void si_allocatorResetFrom(siAllocator* alloc, usize offset);
/* Returns the amount of bytes that are still available in the allocator. */
usize si_allocatorAvailable(siAllocator* alloc);
/* Pushes a byte into the allocator. */
void si_allocatorPush(siAllocator* alloc, siByte byte);
/* Frees the allocator from memory. All allocations from the allocator are also
 * invalidated with this. */
void si_allocatorFree(siAllocator* alloc);

/* alloc - siAllocator* | bytes - usize
 * Allocates 'bytes' amount of memory from the allocator.
 * NOTE: Similar to C's malloc, 'bytes' may increase internally in the function
 * to be aligned to 16 bytes. */
#define si_malloc(alloc, bytes) si_mallocEx(alloc, bytes, __FILE__, __LINE__)
rawptr si_mallocEx(siAllocator* alloc, usize bytes, cstring filename, i32 line);
/* alloc - siAllocator* | num - usize | bytes - usize
 * Allocates 'bytes' multipled by 'num' amount of memory from the allocator.
 * NOTE: Similar to C's malloc, 'bytes' may increase internally in the function
 * to be aligned to 16 bytes. */
#define si_calloc(alloc, num, bytes) si_callocEx(alloc, bytes, __FILE__, __LINE__)
rawptr si_callocEx(siAllocator* alloc, usize num, usize bytes, cstring filename, i32 line);
/* alloc - siAllocator* | ptr - rawptr | oldSize - usize | newSize - usize
 * Equivalent to doing 'si_malloc' and then 'memcpy'.
 * NOTE: Similar to C's malloc, 'bytes' may increase internally in the function
 * to be aligned to 16 bytes. */
#define si_realloc(alloc, ptr, oldSize, newSize) si_reallocEx(alloc, ptr, oldSize, newSize, __FILE__, __LINE__)
rawptr si_reallocEx(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize,
        cstring filename, i32 line);

/* alloc - siAllocator* | ptr - rawptr
 * Syntactic sugar, but also to keep compatibility with non-allocator mode. */
#define si_free(alloc, ptr) si_freeEx(alloc, ptr, __FILE__, __LINE__)
void si_freeEx(siAllocator* alloc, rawptr ptr, cstring filename, i32 line);

#endif

#if !defined(SI_NO_PAIR)
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
	| siPair               |
	========================
*/
// TODO(EimaMEi): Redo this
/* type1 - TYPE | type2 - TYPE
 * A pair structure. */
#define siPair(type1, type2) struct { type1 first; type2 second; }
/* firstValue - EXPRESSION | secondValue - EXPRESSION
 * Creates a pair structure variable. */
#define si_pairMake(firstValue, secondValue) {firstValue, secondValue}
/* pair - siPair
 * Makes a copy of the given pair. */
#define si_pairCopy(pair) si_pairMake(pair.first, pair.second)

#endif

#if !defined(SI_NO_OPTIONAL) || !defined(SI_IMPLEMENTATION_OPTIONAL)
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

/* type - TYPE
 * Specifies the return of a 'siOptional(typpe)'. */
#define siOptionalRet(type) rawptr
/* type - TYPE
 * Denotes a 'siOptional' type. */
#define siOptional(type) struct { type value; b32 hasValue; }*
/* A fully empty 'siOptional' variable. */
#define SI_OPTIONAL_NULL(type) (&(struct { type value; b32 hasValue; }){(type){0}, false})

/* ...VALUE - EXPRESSION
 * Creates a 'siOptional' from the given value. */
#define si_optionalMake(.../* VALUE */) \
    si_intern_optionalMake( \
        si_sallocItem(struct { typeof(__VA_ARGS__) one; b32 two; }), \
        si_anyMakeStatic(__VA_ARGS__), \
        offsetof(struct { typeof(__VA_ARGS__) one; b32 two; }, two) \
    )
/* optionalVar -  siOptional(TYPE) | defaultValue - EXPRESSION
 * Gets the value of the provided 'siObject' object. HOWVER if 'hasValue' is set
 * to false, return 'defaultValue' instead. */
#define si_optionalGetOrDefault(optionalVar, defaultValue) \
    ((optionalVar)->hasValue ? (optionalVar)->value : (typeof((optionalVar)->value))(defaultValue))

/* optionalVar - siOptional(TYPE)
 * Resets the optional variable. */
#define si_optionalReset(optionalVar) \
    do { \
        memset(&(optionalVar)->value, 0, sizeof(typeof((optionalVar)->value))); \
        (optionalVar)->hasValue = false; \
    } while(0)

#endif

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
typedef struct {
    /* The allocator used for creating the array. Used for reallocations, frees, etc. */
    siAllocator* allocator;
    /* The current amount of items. */
	usize len;
    /* The maximum possible count of maximum items. */
	usize capacity;
    /* Size for each item. */
	u32 typeSize;
} siArrayHeader;

/* type - TYPE
 * Denotes a 'siArray' variable. */
#define siArray(type) type*
/* array - siArray(TYPE)
 * Gets the siArray's header. */
#define SI_ARRAY_HEADER(array) ((siArrayHeader*)array - 1)

#if !defined(for_each)
	#if defined(SI_LANGUAGE_C) && SI_STANDARD_VERSION > SI_STANDARD_C89
		/* variableName - NAME | array - siArray(TYPE)
         * Iterates through a siArray. */
        #define for_each(variableName, array) \
            for (\
                typeof(array) variableName = array; \
                variableName != (typeof(array))si_arrayGetPtr(array, si_arrayLen(array)); \
                variableName += 1 \
            )
	#else
		/* variableName - NAME | array - siArray(TYPE)
         * Iterates through a siArray. */
		#define for_each(variableName, array) \
            typeof(array) variableName; \
            for ( \
                variableName = array; \
                variableName != (typeof(array))si_arrayGetPtr(array, si_arrayLen(array)); \
                variableName += 1 \
            )
	#endif
#endif

/* array - siArray(TYPE) | index - usize
 * Gets a specific item's pointer and return it as a 'siByte*' for pointer arithmetic. */
#define si_arrayGetPtr(array, index) \
	(si_cast(siByte*, array) + si_arrayTypeSize(array) * (index))

/* allocator - siAllocator* | ...BUFFER - STATIC BUFFER
 * Creates a siArray from the provided static buffer. */
#define si_arrayMake(allocator, ...) \
    si_arrayMakeList(allocator, __VA_ARGS__, sizeof(typeof(*(__VA_ARGS__))), countof((__VA_ARGS__)))
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
b32 si_arrayEmpty(rawptr array);

/* Returns a pointer of the item. If the index is out of the bounds, the return is nil. */
rawptr si_arrayAt(rawptr array, usize index);
/* Returns a pointer of the front item. */
rawptr si_arrayFront(rawptr array);
/* Returns a pointer of the back item. */
rawptr si_arrayBack(rawptr array);

/* arrayPtr - siArray(TYPE)* | ...VALUE - EXPRESSION
 * Appends the given value to the array. If the array's capacity is reached, the
 * array is reallocated by (currentCapacity * 2). */
#define si_arrayAppend(arrayPtr, .../* VALUE */) si_intern_arrayAppend(arrayPtr, si_anyMakeStatic(__VA_ARGS__))
/* arrayPtr - siArray(TYPE)* | ...VALUE - EXPRESSION
 * Wrapper macro for si_arrayAppend. */
#define si_arrayPush(arrayPtr, .../* VALUE */) si_arrayAppend(arrayPtr, __VA_ARGS__)
/* Erases the last item in the array. */
void si_arrayPop(rawptr arrayPtr);
/* arrayPtr - siArray(TYPE)* | newItem - EXPRESSION | index - usize
 * Inserts 'newItem' at the provided index. */
#define si_arrayInsert(arrayPtr, newItem, index) si_intern_arrayInsert(arrayPtr, si_anyMakeStatic(newItem), index)
/* Erases a certain index. */
void si_arrayErase(rawptr arrayPtr, usize index);
/* Erases a certain index PLUS the amount specified afterwards get erased too. */
void si_arrayEraseCount(rawptr arrayPtr, usize index, usize count);
/* arrayPtr - siArray(TYPE)* | ...VALUE - EXPRESSION
 * Removes every item in the array that equals to the value. */
#define si_arrayRemoveItem(arrayPtr, .../* VALUE */) si_impl_arrayRemoveItem(arrayPtr, si_anyMakeStatic(__VA_ARGS__))
/* arrayPtr - siArray(TYPE)* | index - usize | count - usize | ...VALUE - EXPRESSION
 * 'array[index]...array[index + count]' get filled with 'VALUE'. */
#define si_arrayFill(arrayPtr, index, count, .../* VALUE */ ) si_impl_array_fill(arrayPtr, index, count, si_anyMakeStatic(__VA_ARGS__))

/* array - siArray(TYPE) | ...VALUE - EXPRESSION
 * Attempts to find the first mention of 'VALUE', starting from index 0. Returns
 * -1 if nothing was found. */
#define si_arrayFind(array, ...) si_impl_array_find(array, 0, si_arrayLen(array), si_anyMakeStatic(__VA_ARGS__))
/* array - siArray(TYPE) | start - usize | end - usize | ...VALUE - EXPRESSION
 * Attempts to find the first mention of 'VALUE', starting from 'start' to 'end'.
 * Returns -1 if nothing was found. */
#define si_arrayFindEx(array, start, end, ...) si_impl_array_find(array, start, end, si_anyMakeStatic(__VA_ARGS__))
/* array - siArray(TYPE) | ...VALUE - EXPRESSION
 * Attempts to find the first mention of 'VALUE', starting from the last index.
 * Returns -1 if nothing was found. */
#define si_arrayRFind(array, ...) si_impl_array_rfind(array, si_arrayLen(array) - 1, 0, si_anyMakeStatic(__VA_ARGS__))
/* array - siArray(TYPE) | start - usize | end - usize | ...VALUE - EXPRESSION
 * Attempts to find the last mention of 'VALUE', starting from 'start' to 'end'.
 * Returns -1 if nothing was found. */
#define si_arrayRFindEx(array, start, end, ...) si_impl_array_rfind(array, start, end, si_anyMakeStatic(__VA_ARGS__))
/* array - siArray(TYPE) | oldValue - EXPRESSION | ...newValue - EXPRESSION
 * Attempts to find the first mention of 'VALUE', starting from the last index.
 * Returns -1 if nothing was found. */
#define si_arrayReplace(arrayPtr, oldValue, .../* newValue */) si_impl_array_replace(array, si_anyMakeStatic(oldValue), si_anyMakeStatic(__VA_ARGS__))
/* Reverses the contents of the array. */
void si_arrayReverse(rawptr array);
/* Creates a 'siString' from a 'siArray'. */
char* si_arrayToSistring(siArray(char*) array, cstring separator);

/* Sums the contents of the array as an unsigned number. */
u64 si_arraySumU64(siArray(void) array);
/* Sums the contents of the array as a signed number. */
i64 si_arraySumI64(siArray(void) array);
/* Sums the contents of the array as a float number. */
f64 si_arraySumF64(siArray(void) array);

/* Clears the contents of the array to zero. */
void si_arrayClear(rawptr array);
/* Checks if two arrays equal. */
b32 si_arrayEqual(rawptr lha, rawptr rha);

/* Frees the array from memory. Should not really be used if an allocator system is used.*/
void si_arrayFree(rawptr array);
/* Shrinks the array to its bare minimums in memory. Should REALLY not be used
 * if an allocator system is used.*/
void si_arrayShrinkToFit(rawptr arrayPtr);

/* NOTE(EimaMei): The actual implementations. Shouldn't be used in practice, really. */
#if 1
isize si_impl_array_find(rawptr array, usize start, usize end, siAny value);
isize si_impl_array_rfind(rawptr array, usize start, usize end, siAny value);
rawptr si_intern_arrayAppend(rawptr arrayPtr, siAny value);
void si_intern_arrayInsert(rawptr arrayPtr, siAny value, usize index);
void si_impl_array_replace(rawptr arrayPtr, siAny oldValue, siAny newValue);
void si_impl_arrayRemoveItem(rawptr arrayPtr, siAny item);
void si_impl_array_fill(rawptr arrayPtr, usize index, usize count, siAny item);
#endif

#endif

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

/* siString is just a char* pointer in disguise. */
typedef char* siString;

/* The string header. */
typedef struct {
    /* Allocator used by the string. */
    siAllocator* allocator;
    /* Current length of the string. */
	usize len;
    /* Total memory capacity of the string. */
	usize capacity;
} siStringHeader;


/* Gets the header of the string. */
#define SI_STRING_HEADER(str) ((siStringHeader*)str - 1)
/* TODO(EimaMei): #define SI_STRING_DEFAULT_GROW 64 */


/* Creates a siString from a null-terminated C-string. */
siString si_stringMake(siAllocator* alloc, cstring str);
/* Creates a siString from a C-string with a specified length. */
siString si_stringMakeLen(siAllocator* alloc, cstring str, usize len);
/* Creates a siString from a C-string with a specified length and capacity. */
siString si_stringMakeEx(siAllocator* alloc, cstring str, usize len, usize capacity);
/* Creates a siString with a reserved capacity. */
siString si_stringMakeReverse(siAllocator* alloc, usize capacity);
/* Create a siString from a formatted null-terminated C-string. */
siString si_stringMakeFmt(siAllocator* alloc, cstring str, ...);

/*  Creates a copy of an existing siString. */
siString si_stringCopy(siAllocator* alloc, siString from);

/* Gets the length of the siString. */
usize si_stringLen(siString str);
/* Gets the capacity of a siString. */
usize si_stringCapacity(siString str);
/* Gets the type size of a siString. */
usize si_stringTypeSize(siString str);
/* Gets the allocator of the specific siString. */
siAllocator* si_stringAllocator(siString str);
/* Returns true if the string is nil or length is 0. */
b32 si_stringEmpty(siString str);

/* Gets the character at a specified index in the siString. If the index is out of
 * bounds, the return is '/0'. */
char si_stringAt(siString str, usize index);
/* Gets the first character of the siString. */
char si_stringFront(siString str);
/* Gets the last character of the siString. */
char si_stringBack(siString str);
/* Gets the substring of the siString. and writes it into the allocator. */
char* si_stringSub(siAllocator* alloc, siString str, usize begin, usize len);

/* Finds the first occurrence of a substring in the siString. Returns -1 if nothing
 * was found. */
isize si_stringFind(siString str, cstring cstr);
/* Finds the first occurrence of a substring within a specific range of a siString.
 * Returns -1 if nothing was found. */
isize si_stringFindEx(siString str, usize start, usize end, cstring cstr, usize cstrLen);
/* Finds the last occurrence of a substring in the siString. Returns -1 if nothing
 * was found. */
isize si_stringRFind(siString str, cstring cstr);

/* Finds the last occurrence of a substring within a specific range of the siString.
 * Returns -1 if nothing was found. */
isize si_stringRFindEx(siString str, usize start, usize end, cstring cstr);

/* Joins a siString with a null-terminated C-string using a separator. */
void si_stringJoin(siString* str, cstring cstr, cstring separator);
/* Sets the siString to a null-terminated C-string. */
void si_stringSet(siString* str, cstring cstr);
/* Replaces all occurences of first null-terminated C-string, `oldStr`,
 * in the siString to the second null-terminted C-string, `newStr`. */
void si_stringReplace(siString* str, cstring oldStr, cstring newStr);
/* Replaces all occurences of first C-string with specified length, `oldStr`,
 * in the siString to the second C-string with specified length, `newStr`. */
void si_stringReplaceEx(siString* str, cstring oldValue, usize oldLen, cstring newValue,
        usize newLen);
/* Cuts all occurences of the null-terminated C-string in the siString. */
void si_stringTrim(siString str, cstring cutSet);
/* Cuts all occurences of the C-string with specified length in the siString. */
void si_stringTrimLen(siString str, cstring cutSet, usize cutLen);
/* Enquotes siString (String 'test' becomes "test"). */
void si_stringEnquote(siString* str);

/* Appends a null-terminated C-string to the siString. */
void si_stringAppend(siString* str, cstring other);
/* Appends a C-string with specified length to the siString. */
void si_stringAppendLen(siString* str, cstring other, usize otherLen);
/* Pushes a character into the siString. */
void si_stringPush(siString* str, char other);
/* Pops the last character of the siString.  **/
void si_stringPop(siString str);

/* Inserts a null-terimnated C-string to the siString at the specified index. */
void si_stringInsert(siString* str, cstring cstr, usize index);
/* Inserts a C-string with specified length to the siString at the specified index. */
void si_stringInsertLen(siString* str, cstring cstr, usize cstrLen, usize index);
/* Inserts a C-string with specified length to the siString at the specified index,
 * as erases the index itself if 'eraseIndex' is set to true. */
void si_stringInsertEx(siString* str, cstring cstr, usize cstrLen, usize index, b32 eraseIndex);
/* Erases a substring from the specified index. */
void si_stringErase(siString str, usize index, usize eraseLen);
/* Removes every occurrence of the provided null-terminated C-string. */
void si_stringRemoveCstr(siString str, cstring cstr);
/* TODO(EimaMei): void si_string_swap(siString* str, cstring cstr1, cstring cstr2); */

/* Removes any leading or trailing spaces and newlines in the string. */
void si_stringStrip(siString str);
/* Reverses the entirety of the siString. */
void si_stringReverse(siString str);
/* Reverses a front section of the string. */
void si_stringReverseLen(siString str, usize len);

/* Returns a siArray of substrings that were split based on the delimiter. */
siArray(siString) si_stringSplit(siAllocator* alloc, siString str, cstring delimiter);
/* Clears the string by setting the first letter and length to 0. */
void si_stringClear(siString str);

/* Frees the string from memory. Does nothing when the allocator system is used. */
void si_stringFree(siString str);
/* Shrinks the string to the point where the capacity is equal to the length of
 * the string. Should _not_ be used with an allocator system, it'll consume
 * even more memory than reduce it. */
void si_stringShrinkToFit(siString* str);
/* Internal function. Allocates more space to the string. */
void si_stringMakeSpaceFor(siString* str, usize addLen);
#endif

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
char si_charLower(char c);
/* Returns the uppercased version of the given character. */
char si_charUpper(char c);

/* Returns true  if the given character is a space. */
b32 si_charIsSpace(char c);
/* Returns true if the given character is from '0' to '9'. */
b32 si_charIsDigit(char c);
/* Returns true if the given character is a hex digit (between '0'..'9'; 'a'...'f'; 'A'...'F'). */
b32 si_charIsHexDigit(char c);
/* Returns true if the given character is in the English alphabet. */
b32 si_charIsAlpha(char c);
/* Returns true if the given character is in the English alphabet OR is a number. */
b32 si_charIsAlphanumeric(char c);
/* Converts '0'...'9' to an actual integer ('3' -> 3). */
i32 si_charDigitToInt(char c);
/* Converts a hex digit into an actual integer ('F' -> 15). */
i32 si_charHexDigitToInt(char c);
#endif

#if !defined(SI_NO_STRING)
/*
	========================
	|  cstring             |
	========================
*/

#define SI_NUM_MAX_DIGITS 20

/* Writes a NULL-terminated C-string into the allocator. */
char* si_cstrMake(siAllocator* alloc, cstring cstr);
/* Writes a C-string with specified length into the allocator. */
char* si_cstrMakeLen(siAllocator* alloc, cstring cstr, usize len);
/* Writes a formatted NULL-terminated C-string into the allocator. */
char* si_cstrMakeFmt(siAllocator* alloc, cstring cstr, ...);

/* Returns the length of a NULL-terminated C-string. */
usize si_cstrLen(cstring str);

/* Sets every character in C-string to uppercased. */
void si_cstrUpper(char* str);
/* Sets every character in C-string to lowercased. */
void si_cstrLower(char* str);
/* Sets every word's first letter to uppercase, while the rest lowercase. */
void si_cstrTitle(char* str);
/* Uppercases the first NULL-terminated C-string's character, then the rest are lowercased. */
void si_cstrCapitalize(char* str);

/* Returns true if both NULL-terminated C-strings are equal in memory. */
b32 si_cstrEqual(cstring str1, cstring str2);
/* Returns true if both C-strings with specified lengths are equal in memory. */
b32 si_cstrEqualLen(cstring str1, usize str1Len, cstring str2, usize str2Len);

/* Returns an unsigned 64-bit integer form of the NULL-terminated C-string.
 * The string must be a number. */
u64 si_cstrToU64(cstring str);
/* Returns an unsigned 64-bit integer form of the C-string with specified length.
 * The string must be a number. */
u64 si_cstrToU64Len(cstring str, usize len);
/* Returns a signed 64-bit integer form of the NULL-terminated C-string.
 * The string must be a number. */
i64 si_cstrToI64(cstring str);
/* Returns an signed 64-bit integer form of the C-string with specified length.
 * The string must be a number. */
u64 si_cstrToI64Len(cstring str, usize len);
// TODO(EimaMei): f64 si_cstr_to_f64(cstring str);

// TODO(EimaMei): Fix this to use alloc
/* Creates a string from the given unsigned number and stores it in the allocator. */
char* si_u64ToCstr(siAllocator* alloc, u64 num);
/* Creates a string from the given signed number and stores it in the allocator. */
char* si_i64ToCstr(siAllocator* alloc, i64 num);
/* TODO(EimaMei): si_f64_to_cstr */
#endif

#if !defined(SI_NO_HASHTABLE)

/* type - TYPE
 * Denotes that this is a 'siHashTable' variable. */
#define siHt(type) siHashTable*

typedef struct {
    /* Key name for the value. */
    cstring key;
    /* Pointer to the value. */
    rawptr value;
} siHashEntry;

typedef struct {
    /* Allocator used to store the hash entries. */
    siAllocator* alloc;
    /* Array of the hash entries. */
    siArray(siHashEntry) entries;
} siHashTable;

/* Creates a hash table using the given names and data. */
siHashTable si_hashtableMake(siAllocator* alloc, cstring* keyArray, rawptr dataArray,
        usize sizeofElement, usize len);
/* Reserves a 'capacity' amount of items for the hash table. */
siHashTable si_hashtableMakeReserve(siAllocator* alloc, usize capacity);
/* Returns the key entry's value pointer from the hash table. If not found, nil
 * is returned. */
rawptr si_hashtableGet(siHashTable ht, cstring key);
/* ht - siHashTable* | key - cstring | type - TYPE
 * Returns the key entry's value as the specified type. If the item does NOT exist,
 * this will crash. */
#define si_hashtableGetItem(ht, key, type) (*(type*)si_hashtableGet(ht, key))
/* ht - siHashTable* | key - cstring | value - EXPRESSION
 * Adds a 'key' entry to the hash table. */
#define si_hashtableSet(ht, key, value) si_hashtableSetEx(ht, key, si_anyMakeStatic(value))
/* Adds a 'key' entry to the hash table, with the value being siAny. */
siHashEntry* si_hashtableSetEx(siHashTable* ht, cstring key, siAny value);

/* Frees the hash table. */
void si_hashtableFree(siHashTable ht);
#endif

#if !defined(SI_NO_FILE)
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
	|  siFile & siPath     |
	========================
*/

typedef struct {
    /* libc FILE pointer. */
    FILE* ptr;
    /* Size of the read file. */
	usize size;
    /* Filename of the file. */
    cstring filename;
    /* Last time it was written at. */
    u64 lastWriteTime;
} siFile;

#if defined(SI_SYSTEM_WINDOWS)
	const char SI_PATH_SEPARATOR = '\\';
#else
	const char SI_PATH_SEPARATOR = '/';
#endif

/*
	========================
	|  siPath              |
	========================
*/

/* Returns a boolean indicating if the NULL-terminated C-string path exists. */
b32 si_pathExists(cstring path);
/* Copies the File System object from 'existingPath' to 'newPath'. Returns the size
 * of the copied file. */
isize si_pathCopy(cstring existingPath, cstring newPath);
/* Moves the File System object from 'existingPath' to 'newPath'. Returns true
 * if the path was moved successfully. */
b32 si_pathMove(cstring existingPath, cstring newPath);
/* Removes the File System object specified at 'path'. Returns true if the removal
 * was successful. */
b32 si_pathRemove(cstring path);

/* Returns a pointer that points to the start of the base name. */
cstring si_pathBaseName(cstring path);
/* Returns a pointer that points to the start of the file extension, if any. */
// TODO(EimaMei): Test this with a function that does not contain a dot.
cstring si_pathExtension(cstring path);
/* Returnss a siString of the full filename of the given path and and writes it
 * into the allocator. */
siString si_pathGetFullName(siAllocator* alloc, cstring path);

/* Returns true if the path is absolute. */
b32 si_pathIsAbsolute(cstring path);
/* Returns true if the path is relative. */
b32 si_pathIsRelative(cstring path);

/* Returns the last time the path was edited. */
u64 si_pathLastWriteTime(cstring filename);

/*
	========================
	|  siFile              |
	========================
*/
/* Creates a file if the path doesn't exist. Mode is "w+". */
siFile si_fileCreate(cstring path);
/* Opens the provided file. Mode is "r+". */
siFile si_fileOpen(cstring path);
/* Opens the provided file in binary. Mode is "rb+". */
siFile si_fileOpenBin(cstring path);
/* The file is opened under the user specified mode. */
siFile si_fileOpenMode(cstring path, cstring mode);

/* Returns the size of the file. */
usize si_fileSize(siFile file);
/* Updates the size of the file in the structure. */
void si_fileSizeUpdate(siFile* file);

/* Reads the contents of the file, writes it into the allocator and then returns
 * a pointer for the data. */
rawptr si_fileRead(siAllocator* alloc, siFile file);
/* Reads the contents of the file, allocates 'fileSize + additionalAllocSpace' bytes,
 * then the read data gets written into it. Afterwards the function returns a pointer
 * to the data. */
rawptr si_fileReadEx(siAllocator* alloc, siFile file, usize additionalAllocSpace);
/* Reads the contents of the file from 'offset' to 'offset + len', writes it
 * into the allocator and then returns a pointer to the data. */
rawptr si_fileReadAt(siAllocator* alloc, siFile file, usize offset, usize len);
/* Reads the contents of the file from 'offset' to 'offset + len', allocates
 * 'fileSize + additionalAllocSpace' bytes, then the read data gets written into
 * it. Afterwards the function returns a pointer to the data. */
rawptr si_fileReadAtEx(siAllocator* alloc, siFile file, usize offset, usize len,
        usize additionalAllocSpace);
/* Reads the contents of the file until it reaches a new line. It then writes to
 * the allocator and returns the C-string. If EOF is reached, nil is returned. */
rawptr si_fileReadLine(siAllocator* alloc, siFile file);
/* Reads the contents of the file, then splits every line and writes them all
 * into a 'siArray(siString)'.*/
siArray(siString) si_fileReadlines(siAllocator* alloc, siFile file);

/* writes a null-terminated c-string into the file. returns the amount of characters
 * that were written into the file. */
usize si_fileWrite(siFile* file, rawptr content);
/* Writes a C-string with specified length into the file. Returns the amount of
 * characters that were written into the file. */
usize si_fileWriteLen(siFile* file, rawptr content, usize len);
/* Writes a NULL-terminated string into the file at the specified offset. Returns
 * the amount of characters that were written into the file. */
usize si_fileWriteAt(siFile* file, rawptr content, usize offset);
/* Writes a C-string with specified length into the file at the specified offset.
 * Returns the amount of characters that were written into the file. */
usize si_fileWriteAtLen(siFile* file, rawptr content, usize contentLen, usize offset);
/* Writes a C-string into the file at the specified line. Writes the amount of
 * characters that were written into the file. */
usize si_fileWriteAtLine(siFile* file, cstring content, usize index);

/* Seeks the file offset to the specified offset. */
b32 si_fileSeek(siFile file, usize offset);
/* Seeks to the end of the file. */
b32 si_fileSeekToEnd(siFile file);
// TODO(EimaMei): si_fileSeekToFront

/* Closes the file. */
void si_fileClose(siFile file);

/*
	========================
	|  siDirectory & siIO  |
	========================
*/
typedef SI_ENUM(i32, siIOType) {
    SI_IO_TYPE_ANY = 1,
    SI_IO_TYPE_FILE = DT_REG,
    SI_IO_TYPE_DIR = DT_DIR,
    SI_IO_TYPE_LINK = DT_LNK,
};

/* TODO(EimaMei): Add windows support. */
typedef struct {
    /* A string containing the path. */
    char path[256];
    /* */
    usize len;
    /* An integer denoting if the object is a file, directory or link. */
    siIOType type;
} siDirectoryEntry;

typedef struct {
    rawptr handle;
    char* path;
} siDirectory;

// TODO(EimaMei): Put this in the IMPLEMENTATION section, obviously.

/* Reads every file, directory and link from a NULL-terminated string path and
 * creates an array containing the results. All 'siIOInfo' paths aren't prefixed
 * with 'basePath', just their filenames. */
siDirectory si_dirOpen(char* path) {
#if defined(SI_SYSTEM_WINDOWS)

#else
    DIR* handle = opendir(path);
    siDirectory dir;
    dir.handle = handle;
    dir.path = path;

    if (handle == nil) {
        cstring message;
        switch (errno) {
            case EACCES:  message = "Failed to execute 'si_dirOpen': Permission denied (EACCES).";
            case EBADF:   message = "Failed to execute 'si_dirOpen': fd is not a valid file descriptor opened for reading (EBADF).";
            case EMFILE:  message = "Failed to execute 'si_dirOpen': The per-process limit on the number of open file descriptors has been reached. (EMFILE).";
            case ENFILE:  message = "Failed to execute 'si_dirOpen': The system-wide limit on the total number of open files has been reached (ENFILE).";
            case ENOENT:  message = "Failed to execute 'si_dirOpen': Directory does not exist, or path is an empty string (ENOENT).";
            case ENOMEM:  message = "Failed to execute 'si_dirOpen': Insufficient memory to complete the operation (ENOMEM).";
            case ENOTDIR: message = "Failed to execute 'si_dirOpen': path is not a directory (ENOTDIR).";
        }
        SI_PANIC_MSG(message);
        return dir;
#endif
    }
    { (void)readdir(handle); (void)readdir(handle); }

    return dir;
#endif
}

siDirectoryEntry* si_dirPollEntry(siDirectory dir, siDirectoryEntry* entry) {
#if defined (SI_SYSTEM_WINDOWS)

#else
    SI_ASSERT_NOT_NULL(entry);

    // TODO(EimaMei): figure out a way to somehow get the actual length.
    struct dirent* direntEntry = readdir((DIR*)dir.handle);
    SI_STOPIF(direntEntry == nil, return nil);

    entry->len = strlen(direntEntry->d_name);
    memcpy(entry->path, direntEntry->d_name, entry->len + 1);
    entry->type = direntEntry->d_type;

    return entry;
#endif
}


//siArray(siIOInfo) si_directoryReadEx(siAllocator* alloc, cstring basePath,
//        usize basePathLen, b32 fullPath);


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


typedef struct {
	#if defined(SI_SYSTEM_WINDOWS)
        /* Win32 thread ID. */
		HANDLE id;
	#else
        /* POSIX thread ID. */
		pthread_t id;
	#endif
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

/* function - NAME | arg - rawptr
 * Returns a siThread from the given function name and argument. */
#define si_threadCreate(function, arg) si_threadCreateEx(siFunc(function), arg)
/* Returns a siThread from the given name and argument. */
siThread si_threadCreateEx(siFunction function, rawptr arg);

/* Starts the thread. */
void si_threadStart(siThread* t);
/* Starts the thread with a specified stack size. */
void si_threadStartStack(siThread* t, usize stackSize);
/* Starts the thread while hanging the parent thread until the specified thread
 * is finished. */
void si_threadJoin(siThread* t);
/* thread - siThread | type - TYPE
 * Casts 'thread.returnValue' correctly to match the given type instead of being rawptr. */
#define si_threadGetReturn(thread, type) (*(type*)(&thread.returnValue))

/* Stops the thread mid-execution. */
void si_threadCancel(siThread* t); /* NOTE(EimaMei): Only works on Unix. */
/* Destroys the thread. */
void si_threadDestroy(siThread* t);

/* Sets the priority of the thread. */
void si_threadPrioritySet(siThread t, i32 priority);

#endif

#if !defined(SI_TIME_UNDEFINE)
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

#if !defined(SI_TIME_FIRST_WEEKDAY_MONDAY) && !defined(SI_TIME_FIRST_WEEKDAY_FRIDAY) && !defined(SI_TIME_FIRST_WEEKDAY_SATURDAY) && !defined(SI_TIME_FIRST_WEEKDAY_SUNDAY)
	#define SI_TIME_FIRST_WEEKDAY_MONDAY
#endif

typedef SI_ENUM(usize, siMonth) {
	siJanuary = 1,
	siFebruary,
	siMarch,
	siApril,
	siMay,
	siJune,
	siJuly,
	siAugust,
	siSeptember,
	siOctober,
	siNovember,
	siDecember
};

/* NOTE(EimaMei): For whatever reason some places uses Sunday as the first weekday, while MOST of Europe views monday as the first weekday.
				 So by default Monday will be set as the first weekday with the options to change it depending on your taste. */
typedef SI_ENUM(usize, siWeek) {
	#if defined(SI_TIME_FIRST_WEEKDAY_MONDAY)
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,
		Sunday
	#elif defined(SI_TIME_FIRST_WEEKDAY_FRIDAY) /* NOTE(EimaMei): why do the Maldives do this??? */
        Friday,
		Saturday,
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday
	#elif defined(SI_TIME_FIRST_WEEKDAY_SATURDAY) /* NOTE(EimaMei): Shoutout to that one Arab using this library. */
		Saturday,
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday
	#elif defined(SI_TIME_FIRST_WEEKDAY_SUNDAY)
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
	#endif
};

/* Executes the 'RDTSC' asm instruction and returns the value in nano seconds.
 * NOTE: Only natively works for x86-64, i386, ARM64 and PPC CPUs. On other CPUs
 * the function relies on OS functions like 'gettimeofday'. */
u64 si_RDTSC(void);
/* Returns the number of microseconds since 1601-01-01 UTC */
u64 si_timeNowUTC(void);
/* Gets the current time in the local time zone format.*/
// TODO(EimaMei): u64 si_timeNowLocal(void);

/* Makes the CPU sleep for a certain amount of miliseconds. */
void si_sleep(usize miliseconds);

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

typedef SI_ENUM(usize, siBitType) {
    /* 0b00000000. */
    SI_BIT_ZERO,
    /* 0b00000001. */
    SI_BIT_ONE
};

/* x - UINT
 * Shortcut for (1 << x). */
#define SI_BIT(x) ((usize)1 << si_cast(usize, x))
/* num - INT
 * Gets the most significant bit of the number. */
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

/* value - EXPRESSION | flag - UINT
 * Returns 1 or 0 depending if the bit flag is set in the value. */
#define SI_NUM_BIT_GET(value, flag) (((value) & (flag)) != 0)
/* variable - VARIABLE | flag - UINT | condition - EXPRESSION
 * If 'condition' evaluates to true, the provided 'variable's' 'flag' is set.
 * If it evaluates to false, then the 'flag' is set to 0. */
#define SI_NUM_BIT_SET(variable, flag, condition) \
    (condition) ? ((variable) |= (flag)) : ((variable) &= ~(flag))


/* ptr - rawptr | sizeofNum - usize
 * Extracts 'sizeofNum' amount of bytes from a given pointer. Maximum is 8 bytes. */
#define si_u64FromPtr(ptr, sizeofNum) \
    (*si_cast(u64*, ptr) & ((sizeofNum < 8) ? (SI_BIT((sizeofNum) * 8) - 1) : UINT64_MAX) )

/* x - INT
 * Returns true if x is negative. */
#define si_numIsNeg(x) (x < 0)

/* Returns how many 1 bits are in an 8-bit number. */
usize si_numCountBitsU8(u8 num);
/* Returns how many 1 bits are in a 32-bit number. */
usize si_numCountBitsU32(u32 num);
/* Returns how many 1 bits are in a 64-bit number. */
usize si_numCountBitsU64(u64 num);

#if SI_STANDARD_VERSION >= SI_STANDARD_C11
    #define si_numCountBits(num) \
        _Generic((num), \
            u8  : si_numCountBitsU8(num), \
            u16 : si_numCountBitsU32(num), \
            u32 : si_numCountBitsU32(num), \
            u64 : si_numCountBitsU64(num), \
            default: si_numCountBitsU64(num) \
        )(X)
#endif

/* num - UINT | bitType - siBitType
 * Returns the amount of leading bits of the specified bit type (0 or 1). */
#define si_numLeadingBit(num, bitType) si_numLeadingBitEx(num, sizeof(typeof(num)) * 8, bitType)
/* num - UINT | bitType - siBitType
 * Returns the amount of trailing bits of the specified bit type (0 or 1). */
#define si_numTrailingBit(num, bitType) si_numTrailingBitEx(num, sizeof(typeof(num)) * 8, bitType)
/* num - UINT | bits - usize
 * Rotates the bits of the number left by 'bits' amount. */
#define si_numRotateLeft(num, bits) si_numRotateLeftEx(num, sizeof(typeof(num)) * 8, bits)
/* num - UINT | bits - usize
 * Rotates the bits of the number right by 'bits' amount. */
#define si_numRotateRight(num, bits) siNumRotateRightEx(num, sizeof(typeof(num)) * 8, bits)
/* num - UINT
 * Reverses the bits of the number. */
#define si_numReverseBits(num) siNumReverseBitsEx(num, sizeof(typeof(num)) * 8 )
/* allocator - siAllocator* | num - UINT
 * Creates a 'siArray(u8)' from the specified number and writes it into the allocator. */
#define si_numToBytes(allocator, num) si_numToBytesEx(allocator, num, sizeof(typeof(num)))
/* Makes a number from the specified siArray(u8). */
u64 si_bytesToNumSiArr(siArray(u8) arr);
/* TODO(EimaMei): u64 si_bytesToNumCArr */

/* Returns the length of the number. */
usize si_numLen(u64 num);
/* Rounds up the number to the nearest multiple and returns it. */
u64 si_numRoundNearestMultiple(u64 num, usize multiple);


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
typedef usize siMhz;

/* Returns the CPU's clock speed in mHz.
 * NOTE: MAKE SURE TO CACHE AND STORE THIS, as it takes 100 ms to use this function. */
siMhz si_cpuClockSpeed(void);

#endif


#if !defined(SI_NO_BENCHMARK)

#define SI_PERFORMANCE_MSG \
    "====== BENCHMARK DATA ======\n" \
    "General:\n" \
        "\tFunction - '%s'\n" \
        "\tDuration - '%f' ms\n" \
        "\tExecution count - '%f' million\n"

void si_intern_benchmarkThread(u32* arg);

/* timesToLoop - usize | function - NAME
 * Runs the function 'timesToLoop' times and prints how long it took to finish. */
#define si_benchmarkRunsPerLoop(timesToLoop, function) \
	do { \
		usize index; \
		clock_t timeSince = clock(); \
		for (index = 0; index < (usize)timesToLoop; index++) { \
			function; \
		} \
		timeSince = clock() - timeSince; \
		f64 timeTaken = ((f64)timeSince) / 1000.0; /* NOTE(EimaMei): This will take the time in ms, not seconds. */ \
		printf(SI_PERFORMANCE_MSG, #function, timeTaken, (f64)timesToLoop / 1000000.0); \
	} while(0)
/* timesToLoop - usize | function - NAME
 * Runs the function 'timesToLoop' for 'ms' miliseconds and prints how many times
 * the function got executed.  */
#define si_benchmarkExecutesPerMs(ms, function) \
	do { \
		u64 counter = 0; \
		u32 miliseconds = (u32)ms; \
		siThread thread = si_threadCreate(si_intern_benchmarkThread, &miliseconds); \
		si_threadStart(&thread); \
		while (thread.isRunning) { \
			function; \
			counter += 1; \
		} \
		printf(SI_PERFORMANCE_MSG, #function, (f64)miliseconds, (f64)counter / 1000000.0); \
	} while(0)

/* TODO(EimaMei): Clean this up. */
typedef struct {
    char runsCstr[countof("runs")];
    char spacePad[SI_NUM_MAX_DIGITS];
} siPerformanceMSG;

intern siPerformanceMSG* si_benchmarkGetMsgVars(siAllocator* alloc,
        usize multiplesOf10, usize medianIndex) {

    char runsCstr[] = "runs";
    SI_STOPIF(medianIndex == 1, runsCstr[3] = ' ');

    isize spacePadLen = si_numLen(multiplesOf10) - si_numLen(medianIndex);
    char spacePad[SI_NUM_MAX_DIGITS] = {0};
    memset(spacePad, ' ', spacePadLen);


    siPerformanceMSG* msg = (siPerformanceMSG*)si_malloc(alloc, sizeof(*msg));
    memcpy(msg->runsCstr, runsCstr, countof(runsCstr));
    memcpy(msg->spacePad, spacePad, spacePadLen);

    return msg;
}

typedef struct { u64 first; cstring second; } siBenchmarkLimit;

intern const siBenchmarkLimit siBenchLimit[] = {
    {(u64)0001, "ns"},
    {(u64)1000, "μs"},
    {(u64)1000 * 1000, "ms"},
    {(u64)1000 * 1000 * 1000, " s"},
    {(u64)1000 * 1000 * 1000 * 60, "min"},
    {(u64)1000 * 1000 * 1000 * 60 * 60, " h"},
    {(u64)1000 * 1000 * 1000 * 60 * 60 * 24, " d"} /* NOTE(EimaMei): I'll be shocked if someone someone's performance loop lasted an entire day. Also a bug happens here, I think. Oops. */
};

F_TRAITS(intern)
const siBenchmarkLimit* si_benchmarkLimitLoop(u64 time) {
    const siBenchmarkLimit* element = siBenchLimit;
    const siBenchmarkLimit* end = siBenchLimit + countof(siBenchLimit);

    while (element != end) {
        if (si_between(time, element->first, (element + 1)->first)) {
            break;
        }
        element += 1;
    }

    return element;
}

void si_benchmarkLoopsAvgPrint(siAllocator* alloc, cstring funcname,
        usize start, usize end, siArray(u64) cycles, f32 ghzFreq) {
	printf(
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
        amountTime /= element->first;

        siPerformanceMSG* msg = si_benchmarkGetMsgVars(alloc, end, medianIndex);
        printf(
            "\t%s%zu %s - %9.4f %s (%"PRIu64" cycles)\n",
            msg->spacePad, medianIndex, msg->runsCstr,
            amountTime, element->second, amountCycles
        );
    }

    f64 cyclesMedian = (f64)si_arraySumU64(cycles) / si_arrayLen(cycles);
    f64 timeMedian = cyclesMedian / ghzFreq;
    element = si_benchmarkLimitLoop(timeMedian);
    timeMedian /= element->first;

    printf(
        "Final result:\n"
            "\tTime average   - %9.4f %s\n"
            "\tCycles average - %"PRIu64" cycles\n",
        timeMedian, element->second,
        (u64)cyclesMedian
    );
}

#if defined(SI_SYSTEM_UNIX)
    #define SI_TGRN "\33[0;32m"
    #define SI_TRED "\33[0;31m"
    #define SI_TEND "\33[0m"
#else
    #define SI_TRED ""
    #define SI_TGRN ""
    #define SI_TEND ""
#endif

void si_benchmarkLoopsAvgCmpPrint(siAllocator* alloc, cstring funcname[2],
        usize start, usize end, siArray(u64) cycles[2], f32 ghzFreq) {
	printf(
        "====== BENCHMARK DATA ======\n"
        "General:\n"
            "\tFunction 1 - '%s'\n"
            "\tFunction 2 - '%s'\n"
        "Runs:\n",
        funcname[0], funcname[1]
    );
    const siBenchmarkLimit* element_0 = nil, *element_1 = nil;

    usize arrayIndex = 0, medianIndex;
    for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10) {
        u64 cycles0 = cycles[0][arrayIndex],
            cycles1 = cycles[1][arrayIndex];
        f64 time_0 = (f64)cycles0 / ghzFreq,
            time_1 = (f64)cycles1 / ghzFreq;

        b32 zeroIsSlower = (time_0 > time_1);
        f64 ratio = zeroIsSlower ? (time_0 / time_1) : (time_1 / time_0);
        cstring clr0 = zeroIsSlower ? SI_TRED : SI_TGRN,
                clr1 = zeroIsSlower ? SI_TGRN : SI_TRED;

        element_0 = si_benchmarkLimitLoop(time_0);
        element_1 = si_benchmarkLimitLoop(time_1);
        time_0 /= element_0->first;
        time_1 /= element_1->first;

        siPerformanceMSG* msg = si_benchmarkGetMsgVars(alloc, end, medianIndex);
        printf(
            "\t%s%zu %s - %s%9.4f %s%s vs %s%9.4f %s%s (%4.4f ratio, %"PRIu64" vs %"PRIu64" cycles)\n",
            msg->spacePad, medianIndex, msg->runsCstr,
            clr0, time_0, element_0->second, SI_TEND, clr1, time_1, element_1->second, SI_TEND,
            ratio, cycles0, cycles1
        );

        arrayIndex += 1;
    }

    f64 cycles_median_0 = (f64)si_arraySumU64(cycles[0]) / si_arrayLen(cycles[0]),
        cycles_median_1 = (f64)si_arraySumU64(cycles[1]) / si_arrayLen(cycles[1]);
    f64 time_median_0 = cycles_median_0 / ghzFreq,
        time_median_1 = cycles_median_1 / ghzFreq;

    b32 zeroIsSlower = (time_median_0 > time_median_1);
    f64 ratio = zeroIsSlower ? (time_median_0 / time_median_1) : (time_median_1 / time_median_0);
    cstring clr0 = zeroIsSlower ? SI_TRED : SI_TGRN,
            clr1 = zeroIsSlower ? SI_TGRN : SI_TRED;

    element_0 = si_benchmarkLimitLoop(time_median_0);
    element_1 = si_benchmarkLimitLoop(time_median_1);
    time_median_0 /= element_0->first;
    time_median_1 /= element_1->first;

    printf(
        "Final result:\n"
            "\tTime average   - %s%9.4f %s%s vs %s%9.4f %s%s (%4.4f ratio)\n"
            "\tCycles average - %"PRIu64" cycles vs %"PRIu64" cycles\n",
        clr0, time_median_0, element_0->second, SI_TEND,
        clr1, time_median_1, element_1->second, SI_TEND,
        ratio,
        (u64)cycles_median_0, (u64)cycles_median_1
    );
}

#define si_benchmarkLoop(function, cycles, start, end) \
    do { \
        usize arrayIndex = 0; \
        u64 medianIndex; \
        for (medianIndex = start; medianIndex < 10 * end; medianIndex *= 10) { \
            u64 index; \
            u64 begin = si_RDTSC(); \
            for (index = 0; index < medianIndex; index++) { \
                function; \
            } \
            begin = si_RDTSC() - begin; \
            cycles[arrayIndex] = begin; \
            arrayIndex += 1; \
        } \
        SI_ARRAY_HEADER(cycles)->len = arrayIndex; \
    } while (0)
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
        siAllocator* alloc = si_allocatorMakeStack(SI_KILO(4)); \
		siArray(u64) cycles = si_arrayMakeReserve(alloc, sizeof(u64), 20); \
        si_benchmarkLoop(function, cycles, start, end); \
        \
        si_benchmarkLoopsAvgPrint(alloc, #function, start, end, cycles, (f64)si_cpuClockSpeed() / 1000); \
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
        siAllocator* alloc = si_allocatorMakeStack(SI_KILO(4)); \
		siArray(u64) cycles[2] = { \
            (u64*)si_arrayMakeReserve(alloc, sizeof(u64), 20), \
            (u64*)si_arrayMakeReserve(alloc, sizeof(u64), 20)  \
        };\
        si_benchmarkLoop(function1, cycles[0], start, end); \
        si_benchmarkLoop(function2, cycles[1], start, end); \
        \
        cstring funcnames[2] = {#function1, #function2}; \
        si_benchmarkLoopsAvgCmpPrint(alloc, funcnames, start, end, cycles, (f64)si_cpuClockSpeed() / 1000); \
	} while(0)
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
	#define SI_IMPLEMENTATION_ALLOCATOR
	#define SI_IMPLEMENTATION_PAIR
	#define SI_IMPLEMENTATION_OPTIONAL
	#define SI_IMPLEMENTATION_ARRAY
	#define SI_IMPLEMENTATION_STRING
	#define SI_IMPLEMENTATION_CHAR
    #define SI_IMPLEMENTATION_HASHTABLE
	#define SI_IMPLEMENTATION_FILE
	#define SI_THREAD_IMPLEMENTATION
	#define SI_IMPLEMENTATION_BIT
    #define SI_IMPLEMENTATION_CPU
    #define SI_IMPLEMENTATION_BENCHMARK
#endif


#if defined(SI_IMPLEMENTATION_ALLOCATOR)
#define SI_DEFAULT_MEMORY_ALIGNMENT (sizeof(usize) * 2)

F_TRAITS(inline)
usize si_alignCeil(usize n) {
    SI_STOPIF(n % SI_DEFAULT_MEMORY_ALIGNMENT == 0, return n);
    n |= SI_DEFAULT_MEMORY_ALIGNMENT - 1;
    n += 1;
    return n;
}

F_TRAITS(inline)
siAllocator* si_allocatorMake(usize bytes) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	rawptr ptr = malloc(sizeof(siAllocator) + bytes);
    SI_ASSERT_NOT_NULL(ptr);

	siAllocator* res = (siAllocator*)ptr;
	res->ptr = (siByte*)ptr + sizeof(siAllocator);
	res->offset = 0;
	res->maxLen = bytes;

	return res;
#else
    SI_UNUSED(bytes);
    return nil;
#endif
}
F_TRAITS(intern inline)
siAllocator* si_intern_allocatorMakeStack(usize bytes, rawptr ptr) {
    siAllocator* res = (siAllocator*)ptr;
    res->ptr = (siByte*)ptr + sizeof(siAllocator);
    res->offset = 0;
    res->maxLen = bytes;

    return res;
}
F_TRAITS(inline)
void si_allocatorFree(siAllocator* alloc) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	SI_ASSERT_NOT_NULL(alloc->ptr);
	free(alloc->ptr - sizeof(siAllocator));
#endif
    SI_UNUSED(alloc);
}

F_TRAITS(inline)
void si_allocatorReset(siAllocator* alloc) {
    SI_ASSERT_NOT_NULL(alloc);
    alloc->offset = 0;
}

F_TRAITS(inline)
void si_allocatorResetFrom(siAllocator* alloc, usize offset) {
    SI_ASSERT_NOT_NULL(alloc);
    SI_ASSERT_MSG(si_between(offset, 0, alloc->maxLen), "Provided offset is too large.");
    alloc->offset = offset;
}
F_TRAITS(inline)
usize si_allocatorAvailable(siAllocator* alloc) {
    return alloc->maxLen - alloc->offset;
}
F_TRAITS(inline)
void si_allocatorPush(siAllocator* alloc, siByte byte) {
    SI_ASSERT_NOT_NULL(alloc);
    SI_ASSERT_FMT(si_between(alloc->offset + 1, 0, alloc->maxLen),
        "Exceeded the available memory for an allocation",
        " (Tried writing '%zd' bytes into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
        alloc->maxLen, alloc->offset);

    alloc->ptr[alloc->offset] = byte;
    alloc->offset += 1;
}
#if !defined(SI_RELEASE_MODE)
F_TRAITS(inline)
rawptr si_mallocEx(siAllocator* alloc, usize bytes, cstring filename, i32 line) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	SI_ASSERT_NOT_NULL(alloc);

	siByte* res = alloc->ptr + alloc->offset;
	alloc->offset += bytes;

	if (alloc->offset > alloc->maxLen) {
        fprintf(stderr,
            "%s:%i: si_malloc: Exceeded the available memory for an allocation"
            " (Tried writing '%zd' bytes into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
              filename, line, bytes, alloc->maxLen, alloc->offset
        );
        exit(1);
    }

	return res;
#else
    return malloc(bytes);
    SI_UNUSED(alloc); SI_UNUSED(bytes); SI_UNUSED(filename); SI_UNUSED(line);
#endif
}

F_TRAITS(inline)
rawptr si_callocEx(siAllocator* alloc, usize num, usize bytes,
        cstring filename, i32 line) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	rawptr res = si_mallocEx(alloc, num * bytes, filename, line);
	memset(res, 0, num * bytes);

	return res;
#else
    return calloc(bytes, num);
    SI_UNUSED(alloc); SI_UNUSED(num); SI_UNUSED(bytes) SI_UNUSED(filename); SI_UNUSED(line);
#endif
}
// TODO(EimaMei): Improve 'si_free'
F_TRAITS(inline)
void si_freeEx(siAllocator* alloc, rawptr ptr, cstring filename, i32 line) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(ptr);
	SI_ASSERT_FMT(
        si_between((siByte*)ptr, alloc->ptr, alloc->ptr + alloc->maxLen),
        "Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
        ptr, alloc->ptr, alloc->ptr + alloc->maxLen
    );
#else
    free(ptr);
    SI_UNUSED(alloc); SI_UNUSED(ptr); SI_UNUSED(filename); SI_UNUSED(line);
#endif
}
rawptr si_reallocEx(siAllocator* alloc, rawptr ptr, usize oldSize, usize newSize,
        cstring filename, i32 line) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
    SI_ASSERT_NOT_NULL(alloc);
    SI_STOPIF(ptr == nil, return si_malloc(alloc, newSize));

    SI_ASSERT_FMT(
        si_between((siByte*)ptr, alloc->ptr, alloc->ptr + alloc->maxLen),
        "%s:%i: Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
        filename, line, ptr, alloc->ptr, alloc->ptr + alloc->maxLen
    );
    rawptr out = si_malloc(alloc, newSize);
    memcpy(out, ptr, oldSize);
    return out;
#else
    return realloc(ptr, newSize);
    SI_UNUSED(alloc); SI_UNUSED(ptr); SI_UNUSED(oldSize); SI_UNUSED(newSize) SI_UNUSED(filename); SI_UNUSED(line);
#endif
}
#endif

#endif


#if defined(SI_IMPLEMENTATION_GENERAL)

usize si_intern_assert(b32 condition, cstring conditionStr, cstring file, i32 line, cstring func, cstring message, ...) {
    SI_STOPIF(condition, return 0);
    fprintf(stderr, "Assertion \"%s\" at \"%s:%d\": %s%s", conditionStr, file, line, func, (message != nil ? ": " : ""));

    if (message != nil) {
        va_list va;

        va_start(va, message);
        vprintf(message, va);
        print("");
        va_end(va);
    }
    else { print(""); }

    abort();

    return 1;
}

F_TRAITS(inline)
u64 si_timeNowUTC(void) {
    time_t rawtime;
    time(&rawtime);

    return rawtime;
}

F_TRAITS(inline)
void si_sleep(usize miliseconds) {
#if defined(SI_SYSTEM_WINDOWS)
    Sleep(miliseconds);
#else
    struct timespec ts = {
        (time_t)miliseconds / 1000,
        si_cast(time_t, miliseconds % 1000) * 1000000
    };
    nanosleep(&ts, &ts);
#endif
}

F_TRAITS(inline)
u64 si_RDTSC(void) {
    /* NOTE(EimaMei): Shoutout to gb.h for the i386 and PPC code! (Link: https://github.com/gingerBill/gb/blob/master/gb.h#L8682C1-L8715C7)*/
    /* TODO(EimaMei): Test these later to make sure all of these work properly. */
    /* todo(EimaMei): Add win32 support and add an option to disable inline asm. */

#if defined(SI_COMPILER_MSVC)
    return __rdtsc();
#elif defined(SI_CPU_X86) && !defined(SI_ARCH_64_BIT)
    u64 res;
    si_asm (".byte 0x0f, 0x31", : "=A" (res));
    return res;
#elif defined(SI_CPU_X86) && defined(SI_ARCH_64_BIT)
    u64 result;
    si_asm(
        "rdtsc"           SI_ASM_NL
        "shl rdx, 0x20"   SI_ASM_NL
        "or rax, rdx",
        SI_ASM_OUTPUT("=a"(result))
      );

    return result;
#elif defined(SI_CPU_PPC)
    u32 high, low, tmp;
    si_asm (
            "0:                   \n"
            "\tmftbu   %0         \n"
            "\tmftb    %1         \n"
            "\tmftbu   %2         \n"
            "\tcmpw    %2,%0      \n"
            "\tbne     0b         \n",
            : "=r"(high), "=r"(low), "=r"(tmp)
           )*/;

    return ((u64)high << 32) | low;
#elif defined(SI_CPU_ARM64)
    u64 result;
    si_asm ("mrs %0, cntvct_el0", SI_ASM_OUTPUT("=r"(result)));
    return result;

#elif defined(SI_SYSTEM_UNIX)
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
#else
    SI_PANIC_MSG("RDTSC is not supported on this platform.");
#endif
}


F_TRAITS(inline)
void si_ptrMoveBy(rawptr src, usize srcLen, isize moveBy, siDirection direction) {
    isize mul = (direction == SI_DIRECTION_RIGHT ? 1 : -1); /* NOTE(EimaMei): Branchless way of moving. */
    memcpy((siByte*)src + (moveBy * mul), src, srcLen);
}

/* TODO(EimaMei): Check for every usage of malloc, calloc, realloc and free to see if they're correct. */
#endif


#if defined(SI_IMPLEMENTATION_PAIR) && !defined(SI_NO_PAIR)
F_TRAITS(intern inline)
rawptr si_intern_pairMake(siAllocator* alloc, siAny first, siAny second) {
	siByte* res = (siByte*)si_malloc(alloc, first.typeSize + second.typeSize);
	memcpy(res, first.ptr, first.typeSize);
	memcpy(res + first.typeSize, second.ptr, second.typeSize);

	return (rawptr)res;
}
#endif

#if defined(SI_IMPLEMENTATION_OPTIONAL) && !defined(SI_NO_OPTIONAL)
F_TRAITS(intern inline)
rawptr si_intern_optionalMake(rawptr res, siAny any, usize offset) {
    siByte* ptr = (siByte*)res;
    memcpy(ptr, any.ptr, any.typeSize);
    *(b32*)(ptr + offset) = true;
    return ptr;
}
#endif

#if defined(SI_IMPLEMENTATION_ARRAY) && !defined(SI_NO_ARRAY)

F_TRAITS(inline)
rawptr si_arrayMakeList(siAllocator* alloc, rawptr list, usize sizeofItem, usize count)  {
	rawptr array = si_arrayMakeReserve(alloc, sizeofItem, count);
	memcpy(array, list, sizeofItem * count);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	header->len = count;

	return array;
}
F_TRAITS(inline)
rawptr si_arrayCopy(siAllocator* alloc, rawptr array) {
	siArrayHeader* header = SI_ARRAY_HEADER(array);
	return si_arrayMakeList(alloc, array, header->typeSize, header->len);
}
rawptr si_arrayMakeReserve(siAllocator* alloc, usize sizeofItem, usize count) {
	SI_STOPIF(count == 0, count = 1);

	rawptr ptr = si_malloc(alloc, sizeof(siArrayHeader) + (sizeofItem * count));
	rawptr array = (siByte*)ptr + sizeof(siArrayHeader);

	siArrayHeader* header = (siArrayHeader*)ptr;
	header->len = 0;
	header->capacity = count;
	header->typeSize = sizeofItem;
    header->allocator = alloc;

	return array;
}

F_TRAITS(inline)
usize si_arrayLen(rawptr array) {
	return SI_ARRAY_HEADER(array)->len;
}
F_TRAITS(inline)
usize si_arrayCapacity(rawptr array) {
	return SI_ARRAY_HEADER(array)->capacity;
}
F_TRAITS(inline)
usize si_arrayTypeSize(rawptr array) {
	return SI_ARRAY_HEADER(array)->typeSize;
}
F_TRAITS(inline)
usize si_arrayTotalSize(rawptr array) {
	return si_arrayCapacity(array) * si_arrayTypeSize(array);
}
inline siAllocator* si_arrayAllocator(rawptr array) {
    return SI_ARRAY_HEADER(array)->allocator;
}
F_TRAITS(inline)
b32 si_arrayEmpty(rawptr array) {
    return (si_arrayLen(array) == 0 || array == nil);
}

F_TRAITS(inline)
rawptr si_arrayAt(rawptr array, usize index) {
	SI_STOPIF(index >= si_arrayLen(array), return nil);

    return si_arrayGetPtr(array, index);
}
F_TRAITS(inline)
rawptr si_arrayFront(rawptr array) {
	SI_ASSERT_MSG(si_arrayLen(array) != 0, "Array is empty.");
	return array;
}
F_TRAITS(inline)
rawptr si_arrayBack(rawptr array) {
	SI_ASSERT_MSG(si_arrayLen(array) != 0, "Array is empty.");
	return si_arrayGetPtr(array, si_arrayLen(array) - 1);
}

isize si_impl_array_find(rawptr array, usize start, usize end, siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_MSG(value.typeSize <= si_arrayTypeSize(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start < end, "Value 'start' is larger than 'end'");

	b32 found = false;
	usize i;
	for (i = start; i < end; i++) {
		if (memcmp((siByte*)array + i * si_arrayTypeSize(array), value.ptr, value.typeSize) == 0) {
			found = true;
			break;
		}
	}

	return (found ? (isize)i : SI_ERROR);
}
isize si_impl_array_rfind(rawptr array, usize start, usize end, siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_MSG(value.typeSize <= si_arrayTypeSize(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start >= end, "Value 'end' is larger than 'start'");

	b32 found = false;
	usize i;
	for (i = start; i < end - 1; i--) {
		if (memcmp((siByte*)array + i * si_arrayTypeSize(array), value.ptr, value.typeSize) == 0) {
			found = true;
			break;
		}
	}

	return (found ? (isize)i : SI_ERROR);
}

void si_impl_array_replace(rawptr array, siAny oldValue, siAny newValue) {
	SI_ASSERT_NOT_NULL(array);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	SI_ASSERT_MSG(
		si_between(newValue.typeSize, oldValue.typeSize, header->typeSize),
		"The given value's sizeof is too large compared to the elements' in the array."
	);

	isize index = 0;
	while (true) {
		index = si_impl_array_find(array, index, header->len, oldValue);
		if (index == SI_ERROR) {
			return ;
    	}

	    memcpy(si_arrayGetPtr(array, index), newValue.ptr, newValue.typeSize);
    }
}
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
		memcpy(tmp, a, typeSize);
		memcpy(a, b, typeSize);
		memcpy(b, tmp, typeSize);

		a += typeSize;
        b -= typeSize;
	}
}
siString si_arrayToSistring(siArray(char*) array, cstring separator) {
	SI_ASSERT_NOT_NULL(array);

	usize separatorLen = si_cstrLen(separator);
	usize totalSize = 0;

	for_range (i, 0, si_arrayLen(array)) {
		totalSize += si_cstrLen(array[i]) + separatorLen;
	}

	siString result = si_stringMakeReverse(SI_ARRAY_HEADER(array)->allocator, totalSize);
	rawptr backPtr = si_arrayBack(array);
	for_each (str, array) {
		if (separator != nil && (&str) != backPtr) {
			si_stringJoin(&result, separator, *str);
		}
		else {
			si_stringAppend(&result, *str);
		}
	}

	return result;
}

F_TRAITS(inline)
void si_arrayPop(rawptr arrayPtr) {
    siByte* array = *si_cast(siByte**, arrayPtr);
    SI_ARRAY_HEADER(array)->len -= 1;
}

void si_intern_arrayInsert(rawptr arrayPtr, siAny newItem, usize index) {
    SI_ASSERT_NOT_NULL(arrayPtr);

	siByte* array = *si_cast(siByte**, arrayPtr);
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	usize previous_len = header->len;
	usize before_index_len = previous_len - index;

	SI_ASSERT_FMT(index < previous_len, "Index is higher than the length of the array (array - '%zd', index - '%zd')", previous_len, index);
	header->len += 1;

	if (header->capacity < header->len) {
		rawptr result = si_realloc(
            header->allocator,
            header,
            sizeof(siArrayHeader) + si_arrayTotalSize(array),
            sizeof(siArrayHeader) + 2 * (header->capacity * header->typeSize)
        );

		array = (siByte*)result + sizeof(siArrayHeader);
		*si_cast(siByte**, arrayPtr) = array;

		header = (siArrayHeader*)result;
		header->capacity *= 2;
	}

	memcpy(
        array + (header->len - before_index_len) * header->typeSize,
        array + index * header->typeSize,
        before_index_len * header->typeSize
    );
	memcpy(array + index * header->typeSize, newItem.ptr, newItem.typeSize);
}
F_TRAITS(inline)
void si_arrayErase(rawptr arrayPtr, usize index) {
    si_arrayEraseCount(arrayPtr, index, 1);
}
void si_arrayEraseCount(rawptr arrayPtr, usize index, usize count) {
    SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	usize len = si_arrayLen(array);
	SI_ASSERT_MSG(index < len, "Index is higher than the length of the string.");
    SI_ASSERT_FMT(index + count <= len, "Index + count is higher than the length of the array (%zd > %zd).", index + count, len);

	memcpy(array + index * si_arrayTypeSize(array), array + (index + count) * si_arrayTypeSize(array), (len - index - count) * si_arrayTypeSize(array));

	SI_ARRAY_HEADER(array)->len -= count;
}
void si_impl_arrayRemoveItem(rawptr arrayPtr, siAny value) {
    SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);

    SI_ASSERT_MSG(value.typeSize <= si_arrayTypeSize(array), "The given value's sizeof is too large compared to the elements' in the array.");
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	while (header->len != 0) {
		isize index = si_impl_array_rfind(array, header->len - 1, 0, value);
		if (index == -1) {
			break;
		}

		memcpy(si_arrayGetPtr(array, index), si_arrayGetPtr(array, index + header->len), value.typeSize);

        if (header->len != 0) {
            header->len -= 1;
        }
	}
}
void si_impl_array_fill(rawptr arrayPtr, usize index, usize count, siAny value) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	SI_ASSERT_MSG(value.typeSize <= header->typeSize, "The given value's sizeof is too large compared to the elements' in the array.");
    SI_ASSERT_MSG(index < header->capacity, "Index is higher than the array's length.");

    usize previous_len = header->len;
    isize size_dif = si_abs((isize)index - (isize)count);
    header->len += size_dif;

	if (header->capacity < header->len) {
		rawptr result = si_realloc(
            header->allocator,
            header,
            sizeof(siArrayHeader) + si_arrayTotalSize(array),
            sizeof(siArrayHeader) + size_dif +  2 * header->capacity * header->typeSize
        );
		SI_ASSERT_NOT_NULL(result);

		array = (siByte*)result + sizeof(siArrayHeader);
		*si_cast(siByte**, arrayPtr) = array;

		header = (siArrayHeader*)result;
		header->capacity = header->capacity * 2 + size_dif;
	}

    for_range(i, index, count) {
        memcpy(si_arrayGetPtr(array, previous_len + i), value.ptr, header->typeSize);
    }
}

rawptr si_intern_arrayAppend(rawptr arrayPtr, siAny value) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	SI_ASSERT_MSG(value.typeSize <= header->typeSize, "The given value's sizeof is too large compared to the elements' in the arra");

    usize previous_len = header->len;
    header->len += 1;

	if (header->capacity < header->len) {
		rawptr result = si_realloc(
            header->allocator,
            header,
            sizeof(siArrayHeader) + si_arrayTotalSize(array),
            sizeof(siArrayHeader) + 2 * si_arrayTotalSize(array)
        );

		array = (siByte*)result + sizeof(siArrayHeader);
		*si_cast(siByte**, arrayPtr) = array;

		header = (siArrayHeader*)result;
		header->capacity *= 2;
	}

	return memcpy(si_arrayGetPtr(array, previous_len), value.ptr, value.typeSize);
}

void si_arrayClear(rawptr arrayPtr) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	rawptr array = si_cast(siByte*, arrayPtr);

	memset(array, 0, si_arrayTotalSize(array));
}

b32 si_arrayEqual(rawptr lha, rawptr rha) {
	siArrayHeader* lha_header = SI_ARRAY_HEADER(lha);
	siArrayHeader* rha_header = SI_ARRAY_HEADER(rha);

	if (lha_header->len != rha_header->len || lha_header->typeSize != rha_header->typeSize) {
		return false;
	}

	usize width = lha_header->typeSize;
	usize i;
	for (i = 0; i < lha_header->len; i++) {
		if (memcmp((siByte*)lha + i * width, (siByte*)rha + i * width, width) != SI_OKAY) {
			return false;
		}
	}

	return true;
}

F_TRAITS(inline)
void si_arrayFree(rawptr array) {
    SI_ASSERT_NOT_NULL(array);
	free(SI_ARRAY_HEADER(array));
}

void si_arrayShrinkToFit(rawptr arrayPtr) {
	SI_ASSERT_NOT_NULL(arrayPtr);
	siByte* array = *si_cast(siByte**, arrayPtr);
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	header = (typeof(header))si_realloc(
        header->allocator,
        header,
        sizeof(siArrayHeader) + si_arrayTotalSize(array),
        sizeof(siArrayHeader) + header->len * header->typeSize
    );
	header->capacity = header->len;
}

F_TRAITS(inline)
u64 si_arraySumU64(siArray(void) array) {
    SI_ASSERT_NOT_NULL(array);
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	u64 res = 0;
	siByte* ptr = (siByte*)array;
    siByte* end = ptr + header->len * header->typeSize;

    while (ptr < end) {
		res += si_u64FromPtr(ptr, header->typeSize);
	    ptr += header->typeSize;
    }

	return res;
}
F_TRAITS(inline)
i64 si_arraySumI64(siArray(void) array) {
    SI_ASSERT_NOT_NULL(array);
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	i64 res = 0;
	siByte* ptr = (siByte*)array;
    siByte* end = ptr + header->len * header->typeSize;

    while (ptr < end) {
		res += (i64)si_u64FromPtr(ptr, header->typeSize);
	    ptr += header->typeSize;
    }

	return res;
}
F_TRAITS(inline)
f64 si_arraySumF64(siArray(void) array) {
    SI_ASSERT_NOT_NULL(array );
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	f64 res = 0;
	siByte* ptr = (siByte*)array;
    siByte* end = ptr + header->len * header->typeSize;

    while (ptr < end) {
		res += (f64)si_u64FromPtr(ptr, header->typeSize);
	    ptr += header->typeSize;
    }

	return res;
}

#if defined(SI_IMPLEMENTATION_STRING) && !defined(SI_NO_STRING)

F_TRAITS(inline)
siString si_stringMake(siAllocator* alloc, cstring str) {
	return si_stringMakeLen(alloc, str, str ? si_cstrLen(str) : 0);
}
F_TRAITS(inline)
siString si_stringMakeLen(siAllocator* alloc, cstring str, usize len) {
    return si_stringMakeEx(alloc, str, len, len);
}
siString si_stringMakeEx(siAllocator* alloc, cstring str, usize len, usize capacity) {
	siString resStr = si_stringMakeReverse(alloc, capacity);
	memcpy(resStr, str, len);

	siStringHeader* header = SI_STRING_HEADER(resStr);
	header->len = len;

	return resStr;
}
siString si_stringMakeFmt(siAllocator* alloc, cstring str, ...) {
    SI_ASSERT_NOT_NULL(str);

    va_list args, copy;
    va_start(args, str);

    va_copy(copy, args);
    usize  size = vsnprintf(nil, 0, str, copy);
    va_end(copy);

    siString buffer = si_stringMakeReverse(alloc, size);
    vsnprintf(buffer, size + 1, str, args);
    va_end(args);

    return buffer;
}
siString si_stringMakeReverse(siAllocator* alloc, usize capacity) {
	rawptr ptr = si_malloc(alloc, sizeof(siStringHeader) + (capacity +  1));
	SI_ASSERT_NOT_NULL(ptr);

	siString resStr = (siString)ptr + sizeof(siStringHeader);
	resStr[capacity] = '\0';

	siStringHeader* header = SI_STRING_HEADER(resStr);
	header->len = 0;
	header->capacity = capacity;
    header->allocator = alloc;
	/*header->typeSize = sizeof(char); */

	return resStr;
}

F_TRAITS(inline)
siString si_stringCopy(siAllocator* alloc, siString from) {
	return si_stringMakeLen(alloc, from, si_stringLen(from));
}

F_TRAITS(inline)
usize si_stringLen(siString str) {
	return SI_STRING_HEADER(str)->len;
}
F_TRAITS(inline)
usize si_stringCapacity(siString str) {
	return SI_STRING_HEADER(str)->capacity;
}
F_TRAITS(inline)
usize si_stringTypeSize(siString str) {
	return sizeof(char);
    SI_UNUSED(str);
}
inline siAllocator* si_stringAllocator(siString str) {
    return SI_STRING_HEADER(str)->allocator;
}
F_TRAITS(inline)
b32 si_stringEmpty(siString str) {
	return (str == nil || SI_STRING_HEADER(str)->len == 0);
}

F_TRAITS(inline)
char si_stringAt(siString str, usize index) {
	SI_ASSERT_NOT_NULL(str);
	if (index > si_stringLen(str) || si_stringLen(str) == 0) {
		return SI_ERROR;
	}

	return str[index];
}
F_TRAITS(inline)
char si_stringFront(siString str) {
	SI_ASSERT_NOT_NULL(str);
	if (si_stringLen(str) == 0) {
		return SI_OKAY;
	}

	return str[0];
}
F_TRAITS(inline)
char si_stringBack(siString str) {
	SI_ASSERT_NOT_NULL(str);
	if (si_stringLen(str) == 0) {
		return '\0';
	}

	return str[si_stringLen(str) - 1];
}
char* si_stringSub(siAllocator* alloc, siString str, usize begin, usize len) {
    char* result = si_mallocArray(alloc, char, len);
	memcpy(result, str + begin, len);
	*(result + len) = '\0';
    return result;
}

F_TRAITS(inline)
isize si_stringFind(siString str, cstring cstr) {
	return si_stringFindEx(str, 0, si_stringLen(str), cstr, si_cstrLen(cstr));
}
isize si_stringFindEx(siString str, usize start, usize end, cstring cstr, usize cstrLen) {
	SI_ASSERT_NOT_NULL(str);

	b32 found = false;

	usize i;
	usize cur_count = 0;
	for (i = start; i < end; i++) {
		if (str[i] == cstr[cur_count]) {
			cur_count++;

			if (cur_count == cstrLen) {
				found = true;
				break;
			}
		}
		else if (cur_count != 0) {
			cur_count = 0;
		}
	}

	return (found ? si_cast(isize, i - cstrLen + 1) : SI_ERROR);
}
F_TRAITS(inline)
isize si_stringRFind(siString str, cstring cstr) {
	return si_stringRFindEx(str, si_stringLen(str) - 1, 0, cstr);
}
isize si_stringRFindEx(siString str, usize start, usize end, cstring cstr) {
	SI_ASSERT_NOT_NULL(str);

	usize cstrLen = si_cstrLen(cstr);
	b32 found = false;

	usize i;
	isize cur_count = cstrLen - 1;
	for (i = start; i < end - 1; i--) {
		if (str[i] == cstr[cur_count]) {
			cur_count -= 1;

			if (cur_count < 0) {
				found = true;
				break;
			}
		}
	}

	return (found ? (isize)i : SI_ERROR);
}

void si_stringJoin(siString* str, cstring cstr, cstring separator) {
	usize cstrLen = si_cstrLen(cstr);
	usize separatorLen = si_cstrLen(separator);

	siAllocator* alloc = si_allocatorMakeStack(SI_KILO(8)); /* NOTE(EimaMei):
                                            8k should be enough for most cases,
                                            I think. Maybe it's still more faster
                                            and safer to just do a heap allocation?
                                            */

	memcpy(alloc, separator, separatorLen);
	memcpy((siByte*)alloc + separatorLen, cstr, cstrLen);

	si_stringAppendLen(str, (char*)alloc, separatorLen + cstrLen);
}
void si_stringSet(siString* str, cstring cstr) {
	SI_ASSERT_NOT_NULL(str);
	siString curStr = *str;

	usize len = si_cstrLen(cstr);

	siStringHeader* header = SI_STRING_HEADER(curStr);

	if (header->capacity < len) {
		si_stringMakeSpaceFor(str, len - header->capacity);
		curStr = *str;
		header = SI_STRING_HEADER(curStr);
	}
	header->len = len;

	memcpy(curStr, cstr, len);
	curStr[len] = '\0';
}
F_TRAITS(inline)
void si_stringReplace(siString* str, cstring oldValue, cstring newValue) {
    si_stringReplaceEx(str, oldValue, si_cstrLen(oldValue), newValue, si_cstrLen(newValue));
}
void si_stringReplaceEx(siString* str, cstring oldValue, usize oldLen, cstring newValue,
        usize newLen) {
	isize index = 0;

	while (true) {
        siString curStr = *str;
		index = si_stringFindEx(curStr, index, si_stringLen(curStr), oldValue, oldLen);
		if (index == SI_ERROR) {
			break;
		}

		si_stringErase(curStr, index, oldLen);
		si_stringInsertEx(str, newValue, newLen, index, false);
	}
}
F_TRAITS(inline)
void si_stringTrim(siString str, cstring cutSet) {
    si_stringTrimLen(str, cutSet, strlen(cutSet));
}
void si_stringTrimLen(siString str, cstring cutSet, usize cutLen) {
	char* start = str;
	char* end  = str + si_stringLen(str) - 1;
	char* start_pos = start;
	char* end_pos = end;

	siStringHeader* header = SI_STRING_HEADER(str);
	header->len -= cutLen;

	while (start_pos <= end && strchr(cutSet, *start_pos))
		start_pos++;
	while (end_pos > start_pos && strchr(cutSet, *end_pos))
		end_pos--;

	usize len = (start_pos > end_pos) ? 0 : ((end_pos - start_pos)+1);

	if (str != start_pos) {
		memmove(str, start_pos, len);
	}
	str[len] = '\0';
}
void si_stringEnquote(siString* str) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	header->len += 2;

	if (header->capacity < header->len) {
		si_stringMakeSpaceFor(str, 2);
		header = SI_STRING_HEADER(*str);
	}
	siString curStr = *str;

	si_ptrMoveBy(curStr, header->len, 1, SI_DIRECTION_RIGHT);
	curStr[0] = '\"';
	curStr[header->len - 1] = '\"';
}

F_TRAITS(inline)
void si_stringAppend(siString* str, cstring other) {
	si_stringAppendLen(str, other, si_cstrLen(other));
}
void si_stringAppendLen(siString* str, cstring other, usize otherLen) {
	SI_ASSERT_NOT_NULL(str);
	siString curStr = *str;

	siStringHeader* header = SI_STRING_HEADER(curStr);
	usize previous_len = header->len;
	header->len += otherLen;

	if (header->capacity < header->len) {
        si_stringMakeSpaceFor(str, otherLen);
		curStr = *str;
		header = SI_STRING_HEADER(curStr);
	}

	memcpy(curStr + previous_len, other, otherLen);
	curStr[header->len] = '\0';
}
F_TRAITS(inline)
void si_stringPush(siString* str, char other) {
	si_stringAppendLen(str, &other, 1);
}
F_TRAITS(inline)
void si_stringPop(siString str) {
    SI_ASSERT_NOT_NULL(str);
    str[si_stringLen(str)] = '\0';
	SI_STRING_HEADER(str)->len -= 1;
}

F_TRAITS(inline)
void si_stringInsert(siString* str, cstring cstr, usize index) {
	si_stringInsertEx(str, cstr, si_cstrLen(cstr), index, false);
}
F_TRAITS(inline)
void si_stringInsertLen(siString* str, cstring cstr, usize cstrLen, usize index) {
	si_stringInsertEx(str, cstr, cstrLen, index, false);
}
void si_stringInsertEx(siString* str, cstring cstr, usize cstrLen, usize index, b32 eraseIndex) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize previous_len = header->len;
	usize before_index_len = previous_len - index;

	SI_ASSERT_FMT(index < previous_len, "Index is higher than the length of the string (strLen - '%zd', index - '%zd')", previous_len, index);
	header->len += cstrLen - eraseIndex;

	if (header->capacity < header->len) {
		si_stringMakeSpaceFor(str, header->len - header->capacity);
		header = SI_STRING_HEADER(*str);
	}
	siString curStr = *str;

	char* ptr = (char*)memcpy(curStr + header->len - before_index_len, curStr + index, before_index_len);
	memcpy(curStr + index, cstr, cstrLen);
	ptr[before_index_len] = '\0';
}
void si_stringErase(siString str, usize index, usize eraseLen) {
	SI_ASSERT_NOT_NULL(str);

	usize strLen = si_stringLen(str);
	SI_ASSERT_FMT(index < strLen, "Index is higher than the length of the string. (strLen - '%zd', index - '%zd')", strLen, index);

	usize after_index_len = index + eraseLen;

	char* ptr = (char*)memcpy(
        str + index,
        str + after_index_len,
        strLen - after_index_len
    );
	ptr[strLen - after_index_len] = '\0';

	SI_STRING_HEADER(str)->len -= eraseLen;
}
void si_stringRemoveCstr(siString str, cstring cstr) {
	SI_ASSERT_NOT_NULL(str);

	siStringHeader* header = SI_STRING_HEADER(str);
	usize cstrLen = si_cstrLen(cstr);
	while (true) {
		isize index = si_stringRFindEx(str, header->len - 1, 0, cstr);
		if (index == -1) {
			break;
		}

		usize after_index_len = index + cstrLen;

		char* ptr = (char*)memcpy(
            str + index,
            str + after_index_len,
            header->len - after_index_len
        );
		ptr[header->len - after_index_len] = '\0';

		header->len -= cstrLen;
	}
}
void si_string_swap(siString* str, cstring cstr1, cstring cstr2);

void si_stringStrip(siString str) {
	SI_ASSERT_NOT_NULL(str);

	usize start = 0, end = 0;
    isize i;

	for_range (i, 0, si_stringLen(str)) {
		if (!si_charIsSpace(str[i])) {
			break;
		}
		start += 1;
	}

	for (i = si_stringLen(str) - 1; i != -1; i--) {
		if (!si_charIsSpace(str[i])) {
			break;
		}
		end += 1;
	}
	char* stripped_str = str + start * si_stringTypeSize(str);
	stripped_str[si_stringLen(str) - start - end] = '\0';

	si_stringSet(&str, stripped_str); // TODO(EimaMei): Update the function to not use set.
}
F_TRAITS(inline)
void si_stringReverse(siString str) {
	si_stringReverseLen(str, si_stringLen(str));
}
void si_stringReverseLen(siString str, usize len) {
    SI_ASSERT_NOT_NULL(str);

    char* a = str;
	char* b = str + len - 1;
	len /= 2;

	while (len--) {
		si_swap(*a, *b);
		a += 1;
        b -= 1;
	}
}

siArray(siString) si_stringSplit(siAllocator* alloc, siString str, cstring delimiter) {
	SI_ASSERT_NOT_NULL(str);

	usize strLen = si_stringLen(str);
	usize separatorLen = si_cstrLen(delimiter);

	usize count = 0;
	usize beginPos = 0;
	usize posBuffer[strLen]; /* TODO(EimaMei): Possibly use regular stack alloc? */

	while (true) {
		isize pos = si_stringFindEx(str, beginPos, strLen, delimiter, separatorLen);
		if (pos == SI_ERROR) {
			posBuffer[count] = posBuffer[count - 1];
			count++;
			break;
		}
		posBuffer[count] = pos - beginPos;
		count++;

		beginPos = pos + separatorLen;
	}
	siArray(siString) res = (siString*)si_arrayMakeReserve(alloc, sizeof(*res), count);
	SI_ARRAY_HEADER(res)->len = count;

	for_range (i, 0, count) {
		res[i] = si_stringMakeLen(alloc, str, posBuffer[i]);
		str += posBuffer[i] + separatorLen;
	}

	return res;
}
F_TRAITS(inline)
void si_stringClear(siString str) {
    SI_ASSERT_NOT_NULL(str);
	*str = '\0';
	SI_STRING_HEADER(str)->len = 0;
}

F_TRAITS(inline)
void si_stringFree(siString str) {
	SI_ASSERT_NOT_NULL(str);
    si_free(SI_STRING_HEADER(str)->allocator, SI_STRING_HEADER(str));
}
void si_stringMakeSpaceFor(siString* str, usize add_len) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize oldSize = sizeof(siStringHeader) + header->len + 1;
	usize newSize = oldSize + add_len;

	siStringHeader* new_header = (siStringHeader*)si_realloc(
        header->allocator,
        header,
        oldSize,
        newSize
    );

    *str = (char*)new_header + sizeof(siStringHeader);
	new_header->capacity += add_len;
}
void si_stringShrinkToFit(siString* str) {
	SI_ASSERT_NOT_NULL(str);
    siString curStr = *str;
    siStringHeader* header = SI_STRING_HEADER(curStr);

	header = (siStringHeader*)si_realloc(
        header->allocator,
        header,
        sizeof(siStringHeader*) + header->capacity + 1,
        sizeof(siStringHeader*) + header->len + 1
    );
	header->capacity = header->len;
}

inline char* si_cstrMake(siAllocator* alloc, cstring cstr) {
	return si_cstrMakeLen(alloc, cstr, si_cstrLen(cstr));
}
inline char* si_cstrMakeLen(siAllocator* alloc, cstring cstr, usize len) {
	char* str = (char*)si_malloc(alloc, len + 1);
	memcpy(str, cstr, len + 1);

	return str;
}
char* si_cstrMakeFmt(siAllocator* alloc, cstring format, ...) {
    SI_ASSERT_NOT_NULL(format);

    va_list args, copy;
    va_start(args, format);

    va_copy(copy, args);
    usize  size = vsnprintf(nil, 0, format, copy);
    va_end(copy);

    char* buffer = (char*)si_malloc(alloc, size + 1);

    vsnprintf(buffer, size + 1, format, args);
    va_end(args);

    return buffer;
}

F_TRAITS(inline)
usize si_cstrLen(cstring str) {
	return strlen(str); /* NOTE(EimaMei): Generally for performance, OG strlen is best for both portability and performance. */
}

F_TRAITS(inline)
void si_cstrUpper(char* str) {
	SI_ASSERT_NOT_NULL(str);

	char x = '\0';
	while ((x = *str)) {
		*str = si_charUpper(x);
		str++;
	}
}
F_TRAITS(inline)
void si_cstrLower(char* str) {
	SI_ASSERT_NOT_NULL(str);

	char x = '\0';
	while ((x = *str)) {
		*str = si_charLower(x);
		str += 1;
	}
}
void si_cstrTitle(char* str) {
	SI_ASSERT_NOT_NULL(str);

	b32 change = true;
	char x = '\0';
	while ((x = *str)) {
		if (si_charIsSpace(x)) {
			change = true;
		}
		else if (change) {
			*str = si_charUpper(x);
			change = false;
		}
		str += 1;
	}
}
F_TRAITS(inline)
void si_cstrCapitalize(char* str) {
	SI_ASSERT_NOT_NULL(str);

	si_cstrLower(str);
	*str = si_charUpper(*str);
}

F_TRAITS(inline)
b32 si_cstrEqual(cstring str1, cstring str2) {
	return si_cstrEqualLen(str1, si_cstrLen(str1), str2, si_cstrLen(str2));
}
b32 si_cstrEqualLen(cstring str1, usize str1Len, cstring str2, usize str2Len) {
	if (str1 == str2) {
		return true;
	}
	if (str1Len != str2Len) {
		return false;
	}

	usize i;
	for (i = 0; i < str1Len; i++) {
		if (str1[i] != str2[i])
			return false;
	}

	return true;
}
char* si_u64ToCstr(siAllocator* alloc, u64 num) {
    SI_ASSERT_NOT_NULL(alloc);
    char* res = si_mallocArray(alloc, char, num);

    char* endPtr = res + SI_NUM_MAX_DIGITS - 1;
    *endPtr = '\0';

    do {
        *(--endPtr) = (num % 10) + '0'; /* NOTE(EimaMei): We build the string from behind (not the start) so that we
                                                           wouldn't have to reverse the string after we build the string. */
        num /= 10;
    } while (num > 0);

    return res + (endPtr - res);
}


u64 si_cstrToU64(cstring str) {
	SI_ASSERT_NOT_NULL(str);

	u64 result = 0;
	char cur;
	while ((cur = *str++)) {
		if (si_between(cur, '0', '9')) {
			result = (result * 10) + (cur - '0');
		}
		else {
			SI_ASSERT_MSG(si_between(cur, '0', '9'), "Attempted to use `si_cstrToU64` with a string that contains non numbers.");
		}
	}

	return result;
}
u64 si_cstrToU64Len(cstring str, usize len) {
	SI_ASSERT_NOT_NULL(str);

	u64 result = 0;
	while (len != 0) {
		if (si_between(*str, '0', '9')) {
			result = (result * 10) + (*str - '0');
		}
		else {
			SI_ASSERT_MSG(si_between(*str, '0', '9'), "Attempted to use `si_cstrToU64` with a string that contains non numbers.");
		}
        len -= 1;
        str += 1;
	}

	return result;
}
char* si_i64ToCstr(siAllocator* alloc, i64 num) {
    SI_ASSERT_NOT_NULL(alloc);
    char* res = (char*)si_malloc(alloc, SI_NUM_MAX_DIGITS + 1);

    char* endPtr = res + SI_NUM_MAX_DIGITS - 1;
    *endPtr = '\0';

    isize isNegative = si_numIsNeg(num);
    usize unsignedNum = (num ^ -isNegative) + isNegative;

    do {
        *(--endPtr) = unsignedNum % 10 + '0';
        unsignedNum /= 10;
    } while (unsignedNum > 0);

    SI_STOPIF(isNegative, *(--endPtr) = '-');
    return res + (endPtr - res);
}
i64 si_cstrToI64(cstring str) {
	SI_ASSERT_NOT_NULL(str);

	i64 result = 0;
	char cur;
	b32 negative = false;

	if ((*str) == '-') {
		negative = true;
		str++;
	}

	while ((cur = *str++)) {
		if (cur >= '0' && cur <= '9') {
			result = (result * 10) + (cur - '0');
		}
		else {
			SI_ASSERT_MSG(!(cur >= '0' && cur <= '9'), "Attempted to use `si_cstrToI64` with a string that contains non numbers.");
		}
	}

	if (negative) {
		result = -result;
	}

	return result;
}
u64 si_cstrToI64Len(cstring str, usize len) {
	SI_ASSERT_NOT_NULL(str);

    i64 result = 0;
	b32 negative = false;

	if ((*str) == '-') {
		negative = true;
		str += 1;
        len -= 1;
	}

	while (len != 0) {
		if (*str >= '0' && *str <= '9') {
			result = (result * 10) + (*str - '0');
		}
		else {
			SI_ASSERT_MSG(!(*str >= '0' && *str <= '9'), "Attempted to use `si_cstrToI64` with a string that contains non numbers.");
		}
        len -= 1;
        str += 1;
	}

	if (negative) {
		result = -result;
	}

	return result;
}

#endif

#if defined(SI_IMPLEMENTATION_CHAR) && !defined(SI_NO_CHAR)

F_TRAITS(inline)
char si_charLower(char c) {
	if (c >= 'A' && c <= 'Z') {
		return c + 32;
	}
	return c;
}

F_TRAITS(inline)
char si_charUpper(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 32;
    }
	return c;
}

F_TRAITS(inline)
b32 si_charIsSpace(char c) {
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}
F_TRAITS(inline)
b32 si_charIsDigit(char c) {
	return (c >= '0' && c <= '9');
}
F_TRAITS(inline)
b32 si_charIsHexDigit(char c) {
	return (si_charIsDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}
F_TRAITS(inline)
b32  si_charIsAlpha(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
F_TRAITS(inline)
b32 si_charIsAlphanumeric(char c) {
	return si_charIsAlpha(c) || si_charIsDigit(c);
}
F_TRAITS(inline)
i32 si_charDigitToInt(char c) {
	SI_ASSERT_MSG(si_charIsDigit(c), "Character is not a digit.");
    return (c - '0'); /* NOTE(EimaMei): If the user inputs a non-number character, then it's their problem (like read the damn function name ffs). */
}
i32 si_charHexDigitToInt(char c) {
	if (si_charIsDigit(c)) {
		return si_charDigitToInt(c);
	}
	else if (c <= 'a' && c >= 'f') {
		return c - 'a' + 10;
	}
	else if (c <= 'A' && c >= 'F') {
		return c - 'A' + 10;
	}

	return -1;
}
#endif

#if defined(SI_IMPLEMENTATION_HASHTABLE) && !defined(SI_NO_HASHTABLE)

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

F_TRAITS(intern)
u64 si_hashKey(cstring key) {
    uint64_t hash = FNV_OFFSET;
    cstring p;
    for (p = key; *p; p++) {
        hash ^= (uint64_t)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

#undef FNV_OFFSET
#undef FNV_PRIME

F_TRAITS(inline)
siHashTable si_hashtableMake(siAllocator* alloc, cstring* keyArray, rawptr dataArray,
        usize sizeofElement, usize len) {
    siHashTable table;
    table.alloc = alloc;
    table.entries = si_arrayMakeReserve(alloc, sizeof(siHashEntry), len);

    siArrayHeader* arrayHeader = SI_ARRAY_HEADER(table.entries);
    arrayHeader->len = len;

    siAny any = (siAny){.ptr = dataArray, .typeSize = sizeofElement};
    for_range (i, 0, len) {
        si_hashtableSetEx(&table, keyArray[i], any);
        any.ptr = (siByte*)any.ptr + sizeofElement;
    }

    return table;
}
F_TRAITS(inline)
siHashTable si_hashtableMakeReserve(siAllocator* alloc, usize capacity) {
    siHashTable table;
    table.alloc = alloc;
    table.entries = si_arrayMakeReserve(alloc, sizeof(siHashEntry), capacity);
    memset(table.entries, 0, capacity * sizeof(siHashEntry));

    return table;
}
rawptr si_hashtableGet(siHashTable ht, cstring key) {
    SI_STOPIF(si_arrayLen(ht.entries) == 0, return nil);

    u64 hash = si_hashKey(key);
    usize index = (usize)(hash & (si_arrayCapacity(ht.entries) - 1));

    siHashEntry* entry = ht.entries + index;
    siHashEntry* old_entry = ht.entries + index;
    siHashEntry* end = ht.entries + si_arrayCapacity(ht.entries);
    do {
        SI_STOPIF(entry->key == nil, goto increment_entry);

        if (*key == *entry->key && strcmp(key + 1, entry->key + 1) == 0) {
            return entry->value;
        }

increment_entry:
        entry += 1;
        if (entry == end) {
            entry = ht.entries;
        }
    } while (entry != old_entry);

    return nil;
}
siHashEntry* si_hashtableSetEx(siHashTable* ht, cstring key, siAny value) {
    SI_ASSERT_NOT_NULL(ht);
    siArrayHeader* arrHeader = SI_ARRAY_HEADER(ht->entries);

    SI_ASSERT_NOT_NULL(key);
    SI_ASSERT_MSG(arrHeader->len < arrHeader->capacity, "Not enough capacity in the hash table.");

    u64 hash = si_hashKey(key);
    usize index = hash & (arrHeader->capacity - 1);

    siHashEntry* entry = ht->entries + index;
    siHashEntry* end = ht->entries + arrHeader->capacity;

    while (entry->key != nil) {
        if (*key == *entry->key && strcmp(key + 1, entry->key + 1) == 0) {
            return ht->entries + index;
        }

        entry += 1;
        SI_STOPIF(entry == end, entry = ht->entries);
    }
    entry->key = si_cstrMake(ht->alloc, key);
    entry->value = si_malloc(ht->alloc, value.typeSize);
    memcpy(entry->value, value.ptr, value.typeSize);

    arrHeader->len += 1;
    return ht->entries + index;
}

/* Frees the hash table. */
void si_hashtableFree(siHashTable ht)  {
    usize capacity = si_arrayCapacity(ht.entries);
    usize i;
    for (i = 0; i < capacity; i++) {
        if (ht.entries[i].key != nil) {
            si_free(ht.alloc, (rawptr)ht.entries[i].key);
            si_free(ht.alloc, ht.entries[i].value);
        }
    }
    si_free(ht.alloc, ht.entries);
}
#endif


#if defined(SI_IMPLEMENTATION_FILE) && !defined(SI_NO_FILE)

F_TRAITS(inline)
b32 si_pathExists(cstring path) {
    SI_ASSERT_NOT_NULL(path);
	#if defined(SI_SYSTEM_WINDOWS)
		DWORD file_attrib = GetFileAttributes(path);
		return file_attrib != INVALID_FILE_ATTRIBUTES;
	#else
		struct stat tmp;
		return (stat(path, &tmp) == SI_OKAY);
	#endif
}
isize si_pathCopy(cstring existingPath, cstring newPath) {
	#if defined(SI_SYSTEM_WINDOWS)
		return (CopyFile(existingPath, newPath, true) != 0);
	#else
		i32 existingFile = open(existingPath, O_RDONLY, 0);
		i32 newFile = open(newPath, O_WRONLY | O_CREAT, 0666);

		struct stat stat_existing;
		fstat(existingFile, &stat_existing);

		#if defined(SI_SYSTEM_UNIX)
			isize size = sendfile(newFile, existingFile, 0, stat_existing.st_size);
		#else
			isize size = sendfile(newFile, existingFile, 0, &stat_existing.st_size, NULL, 0);
		#endif

		close(newFile);
		close(existingFile);

		return size;
	#endif
}
F_TRAITS(inline)
b32 si_pathMove(cstring existingPath, cstring newPath) {
	#if defined(SI_SYSTEM_WINDOWS)
		return (MoveFile(existingPath, newPath) != 0);
	#else
		if (link(existingPath, newPath) == 0) {
			return (unlink(existingPath) == 0);
		}
		return false;
	#endif
}
F_TRAITS(inline)
b32 si_pathRemove(cstring path) {
	#if defined(SI_SYSTEM_WINDOWS)
		isize pos = si_stringRFindEx((siString)path, si_cstrLen(path), 0, ".");
		if (pos != SI_ERROR) {
			return (DeleteFileA(path) != 0);
		}
		else {
			return (RemoveDirectoryA(path) != 0);
		}
	#else
		return remove(path) == 0;
	#endif
}

F_TRAITS(inline)
cstring si_pathBaseName(cstring path) {
	isize pos = si_stringRFindEx((siString)path, si_cstrLen(path) - 1, 0, &SI_PATH_SEPARATOR);

	return (pos != SI_ERROR ? (path + pos + 1) : path);
}
F_TRAITS(inline)
cstring si_pathExtension(cstring path) {
	isize pos = si_stringRFindEx((siString)path, si_cstrLen(path) - 1, 0, ".");

	return (pos != SI_ERROR ? (path + pos + 1) : path);
}
F_TRAITS(inline)
siString si_pathGetFullName(siAllocator* alloc, cstring path) {
    SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		char buffer[MAX_PATH];
		GetFullPathNameA(path, MAX_PATH, buffer, nil);

		return si_stringMake(alloc, buffer);
	#else
		char actualPath[4096];
		realpath(path, actualPath);

		return si_stringMake(alloc, actualPath);
	#endif
}

F_TRAITS(inline)
b32 si_pathIsAbsolute(cstring path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		return (
			si_cstrLen(path) > 2 &&
			si_charIsAlpha(path[0]) &&
			(path[1] == ':' && path[2] == SI_PATH_SEPARATOR)
		);
	#else
		return (1 < si_cstrLen(path) && path[0] == SI_PATH_SEPARATOR);
	#endif
}
F_TRAITS(inline)
b32 si_pathIsRelative(cstring path) {
	return !si_pathIsAbsolute(path);
}


u64 si_pathLastWriteTime(cstring filename) {
#if defined(SI_PLATFORM_API_WIN32)
    FILETIME lastWriteTime = {0};
    WIN32_FILE_ATTRIBUTE_DATA data = {0};

#if 0 // TODO(EimaMei): create utf-8 to ucs2
    wchar_t *w_text = gb__alloc_utf8_to_ucs2(a, filepath, NULL);
    if (w_text == NULL) {
        return 0;
    }
#endif

    if (GetFileAttributesExW(w_text, GetFileExInfoStandard, &data)) {
        lastWriteTime = data.ftLastWriteTime;
    }

    ULARGE_INTEGER res;
    res.LowPart = lastWriteTime.dwLowDateTime;
    res.HighPart = lastWriteTime.dwHighDateTime;
    return (u64)res.QuadPart;
#else
    u64 result = 0;
    struct stat fileStat;

    if (stat(filename, &fileStat) == 0) {
        result = fileStat.st_mtime;
    }

    return result;
#endif
}



F_TRAITS(inline)
siFile si_fileCreate(cstring path) {
	return si_fileOpenMode(path, "w+");
}
F_TRAITS(inline)
siFile si_fileOpen(cstring path) {
	return si_fileOpenMode(path, "r+");
}
F_TRAITS(inline)
siFile si_fileOpenBin(cstring path) {
	return si_fileOpenMode(path, "rb+");
}
siFile si_fileOpenMode(cstring path, cstring mode) {
	SI_ASSERT_NOT_NULL(path);
    SI_ASSERT_NOT_NULL(mode);

	FILE* f = fopen(path, mode);

	if (f == nil) {
        cstring message;
		switch (errno) {
            case EACCES:       message = "Failed to execute 'si_fileOpenMode': Permission denied (EACCES).";
            case EINTR:        message = "Failed to execute 'si_fileOpenMode': A signal was caught during fopen() (EINTR).";
            case EISDIR:       message = "Failed to execute 'si_fileOpenMode': 'path' is a directory (EISDIR).";
            case ELOOP:        message = "Failed to execute 'si_fileOpenMode': Too many symbolic links were encountered in resolving 'path' (ELOOP).";
            case EMFILE:       message = "Failed to execute 'si_fileOpenMode': The per-process limit on the number of open file descriptors has been reached. (EMFILE).";
            case ENAMETOOLONG: message = "Failed to execute 'si_fileOpenMode': The length of 'path' exceeds PATH_MAX or a pathname component is longer than NAME_MAX (ENAMETOOLONG).";
            case ENFILE:       message = "Failed to execute 'si_fileOpenMode': The system-wide limit on the total number of open files has been reached (ENFILE).";
            case ENOENT:       message = "Failed to execute 'si_fileOpenMode': File does not exist, or path is an empty string (ENOENT).";
            case ENOSPC:       message = "Failed to execute 'si_fileOpenMode': Not enough space left in the directory to contain the new file (ENOSPC).";
            case ENOMEM:       message = "Failed to execute 'si_fileOpenMode': Insufficient memory to complete the operation (ENOMEM).";
            case ENOTDIR:      message = "Failed to execute 'si_fileOpenMode': A component of the path prefix is not a directory (ENOTDIR).";
            case ENXIO:        message = "Failed to execute 'si_fileOpenMode': The named file is a character special or block special file, and the device associated with this file does not exist (ENXIO).";
            case EOVERFLOW:    message = "Failed to execute 'si_fileOpenMode': The named file is a regular file and the size of the file cannot be represented correctly in an object of type off_t (EOVERFLOW).";
            case EROFS:        message = "Failed to execute 'si_fileOpenMode': The named file resides on a read-only file system and mode requires write access (EROFS).";
            case EINVAL:       message = "Failed to execute 'si_fileOpenMode': The value of 'mode ' is not valid (EINVAL).";
            case ETXTBSY:      message = "Failed to execute 'si_fileOpenMode': The file is a pure procedure (shared text) file that is being executed and mode requires write access (ETXTBSY).";
            default:           message = "Failed to execute 'si_fileOpenMode': Unknown reason.";
		}
        SI_PANIC_MSG(message);
        return (siFile){0};
	}

	siFile result = {
        f,
		0,
        path,
        si_pathLastWriteTime(path)
	};
	si_fileSizeUpdate(&result);

	return result;
}

usize si_fileSize(siFile file) {
	fseek(file.ptr, 0, SEEK_END);
	usize size = ftell(file.ptr);
	rewind(file.ptr);

	return size;
}
F_TRAITS(inline)
void si_fileSizeUpdate(siFile* file) {
	SI_ASSERT_NOT_NULL(file);

    fseek(file->ptr, 0, SEEK_END);
	file->size = ftell(file->ptr);
	rewind(file->ptr);
}

F_TRAITS(inline)
rawptr si_fileRead(siAllocator* alloc, siFile file) {
    return si_fileReadAtEx(alloc, file, 0, file.size, 0);
}
F_TRAITS(inline)
rawptr si_fileReadEx(siAllocator* alloc, siFile file, usize additionalAllocSpace) {
	return si_fileReadAtEx(alloc, file, 0, file.size, additionalAllocSpace);
}
F_TRAITS(inline)
rawptr si_fileReadAt(siAllocator* alloc, siFile file, usize offset, usize len) {
    return si_fileReadAtEx(alloc, file, offset, len, 0);
}
rawptr si_fileReadLine(siAllocator* alloc, siFile file) {
    SI_ASSERT_NOT_NULL(alloc);
    return fgets((char*)alloc->ptr, si_allocatorAvailable(alloc), file.ptr);
}
F_TRAITS(inline)
rawptr si_fileReadAtEx(siAllocator* alloc, siFile file, usize offset, usize len,
        usize additionalAllocSpace) {
	SI_ASSERT_NOT_NULL(file.ptr);

    rawptr buffer = si_malloc(alloc, len + additionalAllocSpace);

    fseek(file.ptr, offset, SEEK_SET);
    fread(buffer, len, sizeof(siByte), file.ptr);
    rewind(file.ptr);

    return buffer;
}
siArray(siString) si_fileReadlines(siAllocator* alloc, siFile file) {
    siAllocator* tmp = si_allocatorMake(2 * file.size * (sizeof(siString) + sizeof(siStringHeader)));

	siString buffer = (siString)si_fileRead(tmp, file);
	siArray(siString) res = si_stringSplit(alloc, buffer, "\n");

    si_allocatorFree(tmp);
	return res;
}

F_TRAITS(inline)
usize si_fileWrite(siFile* file, rawptr content) {
	return si_fileWriteAtLen(file, content, si_cstrLen(content), 0);
}
F_TRAITS(inline)
usize si_fileWriteLen(siFile* file, rawptr content, usize len) {
	return si_fileWriteAtLen(file, content, len, 0);
}
F_TRAITS(inline)
usize si_fileWriteAt(siFile* file, rawptr content, usize offset) {
	return si_fileWriteAtLen(file, content, si_cstrLen(content), offset);
}
usize si_fileWriteAtLen(siFile* file, rawptr content, usize contentLen, usize offset) {
	SI_ASSERT_NOT_NULL(file->ptr);
	SI_ASSERT_NOT_NULL(content);

    fseek(file->ptr, offset, SEEK_SET);
    usize len = fwrite(content, sizeof(siByte), contentLen, file->ptr);
    rewind(file->ptr);

	si_fileSizeUpdate(file);
	return len;
}
usize si_fileWriteAtLine(siFile* file, cstring content, usize index) {
	SI_ASSERT_NOT_NULL(file->ptr);

    siAllocator* tmp = si_allocatorMake(2 * file->size * (sizeof(siStringHeader) + sizeof(siString)));
	siArray(siString) buffer = si_fileReadlines(tmp, *file);
#if defined(SI_ALLOCATOR_UNDEFINE)
    siString previousLine = buffer[index];
#endif
    SI_ASSERT_MSG(index < si_arrayLen(buffer), "Index is either not 0 or higher than the amount of lines in the file.");

	buffer[index] = (siString)content;
	siString newFileContent = si_arrayToSistring(buffer, "\n");

	usize len = si_fileWriteLen(file, newFileContent, si_stringLen(newFileContent));
#if defined(SI_ALLOCATOR_UNDEFINE)
	usize i;
	for (i = 0; i < si_arrayLen(buffer); i++) {
		if (i == index) {
			continue;
		}
		si_stringFree(buffer[i]);
	}
	si_stringFree(previousLine);
	si_arrayFree(buffer);
	si_stringFree(newFileContent);
#endif

    return len;
}

F_TRAITS(inline)
b32 si_fileSeek(siFile file, usize offset) {
	SI_ASSERT_NOT_NULL(file.ptr);
	return (fseek(file.ptr, offset, SEEK_SET) == 0);
}
F_TRAITS(inline)
b32 si_fileSeekToEnd(siFile file) {
	SI_ASSERT_NOT_NULL(file.ptr);
	return (fseek(file.ptr, 0, SEEK_END) == 0);
}

void si_fileClose(siFile file) {
	SI_STOPIF(file.ptr != nil, fclose(file.ptr));
}

#endif

#if defined(SI_THREAD_IMPLEMENTATION) && !defined(SI_NO_THREAD)

inline static rawptr si_impl_thread_proc(rawptr arg) {
	siThread* t = (siThread*)arg;
	t->returnValue = t->func.ptr(t->arg);
	t->isRunning = false;

	return nil;
}

F_TRAITS(inline)
siThread si_threadCreateEx(siFunction function, rawptr arg) {
	siThread t;
	t.isRunning = false;
	t.func = function;
	t.arg = arg;

	return t;
}


F_TRAITS(inline)
void si_threadStart(siThread* t) {
	si_threadStartStack(t, 0);
}

void si_threadStartStack(siThread* t, usize stackSize) {
	#if defined(SI_SYSTEM_WINDOWS)
		t->id = CreateThread(nil, stackSize, (LPTHREAD_START_ROUTINE)si_impl_thread_proc, t, 0, nil);
		t->isRunning = true;
		SI_ASSERT_MSG(t->id != nil, "Something went wrong with creating a new thread.");
	#else
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_attr_setstacksize(&attr, stackSize);
		t->isRunning = true;

		usize error_code = pthread_create(&t->id, &attr, si_impl_thread_proc, t);

		cstring error_msg = nil;
		switch (error_code) {
			case SI_OKAY: break;
			case EAGAIN:  error_msg = "The system lacked the necessary resources to create another thread, or the system-imposed"
                          "limit on the total number of threads in a process PTHREAD_THREADS_MAX would be exceeded."; break;
			case EINVAL:  error_msg = "The value specified by attr is invalid at 'pthread_create'."; break;
			case EPERM:   error_msg = "The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy."; break;
			default:      error_msg = "Unknown error code (%li)."; break;
		}
		SI_ASSERT_FMT(error_code == SI_OKAY, error_msg, error_code);
		pthread_attr_destroy(&attr);
	#endif
}

void si_threadJoin(siThread* t) {
	t->isRunning = true;

	#if defined(SI_SYSTEM_WINDOWS)
		WaitForSingleObject(t->id, INFINITE);
		CloseHandle(t->id);
		t->id = INVALID_HANDLE_VALUE;
	#else
		usize error_code = pthread_join(t->id, nil);

		cstring error_msg = nil;
		switch (error_code) {
			case SI_OKAY: break;
			case EDEADLK: error_msg = "A deadlock was detected."; break;
			default:      error_msg = "Unknown error code (%li)."; break;
		}
		SI_ASSERT_FMT(error_code == SI_OKAY, error_msg, error_code);
	#endif

	t->isRunning = false;
}
void si_threadCancel(siThread* t) {
	#if defined(SI_SYSTEM_WINDOWS)
		print("si_threadCancel: This feature on Windows is not supported as of now.");
		SI_UNUSED(t);
	#else

		usize error_code = pthread_cancel(t->id);
		t->isRunning = false;

		cstring error_msg = nil;
		switch (error_code) {
			case SI_OKAY:   break;
			case ESRCH:     error_msg = "No thread could be found corresponding to that specified by the given thread ID."; break;
			default:        error_msg = "Unknown error code (%li)."; break;
		}
		SI_ASSERT_FMT(error_code == SI_OKAY, error_msg, error_code);

	#endif
}
F_TRAITS(inline)
void si_threadDestroy(siThread* t) {
	si_threadJoin(t);
}

void si_threadPrioritySet(siThread t, i32 priority) {
	#if defined(SI_SYSTEM_WINDOWS)
		isize res = SetThreadPriority(t.id, priority);
		SI_ASSERT_MSG(res != 0, "Something went wrong setting the thread priority.");
	#elif defined(SI_SYSTEM_UNIX)
		usize error_code = pthread_setschedprio(t.id, priority);

		cstring error_msg = nil;
		switch (error_code) {
			case SI_OKAY:  break;
			case EINVAL:   error_msg = "The value of 'priority' is invalid for the scheduling policy of the specified thread."; break;
			case ENOTSUP:  error_msg = "An attempt was made to set the priority to an unsupported value."; break;
			case EPERM:    error_msg = "The caller does not have the appropriate permission to set the scheduling policy"
                           "of the specified thread OR the implementation does not allow the application to modify the"
                            "priority to the value specified."; break;
			case ESRCH:    error_msg = "The value specified by thread does not refer to an existing thread."; break;
			default:       error_msg = "Unknown error code (%li)."; break;
		}
		SI_ASSERT_FMT(error_code == SI_OKAY, error_msg, error_code);
	#else
		SI_PANIC_MSG("si_threadPrioritySet: Not supported on MacOS.");
		SI_UNUSED(t);
		SI_UNUSED(priority);
	#endif
}
#endif

#if defined(SI_IMPLEMENTATION_BIT) && !defined(SI_NO_BIT)

F_TRAITS(inline)
usize si_numCountBitsU8(u8 num) {
    return (num * (u64)01001001001L & (u64)042104210421) % 017;
}

F_TRAITS(inline)
usize si_numCountBitsU32(u32 num) {
     num -= ((num >> 1) & 0x55555555);
     num = (num & 0x33333333) + ((num >> 2) & 0x33333333);
     num = (num + (num >> 4)) & 0x0F0F0F0F;
     return (num * 0x01010101) >> 24;
}

F_TRAITS(inline)
usize si_numCountBitsU64(u64 num) {
    num = num - ((num >> 1) & 0x5555555555555555);
    num = (num & 0x3333333333333333) + ((num >> 2) & 0x3333333333333333);
    num = (num + (num >> 4)) & 0xF0F0F0F0F0F0F0F;
    return (num * 0x101010101010101) >> 56;
}

F_TRAITS(inline)
usize si_numLeadingBitEx(u64 num, usize total_bits, siBitType bit) {
    u64 count = 0;

    usize i;
    for (i = total_bits - 1; i < USIZE_MAX; i -= 1) {
        if (SI_NUM_BIT_GET(num, i) == bit) {
            count += 1;
        }
        else {
            return count;
        }
    }

    return count;
}
F_TRAITS(inline)
usize si_numTrailingBitEx(u64 num, usize total_bits, siBitType bit) {
    u64 count = 0;

    for_range (i, 0, total_bits) {
        if (SI_NUM_BIT_GET(num, i) == bit) {
            count += 1;
        }
        else {
            return count;
        }
    }

    return count;
}
F_TRAITS(inline)
u64 si_numRotateLeftEx(u64 num, usize total_bits, usize bits) {
    return (num << bits) | (num >> (total_bits - bits));
}
F_TRAITS(inline)
u64 siNumRotateRightEx(u64 num, usize total_bits, usize bits) {
    return (num >> bits) | (num << (total_bits - bits));
}

F_TRAITS(inline)
u64 siNumReverseBitsEx(u64 num, usize total_bits) {
    u64 res = 0;

    for_range(i, 0, total_bits) {
        res <<= 1;
        res |= (num & 1);
        num >>= 1;
    }

    return res;
}

inline siArray(u8) si_numToBytesEx(siAllocator* alloc, u64 num, usize num_sizeof) {
    siArray(u8) res = (u8*)si_arrayMakeReserve(alloc, sizeof(*res), num_sizeof);
    SI_ARRAY_HEADER(res)->len = num_sizeof;

    for_range (i, 0, num_sizeof) {
        res[i] = (num >> ((num_sizeof - 1 - i) * 8)) & 255;
    }

    return res;
}
F_TRAITS(inline)
u64 si_bytesToNumSiArr(siArray(u8) bytes) {
    SI_ASSERT_NOT_NULL(bytes);
    usize res = 0;
    usize shift = (si_arrayLen(bytes) - 1) * 8;

    for_each (byte, bytes) {
        res |= si_cast(usize, *byte) << shift;
        shift -= 8;
    }

    return res;
}

F_TRAITS(inline)
usize si_numLen(u64 num) {
    usize count = 0;

    do {
        count += 1;
        num /= 10;
    } while (num > 0);

    return count;
}

F_TRAITS(inline)
u64 si_numRoundNearestMultiple(u64 num, usize multiple) {
    return ((num + multiple / 2) / multiple) * multiple;
}

#endif

#if defined(SI_IMPLEMENTATION_CPU) && !defined(SI_NO_CPU)

F_TRAITS(inline)
siMhz si_cpuClockSpeed(void) {
	/* TODO(EimaMei): Possibly add a better version for ARM processors. */
    u64 begin = si_RDTSC();
    si_sleep(100);
    u64 end = si_RDTSC();

    return si_numRoundNearestMultiple((end - begin) / 100000, 10);
}

#endif

#if defined(SI_IMPLEMENTATION_BENCHMARK) && !defined(SI_NO_BENCHMARK)

F_TRAITS(inline)
void si_intern_benchmarkThread(u32* arg) {
	si_sleep(*arg);
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

#endif /* SI_IMPLEMENTATION */

/*
------------------------------------------------------------------------------
Copyright (C) 2023 EimaMei

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
