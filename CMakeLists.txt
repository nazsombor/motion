cmake_minimum_required(VERSION 3.28)
project(cairomotion)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(thirdparty/ink-stroke-modeler)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

add_executable(cairomotion main.cpp
        Cairomotion.cpp
        Cairomotion.h
        ui/PopupBar.cpp
        ui/PopupBar.h
        ui/Placeholder.cpp
        ui/Placeholder.h
        ui/CanvasContainer.cpp
        ui/CanvasContainer.h
        ui/Canvas.cpp
        ui/Canvas.h
        Drawings.cpp
        Drawings.h)

target_link_libraries(cairomotion ${GTKMM_LIBRARIES} InkStrokeModeler::stroke_modeler InkStrokeModeler::types InkStrokeModeler::params)