#include "./jack-tokinizer.h"

const int MAX_LINE_SIZE = 1000;
const int MAX_TOKEN_SIZE = 100;

void tokenize(FILE *sfp, FILE *dfp){
	char line[MAX_LINE_SIZE];
	int i = 0;
	char *token;
	int tokenLength = 0;
	enum TOKEN_TYPE typeOfToken; 

	fprintf(dfp,"<tokens>\n");
	while(1){
		if(hasMoreTokens(sfp)) {
			token = advance(&sfp);
			typeOfToken = tokenType(token);

			switch(typeOfToken){
				case UNKNOWN:
					fprintf(dfp, "<unknown> %s </uknown>\n", token);
					break;
				case KEYWORD:
					fprintf(dfp, "<keyword> %s </keyword>\n", token);
					break;
				case SYMBOL:
					fprintf(dfp, "<symbol> %s </symbol>\n", token);
					break;
				case IDENTIFIER:
					fprintf(dfp, "<identifier> %s </identifier>\n", token);
					break;
				case INT_CONST:
					fprintf(dfp, "<integerConstant> %s </integerConstant>\n", token);
					break;
				case STRING_CONST:
					fprintf(dfp, "<stringConstant> %s </stringConstant>\n", token);
					break;
				case BLANK:
					break;
				default:
					fprintf(dfp, "<unknown> %s </uknown>\n", token);
					break;
			}

			i++;
		} else {
			break;
		}

	}
	fprintf(dfp,"</tokens>\n");
}

bool hasMoreTokens(FILE *sfp){
	return feof(sfp) == false;
}

char * advance(FILE **sfp){
	char c; 
	int i = 0, j = 0, initialPtrPosition = ftell(*sfp);
	char *token;

	skipComments(sfp);

	while(true){
		c = getc(*sfp);
		i++;
		if(
			(c == ' ') || 
			(c == '\n') || 
			(c == '\t') || 
			(c == '\0') || 
			(c == -1) || 
			(c == '.') || 
			(c == '(') || 
			(c == ')') ||
			(c == ';') 
		){
			break;
		}
	}
	fseek(*sfp, -i, SEEK_CUR);

	token = malloc(sizeof(char) * (i + 1));

	while(true){
		c = getc(*sfp);
		if(
			(c == ' ') || 
			(c == '\n') || 
			(c == '\t') || 
			(c == '\0') || 
			(c == -1) || 
			(c == '.') || 
			(c == '(') || 
			(c == ')') ||
			(c == ';') 
		){
			if(j == 0){
				*(token + j) = c;
				j++;
			} else {
				fseek(*sfp, -1, SEEK_CUR);
			}
			break;
		}
		*(token + j) = c;
		j++;
	}

	token[j] = '\0';

	if(initialPtrPosition == ftell(*sfp)){
		// EOF
		getc(*sfp);
	}

	return token;
}


enum TOKEN_TYPE tokenType(char *token){
	if(isKeyword(token)){
		return KEYWORD;
	} 
	else if(isBlank(token)){
		return BLANK;
	}
	else if(isSymbol(token)){
		return SYMBOL;
	}
	else if(isVal(token)){
		return INT_CONST;
	} 
	else if(isStringVal(token)){
		return STRING_CONST;
	}
	else if(isIdentifier(token)){
		return IDENTIFIER;
	}

	return UNKNOWN;
}

bool isKeyword(char *token){
	return strcmp(token, "class") == 0 ||
	strcmp(token, "method") == 0 ||
	strcmp(token, "function") == 0 ||
	strcmp(token, "constructor") == 0 ||
	strcmp(token, "int") == 0 ||
	strcmp(token, "boolean") == 0 ||
	strcmp(token, "field") == 0 ||
	strcmp(token, "let") == 0 ||
	strcmp(token, "do") == 0 ||
	strcmp(token, "if") == 0 ||
	strcmp(token, "else") == 0 ||
	strcmp(token, "while") == 0 ||
	strcmp(token, "return") == 0 ||
	strcmp(token, "true") == 0 ||
	strcmp(token, "false") == 0 ||
	strcmp(token, "null") == 0 ||
	strcmp(token, "this") == 0 ||
	strcmp(token, "void") == 0 ||
	strcmp(token, "var") == 0;
}

bool isSymbol(char *token){
	return strcmp(token, "{") == 0 ||
	strcmp(token, "}") == 0 ||
	strcmp(token, "(") == 0 ||
	strcmp(token, ")") == 0 ||
	strcmp(token, "[") == 0 ||
	strcmp(token, "]") == 0 ||
	strcmp(token, ".") == 0 ||
	strcmp(token, ",") == 0 ||
	strcmp(token, ";") == 0 ||
	strcmp(token, "+") == 0 ||
	strcmp(token, "-") == 0 ||
	strcmp(token, "*") == 0 ||
	strcmp(token, "/") == 0 ||
	strcmp(token, "&") == 0 ||
	strcmp(token, "|") == 0 ||
	strcmp(token, "<") == 0 ||
	strcmp(token, ">") == 0 ||
	strcmp(token, "=") == 0 ||
	strcmp(token, "~") == 0;
}

bool isIdentifier(char *token){
	return true;
}

bool isVal(char *token){
	int i = 0;

	while(isdigit(token[i++])) i++;
	return strlen(token) == i;
}

bool isStringVal(char *token){
	int tokenLength = strlen(token);

	if((token[0] == '"') && (token[tokenLength - 1] == '"')){
		return true;
	}

	return false;
}

bool isBlank(char *token){
	char c = token[0];

	return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\0') || (c == -1);
}

void skipComments(FILE **sfp){
	// Skip comments
	while(getc(*sfp) == '/'){
		fseek(*sfp, -1, SEEK_CUR);

		if(getc(*sfp) == '/'){
			if(getc(*sfp) == '/'){
				while(getc(*sfp) != '\n');
			}
			else {
				fseek(*sfp, -2, SEEK_CUR);
			}
		} else {
			fseek(*sfp, -1, SEEK_CUR);
		}
		
		if(getc(*sfp) == '/'){
			if(getc(*sfp) == '*'){
				while(true){
					if(getc(*sfp) == '*' ){
						if(getc(*sfp) == '/'){
							break;
						} 
						else {
							fseek(*sfp, -2, SEEK_CUR);
						}
					} 
					else {
						fseek(*sfp, -1, SEEK_CUR);
					}
					getc(*sfp);
				}
			}
			else {
				fseek(*sfp, -2, SEEK_CUR);
			}
		} 
		else {
			fseek(*sfp, -1, SEEK_CUR);
		}
	}
	fseek(*sfp, -1, SEEK_CUR);
}

