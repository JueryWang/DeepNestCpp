cmake_minimum_required(VERSION 3.10)

project(DxfViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC OFF)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

# 修正1：正确的find_package语法
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# 修正2：正确的变量引用方式
include_directories(${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/OGL)
add_compile_options(${Qt${QT_VERSION_MAJOR}_EXECUTABLE_COMPILE_FLAGS})

set(OPENGL_LIBS
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/glew32.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/glfw3.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/OpenGL32.Lib
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/glad.lib
)

file(GLOB HEADERS src/*.h)
file(GLOB SOURCES src/*.cpp)

add_executable(DxfViewer 
        ${HEADERS}
        ${SOURCES})

# 修正3：正确的target_link_libraries语法
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    ${OPENGL_LIBS}
)

message(STATUS "Qt version: ${QT_VERSION_MAJOR}")
message(STATUS "Qt libraries: ${Qt${QT_VERSION_MAJOR}_LIBRARIES}")
message(STATUS "Qt include dirs: ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS}")