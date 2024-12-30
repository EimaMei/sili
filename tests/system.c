#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <tests/test.h>

int main(void) {
	siString name = SI_STRC("test");
	siString value = SI_STRC("Už žalių miškelių, kur aukšta kalva.");

	b32 res = si_envVarSet(name, value);
	SI_ASSERT(res);

	usize len = si_envVarGetLength(name);
	TEST_EQ_USIZE(len, value.len);

	u8 buf[1024];
	siString out = si_envVarGetData(name, buf, countof(buf));
	SI_ASSERT_NOT_NULL(out.data);
	TEST_EQ_USIZE(out.len, value.len);
	SI_ASSERT(si_stringEqual(out, value));

	res = si_envVarUnset(name);
	SI_ASSERT(res);

	out = si_envVarGetData(name, buf, countof(buf));
	SI_ASSERT(out.data == nil);

#if SI_SYSTEM_IS_WINDOWS
	siWindowsVersion ver = si_windowsGetVersion();
	TEST_EQ_U32(ver, siWindowsVersion_10);

#elif defined(SI_SYSTEM_LINUX)
	res = si_unixIsWayland();
	SI_ASSERT(!res);

	res = si_unixIsX11();
	SI_ASSERT(res);

	siUnixDE de = si_unixGetDE();
	si_printf("DE: %i\n", de);

#endif
	u32 count = si_cpuProcessorCount();
	si_printf("Proc count: %i\n", count);

	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColor3bit_Yellow, true, false));
}

