#ifndef _TOML_H
#define _TOML_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct toml_table toml_table;
typedef struct toml_array toml_array;

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

/// @return The root table. name will be a null pointer.
toml_table * toml_parse(const char * src);

#endif // _TOML_H
