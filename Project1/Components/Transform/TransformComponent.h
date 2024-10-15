#pragma once
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/type_ptr.hpp"

using namespace glm;
using namespace std;

class TransformComponent {
	public:	
		vec3 Position();
		vec3 Rotation();
		vec3 Scale();
		void setPos(vec3 newPos);
		void setRot(vec3 axis);
		void setScale(vec3 newPos);
		mat4 TransformationMatrix();
		TransformComponent(vec3 position = vec3(0.0f), vec3 rotation = vec3(0.0f),vec3 scale = vec3(1.0f));
		

	private:
		vec3 _position, _rotation, _scale;
};
