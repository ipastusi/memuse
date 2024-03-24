cmake_minimum_required(VERSION 3.22)
project(memuse C)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -pedantic -Wall -Wextra -Werror -O3 -D_FORTIFY_SOURCE=2 -g3")

add_executable(memuse src/memuse.c)
