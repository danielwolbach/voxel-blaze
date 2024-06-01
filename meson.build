project('voxel-blaze', 'c', 'cpp')

dependencies = [
  dependency('glfw3'),
  dependency('spdlog'),
  dependency('glm'),
  dependency('fmt'),
]

source_files = [
  'source/main.cpp',
  'source/consts.cpp',
  'source/graphics/window.cpp',
  'source/graphics/shader.cpp',
  'source/graphics/model.cpp',
  'source/graphics/renderer.cpp',
  'source/graphics/vertex.cpp',
  'source/graphics/camera.cpp',
  'source/voxels/voxel_grid.cpp',
  'source/voxels/array_voxel_grid.cpp',
  'source/parsers/vox_parser.cpp',
  'lib/glad.c',
]

include_directories = include_directories('include')

executable('voxel-blaze', source_files, dependencies: dependencies, include_directories: include_directories)

