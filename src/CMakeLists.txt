

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/tran/C_pp/Qt_gui/Qtwidgets/build/app")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++2a") #-Werror

SET(QT_USE_QTMULTIMEDIA TRUE)
SET(QT_USE_QTMULTIMEDIAWIDGETS TRUE)

include_directories( ${OpenCV_INCLUDE_DIRS})


file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

message (${HEADERS})
add_executable(${PROJECT} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT}
    Qt5::Core
    Qt5::Widgets
    Qt5::Quick
    Qt5::Network
    Qt5::SerialPort
    Qt5::SerialBus
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    ${OpenCV_LIBS}
    )
