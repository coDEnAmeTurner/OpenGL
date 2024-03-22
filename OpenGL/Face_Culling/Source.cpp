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
    float windowVertices[] = {
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
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

    GLuint windowVAO;
    glGenVertexArrays(1, &windowVAO);
    glBindVertexArray(windowVAO);
    GLuint windowBuffer;
    glGenBuffers(1, &windowBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, windowBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), windowVertices, GL_STATIC_DRAW);
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

    int windowWidth, windowHeight, windowComp;
    unsigned char* window_tex = stbi_load("D:\\OpenGL\\OpenGL\\Textures\\blending_transparent_window.png", &windowWidth, &windowHeight, &windowComp, 0);
    if (!window_tex) {
        std::cout << "load window texture failed" << std::endl;
        stbi_image_free(window_tex);
    }
    unsigned int windowTex;
    glGenTextures(1, &windowTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, windowTex);
    if (windowComp == 1)
        colorFormat = GL_RED;
    if (windowComp == 4)
        colorFormat = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, windowWidth, windowHeight, 0, colorFormat, GL_UNSIGNED_BYTE, window_tex);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(window_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Shader shader("D:\\OpenGL\\OpenGL\\Blending\\shader.vert", 
        "D:\\OpenGL\\OpenGL\\Blending\\shader.frag");

    std::vector<glm::mat4> cube_models;
    cube_models.push_back(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)));
    cube_models.push_back(glm::translate(glm::vec3(2.0f, 0.1f, -2.5f)));

    std::vector<glm::vec3> window_poss;
    window_poss.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    window_poss.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    window_poss.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    window_poss.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    window_poss.push_back(glm::vec3(1.5f, 0.0f, -0.6f));

    std::map<float, glm::vec3> window_poss_sorted;
    for (const glm::vec3& pos : window_poss) {
        float distance = glm::length(-pos + camera.getCameraPos());
        window_poss_sorted[distance] = pos;
    }

	while (!(glfwWindowShouldClose(window))) {
		glfwPollEvents();

		camera.processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

        glBindVertexArray(windowVAO);
        shader.setInt("texture", 2);
        if (camera.getCameraDirection().z > 0)
            for (std::map<float, glm::vec3>::iterator i = window_poss_sorted.begin(); i != window_poss_sorted.end(); i++) {
                shader.setMat4("model", glm::translate(i->second));
                glDrawArrays(GL_TRIANGLES, 0, sizeof(windowVertices) / sizeof(float));
            }
        else
            for (std::map<float, glm::vec3>::reverse_iterator i = window_poss_sorted.rbegin(); i != window_poss_sorted.rend(); i++) {
                shader.setMat4("model", glm::translate(i->second));
                glDrawArrays(GL_TRIANGLES, 0, sizeof(windowVertices) / sizeof(float));
            }
        shader.unuse();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}

