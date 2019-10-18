#pragma once
#include "Shape.h"
#include <vector>
#include "Transformation.h"
#include "Mat4.h"
#include "Hit.h"
#include "Ray.h"

namespace shapes {
	class Group : public Shape
	{
	public: 
		Group (const util::Transformation& trans);
		Group (const util::Mat4& matrix);

		std::shared_ptr<cam::Hit> intersect (const cam::Ray& r);
	//protected:TODO
		void add (const std::shared_ptr<shapes::Shape>& shape);
	private:
		std::vector<std::shared_ptr<Shape>> shapeList;
		util::Transformation transform;
	};
}

