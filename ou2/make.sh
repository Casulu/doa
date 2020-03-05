gcc -g -Wall -std=c99 -o stack_test stack_test.c ../datastructures-v1.0.8.2/src/stack/stack.c -I ../datastructures-v1.0.8.2/include/

valgrind --leak-check=full --show-reachable=yes ./stack_test
