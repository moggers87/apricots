src = [
  'ai.cpp',
  'all.cpp', 
  'apricots.cpp',
  'collide.cpp', 
  'drak.cpp',
  'drawall.cpp',
  'fall.cpp',
  'finish.cpp',
  'game.cpp',
  'init.cpp',
  'sampleio.cpp',
  'SDLfont.cpp',
  'setup.cpp',
  'shape.cpp'
]

apricot_deps = [freealut_dep, m_dep, openal_dep, sdl_dep]

executable('apricots', src,
  dependencies: apricot_deps,
  install: true,
  include_directories: '..')

data = [
  'afterburner.wav',
  'alt-8x16.psf',
  'apricots.cfg',
  'apricots.shapes',
  'bomb.wav',
  'engine.wav',
  'explode.wav',
  'finish.wav',
  'fuelexplode.wav',
  'groundhit.wav',
  'gunshot.wav',
  'gunshot2.wav',
  'jet.wav',
  'laser.wav',
  'shot.wav',
  'splash.wav',
  'stall.wav'
]

install_data(data, install_dir: pkgdatadir)
