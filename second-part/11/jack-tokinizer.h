#include <stdio.h>
#include <stdbool.h>

enum TOKEN_TYPE {
	UNKNOWN,
	KEYWORD,
	SYMBOL,
	IDENTIFIER,
	INT_CONST,
	STRING_CONST,
	BLANK
};

void tokenize(FILE *sfp, FILE *dfp);
bool hasMoreTokens(FILE *sfp);
char * advance(FILE **sfp);
enum TOKEN_TYPE tokenType(char *token);
bool isKeyword(char *token);
bool isSymbol(char *token);
bool isIdentifier(char *token);
bool isVal(char *token);
bool isStringVal(char *token);
bool isBlank(char *token);
void skipComments(FILE **sfp);

