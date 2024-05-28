project('voxel-blaze', 'c', 'cpp')

dependencies = [
  dependency('glfw3'),
  dependency('spdlog'),
  dependency('glm'),
]

source_files = [
  'source/main.cpp',
  'source/window.cpp',
  'source/shader.cpp',
  'source/model.cpp',
  'source/voxel.cpp',
  'lib/glad.c',
]

include_directories = include_directories('include')

executable('voxel-blaze', source_files, dependencies: dependencies, include_directories: include_directories)

