#ifndef _WINDOW_H_
#define _WINDOW_H_
 
#include "Rect.h"
#include "Cell.h"

//#PREPROCESSING
#include <string>
//#PREPROCESSING_END

//#DECLARATION
/* definition de quelques characters unicodes interresants */

#define U_LIGHT_V  			L'\u2502'
#define U_LIGHT_H  			L'\u2500'
#define U_LIGHT_UL 			L'\u250C'
#define U_LIGHT_UR 			L'\u2510'
#define U_LIGHT_DL 			L'\u2514'
#define U_LIGHT_DR 			L'\u2518'

#define U_LIGHT_ARC_UL 		L'\u256D'
#define U_LIGHT_ARC_UR 		L'\u256E'
#define U_LIGHT_ARC_DL 		L'\u2570'
#define U_LIGHT_ARC_DR 		L'\u256F'

#define U_HEAVY_V  			L'\u2503'
#define U_HEAVY_H  			L'\u2501'
#define U_HEAVY_UL 			L'\u250F'
#define U_HEAVY_UR 			L'\u2513'
#define U_HEAVY_DL 			L'\u2517'
#define U_HEAVY_DR 			L'\u251B'

#define U_DOUBLE_V  		L'\u2551'
#define U_DOUBLE_H  		L'\u2550'
#define U_DOUBLE_UL 		L'\u2554'
#define U_DOUBLE_UR 		L'\u2557'
#define U_DOUBLE_DL 		L'\u255A'
#define U_DOUBLE_DR 		L'\u255D'

#define U_BLOCK_FULL 		L'\u2588'
#define U_BLOCK_U 			L'\u2580'
#define U_BLOCK_D 			L'\u2584'

#define U_BLOCK_LIGHT 		L'\u2591'
#define U_BLOCK_MEDIUM 		L'\u2592'
#define U_BLOCK_DARK 		L'\u2593'

#define U_DIAMOND 			L'\u25C6'

#define U_POINT 			L'\u25CF'
#define U_CIRCLE 			L'\u25EF'

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS WINDOW

/* Cette class permet d'englober la structure WINDOW* definie dans ncurses. */ 

class Window {

protected:

	WINDOW* m_win;
	Window(WINDOW* win);

public:

	enum BorderType {
		Empty,
		Light,
		LightArc,
		Heavy,
		Double,
		BlockFull,
		BlockLight,
		BlockMedium,
		BlockDark,
		Dash,
		Diamond,
		Point
	};

	Window();
	virtual ~Window();

	operator WINDOW*();

/* return the dimensions of the Window (col, row) (stdscr ne peut pas  être resize et rien ne peut etre plus grand que stdscr a par les pads) */
	Vector2i get_dimension() const;
	//void set_dimension(const Vector2i& dim);

/* return/set the cursor position relative to up-left corner of the Window (col, row) */
	Vector2i get_curs_pos() const;
	void set_curs_pos(const Vector2i& coord);

/* return/set a Window's cell relative to up-left corner of the window (col, row) */
	Cell get_cell(const Vector2i& coord) const;
	void set_cell(const Vector2i& coord, const Cell& cell);
	void set_cell(const Vector2i& coord, wint_t wchar, ColorPair color = ColorPair::Default, Attr attr = Attr::Normal);

/* return/set the Window's attribute when writing */
	Attr get_attr() const; //wattr_set
	void set_attr(Attr); //wattr_get

/* return/set the Window's color when writing */
	ColorPair get_color_pair() const;
	void set_color_pair(ColorPair);

/* set on/off a Window's attribute and/or Color can be or'd with | */
	void set_on(chtype attr_color);
	void set_off(chtype attr_color);

/* return the data of the Window in one wide string */
	std::wstring to_wstring() const;

/* draw a border around the window */
	void set_border(BorderType type, ColorPair c = ColorPair::Default, Attr  a = Attr::Normal); //verifier pas de problème avec assignation par default
	void set_border(Cell left, Cell right, Cell up, Cell down, Cell upLeft, Cell upRight, Cell downLeft, Cell downRight);

/* fill the windows background whith a specific Cell, Color and/or Attribute */
	void fill(const Cell&);
	void fill(ColorPair color, Attr attr = Attr::Normal); 

/* return/set the windows background whith a specific Cell, Color and/or Attribute */
	Cell get_background() const;
	void set_background(const Cell&);
	void set_background(ColorPair color, Attr attr = Attr::Normal); 

/* Return/set the window's background color 
	Color get_background_color() const;
	void set_background_color(const Color& color);

*/

/* copy the content of another Window */
	//void copy(const Window&, const IntRect&, const Vector2i&, bool convert_attr = true);// copywin;

/* clear the Window */
	void clear();
	void clear(const IntRect& zone);

/* display the window to the virtual screen (use Term::update to display the virtual screen) */
	virtual void display(); //pnoutrefresh;

/* display the window directly to the realscreen without optimisation */
	void refresh();
};

//#DECLARATION_END

#endif
