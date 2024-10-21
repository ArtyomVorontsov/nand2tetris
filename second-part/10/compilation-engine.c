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
	if(x) fseek(sfp, -(x + 1), SEEK_CUR);
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
	if(isKeywordTag(token)){
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
	if(isSymbolTag(token)){
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
	if(isSymbolTag(token)){
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
	if(isKeywordTag(token)){
		// ('static' | 'field')
		// printf("%s\n", token);
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(isKeywordTag(token) || isIdentifierTag(token)){
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
	while(true){
		int ptrMoved = 0;
		// (',' varName)*
		token = getToken(sfp);
		if(isSymbolTag(token)){
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
	if(isSymbolTag(token)){
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
	// ('constructor' | 'function' | 'method') (void | 'type') subroutineName '(' parameterList ')' subroutineBody
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(isKeywordTag(token)){
		// ('constructor' | 'function' | 'method')
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	token = getToken(sfp);
	if(isKeywordTag(token) || isIdentifierTag(token)){
		// (void | 'type')
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

	token = getToken(sfp);
	if(isSymbolTag(token)){
		// '('
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	// parameterList
	compileParameterList(sfp, dfp);

	if(isSymbolTag(token)){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(compileSubroutineBody(sfp, dfp) == false){
		return false;
	}

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
	if(isKeywordTag(token)){
		// varName
		ptrMoved += moveFPToNextToken(sfp);

		token = getToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	while(true){
		token = getToken(sfp);
		if(isSymbolTag(token)){
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
	if(isSymbolTag(token)){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	while(compileVarDec(sfp, dfp));

	compileStatements(sfp, dfp);

	token = getToken(sfp);
	if(isSymbolTag(token)){
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
	return true;

}

bool compileVarDec(FILE *sfp, FILE *dfp){
	printMsg("compileVarDec");
	// ('var' type varName (',' type varName)*)?
	char *token;
	int ptrMoved = 0;

	token = getToken(sfp);
	if(isKeywordTag(token)){
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

	while(true){
		token = getToken(sfp);
		if(isSymbolTag(token)){
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

	return true;
}

bool compileStatements(FILE *sfp, FILE *dfp){
	printMsg("compileStatements");
	// statement*

	while(true){
		// letStatement | ifStatement | whileStatement | doStatement | returnStatement
		if(
				compileIf(sfp, dfp) == false && 
				compileWhile(sfp, dfp) == false && 
				compileDo(sfp, dfp) == false && 
				compileReturn(sfp, dfp) == false 
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

	if(isKeywordTag(token)){
		// 'let'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(isIdentifierTag(token)){
		// 'varName'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(isSymbolTag(token)){
		// '[' ?
		ptrMoved += moveFPToNextToken(sfp);

		if(compileExpression(sfp, dfp) == false){
			// expression ? 
			return false;
		}

		if(isSymbolTag(token)){
			// ']' ?
			ptrMoved += moveFPToNextToken(sfp);
		} else {
			moveFPBack(sfp, ptrMoved);
		}
	} else {
		moveFPBack(sfp, ptrMoved);
	}

	
	if(isSymbolTag(token)){
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

	if(isSymbolTag(token)){
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
	
	if(isKeywordTag(token)){
		// 'if'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(isSymbolTag(token)){
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

	if(isSymbolTag(token)){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(isSymbolTag(token)){
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

	if(isSymbolTag(token)){
		// '}'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(isKeywordTag(token)){
		// 'else' ?
		ptrMoved += moveFPToNextToken(sfp);

		if(isSymbolTag(token)){
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

		if(isSymbolTag(token)){
			// '}' ?
			ptrMoved += moveFPToNextToken(sfp);
		} else {
			moveFPBack(sfp, ptrMoved);
			return false;
		}
	} else {
		moveFPBack(sfp, ptrMoved);
	}



	return true;
}

bool compileWhile(FILE *sfp, FILE *dfp){
	printMsg("compileWhile");
	// 'while' '(' expression ')' '{' statements '}'
	char *token;
	int ptrMoved = 0;

	if(isKeywordTag(token)){
		// 'while'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(isSymbolTag(token)){
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

	if(isSymbolTag(token)){
		// ')'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(isSymbolTag(token)){
		// '{'
		ptrMoved += moveFPToNextToken(sfp);
	} else {
		moveFPBack(sfp, ptrMoved);
		return false;
	}

	if(compileStatements(sfp, dfp) == false){
		return false;
	}

	if(isSymbolTag(token)){
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

	if(isKeywordTag(token)){
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

	if(isSymbolTag(token)){
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

	if(isKeywordTag(token)){
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

	if(isSymbolTag(token)){
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

