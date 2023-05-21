/*
sili.h - An alternative to the C/C++ standard library.
===========================================================================
	- YOU MUST DEFINE 'SI_IMPLEMENTATION' in EXACTLY _one_ C file that includes this header, BEFORE the
	include like this:

		#define SI_IMPLEMENTATION
		#include "sili.h"

	- All other files should just include the library without the #define macro.


	- If you want to disable certain features, you can do:
		- #define SI_PAIR_UNDEFINE
		- #define SI_ARRAY_UNDEFINE
		- #define SI_STRING_UNDEFINE
		- #define SI_CHAR_UNDEFINE
	before the SI_IMPLEMENTATION macro, as well as before any other include of `sili.h`.

===========================================================================

LICENSE
	- This software is dual-licensed to the public domain and under the MIT license
	(see the LICENSEs in the bottom of the file).

WARNING
	- This library is _slightly_ experimental and features may not work as expected.
	- This also means that many functions are not documented.

CREDITS
	- Ginger Bill's 'gb.h' for inspiring me to write this, as well as stuff taken from
	library and implemented here.
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
	#ifndef SI_ARCH_64_BIT
	#define SI_ARCH_64_BIT 1
	#endif
#else
	#ifndef SI_ARCH_32_BIT
	#define SI_ARCH_32_BIT 1
	#endif
#endif


#ifndef SI_ENDIAN_ORDER
#define SI_ENDIAN_ORDER
	#define SI_IS_BIG_ENDIAN    (!*(u8*)&(u16){1})
	#define SI_IS_LITTLE_ENDIAN (!SI_IS_BIG_ENDIAN)
#endif

#if defined(_WIN32) || defined(_WIN64)
	#ifndef SI_SYSTEM_WINDOWS
	#define SI_SYSTEM_WINDOWS 1
	#endif
#elif defined(__APPLE__) && defined(__MACH__)
	#ifndef SI_SYSTEM_OSX
	#define SI_SYSTEM_OSX 1
	#endif
#elif defined(__unix__)
	#ifndef SI_SYSTEM_UNIX
	#define SI_SYSTEM_UNIX 1
	#endif

	#if defined(__linux__)
		#ifndef SI_SYSTEM_LINUX
		#define SI_SYSTEM_LINUX 1
		#endif
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#ifndef SI_SYSTEM_FREEBSD
		#define SI_SYSTEM_FREEBSD 1
		#endif
	#else
		#error This UNIX OS is not supported
	#endif
#else
	#error This OS is not supported
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

#if defined(__STDC__)
	#if defined(__STDC_VERSION__)
		#if (__STDC_VERSION__ == 199409L)
			#define SI_STANDARD_C94 1
		#elif (__STDC_VERSION__ == 199901L)
			#define SI_STANDARD_C99 1
		#elif (__STDC_VERSION__ == 201112L)
			#define SI_STANDARD_C11 1
		#elif (__STDC_VERSION__ == 201710L)
			#define SI_STANDARD_C17 1
			#define SI_STANDARD_C18 1
		#elif (__STDC_VERSION__ >= 202311L)
			#define SI_STANDARD_C23 1
		#endif
	#else
		#define SI_STANDARD_C89 1
	#endif
#endif

#ifdef __STRICT_ANSI__
	#define SI_STANDARD_ANSI 1
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
	#ifndef SI_CPU_X86
	#define SI_CPU_X86 1
	#endif
	#ifndef SI_CACHE_LINE_SIZE
	#define SI_CACHE_LINE_SIZE 64
	#endif

#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
	#ifndef SI_CPU_PPC
	#define SI_CPU_PPC 1
	#endif
	#ifndef SI_CACHE_LINE_SIZE
	#define SI_CACHE_LINE_SIZE 128
	#endif

#elif defined(__arm__)
	#ifndef SI_CPU_ARM
	#define SI_CPU_ARM 1
	#endif
	#ifndef SI_CACHE_LINE_SIZE
	#define SI_CACHE_LINE_SIZE 64
	#endif

#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
	#ifndef SI_CPU_MIPS
	#define SI_CPU_MIPS 1
	#endif
	#ifndef SI_CACHE_LINE_SIZE
	#define SI_CACHE_LINE_SIZE 64
	#endif

#else
	#error Unknown CPU Type
#endif


#ifndef SI_STATIC_ASSERT
	#define SI_STATIC_ASSERT3(cond, msg) typedef char static_assertion_##msg[(!!(cond)) * 2 - 1] /* Note(EimaMei): This is absolutely stupid but somehow it works so who cares, really? */
	#define SI_STATIC_ASSERT2(cond, line) SI_STATIC_ASSERT3(cond, static_assertion_at_line_##line)
	#define SI_STATIC_ASSERT1(cond, line) SI_STATIC_ASSERT2(cond, line)

	#define SI_STATIC_ASSERT(cond)        SI_STATIC_ASSERT1(cond, __LINE__)
#endif


#if defined(SI_SYSTEM_WINDOWS)
	#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
	#endif
#endif

#if defined(GB_SYSTEM_UNIX)
	#define _GNU_SOURCE
	#define _LARGEFILE64_SOURCE
#endif

#if !defined(SI_SYSTEM_WINDOWS)
	#include <stdlib.h>
	#include <memory.h>
	#include <stddef.h>

	#if defined(SI_STANDARD_C89)
		#define __USE_ISOC99
		#include <stdio.h>
		#undef __USE_ISOC99
	#else
		#include <stdio.h>
	#endif

	#include <stdarg.h>
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
#else

#endif

#if defined(SI_SYSTEM_OSX)

#endif

#if defined(SI_SYSTEM_UNIX)
#endif

#if defined(SI_COMPILER_MSVC)
	#if _MSC_VER < 1300
	typedef unsigned char       u8;
	typedef char                i8;
	typedef unsigned short     u16;
	typedef short              i16;
	typedef unsigned int       u32;
	typedef int                i32;
	#else
	typedef unsigned __int8     u8;
	typedef __int8              i8;
	typedef unsigned __int16   u16;
	typedef __int16            i16;
	typedef unsigned __int32   u32;
	typedef __int32            i32;
	#endif
	typedef unsigned __int64   u64;
	typedef __int64            i64;
#else
	typedef unsigned char       u8;
	typedef char                i8;
	typedef unsigned short     u16;
	typedef short              i16;
	typedef unsigned int       u32;
	typedef int                i32;
	typedef unsigned long long u64;
	typedef long long          i64;
#endif

#define SI_UINT8_MAX 0xFF
#define SI_UINT8_MIN 0x00
#define SI_INT8_MAX  0x7F
#define SI_INT8_MIN  (-SI_INT8_MAX - 1)

#define SI_UINT16_MAX 0xFFFF
#define SI_UINT16_MIN 0x0000
#define SI_INT16_MAX  0x7FFF
#define SI_INT16_MIN  (-SI_INT16_MAX - 1)

#define SI_UINT32_MAX 0xFFFFFFFF
#define SI_UINT32_MIN 0x00000000
#define SI_INT32_MAX  0x7FFFFFFF
#define SI_INT32_MIN  (-SI_INT32_MAX - 1)
#define SI_UINT64_MAX 0xFFFFFFFFFFFFFFFF
#define SI_UINT64_MIN 0x0000000000000000
#define SI_INT64_MAX  0x7FFFFFFFFFFFFFFF
#define SI_INT64_MIN  (-SI_INT64_MAX - 1)


SI_STATIC_ASSERT(sizeof(u8) == sizeof(i8));
SI_STATIC_ASSERT(sizeof(u16) == sizeof(i16));
SI_STATIC_ASSERT(sizeof(u32) == sizeof(i32));
SI_STATIC_ASSERT(sizeof(u64) == sizeof(i64));

SI_STATIC_ASSERT(sizeof(u8)  == 1);
SI_STATIC_ASSERT(sizeof(u16) == 2);
SI_STATIC_ASSERT(sizeof(u32) == 4);
SI_STATIC_ASSERT(sizeof(u64) == 8);

#define SI_BIT(x) (1 << (x))

#define SI_KILO(x) (       (x) * (usize)1024)
#define SI_MEGA(x) (SI_KILO(x) * (usize)1024)
#define SI_GIGA(x) (SI_MEGA(x) * (usize)1024)
#define SI_TERA(x) (SI_GIGA(x) * (usize)1024)

#define SI_OKAY  0
#define SI_ERROR -1

#ifndef usize
	typedef size_t    usize;
	typedef ptrdiff_t isize;
#endif

SI_STATIC_ASSERT(sizeof(usize) == sizeof(isize));

#ifndef rawptr
	typedef void* rawptr;
#endif


#if !defined(bool)
	typedef u8   b8;
	typedef u16 b16;
	typedef u32 b32;
	typedef u64 b64;

	typedef usize bool;

	#ifndef true
		#define true (0 == 0)
	#endif
	#ifndef false
		#define false (0 != 0)
	#endif
#endif

#ifndef nil
	#define nil NULL
#endif


#if defined(_WIN64)
	typedef __int64          intptr;
	typedef unsigned __int64 uintptr;
#elif defined(_WIN32)
	#ifndef _W64
		#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
			#define _W64 __w64
		#else
			#define _W64
		#endif
	#endif

	typedef _W64 int          intptr;
	typedef _W64 unsigned int uintptr;
#else
	typedef usize uintptr;
	typedef isize  intptr;
#endif

SI_STATIC_ASSERT(sizeof(uintptr) == sizeof(intptr));

typedef float  f32;
typedef double f64;

SI_STATIC_ASSERT(sizeof(f32) == 4);
SI_STATIC_ASSERT(sizeof(f64) == 8);


typedef struct siAny {
	usize type_width;
	rawptr ptr;
} siAny;

#define si_any_make(value) (siAny){.type_width = sizeof(typeof(value)), .ptr = &(typeof(value)){value} }


#if !defined(__cplusplus)
	#if defined(_MSC_VER) && _MSC_VER <= 1800
	#define inline __inline
	#elif !defined(__STDC_VERSION__)
	#define inline __inline__
	#else
	#define inline
	#endif
#endif


#define SI_ASSERT_MSG(condition, message) do { if (!(condition)) { fprintf(stderr, "Assertion \"%s\" at \"%s:%lli\"%s %s\n", #condition, __FILE__, (i64)__LINE__, (message != nil ? ":" : ""), (message != nil ? message : "")); abort(); } } while (0)
#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG(((ptr) != NULL), #ptr " must not be NULL")

#if defined(SI_STANDARD_ANSI) || defined(SI_COMPILER_MSVC) /* NOTE(EimaMei): Fuck Microsoft */
	#define typeof(value) __typeof__(value)
#endif


rawptr si_realloc(rawptr ptr, usize old_size, usize new_size) {
	if (ptr == nil) {
		return malloc(new_size);
	}

	if (old_size == new_size) {
		return ptr;
	}

	rawptr new_ptr = malloc((new_size < old_size) ? old_size : new_size);
	SI_ASSERT_NOT_NULL(new_ptr);

	memcpy(new_ptr, ptr, old_size);
	free(ptr);

	return new_ptr;
}


#ifdef SI_IMPLEMENTATION
	#define SI_PAIR_IMPLEMENTATION
	#define SI_ARRAY_IMPLEMENTATION
	#define SI_STRING_IMPLEMENTATION
	#define SI_CHAR_IMPLEMENTATION
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
#define si_pair_make(first_value, second_value) {.first = first_value, .second = second_value }
#define si_pair_copy(pair) si_pair_make(pair.first, pair.second)

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
	u32 type_width;
} siArrayHeader;

#define siArray(type) type*
#define SI_ARRAY_HEADER(s) ((siArrayHeader*)s - 1)

#define foreach(variable_name, array) __typeof__(array) variable_name; for (variable_name = (array); variable_name != (array) + si_array_len((array)); variable_name += 1)


/* siArray(void) si_array_make(rawptr array); */
#define si_array_make(...) si_array_make_list(__VA_ARGS__, sizeof(*(__VA_ARGS__)), sizeof(__VA_ARGS__) / sizeof(*(__VA_ARGS__)))
siArray(void) si_array_make_list(rawptr list, usize sizeof_element, usize count);
siArray(void) si_array_copy(siArray(void) array);
siArray(void) si_array_make_reserve(usize sizeof_element, usize count);

u32 si_array_len(siArray(void) array);
u32 si_array_capacity(siArray(void) array);
u32 si_array_type_width(siArray(void) array);
u32 si_array_size_in_bytes(siArray(void) array);

siAny si_array_at(siArray(void) array, usize index);
siAny si_array_front(siArray(void) array);
siAny si_array_back(siArray(void) array);

#define si_array_find(array, value) si_impl_array_find(array, si_any_make(value))
#define si_array_replace(array, old, new) si_impl_array_replace(array, si_any_make(old), si_any_make(new))

#define si_array_append(array, value) si_impl_array_append(array, si_any_make(value))
#define si_array_push_back(array, value) si_array_append(array, value)

isize si_array_clear(rawptr array);
bool si_arrays_are_equal(siArray(void) lha, siArray(void) rha);

isize si_array_free(siArray(void) array);

/* NOTE(EimaMei): The actual implementations. */
isize si_impl_array_append(rawptr array, siAny value);
isize si_impl_array_find(rawptr array, siAny value);
isize si_impl_array_replace(rawptr array, siAny old, siAny new);

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
#if !defined(SI_ARRAY_UNDEFINE)
	typedef siArrayHeader siStringHeader;
#else
	typedef struct { u32 len; u32 capacity; u32 type_width; } siStringHeader;
	#define foreach(variable_name, array) __typeof__(array) variable_name; for (variable_name = (array); variable_name != (array) + si_array_len((array)); variable_name += 1)
#endif

typedef char* siString;
#define SI_STRING_HEADER(s) ((siStringHeader*)s - 1)


siString si_string_make(char* str);
siString si_string_make_len(char* str, usize len);
siString si_string_make_fmt(char* str, ...);
siString si_string_make_int(isize num);

siString si_string_copy(siString from);

usize si_string_len(siString str);
usize si_cstr_len(const char* str);

char si_string_at(siString str, usize index);
char si_string_front(siString str);
char si_string_back(siString str);

isize si_string_find(siString str, const char* cstr);
isize si_string_find_start(siString str, usize starting_index, const char* cstr);

isize si_string_join(siString* str, const char* cstr, const char* separator);
isize si_string_set(siString* str, const char* cstr);
isize si_string_replace(siString* str, const char* old, const char* new);
isize si_string_trim(siString* str, const char* cut_set);

isize si_string_upper(siString* str);
isize si_string_lower(siString* str);
isize si_string_title(siString* str);
isize si_string_capitalize(siString* str);

isize si_string_append(siString* str, const char* other);
isize si_string_append_len(siString* str, const char* other, usize other_len);
isize si_string_push_back(siString* str, char other);

isize si_string_clear(siString* str);

#if !defined(SI_ARRAY_UNDEFINE)
	siArray(siString) si_string_split(siString str, const char* separator);
#else
	#if !defined(SI_PAIR_UNDEFINE)
		typedef siPair(siString, usize) siPair_String_Usize; /* NOTE(EimaMei): This is ugly. */
		/* NOTE(EimaMei): this version outputs a siPair as a return. */
		siPair_String_Usize si_string_split(siString str, const char* separator);
	#endif
#endif
isize si_cstr_to_int(const char* str);

bool si_strings_are_equal(const char* lhs, const char* rhs);

isize si_string_free(siString str);
isize si_string_make_space_for(siString* str, usize add_len);

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
	| char stuff          |
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

#endif






#if defined(SI_PAIR_IMPLEMENTATION) && !defined(SI_PAIR_UNDEFINE)
rawptr si_impl_pair_make(siAny first, siAny second) {
	rawptr res = malloc(first.type_width + second.type_width);
	memcpy(res, first.ptr, first.type_width);
	memcpy(res + first.type_width, second.ptr, second.type_width);

	return res;
}
#endif

#if defined(SI_ARRAY_IMPLEMENTATION) && !defined(SI_ARRAY_UNDEFINE)

inline siArray(void) si_array_make_list(rawptr list, usize sizeof_element, usize count)  {
	rawptr array = si_array_make_reserve(sizeof_element, count);
	memcpy(array, list, sizeof_element * count);

	return array;
}
inline siArray(void) si_array_copy(siArray(void) array) {
	siArrayHeader* header = SI_ARRAY_HEADER(array);
	return si_array_make_list(array, header->type_width, header->len);
}
siArray(void) si_array_make_reserve(usize sizeof_element, usize count) {
	rawptr ptr = malloc(sizeof(siArrayHeader) + (sizeof_element * count));
	siArray(void) array = ptr + sizeof(siArrayHeader);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	header->len = count;
	header->capacity = count;
	header->type_width = sizeof_element;

	return array;
}

inline u32 si_array_len(siArray(void) array) {
	return SI_ARRAY_HEADER(array)->len;
}
inline u32 si_array_capacity(siArray(void) array) {
	return SI_ARRAY_HEADER(array)->capacity;
}
inline u32 si_array_type_width(siArray(void) array) {
	return SI_ARRAY_HEADER(array)->type_width;
}
inline u32 si_array_size_in_bytes(siArray(void) array) {
	return si_array_len(array) * si_array_type_width(array);
}

inline siAny si_array_at(siArray(void) array, usize index) {
	SI_ASSERT_MSG(index < si_array_len(array) || si_array_len(array) != 0, "Array is empty OR the provided index is too high.");

	rawptr p = (array + index * si_array_type_width(array));
	return (siAny){.type_width = si_array_type_width(array), .ptr = p};
}
inline siAny si_array_front(siArray(void) array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	rawptr p = (array + 0 * si_array_type_width(array));
	return (siAny){.type_width = si_array_type_width(array), .ptr = p};
}
inline siAny si_array_back(siArray(void) array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	rawptr p = (array + (si_array_len(array) - 1) * si_array_type_width(array));
	return (siAny){.type_width = si_array_type_width(array), .ptr = p};
}

isize si_impl_array_find(rawptr array, siAny value) {
	SI_ASSERT_MSG((value.type_width <= si_array_type_width(array)), "The given value's sizeof is too large compared to the elements' in the array.");
	usize array_len = si_array_len(array);
	bool found = false;


	usize i;
	for (i = 0; i < array_len; i++) {
		if (memcmp(array + i * si_array_type_width(array), value.ptr, value.type_width) == 0) {
			found = true;
			break;
		}
	}

	return (found ? i : SI_ERROR);
}

isize si_impl_array_replace(rawptr array, siAny old, siAny new) {
	rawptr* array_pointer = array;
	SI_ASSERT_MSG(old.type_width <= si_array_type_width(*array_pointer) && new.type_width <= old.type_width, "The given value's sizeof is too large compared to the elements' in the array.");

	isize pos = si_impl_array_find(*array_pointer, old);
	if (pos == SI_ERROR) {
		return SI_ERROR;
	}

	memcpy(*array_pointer + pos * si_array_type_width(*array_pointer), new.ptr, new.type_width);

	return pos;
}

isize si_impl_array_append(rawptr array, siAny value) {
	rawptr* array_pointer = array;
	siArrayHeader* header = SI_ARRAY_HEADER(*array_pointer);
	SI_ASSERT_MSG((value.type_width <= header->type_width), "The given value's sizeof is too large compared to the elements' in the arra");

	if (header->len == header->capacity) {
		rawptr result = si_realloc(header, sizeof(siArrayHeader) + header->capacity * header->type_width, sizeof(siArrayHeader) + 2 * header->capacity * header->type_width);
		if (result == nil) {
			return SI_ERROR;
		}
		*array_pointer = (result + sizeof(siArrayHeader));
		header = result;

		header->capacity *= 2;
	}

	rawptr res = memcpy(*array_pointer + header->len * header->type_width, value.ptr, header->type_width);
	if (res == nil) {
		return SI_ERROR;
	}
	header->len += 1;

	return SI_OKAY;
}

isize si_array_clear(siArray(void) array) {
	if (array == nil) {
		return SI_ERROR;
	}

	siAny** array_pointer = array;

	rawptr result = memset(*array_pointer, 0, si_array_size_in_bytes(*array_pointer));
	if (result == nil) {
		return SI_ERROR;
	}

	return SI_OKAY;
}

bool si_arrays_are_equal(siArray(void) lha, siArray(void) rha) {
	siArrayHeader* lha_header = SI_ARRAY_HEADER(lha);
	siArrayHeader* rha_header = SI_ARRAY_HEADER(rha);

	if (lha_header->len != rha_header->len || lha_header->type_width != rha_header->type_width) {
		return false;
	}

	usize i;
	for (i = 0; i < lha_header->len; i++) {
		if (memcmp(lha + i * lha_header->type_width, rha + i * lha_header->type_width, lha_header->type_width) != SI_OKAY) {
			return false;
		}
	}

	return true;
}

isize si_array_free(siArray(void) array) {
	if (array == nil) {
		return SI_ERROR;
	}

	free(SI_ARRAY_HEADER(array));

	return SI_OKAY;
}

#endif

#if defined(SI_STRING_IMPLEMENTATION) && !defined(SI_STRING_UNDEFINE)


inline siString si_string_make(char* str) {
    return si_string_make_len(str, str ? si_cstr_len(str) : 0);
}
siString si_string_make_len(char* str, usize len) {
	rawptr ptr = malloc(sizeof(siStringHeader) + (len + 1));
	SI_ASSERT_NOT_NULL(ptr);

	siString res_str = ptr + sizeof(siStringHeader);
	memcpy(res_str, str, len);
	res_str[len] = '\0';

	siStringHeader* header = SI_STRING_HEADER(res_str);
	header->len = len;
	header->capacity = len;
	header->type_width = sizeof(char);

	return res_str;
}

siString si_string_make_fmt(char* str, ...) {
	va_list va;
	va_start(va, str);

	isize size = vsnprintf(NULL, 0, str,  va);
	char buf[size + 1];
	vsnprintf(buf, size + 1, str, va);

	va_end(va);

	if (size < SI_OKAY) {
		return nil;
	}

	return si_string_make_len(buf, size);
}
siString si_string_make_int(isize num) {
	usize count = 1;
	isize og = num;
	char buffer[32];

	if (num < 0) {
		count++;
		num = -num;
	}

	while (true) {
		isize number = num % 10;

		if (number == 0 && count != 0) {
			break;
		}

		buffer[count - 1] = '0' + number;
		count++;
		num *= 0.1f;
	}

	if (og < 0) {
		buffer[count - 1] = '-';
		count++;
	}
	buffer[count - 1] = '\0';

	char shuffle[count];
	usize i;
	for (i = 0; i < count; i++) {
		shuffle[i] = buffer[count - 2 - i];
	}

	return si_string_make(shuffle);
}

inline usize si_string_len(siString str) {
	return SI_STRING_HEADER(str)->len;
}
inline usize si_cstr_len(const char* str) {
	register const char* s;
	for (s = str; *s; s++);

	return (s - str);
}

inline char si_string_at(siString str, usize index) {
	SI_ASSERT_NOT_NULL(str);
	if (index > si_string_len(str) || index < 0 || si_string_len(str) == 0) {
		return '\0';
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
		return SI_OKAY;
	}

	return str[si_string_len(str) - 1];
}

isize si_string_find(siString str, const char* cstr) {
	return si_string_find_start(str, 0, cstr);
}

isize si_string_find_start(siString str, usize starting_index, const char* cstr) {
	usize cstr_len = si_cstr_len(cstr);
	bool found = false;

	usize i;
	usize cur_count = 0;
	for (i = starting_index; i < si_string_len(str); i++) {
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

	return (found ? (i - cstr_len + 1) : SI_ERROR);
}
isize si_string_join(siString* str, const char* cstr, const char* separator) {
	usize cstr_len = si_cstr_len(cstr);
	usize separator_len = si_cstr_len(separator);

	char tmp[separator_len + cstr_len + 1];
	memcpy(tmp, separator, separator_len);
	memcpy(tmp + separator_len, cstr, cstr_len);
	tmp[separator_len + cstr_len] = '\0';

	return si_string_append(str, tmp);
}
isize si_string_set(siString* str, const char* cstr) {
	usize cur_len = si_string_len(*str);
	usize len = si_cstr_len(cstr);

	siStringHeader* header = SI_STRING_HEADER(*str);
	header->len += len - cur_len;

	if (cur_len < len) {
		isize result = si_string_make_space_for(str, len - cur_len);
		SI_ASSERT_MSG(result == SI_OKAY, "Failed to make space for string");
	}

	rawptr ptr = memcpy(*str, cstr, len);
	SI_ASSERT_NOT_NULL(ptr);
	(*str)[len] = '\0';

	return SI_OKAY;
}
isize si_string_replace(siString* str, const char* old, const char* new) {
	usize str_len = si_string_len(*str);
	usize new_len = si_cstr_len(new);
	usize old_len = si_cstr_len(old);

	isize pos = si_string_find(*str, old);

	siStringHeader* header = SI_STRING_HEADER(*str);
	header->len += new_len - old_len;

	if (pos == SI_ERROR) {
		return SI_ERROR;
	}

	if (old_len < new_len) {
		isize result = si_string_make_space_for(str, new_len - old_len);
		SI_ASSERT_MSG(result == SI_OKAY, "Failed to make space for string");

		usize i, j;
		char new_value = '\0';
		for (j = 0; j < new_len - old_len; j++) { /* NOTE(EimaMei): There is probably a much faster way to do this other than looping through the string multiple times. However, as a temporary solution, it works nicely. */
			for (i = pos; i < str_len + new_len - old_len; i++) {
				char og = (*str)[j + i];
				(*str)[j + i] = new_value;
				new_value = og;
			}
		}
	}

	if (old_len > new_len) {
		usize i, j;
		char new_value = '\0';
		for (j = 0; j < old_len - new_len; j++) {  /* NOTE(EimaMei): Read the previous comment above. */
			for (i = str_len; i != (usize)SI_ERROR; i--) {
				char og = (*str)[j + i];
				(*str)[j + i] = new_value;
				new_value = og;
			}
		}
	}

	usize i;
	for (i = 0; i < new_len; i++) {
		(*str)[i + pos] = new[i];
	}

	return pos;
}
isize si_string_trim(siString* str, const char* cut_set) {
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

	return SI_OKAY;
}
isize si_string_upper(siString* str) {
	foreach (x, *str) {
		*x = si_char_to_upper(*x);
	}
	return SI_OKAY;
}

isize si_string_lower(siString* str) {
	foreach (x, *str) {
		*x = si_char_to_lower(*x);
	}
	return SI_OKAY;
}
isize si_string_title(siString* str) {
	si_string_capitalize(str);

	bool change = false;
	foreach (x, *str) {
		if (si_char_is_space(*x)) {
			change = true;
		}
		else if (change) {
			*x = si_char_to_upper(*x);
			change = false;
		}
	}

	return SI_OKAY;
}
isize si_string_capitalize(siString* str) {
	if (si_string_len(*str) <= 0) {
		return SI_ERROR;
	}
	si_string_lower(str);

	**str = si_char_to_upper(**str);

	return SI_OKAY;
}

#if !defined(SI_ARRAY_UNDEFINE)
siArray(siString) si_string_split(siString str, const char* separator) {
	usize separator_len = si_cstr_len(separator);

	usize count = 0;
	usize begin_pos = 0;
	usize pos_buffer[si_string_len(str)];
	siString original = str;

	while (true) {
		isize pos = si_string_find_start(str, begin_pos, separator);

		if (pos == SI_ERROR) {
			pos_buffer[count] = si_string_len(str);
			count++;
			break;
		}
		pos_buffer[count] = pos - begin_pos;
		count++;

		begin_pos = pos + separator_len;
	}
	siArray(siString) res = si_array_make_reserve(sizeof(*res), count);

	usize i;
	for (i = 0; i < count; i++) {
		res[i] = si_string_make_len(original, pos_buffer[i]);
		original += pos_buffer[i] + separator_len;
	}

	return res;
}
#else
	#if !defined(SI_PAIR_UNDEFINE)
		siPair_String_Usize si_string_split(siString str, const char* separator) {
			usize separator_len = si_cstr_len(separator);

			usize count = 0;
			usize begin_pos = 0;
			usize pos_buffer[si_string_len(str)];
			siString original = str;

			while (true) {
				isize pos = si_string_find_start(str, begin_pos, separator);

				if (pos == SI_ERROR) {
					pos_buffer[count] = si_string_len(str);
					count++;
					break;
				}
				pos_buffer[count] = pos - begin_pos;
				count++;

				begin_pos = pos + separator_len;
			}

			siPair_String_Usize res;
			res.second = count;
			res.first = malloc(sizeof(siString) * count);

			usize i;
			for (i = 0; i < count; i++) {
				res.first[i] = si_string_make_len(original, pos_buffer[i]);
				original += pos_buffer[i] + separator_len;
			}

			return res;
		}
	#endif
#endif

inline isize si_string_append(siString* str, const char* other) {
	return si_string_append_len(str, other, si_cstr_len(other));
}
isize si_string_append_len(siString* str, const char* other, usize other_len) {
	usize cur_len = si_string_len(*str);

	siStringHeader* header = SI_STRING_HEADER(*str);
	header->len += other_len;

	isize result = si_string_make_space_for(str, other_len);
	SI_ASSERT_MSG(result == SI_OKAY, "Failed to make space for string");

	rawptr ptr = memcpy(*str + cur_len, other, other_len);
	SI_ASSERT_NOT_NULL(ptr);
	(*str)[cur_len + other_len] = '\0';

	return SI_OKAY;
}
inline isize si_string_push_back(siString* str, char other) {
	return si_string_append_len(str, &other, 1);
}

inline siString si_string_copy(siString from) {
	return si_string_make(from);
}
isize si_cstr_to_int(const char* str) {
	SI_ASSERT_NOT_NULL(str);
    isize result = 0;
    isize i = 0;
	bool is_negative = false;

	if (str[0] == '-') {
		i += 1;
		is_negative = true;
	}

    for (i = i; i < si_cstr_len(str); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result *= 10;

            if (str[i] != '0')
                result += (isize)(str[i] - '0');
        }
		else {
			SI_ASSERT_MSG(!(str[i] >= '0' && str[i] <= '9'), "Found a non-number");
		}
    }

	if (is_negative) {
		result = -result;
	}

    return result;
}
inline isize si_string_clear(siString* str) {
	char null_terminator = '\0';
	isize result = si_string_set(str, &null_terminator);

	return result;
}

bool si_strings_are_equal(const char* lhs, const char* rhs) {
	if (lhs == rhs) {
		return true;
	}

	usize lhs_len = si_cstr_len(lhs);
	usize rhs_len = si_cstr_len(rhs);

	if (lhs_len != rhs_len) {
		return false;
	}

	usize i;
	for (i = 0; i < lhs_len; i++) {
		if (lhs[i] != rhs[i])
			return false;
	}

	return true;
}

inline isize si_string_free(siString str) {
	if (str == nil) {
		return SI_ERROR;
	}
	free(SI_STRING_HEADER(str));

	return SI_OKAY;
}
isize si_string_make_space_for(siString* str, usize add_len) {
	usize len = si_string_len(*str);
	usize new_len = len + add_len;


	rawptr ptr = *str - sizeof(siStringHeader);
	usize old_size = sizeof(siStringHeader) + len + 1;
	usize new_size = sizeof(siStringHeader) + new_len + 1;

	rawptr new_ptr = si_realloc(ptr, old_size, new_size);
	SI_ASSERT_NOT_NULL(new_ptr);

	*str = new_ptr + sizeof(siStringHeader);

	return SI_OKAY;
}
#endif

#if defined(SI_CHAR_IMPLEMENTATION) && !defined(SI_STRING_UNDEFINE)
inline char si_char_to_lower(char c) {
	if (c >= 'A' && c <= 'Z') {
		return 'a' + (c - 'A');
	}
	return c;
}
inline char si_char_to_upper(char c) {
	if (c >= 'a' && c <= 'z') {
		return 'A' + (c - 'a');
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
	return (si_char_is_digit(c) ? c - '0' : c - 'W');
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