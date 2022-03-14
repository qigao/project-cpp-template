if(NOT BUILD_WITH_QT6)
  return()
endif()
add_subdirectory(MyLib)
add_subdirectory(App)
add_subdirectory(Qapp)
# ##############################################################################
# Define Install Targets
# ##############################################################################
install(
  TARGETS QtDemoApp QmlDemoApp
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
  LIBRARY DESTINATION ${BINARY_INSTALL_DIR}/lib
  ARCHIVE DESTINATION ${BINARY_INSTALL_DIR}/lib
  PUBLIC_HEADER DESTINATION ${BINARY_INSTALL_DIR}/include)
