#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>

Camera camera;

void frambuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.calculateDirection(xpos, ypos);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create the window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD load failed\n";
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("vShader.vert", "fShader.frag");

	Model backpack("D:\\OpenGL\\OpenGL\\OpenGL\\backpack\\backpack.obj");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		camera.processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		//20, 1.0, 0.22, 0.20
		ourShader.setVec3("lights[1].position", glm::vec3(0.0f, 0.0f, 2));
		ourShader.setVec3("lights[1].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		ourShader.setVec3("lights[1].diffuse", glm::vec3(0.7f, 0.7f, 0.7f));
		ourShader.setVec3("lights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setFloat("lights[1].constant", 1);
		ourShader.setFloat("lights[1].linear", 0.22);
		ourShader.setFloat("lights[1].quadratic", 0.20);
		ourShader.setFloat("lights[1].shininess", 128.0f);

		ourShader.setVec3("lights[0].position", glm::vec3(0.0f, 0.0f, -2));
		ourShader.setVec3("lights[0].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		ourShader.setVec3("lights[0].diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
		ourShader.setVec3("lights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setFloat("lights[0].constant", 1);
		ourShader.setFloat("lights[0].linear", 0.22);
		ourShader.setFloat("lights[0].quadratic", 0.20);
		ourShader.setFloat("lights[0].shininess", 128.0f);

		ourShader.setVec3("cameraFront", camera.getCameraDirection());

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.calculateView();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		backpack.Draw(ourShader);

		ourShader.unuse();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

}