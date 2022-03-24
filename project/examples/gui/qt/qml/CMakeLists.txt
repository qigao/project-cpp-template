find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2
                                     QmlImportScanner)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

cmake_policy(SET CMP0071 NEW)

# Directories where Qt Creator can find QML files. (Not needed for builds, but
# makes Qt Creator code completion happy.)
list(APPEND QML_IMPORT_PATH "cmake-build-debug/lib")

# Prevent adding duplicate values at each run of CMake.
list(REMOVE_DUPLICATES QML_IMPORT_PATH)

# The variable is cached in ${BUILD_DIR}/CMakeCache.txt. We need FORCE to change
# it there immediately. Also, add a comment to the cache file.
set(QML_IMPORT_PATH
    ${QML_IMPORT_PATH}
    CACHE STRING "additional qml import paths" FORCE)
# Set additional application information
set(APPLICATION_NAME "QML Demo App")
set(APPLICATION_DESCRIPTION "Yet another Qt App")
set(APPLICATION_VERSION "0.1")
set(APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resource/icon/app.ico")

add_subdirectory(plugins)
add_subdirectory(app)
