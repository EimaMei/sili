#define SI_IMPLEMENTATION 1
#include <sili.h>


SI_ENUM(isize, Type) {
	Type_i32 = 0,
	Type_string,
	Type_buffer,
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
void example2(void);
/* Shows off the difference between 'siOption' and 'siResult'. */
void example3(void);


int main(void) {
	example1();
	example2();
	example3();

	return 0;
}

/* Returns an optional object either with or without a value depending on the
 * specified boolean. */
siOption(cstring) create(b32 value);

void example1(void) {
	/* Based on https://en.cppreference.com/w/cpp/utility/optional. */
	si_printLn("==============\n\n==============\nExample 1:");

	siOption(cstring) str = create(false);
	si_printfLn(
		"create(false) returned '%s' (hasValue: %B)",
		si_optionalGetOrDefault(str, "empty"), str.hasValue
	);

	str = create(true);
	SI_ASSERT(str.hasValue);

	/* For C11 and above targets, you can access any opional variable's value
	 * via '.data' (or '.error' if it's an error).
	 *
	 * However, if you need to keep your code C99/C++20 compliant, you have to
	 * use '.data.value' internally. for it to work on
	 * multiple standards. */
#if SI_STANDARD_CHECK_MIN(C, C11)
	si_printfLn("create2(true) returned '%s'", str.value);

#else
	si_printfLn("create2(true) returned '%s'", str.data.value);

#endif
}

/* Creates an optional object from the specified type and writes it into the
 * given raw pointer.*/
void createOptional(Type type, void* out, siAllocator alloc);

void example2(void) {
	si_printLn("==============\n\n==============\nExample 2:");

	siOption(i32) opt_i32;
	siOption(siString) opt_string;
	siOption(siArray(i32)) opt_buffer;
	siOption(u128Struct) opt_u128;
	siOption(Type) opt_type;
	siOptionPtr(void) opt_ptr;

	void* opt_array[Type_len] = {
		&opt_i32, &opt_string, &opt_buffer, &opt_u128, &opt_type, &opt_ptr
	};

	siArena arena = si_arenaMakePtr(si_stackAlloc(64), 1);
	siAllocator alloc = si_allocatorArena(&arena);
	for_range (i, 0, Type_len) {
		createOptional((Type)i, opt_array[i], alloc);
	}

	si_printfLn("Element 1: '%X'", opt_i32.data.value);
	si_printfLn("Element 2: '%S'", opt_string.data.value);
	si_printfLn("Element 3: '%S'", si_stringFromArray(opt_buffer.data.value, "%i", SI_ARR_STACK(64)));
	si_printfLn("Element 4: '0x%016lX|%016lX'", opt_u128.data.value.high, opt_u128.data.value.low);
	si_printfLn("Element 5: '%zd'", opt_type.data.value);
	si_printfLn("Element 6: '%p'", opt_ptr.data.value);
}

#define INVALID_ID 1
#define ACCESS_DENIED 2

/* Returns user information from the given index. If the identification is over
 * the total user count, 'INVALID_ID' is returned. If the user is an administrator,
 * 'ACCESS_DENIED' is returned. */
siResult(userInfo) get_name(isize identification);

void example3(void) {
	si_print("==============\n\n==============\nExample 3:\n");

	for_range (id, 0, 3) {
		siResult(userInfo) res = get_name(id);

		if (res.hasValue) {
			si_printfLn("ID %u: %S moneis - %u cents", id, res.data.value.name, res.data.value.moneis);
		}
		else {
			siError err = res.data.error;
			siString time = si_timeToString(si_timeToCalendar(err.time), SI_STR("yyyy-MM-dd hh:mm:ss"), SI_ARR_STACK(64));
			si_printfLn(
				"Couldn't get info on ID '%u': Error '%u' ('%L', occurred on '%S')",
				id, err.code, err.location, time
			);
		}
	}
}


siOption(cstring) create(b32 value) {
	return value ? SI_OPT(cstring, "Godzilla") : SI_OPT_NIL(cstring);
}

void createOptional(Type type, void* out, siAllocator alloc) {
	switch (type) {
		case Type_i32: {
			siOption(i32)* res = (siOption(i32)*)out;
			*res = SI_OPT(i32, INT32_MIN);
		} break;

		case Type_string: {
			siOption(siString)* res = (siOption(siString)*)out;
			*res = SI_OPT(siString, SI_STR("Ayn Rand"));
		} break;

		case Type_buffer: {
			siOption(siArray(i32))* res = (siOption(siArray(i32))*)out;
			*res = SI_OPT(siArray(i32), si_arrayMake(alloc, i32, 1, 2, 4, 6, 8));
		} break;

		case Type_struct: {
			siOption(u128Struct)* res = (siOption(u128Struct)*)out;
			*res = SI_OPT(u128Struct, {0xFF, UINT64_MAX});
		} break;

		case Type_enum: {
			siOption(Type)* res = (siOption(Type)*)out;
			*res = SI_OPT(Type, Type_enum);
		} break;

		case Type_funcPtr: {
			typedef void (create_optional_type)(Type, void*, siAllocator);

			siOptionPtr(void)* res = (siOptionPtr(void)*)out;
			*res = SI_OPT_PTR(void, si_transmute(void*, createOptional, create_optional_type*));
		} break;

		default: SI_PANIC();
	}
}

siResult(userInfo) get_name(isize identification) {
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
