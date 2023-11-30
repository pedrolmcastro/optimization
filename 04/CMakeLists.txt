cmake_minimum_required(VERSION 3.0...3.25)

project(
    CCanvas
    VERSION 0.1.0
    LANGUAGES CXX C
)

find_library(MATH_LIBRARY m)

file(GLOB SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")

add_executable(CCanvas ${SOURCES})
include_directories(src)

add_subdirectory(external/glfw)
add_subdirectory(external/softbuffer)

if(MSVC)
    target_compile_options(CCanvas PRIVATE /W4)
else()
    target_compile_options(CCanvas PRIVATE -Wall -Wextra -pedantic -march=native)
endif()

target_link_libraries(CCanvas PRIVATE glfw softbuffer)
if(MATH_LIBRARY)
    target_link_libraries(CCanvas PUBLIC ${MATH_LIBRARY})
endif()

target_copy_softbuffer_binaries(CCanvas)
