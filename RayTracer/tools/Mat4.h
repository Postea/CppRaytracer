#pragma once

#include <array>

#include "Vec3.h"

namespace util {
class Mat4 {
   public:
	// Constructor
	Mat4(std::array<float, 16> arr);
	Mat4();
	// Operator
	friend std::ostream& operator<<(std::ostream& os, const Mat4& rhs);
	float operator[](std::array<int, 2> i) const;  // int r, int c
	float& operator[](std::array<int, 2> i);       // int r, int c
	Mat4 operator*(const Mat4& rhs) const;
	bool operator==(const Mat4& rhs) const;
	bool operator!=(const Mat4& rhs) const;
	// Methods
	Mat4 transpose() const;
	Vec3 transformDir(Vec3 v) const;
	Vec3 transformPoint(Vec3 v) const;
	Vec3 position() const;
	Mat4 invertFull() const;

   private:
	std::array<float, 16> arr;
};

Mat4 translate(Vec3 xyz);
Mat4 rotate(Vec3 axis, float angle);
Mat4 scale(Vec3 xyz);
Mat4 identity();

}  // namespace util