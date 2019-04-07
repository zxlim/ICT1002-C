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
#include <time.h>
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
	// else if (chatbot_is_debug(inv[0]))
	// 	return chatbot_do_debug(inc, inv, response, n);
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
	// Free the allocated memory.
	knowledge_reset();
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
		unsure:		A string for unsure questions.
		enti:		A string to store the entity.
		ans:		A string to store the answer to the question.
	*/
	int i = 1;
	char unsure[MAX_RESPONSE] = "I don't know. ";
	char enti[MAX_ENTITY] = "";
	char ans[MAX_RESPONSE] = "";
	
	// Check where does the question start.
	if (compare_token(inv[1], "is") == 0 || compare_token(inv[1], "are") == 0) {
		i = 2;
	}

	// Store the entity as "enti" variable.
	for (; i < inc; i++) {
		size_t after_check = (size_t)(strlen(enti) + strlen(inv[i]));

		if (after_check < MAX_ENTITY) {
			// "enti" still has space for the next word.
			strcat(enti, inv[i]);

			if (i != (inc - 1)) {
				// Add a space between words.
				strcat(enti, " ");
			}
		} else {
			// Not enough space to store the word and subsequent words.
			size_t remainder = (size_t)(MAX_ENTITY - (int)(strlen(enti)));
			if (remainder > 0) {
				strncat(enti, inv[i], remainder);
			}
			break;
		}
	}

	if (knowledge_get(inv[0], enti, response, n) == KB_NOTFOUND) {
		// Can't find response. Form string to ask for response.
		for (int j = 0; j < inc; j++) {
			strcat(unsure, inv[j]);

			if (j != (inc - 1)) {
				// Add a space between words.
				strcat(unsure, " ");
			}
		}
		// Account for question mark at end of string.
		strcat(unsure, "?");

		prompt_user(ans, MAX_RESPONSE, "%s", unsure);
		knowledge_put(inv[0], enti, ans);
		snprintf(response, n, "Thank you.");
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
	// Reseed the random number generator.
	srand((unsigned)(time(NULL)));
	// Reset the knowledge base in memory.
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
	return compare_token(intent, "save") == 0;
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
	/*
		fp:		The file pointer.

	*/
	FILE *fp;
	char file_path[MAX_INPUT];

	// Get the file path from the user input.
	if (compare_token(inv[1], "to") == 0 || compare_token(inv[1], "as") == 0) {
		// Save[0] to[1] /path/to/file[2]
		strcpy(file_path, inv[2]);
	} else {
		// save[0] /path/to/file[1]
		strcpy(file_path, inv[1]);
	}

	// Open the file in write mode.
	fp = fopen(file_path, "w");

	if (fp != NULL) {
		// File exists.
		knowledge_write(fp);
		fclose(fp);
		snprintf(response, n, "I have saved the results from the knowledge base to [%s].", file_path);
	} else {
		// File does not exist.
		snprintf(response, n, "Sorry, I can't save the knowledge base to [%s].", file_path);
	}
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
	const char *smalltalk[] = {
		"can", "good", "hello", "hey", "hi", "it", "its", "it's", "time"
	};
	
	for (int i = 0; i < 9; i++) {
		if (compare_token(intent, smalltalk[i]) == 0) {
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
	size_t rand_int;
	const char *random_hi[] = {"Hi!", "Hello!", "Hey there!", "What's Up?"};
	const char *random_can[] = {
		"I don't know. Can I?", "What do you think?", "Maybe I could!", "I could always try..."
	};

	rand_int = (size_t)(rand() % 4);
	
	if (compare_token("time", inv[0]) == 0) {
		char the_time[24];
		time_t timer = time(NULL);
		strftime(the_time, 24, "%I:%M %p, %d %b %Y", localtime(&timer));
		snprintf(response, n, "It is now %s.", the_time);
	} else if (compare_token("good", inv[0]) == 0) {
		if (inc > 1) {
			snprintf(response, n, "Good %s to you too!", inv[1]);
		} else {
			snprintf(response, n, "Good day!");
		}
	} else if (
		compare_token("hello", inv[0]) == 0 ||
		compare_token("hey", inv[0]) == 0 ||
		compare_token("hi", inv[0]) == 0
	) {
		snprintf(response, n, "%s" , random_hi[rand_int]);
	} else if (compare_token("can", inv[0]) == 0) {
		snprintf(response, n, "%s" , random_can[rand_int]);
	} else if (
		compare_token("it", inv[0]) == 0 ||
		compare_token("its", inv[0]) == 0 ||
		compare_token("it's", inv[0]) == 0
	) {
		snprintf(response, n, "Indeed it is.");
	}
	return 0;
}


/*
 * Temporary debug functions.
*/
// int chatbot_is_debug(const char *intent) {
// 	return compare_token(intent, "tempdebug") == 0;
// }

// int chatbot_do_debug(int inc, char *inv[], char *response, int n) {
// 	printf("\n=====[ TEMPORARY DEBUG MODE! REMOVE BEFORE SUBMISSION ]=====\n\n");
// 	printf("==========[what]===========\n");
// 	linkedlist_print(head_what);
// 	printf("==========[where]==========\n");
// 	linkedlist_print(head_where);
// 	printf("===========[who]===========\n");
// 	linkedlist_print(head_who);
// 	printf("===========================\n\n");
// 	printf("=====[ TEMPORARY DEBUG MODE! REMOVE BEFORE SUBMISSION ]=====\n\n");
// 	snprintf(response, n, "Done printing debugging information.");
// 	return 0;
// }
