cmake_minimum_required(VERSION 3.10)

project(ebtask VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EBTASK_VERSION ${PROJECT_VERSION})

set(EBTASK_SOURCES
    src/main.cpp
    src/core/run.cpp
    src/core/config/config.cpp
    src/core/actions/generate_config.cpp
    src/core/actions/remap_layout.cpp
    src/core/os_inputs/guess_input_file.cpp
    src/core/os_inputs/read_input_file.cpp
    src/utils/fs_utils.cpp)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/ebtask_config.hpp.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/core/ebtask_config.hpp")
# Find deps

# rcli
find_package(rcli REQUIRED)

# libudev
find_package(PkgConfig REQUIRED)
pkg_check_modules(UDEV REQUIRED libudev)

# xclip (will be optional)
find_program(XCLIP_EXECUTABLE xclip)
if(NOT XCLIP_EXECUTABLE)
  message(FATAL_ERROR "xclip not found. Please install it.")
endif()

# xdotool (will be optional)
find_program(XDOTOOL_EXECUTABLE xdotool)
if(NOT XDOTOOL_EXECUTABLE)
  message(FATAL_ERROR "xdotool not found. Please install it.")
endif()

# clang-format [optional]
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
if(NOT CLANG_FORMAT_EXECUTABLE)
  message("clang-format not found... format code will not be avalaible")
else()
  add_custom_target(
    format
    COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${EBTASK_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Vérification et formatage du code avec clang-format")
endif()

# Creating the executable and link its deps
add_executable(ebtask ${EBTASK_SOURCES})

target_include_directories(
  ebtask
  PRIVATE ${UDEV_INCLUDE_DIRS}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(
  ebtask
  PRIVATE ${UDEV_LIBRARIES}
  PRIVATE rcli)

# For installation
include(GNUInstallDirs)
install(TARGETS ebtask RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
