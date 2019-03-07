#ifndef _COLOR_H_
#define _COLOR_H_

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED // pour utilisé les fonction wide character et cchar_t
#endif

extern "C"{
#include <ncursesw/curses.h>
}
#include <sstream>
#include <algorithm>
#include <cmath>

 
//#DECLARATION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ENUMERATION COLORUNIT

enum ColorUnit{
	DEFAULT = -1, //La couleur par default du terminal a l'initialisation de ncurses
	BLACK = COLOR_BLACK,
	RED = COLOR_RED,
	GREEN = COLOR_GREEN,
	YELLOW = COLOR_YELLOW,
	BLUE = COLOR_BLUE,
	MAGENTA = COLOR_MAGENTA,
	CYAN = COLOR_CYAN,
	WHITE = COLOR_WHITE
};

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS COLOR

class Color {

public:

	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color();
	Color(uint8_t red, uint8_t green, uint8_t blue);

	std::string to_string() const;

	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Yellow;
	static const Color Blue;
	static const Color Magenta;
	static const Color Cyan;
	static const Color White;
};

bool operator==(const Color& left, const Color& right);
bool operator!=(const Color& left, const Color& right);

Color operator+(const Color& left, const Color& right); 
Color operator-(const Color& left, const Color& right); 
Color operator*(const Color& left, const Color& right);

Color& operator+=(Color& left, const Color& right);
Color& operator-=(Color& left, const Color& right);
Color& operator*=(Color& left, const Color& right);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS COLORPAIR

class ColorPair {
	
public:

	short front;
	short back;

	ColorPair();
	ColorPair(chtype);
	ColorPair(ColorUnit front, ColorUnit back);

/* Renvoie la couleur de devant ou de derrière de la paire de couleur */
	Color get_front_color();
	Color get_back_color();

/* Renvoie le numéro correspondant à la paire de couleur composée de front et back */
	short to_pair() const;

/* Convertie en chtype la paire de couleur composée de front et back */
	operator chtype() const;

	static const ColorPair Default;
	static const ColorPair Black;
	static const ColorPair Red;
	static const ColorPair Green;
	static const ColorPair Yellow;
	static const ColorPair Blue;
	static const ColorPair Magenta;
	static const ColorPair Cyan;
	static const ColorPair White;
};

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS COLORSCALE

class ColorScale {

private:

	uint8_t m_scale;

public:	

	ColorScale();
	ColorScale(uint8_t scale);

/* Renvoie la couleur que represente le numero d'echelle "m_scale" */
	Color to_color() const;

/* Renvoie paire de couleur associer au numero d'echelle "m_scale" */
	short to_pair() const;

/* Convertie en chtype l'echelle de couleur */
	operator chtype() const;
};

////////////////////////////////////////////////// FONCTIONS DECLARATION

/* Initialise toutes les pairs de couleurs possible en fonction du terminal */
void init_color_pairs();

/* Definie de quoi serons composé les couleurs de l'echelle de couleur ColorScale allant de start a end */
void init_color_scale(Color start, Color end, short background = -1);

/* Change les valeurs de rouge vert et bleu associée à une couleur */
void set_color_rgb(ColorUnit color, short r, short g, short b);

/* Verifie si la couleur rgb correspond a un numero de couleur entre 0 et 7 */
bool is_color_0_to_7(uint8_t r, uint8_t g, uint8_t b);

/* Verifie si la couleur rgb correspond a un numero de couleur entre 8 et 7 */
bool is_color_8_to_15(uint8_t r, uint8_t g, uint8_t b);

/* Verifie si la couleur rgb correspond a un numero de niveaux de gris */
bool is_color_greyscale(uint8_t r, uint8_t g, uint8_t b);

/* Convertie la couleur rgb pour un terminal 8 couleurs */
int to_8_color_num(uint8_t r, uint8_t g, uint8_t b);

/* Convertie la couleur rgb pour un terminal 16 couleurs */
int to_16_color_num(uint8_t r, uint8_t g, uint8_t b);

/* Convertie la couleur rgb pour un terminal 88 couleurs */
int to_88_color_num(uint8_t r, uint8_t g, uint8_t b);

/* Convertie la couleur rgb pour un terminal 256 couleurs */
int to_256_color_num(uint8_t r, uint8_t g, uint8_t b);

/* Convertie le numero de la couleur en couleur rgb */
Color to_color_rgb(int color_num);
//#DECLARATION_END

#endif
