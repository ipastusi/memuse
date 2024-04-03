cmake_minimum_required(VERSION 3.22)
project(memuse C)

set(CMAKE_C_STANDARD 17)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -pedantic -Wall -Wextra -Werror -O2 -D_FORTIFY_SOURCE=2 -g3")

    enable_testing()

    add_executable(config_test
            test/config_test.c
            src/config.c
            src/config.h)
    add_test(NAME config_test
            COMMAND $<TARGET_FILE:config_test>)

    add_executable(alloc_test
            test/alloc_test.c
            src/alloc.c
            src/alloc.h
            src/config.c
            src/config.h)
    add_test(NAME alloc_test
            COMMAND $<TARGET_FILE:alloc_test>)

else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -pedantic -Wall -Wextra -Werror -O3 -g0 -DNDEBUG")
endif ()

add_executable(memuse
        src/memuse.c
        src/alloc.c
        src/alloc.h
        src/config.c
        src/config.h)
