#include <GLFW/glfw3.h>
#include <iostream>
#include <voxel_blaze/graphics/window.hpp>

using namespace voxel_blaze;

int main()
{
    Window window(1280, 720);

    while (!window.should_close())
    {
        window.update();
    }

    return 0;
}
