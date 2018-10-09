cmake_minimum_required(VERSION 3.0.0)

project(CubeSimulator)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(CubeSimulator
  src/cube.cpp
  src/inputhandler.cpp
  src/main.cpp
  src/viewport.cpp
)

target_include_directories(CubeSimulator PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(CubeSimulator ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} glfw)
