#include "./compilation-engine.h"

#define DEV_MODE true

void printMsg(char *msg){
	if(DEV_MODE){
		printf("dev: %s\n", msg);
	}
}

void compilationEngine(FILE *sfp, FILE *dfp){
	printMsg("compilationEngine");
	char *token;

	token = getToken(sfp);
	moveFPToNextToken(sfp);

	int res = compileClass(sfp, dfp);
	printf("res %d", res);

}

char *getToken(FILE *sfp){
	char *token = malloc(sizeof(char) * 100);
	int i = 0;

	while(((*(token + i) = getc(sfp)) != '\n') && (feof(sfp) == false)) { i++;}
	token[i] = '\0';

	fseek(sfp, -(i + 1), SEEK_CUR);
	
	return token;
}

int moveFPToNextToken(FILE *sfp){
	int i = 0;
	while((getc(sfp) != '\n') && (feof(sfp) == false)) i++;

	return i;
}

void moveFPBack(FILE *sfp, int x){
	if(x) fseek(sfp, -(x), SEEK_CUR);
}

int depth = 0;

void incrementDepth(){
	printf("inc depth %d \n", depth);
	depth++;
}

void decrementDepth(){
	depth--;
	printf("dec depth %d \n", depth);
}

int getDepth(){
	return depth;
}

void printTabs(FILE *dfp){
	for(int i = 0; i < getDepth(); i++){
		fprintf(dfp, "\t");	
	}
}

void printTag(char *tag, FILE *dfp){
	printTabs(dfp);
	fprintf(dfp, "%s\n", tag);
}

bool compileClass(FILE *sfp, FILE *dfp){
	printMsg("compileClass");
	// 'class' className '{' classVarDec* subroutineDec* '}'
	bool isClass;
	char *classKeyword;
	char *className;
	char *openFigureBracket;
	char *closeFigureBracket;
	int ptrMoved = 0;
	char *token;
	
	printTag("<class>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(strcmp(token, "<keyword> class </keyword>") == 0){
		// 'class' 
		classKeyword = token;
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	
	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// className 
		className = token;
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}


	token = getToken(sfp);
	if(strcmp(token, "<symbol> { </symbol>") == 0){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	while(true){
		// classVarDec* 
		if(compileClassVarDec(sfp, dfp) == false){
			break;
		}
	} 
	
	while(true){
		// subroutineDec* 
		if(compileSubroutine(sfp, dfp) == false){
			break;
		}
	} 

	token = getToken(sfp);
	if(strcmp(token, "<symbol> } </symbol>") == 0){
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</class>", dfp);
	return true;
};

bool compileClassVarDec(FILE *sfp, FILE *dfp){
	incrementDepth();
	printMsg("compileClassVarDec");
	// ('static' | 'field') type varName(',' varName)* ';'
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(
		strcmp(token, "<keyword> static </keyword>") == 0 || 
		strcmp(token, "<keyword> field </keyword>") == 0
	){
		// ('static' | 'field')
		// printf("%s\n", token);
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(
		strcmp(token, "<keyword> int </keyword>") == 0||
		strcmp(token, "<keyword> boolean </keyword>") == 0 ||
		strcmp(token, "<keyword> char </keyword>") == 0||
		isIdentifierTag(token)
	){
		// type
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// varName
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	int i = 0;
	// (',' varName)*
	while(true){
		int ptrMoved = 0;
		token = getToken(sfp);
		if(strcmp(token, "<symbol> , </symbol>") == 0){
			// ','
			ptrMoved += moveFPToNextToken(sfp);

			token = getToken(sfp);
			if(isIdentifierTag(token)){
				// varName
				ptrMoved += moveFPToNextToken(sfp);
			} else {
				moveFPBack(sfp, ptrMoved);
				break;
			}
		} else {
			moveFPBack(sfp, ptrMoved);
			break;
		}

		i++;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ; </symbol>") == 0){
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	return true;
}

bool compileSubroutine(FILE *sfp, FILE *dfp){
	printMsg("compileSubroutine");
	// ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
	char *token;
	int ptrMoved = 0;

	printTag("<subroutineDec>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(
		strcmp(token, "<keyword> constructor </keyword>") == 0||
		strcmp(token, "<keyword> function </keyword>") == 0 ||
		strcmp(token, "<keyword> method </keyword>") == 0
	){
		// ('constructor' | 'function' | 'method')
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<keyword> void </keyword>") == 0 || isIdentifierTag(token)){
		// ('void' | type)
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// subroutineName
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}
	printf("token %s\n", token);

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ( </symbol>") == 0){
		// '('
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	// parameterList
	compileParameterList(sfp, dfp);
	printf("token %s\n", token);

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ) </symbol>") == 0){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}
	printf("token %s\n", token);

	if(compileSubroutineBody(sfp, dfp) == false){
		decrementDepth();
		return false;
	}
	printf("token %s\n", token);

	decrementDepth();
	printTag("</subroutineDec>", dfp);
	return true;
}

bool compileParameterList(FILE *sfp, FILE *dfp){
	printMsg("compileParameterList");
	// ((type varName) (',' type varName)*)?
	char *token;
	int i = 0;
	int ptrMoved = 0;

	printTag("<parameterList>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(isKeywordTag(token)){
		// type
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}
	

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// varName
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	while(true){
		token = getToken(sfp);
		if(strcmp(token, "<symbol> , </symbol>") == 0){
			// ','
			ptrMoved += moveFPToNextToken(sfp);
			printTag(token, dfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			break;
		}

		token = getToken(sfp);
		if(isKeywordTag(token)){
			// type
			ptrMoved += moveFPToNextToken(sfp);
			printTag(token, dfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			decrementDepth();
			return false;
		}
		

		token = getToken(sfp);
		if(isKeywordTag(token)){
			// varName
			ptrMoved += moveFPToNextToken(sfp);
			printTag(token, dfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			decrementDepth();
			return false;
		}

		i++;
	}

	decrementDepth();
	printTag("</parameterList>", dfp);
	return true;
}

bool compileSubroutineBody(FILE *sfp, FILE *dfp){
	printMsg("compileSubroutineBody");
	// '{' varDec* statements '}'
	char *token;
	int ptrMoved;

	printTag("<subroutineBody>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(strcmp(token, "<symbol> { </symbol>") == 0){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	while(compileVarDec(sfp, dfp));

	compileStatements(sfp, dfp);

	token = getToken(sfp);
	if(strcmp(token, "<symbol> } </symbol>") == 0){
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</subroutineBody>", dfp);
	return true;
}

bool compileSubroutineCall(FILE *sfp, FILE *dfp){
	// subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'	
	char *token;
	int ptrMoved = 0;

	printTag("<subroutineCall>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// 'subroutineName'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}
	
	token = getToken(sfp);
	if(strcmp(token, "<symbol> . </symbol>") == 0){
		// '.'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// 'subroutineName'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}
	
	token = getToken(sfp);
	if(strcmp(token, "<symbol> ( </symbol>") == 0){
		// '('
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ) </symbol>") == 0){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
	}

	decrementDepth();
	printTag("</subroutineCall>", dfp);
	return true;

}

bool compileVarDec(FILE *sfp, FILE *dfp){
	printMsg("compileVarDec");
	// 'var' type varName (',' varName)* ';'
	char *token;
	int ptrMoved = 0;

	printTag("<varDec>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(strcmp(token, "<keyword> var </keyword>") == 0){
		// 'var'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// type
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// varName
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}
	// (',' varName)*
	while(true){
		int ptrMoved = 0;
		token = getToken(sfp);
		if(strcmp(token, "<symbol> , </symbol>") == 0){
			// ','
			ptrMoved += moveFPToNextToken(sfp);
			printTag(token, dfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			break;
		}

		token = getToken(sfp);
		if(isKeywordTag(token)){
			// type
			ptrMoved += moveFPToNextToken(sfp);
			printTag(token, dfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			decrementDepth();
			return false;
		}
		

		token = getToken(sfp);
		if(isKeywordTag(token)){
			// varName
			ptrMoved += moveFPToNextToken(sfp);
			printTag(token, dfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			return false;
		}
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ; </symbol>") == 0){
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</varDec>", dfp);

	return true;
}

bool compileStatements(FILE *sfp, FILE *dfp){
	printMsg("compileStatements");
	// statement*
	printTag("</statements>", dfp);
	incrementDepth();

	while(true){
		// letStatement | ifStatement | whileStatement | doStatement | returnStatement
		if(
				compileLet(sfp, dfp) == false && 
				compileIf(sfp, dfp) == false && 
				compileWhile(sfp, dfp) == false && 
				compileDo(sfp, dfp) == false && 
				compileReturn(sfp, dfp) == false
		){
			break;
		}
	}


	decrementDepth();
	printTag("</statements>", dfp);
	return true;
}

bool compileLet(FILE *sfp, FILE *dfp) {
	printMsg("compileLet");
	// 'let' varName ('[' expression ']')? '=' expression ';'
	char *token;
	int ptrMoved = 0;

	printTag("<letStatement>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(strcmp(token, "<keyword> let </keyword>") == 0){
		// 'let'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// 'varName'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	{
		int ptrMoved = 0;
		token = getToken(sfp);
		if(strcmp(token, "<symbol> [ </symbol>") == 0){
			// '[' ?
			ptrMoved += moveFPToNextToken(sfp);

			if(compileExpression(sfp, dfp) == false){
				// expression ? 
				decrementDepth();
				return false;
			}

			token = getToken(sfp);
			if(strcmp(token, "<symbol> ] </symbol>") == 0){
				// ']' ?
				ptrMoved += moveFPToNextToken(sfp);
				printTag(token, dfp);
			} else {
				moveFPBack(sfp, ptrMoved);
			}
		} else {
			moveFPBack(sfp, ptrMoved);
		}
	}

	
	token = getToken(sfp);
	if(strcmp(token, "<symbol> = </symbol>") == 0){
		// '='
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	if(compileExpression(sfp, dfp) == false){
		// expression 
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ; </symbol>") == 0){
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("<letStatement>", dfp);
	return true;
}

bool compileIf(FILE *sfp, FILE *dfp){
	printMsg("compileIf");
	// 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
	char *token;
	int ptrMoved = 0;
	
	printTag("</ifStatement>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(strcmp(token, "<keyword> if </keyword>") == 0){
		// 'if'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ( </symbol>") == 0){
		// '('
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	if(compileExpression(sfp, dfp) == false){
		// expression 
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ) </symbol>") == 0){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;

	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> { </symbol>") == 0){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	if(compileStatements(sfp, dfp) == false){
		// expression 
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> } </symbol>") == 0){
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	{
		int ptrMoved = 0;
		if(strcmp(token, "<keyword> else </keyword>") == 0){
			// 'else' ?
			ptrMoved += moveFPToNextToken(sfp);

			token = getToken(sfp);
			if(strcmp(token, "<symbol> { </symbol>") == 0){
				// '{' ?
				ptrMoved += moveFPToNextToken(sfp);
				printTag(token, dfp);
			} else {
				moveFPBack(sfp, ptrMoved);
				decrementDepth();
				return false;
			}

			if(compileStatements(sfp, dfp) == false){
				// expression ?
				decrementDepth();
				return false;
			}

			token = getToken(sfp);
			if(strcmp(token, "<symbol> } </symbol>") == 0){
				// '}' ?
				ptrMoved += moveFPToNextToken(sfp);
				printTag(token, dfp);
			} else {
				moveFPBack(sfp, ptrMoved);
				decrementDepth();
				return false;
			}
		} else {
			moveFPBack(sfp, ptrMoved);
		}
	}


	decrementDepth();
	printTag("</ifStatement>", dfp);
	return true;
}

bool compileWhile(FILE *sfp, FILE *dfp){
	printMsg("compileWhile");
	// 'while' '(' expression ')' '{' statements '}'
	char *token;
	int ptrMoved = 0;

	incrementDepth();
	printTag("<whileStatement>", dfp);


	token = getToken(sfp);
	if(strcmp(token, "<keyword> while </keyword>") == 0){
		// 'while'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ( </symbol>") == 0){
		// '('
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	if(compileExpression(sfp, dfp) == false){
		// expression
		decrementDepth();
		return false;
	}
	
	token = getToken(sfp);
	if(strcmp(token, "<symbol> ) </symbol>") == 0){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> { </symbol>") == 0){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	if(compileStatements(sfp, dfp) == false){
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> } </symbol>") == 0){
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</whileStatement>", dfp);
	return true;
}

bool compileDo(FILE *sfp, FILE *dfp){
	printMsg("compileDo");
	// 'do' subroutineCall ';'
	char *token;
	int ptrMoved = 0;

	printTag("</doStatement>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(strcmp(token, "<keyword> do </keyword>") == 0){
		// 'do'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	// subroutineCall
	if(compileSubroutineCall(sfp, dfp) == false){
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ; </symbol>") == 0){
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</doStatement>", dfp);

	return true;
}

bool compileReturn(FILE *sfp, FILE *dfp){
	printMsg("compileReturn");
	// 'return' expression? ';'
	char *token;
	int ptrMoved = 0;

	printTag("<returnStatement>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(strcmp(token, "<keyword> return </keyword>") == 0){
		// 'return'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	// expression ?
	if(compileExpression(sfp, dfp) == false){
		decrementDepth();
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ; </symbol>") == 0){
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		decrementDepth();
		return false;
	}

	decrementDepth();
	printTag("</returnStatement>", dfp);

	return true;
}

bool compileExpression(FILE *sfp, FILE *dfp){
	printMsg("compileExpression");
	char *token;
	int ptrMoved = 0;

	printTag("<expression>", dfp);
	incrementDepth();

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		ptrMoved += moveFPToNextToken(sfp);
		printTag(token, dfp);
	} else {
		moveFPBack(sfp, ptrMoved);
	}
	decrementDepth();
	printTag("<expression>", dfp);

	return true;
}

bool compileTerm(FILE *sfp, FILE *dfp){
	printTag("<term>", dfp);
	incrementDepth();
	printMsg("compileTerm");
	decrementDepth();
	printTag("</term>", dfp);

	return true;
}

bool isKeywordTag(char *token){
	char tokencpy[100];
	strcpy(tokencpy, token);
	
	tokencpy[9] = '\0';
	return strcmp(tokencpy, "<keyword>") == 0;
}

bool isSymbolTag(char *token){
	char tokencpy[100];
	strcpy(tokencpy, token);
	tokencpy[8] = '\0';
	return strcmp(tokencpy, "<symbol>") == 0;
}

bool isIdentifierTag(char *token){
	char tokencpy[100];
	strcpy(tokencpy, token);
	tokencpy[12] = '\0';
	return strcmp(tokencpy, "<identifier>") == 0;
}

bool isIntConstTag(char *token){
	char tokencpy[100];
	strcpy(tokencpy, token);
	tokencpy[17] = '\0';
	return strcmp(tokencpy, "<integerConstant>") == 0;
}

bool isStringConstTag(char *token){
	char tokencpy[100];
	strcpy(tokencpy, token);
	tokencpy[16] = '\0';
	return strcmp(tokencpy, "<stringConstant>") == 0;
}

