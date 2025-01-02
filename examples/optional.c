#define SI_IMPLEMENTATION 1
#include <sili.h>


SI_ENUM(usize, Type) {
	Type_i32 = 0,
	Type_string,
	Type_array,
	Type_struct,
	Type_enum,
	Type_funcPtr,

	Type_len
};

typedef struct {
	u64 high, low;
} u128Struct;

typedef struct {
	siString name;
	b32 isAdmin;
	u32 moneis;
} userInfo;

/* For additional siOptional(<TYPE>) types, you need to define them once beforehand. */
si_optional_define(Type);
si_optional_define(u128Struct);
si_optional_define(cstring);
si_optional_define(userInfo);


/* Shows off the basic usage of 'siOption' */
void example1(void);
/* Shows siOption's compatibility with many types. */
void example2(siAllocator stack);
/* Shows off the difference between 'siOption' and 'siResult'. */
void example3(void);


int main(void) {
	siAllocatorArena aData = si_arenaMakePtr(si_stackAlloc(256), SI_DEFAULT_MEMORY_ALIGNMENT);
	siAllocator stack = si_allocatorArena(&aData);

	example1();
	example2(stack);
	example3();

	return 0;
}

/* Returns an optional object either with or without a value depending on the
 * specified boolean. */
siOption(cstring) create(b32 value);

void example1(void) {
	/* Based on https://en.cppreference.com/w/cpp/utility/optional. */
	si_print("==============\n\n==============\nExample 1:\n");

	siOption(cstring) str = create(false);
	si_printf(
		"create(false) returned '%s' (hasValue: %B)\n",
		si_optionalGetOrDefault(str, "empty"), str.hasValue
	);

	str = create(true);
	SI_ASSERT(str.hasValue);

	/* C11 and onwards, you can access the returned data from an 'siOption' variable
	 * via '.data' (or '.error' if it's an error). However, if you need to keep
	 * your code C99 compliant, you can use '.data.value' for it to work on
	 * multiple standards. */
#if SI_STANDARD_CHECK_MAX(C, C99)
	si_printf("create2(true) returned '%s'\n", str.data.value);

#else
	si_printf("create2(true) returned '%s'\n", str.value);

#endif
}

/* Creates an optional object from the specified type and writes it into the
 * given raw pointer.*/
void createOptional(Type type, rawptr out, siAllocator alloc);

void example2(siAllocator alloc) {
	si_print("==============\n\n==============\nExample 2:\n");

	siOption(i32) opt_i32;
	siOption(siString) opt_string;
	siOption(siArray(i32)) opt_buffer;
	siOption(u128Struct) opt_u128;
	siOption(Type) opt_type;
	siOption(rawptr) opt_ptr;

	rawptr opt_array[Type_len] = {
		&opt_i32, &opt_string, &opt_buffer, &opt_u128, &opt_type, &opt_ptr
	};
	for_range (i, 0, Type_len) {
		createOptional((Type)i, opt_array[i], alloc);
	}

	u8 buf[64];
	si_printf("Element 1: '%X'\n", opt_i32.data.value);
	si_printf("Element 2: '%S'\n", opt_string.data.value);
	si_printf("Element 3: '%S'\n", si_stringFromArray(opt_buffer.data.value, "%i", buf, sizeof(buf)));
	si_printf("Element 4: '0x%016lX|%016lX'\n", opt_u128.data.value.high, opt_u128.data.value.low);
	si_printf("Element 5: '%zd'\n", opt_type.data.value);
	si_printf("Element 6: '%p'\n", opt_ptr.data.value);
}

#define INVALID_ID 1
#define ACCESS_DENIED 2

/* Returns user information from the given index. If the identification is over
 * the total user count, 'INVALID_ID' is returned. If the user is an administrator,
 * 'ACCESS_DENIED' is returned. */
siResult(userInfo) get_name(u32 identification);

void example3(void) {
	si_print("==============\n\n==============\nExample 3:\n");

	for_range (id, 0, 3u) {
		siResult(userInfo) res = get_name(id);

		if (res.hasValue) {
			si_printf("ID %u: %S moneis - %u cents\n", id, res.data.value.name, res.data.value.moneis);
		}
		else {
			u8 buf[64];
			siError err = res.data.error;
			siString time = si_timeToString(buf, countof(buf), si_timeToCalendar(err.time), SI_STR("yyyy-MM-dd hh:mm:ss"));
			si_printf(
				"Couldn't get info on ID '%u': Error '%u' ('%s:%i', occurred on '%S')\n",
				id, err.code, err.filename, err.line, time
			);
		}
	}
}


siOption(cstring) create(b32 value) {
	return value ? SI_OPT(cstring, "Godzilla") : SI_OPT_NIL(cstring);
}

void createOptional(Type type, rawptr out, siAllocator alloc) {
	switch (type) {
		case Type_i32: {
			siOption(i32)* res = out;
			*res = SI_OPT(i32, INT32_MIN);
		} break;

		case Type_string: {
			siOption(siString)* res = out;
			*res = SI_OPT(siString, si_stringMake("Ayn Rand", alloc));
		} break;

		case Type_array: {
			siOption(siArray(i32))* res = out;
			*res = SI_OPT(siBuffer, si_arrayMake(alloc, i32, 1, 2, 4, 6, 8));
		} break;

		case Type_struct: {
			siOption(u128Struct)* res = out;
			*res = SI_OPT(u128Struct, {0xFF, UINT64_MAX});
		} break;

		case Type_enum: {
			siOption(Type)* res = out;
			*res = SI_OPT(Type, Type_enum);
		} break;

		case Type_funcPtr: {
			typedef void (create_optional_type)(Type, rawptr, siAllocator);

			siOption(rawptr)* res = out;
			*res = SI_OPT(rawptr, si_transmute(rawptr, createOptional, create_optional_type*));
		} break;

		default: SI_PANIC();
	}
}

siResult(userInfo) get_name(u32 identification) {
	static userInfo database[] = {
		{SI_STRC("Joe"), false, 4000 * 100},
		{SI_STRC("Gitanas Nausėda"), true, UINT32_MAX}
	};

	if (identification >= countof(database)) {
		return SI_OPT_ERR(userInfo, SI_ERROR(INVALID_ID));
	}
	else if (database[identification].isAdmin) {
		return SI_OPT_ERR(userInfo, SI_ERROR(ACCESS_DENIED));
	}

	return SI_OPT(userInfo, database[identification]);
}
