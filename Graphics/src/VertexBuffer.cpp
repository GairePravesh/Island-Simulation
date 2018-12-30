#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	glGenBuffers(1, &m_Renderer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, & m_Renderer_ID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID);
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


