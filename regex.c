#include <stdio.h>
#include <stdlib.h>
#include "regex.h"

/*-----------------------------------------------function prototypes-----------------------------------------*/

/*
	returns a pointer to last location of the matched string
	if the substring of text matches with the pattern
	else return null.
*/
static char *match_here(char *pat, char *text);


/*
	try to match for the all the supported escape characters with the text and 
	returns a pointer to last location of the matched string
	if the substring of text matches with the pattern
	else return null.	
*/
static char *match_escape(char *pat, char *text);


/*
	match the pattern with any digit from the text.
*/
static char *match_escape_d(char *pat, char *text);


/*
	match the pattern with any word character(a-z, A-Z, 0-9) from the text.
*/
static char *match_escape_w(char *pat, char *text);


/*
	match the pattern with any space character(space, tab, newline) from the text.
*/
static char *match_escape_s(char *pat, char *text);


/*
	match the pattern with the character specified from the text.
*/
static char *match_character(char *pat, char *text, char ch);


/*
	match the pattern with the characters specified in the class array from the text.
*/
static char *match_character_class(char *pat, char *text, char *class);


/*
	match the pattern with the character classes specified between the square backets from the text.
*/
static char *match_escape_character_classes(char *pat, char *text);


/*
	matches zero or more occurence of given character and comsumes as few
	characters as possible. 
*/
static char *match_star_non_greedy(char* pat, char* text, char ch);


/*
	matches zero or more occurence of '.' character and comsumes as few
	characters as possible. 
*/
static char *match_star_non_greedy_dot(char *pat, char *text);


/*
	matches zero or more occurence of characters in class and comsumes as few
	characters as possible. 
*/
static char *match_star_non_greedy_class(char *pat, char *text, char *class);


/*
	matches zero or more occurence of given character and comsumes as many
	characters as possible.
*/
static char *match_star_greedy(char* pat, char* text, char ch);


/*
	matches zero or more occurence of '.' character and comsumes as many
	characters as possible.
*/
static char *match_star_greedy_dot(char *pat, char *text);


/*
	matches zero or more occurence of characters in class and comsumes as many
	characters as possible.
*/
static char *match_star_greedy_class(char *pat, char *text, char *class);


/*
	matches one or zero occurence of preceeding character.
*/
static char *match_question(char *pat, char *text, char ch);


/*
	matches one or zero occurence of '.' character.
*/
static char *match_question_dot(char *pat, char *text);


/*
	matches one or zero occurences of characters in class array.
*/
static char *match_question_class(char *pat, char *text, char *class);


/*
	checks if ch is present in array of characters or not.
*/
static int is_present(char ch, char *class);


/*-----------------------------------------------global variables-----------------------------------------*/

char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};

char words[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
				'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i','j', 'k',
				'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
				'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\0'};

char spaces[] = {' ', '\t', '\n'};

/*-----------------------------------------------implementation-----------------------------------------*/

node_t *match(char *pat, char *text)
{
	/*
	
		@brief	:	returns starting and ending position of the string 
					if the substring of text matches with the pattern
					else return null.
		
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
			
	*/
	
	char *original;
	char *t;
	char *cur;
	node_t *temp;
	int start;
	int end;
	
	original = text;
	
	if(*pat == '^')
	{
		if((t = match_here(pat + 1, text)) == NULL)
		{
			return NULL;	
		}
		else
		{
			temp = (node_t *)malloc(sizeof(node_t));
			start = 0;
			end = t - original;
			temp->start = start;
			temp->end = end;
			return temp;
		}
	}
	
	do
	{
		cur = text;
		if((t = match_here(pat, text)) != NULL)
		{
			temp = (node_t *)malloc(sizeof(node_t));
			start = cur - original;
			end = t - original;
			temp->start = start;
			temp->end = end;
			return temp;	
		}
	}while(*text++ != '\0');
	
	return NULL;
}


static char *match_here(char *pat, char *text)
{

	/*
		@brief	:	returns a pointer to last location of the matched string
					if the substring of text matches with the pattern
					else return null.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
		
	*/
	
	// empty pattern
	if(*pat == '\0')
	{	
		return text-1;	
	}
	
	// escape character
	if(*pat == '\\')
	{
		return match_escape(pat + 1, text);
	}
	
	// character class
	if(*pat == '[')
	{
		return match_escape_character_classes(pat, text);
	}
	
	// zero or more (non greedy)
	if(pat[1] == '*' && pat[2] == '?')
	{
		return match_star_non_greedy(pat + 3, text, pat[0]);
	}
	
	// zero or more (greedy)
	if(pat[1] == '*')
	{
		return match_star_greedy(pat + 2, text, pat[0]);
	}
		
	// one or more (non greedy)	
	if(pat[1] == '+' && pat[2] == '?')
	{
		if(*text != '\0' && (*pat == '.' || *pat == *text))
		{
			return match_star_non_greedy(pat + 3, text, pat[0]);
		}
		else
		{
			return NULL;
		}
	}
	
	// one or more (greedy)
	if(pat[1] == '+')
	{
		if(*text != '\0' && (*pat == '.' || *pat == *text))
		{
			return match_star_greedy(pat + 2, text, pat[0]);
		}
		else
		{
			return NULL;
		}
	}
	
	// once or none
	if(pat[1] == '?')
	{
		return match_question(pat + 2, text, pat[0]);
	}
	
	// end line character
	if(pat[0] == '$' && pat[1] == '\0')
	{
		return *text == '\0' ? text-1 : NULL;		
	}
	
	if(*text != '\0' && (*pat == '.' || *pat == *text))
	{
		return match_here(pat + 1, text + 1);
	}	
	return NULL;	
}


static char *match_escape(char *pat, char *text)
{	
	/*
		@brief	:	try to match for the all the supported escape characters with the text and 
					returns a pointer to last location of the matched string
					if the substring of text matches with the pattern
					else return null.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	if(*pat == 'd')
	{
		return match_escape_d(pat + 1, text);
	}
	
	if(*pat == 'w')
	{
		return match_escape_w(pat + 1, text);
	}
	
	if(*pat == 's')
	{
		return match_escape_s(pat + 1, text);
	}
	
	return match_character(pat + 1, text, *pat);
}


static char *match_escape_d(char *pat, char *text)
{
	/*
		@brief	:	match the text with any digit from the text.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/

	return match_character_class(pat, text, digits);
}


static char *match_escape_w(char *pat, char *text)
{
	/*
		@brief	:	match the text with any word character(a-z, A-Z, 0-9) from the text.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	return match_character_class(pat, text, words);
}


static char *match_escape_s(char *pat, char *text)
{
	/*
		@brief	:	match the pattern with any space character(space, tab, newline) from the text.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	return match_character_class(pat, text, spaces);
}


static char *match_character(char *pat, char *text, char ch)
{
	/*
		@brief	:	match the pattern with the character specified from the text.
					
		@param	:	ch	-		character needs to be matched			
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	if(*pat == '*' && pat[1] == '?')
	{
		if(ch == '.')
		{
			return match_star_non_greedy_dot(pat + 2, text);
		}
		return match_star_non_greedy(pat + 2, text, ch);
	}
	
	if(*pat == '*')
	{
		if(ch == '.')
		{
			return match_star_greedy_dot(pat + 1, text);
		}
		return match_star_greedy(pat + 1, text, ch);
	}
	
	if(*pat == '+' && pat[1] == '?')
	{
		if(*text != '\0' && *text == ch)
		{
			if(ch == '.')
			{
				return match_star_non_greedy_dot(pat + 2, text);
			}
			return match_star_non_greedy(pat + 2, text, ch);
		}
	}
	
	if(*pat == '+')
	{
		if(*text != '\0' && *text == ch)
		{
			if(ch == '.')
			{
				return match_star_greedy_dot(pat + 1, text);
			}
			return match_star_greedy(pat + 1, text, ch);
		}
	}
	
	if(*pat == '?')
	{
		if(ch == '.')
		{
			return match_question_dot(pat + 1, text);
		}
		return match_question(pat + 1, text, ch);
	}

	if(ch == *text)
	{
		return match_here(pat, text + 1);
	}
	return NULL;	
}

static char *match_character_class(char *pat, char *text, char *class)
{
	/*
		@brief	:	match the pattern with the character classes specified between the square backets from 
					the text.
								
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
		@param	:	class	-	array of characters to be matched
	*/
	
	if(*pat == '*' && pat[1] == '?')
	{
		return match_star_non_greedy_class(pat + 2, text, class);
	}
	
	if(*pat == '*')
	{
		return match_star_greedy_class(pat + 1, text, class);
	}
	
	if(*pat == '+' && pat[1] == '?')
	{
		if(*text != '\0' && is_present(*text++, class))
		{
			return match_star_non_greedy_class(pat + 2, text, class);
		}
	}
	
	if(*pat == '+')
	{
		if(*text != '\0' && is_present(*text++, class))
		{
			return match_star_greedy_class(pat + 1, text, class);
		}
	}
	
	if(*pat == '?')
	{
		return match_question_class(pat + 1, text, class);
	}

	if(is_present(*text, class))
	{
		return match_here(pat, text + 1);
	}
	return NULL;	
}


static char *match_escape_character_classes(char *pat, char *text)
{
	/*
		@brief	:	match the pattern with the character classes specified between the square backets 
					from the text.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	int is_range;
	char *iter;
	char class[256];
	int class_iter;
	
	is_range = 0;
	iter = pat;
	class_iter = 0;
	
	while(*pat != ']')
	{
		++pat;
	}
	++pat;
	
	++iter;
	while(*iter != ']')
	{
		if(*iter == '-')
		{
			is_range = 1;
		}
		else
		{
			if(is_range)
			{
				if(class_iter == 0)
				{
					class[class_iter++] = '-';
				}
				
				else if(class[class_iter -1] <= *iter)
				{
					for(int i = class[class_iter -1] + 1; i <= *iter; ++i)
					{
						class[class_iter++] = i;
					}
				}
				else if(class[class_iter -1] > *iter)
				{
					return NULL;
				}
				
				is_range = 1 - is_range;
			}
			else
			{
				class[class_iter++] = *iter;
			}
		}
		++iter;
	}
	if(is_range)
	{
		class[class_iter++] = '-';
	}
	class[class_iter++] = '\0'; 
	return match_character_class(pat, text, class);
}


static char *match_star_non_greedy(char *pat, char *text, char ch)
{
	/*
		@brief	:	matches zero or more occurence of given character and comsumes as few
					characters as possible. 
					
		@param	:	ch		-	character to be matched
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	char *t;
	
	do
	{
		if((t = match_here(pat, text)) != NULL)
		{
			return t;	
		}
	} while(*text != '\0' && (*text++ == ch || ch == '.'));
	return NULL;
}


static char *match_star_non_greedy_dot(char *pat, char *text)
{
	/*
		@brief	:	matches zero or more occurence of '.' character and comsumes as few
					characters as possible. 

		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	char *t;
	
	do
	{
		if((t = match_here(pat, text)) != NULL)
		{
			return t;	
		}
	} while(*text != '\0' && (*text++ == '.'));
	return NULL;
}


static char *match_star_non_greedy_class(char *pat, char *text, char *class)
{
	/*
		@brief	:	matches zero or more occurence of characters in class and comsumes as few
					characters as possible.

		@param	:	pat		-	regular expression
		@param	:	text	-	text input
		@param	:	class	-	array of characters to be matched
	*/
	
	char *t;
	
	do
	{
		if((t = match_here(pat, text)) != NULL)
		{
			return t;	
		}
	} while(*text != '\0' && (is_present(*text++, class)));
	return NULL;
}


static char *match_star_greedy(char *pat, char *text, char ch)
{
	/*
		@brief	:	matches zero or more occurence of given character and comsumes as many
					characters as possible. 
					
		@param	:	ch		-	character to be matched
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	char *t;
	char *current;
	
	t = NULL;
	do
	{	
		if((current = match_here(pat, text)) != NULL)
		{
			t = current;
		}
	} while(*text != '\0' && (*text++ == ch || ch == '.'));
	return t;
}


static char *match_star_greedy_dot(char *pat, char *text)
{
	/*
		@brief	:	matches zero or more occurence of '.' character and comsumes as many
					characters as possible.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	char *t;
	char *current;
	
	t = NULL;
	do
	{	
		if((current = match_here(pat, text)) != NULL)
		{
			t = current;
		}
	} while(*text != '\0' && (*text++ == '.'));
	return t;
}


static char *match_star_greedy_class(char *pat, char *text, char *class)
{
	/*
		@brief	:	matches zero or more occurence of characters in class and comsumes as many
					characters as possible.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
		@param	:	class	-	array of characters to be matched
	*/
	
	char *t;
	char *current;
	
	t = NULL;
	do
	{	
		if((current = match_here(pat, text)) != NULL)
		{
			t = current;
		}
	} while(*text != '\0' && (is_present(*text++, class)));
	return t;
}


static char *match_question(char *pat, char *text, char ch)
{
	/*
		@brief	:	matches one or zero occurence of given character.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
		@param	:	ch		-	character to be matched
	*/
	
	char *t;
	char *current;
	int count;
	
	t = NULL;
	count = 1;
	
	do
	{
		if((current = match_here(pat, text)) != NULL)
		{
			t = current;
		}
	}
	while(*text != '\0' && (*text++ == ch || ch == '.') && count-- > 0);
	return t;
}


static char *match_question_dot(char *pat, char *text)
{
	/*
		@brief	:	matches one or zero occurence of '.' character.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
	*/
	
	char *t;
	char *current;
	int count;
	
	t = NULL;
	count = 1;
	
	do
	{
		if((current = match_here(pat, text)) != NULL)
		{
			t = current;
		}
	}
	while(*text != '\0' && (*text++ == '.') && count-- > 0);
	return t;
}


static char *match_question_class(char *pat, char *text, char *class)
{
	/*
		@brief	:	matches one or zero occurences of characters in class array.
					
		@param	:	pat		-	regular expression
		@param	:	text	-	text input
		@param	:	class	-	array of characters to be matched
	*/
	
	char *t;
	char *current;
	int count;
	
	t = NULL;
	count = 1;
	
	do
	{
		if((current = match_here(pat, text)) != NULL)
		{
			t = current;
		}
	}
	while(*text != '\0' && (is_present(*text++, class)) && count-- > 0);
	return t;
}


static int is_present(char ch, char *class)
{
	/*
		@brief	:	checks if ch is present in array of characters or not.
					
		@param	:	ch		-	character to be checked
		@param	:	class	-	character array
	*/
	
	int i;
		
	i = 0;

	while(class[i] != '\0' && class[i] != ch)
	{
		++i;
	}
	return (class[i] == '\0' ? 0 : 1);
}

