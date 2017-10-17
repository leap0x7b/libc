/*
* Copyright © 2017 Embedded Artistry LLC.
* License: MIT. See LICENSE file for details.
*/

#include <string.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka/cmocka.h>
// clang-format on

#pragma mark - Definitions -

#define N 80

#pragma mark - Declarations -

static char buf[512];

#pragma mark - Private Functions -

static void* aligned(void* p)
{
	return (void*)(((uintptr_t)p + 63) & -64);
}

static void test_align(int dalign, int salign, int len)
{
	char* src = aligned(buf);
	char* dst = aligned(buf + 128);
	char* want = aligned(buf + 256);
	char* p;
	int i;

	assert_false(salign + len > N);
	assert_false(dalign + len > N);

	for(i = 0; i < N; i++)
	{
		src[i] = '#';
		dst[i] = want[i] = ' ';
	}

	for(i = 0; i < len; i++)
	{
		src[salign + i] = want[dalign + i] = '0' + i;
	}

	p = memcpy(dst + dalign, src + salign, len);

	assert_ptr_equal(p, dst + dalign);

	for(i = 0; i < N; i++)
	{
		assert_int_equal(dst[i], want[i]);
	}
}

static void memcpy_aligned_test(void** state)
{
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			for(int k = 0; k < 64; k++)
			{
				test_align(i, j, k);
			}
		}
	}
}

#pragma mark - Public Functions -

int memcpy_tests(void)
{
	const struct CMUnitTest memcpy_tests[] = {cmocka_unit_test(memcpy_aligned_test)};

	return cmocka_run_group_tests(memcpy_tests, NULL, NULL);
}
