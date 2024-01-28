cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(chestnut VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG e2c92645460f680fd272fd2eed591efb2be7dc31
)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG e69e5f977d458f2650bb346dadf2ad30c5320281
)
FetchContent_MakeAvailable(glfw fmt)

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
