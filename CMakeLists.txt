cmake_minimum_required(VERSION 4.2)
project(EditorCamera2D C)

set(CMAKE_C_STANDARD 99)

find_package(raylib CONFIG REQUIRED)

add_executable(EditorCamera2D main.c)

target_link_libraries(EditorCamera2D PRIVATE raylib)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(EditorCamera2D PRIVATE gdi32 winmm)
else ()
    target_link_libraries(EditorCamera2D PRIVATE GL m pthread dl rt X11)
endif ()
