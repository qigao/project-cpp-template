
find_package(
        Qt6
        COMPONENTS Core Gui Widgets
        REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCE_FILES main.cpp MainWindow.cpp)

set(MOC_HEADERS MainWindow.h)

set(HEADER_FILES ${MOC_HEADERS})

set(UI_SOURCES MainWindow.ui)

set(QRC_SOURCE_FILES res/MyApp.qrc)

# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME QtDemoApp)
set(TARGET_INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/)
set(TARGET_SOURCES ${SOURCE_FILES} ${HEADER_FILES} ${UI_SOURCES}
                   ${QRC_SOURCE_FILES})

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_SOURCES})

# ##############################################################################
# Define Target according to options
# ##############################################################################
add_executable(${TARGET_NAME} WIN32 MACOSX_BUNDLE ${TARGET_SOURCES})

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

#if(WIN32)
#  target_link_options(${TARGET_NAME}  PRIVATE "/entry:mainCRTStartup")
#endif()

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE ${TARGET_INCLUDE_FOLDER}
          # Any other extra include required
          ${CMAKE_CURRENT_SOURCE_DIR}/../MyLib/include)

# link
target_link_libraries(${TARGET_NAME} PUBLIC QtDemoLib Qt6::Core Qt6::Gui
                                            Qt6::Widgets)
