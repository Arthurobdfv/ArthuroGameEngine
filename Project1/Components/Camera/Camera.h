#pragma once
#include "../../glm/glm.hpp"

using namespace glm;

class Camera {
	public:
		vec3 Position();
		float FieldOfView();
		void SetPosition(vec3 newPos);
		void SetFieldOfView(float newFieldOfView);
		Camera(vec3 pos = vec3(0.0f));

	private:
		vec3 _position;
		float _fieldOfView = 45;
};