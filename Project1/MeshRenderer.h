#pragma once
namespace Mesh {
	class MeshRenderer {
		// Vertex Array Object
	public:
		virtual float* VAO() { return m_vao; }
		virtual unsigned int Size() { return size; }

		virtual void SetupMesh() {};
		virtual void RenderMesh(bool* wireframeMode = nullptr) {};

		virtual void SetVerts(float* vertData, size_t numberOfVerts) {};

	protected:
		float* m_vao = nullptr;
		int size = 0;
	};
}
