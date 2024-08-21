#define SI_IMPLEMENTATION 1
#include <sili.h>


SI_ENUM(usize, valueType) {
	TYPE_I32 = 0,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_STRUCT,
	TYPE_ENUM,
	TYPE_FUNC_PTR,

	SI_TYPE_COUNT
};

typedef struct u128Struct {
	u64 high, low;
} u128Struct;


si_optional_define(valueType);

si_optional_define(u128Struct);

si_optional_define(cstring);


/* Example 1 shows off the very basics of 'siOption' and how it can be usually
 * used. */
void example1(void);
/* Example 2 shows 'siOption' type's compatibility with a wide variety of C
 * types. */
void example2(siAllocator* stack);


int main(void) {
	siAllocator stack = si_allocatorMakeStack(0xFF);

	example1();
	example2(&stack);

	return 0;
}



siOption(cstring) create(b32 value);
void createOptional(valueType type, rawptr out, siAllocator* alloc);

void print_arrI(siArrayType(i32) array);


void example1(void) {
	/* Based on https://en.cppreference.com/w/cpp/utility/optional. */
	si_printf("==============\n\n==============\nExample 5.0:\n");

	siOption(cstring) str = create(false);
	si_printf(
		"create(false) returned '%s' (hasValue: %B)\n",
		si_optionalGetOrDefault(str, "empty"), str.hasValue
	);

	str = create(true);
	SI_ASSERT(str.hasValue);

#if SI_STANDARD_CHECK_MAX(C, C99)
	si_printf("create2(true) returned '%s'\n", str.data.value);

#else
	/* C11 and onwards you do not have to specify '.data' to access the value
	 * or error if you do not care to keep your code C99 compliant. */
	si_printf("create2(true) returned '%s'\n", str.value);

#endif
}

void example2(siAllocator* alloc) {
	si_printf("==============\n\n==============\nExample 5.1:\n");

	siOption(i32) opt_i32;
	siOption(siString) opt_string;
	siOption(siArrayType(i32)) opt_buffer;
	siOption(u128Struct) opt_u128;
	siOption(valueType) opt_type;
	siOption(rawptr) opt_ptr;

	rawptr opt_array[] = {
		&opt_i32, &opt_string, &opt_buffer, &opt_u128, &opt_type, &opt_ptr
	};
	for_range (i, 0, countof(opt_array)) {
		createOptional(i, opt_array[i], alloc);
	}

	si_printf("Element 1: '%X'\n", opt_i32.data.value);
	si_printf("Element 2: '%S'\n", opt_string.data.value);

	si_print("Element 3: "); print_arrI(opt_buffer.data.value);

	u128Struct num = opt_u128.data.value;
	si_printf("Element 4: '0x%016lX|%016lX'\n", num.high, num.low);

	si_printf("Element 5: '%zd'\n", opt_type.data.value);
	si_printf("Element 6: '%p'\n", opt_ptr.data.value);
}


siOption(cstring) create(b32 value) {
	return value
		? SI_OPT(cstring, "Godzilla")
		: SI_OPT_NIL(cstring);
}

void createOptional(valueType type, rawptr out, siAllocator* alloc) {
	switch (type) {
		case TYPE_I32: {
			siOption(i32)* res = out;
			*res = SI_OPT(i32, INT32_MIN);
		} break;

		case TYPE_STRING: {
			siOption(siString)* res = out;
			*res = SI_OPT(siString, si_stringMake("Ayn Rand", alloc));
		} break;

		case TYPE_ARRAY: {
			siOption(siArrayType(i32))* res = out;
			*res = SI_OPT(siBuffer, si_arrayMake(alloc, i32, 1, 2, 4, 6, 8));
		} break;

		case TYPE_STRUCT: {
			siOption(u128Struct)* res = out;
			*res = SI_OPT(u128Struct, {0xFF, UINT64_MAX});
		} break;

		case TYPE_ENUM: {
			siOption(valueType)* res = out;
			*res = SI_OPT(valueType, TYPE_ENUM);
		} break;

		case TYPE_FUNC_PTR: {
			siOption(rawptr)* res = out;
			*res = SI_OPT(rawptr, (rawptr)createOptional);
		} break;

		default: SI_PANIC();
	}
}


void print_arrI(siArrayType(i32) array) {
	i32* data = (i32*)array.data;
	usize len = 0;

	char buf[SI_KILO(4)];
	buf[0] = '{', len += 1;
	for_range (i, 0, array.len - 1) {
		len += si_snprintf(&buf[len], sizeof(buf) - len, "%i, ", data[i]) - 1;
	}
	si_snprintf(&buf[len], sizeof(buf) - len, "%i}\n", data[array.len - 1]);

	si_print(buf);
}

