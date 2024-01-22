cmake_minimum_required(VERSION 3.5...3.20 FATAL_ERROR)

project(chestnut VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(GLFWPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(deps/glfwpp)

set(SRC
  src/gl.c
  src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
target_link_libraries(${PROJECT_NAME} PRIVATE GLFWPP)

target_include_directories(${PROJECT_NAME} PRIVATE include/)
