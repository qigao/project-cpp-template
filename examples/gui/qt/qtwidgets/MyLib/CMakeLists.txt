find_package(Qt6 COMPONENTS Core REQUIRED)

# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_NAME QtDemoLib)
set(TARGET_INCLUDE_FOLDER include)
set(TARGET_SOURCES src/QtDemoClass.cpp)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_SOURCES})

# ##############################################################################
# Define Target according to options
# ##############################################################################

add_library(${TARGET_NAME} ${TARGET_SOURCES})

# ##############################################################################
# Define Target Properties and Optional OpenSSL Dependency
# ##############################################################################

# generate an export header to expose symbols when using the library
generate_export_header(
  ${TARGET_NAME} EXPORT_FILE_NAME
  ${CMAKE_BINARY_DIR}/exports/${TARGET_NAME}_export.h
)

# add target properties such as debug postfix, MSVC Runtime and library version
set_target_properties(
  ${TARGET_NAME}
  PROPERTIES DEBUG_POSTFIX "-d"
             MSVC_RUNTIME_LIBRARY "${MSVC_RUNTIME_TYPE}"
             VERSION ${CMAKE_PROJECT_VERSION}
             SOVERSION ${CMAKE_PROJECT_VERSION}
)

# set target compile options as defined in the cmake/compiler_options.cmake
# Module
target_compile_options(${TARGET_NAME} PRIVATE ${compiler_options})

# add compiler definition WITH_OPENSSL, if found
target_compile_definitions(
  ${TARGET_NAME}
  PRIVATE ${compiler_definitions} $<$<BOOL:${OPENSSL_FOUND}>:WITH_OPENSSL>
          $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_id}_STATIC_DEFINE>
)

# set target link options as defined in the cmake/compiler_options.cmake Module
target_link_options(${TARGET_NAME} PRIVATE ${linker_flags})

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE ${TARGET_INCLUDE_FOLDER}
          # include OpenSSL directories if present
          $<$<BOOL:${OPENSSL_FOUND}>:${OPENSSL_INCLUDE_DIR}>
          # Any other extra include required
          ${CMAKE_BINARY_DIR}
)

# link OpenSSL libraries if present
if(OPENSSL_FOUND)
  target_link_libraries(${TARGET_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto)
endif()

target_link_libraries(${TARGET_NAME} PUBLIC Qt6::Core)

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
