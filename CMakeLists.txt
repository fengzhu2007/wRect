# Qt6 要求 CMake >= 3.16
cmake_minimum_required(VERSION 3.16)

project(wRect LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Qt5/Qt6 共有组件（只用共有组件做版本探测，否则仅装了其中一个版本时会直接失败）
set(WRECT_QT_COMPONENTS Widgets Core Svg)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${WRECT_QT_COMPONENTS})

# 版本相关组件：
#   Qt6 : QSvgWidget 已从 Svg 拆分到独立的 SvgWidgets 模块
#   Qt5 : Windows 下仍使用 QtWinExtras（Qt6 已移除该模块）
if(QT_VERSION_MAJOR EQUAL 6)
    list(APPEND WRECT_QT_COMPONENTS SvgWidgets)
elseif(WIN32)
    list(APPEND WRECT_QT_COMPONENTS WinExtras)
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${WRECT_QT_COMPONENTS})

# 统一生成 QtX::Yyy 形式的导入目标名
set(WRECT_QT_LIBS "")
foreach(_wr_component ${WRECT_QT_COMPONENTS})
    list(APPEND WRECT_QT_LIBS Qt${QT_VERSION_MAJOR}::${_wr_component})
endforeach()



include_directories(SYSTEM "./src")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DQ_DEBUG)
endif()

set(RESOURCES
    src/res.qrc
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_resources(RESOURCE_ADDED ${RESOURCES})
else()
    qt5_add_resources(RESOURCE_ADDED ${RESOURCES})
endif()


add_library(wRect SHARED
    ${RESOURCE_ADDED}
    src/global.h
    src/w_main_window.h src/w_main_window.cpp
    src/w_window_resizer.h src/w_window_resizer.cpp
    src/w_qss.h src/w_qss.cpp
    src/w_window_nclient.h src/w_window_nclient.cpp
    src/w_system_button.h src/w_system_button.cpp
    src/w_dialog.h src/w_dialog.cpp
    src/w_tags.h src/w_tags.cpp
    src/w_flow_layout.h src/w_flow_layout.cpp
    src/w_toast.h src/w_toast.cpp
    src/w_spin.h src/w_spin.cpp
    src/w_progress_bar.h src/w_progress_bar.cpp
    src/w_window.h src/w_window.cpp
    src/w_badge.h src/w_badge.cpp
    src/w_popup_panel.h src/w_popup_panel.cpp

    src/w_tab_bar.h src/w_tab_bar.cpp

)




target_link_libraries(wRect PRIVATE ${WRECT_QT_LIBS})

target_compile_definitions(wRect PRIVATE WRECT_LIBRARY)


if(WIN32)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}.dll        ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}.dll
    COMMENT "Copying file to binary directory"
)
else()

endif()


################demo#################################

#add_executable(demo
#    demo/main.cpp
#)
#target_link_libraries(demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets wRect)

