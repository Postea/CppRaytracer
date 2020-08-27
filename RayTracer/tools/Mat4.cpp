#include "Mat4.h"

namespace util {
	//Constructor
	Mat4::Mat4 (std::array < float, 16> arr) :arr_ (arr) {};
	Mat4::Mat4 () :arr_ (identity ().arr_) {};
	//Operator
	float Mat4::operator[](std::array<int, 2> i) const {
		return arr_[4 * i[0] + i[1]];
	}
	float& Mat4::operator[](std::array<int, 2> i) {
		return arr_[4 * i[0] + i[1]];
	}
	Mat4 Mat4::operator*(const Mat4& rhs) const
	{
		Mat4 n;
		for (int c = 0; c != 4; c++) {
			for (int r = 0; r != 4; r++) {
				float v = 0;
				for (int k = 0; k != 4; k++)
					v += arr_[4 * k + r] * rhs[{c, k}];
				n[{c, r}] = v;
			}
		}
		return n;
	}
	bool Mat4::operator==(const Mat4& rhs) const
	{
		return arr_ == rhs.arr_;
	}
	bool Mat4::operator!=(const Mat4& rhs) const
	{
		return !(*this == rhs);
	}
	//Methods
	Mat4 Mat4::transpose () const
	{
		Mat4 x;
		for (int c = 0; c != 4; c++) {
			for (int r = 0; r != 4; r++) {
				x[{c, r}] = arr_[4 * r + c];
			}
		}
		return x;

	}
	Vec3 Mat4::transformDir (Vec3 v) const
	{
		Mat4 mat (*this);
		const float x = mat[{0, 0}] * v[0] + mat[{0, 1}] * v[1] + mat[{0, 2}] * v[2];
		const float y = mat[{1, 0}] * v[0] + mat[{1, 1}] * v[1] + mat[{1, 2}] * v[2];
		const float z = mat[{2, 0}] * v[0] + mat[{2, 1}] * v[1] + mat[{2, 2}] * v[2];
		return Vec3 (x, y, z);
	}
	Vec3 Mat4::transformPoint (Vec3 v) const
	{
		Mat4 mat (*this);
		float x = mat[{0, 0}] * v[0] + mat[{0, 1}] * v[1] + mat[{0, 2}] * v[2] + mat[{0, 3}];
		float y = mat[{1, 0}] * v[0] + mat[{1, 1}] * v[1] + mat[{1, 2}] * v[2] + mat[{1, 3}];
		float z = mat[{2, 0}] * v[0] + mat[{2, 1}] * v[1] + mat[{2, 2}] * v[2] + mat[{2, 3}];
		return Vec3 (x, y, z);
	}
	Vec3 Mat4::position () const
	{
		Mat4 mat (*this);
		return Vec3 (mat[{0, 3}], mat[{1, 3}], mat[{2, 3}]);
	}
	Mat4 Mat4::invertFull () const
	{
		Mat4 ret;
		std::array<float, 16> mat (arr_);
		std::array<float, 16> dst (ret.arr_);
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

		Mat4 retu (dst);

		return retu;
	}
	//Static
	Mat4 translate (Vec3 xyz) {
		Mat4 matrix;
		matrix[{0, 3}] = xyz[0];
		matrix[{1, 3}] = xyz[1];
		matrix[{2, 3}] = xyz[2];
		return matrix;
	}
	Mat4 rotate (Vec3 axis, float angle) {
		Mat4 matrix;
		float rad = (angle / 180) * M_PI;
		float cosa = cos (rad);
		float sina = sin (rad);
		float l = sqrt (axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
		float rx = axis[0] / l;
		float ry = axis[1] / l;
		float rz = axis[2] / l;
		float icosa = 1 - cosa;
		{
			matrix[{0, 0}] = (icosa * rx * rx + cosa);
			matrix[{1, 0}] = (icosa * rx * ry + rz * sina);
			matrix[{2, 0}] = (icosa * rx * rz - ry * sina);
		}
		{
			matrix[{0, 1}] = (icosa * rx * ry - rz * sina);
			matrix[{1, 1}] = (icosa * ry * ry + cosa);
			matrix[{2, 1}] = (icosa * ry * rz + rx * sina);
		}
		{
			matrix[{0, 2}] = (icosa * rx * rz + ry * sina);
			matrix[{1, 2}] = (icosa * ry * rz - rx * sina);
			matrix[{2, 2}] = (icosa * rz * rz + cosa);
		}
		return matrix;
	}
	Mat4 scale (Vec3 xyz) {
		Mat4 matrix;
		matrix[{0, 0}] = xyz[0];
		matrix[{1, 1}] = xyz[1];
		matrix[{2, 2}] = xyz[2];
		return matrix;
	}
	Mat4 identity ()
	{
		return Mat4 ({ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 });
	}
	//Out
	std::ostream& operator<<(std::ostream& os, const Mat4& rhs) {
		for (int c = 0; c < 4; c++) {
			os << "(";
			for (int r = 0; r < 4; r++) {
				os << rhs[{c, r}];
				if (r != 3) {
					os << ",";
				}
			}
			os << ")" << std::endl;
		}
		return os;
	}
}