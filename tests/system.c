#define SI_IMPLEMENTATION 1
#include <sili.h>
#include <tests/test.h>



int main(void) {
	TEST_START();

	siString name = SI_STRC("test");
	siString value = SI_STRC("Už žalių miškelių, kur aukšta kalva.");

	bool res = si_envVarSet(name, value);
	TEST_EQ_TRUE(res);

	isize len = si_envVarGetLength(name);
	TEST_EQ_USIZE(len, value.len);

	siString out = si_envVarGetData(name, SI_ARR_STACK(1024));
	TEST_NEQ_NIL(out.data);
	TEST_EQ_USIZE(out.len, value.len);
	TEST_EQ_TRUE(si_stringEqual(out, value));

	res = si_envVarUnset(name);
	TEST_EQ_TRUE(res);

	out = si_envVarGetData(name, SI_ARR_STACK(1024));
	TEST_EQ_NIL(out.data);

#if SI_SYSTEM_IS_WINDOWS
	siWindowsVersion ver = si_windowsGetVersion();
	TEST_EQ_U32(ver, siWindowsVersion_10);

#elif defined(SI_SYSTEM_LINUX)
	res = si_unixIsWayland();
	TEST_EQ_FALSE(res);

	res = si_unixIsX11();
	TEST_EQ_TRUE(res);

	siUnixDE de = si_unixGetDE();
	si_printf("DE: %i\n", de);

#endif
	i32 count = si_cpuProcessorCount();
	TEST_NEQ_U64(count, 0);

	TEST_COMPLETE();
}

