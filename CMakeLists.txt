cmake_minimum_required(VERSION 3.21)
project(OpenGLTestQt6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)

add_executable(OpenGLTest WIN32
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(OpenGLTest PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets
    opengl32
)

if(MINGW)
    target_link_options(OpenGLTest PRIVATE
        -static
        $<$<CONFIG:Release>:-s>
    )
endif()