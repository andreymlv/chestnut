cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(chestnut VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
  include/server.hpp
  src/server.cpp
  src/main.cpp
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Network)
target_include_directories(${PROJECT_NAME} PRIVATE include/)
