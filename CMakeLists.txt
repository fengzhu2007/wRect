cmake_minimum_required(VERSION 3.14)

project(wRect)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core WinExtras)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core WinExtras)


include_directories(SYSTEM "./src")


set(RESOURCES
    src/res.qrc
)

qt5_add_resources(RESOURCE_ADDED ${RESOURCES})


add_library(wRect SHARED
    ${RESOURCE_ADDED}
    src/global.h
    src/w_main_window.h src/w_main_window.cpp
    src/w_window_resizer.h src/w_window_resizer.cpp
    src/qss.h src/qss.cpp
    src/w_window_nclient.h src/w_window_nclient.cpp
    src/w_system_button.h src/w_system_button.cpp
)






target_link_libraries(wRect PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WinExtras)

target_compile_definitions(wRect PRIVATE WRECT_LIBRARY)



add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}.dll        ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.dll
    COMMENT "Copying file to binary directory"
)


################demo#################################

#add_executable(demo
#    demo/main.cpp
#)
#target_link_libraries(demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets wRect)

