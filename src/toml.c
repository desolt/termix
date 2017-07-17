#include "toml.h"
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUCKET_COUNT 16
#define MAX_TOML_ERR_MSG_LEN 128

char toml_err_log[MAX_TOML_ERR_MSG_LEN] = "\0";

struct toml_node
{
	toml_value * val;
	toml_node * next;
};

struct toml_pair
{
	char * key;
	toml_value * val;
	toml_pair * next;
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

void toml_free(toml_table * table);
void toml_free_array(toml_array * arr);
void toml_free_value(toml_value * val);

// header shows it for deleting root, but deletes any table.
void toml_free(toml_table * table)
{
	// free everything the table contains
	for (size_t i = 0; i < table->num_buckets; ++i)
	{
		toml_pair * pair = table->buckets[i];
		while (pair != NULL && pair->next)
		{
			toml_pair * prev = pair;
			pair = prev->next;
			toml_free_value(prev->val);
			free(prev->key);
			free(prev);
		}
	}

	free(table->name);
	free(table->buckets);
	free(table);
}

void toml_free_array(toml_array * arr)
{
	toml_node * node = arr->values;
	while (node != NULL)
	{
		toml_node * prev = node;
		node = node->next;
		toml_free_value(prev->val);
		free(prev);
	}
}

void toml_free_value(toml_value * val)
{
	switch (val->type)
	{
	case TOML_TABLE:
		toml_free(val->val.table);
		break;
	case TOML_ARRAY:
		toml_free_array(val->val.array);
		break;
	case TOML_STRING:
		free(val->val.string);
		break;
	default: break; // rest are stack allocated
	}

	free(val);
}


toml_table * toml_create_table(const char * name, size_t buckets, toml_table * parent)
{
	// these are required, don't hackishly create a root table.
	assert(parent != NULL);
	assert(name != NULL);

	toml_table * table = calloc(1, sizeof(toml_table));
	if (table == NULL) return NULL;

	table->name = strdup(name);
	table->buckets = calloc(1, sizeof(toml_pair *));
	table->num_buckets = buckets;

	// insert the new table into the parent
	toml_value * table_val = calloc(1, sizeof(toml_value));
	table_val->type = TOML_TABLE;
	table_val->val.table = table;
	toml_table_emplace(parent, name, table_val, NULL);

	return table;
}

size_t hash(const char * key)
{
	// Magic numbers:
	const size_t PRIME1 = 31;
	const size_t PRIME2 = 73;
	const size_t PRIME3 = 103;

	size_t hash = PRIME1;
	for (size_t i = 0; i < strlen(key); ++i)
		hash = (hash * PRIME2) ^ (key[i] * PRIME3);

	return hash;
}

bool toml_table_has_child(const toml_table * table, const char * name)
{
	toml_value * val = toml_table_get(table, name);
	if (val == NULL) return false;

	return val->type == TOML_TABLE; // not the same as has_key
}

bool toml_table_has_key(const toml_table * table, const char * name)
{
	return toml_table_get(table, name) != NULL;
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
	toml_node * node = array->values;
	for (size_t i = 0; i < index; ++i)
	{
		if (node == NULL) return NULL;

		node = node->next;
	}

	return node->val;
}

toml_err toml_table_emplace(toml_table * table, const char * key, toml_value * val, toml_pair ** out)
{
	toml_pair * pair = calloc(1, sizeof(toml_pair));
	pair->key = strdup(key);
	pair->val = val;
	pair->next = NULL;

	size_t index = hash(key) % table->num_buckets;
	toml_pair * node = table->buckets[index];
	if (node != NULL)
	{
		while (node->next != NULL)
		{
			if (strcmp(key, node->key))
				node = node->next; // find end of linked list
			else
				break; // replace the current value if the same key.
		}

		node->next = pair;
	}
	else
		table->buckets[index] = pair;

	if (out != NULL) *out = pair;

	return TOML_SUCCESS;
}

toml_err parse_string(const char * src, const char ** loc, char ** out)
{
	assert(*src == '\'' || *src == '"');
	assert(out != NULL);

	bool literal = *src == '\'';
	bool triple  = !strncmp("\"\"\"", src, 3);

	const char * end = NULL;
	if (literal)
		end = strstr(src + 1, "\'");
	else if (triple)
		end = strstr(src + 3, "\"\"\"");
	else
	{
		do
			end = strstr(src + 1, "\"");
		while (end != NULL && (*end != '"' || (*end - 1) == '\\'));
	}

	const char * newlineloc = strstr(src, "\n");
	if (!end || (newlineloc && newlineloc < end && !triple)) // if an end is not found or a newline is made too early
	{
		sprintf(toml_err_log, "string was not terminated.");
		return TOML_PARSE_FAILURE;
	}

	const size_t offset = triple ? 3 : 1; // the length of the quotes
	const size_t len = end - (src + offset);
	char * str = calloc(len, sizeof(char));
	strncpy(str, src + offset, len);
	str[len] = '\0';

	*out = str;
	if (loc != NULL)
		*loc = end + offset;

	return TOML_SUCCESS;
}

toml_err parse_num(const char * src, const char ** loc, int * iout, float * fout, toml_type * type)
{
	assert(isdigit(*src) || *src == '.');
	assert(iout != NULL && fout != NULL);
	assert(type != NULL);

	const char * end = strstr(src, "\n");
	if (end == NULL)
		end = src + strlen(src); // if no newline, expect end of value to be at the end of the string.

	size_t len = end - src;
	char num_str[len];
	num_str[len] = '\0';

	bool isfloat = false;
	bool ishex   = false;
	for (size_t i = 0; i < len; ++i)
	{
		if (!isdigit(src[i]) && src[i] != '.' && tolower(src[i]) != 'x')
		{
			sprintf(toml_err_log, "unexpected character '%c' in number value.", src[i]);
			return TOML_PARSE_FAILURE;
		}

		if (src[i] == '.')
		{
			if (!isfloat && !ishex)
				isfloat = true;
			else
			{
				sprintf(toml_err_log, "unexpected decimal point.");
				return TOML_PARSE_FAILURE;
			}
		}
		else if (tolower(src[i]) == 'x')
		{
			if (!ishex && !isfloat)
				ishex = true;
			else
			{
				sprintf(toml_err_log, "unexpected character '%c'.", src[i]);
				return TOML_PARSE_FAILURE;
			}
		}

		num_str[i] = src[i];
	}

	if (isfloat)
	{
		*fout = strtof(num_str, NULL);
		*type = TOML_FLOAT;
	}
	else
	{
		if (ishex)
			*iout = (int) strtol(src, NULL, 0);
		else
			*iout = (int) strtol(src, NULL, 10);

		*type = TOML_INT;
	}

	printf("%s\n", num_str);
	if (loc != NULL)
		*loc = end;

	return TOML_SUCCESS;
}

toml_err toml_parse(const char * src, toml_table ** out)
{
	assert(src != NULL);
	assert(out != NULL);

	toml_table * root = toml_init(DEFAULT_BUCKET_COUNT);

	const char * ptr = src;
	while (ptr != NULL && *ptr != '\0')
	{
		switch (*ptr)
		{
		case ' ':
		case '\n':
			++ptr; // ignore whitespace
			continue;
		}
	}

	*out = root;

	return TOML_SUCCESS;
}

toml_err toml_parse_file(FILE * file, toml_table ** out)
{
	rewind(file); // ensure that we start at the start of a file
	fseek(file, 0l, SEEK_END);
	long size = ftell(file);
	rewind(file);

	char buffer[size + 1];
	fread(buffer, sizeof(char), size, file);
	buffer[size] = '\0'; // fread doesn't null terminate

	return toml_parse(buffer, out);
}

size_t toml_get_err_msg(char * buffer, size_t buf_len)
{
	size_t err_len = strlen(toml_err_log) + 1;
	if (err_len == 0)
	{
		*buffer = '\0';
		return 0;
	}

	size_t len = err_len < buf_len ? err_len : buf_len;
	strncpy(buffer, toml_err_log, len);
	return len;
}
