cmake_minimum_required(VERSION 3.10)

project(ebtask VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(EBTASK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

find_package(PkgConfig REQUIRED)

find_program(XCLIP_EXECUTABLE xclip)
if(NOT XCLIP_EXECUTABLE)
    message(FATAL_ERROR "xclip not found. Please install it.")
endif()

find_program(XDOTOOL_EXECUTABLE xdotool)
if(NOT XDOTOOL_EXECUTABLE)
    message(FATAL_ERROR "xdotool not found. Please install it.")
endif()

pkg_check_modules(UDEV REQUIRED libudev)

add_subdirectory(src)
