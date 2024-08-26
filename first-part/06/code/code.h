#include "../assembler.h"

int intToBin(int k);
char *intStrToBinStr(char *s);

// Symbol table related
struct SymbolRecord {
	char *symbol;
	int value;
};

struct SymbolRecord *findSymbol(struct SymbolRecord **symbolTableP, char *symbol);
void addSymbol(struct SymbolRecord **symbolTableP, char *label, int value);

