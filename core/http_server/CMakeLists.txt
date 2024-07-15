set(TARGET_NAME http_server)
# ----------------------------------------------------------------------------------------------------------------------

# ##############################################################################
# Support IDE Visualization
# ##############################################################################

file(
    GLOB
    TARGET_SOURCES
    src/*.cpp
    src/http/*.cpp
    src/config/*.cpp
    src/hash/*.cpp
)
file(GLOB TARGET_HEADERS include/http/*.hpp include/config/*.hpp
     include/hash/*.hpp
)
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files"
    FILES ${TARGET_SOURCES}
)
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files"
    FILES ${TARGET_HEADERS}
)
# ----------------------------------------------------------------------------------------------------------------------
# Find dependencies:
if(ENABLE_OPENSSL)
    find_package(OpenSSL REQUIRED)
endif()

find_package(Threads REQUIRED)

# ----------------------------------------------------------------------------------------------------------------------
# Libary part
add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE ${TARGET_HEADERS} ${TARGET_SOURCES})
target_include_interface_directories(
    ${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/core/shared/utils ${CMAKE_SOURCE_DIR}/core/shared/http
)
# Link dependencies:
target_find_dependencies(
    ${TARGET_NAME}
    PUBLIC_CONFIG
    httplib
    spdlog
    yyjson
    yaml-cpp
    cxxopts
    fmt
    bshoshany-thread-pool
)
target_link_system_libraries(
    ${TARGET_NAME}
    PUBLIC
    httplib::httplib
    spdlog::spdlog
    Threads::Threads
    yyjson::yyjson
    yaml-cpp::yaml-cpp
    cxxopts::cxxopts
    fmt::fmt
    bshoshany-thread-pool::bshoshany-thread-pool
    $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::SSL>
    $<$<BOOL:${ENABLE_OPENSSL}>:OpenSSL::Crypto>
)
target_compile_definitions(
    ${TARGET_NAME}
    INTERFACE $<$<BOOL:${ENABLE_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# link project_options/warnings
target_link_libraries(
    ${TARGET_NAME} PRIVATE starter_project_options starter_project_warnings
)

# include all the necessary directories
target_include_directories(
    ${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                           # Any other extra include required
)

# Executable part
target_sources(${TARGET_NAME} PRIVATE src/main.cpp)

# ----------------------------------------------------------------------------------------------------------------------
# install
add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/generate_keys.sh
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}
    COMMENT "Copying cmake file as demo to build dir"
)
# ----------------------------------------------------------------------------------------------------------------------
# tests
if(FEATURE_TESTS)
    add_subdirectory(test)
endif()
