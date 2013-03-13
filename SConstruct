# scons 2.7

env = Environment()

env.Append(CPPFLAGS="-Wall -fPIC -g -std=c++11")
env.Append(CPPDEFINES="GLEW_NO_GLU")
env.Append(CPPPATH=["Engine/include/"])
#these libraries' paths are added automatically by linux
#env.Append(CPPPATH="dependencies/glm-0.9.4.1/")
#env.Append(CPPPATH="dependencies/glew-1.9.0/include/")
#env.Append(CPPPATH="dependencies/FreeImage/include/")

cpp_files = Glob("Engine/src/*.cpp")
lib = env.StaticLibrary(target='Engine', source = cpp_files)


#SFML subpart

env.Append(CPPPATH="dependencies/SFML-2.0/include/")

sfml_files = Glob("dependencies/SFML-2.0/src/System/*.cpp")
sfml_files += Glob("dependencies/SFML-2.0/src/System/Unix/*.cpp")
sfml_files += Glob("dependencies/SFML-2.0/src/Window/*.cpp")
sfml_files += Glob("dependencies/SFML-2.0/src/Window/Linux/*.cpp")

libsfml = env.StaticLibrary(target='SFML', source = sfml_files)
