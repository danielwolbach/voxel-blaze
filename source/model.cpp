#include <voxel-blaze/model.hpp>
#include <voxel-blaze/shader.hpp>

Model::Model(const std::vector<unsigned> &indices, const std::vector<Vertex> &vertices) : vertex_count(indices.size())
{
    // Prepare vertex buffer.
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Prepare vertex array.
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    // Prepare index buffer.
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

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
