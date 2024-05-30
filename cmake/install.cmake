if(CMAKE_SKIP_INSTALL_RULES)
  return()
endif()

# the variables set using CACHE STRING "" are passed to package_project

# Package the project
package_project(
  TARGETS
  my_exe
  my_lib
  my_header_lib
  my_project_options
  my_project_warnings)

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
