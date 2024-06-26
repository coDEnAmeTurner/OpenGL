#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include <Shader.h>
#include <Camera.h>

const unsigned int buffer_width = 800, buffer_height = 600;
Camera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.calculateDirection(xpos, ypos);
}

int main() {
	if (!glfwInit()) {
		std::cout << "Glfw initialization is unsuccessful";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(buffer_width, buffer_height, "Stencil Buffer Demo", NULL, NULL);
	if (!window) {
		std::cout << "Window creation isn't successful" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Glad initialization isn't successful" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, buffer_width, buffer_height);
    glEnable(GL_STENCIL_TEST);

    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    GLuint cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    GLuint cubeBuffer;
    glGenBuffers(1, &cubeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    GLuint planeVAO;
    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);
    GLuint planeBuffer;
    glGenBuffers(1, &planeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, planeBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLenum colorFormat = GL_RGB;
    int marbleWidth, marbleHeight, marbleComp;
    unsigned char* marble = stbi_load("D:\\OpenGL\\OpenGL\\Textures\\marble.jpg", &marbleWidth, &marbleHeight, &marbleComp, 0);
    if (!marble) {
        std::cout << "load marble texture failed" << std::endl;
        stbi_image_free(marble);
    }
    unsigned int marbleTex;
    glGenTextures(1, &marbleTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, marbleTex);
    if (marbleComp == 1)
        colorFormat = GL_RED;
    if (marbleComp == 4)
        colorFormat = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, marbleWidth, marbleHeight, 0, colorFormat, GL_UNSIGNED_BYTE, marble);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(marble);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int metalWidth, metalHeight, metalComp;
    unsigned char* metal = stbi_load("D:\\OpenGL\\OpenGL\\Textures\\metal.png", &metalWidth, &metalHeight, &metalComp, 0);
    if (!metal) {
        std::cout << "load metal texture failed" << std::endl;
        stbi_image_free(metal);
    }
    unsigned int metalTex;
    glGenTextures(1, &metalTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, metalTex);
    if (metalComp == 1)
        colorFormat = GL_RED;
    if (metalComp == 4)
        colorFormat = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, metalWidth, metalHeight, 0, colorFormat, GL_UNSIGNED_BYTE, metal);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(metal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(0);

    Shader shader("D:\\OpenGL\\OpenGL\\Stencil_Buffer\\shader.vert", 
        "D:\\OpenGL\\OpenGL\\Stencil_Buffer\\shader.frag");

    Shader singleColorShader("D:\\OpenGL\\OpenGL\\Stencil_Buffer\\shader.vert",
        "D:\\OpenGL\\OpenGL\\Stencil_Buffer\\singlecolorshader.frag");

	while (!(glfwWindowShouldClose(window))) {
		glfwPollEvents();

		camera.processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        shader.use();
        glStencilMask(0x00);
        shader.setMat4("view", camera.calculateView());
        shader.setMat4("projection", glm::perspective(45.0f, (float)buffer_width / buffer_height, 0.1f, 100.0f));        
        glBindVertexArray(planeVAO);
        shader.setInt("texture", 1);
        shader.setMat4("model", glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(planeVertices) / sizeof(float));

        glStencilMask(0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glBindVertexArray(cubeVAO);
        shader.setInt("texture", 0);
        shader.setMat4("model", glm::translate(glm::vec3(-1.0f, 0.1f, 0.5f)));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float)); 
        
        glStencilFunc(GL_ALWAYS, 2, 0xFF);
        shader.setMat4("model", glm::translate(glm::vec3(2.0f, 0.1f, -2.5f)));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float));
        shader.unuse();

        glDisable(GL_DEPTH_TEST);
        singleColorShader.use();
        singleColorShader.setMat4("view", camera.calculateView());
        singleColorShader.setMat4("projection", glm::perspective(45.0f, (float)buffer_width / buffer_height, 0.1f, 100.0f));

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glBindVertexArray(cubeVAO);
        singleColorShader.setMat4("model", glm::translate(glm::vec3(-1.0f, 0.1f, 0.5f)) * glm::scale(glm::vec3(1.05, 1.05, 1.05)));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float));

        glStencilFunc(GL_NOTEQUAL, 2, 0xFF);
        singleColorShader.setMat4("model", glm::translate(glm::vec3(2.0f, 0.1f, -2.5f)) * glm::scale(glm::vec3(1.05, 1.05, 1.05)));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float));
        singleColorShader.unuse();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}

