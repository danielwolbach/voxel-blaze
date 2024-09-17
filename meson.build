project('voxel-blaze', 'c', 'cpp')

dependencies = [
  dependency('glfw3'),
  dependency('spdlog'),
  dependency('glm'),
]

source_files = [
  'lib/glad.c',
  'source/consts.cpp',
  'source/graphics/camera.cpp',
  'source/graphics/model.cpp',
  'source/graphics/rasterizer.cpp',
  'source/graphics/ray_tracer.cpp',
  'source/graphics/shader.cpp',
  'source/graphics/vertex.cpp',
  'source/graphics/window.cpp',
  'source/main.cpp',
  'source/parsers/vox_parser.cpp',
  'source/voxels/array_voxel_grid.cpp',
  'source/voxels/voxel_grid.cpp',
]

include_directories = include_directories('include')

executable('voxel-blaze', source_files, dependencies: dependencies, include_directories: include_directories)

