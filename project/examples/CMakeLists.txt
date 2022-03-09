# ##############################################################################
# Define Targets
# ##############################################################################
add_subdirectory(hellolib)
add_subdirectory(helloapp)
add_subdirectory(baselib)
add_subdirectory(MyLib)
add_subdirectory(App)


# ##############################################################################
# Define Install Targets
# ##############################################################################
install(
  TARGETS hellolib helloapp QtDemoApp
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
  LIBRARY DESTINATION ${BINARY_INSTALL_DIR}/lib
  ARCHIVE DESTINATION ${BINARY_INSTALL_DIR}/lib
  PUBLIC_HEADER DESTINATION ${BINARY_INSTALL_DIR}/include)



