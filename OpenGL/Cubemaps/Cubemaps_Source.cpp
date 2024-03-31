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
GLuint cubeVAO, skyBoxVAO;

std::vector<glm::mat4> cube_models;
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.calculateDirection(xpos, ypos);
}

void DrawScene(Shader& shader, unsigned int vao, unsigned int texture, GLenum texture_target, 
    int vertices_count, glm::mat4 view, glm::mat4 model) 
{
    shader.setMat4("projection", glm::perspective(45.0f, (float)buffer_width / buffer_height, 0.1f, 100.0f));
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    glBindVertexArray(vao);
    glBindTexture(texture_target, texture);
    glDrawArrays(GL_TRIANGLES, 0, vertices_count);
}

int loadCubemap(const std::vector<std::string>& faces) {
    int width, height, nrChannels;
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
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format,
                GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
        else {
            std::cout << "Cubemap failed to load at " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    return cubeTex;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &skyBoxVAO);
    glBindVertexArray(skyBoxVAO);
    GLuint skyBoxBuffer;
    glGenBuffers(1, &skyBoxBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, skyBoxBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    GLenum colorFormat = GL_RGB;
    int containerWidth, containerHeight, containerComp;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* container = stbi_load("D:\\OpenGL\\OpenGL\\Textures\\container.jpg", &containerWidth, &containerHeight, &containerComp, 0);
    if (!container) {
        std::cout << "load container texture failed" << std::endl;
        stbi_image_free(container);
    }
    unsigned int containerTex;
    glGenTextures(1, &containerTex);
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

    std::vector<std::string> faces{
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\right.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\left.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\top.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\bottom.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\front.jpg",
        "D:\\OpenGL\\OpenGL\\Textures\\skybox\\back.jpg",

    };
    
    int sky_box_tex = loadCubemap(faces);

    Shader shader("D:\\OpenGL\\OpenGL\\Cubemaps\\shader.vert",
        "D:\\OpenGL\\OpenGL\\Cubemaps\\shader.frag");

    Shader skybox_shader("D:\\OpenGL\\OpenGL\\Cubemaps\\sky_box_shader.vert",
        "D:\\OpenGL\\OpenGL\\Cubemaps\\sky_box_shader.frag");

    cube_models.push_back(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)));

    while (!(glfwWindowShouldClose(window))) {
        glfwPollEvents();

        camera.processInput(window);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //first pass
        shader.use();
        shader.setVec3("camera_pos", camera.getCameraPos());
        DrawScene(shader, cubeVAO, sky_box_tex, GL_TEXTURE_CUBE_MAP, 36, camera.calculateView(),
            glm::translate(glm::scale(glm::vec3(2.0, 2.0, 2.0)), glm::vec3(0, 0, 0)));
        shader.unuse();

        ////second
        skybox_shader.use();
        DrawScene(skybox_shader, skyBoxVAO, sky_box_tex, GL_TEXTURE_CUBE_MAP, 36, 
            glm::mat4(glm::mat3(camera.calculateView())), glm::translate(glm::vec3(0, 0, 0)));
        skybox_shader.unuse();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}

