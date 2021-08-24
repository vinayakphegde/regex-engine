#ifndef REGEX_H
#define REGEX_H

/*
	contains the starting and ending index of the matched substring with the pattern. 
*/
struct node
{
	int start;
	int end;
};
typedef struct node node_t;


/*
	returns starting and ending position of the string 
	if the substring of text matches with the pattern
	else returns null.
*/
node_t *match(char *pat, char *text);

#endif
