#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>
#include <Camera.h>
#include <vector>
#include <map>
#include <Model.h>

const unsigned int buffer_width = 800, buffer_height = 600;
Camera camera;
float vertices[] = {
    -0.5, 0.5, 1, 0, 0,
    0.5, 0.5, 0, 1, 0,
    -0.5, -0.5, 0, 1, 1,
    0.5, -0.5, 0, 0, 1
};

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

    GLFWwindow* window = glfwCreateWindow(buffer_width, buffer_height, "Geometry Shader Demo", NULL, NULL);
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

    stbi_set_flip_vertically_on_load(true);
    Model backpack("D:\\OpenGL\\OpenGL\\backpack\\backpack.obj");

    Shader shader("D:\\OpenGL\\OpenGL\\Geometry_Shader\\shader.vert",
        "D:\\OpenGL\\OpenGL\\Geometry_Shader\\shader.frag", NULL);

    Shader normal_display_shader("D:\\OpenGL\\OpenGL\\Geometry_Shader\\normal_display.vert",
        "D:\\OpenGL\\OpenGL\\Geometry_Shader\\normal_display.frag", 
        "D:\\OpenGL\\OpenGL\\Geometry_Shader\\normal_display.geom");

    while (!(glfwWindowShouldClose(window))) {
        glfwPollEvents();
        camera.processInput(window);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("projection", glm::perspective(45.0f, (float)buffer_width / buffer_height, 0.1f, 100.0f));
        shader.setMat4("model", glm::translate(glm::scale(glm::vec3(1, 1, 1)), glm::vec3(0, 0, 0)));
        shader.setMat4("view", camera.calculateView());
        shader.setFloat("time", glfwGetTime());
        backpack.Draw(shader);
        shader.unuse();

        normal_display_shader.use();
        normal_display_shader.setMat4("projection", glm::perspective(45.0f, (float)buffer_width / buffer_height, 0.1f, 100.0f));
        normal_display_shader.setMat4("model", glm::translate(glm::scale(glm::vec3(1, 1, 1)), glm::vec3(0, 0, 0)));
        normal_display_shader.setMat4("view", camera.calculateView());
        normal_display_shader.setFloat("time", glfwGetTime());
        backpack.Draw(normal_display_shader);
        normal_display_shader.unuse();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;

}

