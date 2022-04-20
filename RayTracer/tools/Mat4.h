#pragma once

#include <array>

#include "Vec3.h"

namespace util {
class Mat4 {
	// Some of the functions are from cgg Abgaben of Prof. Tramberend
   public:
	// Constructor
	Mat4(const std::array<float, 16>& arr);
	Mat4();
	// Operator
	friend std::ostream& operator<<(std::ostream& os, const Mat4& rhs);
	float operator[](const std::array<int, 2>& i) const;  // int r, int c
	float& operator[](const std::array<int, 2>& i);       // int r, int c
	Mat4 operator*(const Mat4& rhs) const;
	bool operator==(const Mat4& rhs) const;
	bool operator!=(const Mat4& rhs) const;
	// Methods
	Mat4 transpose() const;
	Vec3 transform_direction(const Vec3& v) const;
	Vec3 transform_point(const Vec3& v) const;
	Vec3 position() const;
	Mat4 invert() const;
	friend Mat4 translate(const Vec3& xyz);
	friend Mat4 rotate(const Vec3& axis, float angle);
	friend Mat4 scale(const Vec3& xyz);

   private:
	std::array<float, 16> arr;
};

Mat4 translate(const Vec3& xyz);
Mat4 rotate(const Vec3& axis, float angle);
Mat4 scale(const Vec3& xyz);
Mat4 identity();

}  // namespace util