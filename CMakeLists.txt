cmake_minimum_required(VERSION 3.10)

project(DeepNestCpp)
add_definitions(-DGLFW_EXPOSE_NATIVE_WIN32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

set(BOOST_ROOT "E:/boost_1_82_0")
find_package(Boost REQUIRED)

#set(GMP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/auxiliary/gmp/include")
#set(GMP_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/auxiliary/gmp/lib")
#set(MPFR_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/auxiliary/gmp/include")
#set(MPFR_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/auxiliary/gmp/lib")
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Boost REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/install/lib/cmake/CGAL")

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/CGAL-6.0.1/include)
include_directories(${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/OGL)
include_directories(${Boost_INCLUDE_DIRS})
message("Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")

add_compile_options(${Qt${QT_VERSION_MAJOR}_EXECUTABLE_COMPILE_FLAGS})

set(OPENGL_LIBS
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/glew32.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/glfw3dll.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/OpenGL32.Lib
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/glad.lib
)
set(DXF_LIB
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/dxflib.lib
)

file(GLOB HEADERS src/*.h)
file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCES})

file(GLOB DLL_FILES "${CMAKE_SOURCE_DIR}/bin/*.dll")

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>>)
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Shader $<1:${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILES} $<1:${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>>
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    ${OPENGL_LIBS}
    ${DXF_LIB}
)