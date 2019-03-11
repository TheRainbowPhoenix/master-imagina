#ifndef _TURTLE_H_
#define _TURTLE_H_

#include "Canvas.h"

//#DECLARATION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS TURTLE
 
class Turtle
{
private:
	Canvas* m_canvas;
	float rotation;
	bool allocated;

public:
	Vector2f position;

	Turtle();
	Turtle(Canvas* canvas);
	~Turtle();

/* draw a line of "distance" pixels */
	void draw(float distance);

/* move the turtle of "distance" pixels */
	void move(float distance);

/* turn the turtle by "angle" degrees to the right if the angle is positive, or to the left if not. */
	void turn(float angle);
};

////////////////////////////////////////////////// FONCTIONS DECLARATION

/* Convert the angle "degree" in radians */
float to_radians(float degree);

/* Draw a polygon on the canvas */
void draw_polygon(Canvas& canvas, Vector2f center, int sides, float radius, float rotation);

//#DECLARATION_END

#endif
