cmake_minimum_required(VERSION 3.10.0)
project(zoomablechartwidget VERSION 0.1.0 LANGUAGES C CXX)

# Determine if zoomablechart is built as a subproject (using add_subdirectory)
# or if it is the master project.
if (NOT DEFINED ZOOMABLECHART_MASTER_PROJECT)
    set(ZOOMABLECHART_MASTER_PROJECT OFF)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
      set(ZOOMABLECHART_MASTER_PROJECT ON)
      message(STATUS "CMake version: ${CMAKE_VERSION}")
    endif ()
  endif ()


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)

add_library(zoomablechart
    rangelimitedvalueaxis.cpp       
    rangelimitedvalueaxis.h
    
    zoomablechart.cpp
    zoomablechart.h

    zoomablechartview.cpp
    zoomablechartview.h

    zoomablechartwidget.cpp
    zoomablechartwidget.h
    zoomablechartwidget.ui
)

set_target_properties(zoomablechart PROPERTIES
    AUTOMOC ON
	AUTOUIC ON
	AUTORCC ON
)

target_link_libraries(zoomablechart PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts)
target_include_directories(zoomablechart PUBLIC ./)

if(ZOOMABLECHART_MASTER_PROJECT)
    add_subdirectory(example)
endif()
