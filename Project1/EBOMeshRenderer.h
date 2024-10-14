#pragma once
#include "MeshRenderer.h"

using namespace Mesh;


class EBOMeshRenderer : public MeshRenderer
{
	public:
		void RenderMesh(bool* wireframeMode = nullptr) override;
		void SetupMesh() override;
		void SetVerts(float* vertData, size_t numberOfVerts) override;
		void SetIndexes(float* indexData, size_t numberOfIndexes);
		unsigned int Size() override;
		float* VAO() override;
		EBOMeshRenderer();
		EBOMeshRenderer(float* vertData, size_t vertDataSize, unsigned int* indexData, size_t indexDataSize);

	private:
		unsigned int m_eboId = 0;
		unsigned int* indexData = nullptr;
		int indexDataSize = 0;

};


