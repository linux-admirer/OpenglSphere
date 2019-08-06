#pragma once

#include <memory>
#include <string>

class Shader
{
public:
	enum class ShaderType : uint8_t
	{
		VERTEX_SHADER = 0,
		GEOMETRY_SHADER,
		FRAGMENT_SHADER
	};

	Shader(ShaderType shaderType, std::string const& shaderFileName);
	~Shader();

	bool compile();

	unsigned int getShaderId() const noexcept;
private:
	struct Private;
	std::unique_ptr<Private> pImpl;
};
