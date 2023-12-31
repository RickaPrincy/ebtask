file(GLOB_RECURSE RKEYLOGGER_SOURCES CONFIGURE_DEPENDS "*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(rkeylogger ${RKEYLOGGER_SOURCES})

target_include_directories(rkeylogger PRIVATE 
    ${UDEV_INCLUDE_DIRS} 
    ${RKEYLOGGER_SOURCE_DIR}/include
)

target_link_libraries(rkeylogger PRIVATE ${UDEV_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS rkeylogger 
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)