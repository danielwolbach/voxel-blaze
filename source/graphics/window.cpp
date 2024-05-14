#define VOXEL_BLAZE_USE_EXTERNAL

#include <voxel_blaze/core/external.hpp>
#include <voxel_blaze/core/types.hpp>
#include <voxel_blaze/graphics/window.hpp>

namespace voxel_blaze
{
    Window::Window(unsigned width, unsigned height)
    {
        if (glfwInit() != GLFW_TRUE)
        {
            throw RuntimeError("Failed to initialize GLFW.");
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        handle = glfwCreateWindow(width, height, "Voxel Blaze", nullptr, nullptr);

        if (handle == nullptr)
        {
            throw RuntimeError("Failed to create window.");
        }

        glfwMakeContextCurrent(static_cast<GLFWwindow *>(handle));
        glfwSwapInterval(1);

        if (u32 error = glewInit())
        {
            const char *error_string = reinterpret_cast<const char *>(glewGetErrorString(error));
            throw RuntimeError(fmt::format("Failed to initialize GLEW: {}", error_string));
        }

        glViewport(0, 0, width, height);
    }

    Window::~Window()
    {
        glfwDestroyWindow(static_cast<GLFWwindow *>(handle));
        glfwTerminate();
    }

    void Window::update() const
    {
        glfwPollEvents();
        glfwSwapBuffers(static_cast<GLFWwindow *>(handle));
    }

    bool Window::should_close() const
    {
        return glfwWindowShouldClose(static_cast<GLFWwindow *>(handle));
    }

}