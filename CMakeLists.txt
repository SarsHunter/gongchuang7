cmake_minimum_required(VERSION 3.5)

project(gongchaungv1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZBAR REQUIRED IMPORTED_TARGET zbar)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    videothread.cpp
    videothread.h

    serialcontrol.h
    serialcontrol.cpp

    carcontroller.h
    carcontroller.cpp

    armcontroller.h
    armcontroller.cpp

    taskmanager.h
    taskmanager.cpp

    sharedserial.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gongchaungv1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(gongchaungv1 SHARED ${PROJECT_SOURCES})
    else()
        add_executable(gongchaungv1 ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(gongchaungv1 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SerialPort
    ${OpenCV_LIBS}
    PkgConfig::ZBAR
)

set_target_properties(gongchaungv1 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS gongchaungv1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gongchaungv1)
endif()
