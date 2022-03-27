# ##############################################################################
# Define Targets
# ##############################################################################
add_subdirectory(hellolib)
add_subdirectory(helloapp)
add_subdirectory(baselib)
add_subdirectory(gui)

if(NOT LINUX)
  add_subdirectory(gateway)
endif()
