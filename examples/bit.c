#define SI_IMPLEMENTATION
#include <sili.h>

usize cpu_archBit(void) {
	#if SI_ARCH_IS_64BIT
		return 64;
	#elif SI_ARCH_IS_32BIT
		return 32;
	#elif
		return 0;
	#endif
}


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
	siAllocator alloc = si_allocatorMakeStack(SI_KILO(1));

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

	u16 adr = 0xFFFE;
	si_printf(
		"0xFFFE (%#b):\n\t"
			"High bits: '%#b', low bits: '%#b'\n\t"
			"MSB: '%#b', LSB: '%#b'\n",
		adr,
		SI_NUM_HIGH_BITS(adr), SI_NUM_LOW_BITS(adr),
		SI_BIT_MSB(adr), SI_BIT_LSB(adr)
	);

	si_printf("Bit 0 of '%#b': '%i'\n", 2, SI_NUM_BIT_GET(2, 0));
	si_printf("'usize' contains '%zd' bits on this CPU architecture.\n", SI_BYTE_TO_BIT(sizeof(usize)));

	usize numBits = si_numCountBitsU32(adr); /* NOTE(EimaMei): On C11 and above, you can just do 'si_numCountBits' and it picks the function for you depending on the number's type. */
	si_printf(
		"Number of 1s in 'adr': '%zd', number of 0s: '%zd'\n",
		numBits, SI_BYTE_TO_BIT(sizeof(adr)) - numBits
	);

	u8 leadTrailNum = 248;
	si_printf(
		 "Leading 1s of '%#b': '%zd', trailing 0s: '%zd'\n",
		 leadTrailNum,
		 si_numLeadingBit(u8, leadTrailNum, SI_BIT_ONE), si_numTrailingBit(u8, leadTrailNum, SI_BIT_ZERO)
	);

	u32 rotateAdr = si_numRotateLeft(u32, 0x00001234, 24);
	si_printf("Rotating '0x00001234' left by 24 bits: '%#08X'\n", rotateAdr);

	rotateAdr = si_numRotateRight(u32, rotateAdr, 24);
	si_printf("Rotating '0x34000012' right by 24 bits: '%#08X'\n", rotateAdr);

	si_printf("Reversing the bits of '0x1234567890123456' gives us: '%#lX'\n", si_numReverseBits(u32, 0x1234567890123456));

	siArray(u8) array = si_numToBytes(&alloc, u32, 0xFF00EEAA);
	si_printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
	for_range (i, 0, si_arrayLen(array)) {
		si_printf("\tElement %zd: '0x%02X'\n", i, array[i]);
	}

	u32 newNum = (u32)si_bytesToNumSiArr(array);
	si_printf("Combining them all back, we get '%#X'\n", newNum);

	adr = si_swap16(adr);
	si_printf("Changing the endian of '0xFFFE' gives us '%#X'\n", adr);

	return 0;
}
