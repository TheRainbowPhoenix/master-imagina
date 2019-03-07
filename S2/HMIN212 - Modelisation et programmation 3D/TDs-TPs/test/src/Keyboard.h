#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//#DECLARATION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS KEYBOARD
 
class Keyboard {

public:
	static int input;
	
	enum Key {
		A, B, C,
		D, E, F, G,
		H, I, J, K,
		L, M, N, O,
		P, Q, R, S,
		T, U, V, W,
		X, Y, Z, Num0,
		Num1, Num2, Num3, Num4,
		Num5, Num6, Num7, Num8,
		Num9, Escape, SemiColon, 
		Comma, Period, Equal, Space, Return,
		BackSpace, Tab, Left, Right, Up, Down,
		Numpad0, Numpad1, Numpad2, Numpad3,
		Numpad4, Numpad5, Numpad6, Numpad7,
		Numpad8, Numpad9, F1, F2,
		F3, F4, F5, F6,
		F7, F8, F9, F10,
		F11, F12,
	};

	static bool is_pressed(Key key);
};

//#DECLARATION_END

#endif
