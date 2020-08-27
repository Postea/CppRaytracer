#pragma once
#include "Hit.h"
#include "Ray.h"

namespace shapes {
	class Shape {
	public:
		virtual std::shared_ptr<cam::Hit> intersect (const cam::Ray& r) = 0;
	};
}