#pragma once
#include <math.h>

class Vector2
{
public:
	float x;
	float y;

	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2(float x_, float y_) {
		x = x_;
		y = y_;
	}

	Vector2 operator+(const Vector2& given) const {
		return Vector2(x + given.x, y + given.y);
	}
	Vector2 operator-(const Vector2& given) const {
		return Vector2(x - given.x, y - given.y);
	}
	Vector2 operator*(const float& given) const {
		return Vector2(x * given, y * given);
	}
	Vector2 operator/(const float& given) const {
		return Vector2(x / given, y / given);
	}

	void operator+=(const Vector2& given) {
		x += given.x;
		y += given.y;
	}
	void operator*=(const float& given) {
		x *= given; 
		y *= given;
	}

	double dist(Vector2 v2) {
		return sqrt(((v2.x - x) * (v2.x - x)) + ((v2.y - y) * (v2.y - y)));
	}

	void normalize() {
		float iradius = 1.f / (sqrtf(x * x + y * y));
		x *= iradius;
		y *= iradius;
	}

	Vector2 normalized()
	{
		float iradius = 1.f / (sqrtf(x * x + y * y));
		return Vector2(x * iradius, y * iradius);
	}

	Vector2 directionTo(Vector2& given)
	{
		Vector2 direction = (*this) - given;
		direction.normalize();
		return Vector2(direction.x, direction.y);
	}
};