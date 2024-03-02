cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(chestnut VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(chestnut)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
  add_subdirectory(cli)
endif()
add_subdirectory(gui)
