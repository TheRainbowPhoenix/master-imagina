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
	int getHeight() const { return height; };
	int getWidth() const { return width; };
	int getTotalSize() const { return nTaille; };
	int getValidity() const { return isValid; };
	bool getColor() const { return color; };
	unsigned char *getData() { return data; };

	void load(const char *filename);
	bool save(const char *filename);

	ImageBase* getPlan(PLAN plan);
	void setPlan(PLAN plan, const ImageBase* plan_data);

	unsigned char *operator[](int l);
};

/* assigne les données du plan à l'image coloré, dont la taille doit être 3 fois superieur à celle du plan */
void set_plan_R(unsigned char *image_data, unsigned char *plan_R, int taille_plan);
void set_plan_G(unsigned char *image_data, unsigned char *plan_G, int taille_plan);
void set_plan_B(unsigned char *image_data, unsigned char *plan_B, int taille_plan);