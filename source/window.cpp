#include "voxel-blaze/common.hpp"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <voxel-blaze/window.hpp>

Window::Window(const unsigned width, const unsigned height)
{
    if (!glfwInit())
    {
        spdlog::error("Failed to initialize GLFW");
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    handle = glfwCreateWindow(width, height, "Voxel Blaze", nullptr, nullptr);

    if (handle == nullptr)
    {
        spdlog::error("Failed to create window");
    }

    glfwMakeContextCurrent(static_cast<GLFWwindow *>(handle));
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
    glfwDestroyWindow(static_cast<GLFWwindow *>(handle));
    glfwTerminate();
}

bool Window::opened() const
{
    glfwSwapBuffers(static_cast<GLFWwindow *>(handle));
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return !glfwWindowShouldClose(static_cast<GLFWwindow *>(handle));
}

bool Window::key_down(int key) const
{
    return glfwGetKey(static_cast<GLFWwindow *>(handle), key) == GLFW_PRESS;
}
