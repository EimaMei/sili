#define SI_IMPLEMENTATION 1
#include <sili.h>


siOptional(char*) create(bool value) {
	return (value ? si_optional_make((char*)"Godzilla") : SI_OPTIONAL_NULL);
}


int main(void) {
	/* Example 5.0: siOptional (based on https://en.cppreference.com/w/cpp/utility/optional). */
	printf("==============\n\n==============\nExample 5.0:\n");

	printf("create(false) returned '%s'\n", si_optional_get_or_default(char*, create(false), "empty"));

	siOptional(char*) str = create(true);
	printf("create2(true) returned '%s'\n", si_any_get(str.value, char*));

	return 0;
}
