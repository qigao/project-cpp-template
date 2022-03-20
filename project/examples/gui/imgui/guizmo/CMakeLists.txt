find_package(glfw3)
find_package(imgui)
set(IMGUI_BACKEND_DIR ${CMAKE_BINARY_DIR}/bindings)
set(SHARED_IMGUI_GIZMO_DIR ${CMAKE_SOURCE_DIR}/project/shared/imguizmo)
# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME guizmo)
set(TARGET_INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB IMGUI_BACKEND_SRC ${IMGUI_BACKEND_DIR}/*.cpp)
file(GLOB IMGUI_BACKEND_INCLUDE ${IMGUI_BACKEND_DIR}/*.h)
file(GLOB APP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
file(GLOB GIZMO_SRC ${SHARED_IMGUI_GIZMO_DIR}/*.cpp)
file(GLOB GIZMO_INCLUDE ${SHARED_IMGUI_GIZMO_DIR}/*.h)
set(TARGET_SOURCES ${APP_SRC})
if(WIN32)
  configure_file(${CMAKE_SOURCE_DIR}/project/application.rc.in
                 "${CMAKE_CURRENT_BINARY_DIR}/application.rc")
  set(APPLICATION_RESOURCE ${CMAKE_CURRENT_BINARY_DIR}/application.rc)
endif(WIN32)
# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_SOURCES} ${ASSETS}
                                                    ${TARGET_INCLUDE_FOLDER})

# ##############################################################################
# Define Target according to options
# ##############################################################################
add_executable(
  ${TARGET_NAME} ${TARGET_SOURCES} ${IMGUI_BACKEND_SRC} ${ASSETS}
                 ${APPLICATION_RESOURCE} ${GIZMO_SRC} ${GIZMO_INCLUDE})

# ##############################################################################
# Define Target Properties
# ##############################################################################

# add target properties such as debug postfix and MSVC runtime
set_target_properties(
  ${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "-d" MSVC_RUNTIME_LIBRARY
                                               "${MSVC_RUNTIME_TYPE}")

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
  set_target_properties(
    ${TARGET_NAME}
    PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
               LINK_FLAGS_RELEASE "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
               LINK_FLAGS_RELWITHDEBINFO
               "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
               LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

endif()

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE ${TARGET_INCLUDE_FOLDER}
          # Any other extra include required
          ${IMGUI_BACKEND_DIR} ${SHARED_IMGUI_GIZMO_DIR})

# link against
target_link_libraries(${TARGET_NAME} PUBLIC imgui::imgui glfw::glfw)
