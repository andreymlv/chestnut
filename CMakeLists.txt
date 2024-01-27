cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(chestnut VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  glfw
  URL https://github.com/glfw/glfw/archive/refs/tags/3.3.9.zip
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/10.2.1.zip
)
FetchContent_MakeAvailable(fmt)

set(SRC
  src/gl.c
  src/main.cpp
  src/ip_addr.cpp
  src/socket.cpp
)

add_executable(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw fmt::fmt)
target_include_directories(${PROJECT_NAME} PRIVATE include/)
