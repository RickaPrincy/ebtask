file(GLOB_RECURSE EBTASK_SOURCES CONFIGURE_DEPENDS "*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(ebtask ${EBTASK_SOURCES})

target_include_directories(ebtask 
    PRIVATE ${UDEV_INCLUDE_DIRS} 
    PRIVATE ${EBTASK_SOURCE_DIR}/include
)

target_link_libraries(ebtask 
    PRIVATE ${UDEV_LIBRARIES}
)

include(GNUInstallDirs)
install(TARGETS ebtask 
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
