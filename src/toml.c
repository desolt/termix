#include "toml.h"

typedef struct toml_pair toml_pair;
typedef struct toml_node toml_node;

struct toml_table
{
	char * name; // NULL if root.
	toml_pair * pairs;
};

struct toml_array
{
	toml_type type; // Arrays in toml only support one type, though it can be arrays.
	toml_node * values;
};

struct toml_node
{
	toml_value * val;
	toml_node * next, *prev;
};

struct toml_pair
{
	char * key;
	toml_value * val;
	toml_pair * next, *prev;
};

toml_table * toml_parse(const char * src)
{
	return NULL;
}
