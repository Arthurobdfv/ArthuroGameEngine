#pragma once
#include "../../../../MeshRenderer.h"

using namespace Mesh;

class VAOMeshRenderer : public MeshRenderer {
	public:
		void RenderMesh(bool* wireframeMode = nullptr) override;
		void SetVerts(float* vertData, size_t vertDataSize, int numberOfVerts);
		unsigned int Size() override;
		float* VAO() override;
		void SetupMesh() override;
		VAOMeshRenderer(int numberOfVerts);
		VAOMeshRenderer(float* vertData, size_t vertDataSize, int numberOfVerts);
	private:
		int _numberOfVerts = 0;
};