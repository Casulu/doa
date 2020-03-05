gcc -g -std=c99 -Wall -o arraytable_test -I ../../datastructures-v1.0.8.2/include/ arraytable.c tabletest-1.9.c ../../datastructures-v1.0.8.2/src/array_1d/array_1d.c
valgrind --leak-check=full --show-reachable=yes ./arraytable_test 1000
