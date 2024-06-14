#include <voxel-blaze/consts.hpp>
#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/renderer.hpp>
#include <voxel-blaze/graphics/shader.hpp>
#include <voxel-blaze/graphics/window.hpp>
#include <voxel-blaze/parsers/vox_parser.hpp>
#include <voxel-blaze/voxels/array_voxel_grid.hpp>

// TODO Handle VOX file with multiple "frames".
// TODO Handle alpha transparency.
// TODO Implement orbit camera.

const unsigned cubic_size = 16;

int main()
{
    spdlog::set_level(spdlog::level::info);
    Window window(720, 720);
    Shader shader(consts::vertex_shader_source, consts::fragment_shader_source);
    Renderer renderer(std::move(shader));

    Camera camera;

    VoxParser parser("resources/monu.vox");
    const auto voxel_grid = parser.get_voxel_grid();
    Model model = voxel_grid->meshify_greedy();

    // auto voxel_grid = ArrayVoxelGrid(cubic_size, cubic_size, cubic_size);
    // voxel_grid.fill_ellipsoid(Voxel{1.0f, 1.0f, 1.0f});
    // // voxel_grid.set_voxel(0, 0, 0, std::nullopt);
    // // voxel_grid.set_voxel(0, 1, 0, std::nullopt);
    // // voxel_grid.set_voxel(0, cubic_size / 2, cubic_size / 2, std::nullopt);
    // auto model = Model(voxel_grid.meshify_greedy());
    // // model.rotate(glm::vec3(-glm::pi<float>() / 2.0f, 0.0f, 0.0f));
    // //model.translate(glm::vec3(cubic_size / 2, cubic_size / 2, cubic_size / 2));

    camera.look_at(glm::vec3(cubic_size / 2, cubic_size / 2, voxel_grid->max_size() * 2), glm::vec3(0));

    while (window.opened())
    {
        const auto delta_time = renderer.draw(camera, model);
        const auto speed = delta_time * 10;

        if (window.key_down(GLFW_KEY_Q))
        {
            if (window.key_down(GLFW_KEY_LEFT_SHIFT))
            {
                model.rotate(glm::vec3(-speed, 0.0f, 0.0f));
            }
            else
            {
                model.rotate(glm::vec3(speed, 0.0f, 0.0f));
            }
        }
        else if (window.key_down(GLFW_KEY_W))
        {
            if (window.key_down(GLFW_KEY_LEFT_SHIFT))
            {
                model.rotate(glm::vec3(0.0f, -speed, 0.0f));
            }
            else
            {
                model.rotate(glm::vec3(0.0f, speed, 0.0f));
            }
        }
        else if (window.key_down(GLFW_KEY_E))
        {
            if (window.key_down(GLFW_KEY_LEFT_SHIFT))
            {
                model.rotate(glm::vec3(0.0f, 0.0f, -speed));
            }
            else
            {
                model.rotate(glm::vec3(0.0f, 0.0f, speed));
            }
        }
    }
}
