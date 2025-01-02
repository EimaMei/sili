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
	siAllocatorArena aData = si_arenaMakePtr(si_stackAlloc(SI_KILO(1)), SI_DEFAULT_MEMORY_ALIGNMENT);
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

	si_printf("'usize' contains '%zd' bits on this CPU architecture.\n", sizeof(usize) * 8);

	u16 adr = 0xFFFE;
	i32 numBits = si_numCountBitsU32(adr); /* NOTE(EimaMei): On C11 and above, you can just do 'si_numCountBits' and it picks the function for you depending on the number's type. */
	si_printf(
		"Number of 1s in 'adr': '%zi', number of 0s: '%zd'\n",
		numBits, si_sizeof(adr) * 8 - numBits
	);

	u8 leadTrailNum = 248;
	si_printf(
		 "Leading 1s of '%#b': '%zd', trailing 0s: '%zd'\n",
		 leadTrailNum,
		 si_numLeadingOnes(u8, leadTrailNum), si_numTrailingBit(u8, leadTrailNum, SI_BIT_ZERO)
	);

	u32 rotateAdr = si_numRotateLeft(u32, 0x00001234, 24);
	si_printf("Rotating '0x00001234' left by 24 bits: '%#08X'\n", rotateAdr);

	rotateAdr = si_numRotateRight(u32, rotateAdr, 24);
	si_printf("Rotating '0x34000012' right by 24 bits: '%#08X'\n", rotateAdr);

	si_printf("Reversing the bits of '0x1234567890123456' gives us: '%#lX'\n", si_numReverseBits(u32, 0x1234567890123456));

	u8 buf[64];
	siArray(u8) array = si_numToBytes(u32, 0xFF00EEAA, alloc);
	si_printf("array: %S, (len: %zd)\n", si_stringFromArray(array, "%#hhX", buf, sizeof(buf)), array.len);

	u32 newNum = (u32)si_numFromBytes(array);
	si_printf("Combining them all back, we get '%#X'\n", newNum);

	adr = si_swap16(adr);
	si_printf("Changing the endian of '0xFFFE' gives us '%#X'\n", adr);

	return 0;
}
