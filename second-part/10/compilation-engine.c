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

	token = getToken(sfp);
	if(strcmp(token, "<keyword> class </keyword>") == 0){
		// 'class' 
		classKeyword = token;
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	
	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// className 
		className = token;
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}


	token = getToken(sfp);
	if(strcmp(token, "<symbol> { </symbol>") == 0){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	/*
	while(true){
		// classVarDec* 
		if(compileClassVarDec(sfp, dfp) == false){
			break;
		}
	} */
	
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
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	return true;
};

bool compileClassVarDec(FILE *sfp, FILE *dfp){
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
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// varName
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
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
		return false;
	}

	return true;
}

bool compileSubroutine(FILE *sfp, FILE *dfp){
	printMsg("compileSubroutine");
	// ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(
		strcmp(token, "<keyword> constructor </keyword>") == 0||
		strcmp(token, "<keyword> function </keyword>") == 0 ||
		strcmp(token, "<keyword> method </keyword>") == 0
	){
		// ('constructor' | 'function' | 'method')
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<keyword> void </keyword>") == 0 || isIdentifierTag(token)){
		// ('void' | type)
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// subroutineName
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}
	printf("token %s\n", token);

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ( </symbol>") == 0){
		// '('
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	// parameterList
	compileParameterList(sfp, dfp);
	printf("token %s\n", token);

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ) </symbol>") == 0){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}
	printf("token %s\n", token);

	if(compileSubroutineBody(sfp, dfp) == false){
		return false;
	}
	printf("token %s\n", token);

	return true;
}

bool compileParameterList(FILE *sfp, FILE *dfp){
	printMsg("compileParameterList");
	// ((type varName) (',' type varName)*)?
	char *token;
	int i = 0;
	int ptrMoved = 0;


	token = getToken(sfp);
	if(isKeywordTag(token)){
		// type
		ptrMoved += moveFPToNextToken(sfp);

		token = getToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}
	

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// varName
		ptrMoved += moveFPToNextToken(sfp);

		token = getToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	while(true){
		token = getToken(sfp);
		if(strcmp(token, "<symbol> , </symbol>") == 0){
			// ','
			ptrMoved += moveFPToNextToken(sfp);

		} else {
			moveFPBack(sfp, ptrMoved);
			break;
		}

		token = getToken(sfp);
		if(isKeywordTag(token)){
			// type
			ptrMoved += moveFPToNextToken(sfp);

			token = getToken(sfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			return false;
		}
		

		token = getToken(sfp);
		if(isKeywordTag(token)){
			// varName
			ptrMoved += moveFPToNextToken(sfp);

			token = getToken(sfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			return false;
		}

		i++;
	}

	return true;
}

bool compileSubroutineBody(FILE *sfp, FILE *dfp){
	printMsg("compileSubroutineBody");
	// '{' varDec* statements '}'
	char *token;
	int ptrMoved;

	token = getToken(sfp);
	if(strcmp(token, "<symbol> { </symbol>") == 0){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	while(compileVarDec(sfp, dfp));
	token = getToken(sfp);
	printf("token: %s\n", token);

	compileStatements(sfp, dfp);
	token = getToken(sfp);
	printf("token: %s\n", token);

	token = getToken(sfp);
	if(strcmp(token, "<symbol> } </symbol>") == 0){
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}
	return true;
}

bool compileSubroutineCall(FILE *sfp, FILE *dfp){
	printMsg("compileSubroutineCall");
	// subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'	
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	printf("compileSubroutineCall %s\n", token);
	if(isIdentifierTag(token)){
		// 'subroutineName'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}
	
	token = getToken(sfp);
	printf("compileSubroutineCall %s\n", token);
	if(strcmp(token, "<symbol> . </symbol>") == 0){
		// '.'
		ptrMoved += moveFPToNextToken(sfp);

	} else {
		moveFPBack(sfp, ptrMoved);
	}

	token = getToken(sfp);
	printf("compileSubroutineCall %s\n", token);
	if(isIdentifierTag(token)){
		// 'subroutineName'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}
	
	token = getToken(sfp);
	printf("compileSubroutineCall %s\n", token);
	if(strcmp(token, "<symbol> ( </symbol>") == 0){
		// '('
		ptrMoved += moveFPToNextToken(sfp);

	} else {
		moveFPBack(sfp, ptrMoved);
	}

	token = getToken(sfp);
	printf("compileSubroutineCall %s\n", token);
	if(strcmp(token, "<symbol> ) </symbol>") == 0){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);

	} else {
		moveFPBack(sfp, ptrMoved);
	}

	return true;

}

bool compileVarDec(FILE *sfp, FILE *dfp){
	printMsg("compileVarDec");
	// 'var' type varName (',' varName)* ';'
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(strcmp(token, "<keyword> var </keyword>") == 0){
		// 'var'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// type
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// varName
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}
	// (',' varName)*
	while(true){
		int ptrMoved = 0;
		token = getToken(sfp);
		if(strcmp(token, "<symbol> , </symbol>") == 0){
			// ','
			ptrMoved += moveFPToNextToken(sfp);

		} else {
			moveFPBack(sfp, ptrMoved);
			break;
		}

		token = getToken(sfp);
		if(isKeywordTag(token)){
			// type
			ptrMoved += moveFPToNextToken(sfp);

			token = getToken(sfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			return false;
		}
		

		token = getToken(sfp);
		if(isKeywordTag(token)){
			// varName
			ptrMoved += moveFPToNextToken(sfp);

			token = getToken(sfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			return false;
		}
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ; </symbol>") == 0){
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	return true;
}

bool compileStatements(FILE *sfp, FILE *dfp){
	printMsg("compileStatements");
	// statement*

	while(true){
		// letStatement | ifStatement | whileStatement | doStatement | returnStatement
		if(
				compileLet(sfp, dfp) == false && 
				(printf("a %s\n", getToken(sfp)), 1) &&
				compileIf(sfp, dfp) == false && 
				(printf("b %s\n", getToken(sfp)), 1) &&
				compileWhile(sfp, dfp) == false && 
				(printf("c %s\n", getToken(sfp)), 1) &&
				compileDo(sfp, dfp) == false && 
				(printf("d %s\n", getToken(sfp)), 1) &&
				compileReturn(sfp, dfp) == false  &&
				(printf("e %s\n", getToken(sfp)), 1) 
		){
			break;
		}
	}


	return true;
}

bool compileLet(FILE *sfp, FILE *dfp) {
	printMsg("compileLet");
	// 'let' varName ('[' expression ']')? '=' expression ';'
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(strcmp(token, "<keyword> let </keyword>") == 0){
		// 'let'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		// 'varName'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
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
				return false;
			}

			token = getToken(sfp);
			if(strcmp(token, "<symbol> ] </symbol>") == 0){
				// ']' ?
				ptrMoved += moveFPToNextToken(sfp);
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
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(compileExpression(sfp, dfp) == false){
		// expression 
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ; </symbol>") == 0){
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	return true;
}

bool compileIf(FILE *sfp, FILE *dfp){
	printMsg("compileIf");
	// 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
	char *token;
	int ptrMoved = 0;
	
	token = getToken(sfp);
	if(strcmp(token, "<keyword> if </keyword>") == 0){
		// 'if'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ( </symbol>") == 0){
		// '('
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(compileExpression(sfp, dfp) == false){
		// expression 
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ) </symbol>") == 0){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> { </symbol>") == 0){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(compileStatements(sfp, dfp) == false){
		// expression 
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> } </symbol>") == 0){
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
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
			} else {
				moveFPBack(sfp, ptrMoved);
				return false;
			}

			if(compileStatements(sfp, dfp) == false){
				// expression ?
				return false;
			}

			token = getToken(sfp);
			if(strcmp(token, "<symbol> } </symbol>") == 0){
				// '}' ?
				ptrMoved += moveFPToNextToken(sfp);
			} else {
				moveFPBack(sfp, ptrMoved);
				return false;
			}
		} else {
			moveFPBack(sfp, ptrMoved);
		}
	}



	return true;
}

bool compileWhile(FILE *sfp, FILE *dfp){
	printMsg("compileWhile");
	// 'while' '(' expression ')' '{' statements '}'
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(strcmp(token, "<keyword> while </keyword>") == 0){
		// 'while'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ( </symbol>") == 0){
		// '('
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(compileExpression(sfp, dfp) == false){
		// expression
		return false;
	}
	
	token = getToken(sfp);
	if(strcmp(token, "<symbol> ) </symbol>") == 0){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> { </symbol>") == 0){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(compileStatements(sfp, dfp) == false){
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> } </symbol>") == 0){
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	return true;
}

bool compileDo(FILE *sfp, FILE *dfp){
	printMsg("compileDo");
	// 'do' subroutineCall ';'
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(strcmp(token, "<keyword> do </keyword>") == 0){
		// 'do'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	// subroutineCall
	if(compileSubroutineCall(sfp, dfp) == false){
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ; </symbol>") == 0){
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	return true;
}

bool compileReturn(FILE *sfp, FILE *dfp){
	printMsg("compileReturn");
	// 'return' expression? ';'
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(strcmp(token, "<keyword> return </keyword>") == 0){
		// 'return'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	// expression ?
	if(compileExpression(sfp, dfp) == false){
		return false;
	}

	token = getToken(sfp);
	if(strcmp(token, "<symbol> ; </symbol>") == 0){
		// ';'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	return true;
}

bool compileExpression(FILE *sfp, FILE *dfp){
	printMsg("compileExpression");
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(isIdentifierTag(token)){
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
	}
	return true;
}

bool compileTerm(FILE *sfp, FILE *dfp){
	printMsg("compileTerm");
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

