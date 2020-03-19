cmake_minimum_required(VERSION 3.15)
project(kod C)

set(CMAKE_C_STANDARD 99)

include_directories(../../datastructures-v1.0.8.2/include/)

add_executable(kod
        graph.c
        is_connected.c
        ../../datastructures-v1.0.8.2/src/dlist/dlist.c
        ../../datastructures-v1.0.8.2/src/queue/queue.c
        ../../datastructures-v1.0.8.2/src/list/list.c
        )
