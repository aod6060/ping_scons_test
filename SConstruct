# Generated Version Header File

import version

version_info = {
    "short_name": str(version.short_name),
    "name": str(version.name),
    "major": int(version.major),
    "minor": int(version.minor),
    "patch": int(version.patch),
    "status": str(version.status),
    "build": str(version.build),
    "year": int(version.year)
}



fp = open("src/engine/version.gen.h", "w")
fp.write(
    """
// Don't edit this file because its generated from buildsystem.
#ifndef VERSION_GEN_H
#define VERSION_GEN_H

#define VERSION_SHORT_NAME "{short_name}"
#define VERSION_NAME "{name}"
#define VERSION_MAJOR {major}
#define VERSION_MINOR {minor}
#define VERSION_PATCH {patch}
#define VERSION_STATUS "{status}"
#define VERSION_BUILD "{build}"
#define VERSION_YEAR {year}

#endif
    """.format(**version_info)
)

fp.close()

env = Environment(
    TARGET_ARCH='x86',
    CPPPATH="include",
    LIBPATH="lib",
    LIBS=[
        'SDL2main',
        'SDL2',
        'SDL2_image',
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

env["source_files"] = []

target="#/bin/run"

# Compile 3rd Party Libraries
SConscript("#/src/3rd/imgui/SConscript", exports="env")
SConscript("#/src/3rd/jsoncpp/SConscript", exports="env")

SConscript("#/src/SConscript", exports="env")
SConscript("#/src/engine/SConscript", exports="env")
SConscript("#/src/game/SConscript", exports="env")

env.Program(target=target, source=env["source_files"])