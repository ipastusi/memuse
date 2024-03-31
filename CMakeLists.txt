cmake_minimum_required(VERSION 3.22)
project(memuse C)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -pedantic -Wall -Wextra -Werror -O3 -D_FORTIFY_SOURCE=2 -g3")

add_executable(memuse
        src/memuse.c
        src/alloc.c
        src/alloc.h
        src/config.c
        src/config.h)

enable_testing()
add_executable(unit_tests
        test/unit_tests.c
        src/config.c
        src/config.h)
add_test(NAME unit_tests
        COMMAND $<TARGET_FILE:unit_tests>)
