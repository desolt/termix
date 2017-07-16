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

toml_err toml_table_emplace(toml_table * table, const char * key, toml_value * val, toml_pair ** out);

toml_table * toml_init(size_t buckets)
{
	toml_table * root = calloc(1, sizeof(toml_table));
	if (root == NULL) return NULL;

	root->name = NULL;
	root->buckets = calloc(buckets, sizeof(toml_pair *));
	root->num_buckets = buckets;

	return root;
}

toml_table * toml_create_table(const char * name, size_t buckets, toml_table * parent)
{
	// these are required, don't hackishly create a root table.
	if (parent == NULL || name == NULL) return NULL;

	toml_table * table = calloc(1, sizeof(toml_table));
	if (table == NULL) return NULL;

	table->name = strdup(name);
	table->buckets = calloc(1, sizeof(toml_pair *));
	table->num_buckets = buckets;

	// insert the new table into the parent
	toml_value * table_val = calloc(1, sizeof(toml_value));
	table_val->type = TOML_TABLE;
	table_val->table_val = table;
	toml_table_emplace(parent, name, table_val, NULL);

	return table;
}

size_t hash(const char * key)
{
	size_t hash = 31;
	for (size_t i = 0; i < strlen(key); ++i)
		hash = (hash * 31) ^ (key[i] * 103);

	return hash;
}

bool toml_table_has_child(const toml_table * table, const char * name)
{
	toml_value * val = toml_table_get(table, name);
	if (val == NULL) return false;

	return val->type == TOML_TABLE; // not the same as has_key
}

toml_value * toml_table_get(const toml_table * table, const char * key)
{
	toml_pair * pair = table->buckets[hash(key) % table->num_buckets];
	if (pair == NULL) return NULL;

	while (pair != NULL)
	{
		if (!strcmp(key, pair->key))
			return pair->val;

		pair = pair->next;
	}

	return NULL;
}

toml_value * toml_array_at(const toml_array * array, size_t index)
{
	toml_node *node = array->values;
	for (size_t i = 0; i < index; ++i) {
		if (node == NULL) return NULL;

		node = node->next;
	}

	return node->val;
}

/// doesn't check if the key exists already
toml_err toml_table_emplace(toml_table * table, const char * key, toml_value * val, toml_pair ** out)
{
	toml_pair * pair = calloc(1, sizeof(toml_pair));
	pair->key = strdup(key);
	pair->val = val;

	size_t index = hash(key) % table->num_buckets;
	toml_pair * node = table->buckets[index];
	if (node != NULL)
	{
		while (node->next != NULL) node = node->next; // find end of linked list
		node->next = pair;
	}
	else
		table->buckets[index] = pair;

	if (out != NULL) *out = pair;

	return toml_success;
}

toml_err toml_parse(const char * src, toml_table ** out)
{
	return toml_success;
}
