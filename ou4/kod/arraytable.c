#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "array_1d.h"

/*
 * Implementation of a generic table using one dimensional
 * arrays for.
 *
 * Duplicates are handled by insert.
 *
 * Author: c19rll@cs.umu.se
 			Rasmus Lyxell
 *
 * Version information:
 *   2020-02-24: v1.0, first version.
 */


#define MAX_ENTRIES 80000

// ===========INTERNAL DATA TYPES============

struct table_entry{
	void *key;
	void *value;
};

struct table{
	int firstEmptyElement;
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
};



// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * table_empty() - Create an empty table.
 * @key_cmp_func: A pointer to a function to be used to compare keys.
 * @key_free_func: A pointer to a function (or NULL) to be called to
 *		   de-allocate memory for keys on remove/kill.
 * @value_free_func: A pointer to a function (or NULL) to be called to
 *		     de-allocate memory for values on remove/kill.
 *
 * Returns: Pointer to a new table.
 */
table *table_empty(compare_function *key_cmp_func, free_function key_free_func, free_function value_free_func)
{
	table *t = calloc(1, sizeof(table));

	t->entries = array_1d_create(0, MAX_ENTRIES - 1, NULL);
	t->firstEmptyElement = array_1d_low(t->entries);
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;

	return t;
}


/**
 * table_is_empty() - Check if a table is empty.
 * @table: Table to check.
 *
 * Returns: True if table contains no key/value pairs, false otherwise.
 */
bool table_is_empty(const table *t)
{
	//Check if the first empty element is at the lowest position
	if(t->firstEmptyElement == array_1d_low(t->entries)){
		return true;
	} else{
		return false;
	}
}

/**
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. If key already exists in the table
 * then the existing entry has its key pointer and/or value-pointer updated
 * to match the latest inserted pair.
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{
	bool keyFound = false;
	int i  = 0;


	while(!keyFound && i < t->firstEmptyElement){
		//Update currEntry to entry at current index in while-loop
		struct table_entry *currEntry = array_1d_inspect_value(t->entries, i);

		if(!(t->key_cmp_func(currEntry->key, key))){ //If keys are equal
			keyFound = true;
			//Free previous key (if it is our business) and update pointer if input key is not the same allocated
			if(currEntry->key != key){
				if(t->key_free_func != NULL){
					t->key_free_func(currEntry->key);
				}
				currEntry->key = key;
			}
			//Free previous value (if it is our business) and update pointer if input calue is not the same allocated memory
			//as previuos entry
			if(currEntry->value != value){
				if(t->value_free_func != NULL){
					t->value_free_func(currEntry->value);
				}
				currEntry->value = value;
			}
		}
		++i;
	}
	//Allocate new entry and insert input key and value into table if no duplicate was found.
	if(keyFound != true && t->firstEmptyElement < MAX_ENTRIES){
		struct table_entry *entry = malloc(sizeof(struct table_entry));
		entry->key = key;
		entry->value = value;

		array_1d_set_value(t->entries, entry, t->firstEmptyElement);
		t->firstEmptyElement++;
	}

}

/**
 * table_lookup() - Look up a given key in a table.
 * @table: Table to inspect.
 * @key: Key to look up.
 *
 * Returns: The value corresponding to a given key, or NULL if the key
 * is not found in the table.
 */
void *table_lookup(const table *t, const void *key)
{
	for (int i = array_1d_low(t->entries); i < t->firstEmptyElement; ++i) {
		struct table_entry *currEntry = array_1d_inspect_value(t->entries, i);

		if(!(t->key_cmp_func(currEntry->key, key))){
			return currEntry->value;
		}
	}
	//Return NULL if no key was found
	return NULL;
}

/**
 * table_choose_key() - Return an arbitrary key.
 * @t: Table to inspect.
 *
 * Return an arbitrary key stored in the table. Can be used together
 * with table_remove() to deconstruct the table. Undefined for an
 * empty table.
 *
 * Returns: An arbitrary key stored in the table.
 */
void *table_choose_key(const table *t)
{
	//Return internally last table_entry.
	struct table_entry *lastEntry = array_1d_inspect_value(t->entries, t->firstEmptyElement - 1);
	return lastEntry->key;
}

/**
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Will call any free functions set for keys/values.
 * Does nothing if key is not found in the table.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key)
{
	int i = t->firstEmptyElement-1;
	bool keyFound = false;

	//Loop backwards from end of table to integrate better with table_choose_key()
	while(!keyFound && i >= 0){
		struct table_entry *currEntry = array_1d_inspect_value(t->entries, i);
		if(!(t->key_cmp_func(currEntry->key, key))){
			keyFound = true;

			if(t->key_free_func != NULL){
				t->key_free_func(currEntry->key);
			}
			if(t->value_free_func != NULL){
				t->value_free_func(currEntry->value);
			}
			free(currEntry);

			//Place the pointer to the last table_entry in the newly "empty" index in the array to
			//fill the gap created.
			struct table_entry *lastEntry = array_1d_inspect_value(t->entries, t->firstEmptyElement - 1);
			array_1d_set_value(t->entries, lastEntry, i);
			t->firstEmptyElement--;
		}
		--i;
	}
}

/*
 * table_kill() - Destroy a table.
 * @table: Table to destroy.
 *
 * Return all dynamic memory used by the table and its elements. If a
 * free_func was registered for keys and/or values at table creation,
 * it is called each element to free any user-allocated memory
 * occupied by the element values.
 *
 * Returns: Nothing.
 */
void table_kill(table *t){
	while(t->firstEmptyElement > 0){
		struct table_entry *lastEntry = array_1d_inspect_value(t->entries, t->firstEmptyElement - 1);
		if (t->key_free_func != NULL) {
			t->key_free_func(lastEntry->key);
		}
		if (t->value_free_func != NULL) {
			t->value_free_func(lastEntry->value);
		}

		free(lastEntry);
		t->firstEmptyElement--;
	}
	array_1d_kill(t->entries);
	free(t);
}

/**
 * table_print() - Print the given table.
 * @t: Table to print.
 * @print_func: Function called for each key/value pair in the table.
 *
 * Iterates over the key/value pairs in the table and prints them.
 * Will print all stored elements.
 *
 * Returns: Nothing.
 */
void table_print(const table *t, inspect_callback_pair print_func){
	for (int i = 0; i < t->firstEmptyElement; ++i) {
		struct table_entry *currEntry = array_1d_inspect_value(t->entries, i);
		print_func(currEntry->key, currEntry->value);
	}
}
