//#DEFINITION
////////////////////////////////////////////////// CONSTRUCTEURS
 
template <typename T>
Vector2<T>::Vector2() : x(), y() {}

template <typename T>
Vector2<T>::Vector2(T x, T y) : x(x), y(y) {}

template <typename T>
template <typename U>
Vector2<T>::Vector2(const Vector2<U> &vector) : x(vector.x), y(vector.y) {}

template <typename T>
Vector2<T>::~Vector2() {}

////////////////////////////////////////////////// METHODES

template <typename T>
Vector2<T> Vector2<T>::normal() const{
	return Vector2<T>(-y, x);
}

template <typename T>
float Vector2<T>::length() const{
	return powf(length_squared(), 0.5);
}

template <typename T>
float Vector2<T>::length_squared() const{
	return (x * x) + (y * y);
}

template <typename T>
void Vector2<T>::normalize(){
	float length = this->length();
	x /= length;
	y /= length;
}

template <typename T>
Vector2<int> Vector2<T>::rounded() const{
	return Vector2<int>(round(x), round(y));
}

///////////////////////////////////////////////////// METHODES STATIQUES

template <typename T>
float Vector2<T>::dot(const Vector2<T>& left, const Vector2<T>& right){
	return (left.x * right.x) + (left.y * right.y); 
}

template <typename T>
float Vector2<T>::distance(const Vector2<T>& left, const Vector2<T>& right){
	return powf(Vector2<T>::distance_squared(left, right), 0.5);
}

template <typename T>
float Vector2<T>::distance_squared(const Vector2<T>& left, const Vector2<T>& right){
	Vector2<T> dv = left - right;
	return (dv.x * dv.x) + (dv.y * dv.y);
}

template <typename T>
Vector2<T> Vector2<T>::min(const Vector2<T>& left, const Vector2<T>& right){
	return left.length_squared() < right.length_squared() ? Vector2<T>(left) : Vector2<T>(right);
}

template <typename T>
Vector2<T> Vector2<T>::max(const Vector2<T>& left, const Vector2<T>& right){
	return left.length_squared() > right.length_squared() ? Vector2<T>(left) : Vector2<T>(right);
}

template <typename T>
Vector2<T> Vector2<T>::normalize(const Vector2<T>& vector){
	return vector / vector.length();
}

template <typename T>
Vector2<T> Vector2<T>::reflect(const Vector2<T>& vector, const Vector2<T>& normal){
	return vector - (2 * Vector2<T>::dot(vector, normal) * normal);
}

///////////////////////////////////////////////////// VARIABLE STATIQUES

template <typename T>
Vector2<T> Vector2<T>::zero(0, 0);

template <typename T>
Vector2<T> Vector2<T>::one(1, 1);

template <typename T>
Vector2<T> Vector2<T>::up(0, -1);

template <typename T>
Vector2<T> Vector2<T>::down(0, 1);

template <typename T>
Vector2<T> Vector2<T>::left(-1, 0);

template <typename T>
Vector2<T> Vector2<T>::right(1, 0);

////////////////////////////////////////////////// OPERATEURS DEFINITION

template <typename T>
Vector2<T> operator-(const Vector2<T>& right){
	return Vector2<T>(-right.x, -right.y);
}

template <typename T>
Vector2<T>& operator+=(Vector2<T>& left, const Vector2<T>& right){
	left.x += right.x;
	left.y += right.y;
	return left;
}

template <typename T>
Vector2<T>& operator-=(Vector2<T>& left, const Vector2<T>& right){
	left += -right;
	return left;
}

template <typename T>
Vector2<T> operator+(const Vector2<T>& left, const Vector2<T>& right){
	return Vector2<T>(left.x + right.x, left.y + right.y);
}

template <typename T>
Vector2<T> operator-(const Vector2<T>& left, const Vector2<T>& right){
	return (left + -right);
}

template <typename T>
Vector2<T> operator*(const Vector2<T>& left, T right){
	return Vector2<T>(left.x * right, left.y * right);
}

template <typename T>
Vector2<T> operator*(T left, const Vector2<T>& right){
	return (right * left);
}

template <typename T>
Vector2<T>& operator*=(Vector2<T>& left, T right){
	left.x *= right;
	left.y *= right;
	return left;
}

template <typename T>
Vector2<T> operator/(const Vector2<T>& left, T right){
	return Vector2<T>(left.x / right, left.y / right);
}

template <typename T>
Vector2<T>& operator/=(Vector2<T>& left, T right){
	left.x /= right;
	left.y /= right;
	return left;
}

template <typename T>
bool operator==(const Vector2<T>& left, const Vector2<T>& right){
	return left.x == right.x && left.y == right.y;
}

template <typename T>
bool operator!=(const Vector2<T>& left, const Vector2<T>& right){
	return !(left == right);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vector){
	os << "{" << vector.x << ", " << vector.y << "}";
	return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, Vector2<T>& vector){
  is >> vector.x >> vector.y;
  return is;
}

//#DEFINITION_END