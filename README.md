# regex-engine
## Introduction
A regular expression (shortened as regex or regexp) is a sequence of characters that specifies a search pattern. Regular expressions are extremely useful in 
extracting information from any text by searching for one or more matches of a specific search pattern 
(i.e. a specific sequence of ASCII or unicode characters).Usually such patterns are used by string-searching algorithms for
"find" or "find and replace" operations on strings, or for input validation.

It is a technique developed in theoretical computer science and formal language theory.
Fields of application range from validation to parsing/replacing strings, passing through translating data to other formats and web scraping.

## Operations supported
* greedy and non-greedy
* Support the following macros
  * []
	*	a-z
	*	A-Z
	*	0-9
	*	Individual characters and numbers
  * ^
  * $
  * .
  * ?
  * \+
  * \*
  * \d
  * \w
  * \s
  * escape characters

## Function/Interface supported
match is the only function supported for the client as an interface. all other functions are not available to the client, they are implementation functions
(internal static functions)
### match
```c
node_t *match(char *pat, char *text);
```
it returns starting and ending position of the string if the substring of text matches with the pattern else returns null.

## Usage
```
$ make
$ ./output 
```
