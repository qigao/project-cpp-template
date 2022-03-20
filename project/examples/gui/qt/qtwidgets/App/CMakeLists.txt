# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME QtDemoApp)

find_package(
  Qt6
  COMPONENTS Core Gui Widgets
  REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CPP_SRC_FILES main.cpp MainWindow.cpp)
set(MOC_HEADERS MainWindow.h)
set(HEADER_FILES ${MOC_HEADERS})
set(UI_SOURCES MainWindow.ui)
set(QRC_SRC_FILES resource/MyApp.qrc)
set(TARGET_INCLUDE_FOLDER .)

# Set additional application information
set(APPLICATION_NAME "Qt6 Demo App")
set(APPLICATION_DESCRIPTION "Yet another Qt App")
set(APPLICATION_VERSION "0.1")
set(APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resource/icon/app.ico")
set(APPLICATION_RESOURCE)
if(WIN32)
  configure_file(${CMAKE_SOURCE_DIR}/project/application.rc.in
                 "${CMAKE_CURRENT_BINARY_DIR}/application.rc")
  set(APPLICATION_RESOURCE ${CMAKE_CURRENT_BINARY_DIR}/application.rc)
endif(WIN32)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
set(TARGET_SOURCES ${CPP_SRC_FILES} ${HEADER_FILES} ${UI_SOURCES}
                   ${QRC_SRC_FILES})

# ##############################################################################
# Define Target according to options
# ##############################################################################
add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${APPLICATION_RESOURCE}
                              ${QRC_SRC_FILES})

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
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE ${TARGET_INCLUDE_FOLDER}
          # Any other extra include required
          ../MyLib/include)

# link
target_link_libraries(${TARGET_NAME} PUBLIC QtDemoLib Qt6::Core Qt6::Gui
                                            Qt6::Widgets)
