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
	bool isAdmin;
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
siOption(cstring) create(bool value);

void example1(void) {
	/* Based on https://en.cppreference.com/w/cpp/utility/optional. */
	si_printLn("==============\n\n==============\nExample 1:");

	siOption(cstring) str = create(false);
	si_printfLn(
		"create(false) returned '%s' (hasValue: %t)",
		si_optionalGetOrDefault(str, "empty"), str.hasValue
	);

	str = create(true);
	SI_ASSERT(str.hasValue);

	si_printfLn("create2(true) returned '%s'", str.value);
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

	siArena arena = si_arenaMakePtr(si_stackAlloc(64));
	siAllocator alloc = si_allocatorArena(&arena);
	for_range (i, 0, Type_len) {
		createOptional((Type)i, opt_array[i], alloc);
	}

	si_printfLn("Element 1: '%X'", opt_i32.value);
	si_printfLn("Element 2: '%s'", opt_string.value);
	si_printfLn("Element 3: '%s'", si_stringFromArray(opt_buffer.value, "%i", SI_ARR_STACK(64)));
	si_printfLn("Element 4: '0x%016lX|%016lX'", opt_u128.value.high, opt_u128.value.low);
	si_printfLn("Element 5: '%zd'", opt_type.value);
	si_printfLn("Element 6: '%p'", opt_ptr.value);
}

#define INVALID_ID 1
#define ACCESS_DENIED 2

/* Returns user information from the given index. */
siResult(userInfo) get_name(isize identification);
/* TODO */
SI_ERROR_PROC(custom_error_log);

void example3(void) {
	si_print("==============\n\n==============\nExample 3:\n");

	for_range (id, 0, 3) {
		siResult(userInfo) res = get_name(id);

		if (res.hasValue) {
			si_printfLn("ID %u: %s moneis - %u cents", id, res.value.name, res.value.moneis);
		}
		else {
			si_printLn("Something happened, I don't know.");
		}
	}
}


siOption(cstring) create(bool value) {
	return value ? SI_OPT(cstring, "Godzilla") : SI_OPT_ERR(cstring);
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
			siOptionPtr(void)* res = (siOptionPtr(void)*)out;
			*res = SI_OPT_PTR(void, transmute(void*, (typeof(createOptional)*)createOptional));
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
		si_errorDeclare(INVALID_ID, nil, custom_error_log, &identification);
		return SI_OPT_ERR(userInfo);
	}
	else if (database[identification].isAdmin) {
		return SI_OPT_ERR(userInfo);
	}

	return SI_OPT(userInfo, database[identification]);
}


SI_ERROR_PROC(custom_error_log) {
	siString time = si_timeToString(
		si_timeToCalendar(error->time), SI_STR("yyyy-MM-dd hh:mm:ss"), SI_ARR_STACK(64)
	);
			
	si_printfLn(
		"Couldn't get info on ID '%i': Error '%i' ('%L', occurred on '%s')",
		*(isize*)data, error->code, error->location, time
	);

	return 0;
}
