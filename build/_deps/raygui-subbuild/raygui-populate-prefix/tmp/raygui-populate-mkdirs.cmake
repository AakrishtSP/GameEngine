# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/ishan/GameEngine/build/_deps/raygui-src")
  file(MAKE_DIRECTORY "/home/ishan/GameEngine/build/_deps/raygui-src")
endif()
file(MAKE_DIRECTORY
  "/home/ishan/GameEngine/build/_deps/raygui-build"
  "/home/ishan/GameEngine/build/_deps/raygui-subbuild/raygui-populate-prefix"
  "/home/ishan/GameEngine/build/_deps/raygui-subbuild/raygui-populate-prefix/tmp"
  "/home/ishan/GameEngine/build/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp"
  "/home/ishan/GameEngine/build/_deps/raygui-subbuild/raygui-populate-prefix/src"
  "/home/ishan/GameEngine/build/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ishan/GameEngine/build/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ishan/GameEngine/build/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
