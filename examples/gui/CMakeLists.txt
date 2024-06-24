# ##############################################################################
# Define Targets
# ##############################################################################
## dependencies
set(DEPENDENCIES_CONFIGURED fmt cxxopts)

foreach(DEPENDENCY ${DEPENDENCIES_CONFIGURED})
  find_package(${DEPENDENCY} CONFIG REQUIRED)
endforeach()

add_subdirectory(hellolib)
add_subdirectory(helloapp)
add_subdirectory(baselib)

if(BUILD_WITH_QT6)
  add_subdirectory(qt)
  return()
endif()
add_subdirectory(imgui)
