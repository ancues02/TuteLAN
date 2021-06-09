#include "Vector2.h"
#include <assert.h>

// needed for visual studio
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

Vector2::Vector2() :
		x_(), y_() {
}

Vector2::Vector2(const Vector2 &v) :
		x_(v.getX()), y_(v.getY()) {
}

Vector2::Vector2(double x, double y) :
		x_(x), y_(y) {
}

Vector2::~Vector2() {
}

Vector2 Vector2::rotate(double degrees) const {
	Vector2 r;

	degrees = fmod(degrees, 360.0);
	if (degrees > 180.0) {
		degrees = degrees - 360.0;
	} else if (degrees <= -180.0) {
		degrees = 360.0 + degrees;
	}

	assert(degrees >= -180.0 && degrees <= 180.0);

	double angle = degrees * M_PI / 180.0;
	double sine = sin(angle);
	double cosine = cos(angle);

	//rotation matix
	double matrix[2][2] = { { cosine, -sine }, { sine, cosine } };

	double x = x_;
	double y = y_;

	r.x_ = matrix[0][0] * x + matrix[0][1] * y;
	r.y_ = matrix[1][0] * x + matrix[1][1] * y;

	return r;

}

double Vector2::angle(const Vector2 &v) const {

	double a2 = atan2(v.getX(), v.getY());
	double a1 = atan2(x_, y_);
	double sign = a1 > a2 ? 1 : -1;
	double angle = a1 - a2;
	double K = -sign * M_PI * 2;
	angle = (abs(K + angle) < abs(angle)) ? K + angle : angle;
	return angle * 180. / M_PI;
}

Vector2 Vector2::normalize() const {
	Vector2 r;
	r.x_ = x_;
	r.y_ = y_;
	double mag = magnitude();
	if (mag > 0.0) {
		r.x_ = r.x_ / mag;
		r.y_ = r.y_ / mag;
	}
	return r;
}

ostream& operator<<(ostream &os, const Vector2 &v) {
	os << "(" << v.getX() << "," << v.getY() << ")";
	return os;
}
