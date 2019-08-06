#pragma once

#include <memory>

class Shader;

class Program
{
public:
	// Reference to one program
	Program();
	~Program();

	void addShader(std::unique_ptr<Shader>&& shader);

	unsigned int getProgramId() const noexcept;

	// After linking shaders will be deleted. Linking can only be done once.
	bool link();

	void useProgram();
private:
	struct Private;
	std::unique_ptr<Private> pImpl;
};
