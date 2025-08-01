#include "./compilation-engine.h"
#include "./vm-writer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define DEV_MODE false
#define SYMBOL_TABLE true
GList *SYMBOL_TABLES_STACK = NULL;

void printMsg(char *msg)
{
	if (DEV_MODE)
	{
		printf("dev: %s\n", msg);
	}
}

void printMsgWithTemplate(char *template, char *msg)
{

	if (DEV_MODE)
	{
		char buffer[1000];
		strcpy(buffer, "dev: ");
		printf(strcat(buffer, template), msg);
	}
}

void compilationEngine(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compilationEngine");
	char *token;

	token = getToken(sfp);
	moveFPToNextToken(sfp);

	int res = compileClass(sfp, dfp, dfpvm);
	printf("Class successfully compiled: %s \n\n", res ? "true" : "false");
}

char *getToken(FILE *sfp)
{
	char *token = malloc(sizeof(char) * 100);
	int i = 0;

	while (((*(token + i) = getc(sfp)) != '\n') && (feof(sfp) == false))
	{
		i++;
	}
	token[i] = '\0';

	fseek(sfp, -(i + 1), SEEK_CUR);

	return token;
}

int moveFPToNextToken(FILE *sfp)
{
	int i = 0;
	while (i++, (getc(sfp) != '\n') && (feof(sfp) == false))
		;

	return i;
}

void moveFPBack(FILE *sfp, int x)
{
	int remain = x % 100;
	int i = x / 100;
	if (x)
	{
		if (x > 100)
		{
			do
			{
				fseek(sfp, -(100), SEEK_CUR);
			} while (--i);
			fseek(sfp, -(remain), SEEK_CUR);
		}
		else
		{
			fseek(sfp, -(x), SEEK_CUR);
		}
	}
}

int depth = 0;

void incrementDepth()
{
	printMsgWithTemplate("inc depth %d \n", depth);
	depth++;
}

void decrementDepth()
{
	depth--;
	printMsgWithTemplate("dec depth %d \n", depth);
}

int _scopeDepth = 0;

void openScope()
{
	if (SYMBOL_TABLE)
	{
		SYMBOL_TABLES_STACK = symbolTableStackPush(SYMBOL_TABLES_STACK, createSymbolTable());
	}
	_scopeDepth++;
}

void closeScope()
{
	if (SYMBOL_TABLE)
	{
		GList *list = malloc(sizeof(GList));
		SYMBOL_TABLES_STACK = symbolTableStackPop(SYMBOL_TABLES_STACK, (gpointer)list);
		free(list);
	}
	_scopeDepth--;
}

int getDepth()
{
	return depth;
}

int printTabs(FILE *dfp)
{
	int i = 0;

	for (i = 0; i < getDepth(); i++)
	{
		fprintf(dfp, "  ");
	}

	return i * 2;
}

int printTag(char *tag, FILE *dfp)
{
	return fprintf(dfp, "%s\n", tag);
}

bool compileClass(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileClass");
	// 'class' className '{' classVarDec* subroutineDec* '}'
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;
	char *token;

	char *nameForSymbolTable = NULL;
	char *typeForSymbolTable = NULL;
	enum KIND kindForSymbolTable = VAR;

	destFilePtrMoved += printTag("<class>", dfp);
	incrementDepth();
	openScope();

	token = getToken(sfp);
	if (strcmp(token, "<keyword> class </keyword>") == 0)
	{
		// 'class'
		typeForSymbolTable = token;
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		moveFPBack(sfp, ptrMoved);
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (isIdentifierTag(token))
	{
		// className
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, DECLARATION, typeForSymbolTable, CLASS, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		nameForSymbolTable = token;
	}
	else
	{
		moveFPBack(sfp, ptrMoved);
		moveFPBack(dfp, destFilePtrMoved);
		decrementDepth();
		closeScope();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> { </symbol>") == 0)
	{
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(sfp, ptrMoved);
		moveFPBack(dfp, destFilePtrMoved);
		decrementDepth();
		closeScope();
		return false;
	}

	while (true)
	{
		// classVarDec*
		if (compileClassVarDec(sfp, dfp, dfpvm) == false)
		{
			break;
		}
	}

	// register all class method names
	{
		int ptrMoved = 0;
		char *typeForSymbolTable;
		while (true)
		{
			ptrMoved += moveFPToNextToken(sfp);
			token = getToken(sfp);
			if (strcmp(token, "<keyword> method </keyword>") == 0 ||
				strcmp(token, "<keyword> function </keyword>") == 0)
			{
				// Handle symbol table
				ptrMoved += moveFPToNextToken(sfp); // skip type
				typeForSymbolTable = getToken(sfp);
				ptrMoved += moveFPToNextToken(sfp); // thats name what we need, method/function name ;)
				token = getToken(sfp);

				struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, DECLARATION, typeForSymbolTable, SUBROUTINE, SYMBOL_TABLES_STACK);
				destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
			}

			// rollback
			if (strcmp(token, "") == 0)
			{
				moveFPBack(sfp, ptrMoved);
				moveFPToNextToken(sfp); // skip type
				break;
			}
		}
	}

	while (true)
	{
		// subroutineDec*
		if (compileSubroutine(sfp, dfp, dfpvm) == false)
		{
			break;
		}
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> } </symbol>") == 0)
	{
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		closeScope();
		return false;
	}

	decrementDepth();
	closeScope();
	printTag("</class>", dfp);

	return true;
};

bool compileClassVarDec(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileClassVarDec");
	// ('static' | 'field') type varName(',' varName)* ';'
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	char *nameForSymbolTable = NULL;
	char *typeForSymbolTable = NULL;
	enum KIND kindForSymbolTable = VAR;
	int symbolsRegistered = 0;

	destFilePtrMoved += printTag("<classVarDec>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if (
		strcmp(token, "<keyword> static </keyword>") == 0 ||
		strcmp(token, "<keyword> field </keyword>") == 0)
	{
		// ('static' | 'field')
		if (strcmp(token, "<keyword> static </keyword>") == 0)
		{
			kindForSymbolTable = STATIC;
		}

		if (strcmp(token, "<keyword> field </keyword>") == 0)
		{
			kindForSymbolTable = FIELD;
		}
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (
		strcmp(token, "<keyword> int </keyword>") == 0 ||
		strcmp(token, "<keyword> boolean </keyword>") == 0 ||
		strcmp(token, "<keyword> char </keyword>") == 0 ||
		isIdentifierTag(token))
	{
		// type
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, USAGE, NULL, UNDEFINED, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		typeForSymbolTable = token;
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (isIdentifierTag(token))
	{
		// varName
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, DECLARATION, typeForSymbolTable, kindForSymbolTable, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		nameForSymbolTable = token;
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	int i = 0;
	// (',' varName)*
	while (true)
	{
		int ptrMoved = 0;
		int destFilePtrMoved = 0;
		int symbolsRegistered = 0;
		token = getToken(sfp);

		if (strcmp(token, "<symbol> , </symbol>") == 0)
		{
			// ','
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);

			token = getToken(sfp);
			if (isIdentifierTag(token))
			{
				// varName
				ptrMoved += moveFPToNextToken(sfp);
				destFilePtrMoved += printTag(token, dfp);

				// Handle symbol table
				struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, DECLARATION, typeForSymbolTable, kindForSymbolTable, SYMBOL_TABLES_STACK);
				destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
				nameForSymbolTable = token;
			}
			else
			{
				revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
				moveFPBack(dfp, destFilePtrMoved);
				moveFPBack(sfp, ptrMoved);
				break;
			}
		}
		else
		{
			revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
			moveFPBack(dfp, destFilePtrMoved);
			moveFPBack(sfp, ptrMoved);
			break;
		}

		i++;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ; </symbol>") == 0)
	{
		// ';'
		destFilePtrMoved += printTag(token, dfp);
		ptrMoved += moveFPToNextToken(sfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	destFilePtrMoved += printTag("</classVarDec>", dfp);
	return true;
}

bool compileSubroutine(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileSubroutine");
	// ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;
	char *nameForSymbolTable = NULL;
	char *typeForSymbolTable = NULL;
	enum KIND kindForSymbolTable = SUBROUTINE;
	char *subroutineType;

	destFilePtrMoved += printTag("<subroutineDec>", dfp);

	incrementDepth();
	openScope();

	token = getToken(sfp);
	subroutineType = token;
	if (
		strcmp(token, "<keyword> constructor </keyword>") == 0 ||
		strcmp(token, "<keyword> function </keyword>") == 0 ||
		strcmp(token, "<keyword> method </keyword>") == 0)
	{
		// ('constructor' | 'function' | 'method')
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		if (strcmp(token, "<keyword> method </keyword>") == 0)
		{
			// Handle "this" param for class method in symbol table
			struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack("<keyword> this </keyword>", DECLARATION, NULL, ARG, SYMBOL_TABLES_STACK);
			destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		}
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		closeScope();
		return false;
	}

	token = getToken(sfp);
	if (isIdentifierTag(token) || isKeywordTag(token))
	{
		// ('void' | type)
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, USAGE, NULL, UNDEFINED, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		typeForSymbolTable = token;
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		closeScope();
		return false;
	}

	token = getToken(sfp);
	if (isIdentifierTag(token))
	{
		// subroutineName
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, DECLARATION, typeForSymbolTable, kindForSymbolTable, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		nameForSymbolTable = token;
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		closeScope();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ( </symbol>") == 0)
	{
		// '('
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		closeScope();
		return false;
	}

	// parameterList
	compileParameterList(sfp, dfp, dfpvm);

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ) </symbol>") == 0)
	{
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		closeScope();
		return false;
	}

	// Generate vm code
	if (strcmp(subroutineType, "<keyword> function </keyword>") == 0)
	{
		struct SymbolTable *currentSymbolTable = getCurrentSymbolTable(SYMBOL_TABLES_STACK);
		char *currentClassName = getCurrentClass(SYMBOL_TABLES_STACK)->name;
		int currentSubroutineArgsAmount = getLastSubroutineArgsAmount(currentSymbolTable);
		char *currentSubroutineName = getLastSubroutineSymbolTableRecord(currentSymbolTable)->name;
		char label[100];

		sprintf(
			label, "%s.%s %d",
			getXMLTagValue(currentClassName),
			getXMLTagValue(currentSubroutineName),
			currentSubroutineArgsAmount);
		writeFunction(dfpvm, label);
	}

	if (compileSubroutineBody(sfp, dfp, dfpvm) == false)
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		closeScope();
		return false;
	}

	decrementDepth();
	closeScope();

	printTag("</subroutineDec>", dfp);
	return true;
}

bool compileParameterList(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileParameterList");
	// ((type varName) (',' type varName)*)?
	char *token;
	int i = 0;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;
	char *nameForSymbolTable = NULL;
	char *typeForSymbolTable = NULL;
	enum KIND kindForSymbolTable = ARG;

	destFilePtrMoved += printTag("<parameterList>", dfp);

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ) </symbol>") == 0)
	{
		// ')'
		destFilePtrMoved += printTag("</parameterList>", dfp);
		return true;
	}

	incrementDepth();

	token = getToken(sfp);
	if (isKeywordTag(token) || isIdentifierTag(token))
	{
		// type
		typeForSymbolTable = token;
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		moveFPBack(sfp, ptrMoved);
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (isIdentifierTag(token))
	{
		// varName
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, DECLARATION, typeForSymbolTable, kindForSymbolTable, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		nameForSymbolTable = token;
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(sfp, ptrMoved);
		moveFPBack(dfp, destFilePtrMoved);
		decrementDepth();
		return false;
	}

	while (true)
	{
		int ptrMoved = 0;
		int destFilePtrMoved = 0;
		int symbolsRegistered = 0;
		token = getToken(sfp);

		if (strcmp(token, "<symbol> , </symbol>") == 0)
		{
			// ','
			ptrMoved += moveFPToNextToken(sfp);
			printTag(token, dfp);
		}
		else
		{
			revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
			moveFPBack(sfp, ptrMoved);
			moveFPBack(dfp, destFilePtrMoved);
			break;
		}

		token = getToken(sfp);
		if (isKeywordTag(token) || isIdentifierTag(token))
		{
			// type
			typeForSymbolTable = token;
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);
		}
		else
		{
			revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
			moveFPBack(sfp, ptrMoved);
			moveFPBack(dfp, destFilePtrMoved);
			decrementDepth();
			return false;
		}

		token = getToken(sfp);
		if (isIdentifierTag(token))
		{
			// varName
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);

			// Handle symbol table
			symbolsRegistered++;
			struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, DECLARATION, typeForSymbolTable, kindForSymbolTable, SYMBOL_TABLES_STACK);
			destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
			nameForSymbolTable = token;
		}
		else
		{
			revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
			moveFPBack(sfp, ptrMoved);
			moveFPBack(dfp, destFilePtrMoved);
			decrementDepth();
			return false;
		}

		i++;
	}

	decrementDepth();
	printTag("</parameterList>", dfp);
	return true;
}

bool compileSubroutineBody(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileSubroutineBody");
	// '{' varDec* statements '}'
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	destFilePtrMoved += printTag("<subroutineBody>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if (strcmp(token, "<symbol> { </symbol>") == 0)
	{
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		moveFPBack(sfp, ptrMoved);
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		decrementDepth();
		return false;
	}

	while (compileVarDec(sfp, dfp, dfpvm))
		;

	compileStatements(sfp, dfp, dfpvm);

	token = getToken(sfp);
	if (strcmp(token, "<symbol> } </symbol>") == 0)
	{
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		moveFPBack(sfp, ptrMoved);
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</subroutineBody>", dfp);
	return true;
}

bool compileSubroutineCall(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	// subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	token = getToken(sfp);
	if (isIdentifierTag(token))
	{
		// 'subroutineName'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, USAGE, NULL, UNDEFINED, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(sfp, ptrMoved);
		moveFPBack(dfp, destFilePtrMoved);
		return false;
	}

	{
		// optional
		int symbolsRegistered = 0;
		token = getToken(sfp);
		if (strcmp(token, "<symbol> . </symbol>") == 0)
		{
			// '.'
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);
		}

		token = getToken(sfp);
		if (isIdentifierTag(token))
		{
			// 'subroutineName'
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);

			// Handle symbol table
			symbolsRegistered++;
			struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, USAGE, NULL, UNDEFINED, SYMBOL_TABLES_STACK);
			destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		}
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ( </symbol>") == 0)
	{
		// '('
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		token = getToken(sfp);
		return false;
	}

	compileExpressionList(sfp, dfp, dfpvm);

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ) </symbol>") == 0)
	{
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	// Generate vm code
	struct SymbolTable *currentSymbolTable = getCurrentSymbolTable(SYMBOL_TABLES_STACK);
	struct SymbolTableRecord *lastSubroutine = getLastSubroutineSymbolTableRecord(currentSymbolTable);
	struct SymbolTableRecord *lastClass = getLastClassSymbolTableRecord(currentSymbolTable);
	char label[100];

	sprintf(
		label, "%s.%s %d",
		getXMLTagValue(lastClass->name),
		getXMLTagValue(lastSubroutine->name),
		lastSubroutine->argsAmount);

	writeCall(dfpvm, label);
	writePop(dfpvm, TEMP_SEG, 0);
	writePush(dfpvm, CONSTANT_SEG, 0);

	return true;
}

bool compileVarDec(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileVarDec");
	// 'var' type varName (',' varName)* ';'
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;
	char *nameForSymbolTable = NULL;
	char *typeForSymbolTable = NULL;
	enum KIND kindForSymbolTable = VAR;

	destFilePtrMoved += printTag("<varDec>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if (strcmp(token, "<keyword> var </keyword>") == 0)
	{
		// 'var'
		kindForSymbolTable = VAR;
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (isIdentifierTag(token) || isKeywordTag(token))
	{
		// type
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, USAGE, typeForSymbolTable, kindForSymbolTable, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		typeForSymbolTable = token;
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (isIdentifierTag(token))
	{
		// varName
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, DECLARATION, typeForSymbolTable, kindForSymbolTable, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
		nameForSymbolTable = token;
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	// (',' varName)*
	while (true)
	{
		int ptrMoved = 0;
		int destFilePtrMoved = 0;
		int symbolsRegistered = 0;
		token = getToken(sfp);
		if (strcmp(token, "<symbol> , </symbol>") == 0)
		{
			// ','
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);
		}
		else
		{
			revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
			moveFPBack(dfp, destFilePtrMoved);
			moveFPBack(sfp, ptrMoved);
			break;
		}

		token = getToken(sfp);
		if (isIdentifierTag(token))
		{
			// varName
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);

			// Handle symbol table
			symbolsRegistered++;
			struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, DECLARATION, typeForSymbolTable, kindForSymbolTable, SYMBOL_TABLES_STACK);
			destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
			nameForSymbolTable = token;
		}
		else
		{
			moveFPBack(sfp, ptrMoved);
			revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
			moveFPBack(dfp, destFilePtrMoved);
			break;
		}
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ; </symbol>") == 0)
	{
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		moveFPBack(sfp, ptrMoved);
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</varDec>", dfp);

	return true;
}

bool compileStatements(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileStatements");
	// statement*
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;
	destFilePtrMoved += printTag("<statements>", dfp);
	incrementDepth();

	int i = 0;
	while (true)
	{
		// letStatement | ifStatement | whileStatement | doStatement | returnStatement
		if (
			compileLet(sfp, dfp, dfpvm) == false &&
			compileIf(sfp, dfp, dfpvm) == false &&
			compileWhile(sfp, dfp, dfpvm) == false &&
			compileDo(sfp, dfp, dfpvm) == false &&
			compileReturn(sfp, dfp, dfpvm) == false)
		{
			break;
		}
		i++;
	}

	decrementDepth();
	destFilePtrMoved += printTag("</statements>", dfp);
	if (i == 0)
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
	}
	return true;
}

bool compileLet(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileLet");
	// 'let' varName ('[' expression ']')? '=' expression ';'
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	destFilePtrMoved += printTag("<letStatement>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if (strcmp(token, "<keyword> let </keyword>") == 0)
	{
		// 'let'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (isIdentifierTag(token))
	{
		// 'varName'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		symbolsRegistered++;
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, USAGE, NULL, UNDEFINED, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	{
		int ptrMoved = 0;
		int destFilePtrMoved = 0;
		int symbolsRegistered = 0;
		token = getToken(sfp);
		if (strcmp(token, "<symbol> [ </symbol>") == 0)
		{
			// '[' ?
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);

			if (compileExpression(sfp, dfp, dfpvm) == false)
			{
				// expression ?
				revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
				moveFPBack(dfp, destFilePtrMoved);
				moveFPBack(sfp, ptrMoved);
				decrementDepth();
				return false;
			}

			token = getToken(sfp);
			if (strcmp(token, "<symbol> ] </symbol>") == 0)
			{
				// ']' ?
				ptrMoved += moveFPToNextToken(sfp);
				destFilePtrMoved += printTag(token, dfp);
			}
			else
			{
				revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
				moveFPBack(dfp, destFilePtrMoved);
				moveFPBack(sfp, ptrMoved);
			}
		}
		else
		{
			revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
			moveFPBack(dfp, destFilePtrMoved);
			moveFPBack(sfp, ptrMoved);
		}
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> = </symbol>") == 0)
	{
		// '='
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);

	if (compileExpression(sfp, dfp, dfpvm) == false)
	{
		// expression
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ; </symbol>") == 0)
	{
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</letStatement>", dfp);
	return true;
}

bool compileIf(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileIf");
	// 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	destFilePtrMoved += printTag("<ifStatement>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if (strcmp(token, "<keyword> if </keyword>") == 0)
	{
		// 'if'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ( </symbol>") == 0)
	{
		// '('
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	if (compileExpression(sfp, dfp, dfpvm) == false)
	{
		// expression
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ) </symbol>") == 0)
	{
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> { </symbol>") == 0)
	{
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	if (compileStatements(sfp, dfp, dfpvm) == false)
	{
		// expression
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> } </symbol>") == 0)
	{
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	{
		int ptrMoved = 0;
		int destFilePtrMoved = 0;
		int symbolsRegistered = 0;
		if (strcmp(token, "<keyword> else </keyword>") == 0)
		{
			// 'else' ?
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);

			token = getToken(sfp);
			if (strcmp(token, "<symbol> { </symbol>") == 0)
			{
				// '{' ?
				ptrMoved += moveFPToNextToken(sfp);
				destFilePtrMoved += printTag(token, dfp);
			}
			else
			{
				revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
				moveFPBack(dfp, destFilePtrMoved);
				moveFPBack(sfp, ptrMoved);
				decrementDepth();
				return false;
			}

			if (compileStatements(sfp, dfp, dfpvm) == false)
			{
				// expression ?
				revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
				moveFPBack(dfp, destFilePtrMoved);
				moveFPBack(sfp, ptrMoved);
				decrementDepth();
				return false;
			}

			token = getToken(sfp);
			if (strcmp(token, "<symbol> } </symbol>") == 0)
			{
				// '}' ?
				ptrMoved += moveFPToNextToken(sfp);
				destFilePtrMoved += printTag(token, dfp);
			}
			else
			{
				revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
				moveFPBack(dfp, destFilePtrMoved);
				moveFPBack(sfp, ptrMoved);
				decrementDepth();
				return false;
			}
		}
		else
		{
			moveFPBack(sfp, ptrMoved);
		}
	}

	decrementDepth();
	printTag("</ifStatement>", dfp);
	return true;
}

bool compileWhile(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileWhile");
	// 'while' '(' expression ')' '{' statements '}'
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	destFilePtrMoved += printTag("<whileStatement>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if (strcmp(token, "<keyword> while </keyword>") == 0)
	{
		// 'while'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ( </symbol>") == 0)
	{
		// '('
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	if (compileExpression(sfp, dfp, dfpvm) == false)
	{
		// expression
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ) </symbol>") == 0)
	{
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> { </symbol>") == 0)
	{
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	if (compileStatements(sfp, dfp, dfpvm) == false)
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> } </symbol>") == 0)
	{
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</whileStatement>", dfp);
	return true;
}

bool compileDo(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileDo");
	// 'do' subroutineCall ';'
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	destFilePtrMoved += printTag("<doStatement>", dfp);
	incrementDepth();

	token = getToken(sfp);

	if (strcmp(token, "<keyword> do </keyword>") == 0)
	{
		// 'do'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	// subroutineCall
	if (compileSubroutineCall(sfp, dfp, dfpvm) == false)
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ; </symbol>") == 0)
	{
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</doStatement>", dfp);

	return true;
}

bool compileReturn(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("compileReturn");
	// 'return' expression? ';'
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	destFilePtrMoved += printTag("<returnStatement>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if (strcmp(token, "<keyword> return </keyword>") == 0)
	{
		// 'return'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	// expression ?
	compileExpression(sfp, dfp, dfpvm);

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ; </symbol>") == 0)
	{
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
		destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}
	writeReturn(dfpvm);

	decrementDepth();
	printTag("</returnStatement>", dfp);

	return true;
}

bool compileExpression(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	// term (op term)*
	printMsg("compileExpression");
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	destFilePtrMoved += printTag("<expression>", dfp);
	incrementDepth();

	if (compileTerm(sfp, dfp, dfpvm) == false)
	{
		moveFPBack(dfp, ptrMoved);
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMoved);
		decrementDepth();
		return false;
	}

	while (true)
	{
		char op;

		if (compileOp(sfp, dfp, dfpvm, &op) && compileTerm(sfp, dfp, dfpvm))
		{
			switch (op)
			{
			case '+':
				writeArithmetic(dfpvm, ADD);
				break;

			case '*':
				writeCall(dfpvm, "Math.multiply 2  ");
				break;

			default:
				break;
			}
			continue;
		}
		break;
	}

	decrementDepth();
	printTag("</expression>", dfp);

	return true;
}

bool compileExpressionList(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	// (expression (',' expression)* )?
	printMsg("compileExpressionList");
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	destFilePtrMoved += printTag("<expressionList>", dfp);

	token = getToken(sfp);
	if (strcmp(token, "<symbol> ) </symbol>") == 0)
	{
		// ')'
		destFilePtrMoved += printTag("</expressionList>", dfp);
		return true;
	}
	incrementDepth();

	compileExpression(sfp, dfp, dfpvm);

	while (true)
	{
		token = getToken(sfp);
		if (strcmp(token, "<symbol> , </symbol>") == 0)
		{
			ptrMoved += moveFPToNextToken(sfp);
			destFilePtrMoved += printTag(token, dfp);
		}
		else
		{
			break;
		}

		compileExpression(sfp, dfp, dfpvm);
	}

	decrementDepth();
	printTag("</expressionList>", dfp);

	return true;
}

bool compileTerm(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	// integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
	printMsg("compileTerm");
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	int destFilePtrMovedForTermTag = printTag("<term>", dfp);
	incrementDepth();

	if (token = getToken(sfp), ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isIntConstTag, &ptrMoved, &destFilePtrMoved))
	{
		// integerConstant
		// Generate vm code
		writePush(dfpvm, CONSTANT_SEG, atoi(getXMLTagValue(token)));
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isStringConstTag, &ptrMoved, &destFilePtrMoved))
	{
		// stringConstant
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileKeywordConstant(sfp, dfp, dfpvm))
	{
		// keywordConstant
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileSubroutineCall(sfp, dfp, dfpvm))
	{
		// subroutineCall
	}
	else if (
		ptrMoved = 0, destFilePtrMoved = 0,
		compileTag(sfp, dfp, dfpvm, isBracketOpenTag, &ptrMoved, &destFilePtrMoved) &&
			compileExpression(sfp, dfp, dfpvm) &&
			compileTag(sfp, dfp, dfpvm, isBracketCloseTag, &ptrMoved, &destFilePtrMoved))
	{
		// '(' expression ')'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileUnaryOp(sfp, dfp, dfpvm) && compileTerm(sfp, dfp, dfpvm))
	{
		// unaryOp term
	}
	else if (
		ptrMoved = 0, destFilePtrMoved = 0,
		indexingExpression(sfp, dfp, dfpvm))
	{
		// varName '[' expression ']'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0,
			 compileIdentifierTag(sfp, dfp, dfpvm, USAGE, NULL, UNDEFINED, &ptrMoved, &destFilePtrMoved) && ++symbolsRegistered)
	{
		// varName
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		moveFPBack(dfp, destFilePtrMovedForTermTag);
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</term>", dfp);

	return true;
}

bool indexingExpression(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	printMsg("indexingExpression");
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	if (compileIdentifierTag(sfp, dfp, dfpvm, USAGE, NULL, UNDEFINED, &ptrMoved, &destFilePtrMoved))
	{
		++symbolsRegistered;
	}
	else
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		return false;
	}

	if (compileTag(sfp, dfp, dfpvm, isSquareBracketOpenTag, &ptrMoved, &destFilePtrMoved) == false)
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		return false;
	}

	if (compileExpression(sfp, dfp, dfpvm) == false)
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		return false;
	}

	if (compileTag(sfp, dfp, dfpvm, isSquareBracketCloseTag, &ptrMoved, &destFilePtrMoved) == false)
	{
		revertSymbolInSymbolTableStack(SYMBOL_TABLES_STACK, symbolsRegistered);
		return false;
	}

	return true;
}

bool compileUnaryOp(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	// '-' | '~'
	printMsg("compileUnaryOp");
	int ptrMoved = 0;
	int destFilePtrMoved = 0;

	if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isMinusSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '-'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isTildaSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '~'
	}
	else
	{
		moveFPBack(sfp, ptrMoved);
		moveFPBack(dfp, destFilePtrMoved);
		return false;
	}

	return true;
}

bool compileOp(FILE *sfp, FILE *dfp, FILE *dfpvm, char *op)
{
	printMsg("compileOp");
	// '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;
	int symbolsRegistered = 0;

	if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isPlusSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '+'
		*op = '+';
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isMinusSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '-'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isMultiplySignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '*'
		*op = '*';
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isDivideSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '/'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isAmpersandSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '&'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isVerticalBarSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '|'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isLessThanSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '<'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isGreaterThanSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '>'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isEqualSignTag, &ptrMoved, &destFilePtrMoved))
	{
		// '='
	}
	else
	{
		return false;
	}

	return true;
}

bool compileKeywordConstant(FILE *sfp, FILE *dfp, FILE *dfpvm)
{
	// 'true' | 'false' | 'null' | 'this'
	char *token;
	int ptrMoved = 0;
	int destFilePtrMoved = 0;

	if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isTrueKeywordConstant, &ptrMoved, &destFilePtrMoved))
	{
		// 'true'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isFalseKeywordConstant, &ptrMoved, &destFilePtrMoved))
	{
		// 'false'
	}
	else if (ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isNullKeywordConstant, &ptrMoved, &destFilePtrMoved))
	{
		// 'null'
	}
	else if (token = getToken(sfp), ptrMoved = 0, destFilePtrMoved = 0, compileTag(sfp, dfp, dfpvm, isThisKeywordConstant, &ptrMoved, &destFilePtrMoved))
	{

		// 'this'
		// Handle symbol table
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, USAGE, UNDEFINED, ARG, SYMBOL_TABLES_STACK);
		destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
	}
	else
	{
		moveFPBack(dfp, destFilePtrMoved);
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	return true;
}

bool isKeywordTag(char *token)
{
	char tokencpy[100];
	strcpy(tokencpy, token);

	tokencpy[9] = '\0';
	return strcmp(tokencpy, "<keyword>") == 0;
}

bool isSymbolTag(char *token)
{
	char tokencpy[100];
	strcpy(tokencpy, token);
	tokencpy[8] = '\0';
	return strcmp(tokencpy, "<symbol>") == 0;
}

bool isIdentifierTag(char *token)
{
	char tokencpy[100];
	strcpy(tokencpy, token);
	tokencpy[12] = '\0';
	return strcmp(tokencpy, "<identifier>") == 0;
}

bool isIntConstTag(char *token)
{
	char tokencpy[100];
	strcpy(tokencpy, token);
	tokencpy[17] = '\0';
	return strcmp(tokencpy, "<integerConstant>") == 0;
}

bool isStringConstTag(char *token)
{
	char tokencpy[100];
	strcpy(tokencpy, token);
	tokencpy[16] = '\0';
	return strcmp(tokencpy, "<stringConstant>") == 0;
}

bool compileTag(FILE *sfp, FILE *dfp, FILE *dfpvm, bool (*compare)(char *token), int *ptrMoved, int *destFilePtrMoved)
{
	char *token;

	token = getToken(sfp);
	if (compare(token))
	{
		*ptrMoved += moveFPToNextToken(sfp);
		*destFilePtrMoved += printTag(token, dfp);
	}
	else
	{
		moveFPBack(dfp, *destFilePtrMoved);
		moveFPBack(sfp, *ptrMoved);
		return false;
	}

	return true;
}

bool compileIdentifierTag(FILE *sfp, FILE *dfp, FILE *dfpvm, enum USAGE_TYPE usageType, char *typeForSymbolTable, enum KIND kindForSymbolTable, int *ptrMoved, int *destFilePtrMoved)
{
	char *token;
	token = getToken(sfp);
	if (isIdentifierTag(token))
	{

		*ptrMoved += moveFPToNextToken(sfp);
		*destFilePtrMoved += printTag(token, dfp);

		// Handle symbol table
		struct SymbolTableRecord *symbolTableRecord = registerSymbolInSymbolTableStack(token, usageType, typeForSymbolTable, kindForSymbolTable, SYMBOL_TABLES_STACK);
		*destFilePtrMoved += printSymbolTableEntry(symbolTableRecord, dfp);
	}
	else
	{
		moveFPBack(dfp, *destFilePtrMoved);
		moveFPBack(sfp, *ptrMoved);
		return false;
	}

	return true;
}

bool isSquareBracketOpenTag(char *token)
{
	return strcmp(token, "<symbol> [ </symbol>") == 0;
}

bool isSquareBracketCloseTag(char *token)
{
	return strcmp(token, "<symbol> ] </symbol>") == 0;
}

bool isBracketOpenTag(char *token)
{
	return strcmp(token, "<symbol> ( </symbol>") == 0;
}

bool isBracketCloseTag(char *token)
{
	return strcmp(token, "<symbol> ) </symbol>") == 0;
}

bool isPlusSignTag(char *token)
{
	return strcmp(token, "<symbol> + </symbol>") == 0;
}

bool isMinusSignTag(char *token)
{
	return strcmp(token, "<symbol> - </symbol>") == 0;
}

bool isMultiplySignTag(char *token)
{
	return strcmp(token, "<symbol> * </symbol>") == 0;
}

bool isDivideSignTag(char *token)
{
	return strcmp(token, "<symbol> / </symbol>") == 0;
}

bool isAmpersandSignTag(char *token)
{
	return strcmp(token, "<symbol> &amp; </symbol>") == 0;
}

bool isVerticalBarSignTag(char *token)
{
	return strcmp(token, "<symbol> | </symbol>") == 0;
}

bool isLessThanSignTag(char *token)
{
	return strcmp(token, "<symbol> &lt; </symbol>") == 0;
}

bool isGreaterThanSignTag(char *token)
{
	return strcmp(token, "<symbol> &gt; </symbol>") == 0;
}

bool isEqualSignTag(char *token)
{
	return strcmp(token, "<symbol> = </symbol>") == 0;
}

bool isTildaSignTag(char *token)
{
	return strcmp(token, "<symbol> ~ </symbol>") == 0;
}

bool isTrueKeywordConstant(char *token)
{
	return strcmp(token, "<keyword> true </keyword>") == 0;
}

bool isFalseKeywordConstant(char *token)
{
	return strcmp(token, "<keyword> false </keyword>") == 0;
}

bool isNullKeywordConstant(char *token)
{
	return strcmp(token, "<keyword> null </keyword>") == 0;
}

bool isThisKeywordConstant(char *token)
{
	return strcmp(token, "<keyword> this </keyword>") == 0;
}

char *getXMLTagValue(char *xmlExpression)
{
	char *p = xmlExpression;
	char value[100];
	char *allocatedValue;
	bool firstTagPassed = false, secondTagPassed = false;
	int valueLength;

	int i = 0;
	while (true)
	{

		p = ++p;

		if ('>' == *p && (firstTagPassed == false))
		{
			firstTagPassed = true;
			p = p + 2; // skip "> "
		}

		if ('<' == *p && (secondTagPassed == false))
		{
			secondTagPassed = true;
			value[i - 1] = '\0';
			break;
		}

		if (firstTagPassed)
		{
			value[i] = *p;
			i++;
		}
	}

	valueLength = strlen(value);

	allocatedValue = malloc((valueLength + 1) * sizeof(char));

	strcpy(allocatedValue, value);
	return allocatedValue;
}