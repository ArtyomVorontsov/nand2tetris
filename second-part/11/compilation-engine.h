#include <stdio.h>
#include <stdbool.h>

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
bool compileExpressionList(FILE *sfp, FILE *dfp);
bool compileTerm(FILE *sfp, FILE *dfp);
bool compileUnaryOp(FILE *sfp, FILE *dfp);
bool compileOp(FILE *sfp, FILE *dfp);
bool compileKeywordConstant(FILE *sfp, FILE *dfp);
void incrementDepth();
void decrementDepth();
int getDepth();
int printTabs(FILE *sfp);
int printTag(char *tag, FILE *dfp);

bool compileTag(FILE *sfp, FILE *dfp, bool (*compare)(char *token), int *ptrMoved, int *destFilePtrMoved);
bool isSquareBracketOpenTag(char *token);
bool isBracketCloseTag(char *token);
bool isBracketOpenTag(char *token);
bool isSquareBracketCloseTag(char *token);
bool isPlusSignTag(char *token);
bool isMinusSignTag(char *token);
bool isMultiplySignTag(char *token);
bool isDivideSignTag(char *token);
bool isAmpersandSignTag(char *token);
bool isVerticalBarSignTag(char *token);
bool isLessThanSignTag(char *token);
bool isGreaterThanSignTag(char *token);
bool isEqualSignTag(char *token);
bool isTildaSignTag(char *token);
bool isTrueKeywordConstant(char *token);
bool isFalseKeywordConstant(char *token);
bool isNullKeywordConstant(char *token);
bool isThisKeywordConstant(char *token);

