#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <time.h>

#define MAX_DEPTH 10

/*
 * Test program for the stack implementation in stack.c.
 *
 * Author: Rasmus Lyxell (c19rll@cs.umu.se).
 *
 * Version information:
 *   2020-02-08: v1.0. First version for first turn in.
 */


void kill_stack_members(stack *s)
{
	void *v;
	//Frees all elements in the stack
	while (!stack_is_empty(s)) {
			v = stack_top(s);
			free(v);
			s = stack_pop(s);
	}
}


void random_use_test()
{
	stack *s = stack_empty(free)
	int stackValues[MAX_DEPTH] = {0};
	int currMaxPos = -1;
	//int pushPopSequence[MAX_DEPTH] = {0}; //0 for pop, 1 for push
	int **pushVal = malloc(sizeof(int) * MAX_DEPTH);
	int doPush;

	for (int i = 0; i < MAX_DEPTH; i++) {
		doPush = rand() % 2; //Get
		*pushVal[i] = ((rand() % 200000) + 1) * ((rand() % 2 + 1) * 2 - 1); //Randomized number between -200000 and 200000 not including 0
		if(currMaxPos == 0 || (doPush && currMaxPos){
			currMaxPos++;
			s = stack_push(s, *pushVal[i]);
			stackValues[currMaxPos] = *pushVal[i];
			pushPopSequence[i] = 1;
		} else{
			s = stack_pop(s);
			stackValues[currMaxPos] = 0;
			pushPopSequence[i] = 0;
			currMaxPos--;
		}
	}

	for (int i = currMaxPos; i >= 0; i--) {
		if(stack_top(s) != stackValues[i]){
			fprintf(stderr, "Value mismatch expected %d got %d\n", stackValues[i], stack_top(s));
			exit(EXIT_FAILURE);
		}
		s = stack_pop(s);
	}

	for (int i = 0; i < MAX_DEPTH; i++) {
		free(pushVal[i]);
	}
	kill_stack_members(s);
	stack_kill(s);
}


/*
 * Verify that a newly created stack is empty. Prints an error message
 * to stderr and exits via exit() with an error signal if the test
 * fails.
 */

void empty_stack_test()
{
	// Create empty stack.
	stack *s=stack_empty(free);

	// Verify that stack is empty.
	if (!stack_is_empty(s)) {
		// Write error message to stderr.
		fprintf(stderr, "FAIL: New stack is not empty!\n");
		// Exit with error message.
		exit(EXIT_FAILURE);
	}

	// The implementation passed the test. Clean up.
    // Deallocate the stack structure.
	stack_kill(s);
}


void push_test()
{
	stack *s = stack_empty(free)

	int testVal = rand();
	*v = testVal;

	s = stack_push(s, v)

	if (stack_top(s) != testVal){
		fprintf(stderr, "FAIL: stack_push does not push value correctly\n");
		exit(EXIT_FAILURE);
	}

	free(v);
	kill_stack_members(s);
	stack_kill(s);
}


/*
 * Verify that a if one element is inserted into an empty stack, the
 * stack is not empty. Prints an error message to stderr and exits via
 * exit() with an error signal if the test fails.
 */
void is_stack_empty_test()
{
	// Create empty stack.
	stack *s=stack_empty(free);

	// Create one element.
	int testVal = rand();
	*v = testVal;

	// Insert element first in stack.
	s = stack_push(s, v);

	// Verify that stack is NOT empty
	if (stack_is_empty(s)) {
		// Write error message to stderr.
		fprintf(stderr, "FAIL: stack_is_empty does not return correct values\n");
		// Exit with error message.
		exit(EXIT_FAILURE);
	}

	s = stack_pop(s)

	if (!stack_is_empty(s)) {
		// Write error message to stderr.
		fprintf(stderr, "FAIL: stack_is_empty does not return correct values\n");
		// Exit with error message.
		exit(EXIT_FAILURE);
	}
	// The implementation passed the test. Clean up.

    // Remove all elements from the stack.
	kill_stack_members(s);
    // Deallocate the stack structure.
	stack_kill(s);
}

/*
 * Verify that if you push a value to a stack then pop the same stack it remains
 * exactly as it was before the push. Prints an error to stderror if stack is
 * not unchanged and exits via exit() with an error signal if the test fails.
 */
void push_pop_test()
{
	/*Axiom 2: If we push a value onto a stack and then remove it the stack is
	ultimately unchanged*/
	// Create empty stack.
	stack *s = stack_empty(free);

	int *inspected_val;
	// Create element with random value.
	int *v = malloc(sizeof(*v));
	int testVal = rand();
	*v = testVal;

	// Insert element.
	s = stack_push(s, v);
	inspected_val = (int*)stack_top(s);
	s = stack_push(s, v);
	s = stack_pop(s);

	// Verify that value 4 is at the head of the stack.
	if (*inspected_val != testVal) {
		// *v = Write error m
		free(v);
		//Message to stderr.
		fprintf(stderr, "FAIL: Expected %d first in stack, got %d!\n",
			4,*inspected_val);
		// Exit with error message.
		exit(EXIT_FAILURE);
	}

	free(v);
        // Empty the stack.
	kill_stack_members(s);

	// Everything OK, clean up after the test. No memory leaks
	// allowed for passed tests.

	stack_kill(s);
}

int main (void)
{
	srand(time(NULL));

	is_stack_empty_test();
	empty_stack_test();
	push_test();
	push_pop_test();
	random_use_test();

	// More tests...

	fprintf(stderr,"SUCCESS: Implementation passed all tests. Normal exit.\n");
	return 0;
}
