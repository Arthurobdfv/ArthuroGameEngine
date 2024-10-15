#include "Camera.h"
#pragma once

vec3 Camera::Position()
{
	return _position;
}

float Camera::FieldOfView()
{
	return _fieldOfView;
}

void Camera::SetPosition(vec3 newPos)
{
	_position = newPos;
}

void Camera::SetFieldOfView(float newFieldOfView)
{
	_fieldOfView = newFieldOfView;
}

Camera::Camera(vec3 pos)
{
	_position = pos;
}
