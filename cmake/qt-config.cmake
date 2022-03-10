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

  if(APPLE)
    set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION
                                                        Resources)

    # Identify MacOS bundle
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING
        "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
    set(MACOSX_BUNDLE_COPYRIGHT ${COPYRIGHT})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER ${IDENTIFIER})
    set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME})
  endif()

  if(APPLE)
    set(${FILES_TO_INCLUDE} ${ICON_FILE})
  elseif(WIN32)
    set(${FILES_TO_INCLUDE} ${RES_FILES})
  endif()
endmacro()

macro(init_os_bundle)
if (APPLE)
    set(OS_BUNDLE MACOSX_BUNDLE)
elseif (WIN32)
    set(OS_BUNDLE WIN32)
endif()
endmacro()
