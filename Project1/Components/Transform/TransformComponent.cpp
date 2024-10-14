#include "TransformComponent.h"

vec3 TransformComponent::Position()
{
	return _position;
}

vec3 TransformComponent::Rotation()
{
	return _rotation;
}

vec3 TransformComponent::Scale()
{
	return _scale;
}

void TransformComponent::setPos(vec3 newPos)
{
	_position = newPos;
}

void TransformComponent::setRot(vec3 axis)
{
	_rotation = axis;
}

void TransformComponent::setScale(vec3 newPos)
{
	_scale = newPos;
}

mat4 TransformComponent::TransformationMatrix()
{
	mat4 transMat = mat4(1.0f);
	transMat = rotate(transMat, _rotation.x, vec3(1.0f, 0.0, 0.0));
	transMat = rotate(transMat, _rotation.y, vec3(0.0f, 1.0, 0.0));
	transMat = rotate(transMat, _rotation.z, vec3(0.0f, 0.0, 1.0));
	transMat = translate(transMat, _position);
	transMat = scale(transMat, _scale);
	return transMat;
}

TransformComponent::TransformComponent(vec3 position, vec3 rotation, vec3 scale)
{
	_position = position;
	_rotation = rotation;
	_scale = scale;
}