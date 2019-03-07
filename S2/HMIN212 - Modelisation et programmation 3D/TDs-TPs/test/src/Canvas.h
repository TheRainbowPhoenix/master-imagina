#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "Term.h"

//#DECLARATION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS CANVAS
 
class Canvas : public Window {

protected:

	Vector2i m_pixel_size;

public:

/* initialise un canvas qui fait la taille de l'ecran */
	Canvas();
	Canvas(int w, int h);
	virtual ~Canvas();

/* place un pixel a la position "point" */
	void set(int x, int y);
	void set(const Vector2i& point);
	
/* enleve le pixel qui ce trouve a la position "point" */
	void unset(int x, int y);
	void unset(const Vector2i& point);

/* place un pixel a la position "point" du canvas si il n'y est pas deja, sinon l'enleve */
	void toggle(int x, int y);
	void toggle(const Vector2i& point);

/* Renvoie les dimension du canvas en nombre de pixel (width, height) */
	Vector2i get_size() const;

/* affiche entièrement le canvas à la position (0, 0) */
	virtual void display();

/* affiche entièrement le canvas à la position "position" */
	void display(const Vector2i& position);

/* affiche à la position "position", le rectangle "view" représentant une vue/camera sur le canvas  */
	void display(const Vector2i& position, const IntRect& view);

/* verifie un pixel existe à la position (x, y) */
	bool is_set(int x, int y);
	bool is_set(const Vector2i& point);

/* verifie si la position (x, y) est à l'interieur du canvas */
	bool is_in(int x, int y);
	bool is_in(const Vector2i& point);
};

///////////////////////////////////////////////// FONCTIONS DECLARATION

/* Converti la position du pixel (x, y) en coordonnée de cellule (row, col) */
Vector2i map_pixel_to_cell(unsigned int x, unsigned int y);
Vector2i map_pixel_to_cell(const Vector2i& point);

/* Converti les coordonnées de la cellule (row, col) en position de pixel (x, y) */
Vector2i map_cell_to_pixel(unsigned int x, unsigned int y);
Vector2i map_cell_to_pixel(const Vector2i& point);

/* verifie si le wide character "wch" est du braille */
bool is_braille(wint_t wch);

/* dessine une ligne sur le canvas allant du point p1 au point p2 */
void draw_line(Canvas& canvas, int x1, int y1, int x2, int y2);
void draw_line(Canvas& canvas, const Vector2i& p1, const Vector2i p2);

//#DECLARATION_END

#endif
