#include "font.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

// TODO: FT_Face NEEDS to be calloc'd here before FT_New_Face, how are we going to ensure that?
// See https://www.freetype.org/freetype2/docs/reference/ft2-user_allocation.html
tx_err tx_load_fontface(FT_Library * ft, FT_Face * face, const char * path)
{
	assert(ft != NULL); assert(face != NULL); assert(path != NULL);

	if (FT_New_Face(*ft, path, 0, face) != 0)
	{
		tx_errlog_append_fmt("failed to load font face from path %s", path);

		// Assume it's invalid path even though we aren't sure
		// TODO: Check the return value of FT_New_Face to see what failed
		return E_INVALID_PATH;
	}

	return E_NONE;
}

tx_err tx_unload_fontface(FT_Face * face)
{
	if (FT_Done_Face(*face) != 0)
	{
		// TODO: Include path of font face in this message
		// We'll need our wrapper struct for that probably
		tx_errlog_append("failed to unload font face");
		return E_FT;
	}

	return E_NONE;
}
