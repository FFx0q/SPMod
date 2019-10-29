project('sqlite', ['c', 'cpp'],
                  license : 'GPLv3',
                  meson_version : '>=0.49.0',
                  version : '0.2.0-dev')

add_project_link_arguments(['-ldl', '-pthread'], language : [ 'c', 'cpp' ])

sourceFiles = files('ext.cpp')

publicIncludesDir = [
    join_paths('..', '..', 'include', 'public')
]

sqliteIncludesDir = [
    join_paths('sqlite')
]

includeDirs = include_directories(publicIncludesDir,
                                  sqliteIncludesDir,
                                  is_system : true) #build directory

shared_library('sqlitemodule', sourceFiles, include_directories : includeDirs)

subdir('sqlite')
