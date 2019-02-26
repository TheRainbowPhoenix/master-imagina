//-------------------------------------------------------------------
// tga.h
//-------------------------------------------------------------------
// Load and save 24 bits RGB pictures in Targa file format (.TGA).
//-------------------------------------------------------------------
// Created  : 05/12/1999
// Modified : 05/12/1999
//-------------------------------------------------------------------


#ifndef TGA_H
#define TGA_H

#define FALSE 0
#define TRUE 1

typedef unsigned char BYTE;

BYTE*	load_tga( char *name, int *width, int *height );
int		save_tga( char *name, int  width, int  height, BYTE *img, int BPP=24);

#endif
