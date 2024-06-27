# ===============================================================================
add_subdirectory(http_client)
add_subdirectory(http_server)
add_subdirectory(shared)
# ===============================================================================

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT  http_server)
