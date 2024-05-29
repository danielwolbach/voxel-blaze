#pragma once

#include <voxel-blaze/common.hpp>
#include <voxel-blaze/graphics/model.hpp>


class Shader : Wrapper
{
  public:
    Shader(const std::string &vertex_shader, const std::string &fragment_shader);
    Shader(Shader &&other);
    ~Shader();
    void upload_transform(const std::string &location, const float *data) const;

  private:
    friend class Renderer;
    unsigned handle = 0;
    unsigned compile(const std::string &source, const unsigned type) const;
};
