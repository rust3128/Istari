cmake_minimum_required(VERSION 3.16)

project(Istari VERSION 0.0.1 LANGUAGES CXX)

################
# Додавання скрипту для оновлення структури бази даних
add_custom_target(update_db_structure
    COMMAND ${CMAKE_COMMAND} -E env cmd /c ${CMAKE_SOURCE_DIR}/istari_exprot.bat
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Оновлення структури бази даних"
)
################

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resource.rc # Додати файл ресурсу
)



# Create the executable target
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Istari
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resourses.qrc
        AppParams/appparams.h AppParams/appparams.cpp
        AppParams/loggincategories.cpp AppParams/loggincategories.h
        AppParams/criptpass.cpp AppParams/criptpass.h AppParams/dbsettingsdialog.cpp AppParams/dbsettingsdialog.h AppParams/dbsettingsdialog.ui AppParams/qaesencryption.cpp AppParams/qaesencryption.h
        AppParams/databaseconfig.h AppParams/databaseconfig.cpp
        DatabseDialog/databsedialog.h DatabseDialog/databsedialog.cpp DatabseDialog/databsedialog.ui
        DatabseDialog/treefilterproxymodel.h DatabseDialog/treefilterproxymodel.cpp

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
# Зв'язування цілі update_db_structure з ціллю збірки MyProject
add_dependencies(Istari update_db_structure)

target_link_libraries(Istari PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Istari PRIVATE Qt6::Sql)
target_link_libraries(Istari PRIVATE Qt6::Core)
target_link_libraries(Istari PRIVATE Qt6::Network)
target_link_libraries(Istari PRIVATE Qt6::Core5Compat)



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
