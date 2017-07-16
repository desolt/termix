#include "minunit.h"
#include "toml.h"

MU_TEST(toml_table_creation)
{
	toml_table * root = toml_init();
	toml_table * child = toml_create_table("hello", root);

	mu_check(toml_table_has_child(root, "hello"));
	mu_check(!toml_table_has_child(root, "goodbye"));
}

MU_TEST_SUITE(toml)
{
	MU_RUN_TEST(toml_table_creation);
}

int main()
{
	MU_RUN_SUITE(toml);
	MU_REPORT();
}
