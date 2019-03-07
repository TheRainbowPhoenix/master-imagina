#include "Mouse.h"

//#DEFINITION
///////////////////////////////////////////////// VARIABLES STATIQUES
 
MEVENT Mouse::event;
mmask_t Mouse::released;

///////////////////////////////////////////////// METHODES

Vector2i Mouse::get_position() {
	return Vector2i(event.x, event.y);
}

bool Mouse::is_pressed(Button button) {

	switch(button)
	{
		case Mouse::Left:
		return event.bstate & BUTTON1_PRESSED;
		break;
		case Mouse::Right:
		return event.bstate & BUTTON3_PRESSED;
		break;
		case Mouse::Middle:
		return event.bstate & BUTTON2_PRESSED;
		break;
		default:
		return false;
		break;
	}
}

bool Mouse::is_released(Button button) {

	mmask_t input_released = 0;

	switch (button)
	{
		case Mouse::Left:
		input_released = event.bstate & BUTTON1_RELEASED;
		break;
		case Mouse::Right:
		input_released = event.bstate & BUTTON3_RELEASED;
		break;
		case Mouse::Middle:
		input_released = event.bstate & BUTTON2_RELEASED;
		break;
		default:
		break;
	}
	
	if(input_released)
		released = input_released;

	return input_released;
}

bool Mouse::is_scrolling(Wheel wheel) {

	switch (wheel)
	{
		case Mouse::ScrollUp:
		return event.bstate & BUTTON4_PRESSED;
		break;
		case Mouse::ScrollDown:
		return false;//return event.bstate & BUTTON5_PRESSED; //pas reperer par le compilateur
		break;
		default:
		return false;
		break;
	}
}

//#DEFINITION_END
