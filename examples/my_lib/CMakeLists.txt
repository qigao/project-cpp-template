# Project Name
set(TARGET_NAME my_lib)

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES private/*.cpp)
file(GLOB TARGET_HEADERS public/*.h)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(TREE . PREFIX "Source Files" FILES ${TARGET_SOURCES})
source_group(TREE . PREFIX "Header Files" FILES ${TARGET_HEADERS})

add_library(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE ${TARGET_SOURCES})
target_include_interface_directories(${TARGET_NAME} private) # Includes
target_link_libraries(
    ${TARGET_NAME} PRIVATE starter_project_options starter_project_warnings
)

target_find_dependencies(
    ${TARGET_NAME} # Find dependencies
    PRIVATE_CONFIG range-v3
)
target_link_system_libraries(
    ${TARGET_NAME} # Link dependencies
    PRIVATE range-v3::range-v3
)

# =================================================================================
# INSTALL
# =================================================================================

# Files to Install
install(
    TARGETS ${TARGET_NAME}
    EXPORT ${TARGET_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET_NAME}
)
install(
    EXPORT ${TARGET_NAME}Targets
    FILE ${TARGET_NAME}Targets.cmake
    NAMESPACE ${TARGET_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}
)
