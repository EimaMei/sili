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

LICENSE
	- This software is dual-licensed to the public domain and under the MIT
	license (see the LICENSEs in the bottom of the file).

WARNING
	- This library is _slightly_ experimental and features may not work as
	expected.
	- This also means that many functions are not documented.

CREDITS
	- Ginger Bill's 'gb.h' for inspiring me to program the Sili Toolchain, as
	well as certain features taken from the library and implemented here.
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

#define SI_STATIC_ASSERT2(cond, msg)  typedef char static_assertion_##msg[(!!(cond)) * 2 - 1] /* Note(EimaMei): This is absolutely stupid but somehow it works so who cares, really? */
#define SI_STATIC_ASSERT1(cond, line) SI_STATIC_ASSERT2(cond, line)

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

#define SI_BIG_ENDIAN      (!*(u8*)&(u16){1})
#define SI_LITTLE_ENDIAN   (!SI_BIG_ENDIAN)


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
	typedef size_t      usize; /* NOTE(EimaMei): Use this as the default int type! */
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

#define SI_F32_MIN 1.17549435e-38f
#define SI_F32_MAX 3.40282347e+38f

#define SI_F64_MIN 2.2250738585072014e-308
#define SI_F64_MAX 1.7976931348623157e+308


#if !defined(b8)
    typedef u8   b8; /* NOTE(EimaMei): Don't use this. Please. :D */
    typedef u16 b16;
    typedef u32 b32;
    typedef u64 b64;
#endif

#ifdef bool
    #undef bool
    typedef usize bool;
#endif


#if !defined(SI_LANGUAGE_CPP) && SI_STANDARD_VERSION <= SI_STANDARD_C17
    #ifndef bool
        typedef usize bool;
    #endif
    #ifndef true
        #define true 1
    #endif
    #ifndef false
        #define false 0
    #endif
#else
    #define bool usize
#endif


SI_STATIC_ASSERT(sizeof(b8) == sizeof(u8));
SI_STATIC_ASSERT(sizeof(b16) == sizeof(u16));
SI_STATIC_ASSERT(sizeof(b32) == sizeof(u32));
SI_STATIC_ASSERT(sizeof(b64) == sizeof(u64));
SI_STATIC_ASSERT(sizeof(bool) == sizeof(usize)); /* NOTE(EimaMei): Make it consistent for all our sake. */

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
#define intern static

#if defined(SI_COMPILER_MSVC)
    #define si_asm(...) __asm  { __VA_ARGS__ }
#else
    #if defined(SI_CPU_X86)
        #define si_asm(asm, ...) /* NOTE(EimaMei): Personally speaking, AT&T syntax sucks. Plus this ensures sili.h will compile, even if the AT&T syntax is enabled. */ \
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
        #define si_asm(asm, ...) __asm__ __volatile__(asm __VA_ARGS__)
    #endif
#endif



#if (defined(SI_LANGUAGE_CPP) && (SI_STANDARD_VERSION >= SI_STANDARD_CPP17)) || (!defined (SI_LANGUAGE_CPP) && SI_STANDARD_VERSION >  SI_STANDARD_C17)
    #define fallthrough [[fallthrough]]
#elif defined(SI_STANDARD_ANSI) || defined(SI_COMPILER_MSVC)
    #define fallthrough do {} while (0) /* NOTE(EimaMei): This does nothing, it's just syntax sugar. */
#else
    #define fallthrough __attribute__((fallthrough)) /* NOTE(EimaMei): Properly notifies the compiler that this IS a fallthrough. */
#endif


#if defined(SI_LANGUAGE_C) && (!defined(SI_STANDARD_ANSI) &&  !defined(SI_COMPILER_MSVC))
    #define auto __auto_type /* NOTE(EimaMei): Wouldn't recommend to use (not portable and generally unclear). */
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
	typedef void* rawptr;
#endif

#if !defined(cstring)
	typedef const char* cstring;
#endif

typedef isize siError;

/*
	========================
	| Constants            |
	========================
*/
#define SI_OKAY  0
#define SI_ERROR -1

#define SI_KILO(x) (       (x) * (usize)1024)
#define SI_MEGA(x) (SI_KILO(x) * (usize)1024)
#define SI_GIGA(x) (SI_MEGA(x) * (usize)1024)
#define SI_TERA(x) (SI_GIGA(x) * (usize)1024)

#if !defined(nil)
	#define nil NULL
#endif

SI_STATIC_ASSERT(sizeof(nil) == sizeof(NULL));

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
    #define SI_BUILTSI_BUILTIN_UNREACHABLE() 0
#endif

#if __has_builtin(__builtin_expect)
    #define SI_BUILTIN_EXPECT(exp, c) __builtin_expect(exp, c)
#else
    #define SI_BUILTINT_EXPECT(exp, c) (exp)
#endif

/*
	========================
	| Declaration macros   |
	========================
*/
#define SI_ENUM(type, name) type name; enum
#define SI_FUNC_PTR(var_name, multiple_args) (*var_name)multiple_args
#define SI_UNUSED(x) (void)(x) /* NOTE(EimaMei) Should be used when you want to supress "parameter 'X' set but not used" errors. */

/*
	========================
	| Macro related        |
	========================
*/
#define SI_CAT(a, b) a##b
#define SI_MULTILINE(...) __VA_ARGS__
#define SI_STR_MULTILINE(...) #__VA_ARGS__

/*
	========================
	| Casting              |
	========================
*/
#define si_cast(type, value) ((type)((value)))
#define si_transmute(type, value) ((union { typeof(value) in; type out; }){value}.out)

/*
	========================
	| Unary operators      |
	========================
*/
#if !defined(typeof) && !(defined(SI_LANGUAGE_C) && SI_STANDARD_VERSION > SI_STANDARD_C17)
	#define typeof(...)  __typeof__(__VA_ARGS__)
#endif

#if !defined(countof)
	#define countof(value) ((usize)sizeof(value) / (usize)sizeof(value[0]))
#endif

#if !defined(offsetof)
	#define offsetof(type, element) ((isize)&(((type*)nil)->element))
#endif

#if (!defined(SI_LANGUAGE_CPP) && !defined(alignof)) || (defined(SI_LANGUAGE_CPP) && SI_STANDARD_VERSION < SI_STANDARD_CPP11)
	#define alignof(type) offsetof(struct { char c; type member; }, member)
#endif

#define siBitInt(x) typeof(siByte[si_cast(usize, x / 8)])
#define siByteInt(x) typeof(siByte[x]) /* NOTE(EimaMei): Use this. */

/*
	========================
	| General functions    |
	========================
*/

#define print(msg) puts(msg)

#define si_stack_alloc(bytes) si_cast(rawptr, (siByte[bytes]){0})
#define si_malloc_item(allocator, type) (type*)si_malloc(allocator, sizeof(type))
#define si_malloc_array(allocator, type, count) (type*)si_malloc(allocator, sizeof(type) * (count))

#define si_buf(type, ...) (type[]){__VA_ARGS__}

#define likely(x)     SI_BUILTIN_EXPECT(!!(x), true)
#define unlikely(x)   SI_BUILTIN_EXPECT(!!(x), false)

#define si_swap(a, b) do { typeof((a)) tmp = (a); (a) = (b); (b) = tmp; } while (0)
#define si_abs(x) ((x) < 0 ? -(x) : (x))
#define si_between(x, lower, upper) (((lower) <= (x)) && ((x) <= (upper)))
#define si_pause() do { printf("Press any key to continue...\n"); getchar(); } while(0)

#define for_range_2(count_var, struct_name, start, end) \
	siISVector2D struct_name = {(isize)start, (isize)end}; \
	for (isize count_var = struct_name.x; count_var < struct_name.y; count_var += 1)
#define for_range_1(count_var, line, start, end) \
	for_range_2(count_var, SI_CAT(__si_impl_range_, line), start, end)
#define for_range(count_var, start, end) \
	for_range_1(count_var, __LINE__, start, end)



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
usize si_impl_assert_msg(bool condition, cstring condition_str, cstring file, i32 line, cstring func, cstring message, ...);

#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
#define SI_ASSERT_MSG(condition, message) si_impl_assert_msg(condition, #condition, __FILE__, __LINE__, __func__, message, "")
#define SI_ASSERT_FMT(condition, message, ...) si_impl_assert_msg(condition, #condition, __FILE__, __LINE__, __func__, message, __VA_ARGS__)
#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL.")

#define SI_PANIC() si_impl_assert_msg(false, "SI_PANIC()", __FILE__, __LINE__, __func__, nil)
#define SI_PANIC_MSG(message) si_impl_assert_msg(false, "SI_PANIC()", __FILE__, __LINE__, __func__, message, "")

#define SI_STOPIF(assertion, error_action) if (assertion) { error_action; } do {} while(0)

/*
	========================
	| siAny                |
	========================
*/

/*
 * IMPORTANT NOTE(EimaMei):
 * 'si_any_make_static' and 'si_any_make_static_static' are very different in use case!
 *
 * 'si_any_make_static' copies the data into the allocator, while 'si_any_make_static_static'
 * only gets the pointer address of the given value.
 *
 * All this means is that if you're planning to return a 'siAny', then you
 * __have__ to use `si_any_make_static` so that the value doesn't get deleted after
 * the function returns.
 *
 * 'si_any_make_static_static' should usually only be used when there's a 'siAny'
 * function argument input, OR when the return data is static.
*/

typedef struct siAny {
	rawptr ptr;
	usize type_size;
} siAny;


#define si_any_make(allocator, ...) \
    (siAny){ \
        si_any_copy( \
            allocator, \
            (rawptr)&((struct { typeof(__VA_ARGS__) in; }){__VA_ARGS__}.in), \
            sizeof(typeof(__VA_ARGS__)) \
        ), \
        sizeof(typeof(__VA_ARGS__)) \
    }

#define si_any_make_static(...) \
    (siAny){ \
        (rawptr)&((struct { typeof(__VA_ARGS__) in; }){__VA_ARGS__}.in), \
        sizeof(typeof(__VA_ARGS__)) \
    }

#define si_any_get(type, any) \
    ((any.ptr != nil) ? \
     *si_cast(type*, si_cast(siByte (*)[any.type_size], any.ptr)) :  \
     *(type*)SI_ASSERT_NOT_NULL(any.ptr))

rawptr si_any_copy(rawptr alloc, rawptr content, usize size);


/*
	========================
	| siGeneric/siFatPtr   |
	========================
*/

typedef struct siGenericHeader {
    usize len;
} siGenericHeader;

#define SI_GENERIC_HEADER(x) ((siGenericHeader*)x - 1)
#define siGenericFat(type) type


/* NOTE(EimaMei): You're responsible for filling out the header, not sili. tl;dr eat shit, faggots */
#define si_fat_ptr_make(header_type, .../*CONTENT*/)  \
    memcpy( \
        si_fat_ptr_make_reserve(header_type, typeof(__VA_ARGS__), 1), \
        &(typeof(__VA_ARGS__)){__VA_ARGS__}, \
        sizeof(typeof(__VA_ARGS__)) \
    )

#define si_fat_ptr_make_reserve(header_type, type, count)  \
    ((siByte*)si_stack_alloc((sizeof(type) * count) + sizeof(header_type)) + sizeof(header_type))


/*
	========================
	| siFunction           |
	========================
*/
typedef struct siFunction {
   rawptr SI_FUNC_PTR(ptr, (rawptr));
} siFunction;

#define siFunc(func) (siFunction){(rawptr (*)(rawptr))func}

/*
	========================
	| Other                |
	========================
*/
typedef struct { i32 x, y; } siIVector2D;
typedef struct { u32 x, y; } siUVector2D;
typedef struct { isize x, y; } siISVector2D;
typedef siIVector2D siVector2D;

typedef struct { u8 r, g, b, a; } siColor;
#define SI_RGBA(r, g, b, a) ((siColor){r, g, b, a})
#define SI_RGB(r, g, b) ((siColor){r, g, b, 255})


typedef SI_ENUM(usize, siDirection) {
	SI_DIRECTION_LEFT,
	SI_DIRECTION_RIGHT
};


void si_ptr_move_by(rawptr src, usize src_len, usize move_by, siDirection direction);

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
typedef struct siAllocator {
	siByte* ptr;
	usize max_size;
	usize index;
} siAllocator;



siAllocator* si_allocator_make(usize bytes);
#define si_allocator_make_stack(bytes) si_allocator_make_stack_ex(bytes, si_stack_alloc(bytes + sizeof(siAllocator)))
siAllocator* si_allocator_make_stack_ex(usize bytes, rawptr res);
void si_allocator_refresh(siAllocator* alloc);
void si_allocator_free(siAllocator* alloc);

rawptr si_malloc(siAllocator* alloc, usize bytes);
rawptr si_calloc(siAllocator* alloc, usize num, usize bytes);
rawptr si_realloc(siAllocator* alloc, rawptr ptr, usize old_size, usize new_size);
void si_free(siAllocator* alloc, rawptr ptr);

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

#define siPair(type1, type2) struct { type1 first; type2 second; }
#define si_pair_make(first_value, second_value) {first_value, second_value }
#define si_pair_copy(pair) si_pair_make(pair.first, pair.second)

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
 * IMPORTANT NOTE(EimaMei):
 * 'si_optional_make' and 'si_optional_make_static' are very different in use case!
 *
 * 'si_optional_make' copies the data into the allocator, while 'si_optional_make_static'
 * only gets the pointer address of the given value.
 *
 * All this means is that if you're planning to return a 'siOptional', then you
 * __have__ to use `si_optional_make` so that the value doesn't get deleted after
 * the function returns.
 *
 * 'si_optional_make_static' should usually only be used when there's a 'siOptionalType'
 * function argument input, OR when the return data is static.
 *
 * For examples of ways to utilize 'siOptional', refer to the 'examples/optional.c'
 * file.
*/

typedef struct siOptionalType {
	siAny value;
	bool has_value;
} siOptionalType;

#define siOptional(type) siOptionalType
const siOptionalType SI_OPTIONAL_NULL = {{nil, 0}, false};

#define si_optional_make(...) (siOptionalType){si_any_make(__VA_ARGS__), true}
#define si_optional_make_static(...) (siOptionalType){si_any_make_static(__VA_ARGS__), true}

#define si_optional_ptr(optional_var) (optional_var.value.ptr)
#define si_optional_type_size(optional_var) (optional_var.value.type_size)

#define si_optional_get(type, optional_var) \
    si_any_get(type, optional_var.value)
#define si_optional_get_or_default(type, optional_var, default_value) \
    (optional_var.has_value ? si_optional_get(type, optional_var) : (type)(default_value))

#define si_optional_reset(optional_var) \
    do { \
        siOptionalType si_new = SI_OPTIONAL_NULL;\
        si_new.value.type_size = optional_var.value.type_size; \
        \
        optional_var = si_new; \
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

typedef struct siArrayHeader {
	usize len;
	usize capacity;
    siAllocator* allocator;
	u16 type_size;
	/* u16 grow; */
} siArrayHeader;

#define siArray(type) type*
#define SI_ARRAY_HEADER(array) ((siArrayHeader*)array - 1)

/**
 * @def for_each(variable_name, array)
 *
 * @brief Macro to iterate over elements of a siString.
 *
 * @param variable_name The name of the iterator variable.
 * @param array The siString to iterate over.
 */
#if !defined(for_each)
	#if defined(SI_LANGUAGE_C) && SI_STANDARD_VERSION > SI_STANDARD_C89
		#define for_each(variable_name, array) for (typeof(array) variable_name = array; variable_name != (typeof(array))si_array_get_ptr(array, si_array_len(array)); variable_name += 1)
	#else
		#define for_each(variable_name, array) typeof(array) variable_name; for (variable_name = array; variable_name != (typeof(array))si_array_get_ptr(array, si_array_len(array)); variable_name += 1)
	#endif
#endif

#define si_array_get_ptr(array, index) \
	(si_cast(siByte*, array) + si_array_type_size(array) * (index))

/* rawptr si_array_make(rawptr array); */
#define si_array_make(allocator, ...) \
    si_array_make_list(allocator, __VA_ARGS__, sizeof(typeof(*(__VA_ARGS__))), countof((__VA_ARGS__)))
rawptr si_array_make_list(siAllocator* alloc, rawptr list, usize sizeof_element, usize count);
rawptr si_array_copy(siAllocator* alloc, rawptr array);
rawptr si_array_make_reserve(siAllocator* alloc, usize sizeof_element, usize count);

usize si_array_len(rawptr array);
usize si_array_capacity(rawptr array);
usize si_array_type_size(rawptr array);
usize si_array_total_size(rawptr array);
siAllocator* si_array_allocator(rawptr array);
bool si_array_empty(rawptr array);

siAny si_array_at(rawptr array, usize index);
siAny si_array_front(rawptr array);
siAny si_array_back(rawptr array);

#define si_array_append(array_ptr, ...) si_impl_array_append(array_ptr, si_any_make_static(__VA_ARGS__))
#define si_array_push(array_ptr, ...) si_array_append(array_ptr, __VA_ARGS__)
void si_array_pop(rawptr array_ptr);
#define si_array_insert(array_ptr, new_item, index) si_impl_array_insert(array_ptr, si_any_make_static(new_item), index)
void si_array_erase(rawptr array_ptr, usize index);
void si_array_erase_count(rawptr array_ptr, usize index, usize count);
#define si_array_remove_item(array_ptr, ...) si_impl_array_remove_item(array_ptr, si_any_make_static(__VA_ARGS__))
#define si_array_fill(array_ptr, index, count, ...) si_impl_array_fill(array_ptr, index, count, si_any_make_static(__VA_ARGS__))

#define si_array_find(array, ...) si_impl_array_find(array, 0, si_array_len(array), si_any_make_static(__VA_ARGS__))
#define si_array_find_ex(array, start, end, ...) si_impl_array_find(array, start, end, si_any_make_static(__VA_ARGS__))
#define si_array_rfind(array, ...) si_impl_array_rfind(array, si_array_len(array) - 1, 0, si_any_make_static(__VA_ARGS__))
#define si_array_rfind_ex(array, start, end, ...) si_impl_array_rfind(array, start, end, si_any_make_static(__VA_ARGS__))

#define si_array_replace(array_ptr, old_value, ...) si_impl_array_replace(array, si_any_make_static(old_value), si_any_make_static(__VA_ARGS__))
void si_array_reverse(rawptr array_ptr);
char* si_array_to_sistring(siArray(char*) array, cstring separator);

u64 si_array_sum_u64(siArray(void) array);
i64 si_array_sum_i64(siArray(void) array);
f64 si_array_sum_f64(siArray(void) array);

void si_array_clear(rawptr array_ptr);
bool si_array_equal(rawptr lha, rawptr rha);

void si_array_free(rawptr array);
void si_array_shrink_to_fit(rawptr array_ptr);

/* NOTE(EimaMei): The actual implementations. Shouldn't be used in practice, really. */
isize si_impl_array_find(rawptr array, usize start, usize end, siAny value);
isize si_impl_array_rfind(rawptr array, usize start, usize end, siAny value);
void si_impl_array_append(rawptr array_ptr, siAny value);
void si_impl_array_insert(rawptr array_ptr, siAny value, usize index);
void si_impl_array_replace(rawptr array_ptr, siAny old_value, siAny new_value);
void si_impl_array_remove_item(rawptr array_ptr, siAny item);
void si_impl_array_fill(rawptr array_ptr, usize index, usize count, siAny item);

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
	/* u8 type_size; */
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
bool si_string_empty(siString str);



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
    si_string_sub_ex(str, begin, len, si_stack_alloc(len + 1))
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
void si_string_insert_ex(siString* str, cstring cstr, usize cstr_len, usize index, bool erase_index);

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

bool  si_char_is_space(char c);
bool  si_char_is_digit(char c);
bool  si_char_is_hex_digit(char c);
bool  si_char_is_alpha(char c);
bool  si_char_is_alphanumeric(char c);
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

bool si_cstr_equal(cstring str1, cstring str2);
bool si_cstr_equal_len(cstring str1, usize str1_len, cstring str2, usize str2_len);

u64 si_cstr_to_u64(cstring str);
i64 si_cstr_to_i64(cstring str);
f64 si_cstr_to_f64(cstring str);

#define si_u64_to_cstr(num) si_u64_to_cstr_ex(num, si_stack_alloc(SI_NUM_MAX_DIGITS + 1))
char* si_u64_to_cstr_ex(u64 num, char* res);
#define si_i64_to_cstr(num) si_i64_to_cstr_ex(num, si_stack_alloc(SI_NUM_MAX_DIGITS + 1))
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
bool si_path_exists(cstring path);
isize si_path_copy(cstring existing_path, cstring new_path);
isize si_path_move(cstring existing_path, cstring new_path);
isize si_path_remove(cstring path);

cstring si_path_base_name(cstring path);
cstring si_path_extension(cstring path);
siString si_path_get_fullname(siAllocator* alloc, cstring path);

bool si_path_is_absolute(cstring path);
bool si_path_is_relative(cstring path);

/*
	========================
	|  siFile              |
	========================
*/
siFile si_file_create(cstring path);
siFile si_file_open(cstring path);
siFile si_file_open_mode(cstring path, const char* mode);

usize si_file_size(siFile file);
void si_file_size_update(siFile* file);

siArray(void) si_file_read(siAllocator* alloc, siFile file);
siArray(void) si_file_read_at(siAllocator* alloc, siFile file, usize index, usize len);
siArray(siString) si_file_readlines(siAllocator* alloc, siFile file);

siError si_file_write(siFile* file, cstring content);
siError si_file_write_len(siFile* file, cstring content, usize len);
siError si_file_write_at(siFile* file, cstring content, usize offset);
siError si_file_write_at_line(siFile* file, cstring content, usize index);
siError si_file_write_at_len(siFile* file, cstring content, usize content_len, usize offset);

isize si_file_seek(siFile file, usize offset);
isize si_file_seek_to_end(siFile file);

void si_file_close(siFile file);

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
	volatile bool is_running;
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
	#elif defined(SI_TIME_FIRST_WEEKDAY_FRIDAY) /* NOTE(EimaMei): WTF MALDIVES???? */
		Saturday,
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday
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
/* TODO(EimaMei): I am pretty sure 'si_RDTSC()' arm64 is wrong, but I can't tell. Docker is giving some weird results.*/
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

#define SI_NUM_GET_BIT(num, x) ((num & SI_BIT(x)) != 0)
#define SI_BYTE_TO_BIT(bits) ((bits) * (usize)8)

#define SI_HIGH_BITS(value) ( \
    (value & ((typeof(value))0xFF << SI_BYTE_TO_BIT(sizeof(typeof(value)) - 1))) \
    >> \
    SI_BYTE_TO_BIT(sizeof(typeof(value)) - 1))

#define SI_LOW_BITS(value) (value & ((typeof(value))0xFF))

#define SI_BIT_GET(variable, flag) (((variable) & (flag)) != SI_BIT_ZERO)
#define SI_BIT_SET(variable, flag, condition) \
    (condition) ? ((variable) |= (flag)) : ((variable) &= ~(flag))


/* NOTE(EimaMei): In practice: Lets say we have array: (u16){0x00FF, 0xAAAA, 0x0008}.
 * We want the first value so we do: si_u64_from_ptr(array, sizeof(u16)).
 * The macro first casts it as an u64* and dereference (which grabs 8 bytes).
 * The current result is: 0x0008AAAA00FF
 * However since the answer is in the low bit, we can bitwise AND the low bit and get the answer.
 * Since u16 has 2 bytes, we bitwise AND it by 16 bits (2 * 8) and get the answer.
 * 0x0008AAAA00FF -> 0xFF
 */
#define si_u64_from_ptr(ptr, int_sizeof) \
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

static const siPair(u64, cstring) si_perf_limit[] = {
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

    f64 cycles_median = (f64)si_array_sum_u64(cycles) / si_array_len(cycles);
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
    #define SI_GRN "\33[0;32m"
    #define SI_RED "\33[0;31m"
    #define SI_END "\33[0m"
#else
    #define SI_RED ""
    #define SI_GRN ""
    #define SI_END ""
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

        bool zero_is_slower = (time_0 > time_1);
        f64 ratio = zero_is_slower ? (time_0 / time_1) : (time_1 / time_0);
        cstring clr_0 = zero_is_slower ? SI_RED : SI_GRN,
                clr_1 = zero_is_slower ? SI_GRN : SI_RED;

        element_0 = si_performance_limit_loop(time_0);
        element_1 = si_performance_limit_loop(time_1);
        time_0 /= element_0->first;
        time_1 /= element_1->first;

        siPerformanceMSG* msg = si_performance_get_msg_vars(alloc, end, median_index);
        printf(
            "\t%s%zu %s - %s%9.4f %s%s vs %s%9.4f %s%s (%4.4f ratio, %"PRIu64" vs %"PRIu64" cycles)\n",
            msg->space_pad, median_index, msg->runs_text,
            clr_0, time_0, element_0->second, SI_END, clr_1, time_1, element_1->second, SI_END,
            ratio, cycles_0, cycles_1
        );

        array_index += 1;
    }

    f64 cycles_median_0 = (f64)si_array_sum_u64(cycles[0]) / si_array_len(cycles[0]),
        cycles_median_1 = (f64)si_array_sum_u64(cycles[1]) / si_array_len(cycles[1]);
    f64 time_median_0 = cycles_median_0 / ghz_freq,
        time_median_1 = cycles_median_1 / ghz_freq;

    bool zero_is_slower = (time_median_0 > time_median_1);
    f64 ratio = zero_is_slower ? (time_median_0 / time_median_1) : (time_median_1 / time_median_0);
    cstring clr_0 = zero_is_slower ? SI_RED : SI_GRN,
            clr_1 = zero_is_slower ? SI_GRN : SI_RED;

    element_0 = si_performance_limit_loop(time_median_0);
    element_1 = si_performance_limit_loop(time_median_1);
    time_median_0 /= element_0->first;
    time_median_1 /= element_1->first;

    printf(
        "Final result:\n"
            "\tTime average   - %s%9.4f %s%s vs %s%9.4f %s%s (%4.4f ratio)\n"
            "\tCycles average - %"PRIu64" cycles vs %"PRIu64" cycles\n",
        clr_0, time_median_0, element_0->second, SI_END,
        clr_1, time_median_1, element_1->second, SI_END,
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
        siAllocator* alloc = si_allocator_make_stack(4096); \
		siArray(u64) cycles = si_array_make_reserve(alloc, sizeof(u64), 20); \
        si_performance_loop(function, cycles, start, end); \
        \
        si_performance_loops_average_print(alloc, #function, start, end, cycles, (f64)si_cpu_clock_speed() / 1000); \
		SI_PERFORMANCE_PRINT_MEMORY(function); \
	} while(0)



#define si_performance_loops_average_cmp(increments_of_10, function1, function2) \
    si_performance_loops_average_cmp_range(1, increments_of_10, function1, function2)

#define si_performance_loops_average_cmp_range(start, end, function1, function2) \
	do { \
        siAllocator* alloc = si_allocator_make_stack(4096); \
		siArray(u64) cycles[2] = { \
            (u64*)si_array_make_reserve(alloc, sizeof(u64), 20), \
            (u64*)si_array_make_reserve(alloc, sizeof(u64), 20)  \
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
#define realloc(ptr, new_size) si_debug_alloc(3, ptr, (siDebugArgs){0, new_size}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define si_realloc(ptr, old_size, new_size) si_debug_alloc(4, ptr, (siDebugArgs){old_size, new_size}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
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


inline siAllocator* si_allocator_make(usize bytes) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	rawptr ptr = malloc(sizeof(siAllocator) + bytes);

	siAllocator* res = (siAllocator*)ptr;
	res->ptr = (siByte*)ptr + sizeof(siAllocator);
	res->index = 0;
	res->max_size = bytes;

	return res;
#else
    return nil;
#endif
}
inline siAllocator* si_allocator_make_stack_ex(usize bytes, rawptr ptr) {
    siAllocator* res = (siAllocator*)ptr;
    res->ptr = (siByte*)ptr + sizeof(siAllocator);
    res->index = 0;
    res->max_size = bytes;

    return res;
}
inline void si_allocator_refresh(siAllocator* alloc) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
    usize size = alloc->max_size;
    si_allocator_free(alloc);
    *alloc = *si_allocator_make(size);
#endif
}
inline void si_allocator_free(siAllocator* alloc) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	SI_ASSERT_NOT_NULL(alloc->ptr);
	free(alloc->ptr - sizeof(siAllocator));
#endif
}

inline rawptr si_malloc(siAllocator* alloc, usize bytes) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	SI_ASSERT_NOT_NULL(alloc);

	siByte* res = alloc->ptr + alloc->index;
	alloc->index += bytes;

	SI_ASSERT_FMT(
        alloc->index <= alloc->max_size,
        "Exceeded the available memory for allocation (Tried writing '%zd' bytes"
        " into a already filled allocator with '%zd' bytes. Current index: '%zd').",
        bytes, alloc->max_size, alloc->index
    );

	return res;
#else
    return malloc(bytes);
#endif
}

inline rawptr si_calloc(siAllocator* alloc, usize num, usize bytes) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	rawptr res = si_malloc(alloc, num * bytes);
	memset(res, 0, num * bytes);

	return res;
#else
    return calloc(bytes, num);
#endif
}

inline void si_free(siAllocator* alloc, rawptr ptr) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(ptr);
	SI_ASSERT_FMT(
        si_between((siByte*)ptr, alloc->ptr, alloc->ptr + alloc->max_size),
        "Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
        ptr, alloc->ptr, alloc->ptr + alloc->max_size
    );

	alloc->index = (siByte*)ptr - alloc->ptr;
#else
    free(ptr);
#endif
}
rawptr si_realloc(siAllocator* alloc, rawptr ptr, usize old_size, usize new_size) {
#if !defined(SI_ALLOCATOR_UNDEFINE)
    SI_ASSERT_NOT_NULL(alloc);

    if (ptr == NULL) {
        return si_malloc(alloc, new_size);
    }

    SI_ASSERT_FMT(
        si_between((siByte*)ptr, alloc->ptr, alloc->ptr + alloc->max_size),
        "Provided pointer wasn't allocated through the provided allocator (ptr is '%p', which isn't between '%p' and '%p')",
        ptr, alloc->ptr, alloc->ptr + alloc->max_size
    );

    if (new_size == 0) {
        si_free(alloc, ptr);
        return nil;
    }

    isize size_diff = new_size - old_size;

    if (size_diff < 0) {
        rawptr new_ptr = si_malloc(alloc, new_size);
        memmove(new_ptr, ptr, new_size);
        si_free(alloc, ptr);

        return new_ptr;
    }
    else if (size_diff > 0) {
        usize old_index = (siByte*)ptr - alloc->ptr;
        alloc->index = old_index + new_size;
    }

    return ptr;
#else
    return realloc(ptr, new_size);
#endif
}


#endif


#if defined(SI_GENERAL_IMPLEMENTATION)

usize si_impl_assert_msg(bool condition, cstring condition_str, cstring file, i32 line, cstring func, cstring message, ...) {
    SI_STOPIF(condition, return 0);
    fprintf(stderr, "Assertion \"%s\" at \"%s:%d\": %s%s", condition_str, file, line, func, (message != nil ? ": " : ""));

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

inline u64 si_time_utc_now(void) {
    time_t rawtime;
    time(&rawtime);

    return rawtime;
}

inline void si_sleep(usize miliseconds) {
#if defined(SI_SYSTEM_WINDOWS)
    Sleep(miliseconds);
#else
    SI_ASSERT_MSG(miliseconds, "Why would you sleep for 0 ms?");
    struct timespec ts = {
        (time_t)miliseconds / 1000,
        si_cast(time_t, miliseconds % 1000) * 1000000
    };
    nanosleep(&ts, &ts);
#endif
}

inline u64 si_RDTSC(void) {
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


inline void si_ptr_move_by(rawptr src, usize src_len, usize move_by, siDirection direction) {
    isize mul = (direction == SI_DIRECTION_RIGHT ? 1 : -1); /* NOTE(EimaMei): Branchless way of moving. */
    memcpy((siByte*)src + ((isize)move_by * mul), src, src_len);
}

/* TODO(EimaMei): Check for every mention of malloc, calloc, realloc and free to see if they're correct. */
inline rawptr si_any_copy(rawptr alloc, rawptr content, usize size) {
    rawptr res = si_malloc((siAllocator*)alloc, size);
    memcpy(res, content, size);

    return res;
}
#endif


#if defined(SI_PAIR_IMPLEMENTATION) && !defined(SI_PAIR_UNDEFINE)

rawptr si_impl_pair_make(siAllocator* alloc, siAny first, siAny second) {
	siByte* res = (siByte*)si_malloc(alloc, first.type_size + second.type_size);
	memcpy(res, first.ptr, first.type_size);
	memcpy(res + first.type_size, second.ptr, second.type_size);

	return (rawptr)res;
}
#endif

#if defined(SI_ARRAY_IMPLEMENTATION) && !defined(SI_ARRAY_UNDEFINE)

inline rawptr si_array_make_list(siAllocator* alloc, rawptr list, usize sizeof_element, usize count)  {
	rawptr array = si_array_make_reserve(alloc, sizeof_element, count);
	memcpy(array, list, sizeof_element * count);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	header->len = count;

	return array;
}
inline rawptr si_array_copy(siAllocator* alloc, rawptr array) {
	siArrayHeader* header = SI_ARRAY_HEADER(array);
	return si_array_make_list(alloc, array, header->type_size, header->len);
}
rawptr si_array_make_reserve(siAllocator* alloc, usize sizeof_element, usize count) {
	if (count == 0) {
        count = 1;
	}

	rawptr ptr = si_malloc(alloc, sizeof(siArrayHeader) + (sizeof_element * count));
	rawptr array = (siByte*)ptr + sizeof(siArrayHeader);

	siArrayHeader* header = (siArrayHeader*)ptr;
	header->len = 0;
	header->capacity = count;
	header->type_size = sizeof_element;
    header->allocator = alloc;

	return array;
}

inline usize si_array_len(rawptr array) {
	return SI_ARRAY_HEADER(array)->len;
}
inline usize si_array_capacity(rawptr array) {
	return SI_ARRAY_HEADER(array)->capacity;
}
inline usize si_array_type_size(rawptr array) {
	return SI_ARRAY_HEADER(array)->type_size;
}
inline usize si_array_total_size(rawptr array) {
	return si_array_capacity(array) * si_array_type_size(array);
}
inline siAllocator* si_array_allocator(rawptr array) {
    return SI_ARRAY_HEADER(array)->allocator;
}
inline bool si_array_empty(rawptr array) {
    return (si_array_len(array) == 0 || array == nil);
}

inline siAny si_array_at(rawptr array, usize index) {
	if (index >= si_array_len(array)) {
        siAny res = {nil, 0};
		return res;
	}

	siAny res = {
        si_array_get_ptr(array, index),
        si_array_type_size(array)
    };
	return res;
}
inline siAny si_array_front(rawptr array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	siAny res = {
        array,
        si_array_type_size(array)
    };
	return res;
}
inline siAny si_array_back(rawptr array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	siAny res = {
        si_array_get_ptr(array, si_array_len(array) -1),
        si_array_type_size(array)
    };
	return res;
}

isize si_impl_array_find(rawptr array, usize start, usize end, siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_MSG(value.type_size <= si_array_type_size(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start < end, "Value 'start' is larger than 'end'");

	bool found = false;
	usize i;
	for (i = start; i < end; i++) {
		if (memcmp((siByte*)array + i * si_array_type_size(array), value.ptr, value.type_size) == 0) {
			found = true;
			break;
		}
	}

	return (found ? (isize)i : SI_ERROR);
}
isize si_impl_array_rfind(rawptr array, usize start, usize end, siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_MSG(value.type_size <= si_array_type_size(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start >= end, "Value 'end' is larger than 'start'");

	bool found = false;
	usize i;
	for (i = start; i < end - 1; i--) {
		if (memcmp((siByte*)array + i * si_array_type_size(array), value.ptr, value.type_size) == 0) {
			found = true;
			break;
		}
	}

	return (found ? (isize)i : SI_ERROR);
}

void si_impl_array_replace(rawptr array, siAny old_value, siAny new_value) {
	SI_ASSERT_NOT_NULL(array);
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	SI_ASSERT_MSG(
		si_between(new_value.type_size, old_value.type_size, header->type_size),
		"The given value's sizeof is too large compared to the elements' in the array."
	);

	isize index = 0;
	while (true) {
		index = si_impl_array_find(array, index, header->len, old_value);
		if (index == SI_ERROR) {
			return ;
    	}

	    memcpy(si_array_get_ptr(array, index), new_value.ptr, new_value.type_size);
    }
}
void si_array_reverse(rawptr array_ptr) {
	SI_ASSERT_NOT_NULL(array_ptr);
	siByte* array = *si_cast(siByte**, array_ptr);

	usize len = si_array_len(array);
	usize type_size = si_array_type_size(array);
	SI_ASSERT_MSG(len != 0, "Array is empty");

	siByte* a = array;
	siByte* b = array + (len - 1) * type_size;
	rawptr tmp[1];
	len *= 0.5f;

	while (len -= 1) {
		memcpy(tmp, a, type_size);
		memcpy(a, b, type_size);
		memcpy(b, tmp, type_size);

		a += type_size;
        b -= type_size;
	}
}
siString si_array_to_sistring(siArray(char*) array, cstring separator) {
	SI_ASSERT_NOT_NULL(array);

	usize separator_len = si_cstr_len(separator);
	usize total_size = 0;

	for_range (i, 0, si_array_len(array)) {
		total_size += si_cstr_len(array[i]) + separator_len;
	}

	siString result = si_string_make_reserve(SI_ARRAY_HEADER(array)->allocator, total_size);
	rawptr back_ptr = si_array_back(array).ptr;
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

inline void si_array_pop(rawptr array_ptr) {
    siByte* array = *si_cast(siByte**, array_ptr);
    SI_ARRAY_HEADER(array)->len -= 1;
}

void si_impl_array_insert(rawptr array_ptr, siAny new_item, usize index) {
    SI_ASSERT_NOT_NULL(array_ptr);

	siByte* array = *si_cast(siByte**, array_ptr);
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	usize previous_len = header->len;
	usize before_index_len = previous_len - index;

	SI_ASSERT_FMT(index < previous_len, "Index is higher than the length of the array (array - '%zd', index - '%zd')", previous_len, index);
	header->len += 1;

	if (header->capacity < header->len) {
		rawptr result = si_realloc(
            header->allocator,
            header,
            sizeof(siArrayHeader) + header->capacity * header->type_size,
            sizeof(siArrayHeader) + 2 * (header->capacity * header->type_size)
        );

		array = (siByte*)result + sizeof(siArrayHeader);
		*si_cast(siByte**, array_ptr) = array;

		header = (siArrayHeader*)result;
		header->capacity *= 2;
	}

	memcpy(
        array + (header->len - before_index_len) * header->type_size,
        array + index * header->type_size,
        before_index_len * header->type_size
    );
	memcpy(array + index * header->type_size, new_item.ptr, new_item.type_size);
}
inline void si_array_erase(rawptr array_ptr, usize index) {
    si_array_erase_count(array_ptr, index, 1);
}
void si_array_erase_count(rawptr array_ptr, usize index, usize count) {
    SI_ASSERT_NOT_NULL(array_ptr);
	siByte* array = *si_cast(siByte**, array_ptr);

	usize len = si_array_len(array);
	SI_ASSERT_MSG(index < len, "Index is higher than the length of the string.");
    SI_ASSERT_FMT(index + count <= len, "Index + count is higher than the length of the array (%zd > %zd).", index + count, len);

	memcpy(array + index * si_array_type_size(array), array + (index + count) * si_array_type_size(array), (len - index - count) * si_array_type_size(array));

	SI_ARRAY_HEADER(array)->len -= count;
}
void si_impl_array_remove_item(rawptr array_ptr, siAny value) {
    SI_ASSERT_NOT_NULL(array_ptr);
	siByte* array = *si_cast(siByte**, array_ptr);

    SI_ASSERT_MSG(value.type_size <= si_array_type_size(array), "The given value's sizeof is too large compared to the elements' in the array.");
	siArrayHeader* header = SI_ARRAY_HEADER(array);

	while (header->len != 0) {
		isize index = si_impl_array_rfind(array, header->len - 1, 0, value);
		if (index == -1) {
			break;
		}

		memcpy(si_array_get_ptr(array, index), si_array_get_ptr(array, index + header->len), value.type_size);

        if (header->len != 0) {
            header->len -= 1;
        }
	}
}
void si_impl_array_fill(rawptr array_ptr, usize index, usize count, siAny value) {
	SI_ASSERT_NOT_NULL(array_ptr);
	siByte* array = *si_cast(siByte**, array_ptr);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	SI_ASSERT_MSG(value.type_size <= header->type_size, "The given value's sizeof is too large compared to the elements' in the array.");
    SI_ASSERT_MSG(index < header->capacity, "Index is higher than the array's length.");

    usize previous_len = header->len;
    isize size_dif = si_abs((isize)index - (isize)count);
    header->len += size_dif;

	if (header->capacity < header->len) {
		rawptr result = si_realloc(
            header->allocator,
            header,
            sizeof(siArrayHeader) + header->capacity * header->type_size,
            sizeof(siArrayHeader) + size_dif +  2 * header->capacity * header->type_size
        );
		SI_ASSERT_NOT_NULL(result);

		array = (siByte*)result + sizeof(siArrayHeader);
		*si_cast(siByte**, array_ptr) = array;

		header = (siArrayHeader*)result;
		header->capacity = header->capacity * 2 + size_dif;
	}

    for_range(i, index, count) {
        memcpy(si_array_get_ptr(array, previous_len + i), value.ptr, header->type_size);
    }
}

void si_impl_array_append(rawptr array_ptr, siAny value) {
	SI_ASSERT_NOT_NULL(array_ptr);
	siByte* array = *si_cast(siByte**, array_ptr);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	SI_ASSERT_MSG(value.type_size <= header->type_size, "The given value's sizeof is too large compared to the elements' in the arra");

    usize previous_len = header->len;
    header->len += 1;

	if (header->capacity < header->len) {
		rawptr result = si_realloc(
            header->allocator,
            header,
            sizeof(siArrayHeader) + header->capacity * header->type_size,
            sizeof(siArrayHeader) + 2 * header->capacity * header->type_size
        );

		array = (siByte*)result + sizeof(siArrayHeader);
		*si_cast(siByte**, array_ptr) = array;

		header = (siArrayHeader*)result;
		header->capacity *= 2;
	}

	memcpy(si_array_get_ptr(array, previous_len), value.ptr, value.type_size);
}

void si_array_clear(rawptr array_ptr) {
	SI_ASSERT_NOT_NULL(array_ptr);
	rawptr array = *si_cast(siByte**, array_ptr);

	memset(array, 0, si_array_total_size(array));
}

bool si_array_equal(rawptr lha, rawptr rha) {
	siArrayHeader* lha_header = SI_ARRAY_HEADER(lha);
	siArrayHeader* rha_header = SI_ARRAY_HEADER(rha);

	if (lha_header->len != rha_header->len || lha_header->type_size != rha_header->type_size) {
		return false;
	}

	usize width = lha_header->type_size;
	usize i;
	for (i = 0; i < lha_header->len; i++) {
		if (memcmp((siByte*)lha + i * width, (siByte*)rha + i * width, width) != SI_OKAY) {
			return false;
		}
	}

	return true;
}

inline void si_array_free(rawptr array) {
    SI_ASSERT_NOT_NULL(array);
	free(SI_ARRAY_HEADER(array));
}

void si_array_shrink_to_fit(rawptr array_ptr) {
	SI_ASSERT_NOT_NULL(array_ptr);
	siByte* array = *si_cast(siByte**, array_ptr);
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	header = (typeof(header))si_realloc(
        header->allocator,
        header,
        sizeof(siArrayHeader*) + header->capacity * header->type_size,
        sizeof(siArrayHeader*) + header->len * header->type_size
    );
	header->capacity = header->len;
}

inline u64 si_array_sum_u64(siArray(void) array) {
    SI_ASSERT_NOT_NULL(array);
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	u64 res = 0;
	siByte* ptr = (siByte*)array;
    siByte* end = ptr + header->len * header->type_size;

    while (ptr < end) {
		res += si_u64_from_ptr(ptr, header->type_size);
	    ptr += header->type_size;
    }

	return res;
}
inline i64 si_array_sum_i64(siArray(void) array) {
    SI_ASSERT_NOT_NULL(array);
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	i64 res = 0;
	siByte* ptr = (siByte*)array;
    siByte* end = ptr + header->len * header->type_size;

    while (ptr < end) {
		res += (i64)si_u64_from_ptr(ptr, header->type_size);
	    ptr += header->type_size;
    }

	return res;
}
inline f64 si_array_sum_f64(siArray(void) array) {
    SI_ASSERT_NOT_NULL(array );
    siArrayHeader* header = SI_ARRAY_HEADER(array);

	f64 res = 0;
	siByte* ptr = (siByte*)array;
    siByte* end = ptr + header->len * header->type_size;

    while (ptr < end) {
		res += (f64)si_u64_from_ptr(ptr, header->type_size);
	    ptr += header->type_size;
    }

	return res;
}

#if defined(SI_STRING_IMPLEMENTATION) && !defined(SI_STRING_UNDEFINE)


inline siString si_string_make(siAllocator* alloc, cstring str) {
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
	/*header->type_size = sizeof(char); */

	return res_str;
}

inline siString si_string_copy(siAllocator* alloc, siString from) {
	return si_string_make_len(alloc, from, si_string_len(from));
}

inline usize si_string_len(siString str) {
	return SI_STRING_HEADER(str)->len;
}
inline usize si_string_capacity(siString str) {
	return SI_STRING_HEADER(str)->capacity;
}
inline usize si_string_type_size(siString str) {
	return sizeof(char);
    SI_UNUSED(str);
}
inline siAllocator* si_string_allocator(siString str) {
    return SI_STRING_HEADER(str)->allocator;
}
inline bool si_string_empty(siString str) {
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

inline usize si_cstr_len(cstring str) {
	return strlen(str); /* NOTE(EimaMei): Generally for performance, OG strlen is best for both portability and performance. */
}

inline bool si_cstr_equal(cstring str1, cstring str2) {
	return si_cstr_equal_len(str1, si_cstr_len(str1), str2, si_cstr_len(str2));
}
bool si_cstr_equal_len(cstring str1, usize str1_len, cstring str2, usize str2_len) {
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

inline char si_string_at(siString str, usize index) {
	SI_ASSERT_NOT_NULL(str);
	if (index > si_string_len(str) || si_string_len(str) == 0) {
		return SI_ERROR;
	}

	return str[index];
}
inline char si_string_front(siString str) {
	SI_ASSERT_NOT_NULL(str);
	if (si_string_len(str) == 0) {
		return SI_OKAY;
	}

	return str[0];
}
inline char si_string_back(siString str) {
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

inline isize si_string_find(siString str, cstring cstr) {
	return si_string_find_ex(str, 0, si_string_len(str), cstr, si_cstr_len(cstr));
}
isize si_string_find_ex(siString str, usize start, usize end, cstring cstr, usize cstr_len) {
	SI_ASSERT_NOT_NULL(str);

	bool found = false;

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
inline isize si_string_rfind(siString str, cstring cstr) {
	return si_string_rfind_ex(str, si_string_len(str) - 1, 0, cstr);
}
isize si_string_rfind_ex(siString str, usize start, usize end, cstring cstr) {
	SI_ASSERT_NOT_NULL(str);

	usize cstr_len = si_cstr_len(cstr);
	bool found = false;

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

	siAllocator* alloc = si_allocator_make_stack(SI_KILO(8)); /* NOTE(EimaMei):
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
void si_string_replace(siString* str, cstring old_value, cstring new_value) {
	usize old_len = si_cstr_len(old_value);
	usize new_len = si_cstr_len(new_value);

	isize index = 0;
	siString cur_str = *str;

	while (true) {
		index = si_string_find_ex(cur_str, index, si_string_len(cur_str), old_value, old_len);
		if (index == SI_ERROR) {
			break;
		}

		si_string_erase(str, index, old_len);
		si_string_insert_ex(str, new_value, new_len, index, false);
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

	si_ptr_move_by(cur_str, header->len, 1, SI_DIRECTION_RIGHT);
	cur_str[0] = '\"';
	cur_str[header->len - 1] = '\"';
}

inline void si_string_append(siString* str, cstring other) {
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
inline void si_string_push(siString* str, char other) {
	si_string_append_len(str, &other, 1);
}
inline void si_string_pop(siString* str) {
	siString cur_str = *str;
	*(cur_str + SI_STRING_HEADER(cur_str)->len - 1) = '\0';
	SI_STRING_HEADER(cur_str)->len -= 1;
}

inline void si_string_insert(siString* str, cstring cstr, usize index) {
	si_string_insert_ex(str, cstr, si_cstr_len(cstr), index, false);
}
void si_string_insert_ex(siString* str, cstring cstr, usize cstr_len, usize index, bool erase_index) {
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

inline void si_cstr_upper(char* str) {
	SI_ASSERT_NOT_NULL(str);

	char x = '\0';
	while ((x = *str)) {
		*str = si_char_to_upper(x);
		str++;
	}
}
inline void si_cstr_lower(char* str) {
	SI_ASSERT_NOT_NULL(str);

	char x = '\0';
	while ((x = *str)) {
		*str = si_char_to_lower(x);
		str += 1;
	}
}
void si_cstr_title(char* str) {
	SI_ASSERT_NOT_NULL(str);

	bool change = true;
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
inline void si_cstr_capitalize(char* str) {
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
inline void si_string_reverse(siString* str) {
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
	siArray(siString) res = (typeof(res))si_array_make_reserve(alloc, sizeof(*res), count);
	SI_ARRAY_HEADER(res)->len = count;

	for_range (i, 0, count) {
		res[i] = si_string_make_len(alloc, str, pos_buffer[i]);
		str += pos_buffer[i] + separator_len;
	}

	return res;
}
inline void si_string_clear(siString* str) {
	siString cur_str = *str;
	*cur_str = '\0';
	SI_STRING_HEADER(cur_str)->len = 0;
}

inline void si_string_free(siString str) {
	SI_ASSERT_NOT_NULL(str);
    si_free(SI_STRING_HEADER(str)->allocator, SI_STRING_HEADER(str));
}
void si_string_make_space_for(siString* str, usize add_len) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize old_size = sizeof(siStringHeader) + header->len + 1;
	usize new_size = old_size + add_len;

	siStringHeader* new_header = (siStringHeader*)si_realloc(header->allocator, header, old_size, new_size);

    *str = (char*)new_header + sizeof(siStringHeader);
	new_header->capacity += add_len;
}
void si_string_shrink_to_fit(siString* str) {
	SI_ASSERT_NOT_NULL(str);
    siString cur_str = *str;
    siStringHeader* header = SI_STRING_HEADER(cur_str);

	header = (typeof(header))si_realloc(
            header->allocator,
            header,
            sizeof(siStringHeader*) + header->capacity + 1,
            sizeof(siStringHeader*) + header->len + 1
    );
	header->capacity = header->len;
}
#endif

#if defined(SI_CHAR_IMPLEMENTATION) && !defined(SI_STRING_UNDEFINE)

inline char si_char_to_lower(char c) {
	if (c >= 'A' && c <= 'Z') {
		return c + 32; /* NOTE(EimaMei): 32 is just the math result of 'A' - 'a'. This performs much better, as the previous version required 2 math operations, while this - only one. */
	}
	return c;
}

inline char si_char_to_upper(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 32; /* NOTE(EimaMei): 32 is just the math result of 'A' - 'a'. This performs much better, as the previous version required 2 math operations, while this - only one. */
	}
	return c;
}

inline bool si_char_is_space(char c) {
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}
inline bool si_char_is_digit(char c) {
	return (c >= '0' && c <= '9');
}
inline bool si_char_is_hex_digit(char c) {
	return (si_char_is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}
inline bool  si_char_is_alpha(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
inline bool si_char_is_alphanumeric(char c) {
	return si_char_is_alpha(c) || si_char_is_digit(c);
}
inline i32 si_digit_to_int(char c) {
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
	bool negative = false;

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

inline bool si_path_exists(cstring path) {
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
inline isize si_path_move(cstring existing_path, cstring new_path) {
	#if defined(SI_SYSTEM_WINDOWS)
		return (MoveFile(existing_path, new_path) != 0);
	#else
		if (link(existing_path, new_path) == 0) {
			return unlink(existing_path);
		}
		return SI_ERROR;
	#endif
}
inline isize si_path_remove(cstring path) {
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

inline cstring si_path_base_name(cstring path) {
	isize pos = si_string_rfind_ex((siString)path, si_cstr_len(path) - 1, 0, &SI_PATH_SEPARATOR);

	return (pos != SI_ERROR ? (path + pos + 1) : path);
}
inline cstring si_path_extension(cstring path) {
	isize pos = si_string_rfind_ex((siString)path, si_cstr_len(path) - 1, 0, ".");

	return (pos != SI_ERROR ? (path + pos + 1) : path);
}
inline siString si_path_get_fullname(siAllocator* alloc, cstring path) {
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

inline bool si_path_is_absolute(cstring path) {
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
inline bool si_path_is_relative(cstring path) {
	return !si_path_is_absolute(path);
}




inline siFile si_file_create(cstring path) {
	return si_file_open_mode(path, "w+");
}
inline siFile si_file_open(cstring path) {
	return si_file_open_mode(path, "r+");
}
siFile si_file_open_mode(cstring path, cstring mode) {
	SI_ASSERT_NOT_NULL(path);

	FILE* f = fopen(path, mode);

	if (f == nil) {
		switch (errno) {
			case ENOENT: SI_PANIC_MSG("Failed to read the provided file, as it doesn't exist."); break;
			case EACCES: SI_PANIC_MSG("Failed to read the provided file, as I don't have suffice permissions to read the file."); break;
			case EINVAL: SI_PANIC_MSG("Failed to read the provided file, as an invalid argument was given."); break;
			default: SI_PANIC_MSG("Failed to read the provided file due to an unspecified reason.");
		}
	}

	siFile result = {
		0,
		f
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
inline void si_file_size_update(siFile* file) {
	usize len = si_file_size(*file);
	file->size = len;
}

inline siArray(void) si_file_read(siAllocator* alloc, siFile file) {
	return si_file_read_at(alloc, file, 0, file.size);
}
siArray(void) si_file_read_at(siAllocator* alloc, siFile file, usize index, usize len) {
	SI_ASSERT_NOT_NULL(file.ptr);

    siArray(void) buffer = si_array_make_reserve(alloc, sizeof(siByte), len);
    SI_ARRAY_HEADER(buffer)->len = len;

    fseek(file.ptr, index, SEEK_SET);
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
    siAllocator* temp = si_allocator_make(file.size * (sizeof(siString) + sizeof(siStringHeader)));

	siString buffer = (siString)si_file_read(temp, file);
	siArray(siString) res = si_string_split(alloc, buffer, "\n");

    si_allocator_free(temp);

	return res;
}

inline siError si_file_write(siFile* file, cstring content) {
	return si_file_write_at_len(file, content, si_cstr_len(content), 0);
}
inline siError si_file_write_len(siFile* file, cstring content, usize len) {
	return si_file_write_at_len(file, content, len, 0);
}
inline siError si_file_write_at(siFile* file, cstring content, usize offset) {
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

    siAllocator* temp = si_allocator_make(file->size * (sizeof(siStringHeader) + sizeof(siString)));
	siArray(siString) buffer = si_file_readlines(temp, *file);
	//siString previous_line = buffer[index];
	SI_ASSERT_MSG(index < si_array_len(buffer), "Index is either not 0 or higher than the amount of lines in the file.");


	buffer[index] = (siString)content;
	siString new_file_content = si_array_to_sistring(buffer, "\n");

	siError err = si_file_write_len(file, new_file_content, si_string_len(new_file_content));

	/*usize i;
	for (i = 0; i < si_array_len(buffer); i++) {
		if (i == index) {
			continue;
		}
		si_string_free(buffer[i]);
	}
	si_string_free(previous_line);
	si_array_free(buffer);

	si_string_free(new_file_content);
	si_file_close(*file);*/
    si_allocator_free(temp);

    return err;
}

inline isize si_file_seek(siFile file, usize offset) {
	SI_ASSERT_NOT_NULL(file.ptr);
	return fseek(file.ptr, offset, SEEK_SET);
}
inline isize si_file_seek_to_end(siFile file) {
	SI_ASSERT_NOT_NULL(file.ptr);
	return fseek(file.ptr, 0, SEEK_SET);
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

inline siThread si_thread_create_ex(siFunction function, rawptr arg) {
	siThread t;
	t.is_running = false;
	t.func = function;
	t.arg = arg;

	return t;
}


inline void si_thread_start(siThread* t) {
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
inline void si_thread_destroy(siThread* t) {
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

inline usize si_num_bits_u8(u8 num) {
    return (num * 01001001001ULL & 042104210421ULL) % 017;
}

inline usize si_num_bits_u32(u32 num) {
     num -= ((num >> 1) & 0x55555555);
     num = (num & 0x33333333) + ((num >> 2) & 0x33333333);
     num = (num + (num >> 4)) & 0x0F0F0F0F;
     return (num * 0x01010101) >> 24;
}

inline usize si_num_bits_u64(u64 num) {
    num = num - ((num >> 1) & 0x5555555555555555);
    num = (num & 0x3333333333333333) + ((num >> 2) & 0x3333333333333333);
    num = (num + (num >> 4)) & 0xF0F0F0F0F0F0F0F;
    return (num * 0x101010101010101) >> 56;
}

inline usize si_num_leading_bit_ex(u64 num, usize total_bits, siBitType bit) {
    u64 count = 0;

    usize i;
    for (i = total_bits - 1; i < USIZE_MAX; i -= 1) {
        if (SI_NUM_GET_BIT(num, i) == bit) {
            count += 1;
        }
        else {
            return count;
        }
    }

    return count;
}
inline usize si_num_trailing_bit_ex(u64 num, usize total_bits, siBitType bit) {
    u64 count = 0;

    for_range (i, 0, total_bits) {
        if (SI_NUM_GET_BIT(num, i) == bit) {
            count += 1;
        }
        else {
            return count;
        }
    }

    return count;
}
inline u64 si_num_rotate_left_ex(u64 num, usize total_bits, usize bits) {
    return (num << bits) | (num >> (total_bits - bits));
}
inline u64 si_num_rotate_right_ex(u64 num, usize total_bits, usize bits) {
    return (num >> bits) | (num << (total_bits - bits));
}

inline u64 si_num_reverse_bits_ex(u64 num, usize total_bits) {
    u64 res = 0;

    for_range(i, 0, total_bits) {
        res <<= 1;
        res |= (num & 1);
        num >>= 1;
    }

    return res;
}

inline siArray(u8) si_num_to_bytes_ex(siAllocator* alloc, u64 num, usize num_sizeof) {
    siArray(u8) res = (u8*)si_array_make_reserve(alloc, sizeof(*res), num_sizeof);
    SI_ARRAY_HEADER(res)->len = num_sizeof;

    for_range (i, 0, num_sizeof) {
        res[i] = (num >> ((num_sizeof - 1 - i) * 8)) & 255;
    }

    return res;
}
inline u64 si_bytes_to_num(siArray(u8) bytes) {
    SI_ASSERT_NOT_NULL(bytes);
    usize res = 0;
    usize shift = (si_array_len(bytes) - 1) * 8;

    for_each (byte, bytes) {
        res |= si_cast(usize, *byte) << shift;
        shift -= 8;
    }

    return res;
}

inline u64 si_num_change_endian_ex(u64 num, usize num_sizeof) {
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

inline u64 si_num_pow2(u64 base, usize exp) {
    return exp << base;
}

inline usize si_num_len(u64 num) {
    usize count = 0;

    do {
        count += 1;
        num /= 10;
    } while (num > 0);

    return count;
}

inline u64 si_num_round_nearest_multiple(u64 num, usize multiple) {
    return ((num + multiple / 2) / multiple) * multiple;
}

#endif

#if defined(SI_CPU_IMPLEMENTATION) && !defined(SI_CPU_UNDEFINE)

inline siMhz si_cpu_clock_speed(void) {
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
		header->type_size = sizeof(siDebugHeader*);
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
	header->index = si_array_len(si_debug.list);

	si_debug.allocs += 1;
	si_debug.total += size;
	si_debug.overall += size;

	si_array_append(&si_debug.list, header);

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
#define realloc(ptr, new_size) si_debug_alloc(3, ptr, (siDebugArgs){0, new_size}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define si_realloc_cpy(ptr, old_size, new_size) si_debug_alloc(4, ptr, (siDebugArgs){old_size, new_size}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
#define free(ptr) si_debug_free(ptr)

#endif

#if defined(SI_PERFORMANCE_IMPLEMENTATION) && !defined(SI_PERFORMANCE_UNDEFINE)

inline void si_impl_performance_thread(u32* arg) {
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
#endif
/*
------------------------------------------------------------------------------

This software is available under 2 licenses - you may choose the one you like.

------------------------------------------------------------------------------

ALTERNATIVE A - MIT License

Copyright (c) 2023 EimaMei

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

------------------------------------------------------------------------------

ALTERNATIVE B - Public Domain (www.unlicense.org)

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------------------------------------------
*/
