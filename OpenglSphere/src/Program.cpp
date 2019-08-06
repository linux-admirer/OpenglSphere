#include "Program.h"

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "Shader.h"

struct Program::Private
{
	Private()
	{
		programId = glCreateProgram();
	}

	std::vector<std::unique_ptr<Shader>> shaders;
	unsigned int programId;
};

Program::Program()
	: pImpl(std::make_unique<Private>())
{
}

Program::~Program() = default;

void Program::addShader(std::unique_ptr<Shader>&& shader)
{
	pImpl->shaders.push_back(std::move(shader));
}

unsigned int Program::getProgramId() const noexcept
{
	return pImpl->programId;
}

bool Program::link()
{
	for (auto& shader : pImpl->shaders)
	{
		if (!shader->compile())
			throw std::exception("Error compiling shaders");
		glAttachShader(pImpl->programId, shader->getShaderId());
	}
	glLinkProgram(pImpl->programId);

	int success;
	glGetProgramiv(pImpl->programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::string buf;
		buf.reserve(512);
		glGetProgramInfoLog(pImpl->programId, 512, NULL, &buf.at(0));
		std::cout << buf;
	}

	pImpl->shaders.clear();
	return success;
}

void Program::useProgram()
{
	glUseProgram(pImpl->programId);
}
