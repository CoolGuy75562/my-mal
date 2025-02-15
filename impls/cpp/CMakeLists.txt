project(mal_cpp)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_compile_options(-g -Wall -Wextra -fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)

# step 0
add_executable(step0_repl step0_repl.cpp)

# step 1
add_executable(step1_read_print
    step1_read_print.cpp
    reader.cpp
    types.cpp
)

# step 2
add_executable(step2_eval
    step2_eval.cpp
    reader.cpp
    types.cpp
)
