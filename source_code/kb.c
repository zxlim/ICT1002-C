#include <stdio.h>
#include <stdlib.h>
#include "kb.h"


node_t * node_create(char *intent, char *entity, char *resp) {
	/*
		This function creates a node.

		Arguments:
			intent 	[char *]:	The intent.
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

	strcpy(node->intent, intent);
	strcpy(node->entity, entity);
	strcpy(node->response, resp);
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
		if (current->next == NULL) {
			// End of linked list.
			current->next = node;
			break;
		}
		// Update the pointer.
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
