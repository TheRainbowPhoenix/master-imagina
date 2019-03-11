#include "Term.h"
 
//#DEFINITION
////////////////////////////////////////////////// VARIABLES STATIQUES

TermScreen* TermScreen::m_screen = NULL;

////////////////////////////////////////////////// CONSTRUCTEURS

TermScreen::TermScreen() : Window(stdscr) {}

TermScreen::~TermScreen() {
	//delete m_screen;
}

////////////////////////////////////////////////// METHODES

TermScreen* TermScreen::get_instance(){
	if(!m_screen)
		m_screen = new TermScreen();

	return m_screen;
}

////////////////////////////////////////////////// VARIABLES STATIQUES

Window Term::scr; 

//int (*Term::scan)(const char*, ...) = scanw; 

////////////////////////////////////////////////// METHODES

void Term::init_curs() {

	setlocale(LC_ALL, "");

	//active les deplacements de la souris si un touche est pressé (-1003 tout le temps)
	
	std::string term(getenv("TERM"));

	std::size_t found = term.find("xterm");

	if(found != std::string::npos)
		setenv("TERM", "xterm-256color", 1); 	

	initscr();    // initialize curses

	init_color_pairs(); // initialize colors for the Term if compatible

	Term::scr = *TermScreen::get_instance();

	set_keypad_enabled(true);
	set_mouse_enabled(true);
	set_curs_vis(0); // 0 -> cursor is invisible.

	set_cooked_mode_enabled(false);
	set_echo_key_enabled(false);
	set_timeout(-1); //-1 -> wait indefinitly for input.
	
	mouseinterval(0); // 0 -> no click resolution.

	refresh();
}

void Term::end_curs(){
	endwin();
}

void Term::save_state(){
	def_prog_mode();// Save the ncurses win (!= savetty)
}

void Term::load_state(){
	reset_prog_mode(); // reset the ncurses win (!= resetty)
}

void Term::set_cooked_mode_enabled(bool enable){
	enable ? nocbreak() : cbreak();
}

void Term::set_echo_key_enabled(bool enable){
	enable ? echo() : noecho();
}

void Term::set_keypad_enabled(bool enable){
	enable ? keypad(stdscr, TRUE) : keypad(stdscr, FALSE);
}

void Term::set_mouse_enabled(bool enable){
	enable ? mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL) : mousemask((mmask_t)NULL, NULL);
}

void Term::set_timeout(int ms){
	timeout(ms);
}

void Term::set_curs_vis(int vis){
	curs_set(vis);
}

int Term::pop_input(){
	return getch();
}

void Term::push_input(int ch){
	ungetch(ch); 
}

int Term::read_input(){

	int ch = -1;
	
	/* si un bouton a été relacher alors on annule l'evenement de relachement */
	if(Mouse::released){
		Mouse::event.bstate = 0;
		Mouse::released = 0;
	}
	
	do
	{
		ch = getch();

		if(ch == KEY_MOUSE){

			MEVENT temp = Mouse::event;
			
			getmouse(&(Mouse::event));

			//Si l'evenement correspond a un deplacement du curseur (REPORT_MOUSE_POSITION = 010000...)
			if(Mouse::event.bstate & REPORT_MOUSE_POSITION)
				Mouse::event.bstate = temp.bstate; //On garde l'etat du bouton de l'evenement precedant.
			
			//Mouse::event.bstate |= temp.bstate;

		}
		else{
			Keyboard::input = ch;
		}
	}
	while(ch == KEY_MOUSE);

	return ch;
}

void Term::get_line(std::string& str){
	char cstr[1024] = "";
	getstr(cstr);
	str = cstr;
}

void Term::clear(){
	erase();
}

void Term::wait(int ms){
	napms(ms);
}

void Term::update(){
	doupdate();
}

//#DEFINITION_END

