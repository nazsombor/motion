cmake_minimum_required(VERSION 3.28)
project(motion)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(thirdparty/ink-stroke-modeler)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

add_executable(motion main.cpp
        MotionApp.cpp
        MotionApp.h
        ui/PopupBar.cpp
        ui/PopupBar.h
        ui/Placeholder.cpp
        ui/Placeholder.h
        ui/CanvasContainer.cpp
        ui/CanvasContainer.h
        ui/Canvas.cpp
        ui/Canvas.h
        Drawings.cpp
        Drawings.h
        ui/ScrolledWidget.cpp
        ui/ScrolledWidget.h
        ui/Tools.cpp
        ui/Tools.h
        ui/ColorPicker.cpp
        ui/ColorPicker.h
        ui/ColorList.cpp
        ui/ColorList.h
        ui/CurrentColor.cpp
        ui/CurrentColor.h
        ui/Timeline.cpp
        ui/Timeline.h
        ui/FileOperation.cpp
        ui/FileOperation.h
        History.cpp
        History.h
)

target_link_libraries(motion ${GTKMM_LIBRARIES} InkStrokeModeler::stroke_modeler InkStrokeModeler::types InkStrokeModeler::params X11)
