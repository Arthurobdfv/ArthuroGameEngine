#include "EBOMeshRenderer.h"
#include <glad/glad.h>
#include <glfw3.h>

void EBOMeshRenderer::RenderMesh() {
	glBindVertexArray(m_vaoId);
	glDrawElements(GL_TRIANGLES, indexDataSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void EBOMeshRenderer::SetupMesh()
{
	glGenBuffers(1, &m_eboId);
	glGenBuffers(1, &m_vboId);
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, m_vao, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexDataSize, indexData, GL_STATIC_DRAW);
}

void EBOMeshRenderer::SetVerts(float* vertData, size_t numberOfVerts)
{

}

void EBOMeshRenderer::SetIndexes(float* indexData, size_t numberOfIndexes)
{

}

unsigned int EBOMeshRenderer::Size()
{
	return size;
}

float* EBOMeshRenderer::VAO()
{
	return m_vao;
}

EBOMeshRenderer::EBOMeshRenderer() {
}

EBOMeshRenderer::EBOMeshRenderer(
	float* vertData, 
	size_t vertDataSize,
	unsigned int* indexData, 
	size_t indexDataSize) : MeshRenderer()
{
	m_vao = vertData;
	this->size = vertDataSize;
	this->indexData = indexData;
	this->indexDataSize = indexDataSize;
}
