#ifndef _VECTOR2_H_
#define _VECTOR2_H_
 
//#PREPROCESSING
#include <iostream>
#include <cmath>
//#PREPROCESSING_END


//#DECLARATION

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS VECTOR2

template <typename T>
class Vector2 {

public:

	T x;
	T y;

	Vector2();
	
	Vector2(T x, T y);
	
	template <typename U>
	explicit Vector2(const Vector2<U>& vector);
	
	~Vector2();

	Vector2<int> rounded() const;

	Vector2<T> normal() const;

	float length() const;

	float length_squared() const;

	void normalize();

	static Vector2<T> zero;
	static Vector2<T> one;
	static Vector2<T> up;
	static Vector2<T> down;
	static Vector2<T> left;
	static Vector2<T> right;

	static float dot(const Vector2<T>& left, const Vector2<T>& right);
	static float distance(const Vector2<T>& left, const Vector2<T>& right);
	static float distance_squared(const Vector2<T>& left, const Vector2<T>& right);

	static Vector2<T> min(const Vector2<T>& left, const Vector2<T>& right);
	static Vector2<T> max(const Vector2<T>& left, const Vector2<T>& right);
	static Vector2<T> normalize(const Vector2<T>& vector);

	/* renvoie la reflexion du vecteur vector par rappor a la normal normal (normal doit etre normalisé) */
	static Vector2<T> reflect(const Vector2<T>& vector, const Vector2<T>& normal);
};

////////////////////////////////////////////////// TYPES COMMUNS

typedef Vector2<int> 			Vector2i;
typedef Vector2<unsigned int> 	Vector2u;
typedef Vector2<float> 			Vector2f;

////////////////////////////////////////////////// OPERATEURS DECLARATION

template <typename T>
Vector2<T> operator-(const Vector2<T>& right);

template <typename T>
Vector2<T>& operator+=(Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T>& operator-=(Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T> operator+(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T> operator-(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T> operator*(const Vector2<T>& left, T right);

template <typename T>
Vector2<T> operator*(T left, const Vector2<T>& right);

template <typename T>
Vector2<T>& operator*=(Vector2<T>& left, T right);

template <typename T>
Vector2<T> operator/(const Vector2<T>& left, T right);

template <typename T>
Vector2<T>& operator/=(Vector2<T>& left, T right);

template <typename T>
bool operator==(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
bool operator!=(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vector);

template <typename T>
std::istream& operator>>(std::istream& is, Vector2<T>& vector);

//#DECLARATION_END

#include "Vector2.tpp"

#endif
