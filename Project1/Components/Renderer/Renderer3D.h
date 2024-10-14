#pragma once
#include "../../Shader.h"
#include "../../MeshRenderer.h"
#include "../Transform/TransformComponent.h"


using namespace Mesh;
class Renderer3D {
	private:
		Shader* _shader;
		MeshRenderer* _meshRenderer;

		// Need to modify this implementation to be injected maybe...
		TransformComponent* _baseTransform;
		void SetupEnvironmentUniforms();

	public:
		Renderer3D(Shader* shader, MeshRenderer* meshRenderer, TransformComponent* transform);
		void Update(mat4* viewMatrix, mat4* projectionMatrix);
};