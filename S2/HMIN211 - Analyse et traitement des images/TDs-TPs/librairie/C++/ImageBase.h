/******************************************************************************
* ICAR_Library
*
* Fichier : ImageBase.h
*
* Description : Classe contennant quelques fonctionnalités de base
*
* Auteur : Mickael Pinto
*
* Mail : mickael.pinto@live.fr
*
* Date : Octobre 2012
*
*******************************************************************************/

#pragma once
#include <stdio.h>
#include <stdlib.h>

class ImageBase
{
	///////////// Enumerations
public:
	typedef enum { PLAN_R, PLAN_G, PLAN_B} PLAN;


	///////////// Attributs
protected:
	unsigned char *data;
	double *dataD;

	bool color;
	int height;
	int width;
	int nTaille;
	bool isValid;


	///////////// Constructeurs/Destructeurs
protected:
	void init();
	void reset();

public:
	ImageBase(void);
	ImageBase(int imWidth, int imHeight, bool isColor);
	~ImageBase(void);
	
	///////////// Methodes
protected:
	void copy(const ImageBase &copy);

public:
	int getHeight() { return height; };
	int getWidth() { return width; };
	int getTotalSize() { return nTaille; };
	int getValidity() { return isValid; };
	bool getColor() { return color; };
	unsigned char *getData() { return data; };


	void load(char *filename);
	bool save(char *filename);

	ImageBase *getPlan(PLAN plan);

	unsigned char *operator[](int l);
};

