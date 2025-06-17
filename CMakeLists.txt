cmake_minimum_required(VERSION 3.31.6)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Tiger VERSION 1.0)
project(Tiger LANGUAGES C)

add_compile_options(-W -Wall -Wextra -fsanitize=address -g)

add_link_options(-fsanitize=address)

include_directories(libs/Zi/array)
add_subdirectory(libs)

# array lib
include_directories(tt)
add_subdirectory(tt)

include(CTest)
add_subdirectory(test)