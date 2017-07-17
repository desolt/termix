#ifndef _FONT_H
#define _FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "err.h"

// In the future, we'll probably want to wrap FT_Face with our own struct, since
// it will allow us to keep information such as the path where we loaded it from

tx_err tx_load_fontface(FT_Library * ft, FT_Face * face, const char * path);
tx_err tx_unload_fontface(FT_Face * face);

#endif
