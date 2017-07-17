#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "err.h"

#define MAX_ERROR_LOG_LENGTH 20

char * error_log[MAX_ERROR_LOG_LENGTH] = { NULL };
size_t log_entries = 0;

void tx_print_errors(void)
{
	assert(log_entries != 0);

	size_t i;
	for (i = 0; i < log_entries; i++)
	{
		fprintf(stderr, "termix: error %zu: %s\n", i + 1, error_log[i]);
		free(error_log[i]);
	}

	memset(error_log, 0, sizeof error_log);
	log_entries = 0;
}

tx_err tx_geterr(void)
{
	// Query OpenGL to see if there were any errors
	size_t num_gl_errors = 0;
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		char * str = NULL;
		num_gl_errors++;
		switch (error)
		{
		case GL_INVALID_ENUM:
			str = "OpenGL: INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			str = "OpenGL: INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			str = "OpenGL: INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			str = "OpenGL: STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			str = "OpenGL: STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			str = "OpenGL: OUT_OF_MEMORY";
			break;
		default:
			str = "OpenGL: Unknown error";
			break;
		}
		tx_errlog_append(str);
	}

	if (num_gl_errors > 0)
		return E_GL;

	// TODO: This might not be a GLFW error (could be one of ours)
	return log_entries == 0 ? E_NONE : E_GLFW;
}

tx_err tx_errlog_append(const char * description)
{
	assert(description != NULL);
	if (log_entries == MAX_ERROR_LOG_LENGTH)
	{
		fprintf(stderr, "termix: warning: no room in error log\n");
		fprintf(stderr, "%s", description);
		return E_LOG_FULL;
	}

	error_log[log_entries++] = strdup(description);
	return E_NONE;
}

// printf style wrapper around tx_errlog_append
tx_err tx_errlog_append_fmt(const char * fmt, ...)
{
	int size = 0;
	va_list args;
	va_list temp_args;

	va_start(args, fmt);

	// Get size
	va_copy(temp_args, args);
	size = vsnprintf(NULL, size, fmt, temp_args);
	assert(size >= 0);
	va_end(temp_args);

	char * str = malloc(size + 1);
	assert(str != NULL); // TODO: Actually handle the error (and possibly abort)

	size = vsprintf(str, fmt, args);
	assert(size >= 0);

	va_end(args);

	tx_err rv = tx_errlog_append(str); // TODO: Don't allocate twice. Wasteful!
	free(str);
	return rv;
}

// http://www.glfw.org/docs/latest/intro_guide.html#error_handling
// http://www.glfw.org/docs/latest/group__errors.html
void tx_glfw_error_callback(int error, const char * description)
{
	(void)error; // Suppress unused warning
	tx_errlog_append(description);
}
