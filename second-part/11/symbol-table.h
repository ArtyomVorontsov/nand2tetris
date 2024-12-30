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
    enum KIND (*kindOf)(struct SymbolTable *this, char *name);
    char *(*typeOf)(struct SymbolTable *this, char *name);
    int (*indexOf)(struct SymbolTable *this, char *name);
};

struct SymbolTableRecord
{
    char *name;
    char *type;
    enum KIND kind;
};

void createSymbolTable();
void reset(struct SymbolTable *this);
void define(struct SymbolTable *this, char *name, char *type, enum KIND kind);
int varCount(struct SymbolTable *this, enum KIND kind);
enum KIND kindOf(struct SymbolTable *this, char *name);
char *typeOf(struct SymbolTable *this, char *name);
int indexOf(struct SymbolTable *this, char *name);
