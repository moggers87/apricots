project('apricots', 'cpp',
  license: 'GPL2',
  version: '0.2.7',
  meson_version: '>= 0.50.0'
)

prefix = get_option('prefix')
datadir = join_paths(prefix, get_option('datadir'))
desktopdir = join_paths(datadir, 'applications')
pkgdatadir = join_paths(datadir, meson.project_name())
sysconfdir = get_option('sysconfdir')

conf = configuration_data()
conf.set_quoted('AP_PATH', pkgdatadir + '/')
conf.set10('AP_AUDIO_OPENAL', true)
config_h = configure_file(configuration: conf, output: 'config.h')

freealut_dep = dependency('freealut')
sdl_dep = dependency('sdl')
openal_dep = dependency('openal')

cxx = meson.get_compiler('cpp')
m_dep = cxx.find_library('m')

subdir('apricots')
subdir('contrib')
