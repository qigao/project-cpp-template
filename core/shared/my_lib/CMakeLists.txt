# Project Name
set(TARGET_NAME my_lib)

file(GLOB TARGET_SOURCES private/*.cpp)
file(GLOB TARGET_HEADERS public/*.h)

add_library(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE ${TARGET_SOURCES})
target_include_interface_directories(${TARGET_NAME} public) # Includes
target_link_libraries(
  ${TARGET_NAME} PRIVATE starter_project_options starter_project_warnings
)
# tests
if(FEATURE_TESTS)
  add_subdirectory(test)
endif()
