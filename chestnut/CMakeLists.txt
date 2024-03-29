project(chestnut VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

find_package(Qt6 REQUIRED COMPONENTS Core Network Multimedia)

qt_standard_project_setup()

qt_add_library(${PROJECT_NAME}
  SHARED
  include/room.hpp
  include/client.hpp
  include/sockaddr.hpp
  include/discover.hpp
  src/room.cpp
  src/client.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_link_libraries(${PROJECT_NAME}
  PUBLIC Qt6::Core
  Qt6::Network
  Qt6::Multimedia
  Opus::opus
)

target_include_directories(${PROJECT_NAME} PUBLIC include/)
# Disable exceptions and runtime type identification
target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions -fno-rtti)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
