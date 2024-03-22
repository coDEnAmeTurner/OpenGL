#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include <Shader.h>
#include <Camera.h>
#include <vector>
#include <map>

const unsigned int buffer_width = 800, buffer_height = 600;
Camera camera;
GLuint cubeVAO, planeVAO, quadVAO;
std::vector<glm::mat4> cube_models;
float cubeVertices[] = {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
     // Bottom face
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     // Top face
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
};
float planeVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
     -5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    -5.0f, -0.5f,  -5.0f,  0.0f, 0.0f,

     5.0f, -0.5f,  -5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, 5.0f,  0.0f, 2.0f,
     5.0f, -0.5f, 5.0f,  2.0f, 2.0f
};
float quadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.calculateDirection(xpos, ypos);
}

void DrawScene(Shader& shader) {
    shader.use();
    shader.setMat4("view", camera.calculateView());
    shader.setMat4("projection", glm::perspective(45.0f, (float)buffer_width / buffer_height, 0.1f, 100.0f));
    glBindVertexArray(planeVAO);
    shader.setInt("texture", 1);
    shader.setMat4("model", glm::translate(glm::vec3(0, 0, 0)));
    glDrawArrays(GL_TRIANGLES, 0, sizeof(planeVertices) / sizeof(float));
    glBindVertexArray(cubeVAO);
    shader.setInt("texture", 0);
    for (const glm::mat4& model : cube_models) {
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float));
    }
    shader.unuse();
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

	GLFWwindow* window = glfwCreateWindow(buffer_width, buffer_height, "Framebuffers Demo", NULL, NULL);
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

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    GLuint quadBuffer;
    glGenBuffers(1, &quadBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLenum colorFormat = GL_RGB;
    int containerWidth, containerHeight, containerComp;
    unsigned char* container = stbi_load("D:\\OpenGL\\OpenGL\\Textures\\container.jpg", &containerWidth, &containerHeight, &containerComp, 0);
    if (!container) {
        std::cout << "load container texture failed" << std::endl;
        stbi_image_free(container);
    }
    unsigned int containerTex;
    glGenTextures(1, &containerTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, containerTex);
    if (containerComp == 1)
        colorFormat = GL_RED;
    if (containerComp == 4)
        colorFormat = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, containerWidth, containerHeight, 0, colorFormat, GL_UNSIGNED_BYTE, container);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(container);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
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

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffer_width, buffer_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, buffer_width, buffer_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader shader("D:\\OpenGL\\OpenGL\\Framebuffers\\shader.vert",
        "D:\\OpenGL\\OpenGL\\Framebuffers\\shader.frag");

    Shader screen_shader("D:\\OpenGL\\OpenGL\\Framebuffers\\screen_shader.vert",
        "D:\\OpenGL\\OpenGL\\Framebuffers\\screen_shader.frag");

    cube_models.push_back(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)));
    cube_models.push_back(glm::translate(glm::vec3(2.0f, 0.1f, -2.5f)));

	while (!(glfwWindowShouldClose(window))) {
		glfwPollEvents();

		camera.processInput(window);

        //first pass
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawScene(shader);

        //second pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screen_shader.use();
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        screen_shader.setInt("texture", 2);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices) / sizeof(float));
        screen_shader.unuse();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}

