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
		- #define SI_BUFFER_UNDEFINE
		- #define SI_STRING_UNDEFINE
		- #define SI_CHAR_UNDEFINE
		- #define SI_FILE_UNDEFINE
		- #define SI_THREAD_UNDEFINE
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


#if defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
	#define SI_ARCH_64_BIT 1
#else
	#define SI_ARCH_32_BIT 1
#endif

#define SI_ENDIAN_BIG     (!*(u8*)&(u16){1})
#define SI_ENDIAN_LITTLE  (!SI_IS_BIG_ENDIAN)

#if defined(_WIN32) || defined(_WIN64)
	#define SI_SYSTEM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
	#define SI_SYSTEM_OSX 1
#elif defined(__unix__)
	#define SI_SYSTEM_UNIX 1

	#if defined(__linux__)
		#define SI_SYSTEM_LINUX 1
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#define SI_SYSTEM_FREEBSD 1
	#endif
#endif

#if defined(_MSC_VER)
	#define SI_COMPILER_MSVC 1
#elif defined(__GNUC__)
	#define SI_COMPILER_GCC 1
#elif defined(__clang__)
	#define SI_COMPILER_CLANG 1
#else
	#error Unknown compiler
#endif


#if defined(__cplus__)
	#define SI_LANGUAGE_CPLUS 1
#elif defined(__cplusplus)
	#define SI_LANGUAGE_CPP 1
#elif defined(__OBJC__)
	#define SI_LANGUAGE_OBJC 1
#else
	#define SI_LANGUAGE_C 1
#endif


#if defined(__STDC__)
	#if defined(__STDC_VERSION__)
		#define SI_STANDARD_VERSION __STDC_VERSION__
		#if (SI_STANDARD_VERSION == 199409L)
			#define SI_STANDARD_C94 1
		#elif (SI_STANDARD_VERSION == 199901L)
			#define SI_STANDARD_C99 1
		#elif (SI_STANDARD_VERSION == 201112L)
			#define SI_STANDARD_C11 1
		#elif (SI_STANDARD_VERSION == 201710L)
			#define SI_STANDARD_C17 1
			#define SI_STANDARD_C18 1
		#elif (SI_STANDARD_VERSION > 201710L)
			#define SI_STANDARD_C23 1
		#endif
	#else
		#if !defined(__cplusplus)
			#define SI_STANDARD_C89 1
		#else
			#define SI_STANDARD_VERSION __cplusplus
			#if (SI_STANDARD_VERSION == 199711L)
				#define SI_STANDARD_CPP98 1
			#elif (SI_STANDARD_VERSION == 201103L)
				#define SI_STANDARD_CPP11 1
			#elif (SI_STANDARD_VERSION == 201402L)
				#define SI_STANDARD_CPP14 1
			#elif (SI_STANDARD_VERSION == 201703L)
				#define SI_STANDARD_CPP17 1
			#elif (SI_STANDARD_VERSION == 202002L)
				#define SI_STANDARD_CPP20 1
			#elif (SI_STANDARD_VERSION > 202002L)
				#define SI_STANDARD_CPP23 1
			#endif
		#endif
	#endif
#endif

#ifdef __STRICT_ANSI__
	#define SI_STANDARD_ANSI 1
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
	#define SI_CPU_X86 1
	#define SI_CACHE_LINE_SIZE 64

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
	#define SI_CPU_PPC 1
	#define SI_CACHE_LINE_SIZE 128

#elif defined(__arm__)
	#define SI_CPU_ARM 1
	#define SI_CACHE_LINE_SIZE 64

#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
	#define SI_CPU_MIPS 1
	#define SI_CACHE_LINE_SIZE 64

#else
	#error Unknown CPU Type
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
	#define _LARGEFILE64_SOURCE
#endif

#if !defined(SI_SYSTEM_WINDOWS)
	#include <stdlib.h>
	#include <memory.h>
	#include <stdarg.h>
	#include <stddef.h>
	#include <errno.h>
	#include <unistd.h>
	#include <pthread.h>

	#include <sys/stat.h>
	#include <sys/fcntl.h>

	#include <stdio.h>
	#include <time.h>
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

#if !defined(u8)
	typedef unsigned char       u8;
	typedef char                i8;
	typedef unsigned short     u16;
	typedef short              i16;
	typedef unsigned int       u32;
	typedef int                i32;
	typedef unsigned long long u64;
	typedef long long          i64;
#endif

#if !defined(usize)
	typedef size_t    usize;
	typedef ptrdiff_t isize;
#endif

#if !defined(uintptr)
	typedef usize uintptr;
	typedef isize  intptr;
#endif

typedef u8 siByte;

#define SI_UINT8_MAX    0xFF
#define SI_UINT8_MIN    0x00
#define SI_INT8_MAX     0x7F
#define SI_INT8_MIN    -0x80

#define  SI_UINT16_MAX  0xFFFF
#define  SI_UINT16_MIN  0x0000
#define  SI_INT16_MAX   0x7FFF
#define  SI_INT16_MIN  -0x8000

#define  SI_UINT32_MAX  0xFFFFFFFF
#define  SI_UINT32_MIN  0x00000000
#define  SI_INT32_MAX   0x7FFFFFFF
#define  SI_INT32_MIN  -0x80000000

#define  SI_UINT64_MAX  0xFFFFFFFFFFFFFFFF
#define  SI_UINT64_MIN  0x0000000000000000
#define  SI_INT64_MAX   0x7FFFFFFFFFFFFFFF
#define  SI_INT64_MIN  -0x8000000000000000

#if defined(SI_ARCH_64_BIT)
	#define SI_USIZE_MAX  0xFFFFFFFFFFFFFFFF
	#define SI_USIZE_MIN  0x0000000000000000
	#define SI_ISIZE_MAX  0x7FFFFFFFFFFFFFFF
	#define SI_ISIZE_MIN -0x8000000000000000
#else
	#define SI_USIZE_MAX   0xFFFFFFFF
	#define SI_USIZE_MIN   0x00000000
	#define SI_ISIZE_MAX   0x7FFFFFFF
	#define SI_ISIZE_MIN  -0x80000000
#endif


SI_STATIC_ASSERT(sizeof(u8) == sizeof(i8));
SI_STATIC_ASSERT(sizeof(u16) == sizeof(i16));
SI_STATIC_ASSERT(sizeof(u32) == sizeof(i32));
SI_STATIC_ASSERT(sizeof(u64) == sizeof(i64));

SI_STATIC_ASSERT(sizeof(u8)  == 1);
SI_STATIC_ASSERT(sizeof(u16) == 2);
SI_STATIC_ASSERT(sizeof(u32) == 4);
SI_STATIC_ASSERT(sizeof(u64) == 8);

SI_STATIC_ASSERT(sizeof(usize) == sizeof(isize));


typedef float  f32;
typedef double f64;

SI_STATIC_ASSERT(sizeof(f32) == 4);
SI_STATIC_ASSERT(sizeof(f64) == 8);

#define SI_F32_MIN 1.17549435e-38f
#define SI_F32_MAX 3.40282347e+38f

#define SI_F64_MIN 2.2250738585072014e-308
#define SI_F64_MAX 1.7976931348623157e+308


#if !defined(bool)
	typedef u8   b8;
	typedef u16 b16;
	typedef u32 b32;
	typedef u64 b64;

	#ifndef __cplusplus
		typedef usize bool;

		#ifndef true
			#define true 1
		#endif
		#ifndef false
			#define false 0
		#endif
	#endif
#endif

#if !defined(nil)
	#define nil NULL
#endif

#if !defined(__cplusplus)
	#if defined(_MSC_VER) && _MSC_VER <= 1800
		#define inline __inline
	#elif !defined(__STDC_VERSION__)
		#define inline __inline__
	#else
		#define inline
	#endif
#endif

SI_STATIC_ASSERT(sizeof(uintptr) == sizeof(intptr));


#if !defined(rawptr)
	typedef void* rawptr;
#endif

#if !defined(cstr)
	typedef const char* cstring;
#endif

/*
	========================
	| Constants            |
	========================
*/
#define SI_OKAY  0
#define SI_ERROR -1

#define SI_BIT(x) (1 << (x))

#define SI_KILO(x) (       (x) * (usize)1024)
#define SI_MEGA(x) (SI_KILO(x) * (usize)1024)
#define SI_GIGA(x) (SI_MEGA(x) * (usize)1024)
#define SI_TERA(x) (SI_GIGA(x) * (usize)1024)

#define SI_BUILTIN_UNREACHABLE() __builtin_unreachable()

/*
	========================
	| Declaration macros   |
	========================
*/
#define SI_ENUM(type, name) type name; enum
#define SI_FUNC_PTR(var_name, ...) (*var_name)__VA_ARGS__
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
#define si_transmute(type, value)  *((type*)&(value))

/*
	========================
	| Unary operators      |
	========================
*/
#if !defined(typeof)
	#define typeof(value) __typeof__(value)
#endif
#if !defined(countof)
	#define countof(array) (sizeof(array) / sizeof(*(array)))
#endif
#if !defined(offsetof)
	#define offsetof(type, element) ((isize)&(((type*)nil)->element))
#endif
#if !defined(alignof)
	#define alignof(type) si_offsetof(struct { char c; type member; }, member)
#endif

/*
	========================
	| Generally functions  |
	========================
*/
#define si_swap(a, b) do { typeof((a)) tmp = (a); (a) = (b); (b) = tmp; } while (0)
#define si_abs(x) ((x) < 0 ? -(x) : (x))
#define si_between(x, lower, upper) (((lower) <= (x)) && ((x) <= (upper)))
#define si_pause() do { printf("Press any key to continue...\n"); getchar(); } while(0)

#define for_range_2(count_var, struct_name, ...) \
	siVector2D struct_name = __VA_ARGS__; \
	for (isize count_var = struct_name.x; count_var < struct_name.y; count_var += 1) \

#define for_range_1(count_var, line, ...) \
	for_range_2(count_var, SI_CAT(__si_impl_range_, line), __VA_ARGS__)

#define for_range(count_var, ...) \
	for_range_1(count_var, __LINE__, __VA_ARGS__)


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
#define si_init(size) do { SI_GLOBAL_ALLOC = si_allocator_init(size); } while(0)
#define si_terminate() do { si_allocator_free(SI_GLOBAL_ALLOC); } while(0)

/*
	========================
	| SI_ASSERT            |
	========================
*/
usize si_impl_assert_msg(bool condition, cstring condition_str, cstring file, i32 line, cstring message, ...);

#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
#define SI_ASSERT_MSG(condition, message) si_impl_assert_msg(condition, #condition, __FILE__, __LINE__, message, "")
#define SI_ASSERT_FMT(condition, message, ...) si_impl_assert_msg(condition, #condition, __FILE__, __LINE__, message, __VA_ARGS__)
#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL")
#define SI_PANIC() si_impl_assert_msg(false, "SI_PANIC()", __FILE__, __LINE__, message, "")
#define SI_PANIC_MSG(message) si_impl_assert_msg(false, "SI_PANIC(" message ")", __FILE__, __LINE__, message, "")


/*
	========================
	| siAny                |
	========================
*/
typedef struct siAny {
	rawptr ptr;
	usize type_size;
} siAny;

#define si_any_make(value) (siAny){&(typeof(value)){value}, sizeof(typeof(value))}
#define si_any_get(any, type) ((any.ptr != nil) ? *((type*)any.ptr) : (type)SI_ASSERT_NOT_NULL(any.ptr))

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
typedef struct siIVector2D {
	isize x;
	isize y;
} siIVector2D;

typedef struct siUVector2D {
	usize x;
	usize y;
} siUVector2D;

typedef siIVector2D siVector2D;


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

siAllocator* SI_GLOBAL_ALLOC;


inline siAllocator* si_allocator_init(usize bytes);
inline void si_allocator_refill(siAllocator* alloc);
inline void si_allocator_free(siAllocator* alloc);

inline rawptr si_malloc(siAllocator* alloc, usize bytes);
inline rawptr si_calloc(siAllocator* alloc, usize num, usize bytes);
inline rawptr si_realloc(siAllocator* alloc, rawptr ptr, usize old_size, usize new_size);


#if !defined(SI_MEMORY_LOGGING)
	#define malloc(bytes) si_malloc(SI_GLOBAL_ALLOC, bytes)
	#define calloc(num, bytes) si_calloc(SI_GLOBAL_ALLOC, num, bytes)
	#define realloc(ptr, old_size, new_size) si_realloc(SI_GLOBAL_ALLOC, ptr, old_size, new_size)
#endif

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

typedef struct siOptionalStruct {
	siAny value;
	bool has_value;
} siOptionalStruct;

#define siOptional(type) siOptionalStruct
const siOptionalStruct SI_OPTIONAL_NULL = {{nil, 0}, false};

#define si_optional_make(value) (siOptionalStruct){si_any_make(value), true}
#define si_optional_get(type, siOptional_var) si_any_get(siOptional_var.value, type)
#define si_optional_get_or_default(type, siOptional_var, default_value) (siOptional_var.has_value ? si_any_get(siOptional_var.value, type) : (type)default_value)
/* NOTE(EimaMei): If the `siOptional` variable is empty, it'll return the 'or_value'. The type of `or_value` must be the same as the type for the `siOptional` variable. */

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
	u32 len;
	u32 capacity;
	u16 type_size;
	u16 grow;
} siArrayHeader;

#define siArray(type) type*
#define SI_ARRAY_HEADER(array) ((siArrayHeader*)array - 1)

/**
 * @def foreach(variable_name, array)
 *
 * @brief Macro to iterate over elements of a siString.
 *
 * @param variable_name The name of the iterator variable.
 * @param array The siString to iterate over.
 */
#if !defined(foreach)
	#if !defined(SI_STANDARD_C89)
		#define foreach(variable_name, array) for (typeof(array) variable_name = array; variable_name != (typeof(array))si_array_get_ptr(array, si_array_len(array)); variable_name += 1)
	#else
		#define foreach(variable_name, array) typeof(array) variable_name; for (variable_name = array; variable_name != (typeof(array))si_array_get_ptr(array, si_array_len(array)); variable_name += 1)
	#endif
#endif

#define si_array_get_ptr(array, index) \
	(si_cast(siByte*, array) + si_array_type_size(array) * (index))

/* rawptr si_array_make(rawptr array); */
#define si_array_make(...) si_array_make_list(__VA_ARGS__, sizeof(*(__VA_ARGS__)), sizeof(__VA_ARGS__) / sizeof(*(__VA_ARGS__)))
rawptr si_array_make_list(rawptr list, usize sizeof_element, usize count);
rawptr si_array_copy(rawptr array);
rawptr si_array_make_reserve(usize sizeof_element, usize count);

usize si_array_len(rawptr array);
usize si_array_capacity(rawptr array);
usize si_array_type_size(rawptr array);
usize si_array_total_size(rawptr array);

siAny si_array_at(rawptr array, usize index);
siAny si_array_front(rawptr array);
siAny si_array_back(rawptr array);

#define si_array_append(array_ptr, value) si_impl_array_append(array_ptr, si_any_make(value))

#define si_array_find(array, value) si_impl_array_find(array, 0, si_array_len(array), si_any_make(value))
#define si_array_find_ex(array, start, end, value) si_impl_array_find(array, start, end, si_any_make(value))
#define si_array_rfind(array, value) si_impl_array_rfind(array, si_array_len(array) - 1, 0, si_any_make(value))
#define si_array_rfind_ex(array, start, end, value) si_impl_array_rfind(array, start, end, si_any_make(value))

#define si_array_replace(array_ptr, old_value, new_value) si_impl_array_replace(array, si_any_make(old_value), si_any_make(new_value))
void si_array_reverse(rawptr array_ptr);
char* si_array_to_sistring(siArray(char*) array, cstring separator);

void si_array_clear(rawptr array_ptr);
bool si_arrays_are_equal(rawptr lha, rawptr rha);

isize si_array_free(rawptr array);

/* NOTE(EimaMei): The actual implementations. Shouldn't be used in practice, really. */
isize si_impl_array_find(rawptr array, usize start, usize end, siAny value);
isize si_impl_array_rfind(rawptr array, usize start, usize end, siAny value);
void si_impl_array_append(rawptr array_ptr, siAny value);
void si_impl_array_replace(rawptr array_ptr, siAny old_value, siAny new_value);

#endif

#if !defined(SI_BUFFER_UNDEFINE)
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
	| siBuffer             |
	========================
*/
typedef struct siBufferHeader {
	usize len;
	u32 type_size;
} siBufferHeader;

#define SI_BUFFER_HEADER(buffer) (&(siBufferHeader){si_buffer_len(buffer), si_buffer_type_width(buffer)})

#define si_buffer_get_ptr(buffer, type_size, index) ((siByte*)buffer + type_size * index)


#define si_buffer_make(type, ...) (type[])__VA_ARGS__
#define si_buffer_make_siarray(buffer) si_buffer_make(si_array_len(type), buffer)
/* #define si_buffer_make */
#define si_buffer_make_reserve(type, count) ((type[count]){})

#define si_buffer_len(buffer) countof(buffer)
#define si_buffer_capacity(buffer) countof(buffer)
#define si_buffer_type_width(buffer) sizeof(*buffer)
#define si_buffer_total_size(buffer) (sizeof(buffer) * sizeof(*(buffer)))

#define si_buffer_at(buffer, index) si_impl_buffer_at(buffer, SI_BUFFER_HEADER(buffer), index)
#define si_buffer_front(buffer) si_impl_buffer_at(buffer, SI_BUFFER_HEADER(buffer), 0)
#define si_buffer_back(buffer) si_impl_buffer_at(buffer, SI_BUFFER_HEADER(buffer), countof(buffer) - 1)

#define si_buffer_find(buffer, value) si_impl_buffer_find(buffer, SI_BUFFER_HEADER(buffer), 0, countof(buffer), si_any_make(value))
#define si_buffer_find_ex(buffer, start, end, value) si_impl_buffer_rfind(buffer, SI_BUFFER_HEADER(buffer), start, end si_any_make(value))
#define si_buffer_rfind(buffer, value) si_impl_buffer_rfind(buffer, SI_BUFFER_HEADER(buffer), countof(buffer) - 1, 0, si_any_make(value))
#define si_buffer_rfind_ex(buffer, start, end, value) si_impl_buffer_rfind(buffer, SI_BUFFER_HEADER(buffer), start, end si_any_make(value))

#define si_buffer_replace(buffer, old_value, new_value) si_impl_buffer_replace(buffer, SI_BUFFER_HEADER(buffer), si_any_make(old_value), si_any_make(new_value))
#define si_buffer_reverse(buffer) si_impl_buffer_reverse(buffer, SI_BUFFER_HEADER(buffer))
#define si_buffer_to_sistring(buffer, separator) si_impl_buffer_to_sistring(buffer, SI_BUFFER_HEADER(buffer), separator)

#define si_buffer_sum_i64(buffer) si_impl_buffer_sum_i64(buffer, SI_BUFFER_HEADER(buffer))
#define si_buffer_sum_f64(buffer) si_impl_buffer_sum_f64(buffer, SI_BUFFER_HEADER(buffer))

#define si_buffer_clear(buffer) si_impl_buffer_clear(buffer, SI_BUFFER_HEADER(buffer))
#define si_buffer_cmp(buffer_left, buffer_right) si_impl_buffer_cmp(buffer_left, SI_BUFFER_HEADER(buffer_left), buffer_right, SI_BUFFER_HEADER(buffer_right))


/* NOTE(EimaMei): The actual implementations. Shouldn't be used in practice, really. */
inline siOptional(rawptr) si_impl_buffer_at(rawptr buffer, siBufferHeader* header, usize index);

isize si_impl_buffer_find(rawptr buffer, siBufferHeader* header, usize start, usize end, siAny value);
isize si_impl_buffer_rfind(rawptr buffer, siBufferHeader* header, usize start, usize end, siAny value);

isize si_impl_buffer_replace(rawptr buffer, siBufferHeader* header, siAny old_value, siAny new_value);
isize si_impl_buffer_reverse(rawptr buffer, siBufferHeader* header);
char* si_impl_buffer_to_sistring(rawptr buffer, siBufferHeader* header, cstring separator);

i64 si_impl_buffer_sum_i64(rawptr buffer, siBufferHeader* header);
f64 si_impl_buffer_sum_f64(rawptr buffer, siBufferHeader* header);

void si_impl_buffer_clear(rawptr buffer, siBufferHeader* header);
bool si_impl_buffer_cmp(rawptr buffer_left, siBufferHeader* header_left, rawptr buffer_right, siBufferHeader* header_right);
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
typedef siArray(char) siString;

/**
 * @typedef siStringHeader
 * @brief siStringHeader is the header structure for siString.
 */
typedef struct siStringHeader {
	u32 len;
	u32 capacity;
	u8 type_size;
	u8 grow;
} siStringHeader;


/**
 * @def SI_STRING_HEADER(str)
 * @brief Macro to get the header of a siString.
 * @param str The siString.
 * @return The header pointer of the siString (siDebugHeader*).
 */
#define SI_STRING_HEADER(str) ((siStringHeader*)str - 1)
#define SI_STRING_DEFAULT_GROW 128




/**
 * @brief Create a new siString from a null-terminated C string.
 *
 * @param str The null-terminated C string.
 *
 * @return The created siString.
 */
siString si_string_make(cstring str);

/**
 * @brief Create a new siString from a formatted null-terminated C string.
 *
 * @param str The formatted null-terminated C string.
 *
 * @return The created siString.
 */
siString si_string_make_fmt(cstring str, ...);

/**
 * @brief Create a new siString from a null-terminated C string with a specified length.
 *
 * @param str The null-terminated C string.
 * @param len The length of the string.
 *
 * @return The created siString.
 */
siString si_string_make_len(cstring str, usize len);

/**
 * @brief Create a new siString with a reserved capacity.
 *
 * @param len The reserved capacity for the siString.
 *
 * @return The created siString.
 */
siString si_string_make_reserve(usize len);



/**
 * @brief Create a copy of an existing siString.
 *
 * @param from The siString to copy.
 *
 * @return The copied siString.
 */
siString si_string_copy(siString from);


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
 * @brief Get a substring of a siString and stores it in the provided buffer.

 * @param str The siString from which to extract the substring.
 * @param begin The starting index of where to extract.
 * @param len The length of the substring.
 * @param result A pointer to a buffer to store the substring. The buffer should
 *               have a size of at least `len + 1` to accommodate the substring
 *               characters and the NULL terminator.
 *
 * @attention The result buffer must be large enough to hold the substring,
 *            including the NULL terminator. See `param result` for more.
 */
void si_string_sub(siString str, usize begin, usize len, char* result);


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
 * @brief Pushes [appends] the character into the siString.
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
void si_string_swap(siString* str, cstring cstr1, cstring cstr2);

void si_string_strip(siString* str); /* NOTE(EimaMei); This strips any leading or trailing spaces in the string. */
void si_string_reverse(siString* str);
void si_string_reverse_len(siString* str, usize len);

siArray(siString) si_string_split(siString str, cstring separator);
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

cstring si_u64_to_cstr(u64 num);
cstring si_i64_to_cstr(i64 num);
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
	siString path;
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
siString si_path_get_fullname(cstring path);

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

siString si_file_read(siFile file);
siString si_file_read_at(siFile file, usize offset);
siArray(siString) si_file_readlines(siFile file);

isize si_file_write(siFile* file, cstring content);
isize si_file_write_len(siFile* file, cstring content, usize len);
isize si_file_write_at(siFile* file, cstring content, usize offset);
isize si_file_write_at_line(siFile* file, cstring content, usize index);
isize si_file_write_at_len(siFile* file, cstring content, usize content_len, usize offset);

isize si_file_seek(siFile file, usize offset);
isize si_file_seek_to_end(siFile file);

isize si_file_close(siFile file);

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


siThread si_thread_create(siFunction function, rawptr arg);

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

/* NOTE(EimaMi): For whatever reason some places uses Sunday as the first weekday, while MOST of Europe views monday as the first weekday.
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
u64 si_time_utc_now(void) {
	time_t rawtime;
	time(&rawtime);

	return rawtime;
}
void si_sleep(usize miliseconds);

#endif

#if !defined(SI_PERFORMANCE_UNDEFINE)

#if !defined(SI_MEMORY_LOGGING)
	#define SI_PERFORMANCE_MSG SI_MULTILINE(\
		"====== BENCHMARK DATA ======\n"\
		"General:\n"\
			"\tFunction - '%s'\n"\
			"\tDuration - '%f' ms\n"\
			"\tExecution count - '%f' million\n"\
	)
	#define SI_PERFORMANCE_PRINT_MEMORY(function) do {} while (0)
#else
	#define SI_PERFORMANCE_MSG SI_MULTILINE(\
		"====== BENCHMARK DATA ======\n"\
		"NOTE: General execution count might be slower due to the memory logging.\n"\
		"General:\n"\
			"\tFunction - '%s'\n"\
			"\tDuration - '%f' ms\n"\
			"\tExecution count - '%f' million\n"\
	)
	#define SI_PERFORMANCE_MSG_DEBUG SI_MULTILINE(\
		"Memory:\n"\
			"\tAllocation count - '%zd'\n"\
			"\tFree count - '%zd'\n"\
			"\tTotal RAM usage - '%zd' bytes\n"\
			"\tOverall allocated RAM left - '%zd' bytes\n"\
	)
	#define SI_PERFORMANCE_PRINT_MEMORY(function)\
		do {\
			siDebugGlobalVars si_debug_old = si_debug;\
			si_debug = (siDebugGlobalVars){0, 0, 0, 0, nil};\
			function;\
			si_debug_print_all();\
			si_debug_print_allocations();\
			si_debug_cleanup();\
			si_debug = si_debug_old;\
		} while(0)
#endif

void si_impl_performance_thread(u32* arg);

#define si_performance_run_per_loop(times_to_loop, function)\
	do {\
		usize si_index;\
		clock_t si_time_since = clock();\
		for (si_index = 0; si_index < (usize)times_to_loop; si_index++) {\
			function;\
		}\
		si_time_since = clock() - si_time_since;\
		f64 si_time_taken = ((f64)si_time_since) / 1000; /* NOTE(EimaMei): This will take the time in ms, not seconds. */\
		printf(SI_PERFORMANCE_MSG, #function, si_time_taken, (f64)times_to_loop / 1000000.0f);\
		SI_PERFORMANCE_PRINT_MEMORY(function);\
	} while(0)

#define si_performance_executes_per_ms(ms, function)\
	do {\
		u64 si_counter = 0;\
		u32 si_miliseconds = (u32)ms;\
		siThread si_thread = si_thread_create(siFunc(si_impl_performance_thread), &si_miliseconds);\
		si_thread_start(&si_thread);\
		while (si_thread.is_running) {\
			function;\
			si_counter += 1;\
		}\
		printf(SI_PERFORMANCE_MSG, #function, (f64)si_miliseconds, (f64)si_counter / 1000000.0f);\
		SI_PERFORMANCE_PRINT_MEMORY(function);\
	} while(0)

#define si_performance_loop_median(increments_of_10, function)\
	do { \
		usize si_array_len = 0; \
		usize si_num = increments_of_10; \
		while (si_num) { \
			si_num /= 10; \
			si_array_len += 1; \
		} \
		f64 si_exec_durations[si_array_len]; \
		\
		usize si_median_index, si_array_index = 0; \
		for (si_median_index = 1; si_median_index < 10 * increments_of_10; si_median_index *= 10) { \
			usize si_index = 0; \
			clock_t si_begin = clock(); \
			for (; si_index < si_median_index; si_index++) { \
				function; \
			} \
			si_begin = clock() - si_begin; \
			si_exec_durations[si_array_index] = (f64)(si_begin) / 1000.0; /* NOTE(EimaMei): This will take the time in ms, not seconds. */\
			si_array_index += 1; \
		} \
		printf( \
			"====== BENCHMARK DATA ======\n"\
			"General:\n"\
				"\tFunction - '%s'\n" \
			"Runs:\n", \
			#function \
		); \
		si_array_index = 0; \
		for (si_median_index = 1; si_median_index < 10 * increments_of_10; si_median_index *= 10) { \
			printf("\t%zd run(s) - '%f' ms\n", si_median_index, si_exec_durations[si_array_index]); \
			si_array_index += 1; \
		} \
		printf("Final result:\n\tMedian - '%f' ms\n", si_buffer_sum_f64(si_exec_durations) / (f64)countof(si_exec_durations)); \
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
	#define SI_BUFFER_IMPLEMENTATION
	#define SI_STRING_IMPLEMENTATION
	#define SI_CHAR_IMPLEMENTATION
	#define SI_FILE_IMPLEMENTATION
	#define SI_THREAD_IMPLEMENTATION
	#define SI_PERFORMANCE_IMPLEMENTATION
#endif

#if defined(SI_GENERAL_IMPLEMENTATION)

usize si_impl_assert_msg(bool condition, cstring condition_str, cstring file, i32 line, cstring message, ...) {
	if (condition) {
		return 0;
	}


	fprintf(stderr, "Assertion \"%s\" at \"%s:%d\"%s", condition_str, file, line, (message != nil ? ": " : ""));

	if (message != nil) {
		va_list va;

		va_start(va, message);
		vprintf(message, va);
		puts("");
		va_end(va);
	}

	abort();

	return 1;
}

#if defined(SI_MEMORY_LOGGING)
	#undef malloc
	#undef si_realloc
	#undef free
#endif

rawptr si_realloc_cpy(rawptr ptr, usize old_size, usize new_size) {
	if (ptr == nil) {
		return malloc(new_size);
	}

	if (old_size == new_size) {
		return ptr;
	}
	else if (new_size < old_size) {
		new_size = old_size;
	}

	rawptr new_ptr = malloc(new_size);
	SI_ASSERT_NOT_NULL(new_ptr);

	memcpy(new_ptr, ptr, old_size);
	free(ptr);

	return new_ptr;
}

#if defined(SI_MEMORY_LOGGING)
	#define malloc(size) si_debug_alloc(1, nil, (siDebugArgs){size, 0}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
	#define si_realloc(ptr, old_size, new_size) si_debug_alloc(4, ptr, (siDebugArgs){old_size, new_size}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
	#define free(ptr) si_debug_free(ptr)
#endif

void si_sleep(usize miliseconds) {
	#if defined(SI_SYSTEM_WINDOWS)
		Sleep(miliseconds);
	#else
		usleep(miliseconds * 1000);
	#endif
}

void si_ptr_move_by(rawptr src, usize src_len, usize move_by, siDirection direction) {
	switch (direction) { /* NOTE(EimaMei): Did you really think I was gonna use 'memmove'? That's much slower! */
		case SI_DIRECTION_RIGHT: memcpy((siByte*)src + move_by, src, src_len); break;
		case SI_DIRECTION_LEFT:  memcpy((siByte*)src - move_by, src, src_len); break;
		default: SI_BUILTIN_UNREACHABLE();
	}
}

#endif

#if defined(SI_ALLOCATOR_IMPLEMENTATION) && !defined(SI_ALLOCATOR_UNDEFINE)

#if !defined(SI_MEMORY_LOGGING)
	#undef malloc
	#undef calloc
	#undef realloc
#endif

inline siAllocator* si_allocator_init(usize bytes) {
	rawptr ptr = malloc(sizeof(siAllocator) + bytes);

	siAllocator* res = (rawptr)ptr;
	res->ptr = (siByte*)ptr + sizeof(siAllocator);
	res->index = 0;
	res->max_size = bytes;

	return res;
}
inline void si_allocator_refill(siAllocator* alloc) {
	SI_ASSERT_MSG(alloc->ptr == nil, "Specified allocator hasn't been freed once, cannot refill.");
	*alloc = *si_allocator_init(alloc->max_size);
}
inline void si_allocator_free(siAllocator* alloc) {
	free(alloc->ptr - sizeof(siAllocator));
}

inline rawptr si_malloc(siAllocator* alloc, usize bytes) {
	SI_ASSERT_NOT_NULL(alloc);

	siByte* res = alloc->ptr + alloc->index;
	alloc->index += bytes;

	SI_ASSERT_FMT(alloc->index <= alloc->max_size, "Exceeded the available memory for allocation (Tried writing '%zd' bytes into a already filled allocator with '%zd' bytes. Current index: '%zd').", bytes, alloc->max_size, alloc->index);

	return res;
}

inline rawptr si_calloc(siAllocator* alloc, usize num, usize bytes) {
	rawptr res = si_malloc(alloc, num * bytes);
	memset(res, 0, num * bytes);

	return res;
}

inline void si_free(siAllocator* alloc, rawptr ptr) {
	SI_ASSERT_NOT_NULL(alloc);
	SI_ASSERT_NOT_NULL(ptr);
	SI_ASSERT(si_between((siByte*)ptr, alloc->ptr, alloc->ptr + alloc->max_size));

	alloc->index = (siByte*)ptr - alloc->ptr;
}
inline rawptr si_realloc(siAllocator* alloc, rawptr ptr, usize new_size, usize old_size) {
	SI_ASSERT_NOT_NULL(alloc);

    if (ptr == nil) {
        return si_malloc(alloc, new_size);
    }
	SI_ASSERT(si_between((siByte*)ptr, alloc->ptr, alloc->ptr + alloc->max_size));

    isize copy_size = (new_size < old_size) ? new_size : old_size;

    if (new_size == 0) {
        si_free(alloc, ptr);
        return nil;
    }

    isize size_diff = new_size - old_size;

    if (size_diff > 0) {
        rawptr new_ptr = si_malloc(alloc, new_size);
        memcpy(new_ptr, ptr, copy_size);
        si_free(alloc, ptr);

        return new_ptr;
    }

    usize old_index = (siByte*)ptr - alloc->ptr + copy_size;
    alloc->index = old_index + copy_size;
    return ptr;
}


#if !defined(SI_MEMORY_LOGGING)
	#define malloc(bytes) si_malloc(SI_GLOBAL_ALLOC, bytes)
	#define calloc(num, bytes) si_calloc(SI_GLOBAL_ALLOC, num, bytes)
	#define realloc(ptr, old_size, new_size) si_realloc(SI_GLOBAL_ALLOC, ptr, old_size, new_size)
#endif

#endif

#if defined(SI_PAIR_IMPLEMENTATION) && !defined(SI_PAIR_UNDEFINE)

rawptr si_impl_pair_make(siAny first, siAny second) {
	siByte* res = (siByte*)malloc(first.type_size + second.type_size);
	memcpy(res, first.ptr, first.type_size);
	memcpy(res + first.type_size, second.ptr, second.type_size);

	return (rawptr)res;
}
#endif

#if defined(SI_ARRAY_IMPLEMENTATION) && !defined(SI_ARRAY_UNDEFINE)

inline rawptr si_array_make_list(rawptr list, usize sizeof_element, usize count)  {
	rawptr array = si_array_make_reserve(sizeof_element, count);
	memcpy(array, list, sizeof_element * count);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	header->len = count;

	return array;
}
inline rawptr si_array_copy(rawptr array) {
	siArrayHeader* header = SI_ARRAY_HEADER(array);
	return si_array_make_list(array, header->type_size, header->len);
}
rawptr si_array_make_reserve(usize sizeof_element, usize count) {
	siByte* ptr = malloc(sizeof(siArrayHeader) + (sizeof_element * count));
	rawptr array = ptr + sizeof(siArrayHeader);

	siArrayHeader* header = (siArrayHeader*)ptr;
	header->len = 0;
	header->capacity = count;
	header->type_size = sizeof_element;

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

inline siAny si_array_at(rawptr array, usize index) {
	if (index >= si_array_len(array)) {
		return (siAny){nil, 0};
	}

	rawptr p = si_array_get_ptr(array, index);
	return (siAny){p, si_array_type_size(array)};
}
inline siAny si_array_front(rawptr array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	return (siAny){array, si_array_type_size(array)};
}
inline siAny si_array_back(rawptr array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	rawptr p = (siByte*)array + (si_array_len(array) - 1) * si_array_type_size(array);
	return (siAny){p, si_array_type_size(array)};
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
	SI_ASSERT_MSG(start > end, "Value 'end' is larger than 'start'");

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
	static rawptr tmp[1];
	len *= 0.5;

	while (len--) {
		memcpy(tmp, a, type_size);
		memcpy(a, b, type_size);
		memcpy(b, tmp, type_size);

		a += type_size, b -= type_size;
	}
}
siString si_array_to_sistring(siArray(char*) array, cstring separator) {
	SI_ASSERT_NOT_NULL(array);

	usize separator_len = si_cstr_len(separator);
	usize total_size = 0;

	foreach (str, array) {
		total_size += si_cstr_len(*str) + separator_len;
	}

	siString result = si_string_make_reserve(total_size);
	rawptr back_ptr = si_array_back(array).ptr;
	foreach (str, array) {
		if (separator != nil && (&str) != back_ptr) {
			si_string_join(&result, separator, *str);
		}
		else {
			si_string_append(&result, *str);
		}
	}

	return result;
}

void si_impl_array_append(rawptr array_ptr, siAny value) {
	SI_ASSERT_NOT_NULL(array_ptr);
	siByte* array = *si_cast(siByte**, array_ptr);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	SI_ASSERT_MSG(value.type_size <= header->type_size, "The given value's sizeof is too large compared to the elements' in the arra");

	if (header->len == header->capacity) {
		if (header->capacity == 0) {
			header->capacity = 1;
		}

		siByte* result = realloc(header, sizeof(siArrayHeader) + header->capacity * header->type_size, sizeof(siArrayHeader) + 2 * header->capacity * header->type_size);
		SI_ASSERT_NOT_NULL(result);

		array = result + sizeof(siArrayHeader);
		*si_cast(siByte**, array_ptr) = array;

		header = (siArrayHeader*)result;
		header->capacity *= 2;
	}

	memcpy(si_array_get_ptr(array, header->len), value.ptr, header->type_size);
	header->len += 1;
}

void si_array_clear(rawptr array_ptr) {
	SI_ASSERT_NOT_NULL(array_ptr);
	rawptr array = *si_cast(siByte**, array_ptr);

	memset(array, 0, si_array_total_size(array));
}

bool si_arrays_are_equal(rawptr lha, rawptr rha) {
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

isize si_array_free(rawptr array) {
	if (array == nil) {
		return SI_ERROR;
	}

	free(SI_ARRAY_HEADER(array));

	return SI_OKAY;
}

#endif

#if defined(SI_BUFFER_IMPLEMENTATION) && !defined(SI_BUFFER_UNDEFINE)

inline siOptional(rawptr) si_impl_buffer_at(rawptr buffer, siBufferHeader* header, usize index) {
	if (index >= header->len) {
		return SI_OPTIONAL_NULL;
	}
	return (siOptionalStruct){(siAny){si_buffer_get_ptr(buffer, header->type_size, index), header->type_size}, true};
}

isize si_impl_buffer_find(rawptr buffer, siBufferHeader* header, usize start, usize end, siAny value) {
	SI_ASSERT_MSG(value.type_size <= header->type_size, "The given value's sizeof is too large compared to the elements' in the buffer.");
	SI_ASSERT_MSG(start < end, "Value 'start' is larger than 'end'");

	bool found = false;
	usize i;
	for (i = start; i < end; i++) {
		if (memcmp((siByte*)buffer + i * header->type_size, value.ptr, value.type_size) == 0) {
			found = true;
			break;
		}
	}

	return (found ? (isize)i : SI_ERROR);
}
isize si_impl_buffer_rfind(rawptr buffer, siBufferHeader* header, usize start, usize end, siAny value) {
	SI_ASSERT_MSG(value.type_size <= header->type_size, "The given value's sizeof is too large compared to the elements' in the buffer.");
	SI_ASSERT_MSG(start > end, "Value 'end' is larger than 'start'");

	bool found = false;
	usize i;
	for (i = start; i < end - 1; i--) {
		if (memcmp((siByte*)buffer + i * header->type_size, value.ptr, value.type_size) == 0) {
			found = true;
			break;
		}
	}

	return (found ? (isize)i : SI_ERROR);
}

isize si_impl_buffer_replace(rawptr buffer, siBufferHeader* header, siAny old_value, siAny new_value) {
	SI_ASSERT_MSG(si_between(new_value.type_size, old_value.type_size, header->type_size), "The given value's sizeof is too large compared to the elements' in the buffer.");

	isize index = 0;
	while (true) {
		index = si_impl_buffer_find(buffer, header, index, header->len, old_value);
		if (index == SI_ERROR) {
			return SI_ERROR;
		}

		memcpy((siByte*)buffer + index * header->type_size, new_value.ptr, new_value.type_size);
	}

	return index;
}
isize si_impl_buffer_reverse(rawptr buffer, siBufferHeader* header) {
	usize len = header->len;
	usize type_size = header->type_size;
	SI_ASSERT_MSG(len != 0, "Buffer is empty");

	siByte* a = (siByte*)buffer;
	siByte* b = (siByte*)buffer + (len - 1) * type_size;
	static rawptr tmp[1];
	len *= 0.5;

	while (len--) {
		memcpy(tmp, a, type_size);
		memcpy(a, b, type_size);
		memcpy(b, tmp, type_size);

		a += type_size, b -= type_size;
	}

	return SI_OKAY;
}
siString si_impl_buffer_to_sistring(rawptr buffer, siBufferHeader* header, cstring separator) {
	usize separator_len = si_cstr_len(separator);
	usize total_size = 0;

	for_range (x, {0, header->len}) {
		cstring cstr = *si_cast(char**, si_buffer_get_ptr(buffer, header->type_size, x));
		total_size += si_cstr_len(cstr) + separator_len;
	}

	siString result = si_string_make_reserve(total_size);

	isize back_index = header->len - 1;
	for_range (i, {0, header->len}) {
		cstring cstr = *si_cast(char**, si_buffer_get_ptr(buffer, header->type_size, i));

		if (separator != nil && i != back_index) {
			si_string_join(&result, separator, cstr);
		}
		else {
			si_string_append(&result, cstr);
		}
	}

	return result;
}

inline i64 si_impl_buffer_sum_i64(rawptr buffer, siBufferHeader* header) {
	i64 res = 0;
	usize i;
	for (i = 0; i < header->len; i++) {
		res += *si_buffer_get_ptr(buffer, header->type_size, i);
	}

	return res;
}
inline f64 si_impl_buffer_sum_f64(rawptr buffer, siBufferHeader* header) {
	f64 res = 0;
	usize i;
	for (i = 0; i < header->len; i++) {
		f64 num = *si_cast(f64*, si_buffer_get_ptr(buffer, header->type_size, i));
		res += num;
	}

	return res;
}

inline void si_impl_buffer_clear(rawptr buffer, siBufferHeader* header) {
	rawptr result = memset(buffer, 0, header->len * header->type_size);
	SI_ASSERT_NOT_NULL(result);
}
bool si_impl_buffer_cmp(rawptr buffer_left, siBufferHeader* header_left, rawptr buffer_right, siBufferHeader* header_right) {
	if (header_left->len != header_right->len || header_left->type_size != header_right->type_size) {
		return false;
	}

	usize width = header_left->type_size;
	usize i;
	for (i = 0; i < header_left->len; i++) {
		if (memcmp((siByte*)buffer_left + i * width, (siByte*)buffer_right + i * width, width) != SI_OKAY) {
			return false;
		}
	}

	return true;
}

#endif

#if defined(SI_STRING_IMPLEMENTATION) && !defined(SI_STRING_UNDEFINE)


inline siString si_string_make(cstring str) {
	return si_string_make_len(str, str ? si_cstr_len(str) : 0);
}
siString si_string_make_len(cstring str, usize len) {
	siString res_str = si_string_make_reserve(len);
	memcpy(res_str, str, len);
	res_str[len] = '\0';

	siStringHeader* header = SI_STRING_HEADER(res_str);
	header->len = len;

	return res_str;
}
siString si_string_make_fmt(cstring str, ...) {
	va_list va;
	va_start(va, str);

	isize size = vsnprintf(nil, 0, str,  va);
	char buf[size + 1];
	vsnprintf(buf, size + 1, str, va);

	va_end(va);

	if (size < SI_OKAY) {
		return nil;
	}

	return si_string_make_len(buf, size);
}
siString si_string_make_reserve(usize len) {
	rawptr ptr = malloc(sizeof(siStringHeader) + (len + 1));
	SI_ASSERT_NOT_NULL(ptr);

	siString res_str = (siString)ptr + sizeof(siStringHeader);
	res_str[len] = '\0';

	siStringHeader* header = SI_STRING_HEADER(res_str);
	header->len = 0;
	header->capacity = len;
	header->type_size = sizeof(char);

	return res_str;
}

inline siString si_string_copy(siString from) {
	return si_string_make_len(from, si_string_len(from));
}

inline usize si_string_len(siString str) {
	return SI_STRING_HEADER(str)->len;
}
inline usize si_string_capacity(siString str) {
	return SI_STRING_HEADER(str)->capacity;
}
inline usize si_string_type_size(siString str) {
	return SI_STRING_HEADER(str)->type_size;
}
inline bool si_string_empty(siString str) {
	return (SI_STRING_HEADER(str)->len == 0);
}


inline usize si_cstr_len(cstring str) {
	cstring s;
	for (s = str; *s; s++);

	return (s - str);
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
void si_string_sub(siString str, usize begin, usize len, char* result) {
	memcpy(result, str + begin, len);
	*(result + len) = '\0';
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

	char tmp[separator_len + cstr_len + 1];
	memcpy(tmp, separator, separator_len);
	memcpy(tmp + separator_len, cstr, cstr_len);
	tmp[separator_len + cstr_len] = '\0'; /* TODO(EimaMei): Pretty sure I can safely remove this. */

	si_string_append_len(str, tmp, separator_len + cstr_len);
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
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize previous_len = header->len;
	header->len += other_len;

	if (header->capacity < header->len) {
		si_string_make_space_for(str, other_len);
		header = SI_STRING_HEADER(*str);
	}

	rawptr ptr = memcpy(*str + previous_len, other, other_len);
	SI_ASSERT_NOT_NULL(ptr);

	(*str)[header->len] = '\0';
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
	header->len += cstr_len - erase_index;

	if (header->capacity < header->len) {
		si_string_make_space_for(str, header->len - header->capacity);
		header = SI_STRING_HEADER(*str); /* NOTE(EimaMei): For some reason we have to refresh the header pointer on Linux. Somewhat makes sense but also what and why. */
	}
	siString cur_str = *str;

	char* ptr = (char*)memcpy(cur_str + header->len - before_index_len, cur_str + index, before_index_len);
	memcpy(cur_str + index, cstr, cstr_len);
	SI_ASSERT_NOT_NULL(ptr);
	ptr[before_index_len] = '\0';
}
void si_string_erase(siString* str, usize index, usize erase_len) {
	siString cur_str = *str;
	SI_ASSERT_NOT_NULL(cur_str);

	usize str_len = si_string_len(cur_str);
	SI_ASSERT_MSG(index < str_len, "Index is higher than the length of the string.");

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

	bool change = false;
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
	char* stripped_str = cur_str + start * SI_STRING_HEADER(cur_str)->type_size;
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
		a++, b--;
	}

	*str = actual_str;
}

siArray(siString) si_string_split(siString str, cstring separator) {
	SI_ASSERT_NOT_NULL(str);

	usize str_len = si_string_len(str);
	usize separator_len = si_cstr_len(separator);

	usize count = 0;
	usize begin_pos = 0;
	usize pos_buffer[str_len];

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
	siArray(siString) res = si_array_make_reserve(sizeof(*res), count);
	SI_ARRAY_HEADER(res)->len = count;

	for_range (i, {0, count}) {
		res[i] = si_string_make_len(str, pos_buffer[i]);
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
	if (str == nil) {
		return ;
	}
	free(SI_STRING_HEADER(str));
}
void si_string_make_space_for(siString* str, usize add_len) {
	usize len = si_string_len(*str);

	rawptr ptr = SI_STRING_HEADER(*str);
	usize old_size = sizeof(siStringHeader) + len + 1;
	usize new_size = old_size + add_len;

	siStringHeader* new_ptr = realloc(ptr, old_size, new_size);
	SI_ASSERT_NOT_NULL(new_ptr);

	*str = (char*)new_ptr + sizeof(siStringHeader);
	new_ptr->capacity += add_len;
}
void si_string_shrink_to_fit(siString* str) {
	SI_ASSERT_NOT_NULL(str);
	siString cur_str = *str;
	siStringHeader copy = *SI_STRING_HEADER(cur_str);

	siStringHeader* header = realloc(SI_STRING_HEADER(cur_str), sizeof(siStringHeader*) + copy.capacity + 1, sizeof(siStringHeader*) + copy.len + 1);
	*header = copy;
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

cstring si_u64_to_cstr(u64 num) {
	static char buffer[20 + 1]; /* NOTE(EimaMei): 20 chars is the maximum of numbers we can have in an u64. */
	char* cur_char = buffer;

	do {
		*cur_char++ = (num % 10) +'0';
		num /= 10;
	} while (num > 0);
	*cur_char = '\0';

	char* res = buffer;
	si_string_reverse_len(&res, (u64)(cur_char - buffer));

	return res;
}
u64 si_cstr_to_u64(cstring str) {
	SI_ASSERT_NOT_NULL(str);

	u64 result = 0;
	char cur;
	while ((cur = *str++)) {
		if (cur >= '0' && cur <= '9') {
			result = (result * 10) + (cur - '0');
		}
		else {
			SI_ASSERT_MSG(!(cur >= '0' && cur <= '9'), "Attempted to use `si_cstr_to_u64` with a string that contains non numbers.");
		}
	}

	return result;
}
cstring si_i64_to_cstr(i64 num) {
	static char buffer[20 + 1]; /* NOTE(EimaMei): 19 chars is the maximum of numbers we can have in an i64. One more char for the possible minus symbol. */
	char* cur_char = buffer;
	bool negative = false;

	u64 value;
	if (num < 0) {
		negative = true;
		num = -num;
	}
	value = (u64)num;

	do {
		*cur_char++ = (num % 10) +'0';
		value /= 10;
	} while (value > 0);

	if (negative) {
		*cur_char++ = '-';
	}

	*cur_char = '\0';

	char* res = buffer;
	si_string_reverse_len(&res, (cur_char - buffer));

	return res;
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
			SI_ASSERT_MSG(!(cur >= '0' && cur <= '9'), "Attempted to use `si_cstr_to_u64` with a string that contains non numbers.");
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
inline siString si_path_get_fullname(cstring path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_SYSTEM_WINDOWS)
		char buffer[MAX_PATH];
		GetFullPathNameA(path, MAX_PATH, buffer, nil);

		return si_string_make(buffer);
	#else
		char actual_path[4096];
		realpath(path, actual_path);

		return si_string_make(actual_path);
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
		siString message = nil;
		switch (errno) {
			case ENOENT: message = "Failed to read the provided file, as it doesn't exist."; break;
			case EACCES: message = "Failed to read the provided file, as I don't have suffice permissions to read the file."; break;
			case EINVAL: message = "Failed to read the provided file, as an invalid argument was given."; break;
			default: message = "Failed to read the provided file due to an unspecified reason."; break;
		}

		SI_ASSERT_MSG(f != nil, message);
	}

	siFile result = {
		si_path_get_fullname(path),
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

inline siString si_file_read(siFile file) {
	SI_ASSERT_NOT_NULL(file.ptr);

	char tmp[file.size];
	fread(tmp, file.size, 1, file.ptr);

	siString txt = si_string_make_len(tmp, file.size);

	return txt;
}
inline siString si_file_read_at(siFile file, usize offset) {
	si_file_seek(file, offset);
	file.size -= offset;
	siString res = si_file_read(file);
	file.size += offset;
	si_file_seek_to_end(file);

	return res;
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
siArray(siString) si_file_readlines(siFile file) {
	siString buffer = si_file_read_at(file, 0);
	siArray(siString) res = si_string_split(buffer, "\n");
	//si_string_free(buffer);

	return res;
}

inline isize si_file_write(siFile* file, cstring content) {
	return si_file_write_at_len(file, content, si_cstr_len(content), 0);
}
inline isize si_file_write_len(siFile* file, cstring content, usize len) {
	return si_file_write_at_len(file, content, len, 0);
}
inline isize si_file_write_at(siFile* file, cstring content, usize offset) {
	return si_file_write_at_len(file, content, si_cstr_len(content), offset);
}
isize si_file_write_at_len(siFile* file, cstring content, usize content_len, usize offset) {
	SI_ASSERT_NOT_NULL(file->ptr);
	SI_ASSERT_NOT_NULL(content);

	si_file_seek(*file, offset);
	isize result = fwrite(content, content_len, 1, file->ptr);
	si_file_seek_to_end(*file);

	si_file_size_update(file);
	return result;
}
isize si_file_write_at_line(siFile* file, cstring content, usize index) {
	SI_ASSERT_NOT_NULL(file->ptr);

	siArray(siString) buffer = si_file_readlines(*file);
	//siString previous_line = buffer[index];
	SI_ASSERT_MSG(index < si_array_len(buffer), "Index is either not 0 or higher than the amount of lines in the file.");


	buffer[index] = (siString)content;
	siString new_file_content = si_array_to_sistring(buffer, "\n");

	siFile new_file = si_file_create(file->path);
	si_file_write_len(&new_file, new_file_content, si_string_len(new_file_content));

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
	*file = new_file;

	return SI_OKAY;
}

inline isize si_file_seek(siFile file, usize offset) {
	SI_ASSERT_NOT_NULL(file.ptr);
	return fseek(file.ptr, offset, SEEK_SET);
}
inline isize si_file_seek_to_end(siFile file) {
	SI_ASSERT_NOT_NULL(file.ptr);
	return fseek(file.ptr, 0, SEEK_SET);
}

isize si_file_close(siFile file) {
	if (file.path != nil) {
		// si_string_free(file.path);
	}

	if (file.ptr != nil) {
		fclose(file.ptr);
	}

	return SI_OKAY;
}

#endif

#if defined(SI_THREAD_IMPLEMENTATION) && !defined(SI_THREAD_UNDEFINE)

inline static rawptr si_impl_thread_proc(rawptr arg) {
	siThread* t = (siThread*)arg;
	t->return_value = t->func.ptr(t->arg);
	t->is_running = false;

	return nil;
}

inline siThread si_thread_create(siFunction function, rawptr arg) {
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
			case EAGAIN:  error_msg = "The system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process PTHREAD_THREADS_MAX would be exceeded."; break;
			case EINVAL:  error_msg = "The value specified by attr is invalid at 'pthread_create'."; break;
			case EPERM:   error_msg = "The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy."; break;
			default:      error_msg = si_string_make_fmt("Unknown error code (%li).", error_code);
		}
		SI_ASSERT_MSG(error_code == SI_OKAY, error_msg);
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
			default:      error_msg = si_string_make_fmt("Unknown error code (%li).", error_code);
		}
		SI_ASSERT_MSG(error_code == SI_OKAY, error_msg);
	#endif

	t->is_running = false;
}
void si_thread_cancel(siThread* t) {
	#if defined(SI_SYSTEM_WINDOWS)
		puts("si_thread_cancel: This feature on Windows is not supported as of now.");
		SI_UNUSED(t);
	#else

		usize error_code = pthread_cancel(t->id);
		t->is_running = false;

		cstring error_msg = nil;
		switch (error_code) {
			case SI_OKAY:   break;
			case ESRCH:     error_msg = "No thread could be found corresponding to that specified by the given thread ID."; break;
			default:        error_msg = si_string_make_fmt("Unknown error code (%li).", error_code);
		}
		SI_ASSERT_MSG(error_code == SI_OKAY, error_msg);

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
			case EPERM:    error_msg = "The caller does not have the appropriate permission to set the scheduling policy of the specified thread OR the implementation does not allow the application to modify the priority to the value specified."; break;
			case ESRCH:    error_msg = "The value specified by thread does not refer to an existing thread."; break;
			default:       error_msg = si_string_make_fmt("Unknown error code (%li).", error_code);
		}
		SI_ASSERT_MSG(error_code == SI_OKAY, error_msg);
	#else
		SI_PANIC_MSG("si_thread_set_priority: Not supported on MacOS.");
		SI_UNUSED(t);
		SI_UNUSED(priority);
	#endif
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
		default: __builtin_unreachable();
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

	foreach (x, si_debug.list) {
		siDebugHeader* header = *x;

		if (header == nil) {
			continue;
		}
		cstring msg = nil;
		switch (header->state) {
			case 0: msg = "NOT FREED"; break;
			case 1: msg = "FREED"; break;
			case 2: msg = "REALLOCED"; break;
			default: __builtin_unreachable();
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

	foreach (x, si_debug.list) {
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

#if defined(__cplusplus)
}
#endif

#endif /* SI_IMPLEMENTATION */

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
