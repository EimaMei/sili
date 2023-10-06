#define SI_IMPLEMENTATION 1
#include <sili.h>


siOptionalRet(cstring) create(b32 value) {
  return (value ? si_optionalMake(&"Godzilla") : SI_OPTIONAL_NULL(cstring));
}

typedef SI_ENUM(usize, siType) {
	SI_TYPE_NULL,
	SI_TYPE_I32,
	SI_TYPE_STRING,
	SI_TYPE_ARRAY,
	SI_TYPE_STRUCT,
	SI_TYPE_ENUM,
	SI_TYPE_FUNC_PTR,

	SI_TYPE_COUNT
};

typedef struct {
	u64 high, low;
} u128Struct;


siOptionalRet(ANY) create_anything(siAllocator* alloc, siType type) {
	siOptionalRet(ANY) res;

	switch (type) {
		case SI_TYPE_NULL:
			res = SI_OPTIONAL_NULL(rawptr);
			break;
		case SI_TYPE_I32:
			res = si_optionalMake(INT32_MIN);
			break;
		case SI_TYPE_STRING:
			res = si_optionalMake(si_stringMake(alloc, "Ayn Rand"));
			break;
		case SI_TYPE_ARRAY:
			res = si_optionalMake(si_arrayMake(alloc, (i32[]){1, 2, 3}));
			break;
		case SI_TYPE_STRUCT:
			res = si_optionalMake((u128Struct){0xFF, UINT64_MAX});
			break;
		case SI_TYPE_ENUM:
			res = si_optionalMake((siMonth)siFebruary);
			break;
		case SI_TYPE_FUNC_PTR:
			res = si_optionalMake(&create_anything);
			break;
	}

	return res;
}

void example1(void) {
	/* Example 5.0: siOptional (based on https://en.cppreference.com/w/cpp/utility/optional). */
	printf("==============\n\n==============\nExample 5.0:\n");

	siOptional(cstring) str = create(false);
	printf("create(false) returned '%s'\n", si_optionalGetOrDefault(str, "empty"));

	str = create(true);
	printf("create2(true) returned '%s'\n", str->value);

	si_optionalReset(str);
	printf("str.has_value: %u\n", str->hasValue);
}


void example2(siAllocator* alloc) {
	/* Example 5.0: siOptional with other types. */
	printf("==============\n\n==============\nExample 5.1:\n");

	siOptionalRet(ANY) results[SI_TYPE_COUNT];

	for_range (i, 0, countof(results)) {
		results[i] = create_anything(alloc, i);
	}

	printf("Element 0: '%s'\n", si_optionalGetOrDefault((siOptional(cstring))results[0],
				"Couldn't get the value, the optional variable is null."));
	printf("Element 1: '%i'\n", ((siOptional(i32))results[1])->value);
	printf("Element 2: '%s'\n", ((siOptional(siString))results[2])->value);

	siArray(i32) arr = ((siOptional(siArray(i32)))results[3])->value;
	printf("Element 3: '{");
		for_each (num, arr) {
			printf("%i", *num);

			if (*num != (i32)SI_ARRAY_HEADER(arr)->len) {
				printf(", ");
			}
		}
	printf("}' (len: '%zd')\n", si_arrayLen(arr));

	u128Struct num = ((siOptional(u128Struct))results[4])->value;
	printf("Element 4: '0x%016zX|%016zX'\n", num.high, num.low);

	printf("Element 5: '%zd'\n", ((siOptional(siMonth))results[5])->value);
	printf("Element 6: '%p'\n", ((siOptional(rawptr))results[6])->value);
}


int main(void) {
	siAllocator * stack = si_allocatorMakeStack(0xFF);

	example1();
	example2(stack);

	return 0;
}
