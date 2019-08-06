#include "VertexArrayObject.h"

#include <glad/glad.h>

struct VertexArrayObject::Private
{
	Private()
	{
		glGenVertexArrays(1, &vertexArrayObjectId);
		glBindVertexArray(vertexArrayObjectId);
	}

	unsigned int vertexArrayObjectId;
};

VertexArrayObject::VertexArrayObject()
	: pImpl(std::make_unique<Private>())
{
}

VertexArrayObject::~VertexArrayObject()
{
}

void VertexArrayObject::unbind()
{
	glBindVertexArray(0);
}

