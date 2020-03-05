gcc -g -std=c99 -Wall -o mtftable_test -I ../../datastructures-v1.0.8.2/include/ mtftable.c tabletest-1.9.c ../../datastructures-v1.0.8.2/src/dlist/dlist.c
valgrind ./mtftable_test --leak-check=full --show-reachable=yes
