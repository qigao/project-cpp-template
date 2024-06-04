## dependencies
set(DEPENDENCIES_CONFIGURED glfw3)

foreach(DEPENDENCY ${DEPENDENCIES_CONFIGURED})
  find_package(${DEPENDENCY} CONFIG REQUIRED)
endforeach()

# shared libs
set(IMGUI_BINDING_DIR
    ${PROJECT_BINARY_DIR}/bindings
    CACHE PATH "IMGUI_BINDING_DIR")
set(IMGUI_GIZMO_DIR
    ${PROJECT_SOURCE_DIR}/project/shared/imguizmo
    CACHE PATH "IMGUI_GIZMO_DIR")
file(GLOB GIZMO_SRC ${IMGUI_GIZMO_DIR}/*.cpp)
file(GLOB GIZMO_HEADER ${IMGUI_GIZMO_DIR}/*.h)
file(GLOB IMGUI_BINDING_SRC ${IMGUI_BINDING_DIR}/*.cpp)
file(GLOB IMGUI_BINDING_HEADER ${IMGUI_BINDING_DIR}/*.h)

# add subdirectories
add_subdirectory(imgui)
add_subdirectory(guizmo)
