cmake_minimum_required(VERSION 3.19.3)
project(checkpointsPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_PREFIX_PATH "C://Qt//6.7.2//mingw_64//lib//cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__USE_MINGW_ANSI_STDIO=1")


find_package(Qt6Core 6.0.0...6.7.7 CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)
find_package(Qt6Gui CONFIG REQUIRED)
find_package(Qt6Network CONFIG REQUIRED)
find_package(Qt6SerialPort CONFIG REQUIRED)

qt_standard_project_setup()

add_subdirectory(qdlt)

add_library(checkpointsPlugin SHARED checkpointsPlugin.cpp chpdialogform.cpp form.cpp sequences.cpp resources.qrc)
target_link_libraries(checkpointsPlugin Qt6::Core Qt6::Widgets qdlt)
