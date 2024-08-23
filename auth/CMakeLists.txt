cmake_minimum_required(VERSION 3.16)
project(AuthModule VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(SOURCES
    authhintdialog.cpp
    authnetworkmanager.cpp
    authwindow.cpp
    inputvalidator.cpp
    loginwidget.cpp
    registerwidget.cpp
    waitwidget.cpp
)

set(HEADERS
    authhintdialog.h
    authnetworkmanager.h
    authwindow.h
    inputvalidator.h
    loginwidget.h
    registerwidget.h
    waitwidget.h
)

set(FORMS
    authhintdialog.ui
    authwindow.ui
    loginwidget.ui
    registerwidget.ui
    waitwidget.ui
)

set(RESOURCES
    ../resources/resources.qrc
)

qt_add_library(auth STATIC
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

target_link_libraries(auth custom_widgets Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)
