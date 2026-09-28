# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/tracy-populate-gitclone-lastrun.txt" AND EXISTS "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/tracy-populate-gitinfo.txt" AND
  "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/tracy-populate-gitclone-lastrun.txt" IS_NEWER_THAN "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/tracy-populate-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/tracy-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"
            clone --no-checkout --config "advice.detachedHead=false" "https://github.com/wolfpld/tracy" "tracy-src"
    WORKING_DIRECTORY "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/wolfpld/tracy'")
endif()

execute_process(
  COMMAND "/usr/bin/git"
          checkout "v0.14.1" --
  WORKING_DIRECTORY "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'v0.14.1'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-src"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/tracy-populate-gitinfo.txt" "/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/tracy-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/simplemonke/Documents/C++Projects/fluidSim/out/Debug/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/tracy-populate-gitclone-lastrun.txt'")
endif()
