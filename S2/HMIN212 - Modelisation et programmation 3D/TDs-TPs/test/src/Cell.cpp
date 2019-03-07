#include "Cell.h"

//#DEFINITION
///////////////////////////////////////////////// CONSTRUCTEURS
 
Cell::Cell() : character(L' '), color(), attr() {}

Cell::Cell(cchar_t c) : character(c.chars[0]), color(c.attr), attr(c.attr) {}

Cell::Cell(wint_t character, ColorPair color, Attr attr) : character(character), color(color), attr(attr) {} 

///////////////////////////////////////////////// METHODES

//la conversion en cchar_t pourrais etre meilleur je pense qu'il y a des characters unicode qui ne sont pas supporté
//mais pour l'instant ca marche et c'est plus simple comme ça, un modification sera facile si besoin. 
Cell::operator cchar_t() const{

	cchar_t c;// = { 0, {0, 0, 0, 0, 0}};
	memset(&c, 0, sizeof(c));

	c.attr = attr | color;

	c.chars[0] = character;
	c.chars[1] = L'\0';

	return c;
}

//#DEFINITION_END