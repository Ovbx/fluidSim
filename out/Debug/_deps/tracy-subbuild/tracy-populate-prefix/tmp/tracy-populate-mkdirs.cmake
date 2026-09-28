# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-src"
  "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-build"
  "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix"
  "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/tmp"
  "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp"
  "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src"
  "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
