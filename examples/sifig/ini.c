#define SI_IMPLEMENTATION 1
#define SIFIG_IMPLEMENTATION 1
#include <sili.h>
#include <sifig.h>

siString path = SI_STRC("examples/sifig/res/mod.ini");

int main(void) {
	siDynamicArena arena = si_dynamicArenaMake(si_allocatorHeap(), SI_KILO(1), SI_KILO(2));
	siAllocator alloc = si_allocatorDynamicArena(&arena);

	siIniFile ini = sifig_iniMake(path, alloc);
	siString name; siIniSection section;
	for_eachMapEx (name, section, ini) {
		si_printfLn("[%s] - %i", name, section.len);

		siString key, value;
		for_eachMapEx (key, value, section) {
			si_printfLn("\t\"%s\" = \"%s\"", key, value);
		}
	}

	si_dynamicArenaFree(&arena);
}
