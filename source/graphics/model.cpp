#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/shader.hpp>

Model::Model(const Mesh &mesh) : vertex_count(mesh.indices.size())
{
    if (vertex_count == 0)
    {
        spdlog::warn("There are no vertices in the provided mesh");
    }

    // Prepare vertex buffer.
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    // Prepare vertex array.
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    // Prepare index buffer.
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned), &mesh.indices[0], GL_STATIC_DRAW);

    // Set up vertex attributes.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void *)(3 * sizeof(float)));

    // Clean up.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Model::~Model()
{
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
}

void Model::rotate(const glm::vec3 angles)
{
    transform = glm::rotate(transform, angles.x, glm::vec3(1.0, 0.0, 0.0));
    transform = glm::rotate(transform, angles.y, glm::vec3(0.0, 1.0, 0.0));
    transform = glm::rotate(transform, angles.z, glm::vec3(0.0, 0.0, 1.0));
}

glm::mat4 Model::get_tranform() const
{
    return transform;
}