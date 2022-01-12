#include "Mat4.h"

#define _USE_MATH_DEFINES

#include <math.h>

namespace util {
// Constructor
Mat4::Mat4(std::array<float, 16> arr) : arr(arr){};
Mat4::Mat4() : arr(identity().arr){};
// Operator
float Mat4::operator[](std::array<int, 2> i) const {
	return arr[4 * i[0] + i[1]];
}
float& Mat4::operator[](std::array<int, 2> i) {
	return arr[4 * i[0] + i[1]];
}
Mat4 Mat4::operator*(const Mat4& rhs) const {
	Mat4 n;
	for (int c = 0; c != 4; c++) {
		for (int r = 0; r != 4; r++) {
			float v = 0;
			for (int k = 0; k != 4; k++)
				v += arr[4 * k + r] * rhs.arr[4 * c + k];
			n.arr[4 * c + r] = v;
		}
	}
	return n;
}
bool Mat4::operator==(const Mat4& rhs) const {
	return arr == rhs.arr;
}
bool Mat4::operator!=(const Mat4& rhs) const {
	return !(*this == rhs);
}
// Methods
Mat4 Mat4::transpose() const {
	Mat4 x;
	for (int c = 0; c < 4; c++) {
		for (int r = 0; r < 4; r++) {
			x.arr[4 * c + r] = arr[4 * r + c];
		}
	}
	return x;
}
Vec3 Mat4::transformDir(const Vec3& v) const {
	const float x = arr[0] * v[0] + arr[1] * v[1] + arr[2] * v[2];
	const float y = arr[4] * v[0] + arr[5] * v[1] + arr[6] * v[2];
	const float z = arr[8] * v[0] + arr[9] * v[1] + arr[10] * v[2];
	return Vec3(x, y, z);
}
Vec3 Mat4::transformPoint(const Vec3& v) const {
	const float x = arr[0] * v[0] + arr[1] * v[1] + arr[2] * v[2] + arr[3];
	const float y = arr[4] * v[0] + arr[5] * v[1] + arr[6] * v[2] + arr[7];
	const float z = arr[8] * v[0] + arr[9] * v[1] + arr[10] * v[2] + arr[11];
	return Vec3(x, y, z);
}
Vec3 Mat4::position() const {
	return Vec3(arr[3], arr[7], arr[11]);
}
Mat4 Mat4::invertFull() const {
	Mat4 ret;
	std::array<float, 16> mat(arr);
	std::array<float, 16> dst(ret.arr);
	std::array<float, 12> tmp;

	/* temparray for pairs */
	std::array<float, 16> src;

	/* array of transpose source matrix */
	float det;

	/* determinant */
	/*
	 * transpose matrix
	 */
	for (int i = 0; i < 4; i++) {
		src[i] = mat[i * 4];
		src[i + 4] = mat[i * 4 + 1];
		src[i + 8] = mat[i * 4 + 2];
		src[i + 12] = mat[i * 4 + 3];
	}

	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];

	/* calculate first 8 elements (cofactors) */
	dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];
	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];

	/* calculate second 8 elements (cofactors) */
	dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

	/* calculate determinant */
	det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

	/* calculate matrix inverse */
	det = 1 / det;

	for (int j = 0; j < 16; j++) {
		dst[j] *= det;
	}

	Mat4 retu(dst);

	return retu;
}
// Static
Mat4 translate(const Vec3& xyz) {
	Mat4 mat;
	mat.arr[3] = xyz[0];
	mat.arr[7] = xyz[1];
	mat.arr[11] = xyz[2];
	return mat;
}
Mat4 rotate(const Vec3& axis, float angle) {
	Mat4 mat;
	float rad = (angle / 180) * M_PI;
	float cosa = cos(rad);
	float sina = sin(rad);
	float l = sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
	float rx = axis[0] / l;
	float ry = axis[1] / l;
	float rz = axis[2] / l;
	float icosa = 1 - cosa;
	{
		mat.arr[0] = (icosa * rx * rx + cosa);
		mat.arr[4] = (icosa * rx * ry + rz * sina);
		mat.arr[8] = (icosa * rx * rz - ry * sina);
	}
	{
		mat.arr[1] = (icosa * rx * ry - rz * sina);
		mat.arr[5] = (icosa * ry * ry + cosa);
		mat.arr[9] = (icosa * ry * rz + rx * sina);
	}
	{
		mat.arr[2] = (icosa * rx * rz + ry * sina);
		mat.arr[6] = (icosa * ry * rz - rx * sina);
		mat.arr[10] = (icosa * rz * rz + cosa);
	}
	return mat;
}
Mat4 scale(const Vec3& xyz) {
	Mat4 mat;
	mat.arr[0] = xyz[0];
	mat.arr[5] = xyz[1];
	mat.arr[10] = xyz[2];
	return mat;
}
Mat4 identity() {
	return Mat4({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
}
// Out
std::ostream& operator<<(std::ostream& os, const Mat4& rhs) {
	for (int c = 0; c < 4; c++) {
		os << "(";
		for (int r = 0; r < 4; r++) {
			os << rhs.arr[4 * c + r];
			if (r != 3) {
				os << ",";
			}
		}
		os << ")" << std::endl;
	}
	return os;
}
}  // namespace util