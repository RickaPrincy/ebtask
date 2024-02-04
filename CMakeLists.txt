cmake_minimum_required(VERSION 3.10)

project(ebtask VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(GLOB_RECURSE EBTASK_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

find_program(XCLIP_EXECUTABLE xclip)
if(NOT XCLIP_EXECUTABLE)
    message(FATAL_ERROR "xclip not found. Please install it.")
endif()

find_program(XDOTOOL_EXECUTABLE xdotool)
if(NOT XDOTOOL_EXECUTABLE)
    message(FATAL_ERROR "xdotool not found. Please install it.")
endif()

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
if (NOT CLANG_FORMAT_EXECUTABLE)
    message("clang-format not found... format code will not be avalaible")
else()
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${EBTASK_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Vérification et formatage du code avec clang-format"
    )
endif()

# Creating the executable and link its deps
add_executable(ebtask ${EBTASK_SOURCES})

target_include_directories(ebtask 
    PRIVATE ${UDEV_INCLUDE_DIRS} 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(ebtask 
    PRIVATE ${UDEV_LIBRARIES}
)

# For installation
include(GNUInstallDirs)
install(TARGETS ebtask 
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
