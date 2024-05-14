#define VOXEL_BLAZE_USE_EXTERNAL

#include <voxel_blaze/core/external.hpp>
#include <voxel_blaze/graphics/index_buffer.hpp>
#include <voxel_blaze/graphics/shader.hpp>
#include <voxel_blaze/graphics/utils.hpp>
#include <voxel_blaze/graphics/vertex_array.hpp>
#include <voxel_blaze/graphics/vertex_buffer.hpp>
#include <voxel_blaze/graphics/window.hpp>

using namespace voxel_blaze;

const char *vertexShaderSource = "#version 440 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 440 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                                   "}\0";

int main()
{
    Window window(1280, 720);

    Vec<u32> indices = {0, 2, 1};
    Vec<f32> vertices = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

    Shared<IndexBuffer> index_buffer = std::make_shared<IndexBuffer>(indices);
    Shared<VertexBuffer> vertex_buffer = std::make_shared<VertexBuffer>(vertices);

    VertexArray vertex_array(index_buffer);
    vertex_array.add_vertex_buffer(vertex_buffer);

    Shader shader(vertexShaderSource, fragmentShaderSource);

    while (!window.should_close())
    {
        window.update();

        shader.use();
        vertex_array.bind();

        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL_CHECK(glEnableVertexAttribArray(0));
        GL_CHECK(glDrawElements(GL_TRIANGLES, vertex_array.element_count(), GL_UNSIGNED_INT, nullptr));
    }

    return 0;
}
