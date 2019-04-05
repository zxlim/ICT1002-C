/*
 * ICT1002 Assignment 2, 2018-19 Trimester 2.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chat1002.h"
extern node_t *head_what;
extern node_t *head_where;
extern node_t *head_who;


/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {
	return "Chatbot";
}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {
	return "User";
}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {
	
	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else if (chatbot_is_debug(inv[0]))
		return chatbot_do_debug(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}
}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {
	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;
}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {
	snprintf(response, n, "Goodbye!");
	return 1;
}

/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {
	return compare_token(intent, "load") == 0;
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {
	/*
		fp:		The file pointer.
		ctr:	The number of successful results retrieved from the file.
	*/
	FILE *fp;
	int ctr = 0;
	char file_path[MAX_INPUT];

	// Get the file path from the user input.
	if (compare_token(inv[1], "from") == 0) {
		// LOAD[0] from[1] /path/to/file[2]
		strcpy(file_path, inv[2]);
	} else {
		// LOAD[0] /path/to/file[1]
		strcpy(file_path, inv[1]);
	}

	// Open the file in read mode.
	fp = fopen(file_path, "r");

	if (fp != NULL) {
		// File exists.
		ctr = knowledge_read(fp);
		fclose(fp);
		snprintf(response, n, "I have loaded %d results from the knowledge base [%s].", ctr, file_path);
	} else {
		// File does not exist.
		snprintf(response, n, "Sorry, I can't load the knowledge base [%s].", file_path);
	}

	return 0;
}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {
	return compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0;
}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {
	/*
		temp: 		A temp pointer to point to the start of the linked list.
		enti:		A string to store the entity.
	*/
	node_t *temp;
	char enti[MAX_INPUT] = "";
	
	// Assign the temp to the linked list based on the question word.
	if (compare_token(inv[0], "what") == 0){
		temp = head_what;
	} else if (compare_token(inv[0], "where") == 0){
		temp = head_where;
	} else if (compare_token(inv[0], "who") == 0){
		temp = head_who;
	}
	
	// Store the entity as "enti" variable.
	for (int i = 2; i < inc; i++){
		strcat(enti, inv[i]);
		if (i != inc - 1){
			strcat(enti, " ");
		}
	}
	
	// Loop the temp linked list to search for the entity.
	while(temp->next!=NULL){
		// If entity is found inside the database.
		if (compare_token(enti, temp->entity) == 0){
			snprintf(response, n, temp->response);
			break;
		}
		temp = temp->next;
	}
	
	// The last item in the linked list.
	if (compare_token(enti, temp->entity) == 0){
		snprintf(response, n, temp->response);
	} else {
		// There is no such item in our database. 
		// Push to the ask user for answer function.
		snprintf(response, n, enti);
	}
	
	return 0;
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {
	return compare_token(intent, "reset") == 0;
}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {
	knowledge_reset();
	snprintf(response, n, "I have reset my knowledge for this session.");
	return 0;
}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {
	/* to be implemented */
	return 0;
}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {
	/* to be implemented */
	return 0;
}
 
 
/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {
	
	char *smalltalk[]= {"good","bye","hello","it's","hi"};
	
	for (int i =0; i<5;i++)
	{
		if(compare_token(intent, smalltalk[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}

/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {
	char *random_bye[] = {"Bye Bye","See you again!"};
	char *random_hi[] = {"Hi","Hello","What's Up!"};
	int hi_rand_num;
	int bye_rand_num;
	
	for (int i=0; i<2;i++)
	{
		bye_rand_num = rand() %2;
	}
	
	for (int i=0; i<3;i++)
	{
		hi_rand_num = rand() %3;
	}
	
	if(compare_token("good", inv[0])==0)
	{
		snprintf(response,n,"Good %s",inv[1]);
	}
	else if(compare_token("bye", inv[0])==0)
	{
		snprintf(response,n,random_bye[bye_rand_num]);
	}
	else if(compare_token("hello", inv[0])==0 || compare_token("hi", inv[0])==0)
	{
		snprintf(response,n,random_hi[hi_rand_num]);
	}
	else if(compare_token("it's", inv[0])==0)
	{
		snprintf(response,n,"Indeed it is.");
	}
	return 0;
}


/*
 * Temporary debug functions.
*/
int chatbot_is_debug(const char *intent) {
	return compare_token(intent, "tempdebug") == 0;
}

int chatbot_do_debug(int inc, char *inv[], char *response, int n) {
	printf("\n=====[ TEMPORARY DEBUG MODE! REMOVE BEFORE SUBMISSION ]=====\n\n");
	printf("==========[what]===========\n");
	linkedlist_print(head_what);
	printf("==========[where]==========\n");
	linkedlist_print(head_where);
	printf("===========[who]===========\n");
	linkedlist_print(head_who);
	printf("===========================\n\n");
	printf("=====[ TEMPORARY DEBUG MODE! REMOVE BEFORE SUBMISSION ]=====\n\n");
	snprintf(response, n, "Done printing debugging information.");
	return 0;
}
