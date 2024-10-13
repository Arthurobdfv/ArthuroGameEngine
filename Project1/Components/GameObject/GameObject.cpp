#include "GameObject.h"

GameObject::GameObject(TransformComponent* transform)
{
	Transform = transform;
}

GameObject::GameObject()
{
	Transform = new TransformComponent();
}
