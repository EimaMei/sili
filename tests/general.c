#define SI_IMPLEMENTATION 1
#include <sili.h>

typedef struct {
	usize one;
	char two;
	f32 three;
} randomStruct;

int main(void) {
	{
		SI_ASSERT(SI_KILO(1) == 1024);
		SI_ASSERT(SI_MEGA(1) == 1024 * 1024);
		SI_ASSERT(SI_GIGA(1) == 1024 * 1024 * 1024);
		SI_ASSERT(SI_TERA(1) == 1024 * 1024 * 1024 * 1024ull);

		SI_ASSERT(SI_BIT(63) == 0x8000000000000000);
		SI_ASSERT(nil == (void*)0);

		isize m = si_transmuteEx(isize, USIZE_MAX, usize);
		SI_ASSERT(m == -1);

		u32 value;
		if (SI_LIKELY(SI_HOST_IS_LITTLE_ENDIAN)) {
			value = 0x44434241;
		}
		else if (SI_UNLIKELY(SI_HOST_IS_BIG_ENDIAN)) {
			value = 0x41424344;
		}
		else {
			SI_PANIC();
		}

		cstring str = "ABCD";
		SI_ASSERT(SI_TO_U32(str) == value);

		SI_ASSERT(countof((i32[]){2, 4, 8}) == 3);
		SI_ASSERT(si_offsetof(randomStruct, three) == 12);
		SI_ASSERT(alignof(randomStruct) == 8);

		char* buf1 = si_buf(char, 'Q', 'W', 'E', 'R', 'T', 'Y', '\0');
		SI_ASSERT(si_isNil(buf1[0]) == false && si_isNil(buf1[6]));
		char* buf2 = "AZERTY";

		si_swap(buf1, buf2);
		SI_ASSERT(strcmp(buf1, "AZERTY") == 0 && strcmp(buf2, "QWERTY") == 0);

		i16 x = 0;
		for_range (i, INT16_MIN, 0) {
			x -= 1;
		}
		SI_ASSERT(x == INT16_MIN);

		u64 src = 0x00FF00FF00FF00FF;
		u32 dst;
		memcpy_s(&dst, sizeof(dst), &src, sizeof(src));
		SI_ASSERT(dst == 0x00FF00FF);

		SI_ASSERT(0x44434241 == si_swap32le(value));
		SI_ASSERT(0xFF00FF00FF00FF00 == si_swap64(0x00FF00FF00FF00FF));
		SI_ASSERT(0xFF == si_swap16(0xFF00));

		u16 y[] = {0, UINT16_MAX};
		si_ptrMoveRight(&y[1], 2, 2);
		SI_ASSERT(y[0] == 0xFFFF);

		y[0] = 0x8080;
		y[1] = 0;
		si_ptrMoveLeft(&y[0], 2, 2);
		SI_ASSERT(y[1] == 0x8080);

	}

	{
		usize* ptr1 = si_sallocItem(usize);
		*ptr1 = USIZE_MAX;
		usize* ptr2 = si_sallocCopy(*ptr1);
		SI_ASSERT(*ptr1 == *ptr2 && *ptr1 == USIZE_MAX);

		siAllocator* allocator = si_allocatorMake(SI_KILO(1));
		randomStruct* alloc1 = si_mallocItem(allocator, randomStruct);
		randomStruct* alloc2 = si_mallocArray(allocator, randomStruct, 3);
		*alloc1 = (randomStruct){USIZE_MIN, INT8_MAX, FLOAT32_MIN};

		randomStruct* alloc3 = si_mallocCopy(allocator, *alloc1);
		SI_ASSERT(alloc1->one == alloc3->one && SI_TO_U64(&alloc1->two) == SI_TO_U64(&alloc3->two));

		si_allocatorFree(allocator);
	}

	{
		siAny any = si_anyMake(23);
		SI_ASSERT(any.typeSize == sizeof((i32)23));

		siPoint p1 = SI_POINT(50, 50),
				p2 = (siPoint){28, 28};
		SI_ASSERT(si_pointCmp(p1, p2) == 0);

		siColor c1 = SI_RGBA(128, 128, 128, 255),
				c2 = SI_RGB(255, 0, 0),
				c3 = SI_HEX(0x808080);
		SI_ASSERT(SI_TO_U32(&c1) == SI_TO_U32(&c3) && SI_TO_U32(&c1) != SI_TO_U32(&c2));

		siArea area = SI_AREA(2, 3);
		siRect r1 = SI_RECT(0, 1, 2, 3),
			   r2 = SI_RECT_A(4, 4, area),
			   r3 = SI_RECT_P(p1, 2, 3),
			   r4 = SI_RECT_PA(SI_POINT(0, 1), area);

		siVec2 v2 = SI_VEC2(2, 2);
		si_vec2Sub(&v2, SI_VEC2(-2, -2));
		SI_ASSERT(v2.x == 4 && v2.y == 4);
		si_vec2Add(&v2, SI_VEC2(-2, -2));
		SI_ASSERT(v2.x == 2 && v2.y == 2);
	}
}
