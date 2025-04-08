cmake_minimum_required(VERSION 3.31.6)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Tiger VERSION 1.0)
project(Tiger LANGUAGES C)

add_compile_options(-W -Wall -Wextra)

# array lib
ADD_LIBRARY( yinTT STATIC tt/tt.c )
include_directories(tt)

include(CTest)
add_subdirectory(test)