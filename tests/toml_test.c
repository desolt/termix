#include "minunit.h"
#include "toml.h"
#include <stdlib.h>

void print_err(toml_err err)
{
	if (err)
	{
		char log[128];
		if (toml_get_err_msg(log, 128))
			printf("%s\n", log);
	}
}

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

	char * str1;
	toml_err e1 = parse_string("\"Hello, world!\"", NULL, &str1);
	print_err(e1);

	mu_check(!e1);
	mu_check(!strcmp(str1, "Hello, world!"));

	char * str2 = NULL;
	toml_err e2 = parse_string("\"Hello,\n\n world!\"", NULL, &str2);
	print_err(e2);
	mu_check(e2);
	mu_check(str2 == NULL);

	char * str3;
	toml_err e3 = parse_string("\"\"\"Hello\nmultiline\nworld!\"\"\"", NULL, &str3);
	puts(str3);
	puts("\n");
	print_err(e3);
	mu_check(!e3);
	mu_check(!strcmp("Hello\nmultiline\nworld!", str3));

	char * str4;
	toml_err e4 = parse_string("'C:\\termix\\windows_support?.txt'", NULL, &str4);
	print_err(e4);
	mu_check(!e4);
	mu_check(!strcmp("C:\\termix\\windows_support?.txt", str4));

	free(str1);
	//free(str2);
	free(str3);
	free(str4);
}

MU_TEST(toml_parse_num)
{
	extern toml_err parse_num(const char * src,
	                          const char ** loc,
	                          int * iout,
	                          float * fout,
	                          toml_type * type);

	int i; float f; toml_type t;
	toml_err e;

	e = parse_num("4213", NULL, &i, &f, &t);
	print_err(e);
	mu_check(t == TOML_INT);
	mu_check(i == 4213);

	e = parse_num("12.52", NULL, &i, &f, &t);
	print_err(e);
	mu_check(t == TOML_FLOAT);
	mu_check(f == 12.52f);

	e = parse_num("0x2152", NULL, &i, &f, &t);
	print_err(e);
	mu_check(t == TOML_INT);
	mu_check(i == 0x2152);

	e = parse_num("235fs342", NULL, &i, &f, &t);
	print_err(e);
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
