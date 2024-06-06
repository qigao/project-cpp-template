set(PROJECT_NAME "http_client_lib")

# ##############################################################################
# Define Target Information
# ##############################################################################

file(GLOB TARGET_SOURCES src/my_lib/*.cpp)
file(GLOB TARGET_HEADERS include/my_lib/*.hpp)

# ##############################################################################
# Support IDE Visualization
# ##############################################################################
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${TARGET_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files" FILES ${TARGET_HEADERS})
add_library(${PROJECT_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES})
# link project_options/warnings
target_link_libraries(${PROJECT_NAME} PRIVATE my_project_options my_project_warnings)

# Includes
target_include_interface_directories(${PROJECT_NAME} include)

# Find dependencies:
if(ENABLE_OPENSSL)
  find_package(OpenSSL REQUIRED)
endif()

#set(OS_WINDOWS TRUE)

# Find dependencies:
target_find_dependencies(${PROJECT_NAME} PRIVATE_CONFIG fmt)
find_package(httplib)

# Link dependencies:
target_link_system_libraries(${PROJECT_NAME} PRIVATE fmt::fmt httplib::httplib)
target_link_libraries(
  ${PROJECT_NAME} PUBLIC $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
                         $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>
)

target_compile_definitions(
  ${PROJECT_NAME} PUBLIC $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

if(FEATURE_TESTS)
  add_subdirectory("./test")
endif()
