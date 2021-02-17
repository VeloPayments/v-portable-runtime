project('vpr', 'c', 'cpp',
  version : '0.4.2-snapshot',
  default_options : ['c_std=c11', 'cpp_std=c++14', 'buildtype=release'],
  meson_version : '>=0.49.0'
)

message('Using build type: ' + get_option('buildtype'))

force_velo_toolchain = get_option('force_velo_toolchain')

if force_velo_toolchain
  cc = meson.get_compiler('c')
  cc_version = run_command(cc.cmd_array() + ['--version'], check : true).stdout()
  if not cc_version.contains('(velo)')
    error('force_velo_toolchain is set and you aren\'t using the velo toolchain.')
  endif
  cpp = meson.get_compiler('cpp')
  cpp_version = run_command(cpp.cmd_array() + ['--version'], check : true).stdout()
  if not cpp_version.contains('(velo)')
    error('force_velo_toolchain is set and you aren\'t using the velo toolchain.')
  endif
else
  warning('Not using the blessed velo gcc toolchain. Here be dragons.')
endif

add_project_arguments('-Wall', '-Werror', '-Wextra', language : 'c')
add_project_arguments('-Wall', '-Werror', '-Wextra', language : 'cpp')

src = run_command('find', './src', '-name', '*.c', check : true).stdout().strip().split('\n')
test_src = run_command('find', './test', '-name', '*.cpp', check : true).stdout().strip().split('\n')

# GTest is currently only used on native x86 builds. Creating a disabler will disable the test exe and test target.
if meson.is_cross_build()
  gtest = disabler()
else
  gtest = dependency('gtest', main : true, required : true, fallback : ['gtest','gtest_dep'])
endif

vcmodel = dependency(
  'vcmodel',
  required : true,
  fallback : ['vcmodel', 'vcmodel_dep']
)

vcmodel_sub = subproject('vcmodel')
vcmodel_include = vcmodel_sub.get_variable('vcmodel_include')

vpr_include = include_directories('include')

vpr_lib = static_library('vpr', src,
  dependencies : vcmodel,
  include_directories : [vpr_include, vcmodel_include]
)

vpr_dep = declare_dependency(
  link_with : vpr_lib,
  include_directories : [vpr_include, vcmodel_include]
)

vpr_test = executable(
  'testvpr',
  test_src,
  include_directories : [vpr_include, vcmodel_include],
  link_with : vpr_lib,
  dependencies : gtest
)

test('testvpr', vpr_test, timeout : 300)

#Model checking
run_target(
    'model-check',
    command : ['scripts/model-check.sh', 'models', meson.current_source_dir()])

# vim: ts=2 sw=2 et colorcolumn=120
