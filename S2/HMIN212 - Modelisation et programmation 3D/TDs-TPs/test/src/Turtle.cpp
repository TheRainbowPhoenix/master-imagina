#include "Turtle.h"

//#DEFINITION
////////////////////////////////////////////////// CONSTRUCTEURS
 
Turtle::Turtle() : m_canvas(new Canvas()), rotation(), allocated(true), position() {}

Turtle::Turtle(Canvas* canvas) : m_canvas(canvas), rotation(), allocated(false), position() {}

Turtle::~Turtle(){
	if(allocated)
		delete m_canvas;
}

////////////////////////////////////////////////// METHODES

void Turtle::draw(float distance){

	Vector2f origin(position);

	move(distance);

	draw_line(*m_canvas, round(origin.x), round(origin.y), round(position.x), round(position.y));
}

void Turtle::move(float distance){

	float angle = to_radians(rotation);

	position += Vector2f(distance * cos(angle), distance * sin(angle));
}

void Turtle::turn(float angle){
	rotation += angle;
}

////////////////////////////////////////////////// FONCTIONS DEFINITION

float to_radians(float degree){
	return degree * M_PI / float(180);
}

void draw_polygon(Canvas& canvas, Vector2f center, int sides, float radius, float rotation){
	
	Turtle T(&canvas);

	float angle = float(360) / sides;
	float length = 2 * radius * sin(M_PI / float(sides));
	float inradius = radius * cos(M_PI / float(sides));
	Vector2f offset(length / 2.0, inradius);

	//canvas->set(round(center.x), round(center.y));

	T.position = center;
	T.turn(rotation);
 
	T.move(-length / 2.0);
	T.turn(90);
	T.move(-inradius);
	T.turn(-90);

	for(int i = 0 ; i < sides ; i++){
		T.draw(length);
		T.turn(angle);
	}
}

//#DEFINITION_END