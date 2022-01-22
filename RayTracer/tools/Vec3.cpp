#include "Vec3.h"

namespace util {

// Constructor
Vec3::Vec3(std::array<float, 3> const& arr) : arr(arr) {
}
Vec3::Vec3(float x, float y, float z) : arr(std::array<float, 3>({x, y, z})) {
}
Vec3::Vec3(float xyz) : arr(std::array<float, 3>({xyz, xyz, xyz})) {
}
Vec3::Vec3() : arr(std::array<float, 3>({})) {
}
// Operator
float Vec3::operator[](int i) const {
	return arr[i];
};
float& Vec3::operator[](int i) {
	return arr[i];
};
Vec3 Vec3::operator-() const {
	Vec3 temp(*this);
	temp[0] = -temp[0];
	temp[1] = -temp[1];
	temp[2] = -temp[2];
	return temp;
}
Vec3 Vec3::operator-(const Vec3& rhs) const {
	Vec3 temp(*this);
	temp -= rhs;
	return temp;
}
Vec3 Vec3::operator+(const Vec3& rhs) const {
	Vec3 temp(*this);
	temp += rhs;
	return temp;
}
Vec3 Vec3::operator*(const Vec3& rhs) const {
	Vec3 temp(*this);
	temp *= rhs;
	return temp;
}
Vec3 Vec3::operator*(float s) const {
	Vec3 temp(*this);
	temp *= s;
	return temp;
}
Vec3 Vec3::operator/(const Vec3& rhs) const {
	Vec3 temp(*this);
	temp /= rhs;
	return temp;
}
Vec3 Vec3::operator/(const float div) const {
	Vec3 x(*this);
	return Vec3(x[0] / div, x[1] / div, x[2] / div);
}
void Vec3::operator-=(const Vec3& rhs) {
	arr[0] -= rhs[0];
	arr[1] -= rhs[1];
	arr[2] -= rhs[2];
}
void Vec3::operator+=(const Vec3& rhs) {
	arr[0] += rhs[0];
	arr[1] += rhs[1];
	arr[2] += rhs[2];
}
void Vec3::operator*=(const Vec3& rhs) {
	arr[0] *= rhs[0];
	arr[1] *= rhs[1];
	arr[2] *= rhs[2];
}
void Vec3::operator*=(float s) {
	arr[0] *= s;
	arr[1] *= s;
	arr[2] *= s;
}
void Vec3::operator/=(const Vec3& rhs) {
	arr[0] /= rhs[0];
	arr[1] /= rhs[1];
	arr[2] /= rhs[2];
}
bool Vec3::operator==(const Vec3& rhs) const {
	for (int i = 0; i < 3; i++) {
		if (arr[i] != rhs[i]) {
			return false;
		}
	}
	return true;
}
bool Vec3::operator!=(const Vec3& rhs) const {
	return !(*this == rhs);
}
float Vec3::x() const {
	return arr[0];
}
float Vec3::y() const {
	return arr[1];
}
float Vec3::z() const {
	return arr[2];
}
Vec3 Vec3::normalize() const {
	Vec3 x(*this);
	float l = x.length();
	if (l == 0) {
		return x;
	}
	return Vec3(x[0] / l, x[1] / l, x[2] / l);
}
float Vec3::length() const {
	return sqrt(arr[0] * arr[0] + arr[1] * arr[1] + arr[2] * arr[2]);
}
bool Vec3::limited(float lower_limit, float upper_limit) const {
	bool x = lower_limit <= arr[0] && arr[0] <= upper_limit;
	bool y = lower_limit <= arr[1] && arr[1] <= upper_limit;
	bool z = lower_limit <= arr[2] && arr[2] <= upper_limit;

	return x && y && z;
}
// Static
float dot(const Vec3 lhs, const Vec3 rhs) {
	return ((lhs[0] * rhs[0]) + (lhs[1] * rhs[1]) + (lhs[2] * rhs[2]));
}
Vec3 cross(const Vec3& lhs, const Vec3& rhs) {
	float x = lhs[1] * rhs[2] - lhs[2] * rhs[1];
	float y = lhs[2] * rhs[0] - lhs[0] * rhs[2];
	float z = lhs[0] * rhs[1] - lhs[1] * rhs[0];
	Vec3 result({x, y, z});
	return result;
}

std::ostream& operator<<(std::ostream& os, const util::Vec3& rhs) {
	os << "(" << rhs[0] << " ," << rhs[1] << " ," << rhs[2] << ")";
	return os;
}

Vec3 operator*(float s, const Vec3& rhs) {
	return rhs * s;
}
}  // namespace util