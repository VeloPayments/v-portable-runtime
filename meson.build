project(
  'vpr', 
  'c', 
  'cpp',
  default_options : ['c_std=c11', 'cpp_std=c++14'],
  meson_version : '>=0.49.0'
)

force_velo_toolchain = get_option('force_velo_toolchain')

if force_velo_toolchain                                                              
  cc = meson.get_compiler('c')                                                       
  cc_version = run_command(cc.cmd_array() + ['--version'], check : true).stdout()
  if not cc_version.contains('(velo)')                                                 
    error('force_velo_toolchain is set and you arn\'t using the velo toolchain.')    
  endif                                                                                                                                                                                       
  cpp = meson.get_compiler('cpp')
  cpp_version = run_command(cc.cmd_array() + ['--version'], check : true).stdout()
  if not cpp_version.contains('(velo)')
    error('force_velo_toolchain is set and you arn\'t using the velo toolchain.')
  endif
else
  warning('Not using the blessed velo gcc toolchain. Here be dragons.')
endif

add_project_arguments('-Wall', '-Werror', '-Wextra', language : 'c')
add_project_arguments('-Wall', '-Werror', '-Wextra', language : 'cpp')

src = run_command('find', './src', '-name', '*.c', check : true).stdout().strip().split('\n')
test_src = run_command('find', './test', '-name', '*.cpp', check : true).stdout().strip().split('\n')

gtest = dependency('gtest', main : true, required : true, fallback : ['gtest','gtest_dep'])

vcmodel = dependency(
  'vcmodel',
  required : true,
  fallback : ['vcmodel', 'vcmodel_dep']
)

vpr_include = include_directories('include')

vpr_lib = static_library(
  'vpr', 
  src,
  dependencies : vcmodel,
  include_directories : vpr_include
)

vpr_test = executable(
  'testvpr',
  test_src,
  include_directories : vpr_include,
  link_with : vpr_lib,
  dependencies : gtest
)

vpr_dep = declare_dependency(
  link_with : vpr_lib, 
  include_directories : vpr_include
)

test('testvpr', vpr_test)
