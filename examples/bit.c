#define SI_IMPLEMENTATION
#include <sili.h>

force_inline
usize cpu_archBit(void) {
	#if SI_ARCH_IS_64BIT
		return 64;
	#elif SI_ARCH_IS_32BIT
		return 32;
	#elif
		return 0;
	#endif
}


force_inline
cstring standard(void) {
	static char res[] =
		#if SI_LANGUAGE_IS_C
			#if SI_STANDARD_VERSION == SI_STANDARD_C89
				"C89"
			#elif SI_STANDARD_VERSION == SI_STANDARD_C94
				"C94"
			#elif SI_STANDARD_VERSION == SI_STANDARD_C99
				"C99"
			#elif SI_STANDARD_VERSION == SI_STANDARD_C11
				"C11"
			#elif SI_STANDARD_VERSION == SI_STANDARD_C17
				"C17"
			#elif SI_STANDARD_VERSION > SI_STANDARD_C17
				"C2x"
			#endif
		#elif SI_LANGUAGE_IS_CPP
			#if SI_STANDARD_VERSION == SI_STANDARD_CPP98
				"C++98"
			#elif SI_STANDARD_VERSION == SI_STANDARD_CPP11
				"C++11"
			#elif SI_STANDARD_VERSION == SI_STANDARD_CPP14
				"C++14"
			#elif SI_STANDARD_VERSION == SI_STANDARD_CPP17
				"C++17"
			#elif SI_STANDARD_VERSION == SI_STANDARD_C20
				"C++20"
			#elif SI_STANDARD_VERSION == SI_STANDARD_C23
				"C++23"
			#endif
		#endif
	;

	return res;
}


SI_STATIC_ASSERT(SI_BIT(8) == 256);

int main(void) {
	siArena aData = si_arenaMakePtr(si_stackAlloc(SI_KILO(1)), SI_DEFAULT_MEMORY_ALIGNMENT);
	siAllocator alloc = si_allocatorArena(&aData);

	si_printf(
		"Information about the system:\n\t"
			"Operating System - '%s'\n\t"
			"CPU Architecture - '%s' (%zd-bit)\n\t"
			"Target endian - '%s'\n"
		"Compilation info:\n\t"
			"Compiler - '%s'\n\t"
			"Language - '%s' (%s)\n\n"
		,
		SI_SYSTEM_STR,
		SI_ARCH_STR, cpu_archBit(),
		SI_ENDIAN_STR, SI_COMPILER_STR, SI_LANGUAGE_STR, standard()
	);

	si_printfLn("'usize' contains '%zi' bits on this CPU architecture.", si_sizeof(usize) * 8);

	u16 adr = 0xFFFE;
	si_printfLn(
		"Number of 1s in 'adr': '%i', number of 0s: '%d'",
		si_countOnes(u32, adr), si_countZeros(u32, adr)
	);

	u8 leadTrailNum = 248;
	si_printfLn(
		 "Leading 1s of '%#b': '%i', trailing 0s: '%i'",
		 leadTrailNum,
		 si_countLeadingOnes(u8, leadTrailNum), si_countTrailingZeros(u8, leadTrailNum)
	);

	u32 rotateAdr = si_bitsRotateLeft(u32, 0x00001234, 24);
	si_printfLn("Rotating '0x00001234' left by 24 bits: '%#08X'", rotateAdr);

	rotateAdr = si_bitsRotateRight(u32, rotateAdr, 24);
	si_printfLn("Rotating '0x34000012' right by 24 bits: '%#08X'", rotateAdr);

	si_printfLn("Reversing the bits of '0x1234567890123456' gives us: '%#lX'", si_bitsReverseBits(u32, 0x1234567890123456));

	siBuffer(u8) buffer = si_bytesToArray(u32, 0xFF00EEAA, alloc);
	si_printfLn("buffer: %S, (len: %zd)", si_stringFromBuffer(buffer, "%#hhX", SI_BUF_STACK(64)), buffer.len);

	u32 newNum = (u32)si_bytesFromArray(buffer);
	si_printfLn("Combining them all back, we get '%#X'", newNum);

	adr = si_swap16(adr);
	si_printfLn("Changing the endian of '0xFFFE' gives us '%#X'", adr);

	return 0;
}
