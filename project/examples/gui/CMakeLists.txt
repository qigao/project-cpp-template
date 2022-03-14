find_package(
  Qt6
  COMPONENTS Core
  REQUIRED)
if(Qt6_NOTFOUND)
  return()
endif()
add_subdirectory(MyLib)
add_subdirectory(App)
add_subdirectory(Qapp)
