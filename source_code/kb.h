#ifndef _KB_H
#define _KB_H

#include <stdio.h>


/* Intent-Entity-Response struct. */
typedef struct node_struct {
	char intent[MAX_INTENT];
	char entity[MAX_ENTITY];
	char response[MAX_RESPONSE];
	struct node_struct *next; // Pointer to the next node in the linked list.
} node_t;


/* Linked lists */
node_t *head_what = NULL;
node_t *head_where = NULL;
node_t *head_who = NULL;


node_t * node_create(char *str);
void linkedlist_add(node_t *head, node_t *node)
void linkedlist_free(node_t *node);


#endif