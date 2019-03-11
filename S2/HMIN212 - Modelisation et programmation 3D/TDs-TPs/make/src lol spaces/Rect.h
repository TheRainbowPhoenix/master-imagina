#ifndef _RECT_H_
#define _RECT_H_
 
#include "Vector2.h"

//#DECLARATION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS RECT

template <typename T>
class Rect {

public:

	T x;
	T y;
	T width;
	T height;

	Rect();
	
	Rect(T x, T y, T width, T height);
	
	Rect(const Vector2<T>& position, const Vector2<T>& size);

   	template <typename U>
	explicit Rect(const Rect<U>& rectangle);

	Vector2<T> center();

	Vector2<T> get_size();
	Vector2<T> get_position();
	
	void set_size(const Vector2<T>& size);
	void set_position(const Vector2<T>& position);

	bool contains(T x, T y)  const;

	bool contains(const Vector2<T>& point) const;

	bool intersects(const Rect<T>& rect) const;

	bool intersects(const Rect<T>& rect, Rect<T>& intersection) const;
};

////////////////////////////////////////////////// TYPES COMMUNS

typedef Rect<int> IntRect;
typedef Rect<float> FloatRect;

////////////////////////////////////////////////// OPERATEURS DECLARATION

template <typename T>
bool operator==(const Rect<T>& left, const Rect<T>& right);

template <typename T>
bool operator!=(const Rect<T>& left, const Rect<T>& right);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Rect<T>& rect);

template <typename T>
std::istream& operator>>(std::istream& is, Rect<T>& rect);

//#DECLARATION_END

#include "Rect.tpp"

#endif
