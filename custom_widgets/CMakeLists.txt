cmake_minimum_required(VERSION 3.16)
project(BasicWidgetsModule VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network WebSockets)

set(SOURCES
    basicdialog.cpp
    basicwidget.cpp
    basictcpnetworkmanager.cpp
)

set(HEADERS
    basicdialog.h
    basicwidget.h
    basictcpnetworkmanager.h
)

qt_add_library(custom_widgets STATIC
    ${SOURCES}
    ${HEADERS}
    basicwebnetworkmanager.h basicwebnetworkmanager.cpp
)

target_link_libraries(
    custom_widgets
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebSockets
)
