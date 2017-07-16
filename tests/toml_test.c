#include "minunit.h"
#include "toml.h"

MU_TEST(toml_table_creation)
{
	toml_table * root = toml_init(4);
	toml_create_table("hello", 4, root);

	mu_check(toml_table_has_child(root, "hello"));
	mu_check(!toml_table_has_child(root, "goodbye"));

	toml_free(root);
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
