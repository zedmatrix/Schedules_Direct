cmake_minimum_required(VERSION 3.5)

project(TVGuide LANGUAGES CXX)

# Specify your C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find and link zlib
find_package(ZLIB REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia Network)

add_executable(TVGuide
    main.cpp
    mainwindow.cpp
    UserInputDialog.cpp
    networkReply.cpp
    decompress.cpp
    parseObject.cpp
    requestToken.cpp
    requestStatus.cpp
    requestHeadends.cpp
    mainwindow.h
    ui_mainwindow.h
    mainwindow.ui
)

target_link_libraries(TVGuide Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia Qt6::Network ZLIB::ZLIB)
