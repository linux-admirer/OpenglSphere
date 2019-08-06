#pragma once

#include <memory>

enum class BufferType : uint8_t
{
	ArrayBuffer,
	ElementBuffer
};

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void unbind();
private:
	struct Private;
	std::unique_ptr<Private> pImpl;
};
