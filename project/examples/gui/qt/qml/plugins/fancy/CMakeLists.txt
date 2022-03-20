set(TARGET_NAME fancy)
qt_add_qml_module(
  ${TARGET_NAME}
  STATIC
  URI
  com.mycompany.test
  VERSION
  1.0
  PLUGIN_TARGET
  ${TARGET_NAME}-plugin
  SOURCES
  thing.cpp
  thing.hpp
  QML_FILES
  FancyComponent.qml)

get_target_property(test ${TARGET_NAME}_qmlcache SOURCES)

target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Qml Qt6::Quick)
