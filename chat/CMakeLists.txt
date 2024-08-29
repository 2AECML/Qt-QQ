cmake_minimum_required(VERSION 3.16)
project(HomeModule VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Sql)

set(SOURCES
    chatwindow.cpp
    emoticonpanel.cpp
    chatnetworkmanager.cpp
    chatdatabasemanager.cpp
)

set(HEADERS
    chatwindow.h
    emoticonpanel.h
    chatmessage.h
    chatnetworkmanager.h
    chatdatabasemanager.h
)

set(FORMS
    chatwindow.ui
)

set(RESOURCES
    ../resources/resources.qrc
)

qt_add_library(chat STATIC
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

target_link_libraries(chat custom_widgets Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Sql)
