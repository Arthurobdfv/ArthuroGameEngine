#include "Renderer3D.h"

void Renderer3D::SetupEnvironmentUniforms()
{
	mat4 model = _baseTransform->TransformationMatrix();
	_shader->SetModelMatrix(_baseTransform->TransformationMatrix());
}

Renderer3D::Renderer3D(Shader* shader, MeshRenderer* meshRenderer, TransformComponent* transform)
{
	_shader = shader;
	_baseTransform = transform;
	_meshRenderer = meshRenderer;
}

void Renderer3D::Update()
{
	
	_meshRenderer->RenderMesh();
}
