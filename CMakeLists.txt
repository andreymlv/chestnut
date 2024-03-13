cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(chestnut VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(ExternalProject)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  # https://clang.llvm.org/docs/UsersManual.html #cmdoption-f-no-sanitize
  # https://www.jetbrains.com/help/clion/google-sanitizers.html
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

# ExternalProject_Add(rnnoise
#   SOURCE_DIR ${CMAKE_SOURCE_DIR}/rnnoise
#   CONFIGURE_COMMAND "./configure"
# )

# opus v1.5.1 doesn't downloads model data automatically
if(NOT EXISTS "deps/opus/dnn/fargan_data.h")
  if(WIN32)
    execute_process(COMMAND cmd //C autogen.bat
                    WORKING_DIRECTORY "deps/opus/")
  else()
    execute_process(COMMAND sh autogen.sh
                    WORKING_DIRECTORY "deps/opus/")
  endif()
endif()

add_subdirectory(deps/opus)


add_subdirectory(chestnut)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
  add_subdirectory(cli)
endif()
add_subdirectory(gui)
