
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)

add_executable(zoomlinechart main.cpp)

set_target_properties(zoomlinechart PROPERTIES
    AUTOMOC ON
	AUTOUIC ON
	AUTORCC ON
)

target_link_libraries(zoomlinechart PRIVATE Qt${QT_VERSION_MAJOR}::Widgets  Qt${QT_VERSION_MAJOR}::Charts zoomablechart)