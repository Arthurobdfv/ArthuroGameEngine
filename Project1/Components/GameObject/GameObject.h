#pragma once

#include "../Transform/TransformComponent.h"
#include "../Renderer/Renderer3D.h"

class GameObject {
	public:
		TransformComponent* Transform;
		Renderer3D* _renderer3D;
		GameObject(TransformComponent* transform, Renderer3D* renderer = nullptr);
		GameObject();
		void SetRenderer(Renderer3D* renderer);
};