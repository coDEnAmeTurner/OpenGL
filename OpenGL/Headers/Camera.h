#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();
	glm::vec3 getCameraPos();
	glm::vec3 getCameraDirection();
	void processInput(GLFWwindow* window);
	glm::mat4 calculateView();
	void calculateDirection(double xpos, double ypos);

private:
	glm::vec3 up;
	glm::vec3 cameraPos;
	glm::vec3 cameraDirection;
	glm::vec3 cameraUp;

	float lastX, lastY;

	float pitch;
	float yaw;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	bool firstMouse;

};

Camera::Camera()
{
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	lastX = 400.0f;
	lastY = 300.0f;
	pitch = 0.0f;
	yaw = -90.0f;

	firstMouse = true;
}

void Camera::processInput(GLFWwindow* window) {
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;

	const float cameraSpeed = 1.6f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraDirection;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraDirection;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraDirection, cameraUp));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraDirection, cameraUp));

}

inline glm::mat4 Camera::calculateView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
}

void Camera::calculateDirection(double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = -ypos + lastY;

	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.3f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); //0, 0
	cameraDirection.y = sin(glm::radians(pitch)); //0, 0
	cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); //1, -1
	cameraDirection = glm::normalize(cameraDirection);

	//modified here
	glm::vec3 right;
	right = glm::normalize(glm::cross(cameraDirection, up));
	cameraUp = glm::normalize(glm::cross(right, cameraDirection));
}

Camera::~Camera()
{

}

inline glm::vec3 Camera::getCameraPos()
{
	return this->cameraPos;
}

inline glm::vec3 Camera::getCameraDirection()
{
	return this->cameraDirection;
}

