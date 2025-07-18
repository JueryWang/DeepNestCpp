cmake_minimum_required(VERSION 3.10)

project(DeepNestCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC OFF)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

set(BOOST_ROOT "E:/boost_1_82_0")
find_package(Boost REQUIRED)

set(GMP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/auxiliary/gmp/include")
set(GMP_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/auxiliary/gmp/lib")
set(MPFR_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/auxiliary/gmp/include")
set(MPFR_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/auxiliary/gmp/lib")
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Boost REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/install/lib/cmake/CGAL")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/include)
include_directories(${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/OGL)
include_directories(${Boost_INCLUDE_DIRS})
message("Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")

add_compile_options(${Qt${QT_VERSION_MAJOR}_EXECUTABLE_COMPILE_FLAGS})

set(OPENGL_LIBS
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/glew32.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/glfw3.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/OpenGL32.Lib
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/glad.lib
)

file(GLOB HEADERS src/*.h)
file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCES})


target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    ${OPENGL_LIBS}
)

message(STATUS "Qt version: ${QT_VERSION_MAJOR}")
message(STATUS "Qt libraries: ${Qt${QT_VERSION_MAJOR}_LIBRARIES}")
message(STATUS "Qt include dirs: ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS}")