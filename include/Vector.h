#pragma once
#include <cmath>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <iostream>

template <typename T>
struct Vector2
{
//instance variables
	T x, y;
//constructors
	Vector2() : x(0), y(0) {}
	Vector2(T x, T y) : x(x), y(y) {}

	Vector2<T> operator+(const Vector2& v2) const
	{
		return Vector2(x + v2.x, y + v2.y);
	}
	Vector2<T> operator-(const Vector2& v2) const
	{
		return Vector2(x - v2.x, y - v2.y);
	}
	Vector2<T> operator*(const T scalar) const 
	{
		return Vector2(x * scalar, y * scalar);
	}
	Vector2<T> operator/(const T scalar) const 
	{
		if (scalar == 0)
			throw std::invalid_argument("DIVISION BY ZERO");

		return Vector2(x / scalar, y / scalar);
	}
	void operator=(const Vector2& v2)
	{
		x = v2.x;
		y = v2.y;
	}
	bool operator==(const Vector2& v2)const
	{
		return(x == v2.x && y == v2.y);
	}
	T dot(const Vector2& v2) const
	{
		return ((x * v2.x) + (y * v2.y));
	}
	T magnitude() 
	{
		return std::sqrt((x * x) + (y * y));
	}
	Vector2 normalize() 
	{
		return (*this/this->magnitude());
	}

};

//print vector
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec)
{
	os << "[" << vec.x << " " << vec.y << "]";
	return os;
}

//3D vector
template <typename T>
struct Vector3
{
	//instance variables
	T x, y, z;
	//constructors
	Vector3() : x(0), y(0), z(0) {}
	Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

	Vector3<T> operator+(const Vector3& v2) const
	{
		return Vector3(x + v2.x, y + v2.y, z+v2.z);
	}
	Vector3<T> operator-(const Vector3& v2) const
	{
		return Vector3(x - v2.x, y - v2.y, z-v2.z);
	}
	Vector3<T> operator*(const T scalar) const
	{
		return Vector3(x * scalar, y * scalar,z*scalar);
	}
	Vector3<T> operator/(const T scalar) const
	{
		if (scalar == 0)
			throw std::invalid_argument("DIVISION BY ZERO");
		return Vector3(x / scalar, y / scalar, z/scalar);
	}
	void operator=(const Vector3& v2)
	{
		x = v2.x;
		y = v2.y;
		z = v2.z;
	}
	bool operator==(const Vector3& v2)const
	{
		return(x == v2.x && y == v2.y && z == v2.z);
	}
	T dot(const Vector3& v2) const
	{
		return ((x * v2.x) + (y * v2.y)+(z*v2.z));
	}
	Vector3 cross(const Vector3& v2) const//cross product not the dot product
	{
		T newX = (y * v2.z) - (z * v2.y);
		T newY = (z * v2.x) - (x * v2.z);
		T newZ = (x * v2.y) - (y * v2.x);
		return Vector3(newX, newY, newZ);
	}
	T magnitude() const
	{
		return std::sqrt((x * x) + (y * y) + (z * z));
	}
	Vector3 normalize() const
	{
		return *this / this->magnitude();
	}

};

//print vector3
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec)
{
	os << "[" << vec.x << " " << vec.y << " " << vec.z << "]";
	return os;
}

