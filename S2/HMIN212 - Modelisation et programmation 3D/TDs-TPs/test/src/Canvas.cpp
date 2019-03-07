#include "Canvas.h"

//#DEFINITION
////////////////////////////////////////////////// VARIABLES STATIQUES
 
 
const wint_t braille_char_offset = 0x2800;

const int pixel_map[4][2] = { {0x01, 0x08}, 
							  {0x02, 0x10},
							  {0x04, 0x20},
							  {0x40, 0x80} };

////////////////////////////////////////////////// CONSTRUCTEURS

Canvas::Canvas(){
	int h = 0, w = 0;

	getmaxyx(stdscr, h, w);

	m_win = newpad(h, w);

	/* converti le nombre de cellules en nombre de pixel */
	m_pixel_size = Vector2i(w * 2, h * 4);
}

Canvas::Canvas(int w, int h) : m_pixel_size(w, h) {		
	/* verification pour ne pas qu'il nous manque de cellules */
	w = w % 2 != 0 ? w + 2 : w;	 
	h = h % 4 != 0 ? h + 4 : h;

	/* converti le nombre de pixels en nombre de colonnes/lignes */
	m_win = newpad(h / 4, w / 2);
} 

Canvas::~Canvas(){}

////////////////////////////////////////////////// METHODES

void Canvas::set(int x, int y){
	set(Vector2i(x, y));
}

void Canvas::set(const Vector2i& point){

	if(is_in(point)){

		cchar_t c;

		Vector2i cell_coord(map_pixel_to_cell(point));
		
		mvwin_wch(m_win, cell_coord.y, cell_coord.x, &c);
		
		if(!is_braille(c.chars[0]))
			c.chars[0] = braille_char_offset;

		//y % 4 == y & 3 / x % 2 == x & 1
		c.chars[0] |= pixel_map[point.y & 3][point.x & 1];

		c.attr &= ~A_COLOR;

		mvwadd_wch(m_win, cell_coord.y, cell_coord.x, &c);
	}
}

void Canvas::unset(int x, int y){
	unset(Vector2i(x, y));
}

void Canvas::unset(const Vector2i& point){

	if(is_in(point)){

		Vector2i cell_coord(map_pixel_to_cell(point));

		wint_t cell = get_cell(cell_coord).character;

		if(!is_braille(cell))
			return;

		cell &= ~pixel_map[point.y % 4][point.x % 2];

		set_cell(cell_coord, cell);
	}
}

void Canvas::toggle(int x, int y){

	toggle(Vector2i(x, y));
}

void Canvas::toggle(const Vector2i& point){
	if(is_set(point))
		unset(point);
	else
		set(point);
}

Vector2i Canvas::get_size() const {
	return m_pixel_size;
}

void Canvas::display(){
	Window::display();
}

void Canvas::display(const Vector2i& position){
	display(position, IntRect(Vector2i::zero, get_dimension()));
} 

void Canvas::display(const Vector2i& position, const IntRect& view){
	int h, w;
	getmaxyx(stdscr, h, w);

	int maxY = position.y + view.height - 1;
	int maxX = position.x + view.width - 1;
	int maxTermY = h - 1;
	int maxTermX = w - 1;

	int pminrow = position.y >= 0 ? view.y : view.y - position.y;
	int pmincol = position.x >= 0 ? view.x : view.x - position.x;
	int sminrow = view.y >= 0 ? position.y : position.y - view.y;
	int smincol = view.x >= 0 ? position.x : position.x - view.x;
	int smaxrow = maxTermY >= maxY ? maxY : maxY - (maxY - maxTermY);
	int smaxcol = maxTermX >= maxX ? maxX : maxX - (maxX - maxTermX);

	pnoutrefresh(m_win, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol);
}

bool Canvas::is_set(int x, int y){
	return is_set(Vector2i(x, y));
}

bool Canvas::is_set(const Vector2i& point){
	wint_t cell = get_cell(map_pixel_to_cell(point)).character;
	return is_braille(cell) && cell & pixel_map[point.y % 4][point.x % 2];
}

bool Canvas::is_in(int x, int y){
	return is_in(Vector2i(x, y));
}

bool Canvas::is_in(const Vector2i& point){
	Vector2i size = get_size();
	return point.x >= 0 && point.y >= 0 && point.x < size.x && point.y < size.y;
}

////////////////////////////////////////////////// FONCTIONS DEFINITION

Vector2i map_pixel_to_cell(unsigned int x, unsigned int y){
	//x / 2 == x >> 1 && y / 4 == y >> 2 
	return Vector2i(x >> 1, y >> 2);
}

Vector2i map_pixel_to_cell(const Vector2i& point){
	return map_pixel_to_cell(point.x, point.y);
}

Vector2i map_cell_to_pixel(unsigned int col, unsigned int row){
	//x * 2 == x << 1 && y * 4 == y << 2 
	return Vector2i(col << 1, row << 2);
}

Vector2i map_cell_to_pixel(const Vector2i& cell_coord){
	return map_cell_to_pixel(cell_coord.x, cell_coord.y);
}

bool is_braille(wint_t cell){
	return cell >= 0x2800 && cell <= 0x28FF;
}

/* version plus simple de draw_line mais un peu moins precise */
void draw_line1(Canvas& canvas, int x1, int y1, int x2, int y2){

	int xdiff = std::max(x1, x2) - std::min(x1, x2);
	int ydiff = std::max(y1, y2) - std::min(y1, y2);
	int xdir = x1 <= x2 ? 1 : -1;
	int ydir = y1 <= y2 ? 1 : -1;

	int r = std::max(xdiff, ydiff);

	for (int i = 0; i < r + 1; ++i){
		int x = x1;
		int y = y1;

		if (ydiff)
			y += (float(i) * ydiff) / r * ydir;
		if (xdiff)
			x += (float(i) * xdiff) / r * xdir;

		canvas.set(x, y);
	}
}

void draw_line(Canvas& canvas, const Vector2i& p1, const Vector2i p2){
	draw_line(canvas, p1.x, p1.y, p2.x, p2.y);
}

void draw_line(Canvas& canvas, int x1, int y1, int x2, int y2){

	int dx = x2 - x1;
	int dy = y2 - y1;

	if(dx != 0){
		if(dx > 0){
			if(dy != 0){
				if(dy > 0){
					// vecteur oblique dans le 1er quadran
					
					if(dx >= dy){
						// vecteur diagonal ou oblique proche de l’horizontale dans le 1er octant

						float e = dx ;
						dx = e * 2;
						dy *= 2;

						while(1){
							canvas.set(x1, y1);

							if((x1++) == x2)
								break;

							if((e -= dy) < 0){
								y1++;
								e += dx;
							}
						}
					}
					else{
						// vecteur oblique proche de la verticale, dans le 2d octant
						
						float e = dy;
						dy = e * 2;
						dx *= 2;

						while(1){
							canvas.set(x1, y1);

							if((y1++) == y2)
								break;

							if((e -= dx) < 0){
								x1++;
								e += dy;
							}
						}
					}
				}
				else{
					// vecteur oblique dans le 4e cadran
					
					if(dx >= -dy){
						// vecteur diagonal ou oblique proche de l’horizontale, dans le 8e octant
						float e = dx;
						dx = e * 2;
						dy *= 2;

						while(1){
							canvas.set(x1, y1);

							if((x1++) == x2)
								break;

							if((e += dy) < 0){
								y1--;
								e += dx;
							}
						}
					}
					else{
						// vecteur oblique proche de la verticale, dans le 7e octant
						
						float e = dy;
						dy = e * 2;
						dx *= 2;

						while(1){
							canvas.set(x1, y1);

							if((y1--) == y2)
								break;

							if((e += dx) > 0){
								x1++;
								e += dy;
							}
						}
					}
				}
			}
			else{
				// vecteur horizontal vers la droite
				
				do{
					canvas.set(x1, y1);
				}while(!((x1++) == x2));
			}
		}
		else{
			if(dy != 0){
				if(dy > 0){
					// vecteur oblique dans le 2d quadran
					
					if(-dx >= dy){
						// vecteur diagonal ou oblique proche de l’horizontale, dans le 4e octant
						
						float e = dx;
						dx = e * 2;
						dy *= 2;

						while(1){
							canvas.set(x1, y1);

							if((x1--) == x2)
								break;

							if((e += dy) >= 0){
								y1++;
								e += dx;
							}
						}
					}
					else{
						// vecteur oblique proche de la verticale, dans le 3e octant
						
						float e = dy;
						dy = e * 2;
						dx *= 2;

						while(1){
							canvas.set(x1, y1);

							if((y1++) == y2)
								break;

							if((e += dx) <= 0){
								x1--;
								e += dy;
							}
						}
					}
				}
				else{
					// vecteur oblique dans le 3e cadran
					
					if(dx <= dy){
						// vecteur diagonal ou oblique proche de l’horizontale, dans le 5e octant
						
						float e = dx;
						dx = e * 2;
						dy *= 2;

						while(1){
							canvas.set(x1, y1);

							if((x1--) == x2)
								break;

							if((e -= dy) >= 0){
								y1--;
								e += dx;
							}
						}
					}
					else{
						// vecteur oblique proche de la verticale, dans le 6e octant
						
						float e = dy;
						dy = e * 2;
						dx *= 2;

						while(1){
							canvas.set(x1, y1);

							if((y1--) == y2)
								break;

							if((e -= dx) >= 0){
								x1--;
								e += dy;
							}
						}
					}
				}
			}
			else{
				// vecteur horizontal vers la gauche
				
				do{
					canvas.set(x1, y1);
				}while(!((x1--) == x2));
			}
		}
	}
	else{
		if(dy != 0){
			if(dy > 0){
				// vecteur vertical croissant
				
				do{
					canvas.set(x1, y1);
				}while(!((y1++) == y2));
			}
			else{
				// vecteur vertical décroissant
				
				do{
					canvas.set(x1, y1);
				}while(!((y1--) == y2));
			}
		}
	}
}

//#DEFINITION_END