#define SI_IMPLEMENTATION
#include <sili.h>

inline cstring operatingSystem(void) {
	static char res[] =
		#if defined(SI_SYSTEM_WINDOWS)
			"Windows"
		#elif defined(SI_SYSTEM_OSX)
			"MacOS"
		#elif defined(SI_SYSTEM_LINUX)
			"Linux"
		#elif defined(SI_SYSTEM_ANDROID)
			"Android"
		#elif defined(SI_SYSTEM_IOS)
			"iOS"
		#elif defined(SI_SYSTEM_WASM)
			"WebAssembly"
		#endif
	;

	return res;
}


inline cstring cpuArch(void) {
	static char res[] =
		#if defined(SI_CPU_X86)
			"x86"
		#elif defined(SI_CPU_PPC)
			"PPC"
		#elif defined(SI_CPU_ARM64)
			"ARM64"
		#elif defined(SI_CPU_ARM)
			"ARM"
		#elif defined(SI_CPU_MIPS)
			"MIPS"
		#elif defined (SI_CPU_SPARC)
			"SPARC"
		#elif defined(SI_CPU_RISC_V)
			"RISC-V"
		#elif defined(SI_CPU_ALPHA)
			"Alpha"
		#else
			"N/A"
		#endif
	;

	return res;
}

inline usize cpu_arch_bit(void) {
	#if defined(SI_ARCH_64_BIT)
		return 64;
	#elif defined(SI_ARCH_32_BIT)
		return 32;
	#endif
}

inline cstring cpuEndian(void) {
	return (SI_HOST_IS_LITTLE_ENDIAN == true) ? "little-endian" : "big-endian";
}

inline cstring compiler(void) {
	static char res[] =
		#if defined(SI_COMPILER_GCC)
			"GCC"
		#elif defined(SI_COMPILER_CLANG)
			"clang"
		#elif defined(SI_COMPILER_MSVC)
			"msvc"
		#else
			"N/A"
		#endif
	;

	return res;
}

inline cstring language(void) {
	static char res[] =
		#if defined(SI_LANGUAGE_C)
			"C"
		#elif defined(SI_LANGUAGE_CPP)
			"C++"
		#elif defined(SI_LANGUAGE_OBJ_CPP)
			"Objective-C++"
		#elif defined(SI_LANGUAGE_OBJ_C) /* NOTE(EimaMei): SI_LANGUAGE_OBJ_C is defined for both Objective-C and Objective-C++. */
			"Objective-C"
		#elif defined(SI_LANGUAGE_CPLUS)
			"C-plus"
		#endif
	;
	return res;
}

inline cstring standard(void) {
	static char res[] =
		#if !defined(SI_LANGUAGE_CPP)
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
		#elif defined(SI_LANGUAGE_CPP)
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


int main(void) {
	siAllocator* alloc = si_allocatorMakeStack(SI_KILO(1));
	SI_STATIC_ASSERT(SI_BIT(8) == 256);

	printf(
		"Information about the system:\n\t"
			"Operating System - '%s'\n\t"
			"CPU Architecture - '%s' (%zd-bit)\n\t"
			"Target endian - '%s'\n\t"
			"CPU cache line size - '%i'\n"
		"Compilation info:\n\t"
			"Compiler - '%s'\n\t"
			"Language - '%s' (%s)\n\n"
		,
		operatingSystem(),
		cpuArch(), cpu_arch_bit(),
		cpuEndian(), SI_CACHE_LINE_SIZE,
		compiler(), language(), standard()
	);

	u16 adr = 0xFFFE; /* The binary expression of 0xFFFE is '0b1111111111111110' */
	printf("High bytes: '%2X', low bytes: '%2X'\n", SI_NUM_HIGH_BITS(adr), SI_NUM_LOW_BITS(adr));
	printf("MSB: '%i', LSB: '%i'\n", SI_BIT_MSB(adr), SI_BIT_LSB(adr));

	printf("Bit 0 of 0b10: '%i'\n", SI_NUM_BIT_GET(2, 0));
	printf("'usize' contains '%zd' bits on this CPU architecture.\n", SI_BYTE_TO_BIT(sizeof(usize)));

	usize num_bits = si_numCountBitsU32(adr); /* NOTE(EimaMei): On C11 and above, you can just do 'si_numCountBits' and it picks the function for you depending on the number's type. */
	printf(
		"Number of 1s in 'adr': '%zd', number of 0s: '%zd'\n",
		num_bits, SI_BYTE_TO_BIT(sizeof(adr)) - num_bits
	);

	/* The binary expression of 248 is '0b11111000'. */
	u8 leadTrailNum = 248;
	printf(
		 "Leading 1s of '248': '%zd', trailing 0s: '%zd'\n",
		 si_numLeadingBit(leadTrailNum, SI_BIT_ONE), si_numTrailingBit(leadTrailNum, SI_BIT_ZERO)
	);

	u32 rotateAdr = si_numRotateLeft((u32)0x00001234, 24);
	printf("Rotating '0x00001234' left by 24 bits: '0x%08X'\n", rotateAdr);

	rotateAdr = si_numRotateRight(rotateAdr, 24);
	printf("Rotating '0x34000012' right by 24 bits: '0x%08X'\n", rotateAdr);

	printf("Reversing the bits of '0x1234567890123456' gives us: '0x%lX'\n", si_numReverseBits(0x1234567890123456));

	siArray(u8) array = si_numToBytes(alloc, (u32)0xFF00EEAA);
	printf("All of the elements in 'array' (len - '%zd'):\n", si_arrayLen(array));
	for_range (i, 0, si_arrayLen(array)) {
		printf("\tElement %zd: '0x%02X'\n", i, array[i]);
	}

	u32 newNum = si_bytesToNumSiArr(array);
	printf("Combining them all back, we get '0x%X'\n", newNum);

	adr = si_swap16(adr);
	printf("Changing the endian of '0xFFFE' gives us '0x%X'\n", adr);

	return 0;
}
