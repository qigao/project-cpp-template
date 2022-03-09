if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(
    DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/release/0.17/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake"
    TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES fmt/8.1.1 GENERATORS cmake_find_package)
conan_cmake_configure(REQUIRES spdlog/1.9.2 GENERATORS cmake_find_package)
conan_cmake_configure(REQUIRES docopt.cpp/0.6.3 GENERATORS cmake_find_package)
conan_cmake_configure(REQUIRES catch2/2.13.8 GENERATORS cmake_find_package)
conan_cmake_configure(REQUIRES doctest/2.4.6 GENERATORS cmake_find_package)
conan_cmake_configure(REQUIRES openssl/1.1.1l GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(
  PATH_OR_REFERENCE
  .
  BUILD
  missing
  REMOTE
  conancenter
  SETTINGS
  ${settings})
