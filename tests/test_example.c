#include "minunit.h"

// For a full example see https://github.com/siu/minunit/blob/master/minunit_example.c

MU_TEST(test_something)
{
	mu_assert_int_eq(8, 8);
	mu_assert_string_eq("C++ is good", "C++ is good");
	mu_assert_double_eq(1.0f, 1.0000000001f);
}

MU_TEST(test_something_else)
{
	mu_check(7 == 7);
	mu_assert(1 + 2 + 3 == 3 + 2 + 1, "my maths teacher lied");
	// You can use mu_fail(message) if you want to unconditionally fail, e.g:
	//mu_fail("some error message");
}

MU_TEST_SUITE(suite)
{
	MU_RUN_TEST(test_something);
	MU_RUN_TEST(test_something_else);
}

int main(void)
{
	MU_RUN_SUITE(suite);
	MU_REPORT(); //  This returns the appropriate error code
}
