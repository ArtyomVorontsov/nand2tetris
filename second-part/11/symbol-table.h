#include <glib.h>

enum KIND
{
    STATIC,
    FIELD,
    ARG,
    VAR
};

struct SymbolTable
{
    GList *list;
    void (*reset)(struct SymbolTable *this);
    void (*define)(struct SymbolTable *this, char *name, char *type, enum KIND kind);
    int (*varCount)(struct SymbolTable *this, enum KIND kind);
    int (*kindOf)(struct SymbolTable *this, char *name);
    char *(*typeOf)(struct SymbolTable *this, char *name);
    int (*indexOf)(struct SymbolTable *this, char *name);
    void (*print)(struct SymbolTable *this);
};

struct SymbolTableRecord
{
    char *name;
    char *type;
    enum KIND kind;
};

struct SymbolTable *createSymbolTable();
void reset(struct SymbolTable *this);
void define(struct SymbolTable *this, char *name, char *type, enum KIND kind);
int varCount(struct SymbolTable *this, enum KIND kind);
int kindOf(struct SymbolTable *this, char *name);
char *typeOf(struct SymbolTable *this, char *name);
int indexOf(struct SymbolTable *this, char *name);
void print(struct SymbolTable *this);
GList *symbolTableStackPush(GList *stack, gpointer data);
GList *symbolTableStackPop(GList *stack, gpointer *data);
gpointer symbolTableStackPeek(GList *stack);

