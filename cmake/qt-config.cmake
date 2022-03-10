macro(add_project_meta FILES_TO_INCLUDE)
  if(APPLE)
    set(ICON_FILE ${RESOURCE_FOLDER}/${ICON_NAME}.icns)
  elseif(WIN32)
    set(ICON_FILE ${RESOURCE_FOLDER}/${ICON_NAME}.ico)
  endif()

  if(WIN32)
    configure_file(
      "${PROJECT_SOURCE_DIR}/templates/qt/resources/windows_metafile.rc.in"
      "windows_metafile.rc")
    set(RES_FILES "windows_metafile.rc")
  endif()
endmacro()
endmacro()
