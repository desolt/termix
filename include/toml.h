#ifndef _TOML_H
#define _TOML_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct toml_table toml_table;
typedef struct toml_array toml_array;
typedef struct toml_pair toml_pair;
typedef struct toml_node toml_node;

typedef enum toml_err
{
	TOML_SUCCESS = 0,
	TOML_BAD_TYPE = 1, // for arrays
	TOML_PARSE_FAILURE = 2,
} toml_err;

// The different types of values toml supports.
typedef enum toml_type
{
	TOML_TABLE,
	TOML_ARRAY,
	TOML_STRING,
	TOML_INT,
	TOML_FLOAT,
	TOML_BOOL,
	TOML_DATETIME,
	TOML_DATE,
	TOML_TIME,
} toml_type;

struct toml_table
{
	char * name; // NULL if root.
	toml_pair ** buckets;
	size_t num_buckets;
};

struct toml_array
{
	toml_type type; // Arrays in toml only support one type, though it can be arrays.
	toml_node * values;
};

// TODO: Complete
struct toml_datetime
{
	int offset;
	int day, month, year;
	int second, minute, hour;
};


// TODO: UTF-8
typedef struct toml_value
{
	toml_type type;

	// Check toml_type before retrieving.
	union
	{
		toml_table * table;
		toml_array * array;

		char * string;
		int integer;
		float floating;
		bool boolean;

		struct toml_datetime datetime;
	} val;
} toml_value;

toml_table * toml_init(size_t buckets);
void toml_free(toml_table * root);
/// Every table requires a parent & name, except for the root table.
toml_table * toml_create_table(const char * name, size_t buckets, toml_table * parent);
bool toml_table_has_child(const toml_table * table, const char * name);
bool toml_table_has_key(const toml_table * table, const char * key);

/// @return the value associated with the key. NULL if does not exist.
toml_value * toml_table_get(const toml_table * table, const char * key);

toml_array * toml_create_array(const toml_table * table);
toml_value * toml_array_at(const toml_array * array, size_t index);

/// @return The root table. name will be a null pointer.
toml_err toml_parse(const char * src, toml_table ** root);
toml_err toml_parse_file(FILE * file, toml_table ** root);

/// @param len the length of the buffer.
/// copies the error message to the buffer, including the null terminator.
size_t toml_get_err_msg(char * buffer, size_t len);

#endif // _TOML_H
