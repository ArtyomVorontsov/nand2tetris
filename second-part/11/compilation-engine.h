#include <stdio.h>
#include <stdbool.h>
#include "./symbol-table.h"

void compilationEngine(FILE *sfp, FILE *dfp, FILE *dfpvm);
char *getToken(FILE *sfp);
int moveFPToNextToken(FILE *sfp);
void moveFPBack(FILE *sfp, int x);
void incrementDepth();
void decrementDepth();
int getDepth();
bool isKeywordTag(char *token);
bool isSymbolTag(char *token);
bool isIntConstTag(char *token);
bool isStringConstTag(char *token);
bool isIdentifierTag(char *token);

bool compileClass(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileClassVarDec(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileSubroutine(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileParameterList(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileSubroutineBody(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileSubroutineCall(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileVarDec(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileStatements(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileLet(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileIf(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileWhile(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileDo(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileReturn(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileExpression(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileExpressionList(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileTerm(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileUnaryOp(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileOp(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool compileKeywordConstant(FILE *sfp, FILE *dfp, FILE *dfpvm);
bool indexingExpression(FILE *sfp, FILE *dfp, FILE *dfpvm);
void incrementDepth();
void decrementDepth();
int getDepth();
int printTabs(FILE *sfp);
int printTag(char *tag, FILE *dfp);

bool compileTag(FILE *sfp, FILE *dfp, FILE *dfpvm, bool (*compare)(char *token), int *ptrMoved, int *destFilePtrMoved);
bool compileIdentifierTag(FILE *sfp, FILE *dfp, FILE *dfpvm, enum USAGE_TYPE usageType, char *typeForSymbolTable, enum KIND kindForSymbolTable, int *ptrMoved, int *destFilePtrMoved);
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
char *getXMLTagValue(char *xmlExpression);