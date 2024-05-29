#include <voxel-blaze/graphics/renderer.hpp>

Renderer::Renderer(Shader &&shader) : shader(std::move(shader))
{
    auto model_transform = glm::mat4(1.0f);
    this->shader.upload_transform("model_transform", glm::value_ptr(model_transform));

    auto projection_transform = glm::perspective(glm::radians(45.0f), 720.0f / 720.0f, 0.1f, 10000.0f);
    this->shader.upload_transform("projection_transform", glm::value_ptr(projection_transform));
}

float Renderer::draw(const Camera &camera, const Model &model)
{
    this->shader.upload_transform("view_transform", camera.matrix_ptr());

    auto start_time = std::chrono::high_resolution_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader.handle);
    glBindVertexArray(model.vertex_array);
    glDrawElements(GL_TRIANGLES, model.vertex_count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(last_time - start_time).count();

    return delta_time;
}