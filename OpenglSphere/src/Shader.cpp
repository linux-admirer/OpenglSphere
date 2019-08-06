#include "Shader.h"

#include <iostream>
#include <fstream>
#include <streambuf>

#include <glad/glad.h>

struct Shader::Private
{
	Private(ShaderType shaderType, std::string const& shaderFileName)
		: shaderType(shaderType), shaderFileName(shaderFileName)
	{
		switch (shaderType)
		{
		case ShaderType::VERTEX_SHADER:
			shaderId = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::GEOMETRY_SHADER:
			shaderId = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case ShaderType::FRAGMENT_SHADER:
			shaderId = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			std::cout << "unkown shader type\n";
		}
	}

	~Private()
	{
		glDeleteShader(shaderId);
	}

	ShaderType shaderType;
	std::string shaderFileName;
	unsigned int shaderId;
};

Shader::Shader(ShaderType shaderType, std::string const& shaderFileName)
	: pImpl(std::make_unique<Private>(shaderType, shaderFileName))
{
}

Shader::~Shader() = default;

std::string readShaderCode(std::string const& filename)
{
	std::ifstream istream(filename);
	if (!istream.is_open())
		std::cout << "file not found\n";
	std::string code((std::istreambuf_iterator<char>(istream)),
		std::istreambuf_iterator<char>());
	return code;
}

bool Shader::compile()
{
	std::string code = readShaderCode(pImpl->shaderFileName);
	const char* data = code.data();
	glShaderSource(pImpl->shaderId, 1, &data, NULL);
	glCompileShader(pImpl->shaderId);

	int success;
	glGetShaderiv(pImpl->shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::string buf;
		buf.reserve(512);
		glGetShaderInfoLog(pImpl->shaderId, 512, NULL, &buf.at(0));
		std::cout << buf;
	}
	return success;
}

unsigned int Shader::getShaderId() const noexcept
{
	return pImpl->shaderId;
}
