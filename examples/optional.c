#define SI_ALLOCATOR_UNDEFINE
#define SI_IMPLEMENTATION 1
#include <sili.h>


/*
 * IMPORTANT NOTE(EimaMei):
 * 'si_optional_make' and 'si_optional_make_static' are very different in use case!
 *
 * 'si_optional_make' copies the data into the allocator, while 'si_optional_make_static'
 * only gets the pointer address of the given value.
 *
 * All this means is that if you're planning to return a 'siOptional', then you
 * __have__ to use `si_optional_make` so that the value doesn't get deleted after
 * the function returns.
 *
 * 'si_optional_make_static' should usually only be used when there's a 'siOptionalType'
 * function argument input, OR when the return data is static.
*/

siOptional(cstring) create(bool value) {
    return (value ? si_optional_make_static(&"Godzilla") : SI_OPTIONAL_NULL);
}

typedef SI_ENUM(usize, siType) {
    SI_TYPE_NULL,
    SI_TYPE_I32,
    SI_TYPE_STRING,
    SI_TYPE_ARRAY,
    SI_TYPE_STRUCT,
    SI_TYPE_ENUM,
    SI_TYPE_FUNC_PTR
};

typedef struct u128_struct {
    u64 high, low;
} u128_struct;

siOptionalType create_anything(siType type) {
    switch (type) {
        case SI_TYPE_NULL: return SI_OPTIONAL_NULL;
        case SI_TYPE_I32: return si_optional_make((i32)INT32_MIN);
        case SI_TYPE_STRING: return si_optional_make(si_string_make("Ayn Rand"));
        case SI_TYPE_ARRAY: return si_optional_make(si_array_make((i32[]){1, 2, 3}));
        case SI_TYPE_STRUCT: return si_optional_make((u128_struct){0xFF, UINT64_MAX});
        case SI_TYPE_ENUM: return si_optional_make(siFebruary);
        case SI_TYPE_FUNC_PTR: return si_optional_make(&create_anything);
    }
    SI_BUILTIN_UNREACHABLE(); /* NOTE(EimaMei): If possible, lets the compiler know that this part of the code won't be reached. */
}

void example_5_0(void) {
    /* Example 5.0: siOptional (based on https://en.cppreference.com/w/cpp/utility/optional). */
    printf("==============\n\n==============\nExample 5.0:\n");

    printf("create(false) returned '%s'\n", si_optional_get_or_default(cstring, create(false), "empty"));

    siOptional(cstring) str = create(true);
    printf("create2(true) returned '%s'\n", si_optional_get(cstring, str));

    si_optional_reset(str);
    printf("str.has_value: %zd\n", str.has_value);
}


void example_5_1(void) {
    /* Example 5.0: siOptional with other types. */
    printf("==============\n\n==============\nExample 5.1:\n");

    siOptionalType results[7]; /* 7 because that's how many types we have in the enum.*/

    for_range (i, 0, countof(results)) {
        results[i] = create_anything(i);
    }

    printf("Element 0: '%s'\n", si_optional_get_or_default(cstring, results[0], "Couldn't get the value, the optional variable is null."));
    printf("Element 1: '%i'\n", si_optional_get(i32, results[1]));
    printf("Element 2: '%s'\n", si_any_get(siString, results[2].value));

    siArray(i32) arr = si_optional_get(typeof(arr), results[3]);
    printf("Element 3: '{");
        foreach(num, arr) {
            printf("%i", *num);

            if (*num != (i32)SI_ARRAY_HEADER(arr)->len) {
                printf(", ");
            }
        }
    printf("}' (len: '%zd')\n", si_array_len(arr));

    u128_struct num = si_optional_get(u128_struct, results[4]);
    printf("Element 4: '0x%zX%zX' (type_size: '%zd')\n", num.high, num.low, si_optional_type_size(results[4]));

    printf("Element 5: '%zd'\n", si_optional_get(siMonth, results[5]));
    printf("Element 6: '%p'\n", si_optional_get(rawptr, results[6]));
}


int main(void) {
    si_init(SI_KILO(1));

    example_5_0();
    example_5_1();

    si_terminate();
    return 0;
}
