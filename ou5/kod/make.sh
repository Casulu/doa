gcc -g -std=c99 -Wall -o is_connected -I ../../datastructures-v1.0.8.2/include/ graph.c is_connected.c
valgrind --leak-check=full --show-reachable=yes ./is_connected
