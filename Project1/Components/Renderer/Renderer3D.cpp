#include "Renderer3D.h"

void Renderer3D::SetupEnvironmentUniforms()
{
	mat4 model = _baseTransform->TransformationMatrix();
	_shader->SetModelMatrix(&model);
}

Renderer3D::Renderer3D(Shader* shader, MeshRenderer* meshRenderer, TransformComponent* transform)
{
	_meshRenderer = meshRenderer;
	_meshRenderer->SetupMesh();
	_shader = shader;
	_baseTransform = transform;
}

void Renderer3D::Update(mat4* viewMatrix, mat4* projectionMatrix, bool* wireFrameMode)
{
	_shader->Use();
	SetupEnvironmentUniforms();
	_shader->SetViewMatrix(viewMatrix);
	_shader->SetProjectionMatrix(projectionMatrix);
	_meshRenderer->RenderMesh(wireFrameMode);
}
