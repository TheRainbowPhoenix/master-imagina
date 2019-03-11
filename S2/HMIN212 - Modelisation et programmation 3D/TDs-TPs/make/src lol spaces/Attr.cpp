#include "Attr.h"

//#DEFINITION
////////////////////////////////////////////////// CONSTRUCTEURS

Attr::Attr() : m_attribute(A_NORMAL) {}

Attr::Attr(chtype c) : m_attribute(c & A_ATTRIBUTES) {}

////////////////////////////////////////////////// METHODES

Attr::operator chtype() const {
	return m_attribute;
}

////////////////////////////////////////////////// VARIABLE STATIQUES

const Attr Attr::Normal(A_NORMAL);
const Attr Attr::Standout(A_STANDOUT);
const Attr Attr::Underline(A_UNDERLINE);
const Attr Attr::Reverse(A_REVERSE);
const Attr Attr::Blink(A_BLINK);
const Attr Attr::HalfBright(A_DIM);
const Attr Attr::Bright(A_BOLD);
const Attr Attr::Protected(A_PROTECT);
const Attr Attr::Invisible(A_INVIS);
const Attr Attr::AltCharset(A_ALTCHARSET);
const Attr Attr::Italic(A_ITALIC);
  
//#DEFINITION_END