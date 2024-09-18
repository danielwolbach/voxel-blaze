#include <voxel-blaze/graphics/rasterizer.hpp>

Rasterizer::Rasterizer(const VoxelGrid &voxel_grid)
    : model(voxel_grid.meshify_greedy())
    , shader(consts::vertex_shader_source, consts::fragment_shader_source)
{
    auto projection_transform = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);
    shader.upload_transform("projection_transform", glm::value_ptr(projection_transform));

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glLineWidth(2.0f);
}

void Rasterizer::render(const OrbitCamera &camera) const
{
    this->shader.upload_transform("view_transform", camera.matrix_ptr());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader.handle);
    glBindVertexArray(model.vertex_array);
    glDrawElements(GL_TRIANGLES, model.vertex_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}
