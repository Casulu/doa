gcc -g -std=c99 -Wall -o dlisttable_test -I ../../datastructures-v1.0.8.2/include/ table2.c tabletest-1.9.c ../../datastructures-v1.0.8.2/src/dlist/dlist.c
valgrind ./dlisttable_test --leak-check=full --show-reachable=yes
