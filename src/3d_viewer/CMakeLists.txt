cmake_minimum_required(VERSION 3.5)

project(3d_viewer VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        myopenglwidgegt.h
        myopenglwidgegt.cpp
        gif/giflib/dgif_lib.c
        gif/giflib/egif_lib.c
        gif/giflib/gif_err.c
        gif/giflib/gif_hash.c
        gif/giflib/gifalloc.c
        gif/giflib/quantize.c
        gif/giflib/gif_font.c
        gif/giflib/gif_hash.h
        gif/giflib/gif_lib.h
        gif/giflib/gif_lib_private.h
        gif/qgifimage.cpp
        gif/qgifglobal.h
        gif/qgifimage.h
        gif/qgifimage_p.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3d_viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        s21_3DViewer.c
        s21_3DViewer.h

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 3d_viewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(3d_viewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(3d_viewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(3d_viewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(3d_viewer PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
set_target_properties(3d_viewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(3d_viewer)
endif()
