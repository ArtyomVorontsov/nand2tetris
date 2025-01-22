#include <glib.h>
#include <stdio.h>

enum KIND
{
    STATIC,
    FIELD,
    ARG,
    VAR,
    CLASS,
    SUBROUTINE,
    UNDEFINED
};

enum USAGE_TYPE
{
    DECLARATION,
    USAGE
};

struct SymbolTable
{
    GList *list;
    void (*reset)(struct SymbolTable *this);
    struct SymbolTableRecord *(*define)(struct SymbolTable *this, char *name, char *type, enum KIND kind, enum USAGE_TYPE usage, GList *SYMBOL_TABLES_STACK);
    int (*varCount)(struct SymbolTable *this, enum KIND kind);
    enum KIND (*kindOf)(struct SymbolTable *this, char *name);
    char *(*typeOf)(struct SymbolTable *this, char *name);
    int (*indexOf)(GList *symbolTableStack, char *name);
    void (*print)(struct SymbolTable *this);
    struct SymbolTable *(*revert)(struct SymbolTable *this);
};

struct SymbolTableRecord
{
    char *name;
    char *type;
    enum KIND kind;
    int index;
    enum USAGE_TYPE usage;
};

struct SymbolTable *createSymbolTable();
void reset(struct SymbolTable *this);
struct SymbolTable *revert(struct SymbolTable *this);
struct SymbolTableRecord *define(struct SymbolTable *this, char *name, char *type, enum KIND kind, enum USAGE_TYPE usage, GList *SYMBOL_TABLES_STACK);
int varCount(struct SymbolTable *this, enum KIND kind);
enum KIND kindOf(struct SymbolTable *this, char *name);
char *typeOf(struct SymbolTable *this, char *name);
int indexOf(GList *symbolTableStack, char *name);
void printSymbolTable(struct SymbolTable *this);
GList *symbolTableStackPush(GList *stack, gpointer data);
GList *symbolTableStackPop(GList *stack, gpointer *data);
gpointer symbolTableStackPeek(GList *stack);
struct SymbolTableRecord *registerSymbolInSymbolTableStack(char *token, enum USAGE_TYPE usageType, char *typeForSymbolTable, enum KIND kindForSymbolTable, GList *symbolTableStack);
void revertSymbolInSymbolTableStack(GList *symbolTableStack, int amount);

void checkSymbolTableStackValidity(GList *sts);
void checkSymbolTableValidity(struct SymbolTable *st, int index);
int printSymbolTableEntry(struct SymbolTableRecord *symbolTableRecord, FILE *dfp);
int getIndexByKind(struct SymbolTable *this, enum KIND kind);