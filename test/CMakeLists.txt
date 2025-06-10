include_directories(${CMAKE_SOURCE_DIR}/tt)

add_executable(ttInitEleVar ttInitEleVar.c)
target_link_libraries(ttInitEleVar yinTT)
add_test(NAME TTTestInitEleVar COMMAND ttInitEleVar)

add_executable(ttReadTT ttReadTT.c)
target_link_libraries(ttReadTT yinTT)
add_test(NAME TTTestReadTT COMMAND ttReadTT)