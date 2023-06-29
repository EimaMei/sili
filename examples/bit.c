#define SI_IMPLEMENTATION
#include <sili.h>


cstring cpu_arch(void) {
    static char res[] = (
        #if defined(SI_CPU_X86)
            "X86"
        #elif defined(SI_CPU_PPC)
            "PPC"
        #elif defined(SI_CPU_ARM)
            "ARM"
        #elif defined(SI_CPU_MIPS)
            "MIPS"
        #else
            "N/A"
        #endif
    );

    return res;
}

usize cpu_type(void) {
    #if defined(SI_ARCH_64_BIT)
        return 64;
    #elif
        return 32;
    #endif
}

cstring cpu_endian(void) {
    return (SI_LITTLE_ENDIAN == true) ? "little-endian" : "big-endian";
}



int main(void) {
    si_init(SI_KILO(1));

    SI_ASSERT(SI_BIT(8) == 256);

    printf(
        SI_MULTILINE(
            "Information about the system:\n\t"
                 "CPU Architecture - '%s' (%zd-bit)\n\t"
                 "Target endian - '%s'\n\t"
                 "CPU cache line size - '%i'\n\n"
        ),
        cpu_arch(), cpu_type(),
        cpu_endian(),
        SI_CACHE_LINE_SIZE
    );

    u16 adr = 0xFFFE; /* The binary expression of 0xFFFE is '0b1111111111111110' */
    printf("High bit: '%2X', low bit: '%2X'\n", SI_HIGH_BIT(adr), SI_LOW_BIT(adr));

    printf("Bit 0: '%i'\n", SI_NUM_GET_BIT(BIT_00000001, 0));
    printf("usize contains '%zd' bits  on this CPU architecture.", SI_BYTE_TO_BIT(sizeof(usize)));

    printf(
        "Number of 1s in 'adr': '%zd', number of 0s: '%zd'\n",
        si_num_count_bit(adr, SI_BIT_ONE), si_num_count_bit(adr, SI_BIT_ZERO)
    );

    /* The binary expression of 248 is '0b11111000'. */
    printf(
         "Leading 1s of '248': '%zd', trailing 0s: '%zd'\n",
         si_num_leading_bit((u8)248, SI_BIT_ONE), si_num_trailing_bit((u8)240, SI_BIT_ZERO)
    );

    u32 rotate_adr = si_num_rotate_left((u32)0x00001234, 24);
    printf("Rotating '0x00001234' left by 24 bits: '0x%08X'\n", rotate_adr);

    rotate_adr = si_num_rotate_right(rotate_adr, 24);
    printf("Rotating '0x34000012' right by 24 bits: '0x%08X'\n", rotate_adr);

    printf("Reversing the bits of '0x1234567890123456' gives us: '0x%lX'\n", si_num_reverse_bits(0x1234567890123456));

    siArray(u8) array = si_num_to_bytes((u32)0xFF00EEAA);
    printf("All of the elements in 'array' (len - '%zd'):\n", si_array_len(array));
    for_range (i, 0, si_array_len(array)) {
		printf("\tElement %zd: '0x%02X'\n", i, array[i]);
	}

    u32 new_num = si_bytes_to_num(array);
    printf("Combining them all back, we get '0x%X'\n", new_num);

    adr = si_num_change_endian(adr);
    printf("Changing the endian of '0xFFFE' gives us '0x%X'\n", adr);

    si_terminate();
    return 0;
}
