#ifndef _ERR_H
#define _ERR_H

#include <stdarg.h>

typedef enum
{
	E_NONE = 0,
	E_GL,   // OpenGL error
	E_GLFW, // GLFW error
	E_FT,   // Freetype error
	E_INVALID_PATH,
	E_LOG_FULL,
	E_NO_MEM,
	E_OTHER
} tx_err;

/* First, checks to see if OpenGL has anything on its error stack. If it does, it appends
 * those errors to the error log. Otherwise, it checks if there are any pending error
 * in the error log and returns them. Usually this should be followed by a call to
 * tx_print_errors, because we know there will be errors in the error log.
 * Internal functions that directly return tx_err can usually skip this function and
 * instead call tx_print_errors straightaway.
 */
tx_err tx_geterr(void);

/* Prints any previous OpenGL or GLFW errors and removes them from the error log once
 * that has been done.
 */
void tx_print_errors(void);

/* Appends a message to the error log.
 * The string is copied, so you can deallocate it if needed afterwards. The string should
 * also not end in a newline (that is added to the output during tx_print_errors).
 * It is suggested to use asprintf to dynamically create strings for this function.
 * Returns E_LOG_FULL and immediately prints the description if there is no room.
 */
tx_err tx_errlog_append(const char * description);

/* Same as the above, except writes a string according to a format instead.
 * e.g tx_errlog_append_fmt("%s:%d", some_string, some_int);
 */
tx_err tx_errlog_append_fmt(const char * fmt, ...);

// GLFW calls this whenever there's an error
void tx_glfw_error_callback(int error, const char * description);

#endif
