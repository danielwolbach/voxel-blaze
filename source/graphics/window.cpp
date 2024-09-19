#include <voxel-blaze/graphics/window.hpp>

void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam)
{
    std::cerr << "OpenGL Debug Message: " << message << std::endl;
}

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
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
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
    return !glfwWindowShouldClose(static_cast<GLFWwindow *>(handle));
}

bool Window::key_down(int key) const
{
    return glfwGetKey(static_cast<GLFWwindow *>(handle), key) == GLFW_PRESS;
}
