#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "Vector2.h"
#include "list"
 
extern "C"{
#include <ncurses.h>
}

//#DECLARATION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS MOUSE

class Mouse {

public:

	/* contient le mask du dernier bouton relacher */
	static mmask_t released;
	static MEVENT event;

	enum Button { Left, Right, Middle };
	enum Wheel { ScrollUp, ScrollDown };

	static Vector2i get_position();

	static bool is_pressed(Button button);
	static bool is_released(Button button);
	static bool is_scrolling(Wheel wheel);
};

//#DECLARATION_END

#endif

