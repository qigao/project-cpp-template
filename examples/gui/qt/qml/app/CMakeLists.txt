# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME qml-app)
if(WIN32)
    configure_file(
        ${CMAKE_SOURCE_DIR}/project/application.rc.in
        "${CMAKE_CURRENT_BINARY_DIR}/application.rc"
    )
    set(APPLICATION_RESOURCE ${CMAKE_CURRENT_BINARY_DIR}/application.rc)
endif(WIN32)
file(GLOB TARGET_HEADERS include/*.hpp)
file(GLOB TARGET_SOURCES src/*.cpp)
qt_add_resources(QT_RESOURCES qml.qrc)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files"
    FILES ${TARGET_SOURCES}
)
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files"
    FILES ${TARGET_HEADERS}
)

# ##############################################################################
# Define Target according to options
# ##############################################################################
qt_add_executable(
    ${TARGET_NAME} ${TARGET_HEADERS} ${APPLICATION_RESOURCE} ${TARGET_SOURCES}
    ${QT_RESOURCES}
)

# ##############################################################################
# Define Target Properties
# ##############################################################################

# add target properties such as debug postfix and MSVC runtime
set_target_properties(
    ${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "-d" MSVC_RUNTIME_LIBRARY
                                                 "${MSVC_RUNTIME_TYPE}"
)
set_target_properties(
    ${TARGET_NAME} PROPERTIES QT_QML_MODULE_VERSION 1.0 QT_QML_MODULE_URI
                                                        Qt6App
)
# set target compile options as defined in the cmake/compiler_options.cmake
# Module
target_compile_options(${TARGET_NAME} PRIVATE ${compiler_options})

# add compiler definition
target_compile_definitions(${TARGET_NAME} PRIVATE ${compiler_definitions})

# set target link options as defined in the cmake/compiler_options.cmake Module
target_link_options(${TARGET_NAME} PRIVATE ${linker_flags})

# Make this a GUI application on Windows
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
endif()
# include all the necessary directories
target_include_directories(${TARGET_NAME} PRIVATE ../plugins/fancy)

# link the libraries
target_link_libraries(
    ${TARGET_NAME} PRIVATE Qt6::Qml Qt6::Quick fancy-plugin fancy
)

# ##############################################################################
# Define Install Targets
# ##############################################################################
install(
    TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
    LIBRARY DESTINATION ${BINARY_INSTALL_DIR}/lib
    ARCHIVE DESTINATION ${BINARY_INSTALL_DIR}/lib
    PUBLIC_HEADER DESTINATION ${BINARY_INSTALL_DIR}/include
)
