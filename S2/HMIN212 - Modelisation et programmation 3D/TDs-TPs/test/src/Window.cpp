#include "Window.h"

//#DEFINITION
///////////////////////////////////////////////// CONSTRUCTEURS
 
Window::Window() : m_win() {}

Window::Window(WINDOW* win) : m_win(win) {}

Window::~Window() {
	delwin(m_win);
}

///////////////////////////////////////////////// METHODES

Window::operator WINDOW*(){
	return m_win;
}

Vector2i Window::get_dimension() const{
	int x = 0, y = 0;
	getmaxyx(m_win, y, x);
	return Vector2i(x, y);
}

Vector2i Window::get_curs_pos() const{
	int x = 0, y = 0;
	getyx(m_win, y, x);
	return Vector2i(x, y);
}

void Window::set_curs_pos(const Vector2i& pos){
	wmove(m_win, pos.y, pos.x);
}

Cell Window::get_cell(const Vector2i& coord) const{
	cchar_t c;
	mvwin_wch(m_win, coord.y, coord.x, &c);
	return Cell(c);
}

void Window::set_cell(const Vector2i& coord, const Cell& cell){
	cchar_t c = cell;

	//if(cell.color == ColorPair::Default)
		c.attr |= get_color_pair(); //affiche dans la couleur courante du terminal

	mvwadd_wch(m_win, coord.y, coord.x, &c);
	//mvwadd_wchnstr(m_win, coord.y, coord.x, &c, 1);
} 

void Window::set_cell(const Vector2i& coord, wint_t wchar, ColorPair color, Attr attr){
	set_cell(coord, Cell(wchar, color, attr));
}

Attr Window::get_attr() const{
	attr_t at;
	short pair; 
	
	wattr_get(m_win, &at, &pair, NULL);

	return Attr(at);
}

void Window::set_attr(Attr attr){ 
	attr_t at = attr;
	short pair = get_color_pair().to_pair();
	
	wattr_set(m_win, at, pair, NULL);
}

ColorPair Window::get_color_pair() const{
	attr_t at; 
	short pair; 

	wattr_get(m_win, &at, &pair, NULL);

	return COLOR_PAIR(pair);
}

void Window::set_color_pair(ColorPair color){
	wcolor_set(m_win, color.to_pair(), NULL);
}

void Window::set_on(chtype attr_color){
	wattron(m_win, attr_color);
}

void Window::set_off(chtype attr_color){
	wattroff(m_win, attr_color);
}

std::wstring Window::to_wstring() const{
	std::wstring data;
	wchar_t* wstr = new wchar_t[get_dimension().x];

	for(int i = 0 ; i < get_dimension().y ; i++){

		mvwinwstr(m_win, 0, 0, wstr);
		data += std::wstring(wstr); 
	}
	return data;
}

void Window::set_border(Cell left, Cell right, Cell up, Cell down, Cell upLeft, Cell upRight, Cell downLeft, Cell downRight){
	cchar_t l = left;
	cchar_t r = right;
	cchar_t u = up;
	cchar_t d = down;
	cchar_t ul = upLeft;
	cchar_t ur = upRight;
	cchar_t dl = downLeft;
	cchar_t dr = downRight;

	wborder_set(m_win, &l, &r, &u, &d, &ul, &ur, &dl, &dr);
}

//check colorpair
void Window::set_border(BorderType type, ColorPair color, Attr a){

	ColorPair c = (color == ColorPair::Default) ? get_color_pair() : color;
 
	switch (type)
	{
		case Empty:
		set_border(Cell(' ', c, a), Cell(' ', c, a), Cell(' ', c, a), Cell(' ', c, a), Cell(' ', c, a), Cell(' ', c, a), Cell(' ', c, a), Cell(' ', c, a));
		break;
		case Light:
		set_border(Cell(U_LIGHT_V, c, a), Cell(U_LIGHT_V, c, a), Cell(U_LIGHT_H, c, a), Cell(U_LIGHT_H, c, a), Cell(U_LIGHT_UL, c, a), Cell(U_LIGHT_UR, c, a), Cell(U_LIGHT_DL, c, a), Cell(U_LIGHT_DR, c, a));
		break;
		case LightArc:
		set_border(Cell(U_LIGHT_V, c, a), Cell(U_LIGHT_V, c, a), Cell(U_LIGHT_H, c, a), Cell(U_LIGHT_H, c, a), Cell(U_LIGHT_ARC_UL, c, a), Cell(U_LIGHT_ARC_UR, c, a), Cell(U_LIGHT_ARC_DL, c, a), Cell(U_LIGHT_ARC_DR, c, a));
		break;
		case Heavy:
		set_border(Cell(U_HEAVY_V, c, a), Cell(U_HEAVY_V, c, a), Cell(U_HEAVY_H, c, a), Cell(U_HEAVY_H, c, a), Cell(U_HEAVY_UL, c, a), Cell(U_HEAVY_UR, c, a), Cell(U_HEAVY_DL, c, a), Cell(U_HEAVY_DR, c, a));
		break;
		case Double:
		set_border(Cell(U_DOUBLE_V, c, a), Cell(U_DOUBLE_V, c, a), Cell(U_DOUBLE_H, c, a), Cell(U_DOUBLE_H, c, a), Cell(U_DOUBLE_UL, c, a), Cell(U_DOUBLE_UR, c, a), Cell(U_DOUBLE_DL, c, a), Cell(U_DOUBLE_DR, c, a));
		break;
		case BlockFull:
		set_border(Cell(U_BLOCK_FULL, c, a), Cell(U_BLOCK_FULL, c, a), Cell(U_BLOCK_U, c, a), Cell(U_BLOCK_D, c, a), Cell(U_BLOCK_FULL, c, a), Cell(U_BLOCK_FULL, c, a), Cell(U_BLOCK_FULL, c, a), Cell(U_BLOCK_FULL, c, a));
		break;
		case BlockLight:
		set_border(Cell(U_BLOCK_LIGHT, c, a), Cell(U_BLOCK_LIGHT, c, a), Cell(U_BLOCK_LIGHT, c, a), Cell(U_BLOCK_LIGHT, c, a), Cell(U_BLOCK_LIGHT, c, a), Cell(U_BLOCK_LIGHT, c, a), Cell(U_BLOCK_LIGHT, c, a), Cell(U_BLOCK_LIGHT, c, a));
		break;
		case BlockMedium:
		set_border(Cell(U_BLOCK_MEDIUM, c, a), Cell(U_BLOCK_MEDIUM, c, a), Cell(U_BLOCK_MEDIUM, c, a), Cell(U_BLOCK_MEDIUM, c, a), Cell(U_BLOCK_MEDIUM, c, a), Cell(U_BLOCK_MEDIUM, c, a), Cell(U_BLOCK_MEDIUM, c, a), Cell(U_BLOCK_MEDIUM, c, a));
		break;
		case BlockDark:
		set_border(Cell(U_BLOCK_DARK, c, a), Cell(U_BLOCK_DARK, c, a), Cell(U_BLOCK_DARK, c, a), Cell(U_BLOCK_DARK, c, a), Cell(U_BLOCK_DARK, c, a), Cell(U_BLOCK_DARK, c, a), Cell(U_BLOCK_DARK, c, a), Cell(U_BLOCK_DARK, c, a));
		break;
		case Dash: 
		set_border(Cell('|', c, a), Cell('|', c, a), Cell('-', c, a), Cell('-', c, a), Cell('+', c, a), Cell('+', c, a), Cell('+', c, a), Cell('+', c, a));
		break;
		case Diamond:
		set_border(Cell(U_DIAMOND, c, a), Cell(U_DIAMOND, c, a), Cell(U_DIAMOND, c, a), Cell(U_DIAMOND, c, a), Cell(U_DIAMOND, c, a), Cell(U_DIAMOND, c, a), Cell(U_DIAMOND, c, a), Cell(U_DIAMOND, c, a));
		break;
		case Point:
		set_border(Cell(U_POINT, c, a), Cell(U_POINT, c, a), Cell(U_POINT, c, a), Cell(U_POINT, c, a), Cell(U_POINT, c, a), Cell(U_POINT, c, a), Cell(U_POINT, c, a), Cell(U_POINT, c, a));
		break;
		default:
		set_border(Cell(U_LIGHT_V, c, a), Cell(U_LIGHT_V, c, a), Cell(U_LIGHT_H, c, a), Cell(U_LIGHT_H, c, a), Cell(U_LIGHT_UL, c, a), Cell(U_LIGHT_UR, c, a), Cell(U_LIGHT_DL, c, a), Cell(U_LIGHT_DR, c, a));
		break;
	}
}
//check colorpair
void Window::fill(const Cell& cell){
	cchar_t c = cell;

	if(cell.color == ColorPair::Default)
		c.attr |= get_color_pair(); //affiche dans la couleur courante du terminal

	wbkgrnd(m_win, &c);
}

//check colorpair
void Window::fill(ColorPair color, Attr attr){
	Cell cell;
	cell.color = color;
	cell.attr = attr;

	fill(cell);
}

Cell Window::get_background() const{
	cchar_t c;

	wgetbkgrnd(m_win, &c);

	return c;
}

void Window::set_background(const Cell& cell){
	cchar_t c = cell;

	wbkgrndset(m_win, &c);
}

//check colorpair
void Window::set_background(ColorPair color, Attr attr){
	Cell cell;
	cell.color = color == ColorPair::Default ? get_color_pair() : color;
	cell.attr = attr;

	set_background(cell);
} 

/*

Color Window::get_background_color() const{
	return m_background;
}

void Window::set_background_color(const Color& color){
	m_background = color;
	assume_default_colors(0, color.to_number());
}
*/
void Window::clear(){
	/* faster than wclear */
	werase(m_win);
} 

void Window::clear(const IntRect& zone){
	Cell back = get_background();

	for(int j = zone.y ; j < zone.height ; j++){

		for (int i = zone.x; i < zone.width; i++){
			set_cell(Vector2i(i, j), back);
		}
	}
}

void Window::display(){
	if(is_pad(m_win)){
		int h, w;
		getmaxyx(stdscr, h, w);
		pnoutrefresh(m_win, 0, 0, 0, 0, h - 1, w -1);
	}
	else
		wnoutrefresh(m_win);
} 

void Window::refresh(){
	if(is_pad(m_win)){
		int h, w;
		getmaxyx(stdscr, h, w);
		prefresh(m_win, 0, 0, 0, 0, h - 1, w -1);
	}
	else
		wrefresh(m_win);
}

//#DEFINITION_END
