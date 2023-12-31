cmake_minimum_required(VERSION 3.10)

project(ClavierMonitor)

set(CMAKE_CXX_STANDARD 17)
set(RKEYLOGGER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
find_package(PkgConfig REQUIRED)

pkg_check_modules(UDEV REQUIRED libudev)

add_subdirectory(src)