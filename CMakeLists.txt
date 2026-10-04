cmake_minimum_required(VERSION 3.16)

project(taskmgr-linux VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(taskmgr-linux
src/main.cpp
src/MainWindow.cpp
src/MainWindow.h
src/ProcessModel.cpp
src/ProcessModel.h
src/SystemMonitor.cpp
src/SystemMonitor.h
resources/taskmgr.qrc
)

target_link_libraries(taskmgr-linux PRIVATE
Qt6::Widgets
)

install(TARGETS taskmgr-linux
RUNTIME DESTINATION bin
)

install(FILES
desktop/taskmgr.desktop
DESTINATION share/applications
)

install(FILES
resources/taskmgr.svg
DESTINATION share/icons/hicolor/scalable/apps
)
