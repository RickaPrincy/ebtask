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
    src/core/actions/configure_layout.cpp
    src/core/actions/listen_event.cpp
    src/os/keyhandler.cpp
    src/os/read_devnode_input_file.cpp
    src/os/execute_command.cpp
    src/utils/fs_utils.cpp)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/generated_config.hpp")

find_package(rcli REQUIRED)

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

target_include_directories(ebtask PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ebtask PRIVATE rcli)

# For installation
include(GNUInstallDirs)
install(TARGETS ebtask RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
