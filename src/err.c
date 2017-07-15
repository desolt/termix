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

void tx_gl_print_errors(void)
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

int tx_gl_geterr(void)
{
	// Query OpenGL to see if there were any errors
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		if (log_entries >= MAX_ERROR_LOG_LENGTH)
		{
			fprintf(stderr, "termix: warning: no room in error log\n");
			break;
		}
		else
		{
			char * str = NULL;
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
			error_log[log_entries++] = strdup(str);
		}
	}
	return log_entries == 0 ? 0 : -1;
}

// http://www.glfw.org/docs/latest/intro_guide.html#error_handling
// http://www.glfw.org/docs/latest/group__errors.html
void tx_gl_error_callback(int error, const char * description)
{
	(void)error; // Suppress unused warning
	if (log_entries >= MAX_ERROR_LOG_LENGTH)
	{
		fprintf(stderr, "warning: no room in error log\n");
	}
	else
	{
		error_log[log_entries++] = strdup(description);
	}
}
