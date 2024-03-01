project(chestnut-cli VERSION 0.0.1 LANGUAGES CXX C)

find_package(Qt6 REQUIRED COMPONENTS Core Multimedia)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
  src/main.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_link_libraries(${PROJECT_NAME}
  PRIVATE chestnut
  PRIVATE Qt6::Core
  PRIVATE Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${Qt6}/Qt6Cored.dll
            ${Qt6}/Qt6Networkd.dll
            ${Qt6}/Qt6Multimediad.dll
            ${Qt6}/Qt6Guid.dll
            ${CMAKE_CURRENT_BINARY_DIR}/
        )
    elseif (
        (CMAKE_BUILD_TYPE STREQUAL "Release") 
        OR (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo") 
        OR (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    )
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${Qt6}/Qt6Core.dll
            ${Qt6}/Qt6Network.dll
            ${Qt6}/Qt6Multimedia.dll
            ${Qt6}/Qt6Gui.dll
            ${CMAKE_CURRENT_BINARY_DIR}/
        )
    endif()
endif()
