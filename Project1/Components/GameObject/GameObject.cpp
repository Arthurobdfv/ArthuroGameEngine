#include "GameObject.h"

GameObject::GameObject(TransformComponent* transform, Renderer3D* renderer)
{
	Transform = transform;
	_renderer3D = renderer;
}

GameObject::GameObject()
{
	Transform = new TransformComponent();
}

void GameObject::SetRenderer(Renderer3D* renderer)
{
	_renderer3D = renderer;
}
