# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME QmlDemoApp)

find_package(
  Qt6
  COMPONENTS Core Quick LinguistTools
  REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CPP_SRC_FILES main.cpp)
set(MOC_HEADERS)
set(HEADER_FILES ${MOC_HEADERS})
set(UI_SOURCES)
set(QRC_SRC_FILES qml/qml.qrc)
set(TARGET_INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/)
set(TS_FILES translations/translation_de_DE.ts)
# Set additional application information
set(APPLICATION_NAME "Qt6 Demo App For QML")
set(APPLICATION_DESCRIPTION "Yet another Qt App")
set(APPLICATION_VERSION "0.1")
set(APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/res/icon/AppIcon.ico")
set(APPLICATION_RESOURCE)
if(WIN32)
  configure_file(${CMAKE_SOURCE_DIR}/project/application.rc.in
                 "${CMAKE_CURRENT_BINARY_DIR}/application.rc")
  set(APPLICATION_RESOURCE ${CMAKE_CURRENT_BINARY_DIR}/application.rc)
endif(WIN32)
configure_file(${CMAKE_SOURCE_DIR}/project/app.h.in
"${CMAKE_CURRENT_BINARY_DIR}/app.h")
set(APP_RESOURCE_HEADER ${CMAKE_CURRENT_BINARY_DIR}/app.h)
# ##############################################################################
# Support IDE Visualization
# ##############################################################################
set(TARGET_SOURCES ${CPP_SRC_FILES} ${HEADER_FILES} ${UI_SOURCES} ${TS_FILES}
                   ${QRC_SRC_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_SOURCES})

# ##############################################################################
# Define Target according to options
# ##############################################################################
add_executable(${TARGET_NAME} ${TARGET_SOURCES} ${APPLICATION_RESOURCE} ${APP_RESOURCE_HEADER})

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
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE ${TARGET_INCLUDE_FOLDER}
          # Any other extra include required
          ${CMAKE_CURRENT_SOURCE_DIR}/../MyLib/include
          ${CMAKE_BINARY_DIR}
          ${CMAKE_CURRENT_BINARY_DIR}/)

# link
target_link_libraries(${TARGET_NAME} PUBLIC QtDemoLib Qt6::Core Qt6::Quick)
