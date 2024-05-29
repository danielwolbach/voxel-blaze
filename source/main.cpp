#include <voxel-blaze/consts.hpp>
#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/renderer.hpp>
#include <voxel-blaze/graphics/shader.hpp>
#include <voxel-blaze/graphics/window.hpp>
#include <voxel-blaze/voxels/array_voxel_grid.hpp>

const unsigned cubic_size = 2;

int main()
{
    Window window(720, 720);
    Shader shader(consts::vertex_shader_source, consts::fragment_shader_source);
    Renderer renderer(std::move(shader));

    Camera camera;
    camera.look_at(glm::vec3(cubic_size * 2), glm::vec3(0));

    ArrayVoxelGrid voxel_grid(cubic_size, cubic_size, cubic_size);
    voxel_grid.fill_cuboid(Voxel{});
    Model model = voxel_grid.meshify_direct();

    while (window.opened())
    {
        renderer.draw(camera, model);
    }
}
