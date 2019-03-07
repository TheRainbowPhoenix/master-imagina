#include "Color.h"

//#DEFINITION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS COLOR

////////////////////////////////////////////////// CONSTRUCTEURS

Color::Color() : r(), g(), b() {}

Color::Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}

////////////////////////////////////////////////// METHODES

std::string Color::to_string() const{
	std::stringstream ss;
	ss << "(" << (int)r << ", " << (int)g << ", " << (int)b << ")";
	return ss.str();
}

////////////////////////////////////////////////// VARIABLES STATIQUES

const Color Color::Black(0, 0, 0);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Yellow(255, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);
const Color Color::White(255, 255, 255);

////////////////////////////////////////////////// OPERATEURS

bool operator==(const Color& left, const Color& right){
	return left.r == right.r && left.g == right.g && left.b == right.b; 
}

bool operator!=(const Color& left, const Color& right){
	return !(left == right);
}

//faire attention au cas ou l'addition depasse 255
Color operator+(const Color& left, const Color& right){
	return Color(std::min(((int)left.r + right.r) / 2, 255), std::min(((int)left.g + right.g) / 2, 255), std::min(((int)left.b + right.b) / 2, 255));
}

//faire attention au cas ou a soustraction est negative
Color operator-(const Color& left, const Color& right){
	return Color(std::max(((int)left.r - right.r) / 2, 0), std::max(((int)left.g - right.g) / 2, 0), std::max(((int)left.b - right.b) / 2, 0));
}

//faire attention au cas ou la multiplication depasse 255
Color operator*(const Color& left, const Color& right){
	return Color(((int)left.r * right.r) / 255, ((int)left.g * right.g) / 255, ((int)left.b * right.b) / 255);
}

Color& operator+=(Color& left, const Color& right){
	left.r = std::min(((int)left.r + right.r) / 2, 255);
	left.g = std::min(((int)left.g + right.g) / 2, 255);
	left.b = std::min(((int)left.b + right.b) / 2, 255);
	return left;
}

Color& operator-=(Color& left, const Color& right){
	left.r = std::max(((int)left.r - right.r) / 2, 0);
	left.g = std::max(((int)left.g - right.g) / 2, 0);
	left.b = std::max(((int)left.b - right.b) / 2, 0);
	return left;
}
 //restraindre a 255
Color& operator*=(Color& left, const Color& right){
	left.r = ((int)left.r * right.r) / 255;
	left.g = ((int)left.g * right.g) / 255;
	left.b = ((int)left.b * right.b) / 255;
	return left;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS COLORPAIR

////////////////////////////////////////////////// CONSTRUCTEURS

ColorPair::ColorPair() : front(DEFAULT), back(DEFAULT) {}

ColorPair::ColorPair(chtype c){

	short f = 0, b = 0;

	pair_content(PAIR_NUMBER(c), &f, &b);

	front = f;
	back = b;
}

ColorPair::ColorPair(ColorUnit front, ColorUnit back) {
	this->front = front;
	this->back = back;
}

////////////////////////////////////////////////// METHODES

Color ColorPair::get_front_color() {

	return to_color_rgb(front);
}

Color ColorPair::get_back_color() {
	return to_color_rgb(back);
}

short ColorPair::to_pair() const {
	return (back > 7 || front > 7) ? front : (back + 1) * 9 + (front + 1);
}

ColorPair::operator chtype() const {
	return COLOR_PAIR(to_pair());
}

////////////////////////////////////////////////// VARIABLE STATIQUES

const ColorPair ColorPair::Default(DEFAULT, DEFAULT);
const ColorPair ColorPair::Black(BLACK, DEFAULT);
const ColorPair ColorPair::Red(RED, DEFAULT);
const ColorPair ColorPair::Green(GREEN, DEFAULT);
const ColorPair ColorPair::Yellow(YELLOW, DEFAULT);
const ColorPair ColorPair::Blue(BLUE, DEFAULT);
const ColorPair ColorPair::Magenta(MAGENTA, DEFAULT);
const ColorPair ColorPair::Cyan(CYAN, DEFAULT);
const ColorPair ColorPair::White(WHITE, DEFAULT);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS COLORSCALE

////////////////////////////////////////////////// CONSTRUCTEURS

ColorScale::ColorScale() : m_scale() {}

ColorScale::ColorScale(uint8_t scale) : m_scale(scale) {}

////////////////////////////////////////////////// METHODES

Color ColorScale::to_color() const {
	
	short front;
	short back;

	short r;
	short g;
	short b;

	pair_content(to_pair(), &front, &back);
	color_content(front, &r, &g, &b);

	return Color(((int)r * 255) / 1000, ((int)g * 255) / 1000, ((int)b * 255) / 1000); //Color(round(((int)r * 255) / 1000.0f), round(((int)g * 255) / 1000.0f), round(((int)b * 255) / 1000.0f));
}

short ColorScale::to_pair() const {
	return 81 + ((int)m_scale * 174) / 255; //round(((int)m_scale * 174) / 255.0f);
}

ColorScale::operator chtype() const {
	return COLOR_PAIR(to_pair());
}

////////////////////////////////////////////////// FONCTIONS DEFINITION

void init_color_pairs(){

	if (has_colors()) {

		start_color();

		use_default_colors(); // -> COLOR_PAIR(0) = Default Color (est un extension de ncurses)
		
		//Les 8 couleurs par default du terminal plus la couleur de base du terminal
		int max_color = 9;

		for(int b = 0 ; b < max_color ; ++b){

			for (int f = 0 ; f < max_color ; ++f){
				init_pair((b * max_color) + f, f - 1, b - 1);
			}
		}

		init_color_scale(Color(0, 0, 0), Color(255, 255, 255));
	}
}

void init_color_scale(Color start, Color end, short background){

	if(COLORS >= 256){

		short min_r = round(((int)start.r * 1000) / 255.0f);
		short min_g = round(((int)start.g * 1000) / 255.0f);
		short min_b = round(((int)start.b * 1000) / 255.0f);

		short max_r = round(((int)end.r * 1000) / 255.0f);
		short max_g = round(((int)end.g * 1000) / 255.0f);
		short max_b = round(((int)end.b * 1000) / 255.0f);

		short max_color_scale = 175;
		
		short r_l = max_r - min_r;
		short g_l = max_g - min_g;
		short b_l = max_b - min_b;

		for (int i = 0; i < max_color_scale; ++i) {

			init_color(81 + i, min_r + (i * r_l) / (max_color_scale - 1), min_g + (i * g_l) / (max_color_scale - 1), min_b + (i * b_l) / (max_color_scale - 1));
			init_pair(81 + i, 81 + i, background);
		}
	}
}

bool is_color_0_to_7(uint8_t r, uint8_t g, uint8_t b){
	return (r == 192 && r == g && r == b) || ((r == 0 || r == 128) && (g == 0 || g == 128) && (b == 0 || b == 128));
}

bool is_color_8_to_15(uint8_t r, uint8_t g, uint8_t b){
	return !(r == 0 && g == 0 && b == 0) && (((r == 128 && r == g && r == b) || ((r == 0 || r == 255) && (g == 0 || g == 255) && (b == 0 || b == 255))));
}

bool is_color_greyscale(uint8_t r, uint8_t g, uint8_t b){
	return (r >= 8 && r <= 238) && r == g && r == b;
}

int to_8_color_num(uint8_t r, uint8_t g, uint8_t b){
	uint8_t red = r > 0 ? 1 : 0; 
	uint8_t green = g > 0 ? 1 : 0; 
	uint8_t blue = b > 0 ? 1 : 0; 

	return red + 2 * green + 4 * blue;
}

int to_16_color_num(uint8_t r, uint8_t g, uint8_t b){

	if(is_color_8_to_15(r, g, b)){
		if(r == 128)
			return 8; 
		else
			return 8 + to_8_color_num(r, g, b);
	}
	else
		return to_8_color_num(r, g, b);
}

/* verifier les echelles de gris */
int to_88_color_num(uint8_t r, uint8_t g, uint8_t b){

	if(is_color_greyscale(r, g, b)){
		return 80 + (r / 32); 
	}
	else{
		uint8_t red = (r / 64); 
		uint8_t green = (g / 64); 
		uint8_t blue = (b / 64); 

		return 16 + blue + 4 * green + 16 * red;
	}
}

int to_256_color_num(uint8_t r, uint8_t g, uint8_t b){

	if(is_color_greyscale(r, g, b)){
		return 232 + (r / 11); 
	}
	else{
		uint8_t red = (r / 43); 
		uint8_t green = (g / 43); 
		uint8_t blue = (b / 43); 

		return 16 + blue + 6 * green + 36 * red;
	}
}

Color to_color_rgb(int color_num){
	short r = 0, g = 0, b = 0;

	color_content(color_num, &r, &g, &b);

	return Color(round((int)(r * 255) / 1000.0f), round((int)(g * 255) / 1000.0f), round((int)(b * 255) / 1000.0f)); 
}

//#DEFINITION_END