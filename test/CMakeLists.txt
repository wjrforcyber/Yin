include_directories(${CMAKE_SOURCE_DIR}/tt)

add_executable(ttInitEleVar ttInitEleVar.c)
target_link_libraries(ttInitEleVar yinTT)
add_test(NAME TTTestInitEleVar COMMAND ttInitEleVar)