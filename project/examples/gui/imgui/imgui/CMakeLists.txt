find_package(glfw3)
find_package(imgui)

# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME imguiDemo)

file(GLOB TARGET_HEADERS include/*.h)
file(GLOB TARGET_SOURCES src/*.cpp)

if(WIN32)
  # Set additional application information
  set(APPLICATION_NAME "QML Demo App")
  set(APPLICATION_DESCRIPTION "Yet another Qt App")
  set(APPLICATION_VERSION "0.1")
  set(APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resource/icon/app.ico")
  configure_file(${CMAKE_SOURCE_DIR}/project/application.rc.in
    "${CMAKE_CURRENT_BINARY_DIR}/application.rc")
  set(APPLICATION_RESOURCE ${CMAKE_CURRENT_BINARY_DIR}/application.rc)
endif(WIN32)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  PREFIX "Source Files"
  FILES ${TARGET_SOURCES})
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  PREFIX "Header Files"
  FILES ${TARGET_HEADERS})

# ##############################################################################
# Define Target according to options
# ##############################################################################
add_executable(
  ${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS} ${IMGUI_BINDING_SRC}
  ${IMGUI_BINDING_HEADER} ${APPLICATION_RESOURCE})

# ##############################################################################
# Define Target Properties and Optional OpenSSL Dependency
# ##############################################################################

# add target properties such as debug postfix and MSVC runtime
set_target_properties(
  ${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "-d" MSVC_RUNTIME_LIBRARY
  "${MSVC_RUNTIME_TYPE}")

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
  set_target_properties(
    ${TARGET_NAME}
    PROPERTIES
    LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
    LINK_FLAGS_RELEASE "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
    LINK_FLAGS_RELWITHDEBINFO
    "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
    LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
endif()

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include

          # Any other extra include required
  ${IMGUI_BINDING_DIR})

# link against
target_link_libraries(${TARGET_NAME} PUBLIC imgui::imgui glfw::glfw)

# ##############################################################################
# Define Install Targets
# ##############################################################################
install(
  TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
  LIBRARY DESTINATION ${BINARY_INSTALL_DIR}/lib
  ARCHIVE DESTINATION ${BINARY_INSTALL_DIR}/lib
  PUBLIC_HEADER DESTINATION ${BINARY_INSTALL_DIR}/include)
