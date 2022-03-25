# ##############################################################################
# Define Target Information
# ##############################################################################
find_package(uvw CONFIG REQUIRED)
set(TARGET_NAME gateway)
file(GLOB TARGET_SOURCES src/*.cpp)
file(GLOB TARGET_HEADERS include/*.h include/*.hpp)

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

# include all the necessary directories
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# link
target_link_libraries(${TARGET_NAME} PRIVATE baselib cxxopts::cxxopts
  uvw::uvw)

if(WIN32)
  target_link_libraries(${TARGET_NAME} PRIVATE ws2_32)
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
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
