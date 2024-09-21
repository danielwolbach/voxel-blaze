#pragma once

#include <voxel-blaze/common.hpp>

class Window : Wrapper
{
  public:
    Window(const unsigned width, const unsigned height);
    ~Window();
    bool opened() const;
    bool key_down(int key) const;
    unsigned get_width() const;
    unsigned get_height() const;

  private:
    void *handle;
};
