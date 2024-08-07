#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <fmt/core.h>
#include <fstream>
#include <functional>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>
#include <vector>

class Wrapper
{
  public:
    Wrapper() = default;
    Wrapper(const Wrapper &) = delete;
    Wrapper &operator=(const Wrapper &) = delete;
};
