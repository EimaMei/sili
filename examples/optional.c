#define SI_IMPLEMENTATION 1
#include <sili.h>


siOptional(cstring) create(bool value) {
    return (value ? si_optional_make("Godzilla") : SI_OPTIONAL_NULL);
}


int main(void) {
    /* Example 5.0: siOptional (based on https://en.cppreference.com/w/cpp/utility/optional). */
    printf("==============\n\n==============\nExample 5.0:\n");

    printf("create(false) returned '%s'\n", si_optional_get_or_default(cstring, create(false), "empty"));

    siOptional(cstring) str = create(true);
    printf("create2(true) returned '%s'\n", si_optional_get(cstring, str));

    si_optional_reset(str);
    printf("str.has_value: %zd\n", str.has_value);

    return 0;
}
