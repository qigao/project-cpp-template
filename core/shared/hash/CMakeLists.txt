# Project Name
set(TARGET_NAME hashfunc)

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES private/src/*.cpp)
file(GLOB TARGET_HEADERS private/include/*.hpp)
file(GLOB TARGET_PUBLIC_HEADERS public/*.hpp)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files"
  FILES ${TARGET_SOURCES}
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header(Private) Files"
  FILES ${TARGET_HEADERS}
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header(Public) Files"
  FILES ${TARGET_PUBLIC_HEADERS}
)

add_library(${TARGET_NAME})
target_sources(
  ${TARGET_NAME} PRIVATE ${TARGET_HEADERS} ${TARGET_SOURCES}
                         ${TARGET_PUBLIC_HEADERS}
)
target_include_interface_directories(
  ${TARGET_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/private/include
  ${CMAKE_CURRENT_SOURCE_DIR}/public
  ${CMAKE_SOURCE_DIR}/core/shared/utils
  ${CMAKE_SOURCE_DIR}/core/shared/http
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${TARGET_NAME}
)
if(ENABLE_OPENSSL)
  find_package(OpenSSL)
endif()
target_link_system_libraries(
  ${TARGET_NAME} PUBLIC $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
  $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>
)
target_compile_definitions(
  ${TARGET_NAME} INTERFACE $<$<BOOL:${ENABLE_OPENSSL}>:WITH_OPENSSL>
)
# link project_options/warnings
target_link_libraries(
  ${TARGET_NAME} PRIVATE starter_project_options starter_project_warnings
)
string(TOUPPER ${TARGET_NAME} API_NAME)

generate_export_header(
  ${TARGET_NAME} EXPORT_FILE_NAME
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${TARGET_NAME}/${TARGET_NAME}_export.h
  EXPORT_MACRO_NAME ${API_NAME}_API
)
# tests
if(FEATURE_TESTS)
  add_subdirectory(test)
endif()
