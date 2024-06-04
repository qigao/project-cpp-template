set(PROJECT_NAME "http_client_lib")
add_library(${PROJECT_NAME} "./src/my_lib/lib.cpp")
# ##############################################################################
# Define Target Information
# ##############################################################################
set(TARGET_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB TARGET_SOURCES src/*.cpp)
file(GLOB TARGET_HEADERS *.h)

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
target_find_dependencies(${PROJECT_NAME} PRIVATE_CONFIG fmt httplib nolhmann_json)

# Link dependencies:
target_link_system_libraries(${PROJECT_NAME} PRIVATE fmt::fmt
        httplib::httplib
        nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PUBLIC $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
                                             $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>)

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>)

if(FEATURE_TESTS)
  add_subdirectory("./test")
endif()
