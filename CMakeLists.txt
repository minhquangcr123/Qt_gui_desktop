cmake_minimum_required(VERSION 3.10)

set(PROJECT "widgets_opencv")

project(${PROJECT})

# 3rd party tools
find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
find_package( Qt5Multimedia REQUIRED )
find_package( Qt5MultimediaWidgets REQUIRED )
find_package( OpenCV REQUIRED )
find_package(Qt5 COMPONENTS Core SerialPort SerialBus REQUIRED)

# Directory with the source code
add_subdirectory(src)

message(OPENCV_VERSION)

