# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME qml-app)
if(WIN32)
  configure_file(${CMAKE_SOURCE_DIR}/project/application.rc.in
                 "${CMAKE_CURRENT_BINARY_DIR}/application.rc")
  set(APPLICATION_RESOURCE ${CMAKE_CURRENT_BINARY_DIR}/application.rc)
endif(WIN32)
file(GLOB CPP_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(TARGET_SOURCES ${CPP_SRC_FILES})
qt_add_resources(RESOURCES qml.qrc)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
set(TARGET_SOURCES ${CPP_SRC_FILES} ${HEADER_FILES} ${RESOURCES} ${APPLICATION_RESOURCE})

# ##############################################################################
# Define Target according to options
# ##############################################################################
qt_add_executable(${TARGET_NAME} ${CPP_SRC_FILES} ${APPLICATION_RESOURCE} ${RESOURCES} )

# ##############################################################################
# Define Target Properties
# ##############################################################################

# add target properties such as debug postfix and MSVC runtime
set_target_properties(
  ${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "-d" MSVC_RUNTIME_LIBRARY
                                               "${MSVC_RUNTIME_TYPE}")
set_target_properties(${TARGET_NAME}  PROPERTIES
    QT_QML_MODULE_VERSION 1.0
    QT_QML_MODULE_URI Qt6App)
# set target compile options as defined in the cmake/compiler_options.cmake
# Module
target_compile_options(${TARGET_NAME} PRIVATE ${compiler_options})

# add compiler definition WITH_OPENSSL, if found
target_compile_definitions(
  ${TARGET_NAME} PRIVATE ${compiler_definitions}
                         $<$<BOOL:${OPENSSL_FOUND}>:WITH_OPENSSL>)

# set target link options as defined in the cmake/compiler_options.cmake Module
target_link_options(${TARGET_NAME} PRIVATE ${linker_flags})

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()
# include all the necessary directories
target_include_directories(${TARGET_NAME} PRIVATE src ../plugins/fancy)


# link the libraries
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Qml Qt6::Quick fancy-plugin fancy)
