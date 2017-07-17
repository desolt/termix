#include "minunit.h"
#include "err.h"
#include "toml.h"
#include <stdlib.h>

MU_TEST(toml_table_creation)
{
	toml_table * root = toml_init(4);
	toml_create_table("hello", 4, root);

	mu_check(toml_table_has_child(root, "hello"));
	mu_check(!toml_table_has_child(root, "goodbye"));

	toml_free(root);
}

MU_TEST(toml_parse_string)
{
	extern toml_err parse_string(const char * src, char ** loc, char ** out);

	toml_err e;
	char * str;

	e = parse_string("\"Hello, world!\"", NULL, &str);
	mu_check(!e);
	mu_check(!strcmp(str, "Hello, world!"));
	free(str);

	str = NULL;
	e = parse_string("\"Hello,\n\n world!\"", NULL, &str);
	mu_check(e);
	mu_check(str == NULL);

	e = parse_string("\"\"\"Hello\nmultiline\nworld!\"\"\"", NULL, &str);
	mu_check(!e);
	mu_check(!strcmp("Hello\nmultiline\nworld!", str));
	free(str);

	e = parse_string("'C:\\termix\\windows_support?.txt'", NULL, &str);
	mu_check(!e);
	mu_check(!strcmp("C:\\termix\\windows_support?.txt", str));
	free(str);
}

MU_TEST(toml_parse_num)
{
	extern toml_err parse_num(const char * src, const char ** loc, toml_value ** val);

	toml_value * v;
	toml_err e;

	e = parse_num("213634", NULL, &v);
	mu_assert(!e, "parsing error!");
	mu_check(v->type == TOML_INT);
	mu_assert_int_eq(v->val.integer, 213634);
	free(v);

	e = parse_num("0x4215ff", NULL, &v);
	mu_assert(!e, "parsing error!");
	mu_check(v->type == TOML_INT);
	mu_assert_int_eq(v->val.integer, 0x4215ff);
	free(v);

	e = parse_num(".4e4", NULL, &v);
	mu_assert(!e, "parsing error!");
	mu_check(v->type == TOML_FLOAT);
	mu_assert_double_eq(v->val.floating, .4e4);
	free(v);

	e = parse_num("523.32", NULL, &v);
	mu_assert(!e, "parsing error!");
	mu_check(v->type == TOML_FLOAT);
	mu_assert_double_eq(v->val.floating, 523.32);
	free(v);

	e = parse_num("324helloworld!43", NULL, &v);
	mu_check(e);

	e = parse_num("123x456", NULL, &v);
	mu_check(e);
}

MU_TEST_SUITE(toml)
{
	MU_RUN_TEST(toml_table_creation);
	MU_RUN_TEST(toml_parse_string);
	MU_RUN_TEST(toml_parse_num);
}

int main()
{
	MU_RUN_SUITE(toml);
	MU_REPORT();
}
