//#DEFINITION
////////////////////////////////////////////////// CONSTRUCTEURS
 
template <typename T>
Rect<T>::Rect() : x(), y(), width(), height() {}

template <typename T>
Rect<T>::Rect(T x, T y, T width, T height) : x(x), y(y), width(width), height(height) {}

template <typename T>
Rect<T>::Rect(const Vector2<T>& position, const Vector2<T>& size) : x(position.x), y(position.y), width(size.x), height(size.y) {}

template <typename T>
template <typename U>
Rect<T>::Rect(const Rect<U>& rectangle) : x(rectangle.x), y(rectangle.y), width(rectangle.width), height(rectangle.height) {}

////////////////////////////////////////////////// METHODES

template <typename T>
Vector2<T> Rect<T>::center(){
	return Vector2<T>(x + (width / 2), y + (height / 2));
}

template <typename T>
Vector2<T> Rect<T>::get_size(){
	return Vector2<T>(width, height);
}

template <typename T>
Vector2<T> Rect<T>::get_position(){
	return Vector2<T>(x, y);
}

template <typename T>
void Rect<T>::set_size(const Vector2<T>& size){
	width = size.x;
	height = size.y;
}

template <typename T>
void Rect<T>::set_position(const Vector2<T>& position){
	x = position.x;
	y = position.y;
}

template <typename T>
bool Rect<T>::contains(T x, T y)  const {
	return x >= this->x && x <= width && y >= this->y && y <= height;
}

template <typename T>
bool Rect<T>::contains(const Vector2<T>& point) const {
	return contains(point.x, point.y);
}

template <typename T>
bool Rect<T>::intersects(const Rect<T>& rect) const {
	return !(rect.x >= x + width || rect.x + rect.width <= x ||
		rect.y >= y + height || rect.y + rect.height <= y);
}

template <typename T>
bool Rect<T>::intersects(const Rect<T>& rect, Rect<T>& intersection) const {
	if (intersects(rect)) {

		int xmin = x > rect.x ? x : rect.x; //std::max(x, rect.x);
		int ymin = y > rect.y ? y : rect.y; //std::max(y, rect.y);
		int xmax = x + width < rect.x + rect.width ? x + width : rect.x + rect.width; //std::min(x + width, rect.x + rect.width);
		int ymax = y + height < rect.y + rect.height ? y + height : rect.y + rect.height; //std::min(y + height, rect.y + rect.height);

		intersection = Rect(xmin, ymin, xmax - xmin, ymax - ymin);
		return true;
	}
	return false;
}

////////////////////////////////////////////////// OPERATEURS DEFINITION

template <typename T>
bool operator==(const Rect<T>& left, const Rect<T>& right){
	return left.x == right.x && left.y == right.y && left.width == right.width && left.height == right.height;
}

template <typename T>
bool operator!=(const Rect<T>& left, const Rect<T>& right){
	return !(left == right);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Rect<T>& rect){
	os << "{" << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << "}";
	return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, Rect<T>& rect){
  is >> rect.x >> rect.y >> rect.width >> rect.height;
  return is;
}

//#DEFINITION_END
