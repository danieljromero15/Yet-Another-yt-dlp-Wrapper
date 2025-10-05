cmake_minimum_required(VERSION 3.31)
project(Yet_Another_yt-dlp_Wrapper)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(Yet_Another_yt-dlp_Wrapper main.cpp
        src/Window.cpp
        src/Window.h
        src/filedownloader.h
        src/filedownloader.cpp)
target_link_libraries(Yet_Another_yt-dlp_Wrapper
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Network
)

