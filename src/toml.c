#include "toml.h"
#include <stdlib.h>
#include <string.h>

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

toml_err toml_table_emplace(toml_table * table, const char * key, toml_value *val, toml_pair ** out);

toml_table * toml_init()
{
	toml_table *root = calloc(1, sizeof(toml_table));
	if (root == NULL) return NULL;

	root->name = NULL;
	root->pairs = NULL;

	return root;
}

toml_table * toml_create_table(const char *name, toml_table * parent)
{
	// these are required, don't hackishly create a root table.
	if (parent == NULL || name == NULL) return NULL;

	toml_table * table = calloc(1, sizeof(toml_table));
	if (table == NULL) return NULL;

	table->name = strdup(name);
	table->pairs = NULL;

	// insert the new table into the parent
	toml_value *table_val = calloc(1, sizeof(toml_value));
	table_val->type = TOML_TABLE;
	table_val->table_val = table;
	toml_table_emplace(parent, name, table_val, NULL);

	return table;
}

bool toml_table_has_child(const toml_table * table, const char * name)
{
	toml_value * val = toml_table_get(table, name);
	if (val == NULL) return false;

	return val->type == TOML_TABLE; // not the same as has_key
}

toml_value * toml_table_get(const toml_table * table, const char * key)
{
	toml_pair *pair = table->pairs;
	if (pair == NULL) return NULL;

	while (pair != NULL) 
	{
		if (!strcmp(key, pair->key)) 
		{
			return pair->val;
		}

		pair = pair->next;
	}

	return false;
}

/// doesn't check if the key exists already
toml_err toml_table_emplace(toml_table * table, const char * key, toml_value *val, toml_pair ** out)
{
	toml_pair *pair = calloc(1, sizeof(toml_pair));
	pair->key = strdup(key);
	pair->val = val;

	toml_pair *node = table->pairs;
	if (node != NULL)
	{
		while (node->next != NULL) node = node->next; // find end of linked list
		node->next = pair;
	} else
	{
		table->pairs = pair;
	}

	if (out != NULL) *out = pair;

	return toml_success;
}

toml_err toml_parse(const char * src, toml_table ** out)
{
	return toml_success;
}
