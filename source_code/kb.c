#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat1002.h"
extern node_t *head_what;
extern node_t *head_where;
extern node_t *head_who;


node_t * node_create(const char *entity, const char *resp) {
	/*
		This function creates a node.

		Arguments:
			entity 	[char *]:	The entity.
			resp 	[char *]:	The response.

		Returns:
			node	[node_t *]:	The newly created node.
			NULL	[NULL]:		Returns NULL if memory allocation fails.
	*/
	node_t *node = malloc(sizeof(node_t));

	if (node == NULL) {
		printf("[!] Memory allocation failure.\n");
		return NULL;
	}

	strncpy(node->entity, entity, MAX_ENTITY);
	strncpy(node->response, resp, MAX_RESPONSE);
	node->next = NULL;

	return node;
}


void linkedlist_add(node_t *head, node_t *node) {
	/*
		This function adds a new node to the linked list.

		Arguments:
			head 	[node_t *]:	The head node of the linked list.
			node 	[node_t *]:	The new node to add to the linked list.
	*/
	node_t *current = head;

	while (current != NULL) {
		if (compare_token(current->entity, node->entity) == 0) {
			// Entity already exist. Overwrite the response.
			strncpy(current->response, node->response, MAX_RESPONSE);
		} else if (current->next == NULL) {
			// End of linked list.
			current->next = node;
			break;
		}
		// Update the pointer.
		current = current->next;
	}
}


void linkedlist_print(node_t *head) {
	/*
		This function prints out the word value of all nodes in a linked list.
		For debugging use only.

		Arguments:
			head 	[node_t *]:	The head node of the linked list.
	*/
	node_t *current = head;

	while (current != NULL) {
		printf("[%s] %s\n", current->entity, current->response);
		current = current->next;
	}
}


void linkedlist_free(node_t *node) {
	/*
		This function attempts to free up all nodes in a linked list.

		Arguments:
			node 	[node_t *]:	The head node of the linked list.
	*/
	node_t *current;

	while (node != NULL) {
		current = node;
		node = node->next;
		free(current);
	}
}


int kb_update_what(node_t *new_node) {
	/*
		This function adds a new node to the "what" knowledge base.

		Arguments:
			new_node 	[node_t *]:	The new node to add to the knowledge base.
	*/
	if (new_node == NULL) {
		// Failed to allocate memory.
		return KB_NOMEM;
	}

	if (head_what == NULL) {
		// Linked list is empty, new_node shall be the first node in the list.
		head_what = new_node;
	} else {
		// Append new_node to the linked list.
		linkedlist_add(head_what, new_node);
	}
	return KB_FOUND;
}


int kb_update_where(node_t *new_node) {
	/*
		This function adds a new node to the "where" knowledge base.

		Arguments:
			new_node 	[node_t *]:	The new node to add to the knowledge base.
	*/
	if (new_node == NULL) {
		// Failed to allocate memory.
		return KB_NOMEM;
	}

	if (head_where == NULL) {
		// Linked list is empty, new_node shall be the first node in the list.
		head_where = new_node;
	} else {
		// Append new_node to the linked list.
		linkedlist_add(head_where, new_node);
	}
	return KB_FOUND;
}


int kb_update_who(node_t *new_node) {
	/*
		This function adds a new node to the "who" knowledge base.

		Arguments:
			new_node 	[node_t *]:	The new node to add to the knowledge base.
	*/
	if (new_node == NULL) {
		// Failed to allocate memory.
		return KB_NOMEM;
	}

	if (head_who == NULL) {
		// Linked list is empty, new_node shall be the first node in the list.
		head_who = new_node;
	} else {
		// Append new_node to the linked list.
		linkedlist_add(head_who, new_node);
	}
	return KB_FOUND;
}
