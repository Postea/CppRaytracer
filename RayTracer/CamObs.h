#include "Mat4.h"
#include "Ray.h"
#pragma once

namespace cam {
	class CamObs
	{
	public:
		//Constructor
		CamObs (const util::Mat4& mat, float theta, int width, int height);
		//Methods
		Ray create (float x, float y) const;
		//Attributes
		const int width_, height_;
	private:
		util::Mat4 mat_;
		float theta_;
	};
}
