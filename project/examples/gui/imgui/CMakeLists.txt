# Set additional application information
set(APPLICATION_NAME "QML Demo App")
set(APPLICATION_DESCRIPTION "Yet another Qt App")
set(APPLICATION_VERSION "0.1")
set(APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resource/icon/app.ico")

conan_cmake_run(
  CONANFILE
  ${CMAKE_SOURCE_DIR}/packages/conan/opengl.txt
  BASIC_SETUP
  CMAKE_TARGETS
  INSTALL_FOLDER
  ${PKG_INSTALL_PREFIX}
  BUILD
  missing)

add_subdirectory(imgui)
add_subdirectory(guizmo)
