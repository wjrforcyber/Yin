add_executable(ttInitEleVar ttInitEleVar.c)
target_link_libraries(ttInitEleVar yinTT)
add_test(NAME TTTestInitEleVar COMMAND ttInitEleVar)

add_executable(ttReadTT ttReadTT.c)
target_link_libraries(ttReadTT yinTT)
add_test(NAME TTTestReadTT COMMAND ttReadTT)

add_executable(ttCofactor ttCofactor.c)
target_link_libraries(ttCofactor yinTT)
add_test(NAME TTTestCofactor COMMAND ttCofactor)

add_executable(ttWeight ttWeight.c)
target_link_libraries(ttWeight yinTT)
add_test(NAME TTTestWeight COMMAND ttWeight)

add_executable(ttGetDistance ttGetDistance.c)
target_link_libraries(ttGetDistance yinTT)
add_test(NAME TTTestGetDistance COMMAND ttGetDistance)

set(TALIBS yinTT ziArray)
add_executable(ttIsop ttIsop.c)
target_link_libraries(ttIsop ${TALIBS})
add_test(NAME TTTestIsop COMMAND ttIsop)

add_executable(ttSim ttSim.c)
target_link_libraries(ttSim ${TALIBS})
add_test(NAME TTTestSim COMMAND ttSim)

add_executable(ttIsUnate ttUnate.c)
target_link_libraries(ttIsUnate ${TALIBS})
add_test(NAME TTTestIsUnate COMMAND ttIsUnate)