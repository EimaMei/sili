#define SI_IMPLEMENTATION 1

/*
 * MAJOR TODOS(EimaMei):
 * - Add documentation for every function, macro and what not.
 * - Rework the beginning text for more info.
 * - Make sure UNDEFINE macros work (and rename them to 'SI_NO_<feature>)'
 * - Check if all features 100% work on _all_ platforms.
 * - Add 'siliextra' stuff into sili.h.
 * - Remove the usage of 'LL' in favour of just type casting.
 * - Add documentation on how functions/macros are documented.
 * - Rename all functions, macros and variables to use the new naming scheme.
 * - Update the old examples.
 * - Add new examples.
 * - Complete the other TODOs in this file plus others.
 */

/*
sili.h - a cross-platform software toolchain for modern C programming
===========================================================================
	- YOU MUST DEFINE 'SI_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SI_IMPLEMENTATION
		#include "sili.h"

	- All other files should just include the library without the #define macro.


	- If you want to disable certain features, you can do:
		- #define SI_PAIR_UNDEFINE
		- #define SI_OPTIONAL_UNDEFINE
		- #define SI_ARRAY_UNDEFINE
		- #define SI_STRING_UNDEFINE
		- #define SI_CHAR_UNDEFINE
		- #define SI_FILE_UNDEFINE
		- #define SI_THREAD_UNDEFINE
        - #define SI_BIT_UNDEFINE
		- #define SI_CPU_UNDEFINE
        - #define SI_PERFORMANCE_UNDEFINE
	before the SI_IMPLEMENTATION macro, as well as before any other include of
	`sili.h`.

	- NOTE: Disabling certain features may or may not create 'undefined'
	compiler errors at certain spots. Use it at your own risk.

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
 * Syntax sugar to differentiate the keywords from the return type. */
#define F_TRAITS(.../* KEYWORDS */) __VA_ARGS__

/* type - TYPE |name - NAME
 * Defines the enum with the given integer type. */
#define SI_ENUM(type, name) type name; enum
/* varName - NAME |multipleArgs - (TYPE1, TYPE2...)
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

#if defined(SI_MEMORY_LOGGING)
	#define SI_MEMORY_LOGGING_IMPLEMENTATION
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
#define SI_ASSERT_PATH_EXISTS(path) SI_ASSERT_FMT(si_path_exists(path), "Path '%s' doesn't exist.", path)
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

/* headerType - STRUCT HEADER |type - TYPE | count - usize
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
void si_ptrMoveBy(rawptr src, usize srcLen, usize moveBy, siDirection direction);

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

/*
 * alloc - siAllocator* | ptr - rawptr
 * Syntactic sugar, but also to keep compatibility with non-allocator mode.
 */
#define si_free(alloc, ptr) si_freeEx(alloc, ptr, __FILE__, __LINE__)
void si_freeEx(siAllocator* alloc, rawptr ptr, cstring filename, i32 line);

#endif

#if !defined(SI_PAIR_UNDEFINE)
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

/*
 * type1 - TYPE
 * type2 - TYPE
 *
 * A pair structure. */
#define siPair(type1, type2) struct { type1 first; type2 second; }
/*
 * firstValue - EXPRESSION
 * secondValue - EXPRESSION
 *
 * Creates a pair structure variable. */
#define si_pairMake(firstValue, secondValue) {firstValue, secondValue}
/*
 * pair - siPair
 *
 * Makes a copy of the given pair. */
#define si_pairCopy(pair) si_pairMake(pair.first, pair.second)

#endif

#if !defined(SI_OPTIONAL_UNDEFINE) || !defined(SI_OPTIONAL_IMPLEMENTATION)
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

/*
 * IMPORTANT NOTE(EimaMei): Since 'siOptional' uses 'siAny', you should refer to
 * the 'si_anyMake' vs 'si_anyMakeStatic' comment for when to use either one.
*/


/* Structure containing the data and if the optional variable has value. */
typedef struct {
    /* The data inside the optional. */
	siAny value;
    /* If the optional's 'value' exists and has actual value. */
	b32 hasValue;
} siOptionalType;

/* type - TYPE
 * Denotes a 'siOptional' type. */
#define siOptional(type) siOptionalType
/* A fully empty 'siOptional' variable. */
const siOptionalType SI_OPTIONAL_NULL = {{nil, 0}, false};

/* allocator - siAllocator* | ...VALUE - EXPRESSION
 * Creates an optional variable from an allocator. */
#define si_optionalMake(allocator, .../* VALUE */) \
    (siOptionalType){si_anyMake(allocator, __VA_ARGS__), true}
/* ...VALUE - EXPRESSION
 * Creates an optional variable. */
#define si_optionalMakeStatic(... /* VALUE */) (siOptionalType){si_anyMakeStatic(__VA_ARGS__), true}

/*optionalVar - siOptional(TYPE)
 * Gets the optional's data pointer. */
#define si_optionalPtr(optionalVar) (optionalVar.value.ptr)
/* optionalVar - siOptional(TYPE)
 * Gets the optional's type size. */
#define si_optionalTypeSize(optionalVar) (optionalVar.value.typeSize)

/* type - TYPE | optionalVar - siOptional(TYPE)
 * Gets the value of the provided 'siObject' object. Crashes the app if 'hasValue'
 * is false. */
#define si_optionalGet(type, optionalVar) \
    si_anyGet(type, optionalVar.value)
/* type - TYPE | optionalVar -  siOptional(TYPE) | defaultValue - EXPRESSION
 * Gets the value of the provided 'siObject' object. HOWVER if 'hasValue' is set
 * to false, return 'defaultValue' instead. */
#define si_optionalGetOrDefault(type, optionalVar, defaultValue) \
    (optionalVar.hasValue ? si_optionalGet(type, optionalVar) : (type)(defaultValue))

/* optionalVar - siOptional(TYPE)
 * Resets the optional variable. */
#define si_optionalReset(optionalVar) \
    do { \
        siOptionalType siNew = SI_OPTIONAL_NULL; \
        siNew.value.typeSize = optionalVar.value.typeSize; \
        \
        optionalVar = siNew; \
    } while(0)

#endif

#if !defined(SI_ARRAY_UNDEFINE)
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
 * Gets a specific item's pointer and return it as a 'siByte*' for pointer arithmetic.
 */
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
 * array is reallocated by (currentCapacity * 2).
 * */
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

#if !defined(SI_STRING_UNDEFINE)
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

/**
 * @typedef siString
 * @brief siString is a dynamic string type.
 */
typedef char* siString;

/**
 * @typedef siStringHeader
 * @brief siStringHeader is the header structure for siString.
 */
typedef struct siStringHeader {
	usize len;
	usize capacity;
    siAllocator* allocator;
	/* u8 typeSize; */
	/* u16 grow; */
} siStringHeader;


/**
 * @def SI_STRING_HEADER(str)
 * @brief Macro to get the header of a siString.
 * @param str The siString.
 * @return The header pointer of the siString (siStringHeader*).
 */
#define SI_STRING_HEADER(str) ((siStringHeader*)str - 1)
/* TODO(EimaMei): #define SI_STRING_DEFAULT_GROW 64 */



/**
 * @brief Create a new siString from a null-terminated C string.
 *
 * @param str The null-terminated C string.
 *
 * @return The created siString.
 */
siString si_string_make(siAllocator* alloc, cstring str);

/**
 * @brief Create a new siString from a formatted null-terminated C string.
 *
 * @param str The formatted null-terminated C string.
 *
 * @return The created siString.
 */
siString si_string_make_fmt(siAllocator* alloc, cstring str, ...);

/**
 * @brief Create a new siString from a null-terminated C string with a specified length.
 *
 * @param str The null-terminated C string.
 * @param len The length of the string.
 *
 * @return The created siString.
 */
siString si_string_make_len(siAllocator* alloc, cstring str, usize len);

/**
 * @brief Create a new siString with a reserved capacity.
 *
 * @param len The reserved capacity for the siString.
 *
 * @return The created siString.
 */
siString si_string_make_reserve(siAllocator* alloc, usize len);



/**
 * @brief Create a copy of an existing siString.
 *
 * @param from The siString to copy.
 *
 * @return The copied siString.
 */
siString si_string_copy(siAllocator*, siString from);


/**
 * @brief Get the length of a siString.
 * @param str The siString.
 * @return The length of the siString.
 */
usize si_string_len(siString str);

/**
 * @brief Get the capacity of a siString.
 * @param str The siString.
 * @return The capacity of the siString.
 */
usize si_string_capacity(siString str);

/**
 * @brief Get the type size of a siString.
 * @param str The siString.
 * @return The type size of the siString.
 */
usize si_string_type_size(siString str);

/**
 * @brief Get the allocator of the specific siString.
 * @param str the siString.
 * @return The allocator for the siString.
 */
siAllocator* si_string_allocator(siString str);

/**
 * @brief Get the character at a specified index in a siString.
 * @param str The siString.
 * @param index The index of the character.
 * @return The character at the specified index.
 */
b32 si_string_empty(siString str);



/**
 * @brief Get the character at a specified index in a siString.
 *
 * @param str The siString.
 * @param index The index of the character.
 *
 * @return The character at the specified index. If the function failed to get an
 *         error
 */
char si_string_at(siString str, usize index);

/**
 * @brief Get the first character of a siString.
 *
 * @param str The siString.
 *
 * @return The first character of the siString.
 */
char si_string_front(siString str);

/**
 * @brief Get the last character of a siString.
 *
 * @param str The siString.
 *
 * @return The last character of the siString.
 */
char si_string_back(siString str);

/**
 * @brief Get a substring of a siString.

 * @param str The siString from which to extract the substring.
 * @param begin The starting index of where to extract.
 * @param len The length of the substring.
 * @param result (si_string_sub_ex) A pointer to a buffer to store the substring.
 *               The buffer should have a size of at least `len + 1` to accommodate
 *               the substring characters and the NULL terminator. Your app WILL
 *               most likely crash, as sili isn't able to check how large result.
 *
 * @return The substring result.
 *
 * @attention !! USE THE NORMAL MACRO AND NOT THE EXPANDED VERSION FOR THE MAJORITY
 *               OF USE CASES !!
 */
#define si_string_sub(str, begin, len) \
    si_string_sub_ex(str, begin, len, si_salloc(len + 1))
char* si_string_sub_ex(siString str, usize begin, usize len, char* result);


/**
 * @brief Find the first occurrence of a substring in a siString.
 *
 * @param str The siString.
 * @param cstr The substring to find.
 *
 * @return The index of the first occurrence of the substring, or -1 if not found.
 */
isize si_string_find(siString str, cstring cstr);

/**
 * @brief Find the first occurrence of a substring within a specific range of a
 *        siString.
 *
 * @note This is the expanded version of the `si_string_find` function. Should be
 *       used for specifying the range and when the substring length is known.
 *
 * @param str The siString to search within.
 * @param start The index to start the search from.
 * @param end The index to end the search at.
 * @param cstr The substring to find within the siString.
 * @param cstr_len The length of the substring.
 *
 * @return The index of the first occurrence of the substring within the given
 *         range, or -1 if the substring is not found.
 */
isize si_string_find_ex(siString str, usize start, usize end, cstring cstr, usize cstr_len);

/**
 * @brief Find the last occurrence of a substring in a siString.
 *
 * @param str The siString.
 * @param cstr The substring to find.
 *
 * @return The index of the last occurrence of the substring, or -1 if not found.
 */
isize si_string_rfind(siString str, cstring cstr);

/**
 * @brief Find the last occurrence of a substring within a specific range of a
 *        siString.
 *
 * @note This is the expanded version of the `si_string_rfind` function. Should be
 *       used for specifying the range and when the substring length is known.
 *
 * @param str The siString to search within.
 * @param start The index to start the search from (inclusive).
 * @param end The index to end the search at (exclusive).
 * @param cstr The substring to find within the siString.
 * @param cstr_len The length of the substring.
 *
 * @return The index of the last occurrence of the substring within the given
 *         range, or -1 if the substring is not found.
 */
isize si_string_rfind_ex(siString str, usize start, usize end, cstring cstr);


/**
 * @brief Join a siString with a null-terminated C string using a separator.
 *
 * This function appends a null-terminated C string, `cstr`, to the end of the
 * given siString, `str`, using a specified separator. The separator is inserted
 * between `str` and `cstr` in the resulting siString. The function modifies the
 * original `str` and returns its new length.
 *
 * @param str The siString to join.
 * @param cstr The null-terminated C string to append.
 * @param separator The separator string to insert between `str` and `cstr`.
 *
 * @return The new length of the siString after the join operation.
 */
void si_string_join(siString* str, cstring cstr, cstring separator);


/**
 * @brief Sets the siString to a null-terminated C string.
 *
 * @param str The siString to set.
 * @param cstr The new contents of the siString.
*/
void si_string_set(siString* str, cstring cstr);

/**
 * @brief Replaces all occurences of first null-terminated C string, `old_str`,
 *	      in the siString to the second null-terminted C string, `new_str`.
 *
 * @param str The siString to edit.
 * @param old_str The string to be replaced.
 * @param new_str The string that'll replace the old string.
*/
void si_string_replace(siString* str, cstring old_str, cstring new_str);

/**
 * @brief Cuts all occurences of the null-terminated C string in the siString.
 *
 * @param str The siString.
 * @param cut_set The string you want to be completely cut.
*/
void si_string_trim(siString* str, cstring cut_set);

/**
 * @brief Enquotes siString (String 'test' becomes '"test"').
 *
 * @param str The siString to enquote.
*/
void si_string_enquote(siString* str);

/**
 * @brief Appends a null-terminated C string to the siString.
 *
 * @param str The siString to append to.
 * @param other The C string to append.
*/
void si_string_append(siString* str, cstring other);

/**
 * @brief Appends a C string to the siString.
 *
 * @note This is the expanded version of the `si_string_append` function. Should
 *       be used when the length of the C string is known.
 *
 * @param str The siString to append to.
 * @param other The C string to append.
 * @param other_len Length of the C string (or how much of it to append).
 *
*/
void si_string_append_len(siString* str, cstring other, usize other_len);

/**
 * @brief Pushes (appends) the character into the siString.
 *
 * @param str The siString to push to.
 * @param other The character to push.
*/
void si_string_push(siString* str, char other);

/**
 * @brief Pops [erases] the last character of siString.
 *
 * @param str The siString to pop.
*/
void si_string_pop(siString* str);

/**
 * @brief Inserts a null-terimnated C-string to the siString at the specified index.
 *
 * @param str The siString to edit.
 * @param cstr The null-terminated C-string to insert.
 * @param index The index in which the string is inserted.
*/
void si_string_insert(siString* str, cstring cstr, usize index);

/**
 * @brief Inserts a C-string to the siString at the specified index.
 *
 * @note This is the expanded version of the `si_string_insert` function. Should
 *       be used when the length of the C string is known and if to erase the
 *       character at the index.
 *
 * @param str The siString to edit.
 * @param cstr The C string to insert.
 * @param cstr_len The length of the C string to insert.
 * @param index The index in which the string is inserted.
 * @param erase_index Deletes the character at the specified index depending on
 *                    the given boolean.
*/
void si_string_insert_ex(siString* str, cstring cstr, usize cstr_len, usize index, b32 erase_index);

/**
 * @brief Erases a substring from the specified index.
 *
 * @param str The siString to edit.
 * @param index The first index to erase.
 * @param erase_len How much of the substring must be erased.
*/
void si_string_erase(siString* str, usize index, usize erase_len);

void si_string_remove_cstr(siString* str, cstring cstr);
/* TODO(EimaMei): void si_string_swap(siString* str, cstring cstr1, cstring cstr2); */

void si_string_strip(siString* str); /* NOTE(EimaMei); This strips any leading or trailing spaces in the string. */
void si_string_reverse(siString* str);
void si_string_reverse_len(siString* str, usize len);

siArray(siString) si_string_split(siAllocator* alloc, siString str, cstring separator);
void si_string_clear(siString* str);

void si_string_free(siString str);
void si_string_make_space_for(siString* str, usize add_len);
void si_string_shrink_to_fit(siString* str);

#endif

#if !defined(SI_CHAR_UNDEFINE)
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
char  si_char_to_lower(char c);
char  si_char_to_upper(char c);

b32  si_char_is_space(char c);
b32  si_char_is_digit(char c);
b32  si_char_is_hex_digit(char c);
b32  si_char_is_alpha(char c);
b32  si_char_is_alphanumeric(char c);
i32   si_digit_to_int(char c);
i32   si_hex_digit_to_int(char c);


/*
	========================
	|  cstring             |
	========================
*/

#define SI_NUM_MAX_DIGITS 20

char* si_cstr_make(siAllocator* alloc, cstring cstr);
char* si_cstr_make_len(siAllocator* alloc, cstring cstr, usize len);
char* si_cstr_make_fmt(siAllocator* alloc, cstring cstr, ...);

usize si_cstr_len(cstring str);

void si_cstr_upper(char* str);
void si_cstr_lower(char* str);
void si_cstr_title(char* str);
void si_cstr_capitalize(char* str);

b32 si_cstr_equal(cstring str1, cstring str2);
b32 si_cstr_equal_len(cstring str1, usize str1_len, cstring str2, usize str2_len);

u64 si_cstr_to_u64(cstring str);
u64 si_cstr_to_u64_len(cstring str, usize len);
i64 si_cstr_to_i64(cstring str);
f64 si_cstr_to_f64(cstring str);

// TODO(EimaMei): Fix this to use alloc
#define si_u64_to_cstr(num) si_u64_to_cstr_ex(num, si_salloc(SI_NUM_MAX_DIGITS + 1))
char* si_u64_to_cstr_ex(u64 num, char* res);
#define si_i64_to_cstr(num) si_i64_to_cstr_ex(num, si_salloc(SI_NUM_MAX_DIGITS + 1))
char* si_i64_to_cstr_ex(i64 num, char* res);
/* TODO(EimaMei): si_f64_to_cstr */

#endif

#if !defined(SI_FILE_UNDEFINE)
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

typedef struct siFile {
	usize size;
	FILE* ptr;
    cstring filename;
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
b32 si_path_exists(cstring path);
isize si_path_copy(cstring existing_path, cstring new_path);
isize si_path_move(cstring existing_path, cstring new_path);
isize si_path_remove(cstring path);

cstring si_path_base_name(cstring path);
cstring si_path_extension(cstring path);
siString si_path_get_fullname(siAllocator* alloc, cstring path);

b32 si_path_is_absolute(cstring path);
b32 si_path_is_relative(cstring path);

u64 si_pathLastWriteTime(cstring filename);

/*
	========================
	|  siFile              |
	========================
*/
siFile si_fileCreate(cstring path);
siFile si_fileOpen(cstring path);
siFile si_fileOpenMode(cstring path, const char* mode);

usize si_file_size(siFile file);
void si_file_size_update(siFile* file);

rawptr si_fileRead(siAllocator* alloc, siFile file);
rawptr si_fileReadAt(siAllocator* alloc, siFile file, usize offset, usize len);
//siArray(siString) si_file_readlines(siAllocator* alloc, siFile file);

siError si_file_write(siFile* file, cstring content);
siError si_file_write_len(siFile* file, cstring content, usize len);
siError si_file_write_at(siFile* file, cstring content, usize offset);
siError si_file_write_at_line(siFile* file, cstring content, usize index);
siError si_file_write_at_len(siFile* file, cstring content, usize content_len, usize offset);

isize si_file_seek(siFile file, usize offset);
isize si_file_seek_to_end(siFile file);

void si_file_close(siFile file);

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

#if !defined(SI_THREAD_UNDEFINE)
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
	#if defined(SI_SYSTEM_WINDOWS)
		HANDLE id;
	#else
		pthread_t id;
	#endif
	volatile b32 is_running;
	rawptr return_value;

	siFunction func;
	rawptr arg;
} siThread;


#define si_thread_create(function, arg) si_thread_create_ex(siFunc(function), arg)
siThread si_thread_create_ex(siFunction function, rawptr arg);

void si_thread_start(siThread* t);
void si_thread_start_stack(siThread* t, usize stack_size);
void si_thread_join(siThread* t);

void si_thread_cancel(siThread* t); /* NOTE(EimaMei): Only works on Unix. */
void si_thread_destroy(siThread* t);

void si_thread_set_priority(siThread t, i32 priority);

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


f64 si_time_now(void);
u64 si_time_utc_now(void);

void si_sleep(usize miliseconds);

u64 si_RDTSC(void); /* NOTE(EimaMei): For non i386, x86, arm64 and PPC CPUs, this defaults to gettimeofday. */
/* TODO(EimaMei): I am pretty sure 'si_RDTSC()' for arm64 is wrong, but I can't tell. Docker is giving some weird results.*/
#endif

#if !defined(SI_BIT_UNDEFINE)
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
    SI_BIT_ZERO,
    SI_BIT_ONE
};


#define SI_BIT(x) (1ULL << si_cast(usize, x))
#define SI_BIT_MSB(num) ((num & SI_BIT(sizeof(typeof(num)) * 8 - 1)) != 0)
#define SI_BIT_LSB(num) ((num & SI_BIT(0)) != 0)

#define SI_BYTE_TO_BIT(bits) ((bits) * (usize)8)

#define SI_NUM_HIGH_BITS(value) ( \
    (value & ((typeof(value))0xFF << SI_BYTE_TO_BIT(sizeof(typeof(value)) - 1))) \
    >> \
    SI_BYTE_TO_BIT(sizeof(typeof(value)) - 1))

#define SI_NUM_LOW_BITS(value) (value & ((typeof(value))0xFF))

#define SI_NUM_BIT_GET(variable, flag) (((variable) & (flag)) != 0)
#define SI_NUM_BIT_SET(variable, flag, condition) \
    (condition) ? ((variable) |= (flag)) : ((variable) &= ~(flag))


/* NOTE(EimaMei): In practice: Lets say we have array: (u16){0x00FF, 0xAAAA, 0x0008}.
 * We want the first value so we do: si_u64FromPtr(array, sizeof(u16)).
 * The macro first casts it as an u64* and dereference (which grabs 8 bytes).
 * The current result is: 0x0008AAAA00FF
 * However since the answer is in the low bit, we can bitwise AND the low bit and get the answer.
 * Since u16 has 2 bytes, we bitwise AND it by 16 bits (2 * 8) and get the answer.
 * 0x0008AAAA00FF -> 0xFF
 */
#define si_u64FromPtr(ptr, int_sizeof) \
    (*si_cast(u64*, ptr) & ((int_sizeof < 8) ? (SI_BIT((int_sizeof) * 8) - 1) : UINT64_MAX) )

#define si_num_is_neg(x) SI_BIT_MSB(x)


usize si_num_bits_u8(u8 num);
usize si_num_bits_u32(u32 num);
usize si_num_bits_u64(u64 num);

#if SI_STANDARD_VERSION >= SI_STANDARD_C11
    #define si_num_bits(num) \
        _Generic((num), \
            u8  : si_num_bits_u8(num), \
            u16 : si_num_bits_u32(num), \
            u32 : si_num_bits_u32(num), \
            u64 : si_num_bits_u64(num), \
            default: si_num_bits_u64(num) \
        )(X)
#endif



#define si_num_leading_bit(num, bit_type) si_num_leading_bit_ex(num, sizeof(typeof(num)) * 8, bit_type)
#define si_num_trailing_bit(num, bit_type) si_num_trailing_bit_ex(num, sizeof(typeof(num)) * 8, bit_type)

#define si_num_rotate_left(num, bits) si_num_rotate_left_ex(num, sizeof(typeof(num)) * 8, bits)
#define si_num_rotate_right(num, bits) si_num_rotate_right_ex(num, sizeof(typeof(num)) * 8, bits)
#define si_num_reverse_bits(num) si_num_reverse_bits_ex(num, sizeof(typeof(num)) * 8 )

#define si_num_to_bytes(allocator, num) si_num_to_bytes_ex(allocator, num, sizeof(typeof(num)))
u64 si_bytes_to_num(siArray(u8));

#define si_num_change_endian(num) si_num_change_endian_ex(num, sizeof(typeof(num)))

u64 si_num_pow(u64 base, usize exp);
u64 si_num_pow2(u64 base, usize exp);
u64 si_num_round_nearest_multiple(u64 num, usize multiple);
usize si_num_len(u64 num);


#if 1 /* NOTE(EimaMei): The actual header definition for the macros. No reason to use these in practice. */
usize si_num_leading_bit_ex(u64 num, usize sizeof_num, siBitType bit);
usize si_num_trailing_bit_ex(u64 num, usize number_sizeof, siBitType bit);

u64 si_num_rotate_left_ex(u64 num, usize num_sizeof, usize bits);
u64 si_num_rotate_right_ex(u64 num, usize num_sizeof, usize n);
u64 si_num_reverse_bits_ex(u64 num, usize num_sizeof);

siArray(u8) si_num_to_bytes_ex(siAllocator* alloc, u64 num, usize num_sizeof);

u64 si_num_change_endian_ex(u64 num, usize num_sizeof);
#endif

#endif

#if !defined(SI_CPU_UNDEFINE)
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

siMhz si_cpu_clock_speed(void);

#endif


#if !defined(SI_PERFORMANCE_UNDEFINE)

#if !defined(SI_MEMORY_LOGGING)
	#define SI_PERFORMANCE_MSG \
		"====== BENCHMARK DATA ======\n" \
		"General:\n" \
			"\tFunction - '%s'\n" \
			"\tDuration - '%f' ms\n" \
			"\tExecution count - '%f' million\n"

	#define SI_PERFORMANCE_PRINT_MEMORY(function) do {} while (0)
#else
	#define SI_PERFORMANCE_MSG \
		"====== BENCHMARK DATA ======\n" \
		"NOTE: General execution count might be slower due to the memory logging.\n" \
		"General:\n" \
			"\tFunction - '%s'\n" \
			"\tDuration - '%f' ms\n" \
			"\tExecution count - '%f' million\n"

	#define SI_PERFORMANCE_MSG_DEBUG(\
		"Memory:\n" \
			"\tAllocation count - '%zd'\n" \
			"\tFree count - '%zd'\n" \
			"\tTotal RAM usage - '%zd' bytes\n" \
			"\tOverall allocated RAM left - '%zd' bytes\n"

    #define SI_PERFORMANCE_PRINT_MEMORY(function) \
		do { \
			siDebugGlobalVars debug_old = si_debug; \
			si_debug = (siDebugGlobalVars){0, 0, 0, 0, nil}; \
			function; \
			si_debug_print_all(); \
			si_debug_print_allocations(); \
			si_debug_cleanup(); \
			si_debug = si_debug_old; \
		} while(0)
#endif

void si_impl_performance_thread(u32* arg);

#define si_performance_run_per_loop(times_to_loop, function) \
	do { \
		usize index; \
		clock_t time_since = clock(); \
		for (index = 0; index < (usize)times_to_loop; index++) { \
			function; \
		} \
		time_since = clock() - time_since; \
		f64 time_taken = ((f64)time_since) / 1000.0; /* NOTE(EimaMei): This will take the time in ms, not seconds. */ \
		printf(SI_PERFORMANCE_MSG, #function, time_taken, (f64)times_to_loop / 1000000.0); \
		SI_PERFORMANCE_PRINT_MEMORY(function); \
	} while(0)

#define si_performance_executes_per_ms(ms, function) \
	do { \
		u64 counter = 0; \
		u32 miliseconds = (u32)ms; \
		siThread thread = si_thread_create(si_impl_performance_thread, &miliseconds); \
		si_thread_start(&thread); \
		while (thread.is_running) { \
			function; \
			counter += 1; \
		} \
		printf(SI_PERFORMANCE_MSG, #function, (f64)miliseconds, (f64)counter / 1000000.0); \
		SI_PERFORMANCE_PRINT_MEMORY(function); \
	} while(0)


typedef struct {
    char runs_text[countof("runs")];
    char space_pad[SI_NUM_MAX_DIGITS];
} siPerformanceMSG;

intern siPerformanceMSG* si_performance_get_msg_vars(siAllocator* alloc,
        usize increments_of_10, usize median_index) {

    char runs_text[] = "runs";
    SI_STOPIF(median_index == 1, runs_text[3] = ' ');

    isize space_pad_len = si_num_len(increments_of_10) - si_num_len(median_index);
    char space_pad[SI_NUM_MAX_DIGITS] = {0};
    memset(space_pad, ' ', space_pad_len);


    siPerformanceMSG* msg = (siPerformanceMSG*)si_malloc(alloc, sizeof(*msg));
    memcpy(msg->runs_text, runs_text, countof(runs_text));
    memcpy(msg->space_pad, space_pad, space_pad_len);

    return msg;
}

intern const siPair(u64, cstring) si_perf_limit[] = {
    {0001ULL, "ns"},
    {1000ULL, "μs"},
    {1000ULL * 1000, "ms"},
    {1000ULL * 1000 * 1000, " s"},
    {1000ULL * 1000 * 1000 * 60, "min"},
    {1000ULL * 1000 * 1000 * 60 * 60, " h"},
    {1000ULL * 1000 * 1000 * 60 * 60 * 24, " d"} /* NOTE(EimaMei): I'll be shocked if someone someone's performance loop lasted an entire day. Also a bug happens here, I think. Oops. */
};

intern typeof(*si_perf_limit)* si_performance_limit_loop(u64 time) {
    typeof(*si_perf_limit)* element = si_perf_limit;
    typeof(*si_perf_limit)* end = si_perf_limit + countof(si_perf_limit);

    while (element != end) {
        SI_STOPIF(
            !si_between(time, element->first, (element + 1)->first),
            { element += 1; continue; }
        );
        break;
    }

    return element;
}

void si_performance_loops_average_print(siAllocator* alloc, cstring funcname,
        usize start, usize end, siArray(u64) cycles, f32 ghz_freq) {
	printf(
        "====== BENCHMARK DATA ======\n"
        "General:\n"
            "\tFunction - '%s'\n"
        "Runs:\n",
        funcname
    );
    typeof(*si_perf_limit)* element = nil;

    usize array_index = 0, median_index;
    for (median_index = start; median_index < 10 * end; median_index *= 10) {
        u64 amount_cycles = cycles[array_index];
        f64 amount_time = (f64)amount_cycles / ghz_freq;

        element = si_performance_limit_loop(amount_time);
        amount_time /= element->first;

        siPerformanceMSG* msg = si_performance_get_msg_vars(alloc, end, median_index);
        printf(
            "\t%s%zu %s - %9.4f %s (%"PRIu64" cycles)\n",
            msg->space_pad, median_index, msg->runs_text,
            amount_time, element->second, amount_cycles
        );

        array_index += 1;
    }

    f64 cycles_median = (f64)si_arraySumU64(cycles) / si_arrayLen(cycles);
    f64 time_median = cycles_median / ghz_freq;
    element = si_performance_limit_loop(time_median);
    time_median /= element->first;

    printf(
        "Final result:\n"
            "\tTime average   - %9.4f %s\n"
            "\tCycles average - %"PRIu64" cycles\n",
        time_median, element->second,
        (u64)cycles_median
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

void si_performance_loops_average_print_cmp(siAllocator* alloc, cstring funcname[2],
        usize start, usize end, siArray(u64) cycles[2], f32 ghz_freq) {
	printf(
        "====== BENCHMARK DATA ======\n"
        "General:\n"
            "\tFunction 1 - '%s'\n"
            "\tFunction 2 - '%s'\n"
        "Runs:\n",
        funcname[0], funcname[1]
    );
    typeof(*si_perf_limit)* element_0 = nil, *element_1 = nil;

    usize array_index = 0, median_index;
    for (median_index = start; median_index < 10 * end; median_index *= 10) {
        u64 cycles_0 = cycles[0][array_index],
            cycles_1 = cycles[1][array_index];
        f64 time_0 = (f64)cycles_0 / ghz_freq,
            time_1 = (f64)cycles_1 / ghz_freq;

        b32 zero_is_slower = (time_0 > time_1);
        f64 ratio = zero_is_slower ? (time_0 / time_1) : (time_1 / time_0);
        cstring clr_0 = zero_is_slower ? SI_TRED : SI_TGRN,
                clr_1 = zero_is_slower ? SI_TGRN : SI_TRED;

        element_0 = si_performance_limit_loop(time_0);
        element_1 = si_performance_limit_loop(time_1);
        time_0 /= element_0->first;
        time_1 /= element_1->first;

        siPerformanceMSG* msg = si_performance_get_msg_vars(alloc, end, median_index);
        printf(
            "\t%s%zu %s - %s%9.4f %s%s vs %s%9.4f %s%s (%4.4f ratio, %"PRIu64" vs %"PRIu64" cycles)\n",
            msg->space_pad, median_index, msg->runs_text,
            clr_0, time_0, element_0->second, SI_TEND, clr_1, time_1, element_1->second, SI_TEND,
            ratio, cycles_0, cycles_1
        );

        array_index += 1;
    }

    f64 cycles_median_0 = (f64)si_arraySumU64(cycles[0]) / si_arrayLen(cycles[0]),
        cycles_median_1 = (f64)si_arraySumU64(cycles[1]) / si_arrayLen(cycles[1]);
    f64 time_median_0 = cycles_median_0 / ghz_freq,
        time_median_1 = cycles_median_1 / ghz_freq;

    b32 zero_is_slower = (time_median_0 > time_median_1);
    f64 ratio = zero_is_slower ? (time_median_0 / time_median_1) : (time_median_1 / time_median_0);
    cstring clr_0 = zero_is_slower ? SI_TRED : SI_TGRN,
            clr_1 = zero_is_slower ? SI_TGRN : SI_TRED;

    element_0 = si_performance_limit_loop(time_median_0);
    element_1 = si_performance_limit_loop(time_median_1);
    time_median_0 /= element_0->first;
    time_median_1 /= element_1->first;

    printf(
        "Final result:\n"
            "\tTime average   - %s%9.4f %s%s vs %s%9.4f %s%s (%4.4f ratio)\n"
            "\tCycles average - %"PRIu64" cycles vs %"PRIu64" cycles\n",
        clr_0, time_median_0, element_0->second, SI_TEND,
        clr_1, time_median_1, element_1->second, SI_TEND,
        ratio,
        (u64)cycles_median_0, (u64)cycles_median_1
    );
}

#define si_performance_loop(function, cycles, start, end) \
    do { \
        usize array_index = 0; \
        u64 median_index; \
        for (median_index = start; median_index < 10 * end; median_index *= 10) { \
            u64 index; \
            u64 begin = si_RDTSC(); \
            for (index = 0; index < median_index; index++) { \
                function; \
            } \
            begin = si_RDTSC() - begin; \
            cycles[array_index] = begin; \
            array_index += 1; \
        } \
        SI_ARRAY_HEADER(cycles)->len = array_index; \
    } while (0)

#define si_performance_loops_average(increments_of_10, function) \
    si_performance_loops_average_range(1, increments_of_10, function)

#define si_performance_loops_average_range(start, end, function) \
	do { \
        siAllocator* alloc = si_allocatorMakeStack(4096); \
		siArray(u64) cycles = si_arrayMakeReserve(alloc, sizeof(u64), 20); \
        si_performance_loop(function, cycles, start, end); \
        \
        si_performance_loops_average_print(alloc, #function, start, end, cycles, (f64)si_cpu_clock_speed() / 1000); \
		SI_PERFORMANCE_PRINT_MEMORY(function); \
	} while(0)



#define si_performance_loops_average_cmp(increments_of_10, function1, function2) \
    si_performance_loops_average_cmp_range(1, increments_of_10, function1, function2)

#define si_performance_loops_average_cmp_range(start, end, function1, function2) \
	do { \
        siAllocator* alloc = si_allocatorMakeStack(4096); \
		siArray(u64) cycles[2] = { \
            (u64*)si_arrayMakeReserve(alloc, sizeof(u64), 20), \
            (u64*)si_arrayMakeReserve(alloc, sizeof(u64), 20)  \
        };\
        si_performance_loop(function1, cycles[0], start, end); \
        si_performance_loop(function2, cycles[1], start, end); \
        \
        cstring funcnames[2] = {#function1, #function2}; \
        si_performance_loops_average_print_cmp(alloc, funcnames, start, end, cycles, (f64)si_cpu_clock_speed() / 1000); \
		SI_PERFORMANCE_PRINT_MEMORY(function); \
	} while(0)
#endif

#if defined(SI_MEMORY_LOGGING)
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
	=========================
	| Memory Logging (Beta) |
	=========================
*/

typedef struct {
	cstring type;
	usize len;
	u8 state; /* 0 - NOT FREED, 1 - FREED, 2 - REALLOCED */

	cstring date, time, file;
	i32 line;
	cstring function;

	u32 index;
} siDebugHeader;
#define SI_DEBUG_HEADER(s) ((siDebugHeader*)s - 1)

typedef siPair(usize, usize) siDebugArgs;

rawptr si_debug_alloc(usize function_id, rawptr ptr, siDebugArgs args, cstring file, i32 line, cstring function, cstring time, cstring date);
void si_debug_free(rawptr ptr);

#define malloc(size) si_debug_alloc(1, nil, (siDebugArgs){size, 0}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define calloc(num, size) si_debug_alloc(2, nil, (siDebugArgs){size, num}, num, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define realloc(ptr, newSize) si_debug_alloc(3, ptr, (siDebugArgs){0, newSize}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define si_realloc(ptr, oldSize, newSize) si_debug_alloc(4, ptr, (siDebugArgs){oldSize, newSize}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define free(ptr) si_debug_free(ptr)

void si_debug_print_all(void);
void si_debug_print_allocations(void);
void si_debug_cleanup(void);

#define SI_QUICK_LOG() si_debug_print_all(); si_debug_print_allocations();

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
	#define SI_GENERAL_IMPLEMENTATION
	#define SI_ALLOCATOR_IMPLEMENTATION
	#define SI_PAIR_IMPLEMENTATION
	#define SI_OPTIONAL_IMPLEMENTATION
	#define SI_ARRAY_IMPLEMENTATION
	#define SI_STRING_IMPLEMENTATION
	#define SI_CHAR_IMPLEMENTATION
	#define SI_FILE_IMPLEMENTATION
	#define SI_THREAD_IMPLEMENTATION
	#define SI_BIT_IMPLEMENTATION
    #define SI_CPU_IMPLEMENTATION
    #define SI_PERFORMANCE_IMPLEMENTATION
#endif


#if defined(SI_ALLOCATOR_IMPLEMENTATION)


#define SI_DEFAULT_MEMORY_ALIGNMENT (sizeof(usize) * 2)

intern usize si_alignCeil(usize n) {
    if (n % SI_DEFAULT_MEMORY_ALIGNMENT == 0) {
        return n;
    }

    usize out = n;
    out |= SI_DEFAULT_MEMORY_ALIGNMENT - 1;
    out++;
    return out;
}

inline siAllocator* si_allocatorMake(usize bytes) {
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
inline siAllocator* si_intern_allocatorMakeStack(usize bytes, rawptr ptr) {
    siAllocator* res = (siAllocator*)ptr;
    res->ptr = (siByte*)ptr + sizeof(siAllocator);
    res->offset = 0;
    res->maxLen = bytes;

    return res;
}
F_TRAITS(inline)
void si_allocator_refresh(siAllocator* alloc) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
    usize size = alloc->maxLen;
    si_allocatorFree(alloc);
    *alloc = *si_allocatorMake(size);
#endif
    SI_UNUSED(alloc);
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
void si_allocator_push(siAllocator* alloc, siByte byte) {
    SI_ASSERT_NOT_NULL(alloc);
    SI_ASSERT_FMT(si_between(alloc->offset + 1, 0, alloc->maxLen),
        "Exceeded the available memory for an allocation",
        " (Tried writing '%zd' bytes into an already filled allocator with '%zd' bytes. Current index: '%zd').\n",
        alloc->maxLen, alloc->offset);

    alloc->ptr[alloc->offset] = byte;
    alloc->offset += 1;
}

F_TRAITS(inline)
rawptr si_mallocEx(siAllocator* alloc, usize bytes, cstring filename, i32 line) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	SI_ASSERT_NOT_NULL(alloc);
    bytes = si_alignCeil(bytes);

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
    usize ratio = ((usize)ptr - (usize)alloc->ptr);
    SI_ASSERT_FMT(ratio % SI_DEFAULT_MEMORY_ALIGNMENT % 4 == 0, "%s:%i: Invalid pointer (pointer wasn't made via 'si_malloc')", filename, line);
	alloc->offset = ratio;
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

    oldSize = si_alignCeil(oldSize);
    newSize = si_alignCeil(newSize);

    SI_ASSERT_FMT(
        si_between((siByte*)ptr, alloc->ptr, alloc->ptr + alloc->maxLen),
        "%s:%i: Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
        filename, line, ptr, alloc->ptr, alloc->ptr + alloc->maxLen
    );
    usize ratio = ((usize)ptr - (usize)alloc->ptr);
    SI_ASSERT_FMT(ratio % SI_DEFAULT_MEMORY_ALIGNMENT % 4 == 0, "%s:%i: Invalid pointer (pointer wasn't made via 'si_malloc')", filename, line);

    rawptr out = si_malloc(alloc, newSize);
    memcpy(out, ptr, oldSize);
    return out;
#else
    return realloc(ptr, newSize);
    SI_UNUSED(alloc); SI_UNUSED(ptr); SI_UNUSED(oldSize); SI_UNUSED(newSize) SI_UNUSED(filename); SI_UNUSED(line);
#endif
}


#endif


#if defined(SI_GENERAL_IMPLEMENTATION)

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
u64 si_time_utc_now(void) {
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
void si_ptrMoveBy(rawptr src, usize srcLen, usize moveBy, siDirection direction) {
    isize mul = (direction == SI_DIRECTION_RIGHT ? 1 : -1); /* NOTE(EimaMei): Branchless way of moving. */
    memcpy((siByte*)src + ((isize)moveBy * mul), src, srcLen);
}

/* TODO(EimaMei): Check for every usage of malloc, calloc, realloc and free to see if they're correct. */
#endif


#if defined(SI_PAIR_IMPLEMENTATION) && !defined(SI_PAIR_UNDEFINE)

rawptr si_impl_pair_make(siAllocator* alloc, siAny first, siAny second) {
	siByte* res = (siByte*)si_malloc(alloc, first.typeSize + second.typeSize);
	memcpy(res, first.ptr, first.typeSize);
	memcpy(res + first.typeSize, second.ptr, second.typeSize);

	return (rawptr)res;
}
#endif

#if defined(SI_ARRAY_IMPLEMENTATION) && !defined(SI_ARRAY_UNDEFINE)

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

	usize separator_len = si_cstr_len(separator);
	usize total_size = 0;

	for_range (i, 0, si_arrayLen(array)) {
		total_size += si_cstr_len(array[i]) + separator_len;
	}

	siString result = si_string_make_reserve(SI_ARRAY_HEADER(array)->allocator, total_size);
	rawptr back_ptr = si_arrayBack(array);
	for_each (str, array) {
		if (separator != nil && (&str) != back_ptr) {
			si_string_join(&result, separator, *str);
		}
		else {
			si_string_append(&result, *str);
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

#if defined(SI_STRING_IMPLEMENTATION) && !defined(SI_STRING_UNDEFINE)


F_TRAITS(inline)
siString si_string_make(siAllocator* alloc, cstring str) {
	return si_string_make_len(alloc, str, str ? si_cstr_len(str) : 0);
}
siString si_string_make_len(siAllocator* alloc, cstring str, usize len) {
	siString res_str = si_string_make_reserve(alloc, len);
	memcpy(res_str, str, len);

	siStringHeader* header = SI_STRING_HEADER(res_str);
	header->len = len;

	return res_str;
}
siString si_string_make_fmt(siAllocator* alloc, cstring str, ...) {
    SI_ASSERT_NOT_NULL(str);

    va_list args, copy;
    va_start(args, str);

    va_copy(copy, args);
    usize  size = vsnprintf(nil, 0, str, copy);
    va_end(copy);

    siString buffer = si_string_make_reserve(alloc, size);
    vsnprintf(buffer, size + 1, str, args);
    va_end(args);

    return buffer;
}
siString si_string_make_reserve(siAllocator* alloc, usize len) {
	rawptr ptr = si_malloc(alloc, sizeof(siStringHeader) + (len + 1));
	SI_ASSERT_NOT_NULL(ptr);

	siString res_str = (siString)ptr + sizeof(siStringHeader);
	res_str[len] = '\0';

	siStringHeader* header = SI_STRING_HEADER(res_str);
	header->len = 0;
	header->capacity = len;
    header->allocator = alloc;
	/*header->typeSize = sizeof(char); */

	return res_str;
}

F_TRAITS(inline)
siString si_string_copy(siAllocator* alloc, siString from) {
	return si_string_make_len(alloc, from, si_string_len(from));
}

F_TRAITS(inline)
usize si_string_len(siString str) {
	return SI_STRING_HEADER(str)->len;
}
F_TRAITS(inline)
usize si_string_capacity(siString str) {
	return SI_STRING_HEADER(str)->capacity;
}
F_TRAITS(inline)
usize si_string_type_size(siString str) {
	return sizeof(char);
    SI_UNUSED(str);
}
inline siAllocator* si_string_allocator(siString str) {
    return SI_STRING_HEADER(str)->allocator;
}
F_TRAITS(inline)
b32 si_string_empty(siString str) {
	return (str == nil || SI_STRING_HEADER(str)->len == 0);
}

inline char* si_cstr_make(siAllocator* alloc, cstring cstr) {
	return si_cstr_make_len(alloc, cstr, si_cstr_len(cstr));
}
inline char* si_cstr_make_len(siAllocator* alloc, cstring cstr, usize len) {
	char* str = (char*)si_malloc(alloc, len + 1);
	memcpy(str, cstr, len + 1);

	return str;
}

char* si_cstr_make_fmt(siAllocator* alloc, cstring format, ...) {
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
usize si_cstr_len(cstring str) {
	return strlen(str); /* NOTE(EimaMei): Generally for performance, OG strlen is best for both portability and performance. */
}

F_TRAITS(inline)
b32 si_cstr_equal(cstring str1, cstring str2) {
	return si_cstr_equal_len(str1, si_cstr_len(str1), str2, si_cstr_len(str2));
}
b32 si_cstr_equal_len(cstring str1, usize str1_len, cstring str2, usize str2_len) {
	if (str1 == str2) {
		return true;
	}
	if (str1_len != str2_len) {
		return false;
	}

	usize i;
	for (i = 0; i < str1_len; i++) {
		if (str1[i] != str2[i])
			return false;
	}

	return true;
}

F_TRAITS(inline)
char si_string_at(siString str, usize index) {
	SI_ASSERT_NOT_NULL(str);
	if (index > si_string_len(str) || si_string_len(str) == 0) {
		return SI_ERROR;
	}

	return str[index];
}
F_TRAITS(inline)
char si_string_front(siString str) {
	SI_ASSERT_NOT_NULL(str);
	if (si_string_len(str) == 0) {
		return SI_OKAY;
	}

	return str[0];
}
F_TRAITS(inline)
char si_string_back(siString str) {
	SI_ASSERT_NOT_NULL(str);
	if (si_string_len(str) == 0) {
		return '\0';
	}

	return str[si_string_len(str) - 1];
}
char* si_string_sub_ex(siString str, usize begin, usize len, char* result) {
	memcpy(result, str + begin, len);
	*(result + len) = '\0';
    return result;
}

F_TRAITS(inline)
isize si_string_find(siString str, cstring cstr) {
	return si_string_find_ex(str, 0, si_string_len(str), cstr, si_cstr_len(cstr));
}
isize si_string_find_ex(siString str, usize start, usize end, cstring cstr, usize cstr_len) {
	SI_ASSERT_NOT_NULL(str);

	b32 found = false;

	usize i;
	usize cur_count = 0;
	for (i = start; i < end; i++) {
		if (str[i] == cstr[cur_count]) {
			cur_count++;

			if (cur_count == cstr_len) {
				found = true;
				break;
			}
		}
		else if (cur_count != 0) {
			cur_count = 0;
		}
	}

	return (found ? si_cast(isize, i - cstr_len + 1) : SI_ERROR);
}
F_TRAITS(inline)
isize si_string_rfind(siString str, cstring cstr) {
	return si_string_rfind_ex(str, si_string_len(str) - 1, 0, cstr);
}
isize si_string_rfind_ex(siString str, usize start, usize end, cstring cstr) {
	SI_ASSERT_NOT_NULL(str);

	usize cstr_len = si_cstr_len(cstr);
	b32 found = false;

	usize i;
	isize cur_count = cstr_len - 1;
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

void si_string_join(siString* str, cstring cstr, cstring separator) {
	usize cstr_len = si_cstr_len(cstr);
	usize separator_len = si_cstr_len(separator);

	siAllocator* alloc = si_allocatorMakeStack(SI_KILO(8)); /* NOTE(EimaMei):
                                            8k should be enough for most cases,
                                            I think. Maybe it's still more faster
                                            and safer to just do a heap allocation?
                                            */

	memcpy(alloc, separator, separator_len);
	memcpy((siByte*)alloc + separator_len, cstr, cstr_len);

	si_string_append_len(str, (char*)alloc, separator_len + cstr_len);
}
void si_string_set(siString* str, cstring cstr) {
	SI_ASSERT_NOT_NULL(str);
	siString cur_str = *str;

	usize len = si_cstr_len(cstr);

	siStringHeader* header = SI_STRING_HEADER(cur_str);

	if (header->capacity < len) {
		si_string_make_space_for(str, len - header->capacity);
		cur_str = *str;
		header = SI_STRING_HEADER(cur_str);
	}
	header->len = len;

	memcpy(cur_str, cstr, len);
	cur_str[len] = '\0';
}
void si_string_replace(siString* str, cstring oldValue, cstring newValue) {
	usize old_len = si_cstr_len(oldValue);
	usize new_len = si_cstr_len(newValue);

	isize index = 0;
	siString cur_str = *str;

	while (true) {
		index = si_string_find_ex(cur_str, index, si_string_len(cur_str), oldValue, old_len);
		if (index == SI_ERROR) {
			break;
		}

		si_string_erase(str, index, old_len);
		si_string_insert_ex(str, newValue, new_len, index, false);
	}
}
void si_string_trim(siString* str, cstring cut_set) {
	char* start = *str;
	char* end  = *str + si_string_len(*str) - 1;
	char* start_pos = start;
	char* end_pos = end;

	siStringHeader* header = SI_STRING_HEADER(*str);
	header->len -= si_cstr_len(cut_set);

	while (start_pos <= end && strchr(cut_set, *start_pos))
		start_pos++;
	while (end_pos > start_pos && strchr(cut_set, *end_pos))
		end_pos--;

	usize len = (start_pos > end_pos) ? 0 : ((end_pos - start_pos)+1);

	if (*str != start_pos) {
		memmove(str, start_pos, len);
	}
	(*str)[len] = '\0';
}
void si_string_enquote(siString* str) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	header->len += 2;

	if (header->capacity < header->len) {
		si_string_make_space_for(str, 2);
		header = SI_STRING_HEADER(*str);
	}
	siString cur_str = *str;

	si_ptrMoveBy(cur_str, header->len, 1, SI_DIRECTION_RIGHT);
	cur_str[0] = '\"';
	cur_str[header->len - 1] = '\"';
}

F_TRAITS(inline)
void si_string_append(siString* str, cstring other) {
	si_string_append_len(str, other, si_cstr_len(other));
}
void si_string_append_len(siString* str, cstring other, usize other_len) {
	SI_ASSERT_NOT_NULL(str);
	siString cur_str = *str;

	siStringHeader* header = SI_STRING_HEADER(cur_str);
	usize previous_len = header->len;
	header->len += other_len;

	if (header->capacity < header->len) {
        si_string_make_space_for(str, other_len);
		cur_str = *str;
		header = SI_STRING_HEADER(cur_str);
	}

	memcpy(cur_str + previous_len, other, other_len);
	cur_str[header->len] = '\0';
}
F_TRAITS(inline)
void si_string_push(siString* str, char other) {
	si_string_append_len(str, &other, 1);
}
F_TRAITS(inline)
void si_string_pop(siString* str) {
	siString cur_str = *str;
	*(cur_str + SI_STRING_HEADER(cur_str)->len - 1) = '\0';
	SI_STRING_HEADER(cur_str)->len -= 1;
}

F_TRAITS(inline)
void si_string_insert(siString* str, cstring cstr, usize index) {
	si_string_insert_ex(str, cstr, si_cstr_len(cstr), index, false);
}
void si_string_insert_ex(siString* str, cstring cstr, usize cstr_len, usize index, b32 erase_index) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize previous_len = header->len;
	usize before_index_len = previous_len - index;

	SI_ASSERT_FMT(index < previous_len, "Index is higher than the length of the string (str_len - '%zd', index - '%zd')", previous_len, index);
	header->len += cstr_len - erase_index;

	if (header->capacity < header->len) {
		si_string_make_space_for(str, header->len - header->capacity);
		header = SI_STRING_HEADER(*str); /* NOTE(EimaMei): For some reason we have to refresh the header pointer on Linux. Somewhat makes sense but also what and why. */
	}
	siString cur_str = *str;

	char* ptr = (char*)memcpy(cur_str + header->len - before_index_len, cur_str + index, before_index_len);
	memcpy(cur_str + index, cstr, cstr_len);
	ptr[before_index_len] = '\0';
}
void si_string_erase(siString* str, usize index, usize erase_len) {
	siString cur_str = *str;
	SI_ASSERT_NOT_NULL(cur_str);

	usize str_len = si_string_len(cur_str);
	SI_ASSERT_FMT(index < str_len, "Index is higher than the length of the string. (str_len - '%zd', index - '%zd')", str_len, index);

	usize after_index_len = index + erase_len;

	char* ptr = (char*)memcpy(cur_str + index, cur_str + after_index_len, str_len - after_index_len);
	ptr[str_len - after_index_len] = '\0';

	SI_STRING_HEADER(cur_str)->len -= erase_len;
}
void si_string_remove_cstr(siString* str, cstring cstr) {
	siString cur_str = *str;
	SI_ASSERT_NOT_NULL(cur_str);

	siStringHeader* header = SI_STRING_HEADER(cur_str);
	usize cstr_len = si_cstr_len(cstr);
	while (true) {
		cur_str = *str;

		isize index = si_string_rfind_ex(cur_str, header->len - 1, 0, cstr);
		if (index == -1) {
			break;
		}

		usize after_index_len = index + cstr_len;

		char* ptr = (char*)memcpy(cur_str + index, cur_str + after_index_len, header->len - after_index_len);
		ptr[header->len - after_index_len] = '\0';

		header->len -= cstr_len;
	}
}
void si_string_swap(siString* str, cstring cstr1, cstring cstr2);

F_TRAITS(inline)
void si_cstr_upper(char* str) {
	SI_ASSERT_NOT_NULL(str);

	char x = '\0';
	while ((x = *str)) {
		*str = si_char_to_upper(x);
		str++;
	}
}
F_TRAITS(inline)
void si_cstr_lower(char* str) {
	SI_ASSERT_NOT_NULL(str);

	char x = '\0';
	while ((x = *str)) {
		*str = si_char_to_lower(x);
		str += 1;
	}
}
void si_cstr_title(char* str) {
	SI_ASSERT_NOT_NULL(str);

	b32 change = true;
	char x = '\0';
	while ((x = *str)) {
		if (si_char_is_space(x)) {
			change = true;
		}
		else if (change) {
			*str = si_char_to_upper(x);
			change = false;
		}
		str += 1;
	}
}
F_TRAITS(inline)
void si_cstr_capitalize(char* str) {
	SI_ASSERT_NOT_NULL(str);

	si_cstr_lower(str);
	*str = si_char_to_upper(*str);
}
void si_string_strip(siString* str) {
	SI_ASSERT_NOT_NULL(str);

	usize start = 0, end = 0;
	siString cur_str = *str;
	siString x = nil;
	for (x = cur_str; x != cur_str + si_string_len(cur_str); x += 1) {
		if (!si_char_is_space(*x)) {
			break;
		}
		start += 1;
	}

	usize i;
	for (i = si_string_len(cur_str) - 1; i < (usize)(-1); i--) {
		if (!si_char_is_space(cur_str[i])) {
			break;
		}
		end += 1;
	}
	char* stripped_str = cur_str + start * si_string_type_size(cur_str);
	stripped_str[si_string_len(cur_str) - start - end] = '\0';

	si_string_set(str, stripped_str);
}
F_TRAITS(inline)
void si_string_reverse(siString* str) {
	si_string_reverse_len(str, si_string_len(*str));
}
void si_string_reverse_len(siString* str, usize len) {
	siString actual_str = *str;

	char* a = actual_str;
	char* b = actual_str + len - 1;
	len *= 0.5;

	while (len--) {
		si_swap(*a, *b);
		a += 1;
        b -= 1;
	}

	*str = actual_str;
}

siArray(siString) si_string_split(siAllocator* alloc, siString str, cstring separator) {
	SI_ASSERT_NOT_NULL(str);

	usize str_len = si_string_len(str);
	usize separator_len = si_cstr_len(separator);

	usize count = 0;
	usize begin_pos = 0;
	usize pos_buffer[str_len]; /* TODO(EimaMei): Possibly use regular stack alloc? */

	while (true) {
		isize pos = si_string_find_ex(str, begin_pos, str_len, separator, separator_len);
		if (pos == SI_ERROR) {
			pos_buffer[count] = pos_buffer[count - 1];
			count++;
			break;
		}
		pos_buffer[count] = pos - begin_pos;
		count++;

		begin_pos = pos + separator_len;
	}
	siArray(siString) res = (typeof(res))si_arrayMakeReserve(alloc, sizeof(*res), count);
	SI_ARRAY_HEADER(res)->len = count;

	for_range (i, 0, count) {
		res[i] = si_string_make_len(alloc, str, pos_buffer[i]);
		str += pos_buffer[i] + separator_len;
	}

	return res;
}
F_TRAITS(inline)
void si_string_clear(siString* str) {
	siString cur_str = *str;
	*cur_str = '\0';
	SI_STRING_HEADER(cur_str)->len = 0;
}

F_TRAITS(inline)
void si_string_free(siString str) {
	SI_ASSERT_NOT_NULL(str);
    si_free(SI_STRING_HEADER(str)->allocator, SI_STRING_HEADER(str));
}
void si_string_make_space_for(siString* str, usize add_len) {
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
void si_string_shrink_to_fit(siString* str) {
	SI_ASSERT_NOT_NULL(str);
    siString cur_str = *str;
    siStringHeader* header = SI_STRING_HEADER(cur_str);

	header = (siStringHeader*)si_realloc(
        header->allocator,
        header,
        sizeof(siStringHeader*) + header->capacity + 1,
        sizeof(siStringHeader*) + header->len + 1
    );
	header->capacity = header->len;
}
#endif

#if defined(SI_CHAR_IMPLEMENTATION) && !defined(SI_STRING_UNDEFINE)

F_TRAITS(inline)
char si_char_to_lower(char c) {
	if (c >= 'A' && c <= 'Z') {
		return c + 32;
	}
	return c;
}

F_TRAITS(inline)
char si_char_to_upper(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 32;
    }
	return c;
}

F_TRAITS(inline)
b32 si_char_is_space(char c) {
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}
F_TRAITS(inline)
b32 si_char_is_digit(char c) {
	return (c >= '0' && c <= '9');
}
F_TRAITS(inline)
b32 si_char_is_hex_digit(char c) {
	return (si_char_is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}
F_TRAITS(inline)
b32  si_char_is_alpha(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
F_TRAITS(inline)
b32 si_char_is_alphanumeric(char c) {
	return si_char_is_alpha(c) || si_char_is_digit(c);
}
F_TRAITS(inline)
i32 si_digit_to_int(char c) {
	return (c - '0'); /* NOTE(EimaMei): If the user inputs a non-number character, then it's their problem (like read the damn function name ffs). */
}
i32 si_hex_digit_to_int(char c) {
	if (si_char_is_digit(c)) {
		return si_digit_to_int(c);
	}
	else if (c <= 'a' && c >= 'f') {
		return c - 'a' + 10;
	}
	else if (c <= 'A' && c >= 'F') {
		return c - 'A' + 10;
	}

	return -1;
}

char* si_u64_to_cstr_ex(u64 num, char* res) {
    char* end_ptr = res + SI_NUM_MAX_DIGITS - 1;
    *end_ptr = '\0';

    do {
        *(--end_ptr) = (num % 10) + '0'; /* NOTE(EimaMei): We build the string from behind (not the start) so that we
                                                           wouldn't have to reverse the string after we build the string. */
        num /= 10;
    } while (num > 0);

    return res + (end_ptr - res);
}


u64 si_cstr_to_u64(cstring str) {
	SI_ASSERT_NOT_NULL(str);

	u64 result = 0;
	char cur;
	while ((cur = *str++)) {
		if (si_between(cur, '0', '9')) {
			result = (result * 10) + (cur - '0');
		}
		else {
			SI_ASSERT_MSG(si_between(cur, '0', '9'), "Attempted to use `si_cstr_to_u64` with a string that contains non numbers.");
		}
	}

	return result;
}
u64 si_cstr_to_u64_len(cstring str, usize len) {
	SI_ASSERT_NOT_NULL(str);

	u64 result = 0;
	while (len != 0) {
		if (si_between(*str, '0', '9')) {
			result = (result * 10) + (*str - '0');
		}
		else {
			SI_ASSERT_MSG(si_between(*str, '0', '9'), "Attempted to use `si_cstr_to_u64` with a string that contains non numbers.");
		}
        len -= 1;
        str += 1;
	}

	return result;
}

char* si_i64_to_cstr_ex(i64 number, char* res) {
    char* end_ptr = res + SI_NUM_MAX_DIGITS - 1;
    *end_ptr = '\0';

    isize is_negative = si_num_is_neg(number);
    usize num = (number ^ -is_negative) + is_negative;

    do {
        *(--end_ptr) = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    SI_STOPIF(is_negative, *(--end_ptr) = '-');

    return res + (end_ptr - res);
}
i64 si_cstr_to_i64(cstring str) {
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
			SI_ASSERT_MSG(!(cur >= '0' && cur <= '9'), "Attempted to use `si_cstr_to_i64` with a string that contains non numbers.");
		}
	}

	if (negative) {
		result = -result;
	}

	return result;
}

#endif

#if defined(SI_FILE_IMPLEMENTATION) && !defined(SI_FILE_UNDEFINE)

F_TRAITS(inline)
b32 si_path_exists(cstring path) {
    SI_ASSERT_NOT_NULL(path);
	#if defined(SI_SYSTEM_WINDOWS)
		DWORD file_attrib = GetFileAttributes(path);
		return file_attrib != INVALID_FILE_ATTRIBUTES;
	#else
		struct stat tmp;
		return (stat(path, &tmp) == SI_OKAY);
	#endif
}
isize si_path_copy(cstring existing_path, cstring new_path) {
	#if defined(SI_SYSTEM_WINDOWS)
		return (CopyFile(existing_path, new_path, true) != 0);
	#else
		isize existing_fd = open(existing_path, O_RDONLY, 0);
		isize new_fd = open(new_path, O_WRONLY | O_CREAT, 0666);

		struct stat stat_existing;
		fstat(existing_fd, &stat_existing);

		#if defined(SI_SYSTEM_UNIX)
			isize size = sendfile(new_fd, existing_fd, 0, stat_existing.st_size);
		#else
			isize size = sendfile(new_fd, existing_fd, 0, &stat_existing.st_size, NULL, 0);
		#endif

		close(new_fd);
		close(existing_fd);

		return size == stat_existing.st_size;
	#endif
}
F_TRAITS(inline)
isize si_path_move(cstring existing_path, cstring new_path) {
	#if defined(SI_SYSTEM_WINDOWS)
		return (MoveFile(existing_path, new_path) != 0);
	#else
		if (link(existing_path, new_path) == 0) {
			return unlink(existing_path);
		}
		return SI_ERROR;
	#endif
}
F_TRAITS(inline)
isize si_path_remove(cstring path) {
	#if defined(SI_SYSTEM_WINDOWS)
		isize pos = si_string_rfind_ex((siString)path, si_cstr_len(path), 0, ".");
		if (pos != SI_ERROR) {
			return (DeleteFileA(path) != 0);
		}
		else {
			return (RemoveDirectoryA(path) != 0);
		}
	#else
		return remove(path);
	#endif
}

F_TRAITS(inline)
cstring si_path_base_name(cstring path) {
	isize pos = si_string_rfind_ex((siString)path, si_cstr_len(path) - 1, 0, &SI_PATH_SEPARATOR);

	return (pos != SI_ERROR ? (path + pos + 1) : path);
}
F_TRAITS(inline)
cstring si_path_extension(cstring path) {
	isize pos = si_string_rfind_ex((siString)path, si_cstr_len(path) - 1, 0, ".");

	return (pos != SI_ERROR ? (path + pos + 1) : path);
}
F_TRAITS(inline)
siString si_path_get_fullname(siAllocator* alloc, cstring path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		char buffer[MAX_PATH];
		GetFullPathNameA(path, MAX_PATH, buffer, nil);

		return si_string_make(alloc, buffer);
	#else
		char actual_path[4096];
		realpath(path, actual_path);

		return si_string_make(alloc, actual_path);
	#endif
}

F_TRAITS(inline)
b32 si_path_is_absolute(cstring path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		return (
			si_cstr_len(path) > 2 &&
			si_char_is_alpha(path[0]) &&
			(path[1] == ':' && path[2] == SI_PATH_SEPARATOR)
		);
	#else
		return (1 < si_cstr_len(path) && path[0] == SI_PATH_SEPARATOR);
	#endif
}
F_TRAITS(inline)
b32 si_path_is_relative(cstring path) {
	return !si_path_is_absolute(path);
}


u64 si_pathLastWriteTime(cstring filename) {
#if defined (SI_PLATFORM_API_WIN32)
    ULARGE_INTEGER li = {0};
    FILETIME last_write_time = {0};
    WIN32_FILE_ATTRIBUTE_DATA data = {0};
    gbAllocator a = gb_heap_allocator();

    wchar_t *w_text = gb__alloc_utf8_to_ucs2(a, filepath, NULL);
    if (w_text == NULL) {
        return 0;
    }

    if (GetFileAttributesExW(w_text, GetFileExInfoStandard, &data)) {
        last_write_time = data.ftLastWriteTime;
    }
    gb_free(a, w_text);

    li.LowPart = last_write_time.dwLowDateTime;
    li.HighPart = last_write_time.dwHighDateTime;
    return cast(gbFileTime)li.QuadPart;
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
siFile si_fileOpenMode(cstring path, cstring mode) {
	SI_ASSERT_NOT_NULL(path);
    SI_ASSERT_PATH_EXISTS(path);
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
		0,
		f,
        path,
        si_pathLastWriteTime(path)
	};
	si_file_size_update(&result);

	return result;
}

usize si_file_size(siFile file) {
	fseek(file.ptr, 0, SEEK_END);
	usize size = ftell(file.ptr);
	rewind(file.ptr);

	return size;
}
F_TRAITS(inline)
void si_file_size_update(siFile* file) {
	usize len = si_file_size(*file);
	file->size = len;
}

F_TRAITS(inline)
rawptr si_fileRead(siAllocator* alloc, siFile file) {
	return si_fileReadAt(alloc, file, 0, file.size);
}
rawptr si_fileReadAt(siAllocator* alloc, siFile file, usize offset, usize len) {
	SI_ASSERT_NOT_NULL(file.ptr);

    rawptr buffer = si_malloc(alloc, len);

    fseek(file.ptr, offset, SEEK_SET);
    fread(buffer, len, sizeof(siByte), file.ptr);
    rewind(file.ptr);

    return buffer;
}
/*
	NOTE/TODO(EimaMei): This has a huge performance hit if the file is, lets say, 400 lines in length. That's over 400
	allocations, and even more if the file is larger! An obvious way to imrpove the performance is to allocate one huge
	memory block and then just copy over the lines with the headers. Problem is, this means the individual strings cannot
	be freed by any means, resulting in crashes. A more optional way is to just output an array of cstrings, but then
	we lose the benefits of siString. Maybe by creating siStaticArray (and by extension siStaticString), this will only
	then require one allocation.

	This also makes me realize that the old traditional heap allocating system is really, really flawed in performance,
	compared to just allocating one HUUUGE memory block (a few MBs minimum) and just writing everything there. I'll think
	about it for sure.
*/
siArray(siString) si_file_readlines(siAllocator* alloc, siFile file) {
    siAllocator* temp = si_allocatorMake(file.size * (sizeof(siString) + sizeof(siStringHeader)));

	siString buffer = (siString)si_fileRead(temp, file);
	siArray(siString) res = si_string_split(alloc, buffer, "\n");

    si_allocatorFree(temp);

	return res;
}

F_TRAITS(inline)
siError si_file_write(siFile* file, cstring content) {
	return si_file_write_at_len(file, content, si_cstr_len(content), 0);
}
F_TRAITS(inline)
siError si_file_write_len(siFile* file, cstring content, usize len) {
	return si_file_write_at_len(file, content, len, 0);
}
F_TRAITS(inline)
siError si_file_write_at(siFile* file, cstring content, usize offset) {
	return si_file_write_at_len(file, content, si_cstr_len(content), offset);
}
siError si_file_write_at_len(siFile* file, cstring content, usize content_len, usize offset) {
	SI_ASSERT_NOT_NULL(file->ptr);
	SI_ASSERT_NOT_NULL(content);

	si_file_seek(*file, offset);
	isize result = fwrite(content, content_len, 1, file->ptr);
	si_file_seek_to_end(*file);

	si_file_size_update(file);
	return result;
}
siError si_file_write_at_line(siFile* file, cstring content, usize index) {
	SI_ASSERT_NOT_NULL(file->ptr);

    siAllocator* temp = si_allocatorMake(file->size * (sizeof(siStringHeader) + sizeof(siString)));
	siArray(siString) buffer = si_file_readlines(temp, *file);
	//siString previous_line = buffer[index];
	SI_ASSERT_MSG(index < si_arrayLen(buffer), "Index is either not 0 or higher than the amount of lines in the file.");


	buffer[index] = (siString)content;
	siString new_file_content = si_arrayToSistring(buffer, "\n");

	siError err = si_file_write_len(file, new_file_content, si_string_len(new_file_content));

	/*usize i;
	for (i = 0; i < si_arrayLen(buffer); i++) {
		if (i == index) {
			continue;
		}
		si_string_free(buffer[i]);
	}
	si_string_free(previous_line);
	si_arrayFree(buffer);

	si_string_free(new_file_content);
	si_file_close(*file);*/
    si_allocatorFree(temp);

    return err;
}

F_TRAITS(inline)
isize si_file_seek(siFile file, usize offset) {
	SI_ASSERT_NOT_NULL(file.ptr);
	return fseek(file.ptr, offset, SEEK_SET);
}
F_TRAITS(inline)
isize si_file_seek_to_end(siFile file) {
	SI_ASSERT_NOT_NULL(file.ptr);
	return fseek(file.ptr, 0, SEEK_END);
}

void si_file_close(siFile file) {
	SI_STOPIF(file.ptr != nil, fclose(file.ptr));
}

#endif

#if defined(SI_THREAD_IMPLEMENTATION) && !defined(SI_THREAD_UNDEFINE)

inline static rawptr si_impl_thread_proc(rawptr arg) {
	siThread* t = (siThread*)arg;
	t->return_value = t->func.ptr(t->arg);
	t->is_running = false;

	return nil;
}

F_TRAITS(inline)
siThread si_thread_create_ex(siFunction function, rawptr arg) {
	siThread t;
	t.is_running = false;
	t.func = function;
	t.arg = arg;

	return t;
}


F_TRAITS(inline)
void si_thread_start(siThread* t) {
	si_thread_start_stack(t, 0);
}

void si_thread_start_stack(siThread* t, usize stack_size) {
	#if defined(SI_SYSTEM_WINDOWS)
		t->id = CreateThread(nil, stack_size, (LPTHREAD_START_ROUTINE)si_impl_thread_proc, t, 0, nil);
		t->is_running = true;
		SI_ASSERT_MSG(t->id != nil, "Something went wrong with creating a new thread.");
	#else
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_attr_setstacksize(&attr, stack_size);
		t->is_running = true;

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

void si_thread_join(siThread* t) {
	t->is_running = true;

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

	t->is_running = false;
}
void si_thread_cancel(siThread* t) {
	#if defined(SI_SYSTEM_WINDOWS)
		print("si_thread_cancel: This feature on Windows is not supported as of now.");
		SI_UNUSED(t);
	#else

		usize error_code = pthread_cancel(t->id);
		t->is_running = false;

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
void si_thread_destroy(siThread* t) {
	si_thread_join(t);
}

void si_thread_set_priority(siThread t, i32 priority) {
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
		SI_PANIC_MSG("si_thread_set_priority: Not supported on MacOS.");
		SI_UNUSED(t);
		SI_UNUSED(priority);
	#endif
}
#endif

#if defined(SI_BIT_IMPLEMENTATION) && !defined(SI_BIT_UNDEFINE)

F_TRAITS(inline)
usize si_num_bits_u8(u8 num) {
    return (num * 01001001001ULL & 042104210421ULL) % 017;
}

F_TRAITS(inline)
usize si_num_bits_u32(u32 num) {
     num -= ((num >> 1) & 0x55555555);
     num = (num & 0x33333333) + ((num >> 2) & 0x33333333);
     num = (num + (num >> 4)) & 0x0F0F0F0F;
     return (num * 0x01010101) >> 24;
}

F_TRAITS(inline)
usize si_num_bits_u64(u64 num) {
    num = num - ((num >> 1) & 0x5555555555555555);
    num = (num & 0x3333333333333333) + ((num >> 2) & 0x3333333333333333);
    num = (num + (num >> 4)) & 0xF0F0F0F0F0F0F0F;
    return (num * 0x101010101010101) >> 56;
}

F_TRAITS(inline)
usize si_num_leading_bit_ex(u64 num, usize total_bits, siBitType bit) {
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
usize si_num_trailing_bit_ex(u64 num, usize total_bits, siBitType bit) {
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
u64 si_num_rotate_left_ex(u64 num, usize total_bits, usize bits) {
    return (num << bits) | (num >> (total_bits - bits));
}
F_TRAITS(inline)
u64 si_num_rotate_right_ex(u64 num, usize total_bits, usize bits) {
    return (num >> bits) | (num << (total_bits - bits));
}

F_TRAITS(inline)
u64 si_num_reverse_bits_ex(u64 num, usize total_bits) {
    u64 res = 0;

    for_range(i, 0, total_bits) {
        res <<= 1;
        res |= (num & 1);
        num >>= 1;
    }

    return res;
}

inline siArray(u8) si_num_to_bytes_ex(siAllocator* alloc, u64 num, usize num_sizeof) {
    siArray(u8) res = (u8*)si_arrayMakeReserve(alloc, sizeof(*res), num_sizeof);
    SI_ARRAY_HEADER(res)->len = num_sizeof;

    for_range (i, 0, num_sizeof) {
        res[i] = (num >> ((num_sizeof - 1 - i) * 8)) & 255;
    }

    return res;
}
F_TRAITS(inline)
u64 si_bytes_to_num(siArray(u8) bytes) {
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
u64 si_num_change_endian_ex(u64 num, usize num_sizeof) {
    usize result = 0;

    for_range (i, 0, num_sizeof) {
        result |= ((num >> SI_BYTE_TO_BIT(i)) & 255) < SI_BYTE_TO_BIT(num_sizeof - 1 - i);
    }

    return result;
}

u64 si_num_pow(u64 base, usize exp) {
    usize result = 1;

    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }

    return result;
}

F_TRAITS(inline)
u64 si_num_pow2(u64 base, usize exp) {
    return exp << base;
}

F_TRAITS(inline)
usize si_num_len(u64 num) {
    usize count = 0;

    do {
        count += 1;
        num /= 10;
    } while (num > 0);

    return count;
}

F_TRAITS(inline)
u64 si_num_round_nearest_multiple(u64 num, usize multiple) {
    return ((num + multiple / 2) / multiple) * multiple;
}

#endif

#if defined(SI_CPU_IMPLEMENTATION) && !defined(SI_CPU_UNDEFINE)

F_TRAITS(inline)
siMhz si_cpu_clock_speed(void) {
	/* TODO(EimaMei): Possibly add a better version for ARM processors. */
    u64 begin = si_RDTSC();
    si_sleep(100);
    u64 end = si_RDTSC();

    return si_num_round_nearest_multiple((end - begin) / 100000, 10);
}

#endif

#if defined(SI_MEMORY_LOGGING_IMPLEMENTATION) && !defined(SI_MEMORY_LOGGING_UNDEFINE)

typedef struct siDebugGlobalVars {
	usize allocs;
	usize frees;
	usize total;
	usize overall;
	siArray(siDebugHeader*) list;
} siDebugGlobalVars;
siDebugGlobalVars si_debug = {0, 0, 0, 0, nil};

#if defined(malloc) /* NOTE(EimaMei): We undef the sili defined macros so that we don't get an infinite loop, where 'malloc' (under the hood si_debug_alloc) calls 'malloc' (under the hood si_debug_alloc) and continue that for the rest of our lives. */
	#undef malloc
	#undef calloc
	#undef realloc
	#undef si_realloc
	#undef free
#endif

rawptr si_debug_alloc(usize function_id, rawptr ptr, siDebugArgs args, cstring file, i32 line, cstring function, cstring time, cstring date) {
	siByte* res = nil;
	/*NOTE(EimaMei): args.first - old size ; args.second - new size */

	if (si_debug.list == nil) {
		siByte* ptr = malloc(sizeof(siArrayHeader) + (sizeof(siDebugHeader*) * 100));
		si_debug.list = (rawptr)(ptr + sizeof(siArrayHeader));

		siArrayHeader* header = (siArrayHeader*)ptr;
		header->len = 0;
		header->capacity = 100;
		header->typeSize = sizeof(siDebugHeader*);
	}

	usize size = 0;
	cstring type = nil;
	switch (function_id) {
		case 1: { /* malloc*/
			res = malloc(sizeof(siDebugHeader) + args.first);
			size = args.first;
			type = "malloc";
			break;
		}
		case 2: { /* calloc*/
			res = calloc(args.second, sizeof(siDebugHeader) + args.first);
			size = args.first * args.second;
			type = "calloc";
			break;
		}
		case 3: { /* realloc */
			siDebugHeader* header = SI_DEBUG_HEADER(ptr);
			si_debug.allocs += 1;
			si_debug.frees += 1;

			res = realloc(header, sizeof(siDebugHeader) + size);

			header->type = "realloc";
			header->len = args.second;
			header->state = 2;
			header->date = date;
			header->time = time;
			header->file = file;
			header->line = line;
			header->function = function;

			return res + sizeof(siDebugHeader);
		}
		case 4: { /* si_realloc_cpy */
			if (ptr == SI_ARRAY_HEADER(si_debug.list)) {
				res = si_realloc_cpy(ptr, args.first, args.second);
				return res;
			}

			siDebugHeader* header = SI_DEBUG_HEADER(ptr);
			si_debug.allocs += 1;
			si_debug.frees += 1;

			res = si_realloc_cpy(header, sizeof(siDebugHeader) + args.first, sizeof(siDebugHeader) + args.second);

			header->type = "si_realloc_cpy";
			header->len = args.second;
			header->state = 2;
			header->date = date;
			header->time = time;
			header->file = file;
			header->line = line;
			header->function = function;

			return res + sizeof(siDebugHeader);
		}
		default: SI_BUILTIN_UNREACHABLE();
	}

	if (res == nil) {
		siString msg;
		if (errno == ENOMEM) {
			msg = si_string_make_fmt("%s %s at '%s:%i'[%s]: Failed to malloc because the system is out of memory (errno ENOMEM).", date, time, file, line, function);
		}
		else {
			msg = si_string_make_fmt("%s %s at '%s:%i'[%s]: Failed to malloc because of an unknown reason (errno '%zd').", date, time, file, line, function, errno);
		}
		SI_ASSERT_MSG(res != NULL, msg);
	}

	siDebugHeader* header = (siDebugHeader*)res;
	header->type = type;
	header->len = size;
	header->state = 0;
	header->date = date;
	header->time = time;
	header->file = file;
	header->line = line;
	header->function = function;
	header->index = si_arrayLen(si_debug.list);

	si_debug.allocs += 1;
	si_debug.total += size;
	si_debug.overall += size;

	si_arrayAppend(&si_debug.list, header);

	return res + sizeof(siDebugHeader);
}
void si_debug_free(rawptr ptr) {
	SI_ASSERT_NOT_NULL(ptr);

	siDebugHeader* header = SI_DEBUG_HEADER(ptr);
	siDebugHeader copy = *header;

	si_debug.frees += 1;
	si_debug.overall -= header->len;

	free(header);

	siDebugHeader* new_header = malloc(sizeof(siDebugHeader));
	*new_header = copy;
	new_header->state = 1;

	si_debug.list[new_header->index] = new_header;
}

void si_debug_print_all(void) {
	if (si_debug.list == nil) {
		return ;
	}

	for_each (x, si_debug.list) {
		siDebugHeader* header = *x;

		if (header == nil) {
			continue;
		}
		cstring msg = nil;
		switch (header->state) {
			case 0: msg = "NOT FREED"; break;
			case 1: msg = "FREED"; break;
			case 2: msg = "REALLOCED"; break;
			default: SI_BUILTIN_UNREACHABLE();
		}

		printf(
			"%d: [%p]: %s %s at '%s:%i'[%s]: %s: %s ('%zd' bytes).\n",
			header->index,
			(rawptr)((siByte*)header + sizeof(siDebugHeader)),
			header->date, header->time,
			header->file, header->line,
			header->function,
			header->type,
			msg,
			header->len
		);
	}
}

void si_debug_print_allocations(void) {
	printf(SI_PERFORMANCE_MSG_DEBUG, si_debug.allocs, si_debug.frees, si_debug.total, si_debug.overall);
}

void si_debug_cleanup(void) {
	if (si_debug.list == nil) {
		return ;
	}

	for_each (x, si_debug.list) {
		free(*x);
	}

	free(SI_ARRAY_HEADER(si_debug.list));

	si_debug = (siDebugGlobalVars){0, 0, 0, 0, nil};
}

#define malloc(size) si_debug_alloc(1, nil, (siDebugArgs){size, 0}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define calloc(num, size) si_debug_alloc(2, nil, (siDebugArgs){size, num}, num, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define realloc(ptr, newSize) si_debug_alloc(3, ptr, (siDebugArgs){0, newSize}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define si_realloc_cpy(ptr, oldSize, newSize) si_debug_alloc(4, ptr, (siDebugArgs){oldSize, newSize}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define free(ptr) si_debug_free(ptr)

#endif

#if defined(SI_PERFORMANCE_IMPLEMENTATION) && !defined(SI_PERFORMANCE_UNDEFINE)

F_TRAITS(inline)
void si_impl_performance_thread(u32* arg) {
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
