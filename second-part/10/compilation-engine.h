#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void compilationEngine(FILE *sfp, FILE *dfp);
char *getToken(FILE *sfp);
int moveFPToNextToken(FILE *sfp);
void moveFPBack(FILE *sfp, int x);
void incrementDepth();
void decrementDepth();
int getDepth();
bool isKeywordTag(char *token);
bool isSymbolTag(char *token);
bool isIdentifierTag(char *token);
bool isIntConstTag(char *token);
bool isStringConstTag(char *token);

bool compileClass(FILE *sfp, FILE *dfp);
bool compileClassVarDec(FILE *sfp, FILE *dfp);
bool compileSubroutine(FILE *sfp, FILE *dfp);
bool compileParameterList(FILE *sfp, FILE *dfp);
bool compileSubroutineBody(FILE *sfp, FILE *dfp);
bool compileSubroutineCall(FILE *sfp, FILE *dfp);
bool compileVarDec(FILE *sfp, FILE *dfp);
bool compileStatements(FILE *sfp, FILE *dfp);
bool compileLet(FILE *sfp, FILE *dfp);
bool compileIf(FILE *sfp, FILE *dfp);
bool compileWhile(FILE *sfp, FILE *dfp);
bool compileDo(FILE *sfp, FILE *dfp);
bool compileReturn(FILE *sfp, FILE *dfp);
bool compileExpression(FILE *sfp, FILE *dfp);
bool compileTerm(FILE *sfp, FILE *dfp);
void incrementDepth();
void decrementDepth();
int getDepth();
void printTabs(FILE *sfp);
void printTag(char *tag, FILE *dfp);

