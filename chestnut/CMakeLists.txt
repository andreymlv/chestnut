project(chestnut VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

find_package(Qt6 REQUIRED COMPONENTS Core Network Multimedia)

qt_standard_project_setup()

qt_add_library(${PROJECT_NAME}
  SHARED
  include/server.hpp
  include/client.hpp
  include/sockaddr.hpp
  src/server.cpp
  src/client.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_link_libraries(${PROJECT_NAME}
  PRIVATE Qt6::Core
  Qt6::Network
  Qt6::Multimedia
)

target_include_directories(${PROJECT_NAME} PUBLIC include/)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
