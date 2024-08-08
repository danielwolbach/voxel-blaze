project('voxel-ray-tracer', 'c', 'cpp')

dependencies = [
  dependency('glfw3', static: true),
  dependency('spdlog', static: true),
  dependency('glm', static: true),
  dependency('glew', static: true),
]

source_files = [
    'main.cpp'
]

executable('voxel-ray-tracer', source_files, dependencies: dependencies, install: true)