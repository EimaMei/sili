/*
sili.h - An alternative to the C/C++ standard library.
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


#if defined(__CPLUS__)
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
	#include <sys/sendfile.h>
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

typedef u8 siByte;

static u8 SI_UINT8_MAX   =  0xFF;
static u8 SI_UINT8_MIN   =  0x00;
static i8 SI_INT8_MAX    =  0x7F;
static i8 SI_INT8_MIN    = -0x80;

static u16 SI_UINT16_MAX =  0xFFFF;
static u16 SI_UINT16_MIN =  0x0000;
static i16 SI_INT16_MAX  =  0x7FFF;
static i16 SI_INT16_MIN  = -0x8000;

static u32 SI_UINT32_MAX =  0xFFFFFFFF;
static u32 SI_UINT32_MIN =  0x00000000;
static i32 SI_INT32_MAX  =  0x7FFFFFFF;
static i32 SI_INT32_MIN  = -0x80000000;

static u64 SI_UINT64_MAX =  0xFFFFFFFFFFFFFFFF;
static u64 SI_UINT64_MIN =  0x0000000000000000;
static i64 SI_INT64_MAX  =  0x7FFFFFFFFFFFFFFF;
static i64 SI_INT64_MIN  = -0x8000000000000000;

#if defined(SI_ARCH_64_BIT)
	#define SI_USIZE_MAX SI_UINT64_MAX
	#define SI_USIZE_MIN SI_UINT64_MIN
	#define SI_ISIZE_MAX SI_INT64_MAX
	#define SI_ISIZE_MIN SI_INT64_MIN
#else
	#define SI_USIZE_MAX SI_UINT32_MAX
	#define SI_USIZE_MIN SI_UINT32_MIN
	#define SI_ISIZE_MAX SI_INT32_MAX
	#define SI_ISIZE_MIN SI_INT32_MIN
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

#if !defined(cstr)
	typedef const char* cstring;
#endif

#define SI_BIT(x) (1 << (x))

#define SI_KILO(x) (       (x) * (usize)1024)
#define SI_MEGA(x) (SI_KILO(x) * (usize)1024)
#define SI_GIGA(x) (SI_MEGA(x) * (usize)1024)
#define SI_TERA(x) (SI_GIGA(x) * (usize)1024)

#define SI_MULTILINE(...) __VA_ARGS__
#define SI_STR_MULTILINE(...) #__VA_ARGS__

#define SI_ENUM_DECLARE(type, name) type name; enum

#define SI_OKAY  0
#define SI_ERROR -1

#define SI_UNUSED(x) (void)(x) /* NOTE(EimaMei) Should be used when you want to supress "parameter 'X' set but not used" errors. */

#define si_swap(a, b) do { typeof((a)) tmp = (a); (a) = (b); (b) = tmp; } while (0)
#define si_between(x, lower, upper) (((lower) <= (x)) && ((x) <= (upper)))

#if defined(SI_STANDARD_ANSI) || defined(SI_COMPILER_MSVC) /* NOTE(EimaMei): Fuck Microsoft. */
	#define typeof(value) __typeof__(value)
#endif

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
	| SI_ASSERT            |
	========================
*/

usize si_impl_assert_msg(bool condition, cstring condition_str, cstring message, cstring file, i32 line);
#define SI_ASSERT_MSG(condition, message) si_impl_assert_msg(condition, #condition, message, __FILE__, __LINE__)
#define SI_ASSERT(condition) SI_ASSERT_MSG(condition, nil)
#define SI_ASSERT_NOT_NULL(ptr) SI_ASSERT_MSG((ptr) != nil, #ptr " must not be NULL")

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
	| siAny & siFunction   |
	========================
*/

typedef struct siAny {
	usize type_width;
	rawptr ptr;
} siAny;

#define si_any_make(value) (siAny){sizeof(typeof(value)), &(typeof(value)){value} }
#define si_any_get(any, type) ((any.ptr != nil) ? *((type*)any.ptr) : (type)SI_ASSERT_NOT_NULL(any.ptr))

typedef struct siFunction {
   rawptr (*ptr)(rawptr);
} siFunction;

#define siFunc(func) (siFunction){(rawptr (*)(rawptr))func}

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

#define si_array_replace(array_address, old_value, new_value) si_impl_array_replace(array_address, si_any_make(old_value), si_any_make(new_value))
isize si_array_reverse(siArray(void) array);
char* si_array_to_sistring(siArray(char*) array, cstring separator);

#define si_array_append(array_address, value) si_impl_array_append(array_address, si_any_make(value))
#define si_array_push_back(array_address, value) si_array_append(array_address, value)

isize si_array_clear(rawptr array_address);
bool si_arrays_are_equal(siArray(void) lha, siArray(void) rha);

isize si_array_free(siArray(void) array);

/* NOTE(EimaMei): The actual implementations. Shouldn't be used in practice, really. */
isize si_impl_array_find_ex(rawptr array, usize start, usize end, siAny value);
isize si_impl_array_rfind_ex(rawptr array, usize start, usize end, siAny value);
isize si_impl_array_append(rawptr array_address, siAny value);
isize si_impl_array_replace(rawptr array_address, siAny old_value, siAny new_value);

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

typedef siArray(char) siString;
typedef siArray(i32) siRunes;
#define SI_STRING_HEADER(s) ((siStringHeader*)s - 1)


siString si_string_make(cstring str);
siString si_string_make_fmt(cstring str, ...);
siString si_string_make_len(cstring str, usize len);
siString si_string_make_reserve(usize len);

siString si_string_copy(siString from);

usize si_string_len(siString str);
usize si_string_capacity(siString str);
usize si_string_type_width(siString str);
bool si_string_is_empty(siString str);

char si_string_at(siString str, usize index);
char si_string_front(siString str);
char si_string_back(siString str);
void si_string_sub(siString str, usize begin, usize end, char* result);

isize si_string_find(siString str, cstring cstr);
isize si_string_find_ex(siString str, usize start, usize end, cstring cstr);
isize si_string_rfind(siString str, cstring cstr);
isize si_string_rfind_ex(siString str, usize start, usize end, cstring cstr);

isize si_string_join(siString* str, cstring cstr, cstring separator);
isize si_string_set(siString* str, cstring cstr);
isize si_string_replace(siString* str, cstring old_str, cstring new_str);
isize si_string_trim(siString* str, cstring cut_set);

isize si_string_append(siString* str, cstring other);
isize si_string_append_len(siString* str, cstring other, usize other_len);
isize si_string_push_back(siString* str, char other);

void si_string_insert(siString* str, cstring cstr, usize index);
void si_string_erase(siString* str, usize begin, usize end);
void si_string_remove_cstr(siString* str, cstring cstr);
void si_string_swap(siString* str, cstring cstr1, cstring cstr2);

isize si_string_upper(siString* str);
isize si_string_lower(siString* str);
isize si_string_title(siString* str);
isize si_string_capitalize(siString* str);
isize si_string_strip(siString* str); /* NOTE(EimaMei); This strips any leading or trailing spaces in the string. */
isize si_string_reverse(siString* str);
isize si_string_reverse_len(siString* str, usize len);


siArray(siString) si_string_split(siString str, cstring separator);
bool si_strings_are_equal(cstring lhs, cstring rhs);
isize si_string_clear(siString* str);

isize si_string_free(siString str);
isize si_string_make_space_for(siString* str, usize add_len);
void si_string_shrink_to_fit(siString str);

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

u64 si_cstr_to_u64(cstring str);
i64 si_cstr_to_i64(cstring str);
/* TODO(EimaMei): si_cstr_to_f64 */

cstring si_u64_to_cstr(u64 num);
cstring si_i64_to_cstr(i64 num);

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
		usize id;
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

typedef SI_ENUM_DECLARE(usize, siMonth) {
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
typedef SI_ENUM_DECLARE(usize, siWeek) {
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

#ifdef SI_IMPLEMENTATION
	#define SI_GENERAL_IMPLEMENTATION
	#define SI_PAIR_IMPLEMENTATION
	#define SI_OPTIONAL_IMPLEMENTATION
	#define SI_ARRAY_IMPLEMENTATION
	#define SI_STRING_IMPLEMENTATION
	#define SI_CHAR_IMPLEMENTATION
	#define SI_FILE_IMPLEMENTATION
	#define SI_THREAD_IMPLEMENTATION
	#define SI_PERFORMANCE_IMPLEMENTATION
#endif

#if defined(SI_GENERAL_IMPLEMENTATION)

usize si_impl_assert_msg(bool condition, cstring condition_str, cstring message, cstring file, i32 line) {
	if (!condition) {
		fprintf(stderr, "Assertion \"%s\" at \"%s:%d\"%s %s\n", condition_str, file, line, (message != nil ? ":" : ""), (message != nil ? message : ""));
		abort();
	}

	return 0;
}

#if defined(SI_MEMORY_LOGGING)
	#undef malloc
	#undef si_realloc
	#undef free
#endif

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
#endif

#if defined(SI_PAIR_IMPLEMENTATION) && !defined(SI_PAIR_UNDEFINE)

rawptr si_impl_pair_make(siAny first, siAny second) {
	siByte* res = (siByte*)malloc(first.type_width + second.type_width);
	memcpy(res, first.ptr, first.type_width);
	memcpy(res + first.type_width, second.ptr, second.type_width);

	return (rawptr)res;
}
#endif

#if defined(SI_ARRAY_IMPLEMENTATION) && !defined(SI_ARRAY_UNDEFINE)

inline siArray(void) si_array_make_list(rawptr list, usize sizeof_element, usize count)  {
	rawptr array = si_array_make_reserve(sizeof_element, count);
	memcpy(array, list, sizeof_element * count);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	header->len = count;

	return array;
}
inline siArray(void) si_array_copy(siArray(void) array) {
	siArrayHeader* header = SI_ARRAY_HEADER(array);
	return si_array_make_list(array, header->type_width, header->len);
}
siArray(void) si_array_make_reserve(usize sizeof_element, usize count) {
	siByte* ptr = (siByte*)malloc(sizeof(siArrayHeader) + (sizeof_element * count));
	siArray(void) array = ptr + sizeof(siArrayHeader);

	siArrayHeader* header = SI_ARRAY_HEADER(array);
	header->len = 0; /* TODO(EimaMei): FIX THIS!!!!!!!!!!!!!!!!!!!!! */
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
	return si_array_capacity(array) * si_array_type_width(array);
}

inline siAny si_array_at(siArray(void) array, usize index) {
	SI_ASSERT_MSG(si_between(index, 0, si_array_len(array)), "Array is empty OR the provided index is too high.");

	rawptr p = (siByte*)array + index * si_array_type_width(array);
	return (siAny){si_array_type_width(array), p};
}
inline siAny si_array_front(siArray(void) array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	return (siAny){si_array_type_width(array), array};
}
inline siAny si_array_back(siArray(void) array) {
	SI_ASSERT_MSG(si_array_len(array) != 0, "Array is empty.");

	rawptr p = (siByte*)array + (si_array_len(array) - 1) * si_array_type_width(array);
	return (siAny){si_array_type_width(array), p};
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

isize si_impl_array_replace(rawptr array_address, siAny old_value, siAny new_value) {
	siByte* array = *((siByte**)array_address);
	usize type_width = si_array_type_width(array);
	SI_ASSERT_MSG(si_between(new_value.type_width, old_value.type_width, type_width), "The given value's sizeof is too large compared to the elements' in the array.");

	isize pos = si_impl_array_find_ex(array, 0, si_array_len(array), old_value);
	if (pos == SI_ERROR) {
		return SI_ERROR;
	}

	memcpy(array + pos * type_width, new_value.ptr, new_value.type_width);

	return pos;
}
isize si_array_reverse(siArray(void) array) {
	SI_ASSERT_NOT_NULL(array);
	siByte* array_pointer = *((siByte**)array);

	usize len = si_array_len(array_pointer);
	usize type_width = si_array_type_width(array_pointer);
	SI_ASSERT_MSG(len != 0, "Array is empty");

	siByte* a = array_pointer;
	siByte* b = array_pointer + (len - 1) * type_width;
	static rawptr tmp[1];
	len *= 0.5;

	while (len--) {
		memcpy(tmp, a, type_width);
		memcpy(a, b, type_width);
		memcpy(b, tmp, type_width);

		a += type_width, b -= type_width;
	}

	return SI_OKAY;
}
siString si_array_to_sistring(siArray(char*) array, cstring separator) {
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

isize si_impl_array_append(rawptr array_address, siAny value) {
	siByte** array = ((siByte**)array_address);
	siArrayHeader* header = SI_ARRAY_HEADER(*array);
	SI_ASSERT_MSG(value.type_width <= header->type_width, "The given value's sizeof is too large compared to the elements' in the arra");

	if (header->len == header->capacity) {
		siByte* result = (siByte*)si_realloc(header, sizeof(siArrayHeader) + header->capacity * header->type_width, sizeof(siArrayHeader) + 2 * header->capacity * header->type_width);
		SI_ASSERT_NOT_NULL(result);

		*array = result + sizeof(siArrayHeader);
		header = (siArrayHeader*)result;

		header->capacity *= 2;
	}

	rawptr res = memcpy(*array + header->len * header->type_width, value.ptr, header->type_width);
	SI_ASSERT_NOT_NULL(res);
	header->len += 1;

	return SI_OKAY;
}

isize si_array_clear(rawptr array_address) {
	SI_ASSERT_NOT_NULL(array_address);

	rawptr array = *((rawptr*)array_address);

	rawptr result = memset(array, 0, si_array_size_in_bytes(array));
	SI_ASSERT_NOT_NULL(result);

	return SI_OKAY;
}

bool si_arrays_are_equal(siArray(void) lha, siArray(void) rha) {
	siArrayHeader* lha_header = SI_ARRAY_HEADER(lha);
	siArrayHeader* rha_header = SI_ARRAY_HEADER(rha);

	if (lha_header->len != rha_header->len || lha_header->type_width != rha_header->type_width) {
		return false;
	}

	usize width = lha_header->type_width;
	usize i;
	for (i = 0; i < lha_header->len; i++) {
		if (memcmp((siByte*)lha + i * width, (siByte*)rha + i * width, width) != SI_OKAY) {
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
	header->type_width = sizeof(char);

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
inline usize si_string_type_width(siString str) {
	return SI_STRING_HEADER(str)->type_width;
}
inline bool si_string_is_empty(siString str) {
	return (SI_STRING_HEADER(str)->len == 0);
}


inline usize si_cstr_len(cstring str) {
	cstring s;
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
void si_string_sub(siString str, usize begin, usize end, char* result) {
	usize len = end - begin + 1;
	memcpy(result, str + begin, len);
	*(result + len) = '\0';
}

inline isize si_string_find(siString str, cstring cstr) {
	return si_string_find_ex(str, 0, si_string_len(str), cstr);
}
isize si_string_find_ex(siString str, usize start, usize end, cstring cstr) {
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

	return (found ? i : SI_ERROR);
}

isize si_string_join(siString* str, cstring cstr, cstring separator) {
	usize cstr_len = si_cstr_len(cstr);
	usize separator_len = si_cstr_len(separator);

	char tmp[separator_len + cstr_len + 1];
	memcpy(tmp, separator, separator_len);
	memcpy(tmp + separator_len, cstr, cstr_len);
	tmp[separator_len + cstr_len] = '\0';

	return si_string_append_len(str, tmp, separator_len + cstr_len);
}
isize si_string_set(siString* str, cstring cstr) {
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
isize si_string_replace(siString* str, cstring old_value, cstring new_value) {
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
isize si_string_trim(siString* str, cstring cut_set) {
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

inline isize si_string_append(siString* str, cstring other) {
	return si_string_append_len(str, other, si_cstr_len(other));
}
isize si_string_append_len(siString* str, cstring other, usize other_len) {
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

void si_string_insert(siString* str, cstring cstr, usize index) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize previous_len = header->len;
	usize cstr_len = si_cstr_len(cstr);
	usize before_index_len = previous_len - index;
	header->len += cstr_len;

	if (header->capacity < header->len) {
		isize result = si_string_make_space_for(str, cstr_len);
		SI_ASSERT_MSG(result == SI_OKAY, "Failed to make space for string");
		header = SI_STRING_HEADER(*str); /* NOTE(EimaMei): For some reason we have to refresh the header pointer on Linux. Somewhat makes sense but also what and why. */
	}
	siString cur_str = *str;

	char* ptr = (char*)memcpy(cur_str + header->len - before_index_len, cur_str + index, before_index_len);
	memcpy(cur_str + index + 1, cstr, cstr_len);
	SI_ASSERT_NOT_NULL(ptr);
	ptr[header->len] = '\0';
}
void si_string_erase(siString* str, usize begin, usize end) {
	usize after_index_len = begin + end;
	siString cur_str = *str;

	char* ptr = (char*)memcpy(cur_str + begin, cur_str + after_index_len, si_string_len(cur_str) - after_index_len);
	SI_ASSERT_NOT_NULL(ptr);
	ptr[si_string_len(cur_str) - after_index_len] = '\0';

	SI_STRING_HEADER(cur_str)->len -= after_index_len;
}
void si_string_remove_cstr(siString* str, cstring cstr) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	usize cstr_len = si_cstr_len(cstr);

	usize cur_len;
	siString cur_str;
	while (true) {
		cur_str = *str;
		cur_len = si_string_len(cur_str);

		isize index = si_string_rfind_ex(cur_str, cur_len - 1, 0, cstr);
		if (index == -1) {
			break;
		}

		usize after_index_len = index + cstr_len;

		char* ptr = (char*)memcpy(cur_str + index, cur_str + after_index_len, cur_len - after_index_len);
		SI_ASSERT_NOT_NULL(ptr);
		ptr[cur_len - after_index_len] = '\0';

		header->len -= cstr_len;
	}
}
void si_string_swap(siString* str, cstring cstr1, cstring cstr2);

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
inline isize si_string_reverse(siString* str) {
	usize len = si_string_len(*str);
	SI_ASSERT_MSG(len != 0, "String is empty");
	return si_string_reverse_len(str, len);
}
isize si_string_reverse_len(siString* str, usize len) {
	siString actual_str = *str;

	char* a = actual_str;
	char* b = actual_str + len - 1;
	len *= 0.5;

	while (len--) {
		si_swap(*a, *b);
		a++, b--;
	}

	*str = actual_str;
	return SI_OKAY;
}

siArray(siString) si_string_split(siString str, cstring separator) {
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
	siArray(siString) res = (siArray(siString))si_array_make_reserve(sizeof(*res), count); /* NOTE(EimaMei): Fuck C++. Seriously, what the fuck? WHY DO WE HAVE TO THIS??? */
	SI_ARRAY_HEADER(res)->len = count;

	usize i;
	for (i = 0; i < count; i++) {
		res[i] = si_string_make_len(original, pos_buffer[i]);
		original += pos_buffer[i] + separator_len;
	}


	return res;
}
bool si_strings_are_equal(cstring lhs, cstring rhs) {
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
inline isize si_string_clear(siString* str) {
	char null_terminator = '\0';
	isize result = si_string_set(str, &null_terminator);

	return result;
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
void si_string_shrink_to_fit(siString str);
#endif

#if defined(SI_CHAR_IMPLEMENTATION) && !defined(SI_STRING_UNDEFINE)

static const char si_impl_num_to_char_table[] = /* NOTE(EimaMei): Required for converting nums to chars quickly. */
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"@$";

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
		*cur_char++ = si_impl_num_to_char_table[num % 10];
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
		*cur_char++ = si_impl_num_to_char_table[value % 10];
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

		isize size = sendfile(new_fd, existing_fd, 0, stat_existing.st_size);

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
	#elif defined(SI_SYSTEM_OSX)
		return unlink(filename);
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
	return si_file_open_mode(path, "w");
}
inline siFile si_file_open(cstring path) {
	return si_file_open_mode(path, "r");
}
siFile si_file_open_mode(cstring path, siFileMode mode) {
	SI_ASSERT_NOT_NULL(path);

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
	siString buffer = si_file_read(file);
	siArray(siString) res = si_string_split(buffer, "\n");
	si_string_free(buffer);

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
	siString previous_line = buffer[index];
	SI_ASSERT_MSG(0 <= index && index <= si_array_len(buffer), "Index is either not 0 or higher than the amount of lines in the file.");


	buffer[index] = (siString)content;
	siString new_file_content = si_array_to_sistring(buffer, "\n");

	siFile new_file = si_file_open_mode(file->path, SI_FILE_MODE_CREATE);
	si_file_write_len(&new_file, new_file_content, si_string_len(new_file_content));

	usize i;
	for (i = 0; i < si_array_len(buffer); i++) {
		if (i == index) {
			continue;
		}
		si_string_free(buffer[i]);
	}
	si_string_free(previous_line);
	si_array_free(buffer);

	si_string_free(new_file_content);
	si_file_close(*file);
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
		si_string_free(file.path);
	}

	if (file.ptr != nil) {
		fclose(file.ptr);
	}

	return SI_OKAY;
}

#endif

#if defined(SI_THREAD_IMPLEMENTATION) && !defined(SI_THREAD_UNDEFINE)

inline rawptr si_impl_thread_proc(rawptr arg) {
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
	usize minimum = 0;
	#if !defined(SI_SYSTEM_WINDOWS)
		minimum = PTHREAD_STACK_MIN;
	#endif
	si_thread_start_stack(t, minimum);
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
	#else
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
		header->type_width = sizeof(siDebugHeader*);
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

			header->type = "si_realloc";
			header->len = args.second;
			header->state = 2;
			header->date = date;
			header->time = time;
			header->file = file;
			header->line = line;
			header->function = function;

			return res + sizeof(siDebugHeader);
		}
		case 4: { /* si_realloc */
			if (ptr == SI_ARRAY_HEADER(si_debug.list)) {
				res = si_realloc(ptr, args.first, args.second);
				return res;
			}

			siDebugHeader* header = SI_DEBUG_HEADER(ptr);
			si_debug.allocs += 1;
			si_debug.frees += 1;

			res = si_realloc(header, sizeof(siDebugHeader) + args.first, sizeof(siDebugHeader) + args.second);

			header->type = "si_realloc";
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
#define si_realloc(ptr, old_size, new_size) si_debug_alloc(4, ptr, (siDebugArgs){old_size, new_size}, __FILE__, __LINE__, __func__, si_u64_to_cstr(si_time_utc_now()), __DATE__)
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
