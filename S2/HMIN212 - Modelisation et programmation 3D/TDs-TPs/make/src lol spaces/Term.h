#ifndef _TERM_H_
#define _TERM_H_
 
#include "Window.h"
#include "Mouse.h"
#include "Keyboard.h"

//#PREPROCESSING
#include <cstdlib>
//#PREPROCESSING_END

//#DECLARATION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS TERMSCREEN

class TermScreen : public Window {

private:

	static TermScreen* m_screen;

	TermScreen();
	TermScreen(const TermScreen&);
	TermScreen& operator=(const TermScreen&);

public:

	~TermScreen();

	static TermScreen* get_instance();
};

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS TERM

class Term {

public:
	
	static Window scr;

	static void init_curs();
	static void end_curs();

	static void save_state();
	static void load_state();

/* all the terminal options to activate/set */
	static void set_cooked_mode_enabled(bool enable);
	static void set_echo_key_enabled(bool enable);
	static void set_keypad_enabled(bool enable);
	static void set_mouse_enabled(bool enable);

	static void set_timeout(int ms);
	static void set_curs_vis(int vis);

/* pop/push an input for the next call to the input queue */
	static int pop_input();
	static void push_input(int input);

/* Read inputs from mouse and keyboard */
	static int read_input();

/* Renvoie un chaine de character taper par l'untilisateur dans la chaine "str" */
	static void get_line(std::string& str);

/* Pointeur vers la fonction scanw */
	//static int (*scan)(const char*, ...);

/* clear the screen */ 
	static void clear();

/* attend jusqu'à ce que "ms" microseconds soit passées */ 
	static void wait(int ms);

/* Display the virtual screen to the real screen */
	static void update();
};

//#DECLARATION_END

#endif