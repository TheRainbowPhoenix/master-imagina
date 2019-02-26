//-------------------------------------------------------------------
// tga.cpp
//-------------------------------------------------------------------
// Load and save 24 bits RGB pictures in Targa file format (.TGA).
//-------------------------------------------------------------------
// Created  : 05/12/1999
// Modified : 05/12/1999
//-------------------------------------------------------------------



#include <stdio.h>
#include <stdlib.h>
#include "tga.h"



//-------------------------------------------------------------------
// load_tga
//-------------------------------------------------------------------
// Load an uncompressed 24 or 32 bpp picture from a Targa file format.
//-------------------------------------------------------------------
// Input :
//     name   (in): picture file name.
//     width (out): picture width.
//     height(out): picture height.
// Output:
//     Pointer on 24 or 32 bits RGB graphic data.
//-------------------------------------------------------------------
BYTE* load_tga( char *name, int *width, int *height )
{
	FILE	*fp;
	BYTE	*img;
	int		i, c, ident_len, bpp, up=false;
	int		ximg, yimg;

	fp = fopen( name, "rb" );
	if( !fp )
		return 0;

	ident_len = getc( fp );					// identification field length

	c = getc( fp );
	if( c != 0 )
		return 0;							// we don't want color map picture type

	c = getc( fp );
	if( c != 2 )
		return 0;							// we only want uncompressed RGBA pictures

	c = getc( fp ); c = getc( fp );
	c = getc( fp ); c = getc( fp );
	c = getc( fp );
	c = getc( fp ); c = getc( fp );
	c = getc( fp ); c = getc( fp );

	c = getc( fp ); ximg = c;
	c = getc( fp ); ximg+= c << 8;			// picture width (pixels)

	c = getc( fp ); yimg = c;
	c = getc( fp ); yimg+= c << 8;			// picture height (pixels)

	bpp = getc( fp );						// nb bits per pixel
	if( bpp!=24 && bpp!=32 )
		return 0;							// we only want 24 or 32 bits/pixel pictures

	c = getc( fp );							// flag
	if( c & 32 )
		up = true;

	for( i=0; i<ident_len; i++ )		// champ d'identification
		c = getc( fp );

	*width = ximg;
	*height= yimg;
	img    = new BYTE[ximg*yimg*(bpp/8)];

	// read graphic data
	if( !up )								// the picture is upside down
	{
		int	offset = (yimg-1)*ximg*(bpp/8);
		for( int y=0; y<yimg; y++ )
		{
			fread( img+offset, 1, ximg*(bpp/8), fp );
			offset -= ximg*(bpp/8);
		}
	}
	else									// the picture is not reversed
	{
		fread( img, 1, ximg*yimg*(bpp/8), fp );
	}
	fclose( fp );

	int	opp = bpp/8;

	for( i=0; i<ximg*yimg; i++ )
	{
		c            = img[i*opp];
		img[i*opp]   = img[i*opp+2];
		img[i*opp+2] = c;
	}

	return img;
}



//-------------------------------------------------------------------
// save_tga
//-------------------------------------------------------------------
// Save an uncompressed 24 bits picture to a Targa file format.
//-------------------------------------------------------------------
// Input :
//     name   (in): picture file name.
//     width  (in): picture width.
//     height (in): picture height.
//     img    (in): pointer to 24 bits RGB graphic data.
// Output:
//     TRUE if the picture has been correctly saved, else FALSE.
//-------------------------------------------------------------------
int save_tga( char *name, int width, int height, BYTE *img, int BPP )
{
	FILE	*fp;
	int		OPP = BPP / 8;

	fp = fopen( name, "wb" );
	if( !fp )
		return FALSE;

	fputc( 0, fp );							// identification field length
	fputc( 0, fp );							// no color map
	fputc( 2, fp );							// picture type = uncompressed RGB
	fputc( 0, fp ); fputc( 0, fp );			// 1st color index in the map
	fputc( 0, fp ); fputc( 0, fp );			// nb colors in the map
	fputc( 0, fp );							// nb bits/color in the map
	fputc( 0, fp ); fputc( 0, fp );			// x-coordinate of the origin
	fputc( 0, fp ); fputc( 0, fp );			// y-coordinate of the origin

	fputc( width & 255, fp );
	fputc( width>>8, fp );					// picture width

	fputc( height & 255, fp );
	fputc( height>>8, fp );					// picture height

	fputc(BPP, fp );						// nb bits per pixel
	fputc(0, fp );							// FLAG		0  32

	BYTE	c;
	for( int i=0; i<width*height*OPP; i+=OPP )
	{
		c        = img[i];
		img[i]   = img[i+2];
		img[i+2] = c;
	}
	fwrite( img, 1, width*height*OPP, fp );	// write graphic data

	fclose( fp );

	return TRUE;
}
