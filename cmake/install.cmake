
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
    "${PROJECT_NAME}-${PROJECT_VERSION}-${GIT_COMMIT_HASH}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
