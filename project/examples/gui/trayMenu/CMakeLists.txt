# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME trayMenu)
file(GLOB TARGET_SOURCES src/*.cpp)
file(GLOB TARGET_HEADERS include/*.h include/*.hpp)

find_package(Threads REQUIRED)
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
add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${TARGET_HEADERS})

# ##############################################################################
# Define Target Properties and Dependency
# ##############################################################################

# add target properties such as debug postfix and MSVC runtime
set_target_properties(
  ${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "-d" MSVC_RUNTIME_LIBRARY
  "${MSVC_RUNTIME_TYPE}")

# set target compile options as defined in the cmake/compiler_options.cmake
# Module
target_compile_options(${TARGET_NAME} PRIVATE ${compiler_options})

# add compiler definition
target_compile_definitions(
  ${TARGET_NAME} PRIVATE ${compiler_definitions}
)

# set target link options as defined in the cmake/compiler_options.cmake Module
target_link_options(${TARGET_NAME} PRIVATE ${linker_flags})

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/project/shared/tray
)
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

# link
target_link_libraries(${TARGET_NAME}
  PUBLIC   Threads::Threads
  PRIVATE
  tray
)

if(WIN32)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resource/icon.ico DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
elseif(APPLE)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resource/icon.png DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# ##############################################################################
# Define Install Targets
# ##############################################################################
install(
  TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
  LIBRARY DESTINATION ${BINARY_INSTALL_DIR}/lib
  ARCHIVE DESTINATION ${BINARY_INSTALL_DIR}/lib
  PUBLIC_HEADER DESTINATION ${BINARY_INSTALL_DIR}/include)
