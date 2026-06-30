cmake_minimum_required(VERSION 3.31.6)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Tiger VERSION 1.0)
project(Tiger LANGUAGES C)

add_compile_options(-W -Wall -Wextra -fsanitize=address -g -Wno-unused-variable)

add_link_options(-fsanitize=address)

include_directories(libs/Zi/array)
add_subdirectory(libs)

# array lib
include_directories(tt)
add_subdirectory(tt)

include(CTest)
add_subdirectory(test)

# --- Documentation target (off by default; needs Doxygen + a docs venv) -----
# Configure with -DYIN_BUILD_DOCS=ON, then:  cmake --build build --target docs
# The target delegates to scripts/build_docs.sh (Doxygen -> Sphinx pipeline).
option(YIN_BUILD_DOCS "Build the Doxygen + Sphinx documentation site" OFF)
if(YIN_BUILD_DOCS)
    find_program(DOXYGEN_EXECUTABLE doxygen REQUIRED)
    add_custom_target(docs
        COMMAND bash scripts/build_docs.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Yin documentation (Doxygen + Sphinx)")
endif()