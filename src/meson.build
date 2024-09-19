project('three-point-pos', 'cpp', default_options: ['cpp_std=c++20'])

fmt_dep = dependency('fmt', method: 'pkg-config')
ftxui_dep = dependency('ftxui', method: 'pkg-config')

tp_pos_deps = [ fmt_dep, ftxui_dep ]
tp_pos_files = [ 'main.cc' ]
executable('pos', tp_pos_files, dependencies: tp_pos_deps)