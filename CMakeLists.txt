cmake_minimum_required(VERSION 3.26)
project(MusicPlayerImproved)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        REQUIRED)

add_executable(MusicPlayerImproved main.cpp
        view/View.cpp
        view/View.h
        model/Model.cpp
        model/Model.h
        controller/Controller.cpp
        controller/Controller.h
        model/Song.cpp
        model/Song.h
        model/Sorting.cpp
        model/Sorting.h
)

add_custom_target(CopyResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:MusicPlayerImproved>/resources
)

add_dependencies(MusicPlayerImproved CopyResources)

target_link_libraries(MusicPlayerImproved
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
)

