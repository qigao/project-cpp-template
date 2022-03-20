add_subdirectory(qtwidgets/MyLib)
add_subdirectory(qtwidgets/App)
add_subdirectory(qml)
# ##############################################################################
# Define Install Targets
# ##############################################################################
install(
  TARGETS QtDemoApp qml-app
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
  LIBRARY DESTINATION ${BINARY_INSTALL_DIR}/lib
  ARCHIVE DESTINATION ${BINARY_INSTALL_DIR}/lib
  PUBLIC_HEADER DESTINATION ${BINARY_INSTALL_DIR}/include)
return()
