#pragma once

#include <GL/glew.h>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

class Renderer
{
public:
	void Clear() const;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};