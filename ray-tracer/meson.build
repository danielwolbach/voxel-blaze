project('voxel-ray-tracer', 'c', 'cpp')

dependencies = [
  dependency('glm'),
  dependency('openmp'),
]

source_files = [
  'source/box.cpp',
  'source/main.cpp',
  'source/octree.cpp',
  'source/ray.cpp',
  'source/renderer.cpp',
]

include_directories = include_directories('include')

executable('voxel-ray-tracer', source_files,
           include_directories: include_directories,
           dependencies: dependencies)

