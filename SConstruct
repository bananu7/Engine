# scons 2.7
import platform
import os

# set up your MinGW path here
mingw_path = "C:/DEV/MinGW32-4.8/mingw32/"
# set your Boost path here
boost_path = "C:/DEV/boost_1_52_0/"

if platform.system() == 'Windows':
    env = Environment(tools = ['mingw'], ENV = os.environ)
    env.PrependENVPath('PATH', mingw_path + "bin/")
    env.PrependENVPath('LIB', mingw_path + "lib/")
    
    # these aren't needed on *ix systems, since libraries are allready
    # in some sort of /usr/lib/
    env.Append(CPPPATH=[
        boost_path,
        "./dependencies/glm-0.9.4.1/",
        "./dependencies/glew-1.9.0/include/",
        "./dependencies/FreeImage/Dist/",
    ])
else:
    env = Environment(ENV = os.environ)

env.Append(CPPFLAGS="-std=c++11 ")
#env.Append(CPPFLAGS="-fdiagnostics-show-caret")
env.Append(CPPDEFINES="GLEW_NO_GLU")
env.Append(CPPPATH=["Engine/include/"])

cpp_files = Glob("Engine/src/*.cpp")
cpp_files += Glob("dependencies/glew-1.9.0/src/glew.c")

lib = env.StaticLibrary(target='Engine', source = cpp_files)

#SFML subpart

env.Append(CPPPATH="dependencies/SFML-2.0/include/")
env.Append(CPPDEFINES=[
    "SFML_STATIC",
    "GLEW_STATIC"
])

# SFML is compiled from sources, so we need to add it
sfml_files = Glob("dependencies/SFML-2.0/src/System/*.cpp")
sfml_files += Glob("dependencies/SFML-2.0/src/Window/*.cpp")

if platform.system() == 'Windows':
    sfml_files += Glob("dependencies/SFML-2.0/src/System/Win32/*.cpp")
    sfml_files += Glob("dependencies/SFML-2.0/src/Window/Win32/*.cpp")
else:
    sfml_files += Glob("dependencies/SFML-2.0/src/System/Unix/*.cpp")
    sfml_files += Glob("dependencies/SFML-2.0/src/Window/Linux/*.cpp")

libsfml = env.StaticLibrary(target='SFML', source = sfml_files)
