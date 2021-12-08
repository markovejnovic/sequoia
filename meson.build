project('sequoia', 'cpp',
  version : '0.1',
  default_options : [ 'cpp_std=c++17' ],
)

opencvdep = dependency('opencv4')
boostdep = dependency('boost', modules: ['thread'])
threaddep = dependency('threads')

so_libs = [
  'lib/libraylib.so',
]

sources = [
  'src/sequoia.cpp',
  'src/config.cpp',
  'src/logging.cpp',
  'src/mainui.cpp',
  'src/event.cpp',
  'src/AppThreadManager.cpp',
  'src/context.cpp',
  'src/ImageReader.cpp',
  'src/ImageProcessor.cpp',
  'src/color.cpp',
  'src/drawing.cpp',
  'src/uicontext.cpp',
  'src/raygui.cpp',
  'src/ocv2ray.cpp',
  'src/filters.cpp',
  'src/Dimensioner.cpp',
  'src/image.cpp',
  'src/cv2debugging.cpp'
]

add_global_arguments([
  '-DSEQUOIA_STATIC_DEBUG',
  '-DSAVE_IMAGES'
], language: 'cpp')

exe = executable('sequoia', sources,
  install: true, dependencies: [opencvdep, boostdep, threaddep],
  objects: so_libs,
  include_directories: include_directories(['include', 'lib/include'])
)

trainer = executable('sequoia_trainer', [
    'src/sequoia_trainer.cpp',
    'src/context.cpp',
    'src/logging.cpp',
    'src/filters.cpp',
    'src/config.cpp'],
  install: true,
  dependencies: [opencvdep],
  include_directories: include_directories(['include'])
)
