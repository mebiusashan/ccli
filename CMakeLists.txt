cmake_minimum_required(VERSION 3.0)

project(ccli LANGUAGES C)

aux_source_directory(. source_list)
add_executable(ccli test.c ${source_list})

install(TARGETS ccli RUNTIME DESTINATION bin)
