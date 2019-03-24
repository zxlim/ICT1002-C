#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() 
{
  char input[255];
  while(1)
  {
    printf("Chatbot: Hello, I'm Chatbot.\n");
    printf("User: ");
    scanf("%s",input);
	strlwr(input);
	
    if (strcmp(input, "exit")==0 ||strcmp(input, "bye")==0 || strcmp(input, "bye bye")==0)
    {
		printf("Chatbot: Let's chat again soon. Good Bye!\n");
		break;
    }
	else if(strcmp(input, "hi")==0 || strcmp(input, "hello")==0)
	{
		printf("Chatbot: Hello, I'm Chatbot. How are you?\n");
		printf("User: ");
		scanf("%s",input);
		strlwr(input);
		if(strcmp(input, "i am fine")==0 || strcmp(input,"fine")== 0)
		{
			printf("Chatbot: Great to hear! How are you feeling now?\n");
			printf("User: ");
			scanf("%s",input);
			strlwr(input);
			
			if (strcmp(input, "happy")==0)
			{
				printf("Chatbot: That's good news! I am happy for you too.\n\n");
				continue;
			}
			else if (strcmp(input, "sad")==0)
			{
				printf("Chatbot: What happened? Wanna hear a joke that might cheer you up?\n");
				printf("User: ");
				scanf("%s",input);
				strlwr(input);
				if(strcmp(input, "yes")==0 || strcmp(input, "sure")==0)
				{
					printf("Chatbot: I told my dad to embrace his mistakes.\n He cried. Then hugged my sister and me.\n Hope this cheers you up :) \n\n");
					continue;
				}
				else
				{
					printf("Chatbot: Awww, hope you will feel better after some time.\n\n");
					continue;
				}
			}
		}

	}
	else
	{
		printf("Chatbot: Sorry I do not understand \"%s\"\n",input);
		continue;
	}
  }
  return 0;
}