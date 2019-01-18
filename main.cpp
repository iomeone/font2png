#include "freetype/ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "lodepng/lodepng.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct
{
	FT_Library lib;
	FT_Face face;
}ftInfo;



int main()
{
	ftInfo ft;

	FT_GlyphSlot g;

	if (FT_Init_FreeType(&ft.lib))
	{
		printf("Failed to initialize freetype");
		return 0;
	}


	if (FT_New_Face(ft.lib, "Bravura.ttf", 0, &ft.face))
	{
		printf("Problem loading fontfile %s", "Bravura.ttf");
		return 0;
	}

	int fontHeight = 256;


	FT_Set_Pixel_Sizes(ft.face, fontHeight, fontHeight);
	g = ft.face->glyph;


	int l = 0xE1E1;

	int glyph_index = FT_Get_Char_Index(ft.face, l);
	FT_Load_Glyph(ft.face, glyph_index, 0);
	FT_Render_Glyph(ft.face->glyph, FT_RENDER_MODE_MONO);

	int pngSize = ft.face->glyph->bitmap.rows * ft.face->glyph->bitmap.width * sizeof(int) + 21;  // why + 21?  i try it out,  20 will crash.
	int *  buf = malloc(pngSize);


	for (int i = 0; i < ft.face->glyph->bitmap.rows; i++)
	{
		int index = ft.face->glyph->bitmap.width * i;
		for (unsigned int j = 0, byteIndex = i * ft.face->glyph->bitmap.pitch; j < ft.face->glyph->bitmap.width; j += 8, byteIndex++)
		{                         //a b g r

			buf[index++] = ((g->bitmap.buffer[byteIndex] & 128) == 128 ? 0xffffffff : 0);
			buf[index++] = ((g->bitmap.buffer[byteIndex] & 64) == 64 ? 0xffffffff : 0);
			buf[index++] = ((g->bitmap.buffer[byteIndex] & 32) == 32 ? 0xffffffff : 0);
			buf[index++] = ((g->bitmap.buffer[byteIndex] & 16) == 16 ? 0xffffffff : 0);
			buf[index++] = ((g->bitmap.buffer[byteIndex] & 8) == 8 ? 0xffffffff : 0);
			buf[index++] = ((g->bitmap.buffer[byteIndex] & 4) == 4 ? 0xffffffff : 0);
			buf[index++] = ((g->bitmap.buffer[byteIndex] & 2) == 2 ? 0xffffffff : 0);
			buf[index++] = ((g->bitmap.buffer[byteIndex] & 1) == 1 ? 0xffffffff : 0);
		}
	}
	lodepng_encode32_file("clam.png", (char*)buf, g->bitmap.width, g->bitmap.rows);

	free(buf);
	return;
}