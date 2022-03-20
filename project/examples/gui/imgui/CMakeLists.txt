# Set additional application information
set(APPLICATION_NAME "QML Demo App")
set(APPLICATION_DESCRIPTION "Yet another Qt App")
set(APPLICATION_VERSION "0.1")
set(APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resource/icon/app.ico")
set(APPLICATION_RESOURCE)
add_subdirectory(imgui)
add_subdirectory(guizmo)
