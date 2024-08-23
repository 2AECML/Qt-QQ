cmake_minimum_required(VERSION 3.16)
project(HomeModule VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(SOURCES
    homewindow.cpp
    homenetworkmanager.cpp
)

set(HEADERS
    homewindow.h
    homenetworkmanager.h
)

set(FORMS
    homewindow.ui
)

set(RESOURCES
    ../resources/resources.qrc
)

qt_add_library(home STATIC
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
    userlistitem.h userlistitem.cpp userlistitem.ui

)

target_link_libraries(home custom_widgets Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)
