# ##############################################################################
# Define the tray target
# ##############################################################################
set(TARGET_NAME tray)
add_library(${TARGET_NAME} INTERFACE)
target_sources(${TARGET_NAME} INTERFACE tray.h)
target_include_directories(${TARGET_NAME} INTERFACE ${TARGET_NAME}_SOURCE_DIRS)
if (WIN32)
    target_compile_definitions(${TARGET_NAME} INTERFACE TRAY_WINAPI)
elseif (APPLE)
    target_compile_definitions(${TARGET_NAME} INTERFACE TRAY_APPKIT)
    target_link_libraries(${TARGET_NAME} INTERFACE "-framework Cocoa")
    # Some warnings the library doesn't deal with
    target_compile_options(${TARGET_NAME} INTERFACE -Wno-unused-parameter)
    target_compile_options(${TARGET_NAME} INTERFACE -Wno-unused-variable)
    target_compile_options(${TARGET_NAME} INTERFACE -Wno-writable-strings)
    target_compile_options(${TARGET_NAME} INTERFACE -Wno-misleading-indentation)
elseif (UNIX)
    target_compile_definitions(${TARGET_NAME} INTERFACE TRAY_APPINDICATOR)
    target_link_libraries(${TARGET_NAME} INTERFACE "$(shell pkg-config --cflags appindicator3-0.1 --libs appindicator3-0.1)")
    # Some warnings the library doesn't deal with
    target_compile_options(${TARGET_NAME} INTERFACE -Wno-unused-parameter)
    target_compile_options(${TARGET_NAME} INTERFACE -Wno-unused-variable)
    target_compile_options(${TARGET_NAME} INTERFACE -Wno-writable-strings)
    target_compile_options(${TARGET_NAME} INTERFACE -Wno-misleading-indentation)
endif ()
