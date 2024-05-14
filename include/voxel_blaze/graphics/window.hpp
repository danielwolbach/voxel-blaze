#pragma once

namespace voxel_blaze
{

class Window
{
  public:
    Window(unsigned width, unsigned height);

    ~Window();

    void update() const;

    bool should_close() const;

    Window(const Window &other) = delete;
    Window &operator=(const Window &other) = delete;

  private:
    void *handle;
};

}
