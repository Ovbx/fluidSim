cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum policy version" FORCE)

project(FluidSim VERSION 1.0 LANGUAGES CXX)

#global settings via standard properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

find_package(glfw3 QUIET)
if (NOT glfw3_FOUND)
  message(STATUS "GLFW3 not found locally. Downloading via FetchContent..")
  set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
  set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
  set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")

  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG       3.5.1
  )
  FetchContent_MakeAvailable(glfw)
endif()

find_package(glm QUIET)
if (NOT glm_FOUND)
  message(STATUS "GLM not found locally. Downloading via FetchContent...")
  FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
  )
  FetchContent_MakeAvailable(glm)
endif()

set(GLAD_INSTALL ON CACHE BOOL "Install glad" FORCE)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile" FORCE)
set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for" FORCE)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

find_package(tracy QUIET)
if(NOT tracy_FOUND)
  message(STATUS "Tracy profiler not found locally... Downloading via FetchContent...")
  set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy profiling" FORCE)
  FetchContent_Declare(
    tracy
    GIT_REPOSITORY https://github.com/wolfpld/tracy
    GIT_TAG v0.14.1
  )
  FetchContent_MakeAvailable(tracy)
endif()

find_package(OpenGL REQUIRED)

#application target
add_executable(FluidSim
  src/main.cpp
  src/Camera.cpp
  src/Fluid.cpp
  src/Input.cpp
  src/Mesh.cpp
  src/Renderer.cpp
  src/Shader.cpp
  src/Window.cpp
)

#include directories
target_include_directories(FluidSim PRIVATE include)

target_link_libraries(FluidSim PRIVATE
  OpenGL::GL
  glad
  ${CMAKE_DL_LIBS}
  $<$<TARGET_EXISTS:glfw>:glfw> $<$<NOT:$<TARGET_EXISTS:glfw>>:glfw3>
    $<$<TARGET_EXISTS:glm::glm>:glm::glm> $<$<NOT:$<TARGET_EXISTS:glm::glm>>:glm>
  Tracy::TracyClient
)

target_compile_definitions(FluidSim PRIVATE TRACY_ENABLE)
if (MSVC)
  target_compile_options(FluidSim PRIVATE /W4)
else()
  target_compile_options(FluidSim PRIVATE -Wall -Wextra -pedantic -Wno-unused-variable -Wno-unused-parameter -O2)

endif()



