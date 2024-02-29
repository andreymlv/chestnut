project(chestnut-cli VERSION 0.0.1 LANGUAGES CXX C)

find_package(Qt6 REQUIRED COMPONENTS Core Network Multimedia)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
  src/main.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_link_libraries(${PROJECT_NAME}
  PRIVATE chestnut
  PRIVATE Qt6::Core
  PRIVATE Qt6::Network
  PRIVATE Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
