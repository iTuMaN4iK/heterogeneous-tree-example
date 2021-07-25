cmake_minimum_required(VERSION 3.19)

project(heterogeneous_tree_example)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rdparty/heterogeneous-tree)
add_subdirectory(3rdparty/CLI11)

add_subdirectory(visitor_impl)

add_executable(example main.cpp)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif ()

target_link_libraries(example
        PRIVATE
        heterogeneous_tree
        visitor_impl
        CLI11::CLI11
        )