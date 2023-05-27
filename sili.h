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
		- #define SI_OPTIONAL_UNDEFINE
		- #define SI_ARRAY_UNDEFINE
		- #define SI_STRING_UNDEFINE
		- #define SI_CHAR_UNDEFINE
		- #define SI_FILE_UNDEFINE
	before the SI_IMPLEMENTATION macro, as well as before any other include of `sili.h`.
	- NOTE: Disabling certain features may or may not create 'undefined' compiler
	errors at certain spots. Use it at your own risk

===========================================================================

LICENSE
	- This software is dual-licensed to the public domain and under the MIT license
	(see the LICENSEs in the bottom of the file).

WARNING
	- This library is _slightly_ experimental and features may not work as expected.
	- This also means that many functions are not documented.

CREDITS
	- Ginger Bill's 'gb.h' for inspiring me to program the sili STL, as well as stuff taken from
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
	#define SI_ARCH_64_BIT 1
#else
	#define SI_ARCH_32_BIT 1
#endif

#define SI_ENDIAN_BIG     (!*(siByte*)&(u16){1})
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

#define SI_STATIC_ASSERT3(cond, msg) typedef char static_assertion_##msg[(!!(cond)) * 2 - 1] /* Note(EimaMei): This is absolutely stupid but somehow it works so who cares, really? */
#define SI_STATIC_ASSERT2(cond, line) SI_STATIC_ASSERT3(cond, static_assertion_at_line_##line)
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

	#include <sys/stat.h>
	#include <sys/sendfile.h>
	#include <sys/fcntl.h>

	#include <stdio.h>

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

typedef i8 siByte;

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

SI_STATIC_ASSERT(sizeof(usize) == sizeof(isize));

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

typedef float  f32;
typedef double f64;

SI_STATIC_ASSERT(sizeof(f32) == 4);
SI_STATIC_ASSERT(sizeof(f64) == 8);

#if !defined(rawptr)
	typedef void* rawptr;
#endif


#define SI_BIT(x) (1 << (x))

#define SI_KILO(x) (       (x) * (usize)1024)
#define SI_MEGA(x) (SI_KILO(x) * (usize)1024)
#define SI_GIGA(x) (SI_MEGA(x) * (usize)1024)
#define SI_TERA(x) (SI_GIGA(x) * (usize)1024)

#define SI_OKAY  0
#define SI_ERROR -1

#define SI_UNUSED(x) (void)(x) /* NOTE(EimaMei) Should be used when you want to supress "parameter 'X' set but not used" errors. */

#if defined(SI_STANDARD_ANSI) || defined(SI_COMPILER_MSVC) /* NOTE(EimaMei): Fuck Microsoft */
	#define typeof(value) __typeof__(value)
#endif


usize si_impl_assert_msg(bool condition, const char* condition_str, const char* message, const char* file, i32 line);
#define SI_ASSERT_MSG(condition, message) si_impl_assert_msg(condition, #condition, message, __FILE__, __LINE__)
#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG((ptr) != NULL, #ptr " must not be NULL")

typedef struct siAny {
	usize type_width;
	rawptr ptr;
} siAny;

#define si_any_make(value) (siAny){sizeof(typeof(value)), &(typeof(value)){value} }
#define si_any_get(any, type) ((any.ptr != NULL) ? *((type*)any.ptr) : (type)SI_ASSERT_NOT_NULL(any.ptr))


#ifdef SI_IMPLEMENTATION
	#define SI_GENERAL_IMPLEMENTATION
	#define SI_PAIR_IMPLEMENTATION
	#define SI_OPTIONAL_IMPLEMENTATION
	#define SI_ARRAY_IMPLEMENTATION
	#define SI_STRING_IMPLEMENTATION
	#define SI_CHAR_IMPLEMENTATION
	#define SI_FILE_IMPLEMENTATION
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
const siOptionalStruct SI_OPTIONAL_NULL = {{0, nil}, false};

#define si_optional_make(value) (siOptionalStruct){si_any_make(value), true}
#define si_optional_if_no_value_type(siOptional_var, or_value, type) (siOptional_var.has_value ? si_any_get(siOptional_var.value, type) : (type)or_value)
#define si_optional_if_no_value(siOptional_var, or_value) si_optional_if_no_value_type(siOptional_var, or_value, typeof(or_value))
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
	u32 type_width;
} siArrayHeader;

#define siArray(type) type*
#define SI_ARRAY_HEADER(s) ((siArrayHeader*)s - 1)

#if defined(SI_STANDARD_C89)
	#define foreach(variable_name, array) typeof(array) variable_name; for (variable_name = (array); variable_name != (array) + si_array_len((array)); variable_name += 1)
#else
	#define foreach(variable_name, array) for (typeof(array) variable_name = (array); variable_name != (array) + si_array_len((array)); variable_name += 1)
#endif

/* siArray(void) si_array_make(rawptr array); */
#define si_array_make(...) si_array_make_list(__VA_ARGS__, sizeof(*(__VA_ARGS__)), sizeof(__VA_ARGS__) / sizeof(*(__VA_ARGS__)))
siArray(void) si_array_make_list(rawptr list, usize sizeof_element, usize count);
siArray(void) si_array_copy(siArray(void) array);
siArray(void) si_array_make_reserve(usize sizeof_element, usize count);

usize si_array_len(siArray(void) array);
usize si_array_capacity(siArray(void) array);
usize si_array_type_width(siArray(void) array);
usize si_array_size_in_bytes(siArray(void) array);

siAny si_array_at(siArray(void) array, usize index);
siAny si_array_front(siArray(void) array);
siAny si_array_back(siArray(void) array);

#define si_array_find(array, value) si_impl_array_find_ex(array, 0, si_array_len(array), si_any_make(value))
#define si_array_find_ex(array, start, end, value) si_impl_array_find_ex(array, start, end, si_any_make(value))
#define si_array_rfind(array, value) si_impl_array_rfind_ex(array, si_array_len(array) - 1, 0, si_any_make(value))
#define si_array_rfind_ex(array, start, end, value) si_impl_array_rfind_ex(array, start, end, si_any_make(value))

#define si_array_replace(array, old_value, new_value) si_impl_array_replace(array, si_any_make(old_value), si_any_make(new_value))
isize si_array_reverse(siArray(void) array);
char* si_array_to_sistring(siArray(char*) array, const char* separator);

#define si_array_append(array, value) si_impl_array_append(array, si_any_make(value))
#define si_array_push_back(array, value) si_array_append(array, value)

isize si_array_clear(rawptr array);
bool si_arrays_are_equal(siArray(void) lha, siArray(void) rha);

isize si_array_free(siArray(void) array);

/* NOTE(EimaMei): The actual implementations. Shouldn't be used in practice, really. */
isize si_impl_array_append(rawptr array, siAny value);
isize si_impl_array_find_ex(rawptr array, usize start, usize end, siAny value);
isize si_impl_array_rfind_ex(rawptr str, usize start, usize end, siAny value);
isize si_impl_array_replace(rawptr array, siAny old_value, siAny new_value);

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
	#if defined(SI_STANDARD_C89)
		#define foreach(variable_name, array) typeof(array) variable_name; for (variable_name = (array); variable_name != (array) + si_array_len((array)); variable_name += 1)
	#else
		#define foreach(variable_name, array) for (typeof(array) variable_name = (array); variable_name != (array) + si_array_len((array)); variable_name += 1)
	#endif
#endif

typedef char* siString;
#define SI_STRING_HEADER(s) ((siStringHeader*)s - 1)


siString si_string_make(char* str);
siString si_string_make_fmt(char* str, ...);
siString si_string_make_int(isize num);
siString si_string_make_len(char* str, usize len);
siString si_string_make_reserve(usize len);

siString si_string_copy(siString from);

usize si_string_len(siString str);
usize si_cstr_len(const char* str);

char si_string_at(siString str, usize index);
char si_string_front(siString str);
char si_string_back(siString str);

isize si_string_find(siString str, const char* cstr);
isize si_string_find_ex(siString str, usize start, usize end, const char* cstr);
isize si_string_rfind(siString str, const char* cstr);
isize si_string_rfind_ex(siString str, usize start, usize end, const char* cstr);

isize si_string_join(siString* str, const char* cstr, const char* separator);
isize si_string_set(siString* str, const char* cstr);
isize si_string_replace(siString* str, const char* old_str, const char* new_str);
isize si_string_trim(siString* str, const char* cut_set);

isize si_string_upper(siString* str);
isize si_string_lower(siString* str);
isize si_string_title(siString* str);
isize si_string_capitalize(siString* str);
isize si_string_strip(siString* str); /* NOTE(EimaMei); This strips any leading or trailing spaces in the string. */
isize si_string_reverse(siString* str);

isize si_string_append(siString* str, const char* other);
isize si_string_append_len(siString* str, const char* other, usize other_len);
isize si_string_push_back(siString* str, char other);

isize si_string_clear(siString* str);

siArray(siString) si_string_split(siString str, const char* separator);
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
typedef enum siFileMode {
	SI_FILE_MODE_READ = SI_BIT(0),
	SI_FILE_MODE_WRITE = SI_BIT(1),
	SI_FILE_MODE_READ_WRITE = SI_BIT(2),
	SI_FILE_MODE_APPEND = SI_BIT(3),
	SI_FILE_MODE_APPEND_READ = SI_BIT(4),
	SI_FILE_MODE_CREATE = SI_BIT(5)
} siFileMode;

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
bool si_path_exists(const char* path);
isize si_path_copy(const char* existing_path, const char* new_path);
isize si_path_move(const char* existing_path, const char* new_path);
isize si_path_remove(const char* path);

const char* si_path_base_name(const char* path);
const char* si_path_extension(const char* path);
siString si_path_get_fullname(const char* path);

bool si_path_is_absolute(const char* path);
bool si_path_is_relative(const char* path);

/*
	========================
	|  siFile              |
	========================
*/
siFile si_file_create(const char* path);
siFile si_file_open(const char* path);
siFile si_file_open_mode(const char* path, siFileMode mode);

usize si_file_size(siFile file);
void si_file_size_update(siFile* file);

siString si_file_read(siFile file);
siString si_file_read_at(siFile file, usize offset);
siArray(siString) si_file_readlines(siFile file);

isize si_file_write(siFile* file, const char* content);
isize si_file_write_len(siFile* file, const char* content, usize len);
isize si_file_write_at(siFile* file, const char* content, usize offset);
isize si_file_write_at_line(siFile* file, const char* content, usize index);
isize si_file_write_at_len(siFile* file, const char* content, usize content_len, usize offset);

isize si_file_seek(siFile file, usize offset);
isize si_file_seek_to_end(siFile file);

isize si_file_close(siFile file);

#endif




#if defined(SI_GENERAL_IMPLEMENTATION)

usize si_impl_assert_msg(bool condition, const char* condition_str, const char* message, const char* file, i32 line) {
	if (!condition) {
		fprintf(stderr, "Assertion \"%s\" at \"%s:%d\"%s %s\n", condition_str, file, line, (message != nil ? ":" : ""), (message != nil ? message : ""));
		abort();
	}

	return 0;
}

rawptr si_realloc(rawptr ptr, usize old_size, usize new_size) {
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
#endif

#if defined(SI_PAIR_IMPLEMENTATION) && !defined(SI_PAIR_UNDEFINE)

rawptr si_impl_pair_make(siAny first, siAny second) {
	siByte* res = malloc(first.type_width + second.type_width);
	memcpy(res, first.ptr, first.type_width);
	memcpy(res + first.type_width, second.ptr, second.type_width);

	return (rawptr)res;
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
	siByte* ptr = malloc(sizeof(siArrayHeader) + (sizeof_element * count));
	siArray(void) array = ptr + sizeof(siArrayHeader);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	header->len = count;
	header->capacity = count;
	header->type_width = sizeof_element;

	return array;
}

inline usize si_array_len(siArray(void) array) {
	return SI_ARRAY_HEADER(array)->len;
}
inline usize si_array_capacity(siArray(void) array) {
	return SI_ARRAY_HEADER(array)->capacity;
}
inline usize si_array_type_width(siArray(void) array) {
	return SI_ARRAY_HEADER(array)->type_width;
}
inline usize si_array_size_in_bytes(siArray(void) array) {
	return si_array_len(array) * si_array_type_width(array);
}

inline siAny si_array_at(siArray(void) array, usize index) {
	SI_ASSERT_MSG(index < si_array_len(array) || si_array_len(array) != 0, "Array is empty OR the provided index is too high.");

	rawptr p = (siByte*)array + index * si_array_type_width(array);
	return (siAny){.type_width = si_array_type_width(array), .ptr = p};
}
inline siAny si_array_front(siArray(void) array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	rawptr p = (siByte*)array + 0 * si_array_type_width(array);
	return (siAny){.type_width = si_array_type_width(array), .ptr = p};
}
inline siAny si_array_back(siArray(void) array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	rawptr p = (siByte*)array + (si_array_len(array) - 1) * si_array_type_width(array);
	return (siAny){.type_width = si_array_type_width(array), .ptr = p};
}

isize si_impl_array_find_ex(rawptr array, usize start, usize end, siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_MSG(value.type_width <= si_array_type_width(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start < end, "Value 'start' is larger than 'end'");

	bool found = false;
	usize i;
	for (i = start; i < end; i++) {
		if (memcmp((siByte*)array + i * si_array_type_width(array), value.ptr, value.type_width) == 0) {
			found = true;
			break;
		}
	}

	return (found ? i : SI_ERROR);
}
isize si_impl_array_rfind_ex(rawptr array, usize start, usize end, siAny value) {
	SI_ASSERT_NOT_NULL(array);
	SI_ASSERT_MSG(value.type_width <= si_array_type_width(array), "The given value's sizeof is too large compared to the elements' in the array.");
	SI_ASSERT_MSG(start > end, "Value 'end' is larger than 'start'");

	bool found = false;
	usize i;
	for (i = start; i < end - 1; i--) {
		if (memcmp((siByte*)array + i * si_array_type_width(array), value.ptr, value.type_width) == 0) {
			found = true;
			break;
		}
	}

	return (found ? i : SI_ERROR);
}

isize si_impl_array_replace(rawptr array, siAny old_value, siAny new_value) {
	rawptr* array_pointer = (rawptr*)array;
	SI_ASSERT_MSG(old_value.type_width <= si_array_type_width(*array_pointer) && new_value.type_width <= old_value.type_width, "The given value's sizeof is too large compared to the elements' in the array.");

	isize pos = si_impl_array_find_ex(*array_pointer, 0, si_array_len(*array_pointer), old_value);
	if (pos == SI_ERROR) {
		return SI_ERROR;
	}

	memcpy((siByte*)(*array_pointer) + pos * si_array_type_width(*array_pointer), new_value.ptr, new_value.type_width);

	return pos;
}
isize si_array_reverse(siArray(void) array) {
	SI_ASSERT_NOT_NULL(array);
	siByte* array_pointer = *((siByte**)array);

	usize len = si_array_len(array_pointer);
	usize type_width = si_array_type_width(array_pointer);
	SI_ASSERT_MSG(len != 0, "Array is empty");

	usize i, j;
	rawptr tmp[1];
	for (i = 0, j = len - 1; i < j; i++, j--) {
		rawptr i_var = array_pointer + i * type_width;
		rawptr j_var = array_pointer + j * type_width;

		memcpy(tmp, i_var, type_width);
		memcpy(i_var, j_var, type_width);
		memcpy(j_var, tmp, type_width);
	}

	return SI_OKAY;
}
siString si_array_to_sistring(siArray(char*) array, const char* separator) {
	SI_ASSERT_NOT_NULL(array);

	usize separator_len = si_cstr_len(separator);
	usize total_size = 0;


	foreach (str, array) {
		total_size += si_cstr_len(*str) + separator_len;
	}

	siString result = si_string_make_reserve(total_size);
	foreach (str, array) {
		if (separator != nil && str != si_array_back(array).ptr) {
			si_string_join(&result, separator, *str);
		}
		else {
			si_string_append(&result, *str);
		}
	}

	return result;
}

isize si_impl_array_append(rawptr array, siAny value) {
	rawptr* array_pointer = (rawptr*)array;
	siArrayHeader* header = SI_ARRAY_HEADER(*array_pointer);
	SI_ASSERT_MSG((value.type_width <= header->type_width), "The given value's sizeof is too large compared to the elements' in the arra");

	if (header->len == header->capacity) {
		siByte* result = si_realloc(header, sizeof(siArrayHeader) + header->capacity * header->type_width, sizeof(siArrayHeader) + 2 * header->capacity * header->type_width);
		if (result == nil) {
			return SI_ERROR;
		}
		*array_pointer = (siByte*)result + sizeof(siArrayHeader);
		header = (siArrayHeader*)result;

		header->capacity *= 2;
	}

	rawptr res = memcpy((siByte*)(*array_pointer) + header->len * header->type_width, value.ptr, header->type_width);
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

	rawptr* array_pointer = (rawptr*)array;

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
		if (memcmp((siByte*)lha + i * lha_header->type_width, (siByte*)rha + i * lha_header->type_width, lha_header->type_width) != SI_OKAY) {
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
	siString res_str = si_string_make_reserve(len);
	memcpy(res_str, str, len);
	res_str[len] = '\0';

	siStringHeader* header = SI_STRING_HEADER(res_str);
	header->len = len;

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
siString si_string_make_reserve(usize len) {
	rawptr ptr = malloc(sizeof(siStringHeader) + (len + 1));
	SI_ASSERT_NOT_NULL(ptr);

	siString res_str = (siString)ptr + sizeof(siStringHeader);
	res_str[len] = '\0';

	siStringHeader* header = SI_STRING_HEADER(res_str);
	header->len = 0;
	header->capacity = len;
	header->type_width = sizeof(char);

	return res_str;
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

inline isize si_string_find(siString str, const char* cstr) {
	return si_string_find_ex(str, 0, si_string_len(str), cstr);
}
isize si_string_find_ex(siString str, usize start, usize end, const char* cstr) {
    SI_ASSERT_NOT_NULL(str);

	usize cstr_len = si_cstr_len(cstr);
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

	return (found ? (i - cstr_len + 1) : SI_ERROR);
}
inline isize si_string_rfind(siString str, const char* cstr) {
	return si_string_rfind_ex(str, si_string_len(str) - 1, 0, cstr);
}
isize si_string_rfind_ex(siString str, usize start, usize end, const char* cstr) {
    SI_ASSERT_NOT_NULL(str);

	usize cstr_len = si_cstr_len(cstr);
	bool found = false;

	usize i;
	isize cur_count = cstr_len - 1;
	for (i = start; i < end - 1; i--) {
		if (str[i] == cstr[cur_count]) {
			cur_count -= 1;

			if (cur_count <= 0) {
				found = true;
				break;
			}
		}
		else if (cur_count != 0) {
			cur_count = 0;
		}
	}

	return (found ? i : SI_ERROR);
}

isize si_string_join(siString* str, const char* cstr, const char* separator) {
	usize cstr_len = si_cstr_len(cstr);
	usize separator_len = si_cstr_len(separator);

	char tmp[separator_len + cstr_len + 1];
	memcpy(tmp, separator, separator_len);
	memcpy(tmp + separator_len, cstr, cstr_len);
	tmp[separator_len + cstr_len] = '\0';

	return si_string_append_len(str, tmp, separator_len + cstr_len);
}
isize si_string_set(siString* str, const char* cstr) {
	usize cur_len = si_string_len(*str);
	usize len = si_cstr_len(cstr);

	siStringHeader* header = SI_STRING_HEADER(*str);
	header->len += len - cur_len;

	if (header->capacity < header->len) {
		isize result = si_string_make_space_for(str, len - cur_len);
		SI_ASSERT_MSG(result == SI_OKAY, "Failed to make space for string");
	}

	rawptr ptr = memcpy(*str, cstr, len);
	SI_ASSERT_NOT_NULL(ptr);
	(*str)[len] = '\0';

	return SI_OKAY;
}
isize si_string_replace(siString* str, const char* old_value, const char* new_value) {
	usize str_len = si_string_len(*str);
	usize new_len = si_cstr_len(new_value);
	usize old_len = si_cstr_len(old_value);

	isize pos = si_string_find(*str, old_value);

	siStringHeader* header = SI_STRING_HEADER(*str);
	header->len += new_len - old_len;

	if (pos == SI_ERROR) {
		return SI_ERROR;
	}

	if (old_len < new_len) {
		isize result = si_string_make_space_for(str, new_len - old_len);
		SI_ASSERT_MSG(result == SI_OKAY, "Failed to make space for string");

		usize i, j;
		char saved_value = '\0';
		for (j = 0; j < new_len - old_len; j++) { /* NOTE(EimaMei): There is probably a much faster way to do this other than looping through the string multiple times. However, as a temporary solution, it works nicely. */
			for (i = pos; i < str_len + new_len - old_len; i++) {
				char og = (*str)[j + i];
				(*str)[j + i] = saved_value;
				saved_value = og;
			}
		}
	}

	if (old_len > new_len) {
		usize i, j;
		char saved_value = '\0';
		for (j = 0; j < old_len - new_len; j++) {  /* NOTE(EimaMei): Read the previous comment above. */
			for (i = str_len; i != (usize)SI_ERROR; i--) {
				char og = (*str)[j + i];
				(*str)[j + i] = saved_value;
				saved_value = og;
			}
		}
	}

	usize i;
	for (i = 0; i < new_len; i++) {
		(*str)[i + pos] = new_value[i];
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
inline isize si_string_upper(siString* str) {
	SI_ASSERT_NOT_NULL(str);

	foreach (x, *str) {
		*x = si_char_to_upper(*x);
	}
	return SI_OKAY;
}

inline isize si_string_lower(siString* str) {
	SI_ASSERT_NOT_NULL(str);

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
inline isize si_string_capitalize(siString* str) {
	SI_ASSERT_NOT_NULL(str);

	if (si_string_len(*str) <= 0) {
		return SI_ERROR;
	}
	si_string_lower(str);
	**str = si_char_to_upper(**str);

	return SI_OKAY;
}
isize si_string_strip(siString* str) {
	SI_ASSERT_NOT_NULL(str);

	usize start = 0, end = 0;
	foreach (x, *str) {
		if (!si_char_is_space(*x)) {
			break;
		}
		start += 1;
	}

	usize i;
	for (i = si_string_len(*str) - 1; i < (usize)(-1); i--) {
		if (!si_char_is_space((*str)[i])) {
			break;
		}
		end += 1;
	}
	char* stripped_str = *str + start * SI_STRING_HEADER(*str)->type_width;
	stripped_str[si_string_len(*str) - start - end] = '\0';

	return si_string_set(str, stripped_str);
}
isize si_string_reverse(siString* str) {
	SI_ASSERT_NOT_NULL(str);

	usize len = si_string_len(*str);
	SI_ASSERT_MSG(len != 0, "String is empty");

	usize i, j;
	char tmp;
	for (i = 0, j = len - 1; i < j; i++, j--) {
		tmp = (*str)[i];
		(*str)[i] = (*str)[j];
		(*str)[j] = tmp;
	}

	return SI_OKAY;
}

siArray(siString) si_string_split(siString str, const char* separator) {
	usize separator_len = si_cstr_len(separator);

	usize count = 0;
	usize begin_pos = 0;
	usize pos_buffer[si_string_len(str)];
	siString original = str;

	while (true) {
		isize pos = si_string_find_ex(str, begin_pos, si_string_len(str), separator);

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

inline isize si_string_append(siString* str, const char* other) {
	return si_string_append_len(str, other, si_cstr_len(other));
}
isize si_string_append_len(siString* str, const char* other, usize other_len) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize previous_len = header->len;
	header->len += other_len;

	if (header->capacity < header->len) {
		isize result = si_string_make_space_for(str, other_len);
		SI_ASSERT_MSG(result == SI_OKAY, "Failed to make space for string");
		header = SI_STRING_HEADER(*str); /* NOTE(EimaMei): For some reason we have to refresh the header pointer on Linux. Somewhat makes sense but also what and why. */
	}

	rawptr ptr = memcpy(*str + previous_len, other, other_len);
	SI_ASSERT_NOT_NULL(ptr);
	(*str)[header->len] = '\0';

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
    usize i = 0;
	bool is_negative = false;

	if (str[0] == '-') {
		i += 1;
		is_negative = true;
	}

    for ( ; i < si_cstr_len(str); i++) {
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

	*str = (char*)new_ptr + sizeof(siStringHeader);

	SI_STRING_HEADER(*str)->capacity += add_len;

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

#if defined(SI_FILE_IMPLEMENTATION) && !defined(SI_FILE_UNDEFINE)

inline bool si_path_exists(const char* path) {
	#if defined(SI_SYSTEM_WINDOWS)

	DWORD file_attrib = GetFileAttributes(path);
	return file_attrib != INVALID_FILE_ATTRIBUTES;

	#else

	struct stat tmp;
 	return (stat(path, &tmp) == SI_OKAY);

	#endif
}
isize si_path_copy(const char* existing_path, const char* new_path) {
	#if defined(SI_SYSTEM_WINDOWS)

	return (CopyFile(existing_path, new_path, true) != 0);

	#else

	int existing_fd = open(existing_path, O_RDONLY, 0);
	int new_fd      = open(new_path, O_WRONLY | O_CREAT, 0666);

	struct stat stat_existing;
	fstat(existing_fd, &stat_existing);

	isize size = sendfile(new_fd, existing_fd, 0, stat_existing.st_size);

	close(new_fd);
	close(existing_fd);

	return size == stat_existing.st_size;

	#endif
}
inline isize si_path_move(const char* existing_path, const char* new_path) {
	#if defined(SI_SYSTEM_WINDOWS)

	return (MoveFile(existing_path, new_path) != 0);

	#else

	if (link(existing_path, new_path) == 0) {
		return unlink(existing_path);
	}
	return SI_ERROR;

	#endif
}
inline isize si_path_remove(const char* path) {
	#if defined(SI_SYSTEM_WINDOWS)

	isize pos = si_string_rfind_ex((siString)path, si_cstr_len(path), 0, ".");
	if (pos != SI_ERROR) {
		return (DeleteFileA(path) != 0);
	}
	else {
		return (RemoveDirectoryA(path) != 0);
	}

	#elif defined(SI_SYSTEM_OSX)

	return unlink(filename);

	#else

	return remove(path);

	#endif
}

inline const char* si_path_base_name(const char* path) {
	isize pos = si_string_rfind_ex((siString)path, si_cstr_len(path) - 1, 0, &SI_PATH_SEPARATOR);

	return (pos != SI_ERROR ? (path + pos + 1) : path);
}
inline const char* si_path_extension(const char* path) {
	isize pos = si_string_rfind_ex((siString)path, si_cstr_len(path) - 1, 0, ".");

	return (pos != SI_ERROR ? (path + pos + 1) : path);
}
inline siString si_path_get_fullname(const char* path) {
	#if defined(SI_SYSTEM_WINDOWS)

	char buffer[MAX_PATH];
	GetFullPathNameA(path, MAX_PATH, buffer, NULL);

	return si_string_make(buffer);

	#else

	char actual_path[4096];
	realpath(path, actual_path);

	return si_string_make(actual_path);

	#endif
}

inline bool si_path_is_absolute(const char* path) {
	SI_ASSERT_NOT_NULL(path);

	#if defined(SI_WINDOWS)
	return (
		si_cstr_len(path) > 2 &&
		gb_char_is_alpha(path[0]) &&
		(path[1] == ':' && path[2] == GB_PATH_SEPARATOR)
	);
	#else
	return (1 < si_cstr_len(path) && path[0] == SI_PATH_SEPARATOR);
	#endif
}
inline bool si_path_is_relative(const char* path) {
	return !si_path_is_absolute(path);
}




inline siFile si_file_create(const char* path) {
	return si_file_open_mode(path, SI_FILE_MODE_CREATE);
}
inline siFile si_file_open(const char* path) {
	return si_file_open_mode(path, SI_FILE_MODE_READ);
}
siFile si_file_open_mode(const char* path, siFileMode mode) {
	SI_ASSERT_NOT_NULL(path);

	siString mode_str = nil;
	switch (mode) {
		case SI_FILE_MODE_READ: mode_str = "r"; break;
		case SI_FILE_MODE_WRITE: mode_str = "w"; break;
		case SI_FILE_MODE_READ_WRITE: mode_str = "r+"; break;
		case SI_FILE_MODE_APPEND: mode_str = "a"; break;
		case SI_FILE_MODE_APPEND_READ: mode_str = "a+"; break;
		case SI_FILE_MODE_CREATE: mode_str = "w+"; break;
		default: SI_ASSERT_MSG(mode_str != nil, "Provided mode doesn't exist"); break;
	}

	FILE* f = fopen(path, mode_str);

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
	return si_file_read_at(file, 0);
}
siString si_file_read_at(siFile file, usize offset) {
	SI_ASSERT_NOT_NULL(file.ptr);
	usize read_len = file.size - offset;
	char tmp[read_len];

	si_file_seek(file, offset);
	fread(tmp, read_len, 1, file.ptr);
	si_file_seek_to_end(file);

	return si_string_make_len(tmp, read_len);
}
siArray(siString) si_file_readlines(siFile file) {
	siString buffer = si_file_read(file);
	siArray(siString) res = si_string_split(buffer, "\n");
	si_string_free(buffer);

	return res;
}

inline isize si_file_write(siFile* file, const char* content) {
	return si_file_write_at_len(file, content, si_cstr_len(content), 0);
}
inline isize si_file_write_len(siFile* file, const char* content, usize len) {
	return si_file_write_at_len(file, content, len, 0);
}
inline isize si_file_write_at(siFile* file, const char* content, usize offset) {
	return si_file_write_at_len(file, content, si_cstr_len(content), offset);
}
isize si_file_write_at_len(siFile* file, const char* content, usize content_len, usize offset) {
	SI_ASSERT_NOT_NULL(file->ptr);
	SI_ASSERT_NOT_NULL(content);

	si_file_seek(*file, offset);
	isize result = fwrite(content, content_len, 1, file->ptr);
	si_file_seek_to_end(*file);

	si_file_size_update(file);
	return result;
}
isize si_file_write_at_line(siFile* file, const char* content, usize index) {
	SI_ASSERT_NOT_NULL(file->ptr);

	#if defined(SI_ARRAY_UNDEFINE)
	SI_ASSERT_MSG(SI_ARRAY_UNDEFINE != 1, "siArray is undefined, cannot use this function.");

	SI_UNUSED(file);
	SI_UNUSED(content);
	SI_UNUSED(index);

	return SI_ERROR;
	#else

	siArray(siString) buffer = si_file_readlines(*file);
	SI_ASSERT_MSG(0 <= index && index <= si_array_len(buffer), "Index is either not 0 or higher than the amount of lines in the file.");

	buffer[index] = (siString)content;
	siString new_file_content = si_array_to_sistring(buffer, "\n");

	siFile new_file = si_file_open_mode(file->path, SI_FILE_MODE_CREATE);
	si_file_write_len(&new_file, new_file_content, si_string_len(new_file_content));

	si_file_close(*file);
	*file = new_file;

	return SI_OKAY;
	#endif
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
		si_string_free(file.path);
	}

	if (file.ptr != nil) {
		fclose(file.ptr);
	}

	return SI_OKAY;
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
