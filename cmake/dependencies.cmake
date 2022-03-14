list(APPEND CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
# Required for Testing
if(BUILD_TESTING)
  find_package(doctest REQUIRED)
endif()

# Optional Dependency, doesn't trigger error if missing
find_package(OpenSSL)
find_package(cxxopts)
