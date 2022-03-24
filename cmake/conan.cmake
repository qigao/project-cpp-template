if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(
    DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/0.17.0/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake"
    EXPECTED_HASH
      SHA256=3bef79da16c2e031dc429e1dac87a08b9226418b300ce004cc125a82687baeef
    TLS_VERIFY ON)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_autodetect(settings BUILD_TYPE ${CMAKE_BUILD_TYPE})
conan_cmake_run(
  CONANFILE
  packages/conan/base.txt
  BASIC_SETUP
  CMAKE_TARGETS
  INSTALL_FOLDER
  ${PKG_INSTALL_PREFIX}
  BUILD
  missing)

conan_cmake_run(
  CONANFILE
  packages/conan/networking.txt
  BASIC_SETUP
  CMAKE_TARGETS
  INSTALL_FOLDER
  ${PKG_INSTALL_PREFIX}
  BUILD
  missing)
