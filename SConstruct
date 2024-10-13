env = Environment(
    TARGET_ARCH='x86',
    CPPPATH="include",
    LIBPATH="lib",
    LIBS=[
        'SDL2main',
        'SDL2',
        'opengl32',
        'glew32',
        'kernel32.lib',
        'user32.lib',
        'gdi32.lib',
        'winspool.lib',
        'comdlg32.lib',
        'advapi32.lib' ,
        'shell32.lib',
        'ole32.lib',
        'oleaut32.lib',
        'uuid.lib',
        'odbc32.lib',
        'odbccp32.lib'
    ],
    CPPFLAGS=[
        '/std:c++17',
        '/Gd',
        '/EHsc',
        '/fp:precise',
        '/MD',
        '/sdl'
    ],
    LINKFLAGS=[
        '/SUBSYSTEM:CONSOLE',
        '/MACHINE:X86'])

target="#/bin/run"

src_files = [
    '#/src/main.cpp',
    '#/src/app.cpp',
    '#/src/input.cpp',
    "#/src/game.cpp"
]

env.Program(target=target, source=src_files)