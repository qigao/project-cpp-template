find_program(RAGEL 6.10)
set(TARGET_NAME dice)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

ragel_target(
    ${TARGET_NAME} ${CMAKE_CURRENT_LIST_DIR}/diceparser.rl
    ${CMAKE_CURRENT_LIST_DIR}/diceparser_rl.cpp
)
add_executable(
    ${TARGET_NAME} diceparser.cpp diceparser.h diceparser_rl.cpp main.cpp
)
set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${TARGET_NAME} benchmark::benchmark)
