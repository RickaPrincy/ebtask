cmake_minimum_required(VERSION 3.10)

project(ebtask VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 17)
set(EBTASK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

find_package(PkgConfig REQUIRED)

pkg_check_modules(UDEV REQUIRED libudev)
add_subdirectory(src)
