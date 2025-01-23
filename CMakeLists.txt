cmake_minimum_required(VERSION 3.16)

project(Istari VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Ensure version.txt exists and increment build version
set(VERSION_FILE "${CMAKE_BINARY_DIR}/version.txt")
if(EXISTS "${VERSION_FILE}")
    file(READ "${VERSION_FILE}" CURRENT_VERSION_PATCH)
    math(EXPR PROJECT_VERSION_PATCH "${CURRENT_VERSION_PATCH} + 1")
else()
    set(PROJECT_VERSION_PATCH 1)
endif()
file(WRITE "${VERSION_FILE}" "${PROJECT_VERSION_PATCH}")

# Get build date
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d %H:%M:%S")

# Configure version.h
configure_file(
    ${CMAKE_SOURCE_DIR}/version.h.in
    ${CMAKE_BINARY_DIR}/version.h
    @ONLY
)

# Include directory for version.h
include_directories(${CMAKE_BINARY_DIR})

# Custom target to generate version header
add_custom_target(generate_version_header
    DEPENDS ${CMAKE_BINARY_DIR}/version.h
)

# Create the executable target
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Istari
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Istari SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Istari
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Ensure the version header is generated before building the executable
add_dependencies(Istari generate_version_header)

target_link_libraries(Istari PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Istari)
endif()
set_target_properties(Istari PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Istari
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Istari)
endif()
