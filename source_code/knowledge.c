/*
 * ICT1002 Assignment 2, 2018-19 Trimester 2.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * kowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"
#include "kb.h"


/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n) {
	/* to be implemented */
	return KB_NOTFOUND;
}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {
	node_t *head, *new_node = NULL;

	if (compare_token(intent, "what") == 0) {
		// Intent is what.
		head = head_what;
	} else if (compare_token(intent, "where") == 0) {
		// Intent is where.
		head = head_where;
	} else if (compare_token(intent, "who") == 0) {
		// Intent is who.
		head = head_who;
	} else {
		// Invalid intent.
		return KB_INVALID;
	}

	new_node = node_create(intent, entity, response);

	if (new_node == NULL) {
		// Failed to allocate memory.
		return KB_NOMEM;
	}

	if (head == NULL) {
		// First node in the linked list.
		head = new_node;
	} else {
		// Continue the linked list.
		linkedlist_add(head, new_node);
	}

	return KB_FOUND;
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {
	/* to be implemented */
	return 0;
}


/*
 * Reset the knowledge base, removing all know entities from all intents.
 */
void knowledge_reset() {
	linkedlist_free(head_what);
	linkedlist_free(head_where);
	linkedlist_free(head_who);
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {
	/* to be implemented */
}
