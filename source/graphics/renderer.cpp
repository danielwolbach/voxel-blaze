#include <voxel-blaze/graphics/renderer.hpp>

Renderer::Renderer(Shader &&shader) : shader(std::move(shader))
{
    auto projection_transform = glm::perspective(glm::radians(45.0f), 1280.0f / 1280.0f, 0.1f, 10000.0f);
    this->shader.upload_transform("projection_transform", glm::value_ptr(projection_transform));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.0f);
}

float Renderer::draw(const Camera &camera, const Model &model)
{
    this->shader.upload_transform("view_transform", camera.matrix_ptr());
    this->shader.upload_transform("model_transform", glm::value_ptr(model.get_tranform()));

    const auto start_time = std::chrono::high_resolution_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader.handle);
    glBindVertexArray(model.vertex_array);
    glDrawElements(GL_TRIANGLES, model.vertex_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);

    const auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end_time - start_time;
    float delta_time = duration.count();

    return delta_time;
}
