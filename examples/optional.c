#define SI_IMPLEMENTATION 1
#include <sili.h>


SI_ENUM(usize, siType) {
	SI_TYPE_NULL = 0,
	SI_TYPE_I32,
	SI_TYPE_STRING,
	SI_TYPE_ARRAY,
	SI_TYPE_STRUCT,
	SI_TYPE_ENUM,
	SI_TYPE_FUNC_PTR,

	SI_TYPE_COUNT
};

typedef struct u128Struct {
	u64 high, low;
} u128Struct;



/* Example 1 shows off the very basics of 'siOptional' and how it can be usually
 * used. */
void example1(void);
/* Example 2 shows 'siOptional' type's compatibility with a wide variety of C
 * types. */
void example2(siAllocator* stack);


int main(void) {
	siAllocator * stack = si_allocatorMakeStack(0xFF);

	example1();
	example2(stack);

	return 0;
}



siOptionalRet(cstring) create(b32 value);
siOptionalRet(ANY) createOptional(siAllocator* alloc, siType type);

void print_arrI(siArray(void) arr);


void example1(void) {
	/* Based on https://en.cppreference.com/w/cpp/utility/optional. */
	si_printf("==============\n\n==============\nExample 5.0:\n");

	siOptional(cstring) str = create(false);
	si_printf("create(false) returned '%s'\n", si_optionalGetOrDefault(str, "empty"));

	str = create(true);
	si_printf("create2(true) returned '%s'\n", str->value);

	si_optionalReset(str);
	si_printf("str.has_value: %B\n", str->hasValue);
}

void example2(siAllocator* alloc) {
	si_printf("==============\n\n==============\nExample 5.1:\n");

	siOptionalRet(ANY) results[SI_TYPE_COUNT];

	for_range (i, 0, countof(results)) {
		results[i] = createOptional(alloc, i);
	}

	si_printf("Element 0: '%s'\n", si_optionalGetOrDefault((siOptional(cstring))results[0],
				"Couldn't get the value, the optional variable is null."));
	si_printf("Element 1: '%X'\n", ((siOptional(i32))results[1])->value);
	si_printf("Element 2: '%s'\n", ((siOptional(siString))results[2])->value);

	siArray(i32) arr = ((siOptional(siArray(i32)))results[3])->value;
	si_print("Element 3: "); print_arrI(arr);

	u128Struct num = ((siOptional(u128Struct))results[4])->value;
	si_printf("Element 4: '0x%016lX|%016lX'\n", num.high, num.low);

	si_printf("Element 5: '%zd'\n", ((siOptional(siType))results[5])->value);
	si_printf("Element 6: '%p'\n", ((siOptional(rawptr))results[6])->value);
}


siOptionalRet(cstring) create(b32 value) {
	return (value ? si_optionalMakeEx(cstring, "Godzilla") : SI_OPTIONAL_NULL);
}

siOptionalRet(ANY) createOptional(siAllocator* alloc, siType type) {
	siOptionalRet(ANY) res = NULL;

	switch (type) {
		case SI_TYPE_NULL:
			res = SI_OPTIONAL_NULL;
			break;
		case SI_TYPE_I32:
			res = si_optionalMake(INT32_MIN);
			break;
		case SI_TYPE_STRING:
			res = si_optionalMake(si_stringMake(alloc, "Ayn Rand"));
			break;
		case SI_TYPE_ARRAY:
			res = si_optionalMake(si_arrayMake(alloc, si_buf(i32, 1, 2, 4, 6, 8)));
			break;
		case SI_TYPE_STRUCT:
			res = si_optionalMake((u128Struct){0xFF, UINT64_MAX});
			break;
		case SI_TYPE_ENUM:
			res = si_optionalMake((siType)SI_TYPE_ENUM);
			break;
		case SI_TYPE_FUNC_PTR:
			res = si_optionalMake(&createOptional);
			break;
		default: SI_PANIC();
	}

	return res;
}


void print_arrI(siArray(void) arr) {
	usize len = 0;
	char buf[SI_KILO(4)];

	buf[0] = '{', len += 1;
	for_range (i, 0, si_arrayLen(arr) - 1) {
		len += si_snprintf(&buf[len], sizeof(buf) - len, "%i, ", ((i32*)arr)[i]) - 1;
	}
	si_snprintf(&buf[len], sizeof(buf) - len, "%i}\n", ((i32*)arr)[si_arrayLen(arr)]);

	si_printf(buf);
}

