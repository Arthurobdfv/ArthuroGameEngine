#include "BaseLight.h"

LightType BaseLight::Type()
{
	return _type;
}

vec3 BaseLight::Color()
{
	return _color;
}

vec3 BaseLight::Orientation()
{
	return _orientation;
}

float* BaseLight::ToVao(unsigned int* arraySize)
{
	const unsigned int size = 8;
	*arraySize = size;
	float values[size]{
		Type(), 
		Color().x, 
		Color().y, 
		Color().z,
		Intensity(),
		Orientation().x, 
		Orientation().y, 
		Orientation().z 
	};
	return values;
}

float BaseLight::Intensity()
{
	return _intensity;
}

BaseLight::BaseLight(vec3 color, vec3 orientation, 
	LightType type, float intensity)
{
	_color = color;
	_orientation = orientation;
	_type = type;
	_intensity = intensity;
}

void BaseLight::SetOrientation(vec3 orientation)
{
	_orientation = orientation;
}
