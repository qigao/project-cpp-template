# - A macro that fetches project_options
# This module fetches project_options from the specified repository and tag
#
include(FetchContent)
set(PROJECT_OPTIONS_VERSION "v0.35.1")
FetchContent_Declare(
  _project_options
  URL https://github.com/aminya/project_options/archive/refs/tags/${PROJECT_OPTIONS_VERSION}.zip
)

FetchContent_MakeAvailable(_project_options)

include(${_project_options_SOURCE_DIR}/Index.cmake)
include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)
