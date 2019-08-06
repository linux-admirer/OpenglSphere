#include <iostream>
#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Program.h"
#include "DebugCallback.h"
#include "Sphere.h"
#include "STLParser.h"

void frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

template <typename T, size_t N>
unsigned int getArraySize(T (&)[N])
{
	return N;
}

int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	// enable debug context to get more info on errors
	// also need to register debug callback on glad
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenglSphere", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "failed to create window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize glad\n";
		return -1;
	}

	querryAndRegisterDebugCallback();
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

	Program program;
	program.addShader(std::make_unique<Shader>(Shader::ShaderType::VERTEX_SHADER, "./src/VertexShader"));
	program.addShader(std::make_unique<Shader>(Shader::ShaderType::FRAGMENT_SHADER, "./src/FragmentShader"));
	program.link();


	auto sphereVertices = Sphere().getCoordinates(5.);

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * 3 * 3 * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	auto modelMatrixId = glGetUniformLocation(program.getProgramId(), "modelMatrix");
	auto viewMatrixId = glGetUniformLocation(program.getProgramId(), "viewMatrix");
	auto projectionMatrixID = glGetUniformLocation(program.getProgramId(), "projectionMatrix");

	float transparency = 0.;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.useProgram();
		glBindVertexArray(vao);

		glm::mat4 model(1.0f);
		//model = glm::translate(model, glm::vec3(0.1f, 0.1, 0.1));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.f, 0.0f, -10.0f));
		glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(80.0f), 800.f / 600.f, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size() * 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program.getProgramId());

	glfwTerminate();
	return 0;
}
