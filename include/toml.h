#ifndef _TOML_H
#define _TOML_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct toml_table toml_table;
typedef struct toml_array toml_array;
typedef struct toml_pair toml_pair;
typedef struct toml_node toml_node;

typedef enum toml_err
{
	toml_success = 0,
} toml_err;

// The different types of values toml supports.
typedef enum toml_type
{
	TOML_TABLE,
	TOML_ARRAY,
	TOML_STRING,
	TOML_INT,
	TOML_FLOAT,
	TOML_BOOL
} toml_type;

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


// TODO: UTF-8
typedef struct toml_value
{
	toml_type type;

	// Check toml_type before retrieving.
	union
	{
		toml_table * table_val;
		toml_array * arr_val;

		const char * str_val;
		int int_val;
		float float_val;
		bool bool_val;

		struct
		{
			uint8_t offset;
		} datetime; // TODO: Complete
	};
} toml_value;

toml_table * toml_init();
/// Every table requires a parent & name, except for the root table.
toml_table * toml_create_table(const char * name, toml_table * parent);
bool toml_table_has_child(const toml_table * table, const char * name);

/// @return the value associated with the key. NULL if does not exist.
toml_value * toml_table_get(const toml_table * table, const char * key);
toml_value * toml_array_at(const toml_array * array, size_t index);

/// @return The root table. name will be a null pointer.
toml_err toml_parse(const char * src, toml_table ** root);

#endif // _TOML_H
