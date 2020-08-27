#pragma once
#include "Shape.h"

namespace shapes {
	class CirclePlane : public Shape
	{
	public:
		CirclePlane (float radius,const std::shared_ptr<material::Material>& material);
		std::shared_ptr<cam::Hit> intersect (const cam::Ray& r);
	private:
		std::shared_ptr <material::Material> material_;
		const float radius_;
	};
}
