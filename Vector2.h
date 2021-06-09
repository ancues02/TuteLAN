#pragma once

#include <iostream>
#include <math.h>

using namespace std;

class Vector2 {
	double x_;  // first coordinate
	double y_;  // second coordinate
public:
	Vector2();
	Vector2(const Vector2 &v);
	Vector2(double x, double y);
	virtual ~Vector2();

	inline double getX() const {
		return x_;
	}
	inline double getY() const {
		return y_;
	}
	inline void setX(double x) {
		x_ = x;
	}
	inline void setY(double y) {
		y_ = y;
	}
	inline void set(const Vector2 &v) {
		x_ = v.x_;
		y_ = v.y_;
	}
	inline void set(double x, double y) {
		x_ = x;
		y_ = y;
	}
	inline double magnitude() const {
		return sqrt(pow(x_, 2) + pow(y_, 2));
	}

	Vector2 rotate(double degrees) const;
	double angle(const Vector2 &v) const;
	Vector2 normalize() const;

	inline Vector2 operator-(const Vector2 &v) const {
		return Vector2(x_ - v.x_, y_ - v.y_);
	}

	inline Vector2 operator+(const Vector2 &v) const {
		return Vector2(x_ + v.x_, y_ + v.y_);
	}
	inline Vector2 operator*(double d) const {
		return Vector2(x_ * d, y_ * d);
	}
	inline double operator *(const Vector2 &d) const {
		return d.x_ * x_ + d.y_ * y_;
	}
	Vector2 operator/(double d) const {
		return Vector2(x_ / d, y_ / d);
	}
	friend ostream& operator<<(ostream &os, const Vector2 &v);
};
