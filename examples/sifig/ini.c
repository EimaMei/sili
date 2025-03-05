#define SI_IMPLEMENTATION 1
#define SIFIG_IMPLEMENTATION 1
#include <sili.h>
#include <sifig.h>


int main(int argc, char* argv[]) {
    SI_ASSERT(argc > 1);

	siDynamicArena arena = si_dynamicArenaMake(si_allocatorHeap(), SI_KILO(1), SI_KILO(2));
	siAllocator alloc = si_allocatorDynamicArena(&arena);

	siIniFile ini = sifig_iniMake(SI_CSTR(argv[1]), alloc);

	siString name; siIniSection section;
	for_eachMapEx (name, section, ini) {
		si_printfLn("[%S] - %i", name, section.len);

		siString key, value;
		for_eachMapEx (key, value, section) {
			si_printfLn("\t\"%S\" = \"%S\"", key, value);
		}
	}


	si_dynamicArenaFree(&arena);
}
