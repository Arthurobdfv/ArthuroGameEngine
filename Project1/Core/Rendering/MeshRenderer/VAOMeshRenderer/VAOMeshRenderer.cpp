#include "VAOMeshRenderer.h"
#include <glad/glad.h>
#include <glfw3.h>

void VAOMeshRenderer::RenderMesh(bool* wireframeMode)
{
	if (wireframeMode != nullptr) {
		if (*wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glBindVertexArray(m_vaoId);
	glDrawArrays(GL_TRIANGLES, 0, _numberOfVerts);
}

void VAOMeshRenderer::SetVerts(float* vertData, size_t vertDataSize, int numberOfVerts)
{
}

unsigned int VAOMeshRenderer::Size()
{
	return size;
}

float* VAOMeshRenderer::VAO()
{
	return m_vao;
}

void VAOMeshRenderer::SetupMesh()
{
	glGenBuffers(1, &m_vboId);
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, size, m_vao, GL_STATIC_DRAW);
}

VAOMeshRenderer::VAOMeshRenderer(int numberOfVerts)
{
	_numberOfVerts = numberOfVerts;
}

VAOMeshRenderer::VAOMeshRenderer(float* vertData, size_t vertDataSize, int numberOfVerts)
{
	m_vao = vertData;
	size = vertDataSize;
	_numberOfVerts = numberOfVerts;
}
