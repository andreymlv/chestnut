project(chestnut-gui VERSION 0.0.1 LANGUAGES CXX C)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
  include/chestnut.hpp
  src/main.cpp
)

qt_add_qml_module(${PROJECT_NAME}
    URI chestnut
    VERSION 1.0
    RESOURCE_PREFIX "/andreymlv"
    QML_FILES Main.qml
)

qt_add_resources(${PROJECT_NAME}
    "controls_conf"
    PREFIX "/"
    FILES "qtquickcontrols2.conf"
)

qt_add_resources(${PROJECT_NAME}
    "assets"
    PREFIX "/"
    FILES "resources/icon.png"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_link_libraries(${PROJECT_NAME}
  PRIVATE chestnut
  Qt6::Quick
)

target_include_directories(${PROJECT_NAME} PRIVATE include/)
# Disable exceptions and runtime type identification
target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions -fno-rtti)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(WIN32)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy ${Qt6}/Qt6Cored.dll ${Qt6}/Qt6Networkd.dll
    ${Qt6}/Qt6Multimediad.dll ${Qt6}/Qt6Guid.dll ${CMAKE_CURRENT_BINARY_DIR}/
    COMMENT "Copying DLLs")
  elseif((CMAKE_BUILD_TYPE STREQUAL "Release")
        or(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        or(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel"))
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy ${Qt6}/Qt6Core.dll ${Qt6}/Qt6Network.dll
    ${Qt6}/Qt6Multimedia.dll ${Qt6}/Qt6Gui.dll ${CMAKE_CURRENT_BINARY_DIR}/
    COMMENT "Copying DLLs")
  endif()
endif()
