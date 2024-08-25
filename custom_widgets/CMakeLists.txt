cmake_minimum_required(VERSION 3.16)
project(CustomWidgetsModule VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SOURCES
    customdialog.cpp
    customwidget.cpp

)

set(HEADERS
    customdialog.h
    customwidget.h

)

qt_add_library(custom_widgets STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(custom_widgets Qt${QT_VERSION_MAJOR}::Widgets)
