
cmake_minimum_required(VERSION 3.10)

project(BST)

set(HEADER_FILES )
set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(COMPILER_FLAGS -g -std=c++17 -w -Wall -Wextra -O0)

add_compile_options(${COMPILER_FLAGS})

add_executable(BST  ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(BST PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(BST PROPERTIES LINKER_LANGUAGE CXX)







