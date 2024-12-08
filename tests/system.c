#define SI_IMPLEMENTATION 1
#include <sili.h>


int main(void) {
	siString name = SI_STRC("test");
	siString value = SI_STRC("Už žalių miškelių, kur aukšta kalva.");

	b32 res = si_envVarSet(name, value);
	SI_ASSERT(res == true);

	u8 buf[1024];
	siString out = si_envVarGet(name, buf, countof(buf));
	SI_ASSERT(out.data != nil);
	SI_ASSERT(si_stringEqual(out, value));

	res = si_envVarUnset(name);
	SI_ASSERT(res == true);

	out = si_envVarGet(name, buf, countof(buf));
	SI_ASSERT(out.data == nil);

#if SI_SYSTEM_IS_WINDOWS
	siWindowsVersion ver = si_windowsGetVersion();
	SI_ASSERT_FMT(ver == siWindowsVersion_10, "%i", ver);

#elif defined(SI_SYSTEM_LINUX)
	res = si_unixIsWayland();
	SI_ASSERT(res == false);

	res = si_unixIsX11();
	SI_ASSERT(res == true);

	siUnixDE de = si_unixGetDE();
	si_printf("DE: %i\n", de);

#endif
	u32 count = si_cpuProcessorCount();
	si_printf("Proc count: %i\n", count);

	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColor3bit_Yellow, true, false));
}

