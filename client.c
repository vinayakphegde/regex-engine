#include <stdio.h>
#include <stdlib.h>
#include "regex.h"

int main()
{
	int test;
	char z;
	char *text;
	char *pattern;
	node_t *t;
	
	text = (char *)malloc(sizeof(char)*4000);
	pattern = (char *)malloc(sizeof(char)*1000);
	
	scanf("%[^\n]s", text);
	scanf("%c",&z);
	scanf("%d",&test);
	scanf("%c",&z);
	
	while(test--)
	{
		scanf("%[^\n]s", pattern);
		scanf("%c", &z);
		t = match(pattern, text);
		if(t == NULL)
		{
			printf("No match found\n");
		}
		else
		{
			printf("%d %d\n", t->start, t->end == -1 ? 0 : t->end);
		}
	}
	return 0;
}

