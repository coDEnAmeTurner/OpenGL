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
GLuint cubeVAO;
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
    shader.setInt("texture", 0);
    shader.setMat4("model", glm::translate(glm::vec3(0, 0, 0)));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(cubeVAO);
    shader.setInt("texture", 0);
    for (const glm::mat4& model : cube_models) {
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
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

    GLFWwindow* window = glfwCreateWindow(buffer_width, buffer_height, "Cubemaps Demo", NULL, NULL);
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

    GLenum colorFormat = GL_RGB;
    int containerWidth, containerHeight, containerComp;
    stbi_set_flip_vertically_on_load(true);
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

    int width, height, nrChannels;
    std::vector<std::string> faces{
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\right.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\left.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\top.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\bottom.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\front.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\back.jpg",

    };
    unsigned char* data;
    unsigned int cubeTex;
    GLenum format = GL_RED;
    glGenTextures(1, &cubeTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
    for (int i = 0; i < faces.size(); i++) {
        data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            if (nrChannels == 3) format = GL_RGB;
            if (nrChannels == 4) format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            std::cout << "Cubemap failed to load at " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    Shader shader("D:\\OpenGL\\OpenGL\\Cubemaps\\shader.vert",
        "D:\\OpenGL\\OpenGL\\Cubemaps\\shader.frag");

    cube_models.push_back(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)));

    while (!(glfwWindowShouldClose(window))) {
        glfwPollEvents();

        camera.processInput(window);

        //first pass
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawScene(shader);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}

