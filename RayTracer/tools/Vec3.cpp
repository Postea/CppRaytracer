#include "Vec3.h"

namespace util {

//Constructor
Vec3::Vec3(std::array<float, 3> const& arr)
    : arr_(arr)
{
}
Vec3::Vec3(float x, float y, float z)
    : arr_(std::array<float, 3>({ x, y, z }))
{
}
Vec3::Vec3()
    : arr_(std::array<float, 3>({}))
{
}
//Operator
float Vec3::operator[](int i) const { return arr_[i]; };
float& Vec3::operator[](int i) { return arr_[i]; };
Vec3 Vec3::operator-() const
{
    Vec3 temp(*this);
    temp[0] = -temp[0];
    temp[1] = -temp[1];
    temp[2] = -temp[2];
    return temp;
}
Vec3 Vec3::operator-(const Vec3& rhs) const
{
    Vec3 temp(*this);
    temp -= rhs;
    return temp;
}
Vec3 Vec3::operator+(const Vec3& rhs) const
{
    Vec3 temp(*this);
    temp += rhs;
    return temp;
}
Vec3 Vec3::operator*(const Vec3& rhs) const
{
    Vec3 temp(*this);
    temp *= rhs;
    return temp;
}
Vec3 Vec3::operator*(float s) const
{
    Vec3 temp(*this);
    temp *= s;
    return temp;
}
Vec3 Vec3::operator/(const Vec3& rhs) const
{
    Vec3 temp(*this);
    temp /= rhs;
    return temp;
}
Vec3 Vec3::operator/(const float div) const
{
    Vec3 x(*this);
    return Vec3(x[0] / div, x[1] / div, x[2] / div);
}
void Vec3::operator-=(const Vec3& rhs)
{
    arr_[0] -= rhs[0];
    arr_[1] -= rhs[1];
    arr_[2] -= rhs[2];
}
void Vec3::operator+=(const Vec3& rhs)
{
    arr_[0] += rhs[0];
    arr_[1] += rhs[1];
    arr_[2] += rhs[2];
}
void Vec3::operator*=(const Vec3& rhs)
{
    arr_[0] *= rhs[0];
    arr_[1] *= rhs[1];
    arr_[2] *= rhs[2];
}
void Vec3::operator*=(float s)
{
    arr_[0] *= s;
    arr_[1] *= s;
    arr_[2] *= s;
}
void Vec3::operator/=(const Vec3& rhs)
{
    arr_[0] /= rhs[0];
    arr_[1] /= rhs[1];
    arr_[2] /= rhs[2];
}
bool Vec3::operator==(const Vec3& rhs) const
{
    for (int i = 0; i < 3; i++) {
        if (arr_[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}
bool Vec3::operator!=(const Vec3& rhs) const
{
    return !(*this == rhs);
}
Vec3 Vec3::normalize() const
{
    Vec3 x(*this);
    float l = x.length();
    return Vec3(x[0] / l, x[1] / l, x[2] / l);
}
float Vec3::length() const
{
    return sqrt(arr_[0] * arr_[0] + arr_[1] * arr_[1] + arr_[2] * arr_[2]);
}
//Static
float dot(const Vec3 lhs, const Vec3 rhs)
{
    return ((lhs[0] * rhs[0]) + (lhs[1] * rhs[1]) + (lhs[2] * rhs[2]));
}
Vec3 cross(const Vec3& lhs, const Vec3& rhs)
{
    float x = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    float y = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    float z = lhs[0] * rhs[1] - lhs[1] * rhs[0];
    Vec3 result({ x, y, z });
    return result;
}

std::ostream& operator<<(std::ostream& os, const util::Vec3& rhs)
{
    os << "(" << rhs[0] << " ," << rhs[1] << " ," << rhs[2] << ")";
    return os;
}
}