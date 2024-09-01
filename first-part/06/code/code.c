#include "./code.h"

char **code(struct CInstruction **instructions){
	char **generatedCode = (char **) malloc(sizeof(char *) * 1000);
	int i = 0, j = 0, line = 0;
	struct CInstruction *inst;
	struct SymbolRecord **symbolTable = malloc(sizeof(struct SymbolRecord*) * 100);
	*symbolTable = NULL;

	int lastSymbolValue = 16;

	
	while((inst = *(instructions + i)) != NULL){
		if(inst->label){
			addSymbol(symbolTable, inst->label, line);
		}
		if(inst->label == NULL){
			line++;
		}

		i++;
	}

	i = 0;
	line = 0;
	while((inst = *(instructions + i)) != NULL){
		if(inst->symbol && (findSymbol(symbolTable, inst->symbol) == NULL)){
			if(strcmp(inst->symbol, "SCREEN") == 0){
				addSymbol(symbolTable, inst->symbol, 16384);
			}
			else if(strcmp(inst->symbol, "KEYBOARD") == 0){
				addSymbol(symbolTable, inst->symbol, 24576);
			}
			else if(strcmp(inst->symbol, "R0") == 0){
				addSymbol(symbolTable, inst->symbol, 0);
			} 
			else if(strcmp(inst->symbol, "R1") == 0){
				addSymbol(symbolTable, inst->symbol, 1);
			} 
			else if(strcmp(inst->symbol, "R2") == 0){
				addSymbol(symbolTable, inst->symbol, 2);
			} 
			else if(strcmp(inst->symbol, "R3") == 0){
				addSymbol(symbolTable, inst->symbol, 3);
			} 
			else if(strcmp(inst->symbol, "R4") == 0){
				addSymbol(symbolTable, inst->symbol, 4);
			} 
			else if(strcmp(inst->symbol, "R5") == 0){
				addSymbol(symbolTable, inst->symbol, 5);
			} 
			else if(strcmp(inst->symbol, "R6") == 0){
				addSymbol(symbolTable, inst->symbol, 6);
			} 
			else if(strcmp(inst->symbol, "R7") == 0){
				addSymbol(symbolTable, inst->symbol, 7);
			} 
			else if(strcmp(inst->symbol, "R8") == 0){
				addSymbol(symbolTable, inst->symbol, 8);
			} 
			else if(strcmp(inst->symbol, "R9") == 0){
				addSymbol(symbolTable, inst->symbol, 9);
			} 
			else if(strcmp(inst->symbol, "R10") == 0){
				addSymbol(symbolTable, inst->symbol, 10);
			} 
			else if(strcmp(inst->symbol, "R11") == 0){
				addSymbol(symbolTable, inst->symbol, 11);
			} 
			else if(strcmp(inst->symbol, "R12") == 0){
				addSymbol(symbolTable, inst->symbol, 12);
			} 
			else if(strcmp(inst->symbol, "R13") == 0){
				addSymbol(symbolTable, inst->symbol, 13);
			} 
			else if(strcmp(inst->symbol, "R14") == 0){
				addSymbol(symbolTable, inst->symbol, 14);
			} 
			else if(strcmp(inst->symbol, "R15") == 0){
				addSymbol(symbolTable, inst->symbol, 15);
			} 
			else {
				addSymbol(symbolTable, inst->symbol, lastSymbolValue);
				lastSymbolValue = lastSymbolValue + 1;
			}
		} 

		if(inst->label == NULL){
			line++;
		}

		i++;
	} 

	//print symbol table
	for(int i = 0; *(symbolTable + i) != NULL; i++){
		printf("symbol: %s\nvalue: %d\n\n", 
			(*(symbolTable + i))->symbol, 
			(*(symbolTable + i))->value
		);
	} 
	


	i = 0;
	while((inst = *(instructions + i)) != NULL){
		if(inst->dest || inst->comp || inst->jump){
			char * binary = malloc(sizeof(char) * 17);

			binary[0] = '1';
			binary[1] = '1';
			binary[2] = '1';

			char * a = binary + 3;
			char * c0 = binary + 4;
			char * c1 = binary + 5;
			char * c2 = binary + 6;
			char * c3 = binary + 7;
			char * c4 = binary + 8;
			char * c5 = binary + 9;

			char * d0 = binary + 10;
			char * d1 = binary + 11;
			char * d2 = binary + 12;

			char * j0 = binary + 13;
			char * j1 = binary + 14;
			char * j2 = binary + 15;

			// comp
			if(inst->comp == NULL){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '0';
				*c3 = '0';
				*c4 = '0';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "0") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '0';
				*c2 = '1';
				*c3 = '0';
				*c4 = '1';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "1") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '1';
				*c3 = '1';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "-1") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '1';
				*c3 = '0';
				*c4 = '1';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "D") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '1';
				*c3 = '1';
				*c4 = '0';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "A") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '0';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "M") == 0){
				*a = '1'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '0';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "!D") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '1';
				*c3 = '1';
				*c4 = '0';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "!A") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '0';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "!M") == 0){
				*a = '1'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '0';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "-D") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '1';
				*c3 = '1';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "-A") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "-M") == 0){
				*a = '1'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "D+1") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '1';
				*c2 = '1';
				*c3 = '1';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "A+1") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '1';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "M+1") == 0){
				*a = '1'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '1';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "D-1") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '1';
				*c3 = '1';
				*c4 = '1';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "A-1") == 0){
				*a = '0'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '1';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "M-1") == 0){
				*a = '1'; // a

				*c0 = '1';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '1';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "D+A") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '0';
				*c3 = '0';
				*c4 = '1';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "D+M") == 0){
				*a = '1'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '0';
				*c3 = '0';
				*c4 = '1';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "D-A") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "D-M") == 0){
				*a = '1'; // a

				*c0 = '0';
				*c1 = '1';
				*c2 = '0';
				*c3 = '0';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "A-D") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '0';
				*c3 = '1';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "M-D") == 0){
				*a = '1'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '0';
				*c3 = '1';
				*c4 = '1';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "D&A") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '0';
				*c3 = '0';
				*c4 = '0';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "D&M") == 0){
				*a = '1'; // a

				*c0 = '0';
				*c1 = '0';
				*c2 = '0';
				*c3 = '0';
				*c4 = '0';
				*c5 = '0';
			}
			else if(strcmp(inst->comp, "D|A") == 0){
				*a = '0'; // a

				*c0 = '0';
				*c1 = '1';
				*c2 = '0';
				*c3 = '1';
				*c4 = '0';
				*c5 = '1';
			}
			else if(strcmp(inst->comp, "D|M") == 0){
				*a = '1'; // a

				*c0 = '0';
				*c1 = '1';
				*c2 = '0';
				*c3 = '1';
				*c4 = '0';
				*c5 = '1';
			}

			// dest
			if(inst->dest == NULL){
				*d0 = '0';
				*d1 = '0';
				*d2 = '0';
			}
			else if(strcmp(inst->dest, "M") == 0){
				*d0 = '0';
				*d1 = '0';
				*d2 = '1';
			}
			else if(strcmp(inst->dest, "D") == 0){
				*d0 = '0';
				*d1 = '1';
				*d2 = '0';
			}
			else if((strcmp(inst->dest, "DM") == 0) || (strcmp(inst->dest, "MD") == 0)){
				*d0 = '0';
				*d1 = '1';
				*d2 = '1';
			}
			else if(strcmp(inst->dest, "A") == 0){
				*d0 = '1';
				*d1 = '0';
				*d2 = '0';
			}
			else if(strcmp(inst->dest, "AM") == 0){
				*d0 = '1';
				*d1 = '0';
				*d2 = '1';
			}
			else if(strcmp(inst->dest, "AD") == 0){
				*d0 = '1';
				*d1 = '1';
				*d2 = '0';
			}
			else if(strcmp(inst->dest, "ADM") == 0){
				*d0 = '1';
				*d1 = '1';
				*d2 = '1';
			}


			// jump 
			if(inst->jump == NULL){
				*j0 = '0';
				*j1 = '0';
				*j2 = '0';
			} 
			else if(strcmp(inst->jump, "JGT") == 0){
				*j0 = '0';
				*j1 = '0';
				*j2 = '1';
			} 
			else if(strcmp(inst->jump, "JEQ") == 0){
				*j0 = '0';
				*j1 = '1';
				*j2 = '0';
			} 
			else if(strcmp(inst->jump, "JGE") == 0){
				*j0 = '0';
				*j1 = '1';
				*j2 = '1';
			} 
			else if(strcmp(inst->jump, "JLT") == 0){
				*j0 = '1';
				*j1 = '0';
				*j2 = '0';
			} 
			else if(strcmp(inst->jump, "JNE") == 0){
				*j0 = '1';
				*j1 = '0';
				*j2 = '1';
			} 
			else if(strcmp(inst->jump, "JLE") == 0){
				*j0 = '1';
				*j1 = '1';
				*j2 = '0';
			} 
			else if(strcmp(inst->jump, "JMP") == 0){
				*j0 = '1';
				*j1 = '1';
				*j2 = '1';
			} 

			binary[16] = '\n';
			generatedCode[j] = binary;
			j++;
		}
		else if (inst->address || inst->symbol){
			char *binary = malloc(sizeof(char) * 17);
			binary[0] = '0';

			if(inst->address){
				char *binString = intStrToBinStr(inst->address);

				binary[1] = *(binString + 14);
				binary[2] = *(binString + 13);
				binary[3] = *(binString + 12);
				binary[4] = *(binString + 11);
				binary[5] = *(binString + 10);
				binary[6] = *(binString + 9);
				binary[7] = *(binString + 8);
				binary[8] = *(binString + 7);
				binary[9] = *(binString + 6);
				binary[10] = *(binString + 5);
				binary[11] = *(binString + 4);
				binary[12] = *(binString + 3);
				binary[13] = *(binString + 2);
				binary[14] = *(binString + 1);
				binary[15] = *(binString + 0);
			}
			else if(inst->symbol){
				struct SymbolRecord *symbolRecord = findSymbol(symbolTable, inst->symbol);
				char value[100];

				sprintf(value, "%d", symbolRecord->value);
				
				char *binString = intStrToBinStr(value);

				binary[1] = *(binString + 14);
				binary[2] = *(binString + 13);
				binary[3] = *(binString + 12);
				binary[4] = *(binString + 11);
				binary[5] = *(binString + 10);
				binary[6] = *(binString + 9);
				binary[7] = *(binString + 8);
				binary[8] = *(binString + 7);
				binary[9] = *(binString + 6);
				binary[10] = *(binString + 5);
				binary[11] = *(binString + 4);
				binary[12] = *(binString + 3);
				binary[13] = *(binString + 2);
				binary[14] = *(binString + 1);
				binary[15] = *(binString + 0); 
			}

			binary[16] = '\n';
			generatedCode[j] = binary;
			j++;
		}
		i++;
	}
	generatedCode[i] = NULL;

	return generatedCode;
	
}

char *intStrToBinStr(char *s){
	int address = atoi(s);
	char *binStrAddress = int2bin(address, 16);
	

	return binStrAddress;
}


char* int2bin(unsigned integer, int n)
{
  char* binary = (char*)malloc(n+1);
  for (int i=0;i<n;i++)   
    binary[i] = (integer & (int)1<<(n-i-1)) ? '1' : '0';
  binary[n]='\0';

  char reversed[n];

  for(int i = 0; i < n - 1; i++)
	  reversed[i] = binary[n - i - 1];

  reversed[n] = '\0';

  for(int i = 0; i < n; i++)
	  binary[i] = reversed[i];

  return binary;
}


struct SymbolRecord *findSymbol(struct SymbolRecord **symbolTableP, char *symbol){
	
	while(*(symbolTableP) != NULL){
		if(((*symbolTableP)->symbol != NULL) && (strcmp((*symbolTableP)->symbol, symbol) == 0))
			break;	

		symbolTableP++;
	}

	return *symbolTableP;
}

void addSymbol(struct SymbolRecord **symbolTableP, char *label, int value){
	
	struct SymbolRecord *record = findSymbol(symbolTableP, label);

	if(record == NULL){
		int i = 0;
		struct SymbolRecord *newRecord = malloc(sizeof(struct SymbolRecord));
		while((*(symbolTableP + i) != NULL) && (i < 99)) i++;

		newRecord->symbol = label;
		newRecord->value = value;

		*(symbolTableP + i) = newRecord;
		*(symbolTableP + i + 1) = NULL;
	} else {
		record->value = value;
	}
}



