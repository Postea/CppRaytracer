#pragma once

#include <array>
#include <cassert>
#include <iostream>

namespace util {
	class Vec3 {
	public:
		//Constructor
		Vec3 (std::array<float, 3> const& arr);
		Vec3 (float x, float y, float z);
		Vec3 ();
		//Operator
		friend std::ostream& operator<<(std::ostream& os, const util::Vec3& rhs);
		float operator[](int i)const;
		float& operator[](int i);
		Vec3 operator-() const;
		Vec3 operator-(const Vec3& rhs) const;
		Vec3 operator+(const Vec3& rhs) const;
		Vec3 operator*(const Vec3& rhs) const;
		Vec3 operator*(float s) const;
		Vec3 operator/(const Vec3& rhs) const;
		Vec3 operator/(const float div) const;
		void operator-=(const Vec3& rhs);
		void operator+=(const Vec3& rhs);
		void operator*=(const Vec3& rhs);
		void operator*=(float s);
		void operator/=(const Vec3& rhs);
		bool operator==(const Vec3& rhs) const;
		bool operator!=(const Vec3& rhs) const;
		//Methods
		Vec3 normalize () const;
		float length () const;
	private:
		std::array<float, 3> arr_;
	};

	float dot (const Vec3 lhs, const Vec3 rhs);
	Vec3 cross (const Vec3& lhs, const Vec3& rhs);
}