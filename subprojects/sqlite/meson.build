project('sqlite', ['c', 'cpp'],
                  license : 'GPLv3',
                  meson_version : '>=0.49.0',
                  version : '0.2.0-dev')

add_project_link_arguments(['-L.', '-ldl', '-pthread', '-lsqlite'], language : [ 'c', 'cpp' ])
rehldsIncludesDir = [
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'common'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'dlls'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'engine'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'game_shared'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'pm_shared'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'public'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'public', 'rehlds')
]

sourceFiles = files('ext.cpp', 'SQLiteHandler.cpp', 'SQLiteInterface.cpp', 'SQLiteModuleInterface.cpp')

publicIncludesDir = [
    join_paths('..', '..', 'include', 'public')
]

sqliteIncludesDir = [
    join_paths('sqlite')
]

includeDirs = include_directories(publicIncludesDir,
                                  sqliteIncludesDir,
                                  rehldsIncludesDir,
                                  is_system : true) #build directory

shared_library('sqlitemodule', sourceFiles, include_directories : includeDirs)

subdir('sqlite')
