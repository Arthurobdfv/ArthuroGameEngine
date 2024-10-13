#pragma once

#include "../Transform/TransformComponent.h"
#include "../Renderer/Renderer3D.h"

class GameObject {
	public:
		TransformComponent* Transform;
		Renderer3D* Renderer3D = nullptr;
		GameObject(TransformComponent* transform);
		GameObject();
};