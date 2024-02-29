project(chestnut-gui VERSION 0.0.1 LANGUAGES CXX C)

find_package(Qt6 REQUIRED COMPONENTS Core Network Multimedia Quick)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
  src/main.cpp
)

qt_add_qml_module(${PROJECT_NAME}
    URI chestnut
    VERSION 1.0
    QML_FILES Main.qml
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_link_libraries(${PROJECT_NAME}
  PRIVATE chestnut
  PRIVATE Qt6::Core
  PRIVATE Qt6::Network
  PRIVATE Qt6::Multimedia
  PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
