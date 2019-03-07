#ifndef _ATTR_H_
#define _ATTR_H_

//#PREPROCESSING

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED // pour utilisé les fonction wide character et cchar_t
#endif

extern "C"{
#include <ncursesw/curses.h>
}

//#PREPROCESSING_END

//#DECLARATION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS ATTR

class Attr
{
private:
	chtype m_attribute;

public:

	Attr();
	Attr(chtype);
	
	operator chtype() const;

	static const Attr Normal;
	static const Attr Standout;
	static const Attr Underline;
	static const Attr Reverse;
	static const Attr Blink;
	static const Attr HalfBright;
	static const Attr Bright;
	static const Attr Protected;
	static const Attr Invisible;
	static const Attr AltCharset;
	static const Attr Italic;
};
 
//#DECLARATION_END

#endif
