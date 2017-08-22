/**
*  @todo  use our own longjmp instead of libpng's.  this way we don't need
*         to use PNG_SETJMP_SUPPORTED in Windows, and don't depend on
*         png_ptr->jmpbuf in older versions of libpng.
*/


#include "png.h"
#include "Debug.h"
#include "Open.h"
#include "SimpleImage.h"
#include "Utility.h"

namespace corona {

	typedef unsigned char byte;


	//////////////////////////////////////////////////////////////////////////////

	void PNG_read_function(png_structp png_ptr,
		png_bytep data,
		png_size_t length) {
			File* file = (File*)png_get_io_ptr(png_ptr);
			if (file->read(data, length) != int(length)) {
				png_error(png_ptr, "Read error");
			}
	}

	//////////////////////////////////////////////////////////////////////////////

	void PNG_warning_function(png_structp png_ptr, png_const_charp error) {
		// no warnings
	}

	//////////////////////////////////////////////////////////////////////////////

	void PNG_error_function(png_structp png_ptr, png_const_charp warning) {
		// copied from libpng's pngerror.cpp, but without the fprintf
		jmp_buf jmpbuf;
		memcpy(jmpbuf, png_ptr->jmpbuf, sizeof(jmp_buf));
		longjmp(jmpbuf, 1);
	}

	//////////////////////////////////////////////////////////////////////////////

	void fill_palette(png_structp png, png_infop info, png_color palette[256]) {

		COR_GUARD("fill_palette");

		// by default, the palette is greyscale
		for (int i = 0; i < 256; ++i) {
			palette[i].red   = i;
			palette[i].green = i;
			palette[i].blue  = i;
		}

		// do we have a palette and is it big enough?
		png_colorp png_palette;
		int num_palette = 0;
		png_get_PLTE(png, info, &png_palette, &num_palette);

		COR_IF_DEBUG {
			char str[80];
			sprintf(str, "palette size: %d", num_palette);
			COR_LOG(str);
		}

		if (num_palette >= 256) {

#if 0
			COR_IF_DEBUG {
				for (int i = 0; i < 256; ++i) {
					char str[80];
					sprintf(str, "r(%d) g(%d) b(%d)",
						int(palette[i].red),
						int(palette[i].green),
						int(palette[i].blue));
					COR_LOG(str);
				}
			}
#endif

			memcpy(palette, png_palette, 256 * sizeof(png_color));
		}
	}


	////////////////////////////////////////////////////////////////////////////////
	// PngRead

	//static
	void PngRead( png_structp pPngRead, png_bytep pData, png_size_t Length )
	{
		File* file = ( File* ) png_get_io_ptr( pPngRead );
		file->read( pData, Length );
	}

//////////////////////////////////////////////////////////////////////////////

Image* OpenPNG(File* file)
{
	// verify PNG signature
	byte sig[8];
	file->read(sig, 8);
	if (png_sig_cmp(sig, 0, 8)) {
		return 0;
	}

	// Create the read struct
	png_structp pPngRead = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if ( pPngRead == NULL )
	{
	}

	// Create the info structure
	png_infop pPngInfo = png_create_info_struct( pPngRead );
	if ( pPngInfo == NULL )
	{
		png_destroy_read_struct( &pPngRead, NULL, NULL );
	}

	png_infop pPngEndInfo = png_create_info_struct( pPngRead );
	if ( pPngEndInfo == NULL )
	{
		png_destroy_read_struct( &pPngRead, &pPngInfo, NULL );
	}

	// Set error handling
	if ( setjmp( png_jmpbuf( pPngRead ) ) )
	{
		png_destroy_read_struct( &pPngRead, &pPngInfo, &pPngEndInfo );
	}

	// Tell libpng how many bytes we used for header
	png_set_sig_bytes( pPngRead, 8 );

	// Set up our custom read function
	png_set_read_fn( pPngRead, file, &PngRead );

	// Read the info data
	png_read_info( pPngRead, pPngInfo );

	// Get the info and decide what we want to do with this image
	png_uint_32 PngWidth;
	png_uint_32 PngHeight;
	int PngBitDepth;
	int PngColourType;
	int PngInterlaceMethod;
	int PngCompressionMethod;
	int PngFilterMethod;

	png_get_IHDR( pPngRead, pPngInfo, &PngWidth, &PngHeight, &PngBitDepth, &PngColourType, &PngInterlaceMethod, &PngCompressionMethod, &PngFilterMethod );

	// Read the palette if present
	png_colorp pPngPalette = NULL;
	int nPngPaletteEntries = 0;
	if ( png_get_valid( pPngRead, pPngInfo, PNG_INFO_PLTE ) )
	{
		png_get_PLTE( pPngRead, pPngInfo, &pPngPalette, &nPngPaletteEntries );
	}

	// Read the transparency data if present
	png_bytep pPngPaletteAlphaData = NULL;
	png_color_16p pPngAlphaData=NULL;
	int nPngAlphaEntries = 0;
	if ( png_get_valid( pPngRead, pPngInfo, PNG_INFO_tRNS ) )
	{
		png_get_tRNS( pPngRead, pPngInfo, &pPngPaletteAlphaData, &nPngAlphaEntries, &pPngAlphaData );
	}

	// Ignore gamma
	float PngImageGamma = 1.0f / 2.2f;
	float PngScreenGamma = 2.2f;
	png_set_gamma( pPngRead, PngScreenGamma, PngImageGamma );

	// Make sure that the image always unpacks to 8-bit
	if ( PngBitDepth == 16 )
	{
		png_set_strip_16( pPngRead );
	}
	if ( PngBitDepth < 8 )
	{
		png_set_packing( pPngRead );
	}

	// Update our custom settings
	png_read_update_info( pPngRead, pPngInfo );

	bool isPalette = false;

	int pixelsize = 0;
	int palettesize = 0;

	switch( PngColourType )
	{
		case PNG_COLOR_TYPE_PALETTE:
			{
				//PixFmt = ( nPngPaletteEntries > 16 ) ? PAL_8BPP : PAL_4BPP;
				isPalette = true;

				if( nPngAlphaEntries == 0 )
				{
					palettesize = 3;
				}
				else
				{
					palettesize = 4;
				}

				pixelsize = 1;
			}
			break;

		case PNG_COLOR_TYPE_RGB:
			{
				pixelsize = 3;
				isPalette = false;
			}
			break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
			{
				if ( nPngPaletteEntries > 0 )
				{
					int a=0;
					a++; // error here
				}

				pixelsize = 4;
				isPalette = false;
			}
			break;
	}

	corona::byte* palette = NULL;

	if( isPalette == true )
	{
		palette = (corona::byte*)new byte[ 256 * palettesize ];
	}

	int palette_size = 256;

	corona::byte* pixels = (corona::byte*)new byte[ PngWidth * PngHeight * pixelsize ];

	corona::byte* currentPalette = palette;

	// Set the image palette
	if ( nPngPaletteEntries > 0 )
	{
		for ( int iPal = 0; iPal < nPngPaletteEntries; ++iPal )
		{
			unsigned char red, green, blue;
			red   = pPngPalette[ iPal ].red;
			green = pPngPalette[ iPal ].green;
			blue  = pPngPalette[ iPal ].blue;

			*currentPalette++ = blue;
			*currentPalette++ = green;
			*currentPalette++ = red;

			if( nPngAlphaEntries > 0 )
			{
				*currentPalette++ =  pPngPaletteAlphaData[ iPal ];
			}
		}
	}

	png_bytepp pPngRowPointers = new png_bytep[ PngHeight ];

	int ImageRowSize = PngWidth * pixelsize;

	int ImageSize = ImageRowSize * PngHeight;

	//png_bytep pPngImageData = new png_byte[ ImageSize ];

	for ( unsigned int iRow = 0; iRow < PngHeight; ++iRow )
	{
		pPngRowPointers[ iRow ] = pixels + ( iRow * ImageRowSize );
	}

	// Read in the image data
	png_read_image( pPngRead, pPngRowPointers );

	// Close PNG
	png_destroy_read_struct( &pPngRead, &pPngInfo, &pPngEndInfo );

	// Delete the temporary row pointers
	delete [] pPngRowPointers;

	if( isPalette )
	{
		if( nPngAlphaEntries )
		{
			return new SimpleImage(PngWidth, PngHeight, PF_I8, pixels, palette, palette_size, PF_B8G8R8A8 );
		}
		else
		{
			return new SimpleImage(PngWidth, PngHeight, PF_I8, pixels, palette, palette_size, PF_B8G8R8 );
		}
	}
	else
	{
		if( pixelsize == 3 )
		{
			return new SimpleImage(PngWidth, PngHeight, PF_R8G8B8, pixels );
		}
		else
		{
			return new SimpleImage(PngWidth, PngHeight, PF_R8G8B8A8, pixels );
		}
	}
}
/*
	COR_GUARD("OpenPNG");

	// verify PNG signature
	byte sig[8];
	file->read(sig, 8);
	if (png_sig_cmp(sig, 0, 8)) {
		return 0;
	}

	COR_LOG("Signature verified");

	// read struct
	png_structp png_ptr = png_create_read_struct(
		PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);
	if (!png_ptr) {
		return 0;
	}

	COR_LOG("png struct created");

	// info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	COR_LOG("info struct created");

	// the PNG error function calls longjmp(png_ptr->jmpbuf)
	if (setjmp(png_jmpbuf(png_ptr))) {
		COR_LOG("Error loading PNG");
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 0;
	}

	COR_LOG("setjmp() succeeded");

	// set the error function
	png_set_error_fn(png_ptr, 0, PNG_error_function, PNG_warning_function);

	// read the image
	png_set_read_fn(png_ptr, file, PNG_read_function);
	png_set_sig_bytes(png_ptr, 8);  // we already read 8 bytes for the sig
	// always give us 8-bit samples (strip 16-bit and expand <8-bit)
	int png_transform = PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_EXPAND;
	png_read_png(png_ptr, info_ptr, png_transform, NULL);

	COR_LOG("PNG read");

	if (!png_get_rows(png_ptr, info_ptr)) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 0;
	}

	int width  = png_get_image_width(png_ptr, info_ptr);
	int height = png_get_image_height(png_ptr, info_ptr);
	byte* pixels = 0;  // allocate when we know the format
	PixelFormat format;
	byte* palette = 0;
	PixelFormat palette_format;

	// decode based on pixel format
	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	int num_channels = png_get_channels(png_ptr, info_ptr);
	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	bool is_palettised = false;

	if( png_ptr->num_palette > 0 )
	{
		is_palettised = true;
	}
		// 32-bit RGBA
		if (bit_depth == 8 && num_channels == 4 && !is_palettised) {
			COR_LOG("32-bit RGBA: bit_depth = 8 && num_channels = 4");

			format = PF_R8G8B8A8;
			pixels = new byte[width * height * 4];
			for (int i = 0; i < height; ++i) {
				memcpy(pixels + i * width * 4, row_pointers[i], width * 4);
			}

			// 24-bit RGB
		} else if (bit_depth == 8 && num_channels == 3 && !is_palettised) {
			COR_LOG("24-bit RGB: bit_depth = 8 && num_channels = 3");

			format = PF_R8G8B8;
			pixels = new byte[width * height * 3];
			for (int i = 0; i < height; ++i) {
				memcpy(pixels + i * width * 3, row_pointers[i], width * 3);
			}

			// palettized or greyscale with alpha
		} else if ( is_palettised && bit_depth == 8)
		{
				png_color png_palette[256];
				fill_palette(png_ptr, info_ptr, png_palette);

				if (num_channels == 2) {
					COR_LOG("bit_depth = 8 && num_channels = 2");

					format = PF_R8G8B8A8;
					pixels = new byte[width * height * 4];
					byte* out = pixels;

					for (int i = 0; i < height; ++i) {
						byte* in = row_pointers[i];
						for (int j = 0; j < width; ++j) {
							byte c = *in++;
							*out++ = png_palette[c].red;
							*out++ = png_palette[c].green;
							*out++ = png_palette[c].blue;
							*out++ = *in++;  // alpha
						}
					}

				} else { // (num_channels == 1)

					pixels = new byte[width * height];
					format = PF_I8;
					palette = new byte[256 * 4];
					palette_format = PF_R8G8B8A8;


					// get the transparent palette flags
					png_bytep trans;
					int num_trans = 0;
					png_color_16p trans_values; // XXX not used - should be?
					png_uint_32 rc = png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);

					// copy the palette from the PNG
					for (int i = 0; i < 256; ++i) {
						palette[i * 4 + 0] = png_palette[i].red;
						palette[i * 4 + 1] = png_palette[i].green;
						palette[i * 4 + 2] = png_palette[i].blue;
						palette[i * 4 + 3] = 255;
					}
					// apply transparency to palette entries
					for (int i = 0; i < num_trans; ++i) {
						palette[trans[i] * 4 + 3] = 0;
					}

					byte* out = pixels;
					for (int i = 0; i < height; ++i) {
						memcpy(out, row_pointers[i], width);
						out += width;
					}
			}

		} else {  // unknown format
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			return 0;
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		if (palette) {
			return new SimpleImage(width, height, format, pixels,
				palette, 256, palette_format);
		} else {
			return new SimpleImage(width, height, format, pixels);
		}
}

//////////////////////////////////////////////////////////////////////////////

*/
}
