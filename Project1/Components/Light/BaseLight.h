#pragma once
#include "../../glm/glm.hpp"

using namespace glm;

enum LightType {
	Directional = 0,
	Point = 1,
	Ambient = 2,
	FlashLight = 3
};
class BaseLight {
	public:
		LightType Type();
		vec3 Color();
		vec3 Orientation();
		virtual float* ToVao(unsigned int* arraySize);
		float Intensity();
		BaseLight(vec3 color = vec3(1), vec3 orientation = vec3(0), LightType type = Directional, float intensity=1);
		void SetOrientation(vec3 orientation);
	protected:
		LightType _type = Directional;
		vec3 _orientation = vec3(0.0), _color = vec3(0.0);
		float _intensity = 0;
};