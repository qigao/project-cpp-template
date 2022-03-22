# Set additional application information
set(APPLICATION_NAME "QML Demo App")
set(APPLICATION_DESCRIPTION "Yet another Qt App")
set(APPLICATION_VERSION "0.1")
set(APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resource/icon/app.ico")
set(APPLICATION_RESOURCE)

conan_cmake_run(
  CONANFILE
  ${CMAKE_SOURCE_DIR}/packages/conan/opengl.txt
  BASIC_SETUP
  CMAKE_TARGETS
  INSTALL_FOLDER
  ${CMAKE_INSTALL_PREFIX}
  BUILD
  missing)

add_subdirectory(imgui)
add_subdirectory(guizmo)

# ##############################################################################
# Define Install Targets
# ##############################################################################
install(
  TARGETS imguiDemo guizmo
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
  LIBRARY DESTINATION ${BINARY_INSTALL_DIR}/lib
  ARCHIVE DESTINATION ${BINARY_INSTALL_DIR}/lib
  PUBLIC_HEADER DESTINATION ${BINARY_INSTALL_DIR}/include)
