ADD_LIBRARY( yinTT STATIC tt.c isop.c simulation.c unate.c threshold.c symmetric.c parser.c selfdual.c)
target_link_libraries(yinTT PUBLIC ziArray)