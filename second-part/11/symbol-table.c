#include "./symbol-table.h"
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

GList *symbolTableStackPush(GList *stack, gpointer data)
{
    printf("Symbol table depth (push): %d\n\n", g_list_length(stack) + 1);
    // TODO: move to separate stack component
    return g_list_last(g_list_append(g_list_last(stack), data)); // append adds the element to the back
}

GList *symbolTableStackPop(GList *stack, gpointer *data)
{

    GList *stackFirstElement = g_list_first(stack);
    GList *stackLastElement = g_list_last(stack);
    printf("Symbol table depth (pop): %d\n\n", g_list_length(stackFirstElement));
    if (stack != NULL)
    {
        ((struct SymbolTable *)stackLastElement->data)->print((struct SymbolTable *)stackLastElement->data);
    }

    // TODO: move to separate stack component
    if (stackLastElement != NULL)
    {
        GList *prev = stackLastElement->prev; // Get the rest of the stack
        if (prev)
            prev->next = NULL;
        return prev; // Return the rest of the stack
    }
    *data = NULL; // If stack is empty, return NULL
    return NULL;
}

struct SymbolTableRecord *registerSymbolInSymbolTableStack(char *token, enum USAGE_TYPE usageType, char *typeForSymbolTable, enum KIND kindForSymbolTable, GList *symbolTableStack)
{

    char *nameForSymbolTable = token;

    if (usageType == USAGE)
    {
        // If identifier is used in some expression that means kind and type should be already in symbol table,
        // so we need to find declared record in symbol table

        GList *symbolTablesStackPtr = g_list_last(symbolTableStack);
        int stackDepth = g_list_length(g_list_first(symbolTablesStackPtr));
        while (symbolTablesStackPtr)
        {

            struct SymbolTable *symbolTable = (struct SymbolTable *)symbolTablesStackPtr->data;

            int kindForSymbolTableFromSymbolTable = symbolTable->kindOf(symbolTable, nameForSymbolTable);
            char *typeForSymbolTableFromSymbolTable = symbolTable->typeOf(symbolTable, nameForSymbolTable);

            if (kindForSymbolTableFromSymbolTable != UNDEFINED && typeForSymbolTableFromSymbolTable)
            {
                struct SymbolTable *currentSymbolTable = g_list_last(symbolTableStack)->data;

                return currentSymbolTable->define(currentSymbolTable,
                                                  nameForSymbolTable,
                                                  typeForSymbolTableFromSymbolTable,
                                                  kindForSymbolTableFromSymbolTable,
                                                  usageType,
                                                  symbolTableStack);
                break;
            }
            symbolTablesStackPtr = symbolTablesStackPtr->prev;
            stackDepth--;

            if (stackDepth < 1)
            {
                printf("Variable %s definition not found!\n", token);
            }
        }
    }
    else if (usageType == DECLARATION)
    {
        // If identifier is declared then we write declaration to symbol table
        struct SymbolTable *symbolTable = symbolTableStackPeek(symbolTableStack);
        return symbolTable->define(symbolTable, nameForSymbolTable, typeForSymbolTable, kindForSymbolTable, usageType, symbolTableStack);
    }

    return NULL;
}

void revertSymbolInSymbolTableStack(GList *symbolTableStack, int amount)
{
    struct SymbolTable *symbolTable = g_list_last(symbolTableStack)->data;
    while (amount)
    {
        symbolTable->revert(symbolTable);
        amount--;
    }
}

gpointer symbolTableStackPeek(GList *stack)
{
    GList *stackLastElement = g_list_last(stack);
    // TODO: move to separate stack component
    if (stackLastElement != NULL)
    {
        return stackLastElement->data; // Return the data at the top of the stack
    }
    return NULL; // Return NULL if stack is empty
}

struct SymbolTable *createSymbolTable()
{
    struct SymbolTable *symbolTable = malloc(sizeof(struct SymbolTable));

    symbolTable->define = define;
    symbolTable->indexOf = indexOf;
    symbolTable->typeOf = typeOf;
    symbolTable->print = printSymbolTable;
    symbolTable->kindOf = kindOf;
    symbolTable->reset = reset;
    symbolTable->varCount = varCount;
    symbolTable->revert = revert;
    symbolTable->list = NULL;

    return symbolTable;
}

void reset(struct SymbolTable *this)
{
    g_list_free_full(this->list, (GDestroyNotify)g_free);
}

struct SymbolTableRecord *define(struct SymbolTable *this, char *name, char *type, enum KIND kind, enum USAGE_TYPE usage, GList *SYMBOL_TABLES_STACK)
{
    struct SymbolTableRecord *symbolTableRecord = malloc(sizeof(struct SymbolTableRecord));

    symbolTableRecord->name = g_strdup(name);
    symbolTableRecord->type = g_strdup(type);
    symbolTableRecord->kind = kind;
    symbolTableRecord->usage = usage;

    if (usage == DECLARATION)
    {
        symbolTableRecord->index = getIndexByKind(this, symbolTableRecord->kind);
    }
    else if (usage == USAGE)
    {
        symbolTableRecord->index = this->indexOf(SYMBOL_TABLES_STACK, name);
    }

    this->list = g_list_append(this->list, symbolTableRecord);

    return symbolTableRecord;
}

struct SymbolTable *revert(struct SymbolTable *this)
{
    GList *last = g_list_last(this->list);
    if (last->prev)
    {
        last->prev->next = NULL;
    }

    return this;
};

int varCount(struct SymbolTable *this, enum KIND kind)
{
    GList *list = g_list_first(this->list);

    int count = 0;
    while (list)
    {
        if (((struct SymbolTableRecord *)list->data)->kind == kind)
        {
            count++;
        }
        list = list->next;
    }

    return count;
}

enum KIND kindOf(struct SymbolTable *this, char *name)
{
    GList *list = g_list_first(this->list);

    struct SymbolTableRecord *foundListItem = NULL;
    while (list)
    {
        if (strcmp(((struct SymbolTableRecord *)list->data)->name, name) == 0)
        {
            foundListItem = list->data;
            break;
        }
        list = list->next;
    }

    if (foundListItem == NULL)
    {
        return UNDEFINED;
    }
    return foundListItem->kind;
}

char *typeOf(struct SymbolTable *this, char *name)
{
    GList *list = g_list_first(this->list);

    struct SymbolTableRecord *foundListItem = NULL;
    while (list)
    {
        if (strcmp(((struct SymbolTableRecord *)list->data)->name, name) == 0)
        {
            foundListItem = list->data;
            break;
        }
        list = list->next;
    }

    if (foundListItem == NULL)
    {
        return NULL;
    }

    return foundListItem->type;
}

int indexOf(GList *symbolTableStack, char *name)
{
    int index = 0;
    GList *symbolTableStackPtr = g_list_last(symbolTableStack);

    // Iterate through all symbol tables starting from the last in the stack
    while (symbolTableStackPtr)
    {
        struct SymbolTable *symbolTablePtr = symbolTableStackPtr->data;

        GList *symbolTableRecordsListPtr = g_list_first(symbolTablePtr->list);

        // Iterate through all symbol table records starting from the first
        while (symbolTableRecordsListPtr)
        {
            struct SymbolTableRecord *symbolTableRecord = symbolTableRecordsListPtr->data;
            if (strcmp(symbolTableRecord->name, name) == 0)
            {
                index = symbolTableRecord->index;
                break;
            }
            symbolTableRecordsListPtr = symbolTableRecordsListPtr->next;
        }

        symbolTableStackPtr = symbolTableStackPtr->prev;
    }

    return index;
}

int getIndexByKind(struct SymbolTable *this, enum KIND kind)
{
    GList *list = g_list_first(this->list);

    int index = 0;
    while (list)
    {

        struct SymbolTableRecord *record = list->data;
        if (record->kind == kind)
        {
            index++;
        }

        list = list->next;
    }

    return index;
}

void printSymbolTable(struct SymbolTable *this)
{
    if (this)
    {
        printf("Symbol table print:\n");

        if (!this->list)
        {
            printf("Table is empty.\n\n");
            return;
        }

        GList *list = this->list;
        printf("============\n");
        while (list)
        {
            struct SymbolTableRecord *data = ((struct SymbolTableRecord *)list->data);

            char *kind[] = {
                "STATIC",
                "FIELD",
                "ARG",
                "VAR",
                "CLASS",
                "SUBROUTINE"};

            char *usageType[] = {
                "DECLARATION",
                "USAGE"};

            printf("name: %s \ntype: %s \nkind: %s\nindex: %d\nusage type: %s\n============\n", data->name, data->type, kind[data->kind], data->index, usageType[data->usage]);
            list = list->next;
        }
        printf("\n");
    }
}

void checkSymbolTableValidity(struct SymbolTable *st, int index)
{
    if (st->kindOf != kindOf)
    {
        printf("kindOf pointer is lost");
        exit(1);
    }
    if (st->typeOf != typeOf)
    {
        printf("typeOf pointer is lost");
        exit(1);
    }
    if (st->varCount != varCount)
    {
        printf("varCount pointer is lost");
        exit(1);
    }
    printf("Symbol table with index %d and length %d is valid\n", index, g_list_length(st->list));
    printf("And variables: ");
    GList *stl = st->list;

    while (stl)
    {
        struct SymbolTableRecord *str = str = stl->data;
        printf("%s, ", str->name);
        stl = stl->next;
    }
    printf("\n");
}

void checkSymbolTableStackValidity(GList *sts)
{
    printf("Check symbol table stack\n");
    printf("Depth: %d\n", g_list_length(sts));

    GList *stsp = sts;
    int i = 0;
    while (stsp)
    {
        struct SymbolTable *st = stsp->data;
        checkSymbolTableValidity(st, i);
        i++;
        stsp = stsp->prev;
    }
    printf("\n");
}

int printSymbolTableEntry(struct SymbolTableRecord *symbolTableRecord, FILE *dfp)
{
    int destFilePtrMoved = 0;

    if (symbolTableRecord == NULL)
    {
        return destFilePtrMoved;
    }

    char *symbolTableRecordOpenTag = "<symbolTableRecord>";
    char *symbolTableRecordCloseTag = "</symbolTableRecord>";
    char name[100];
    char type[100];
    char kind[100];
    char index[100];
    char usage[100];

    sprintf(name, "<name> %s </name>", symbolTableRecord->name);
    sprintf(type, "<type> %s </type>", symbolTableRecord->type);
    sprintf(kind, "<kind> %d </kind>", symbolTableRecord->kind);
    sprintf(index, "<index> %d </index>", symbolTableRecord->index);
    sprintf(usage, "<usage> %d </usage>", symbolTableRecord->usage);

    destFilePtrMoved += printTag(symbolTableRecordOpenTag, dfp);
    incrementDepth();
    destFilePtrMoved += printTag(name, dfp);
    destFilePtrMoved += printTag(type, dfp);
    destFilePtrMoved += printTag(kind, dfp);
    destFilePtrMoved += printTag(index, dfp);
    destFilePtrMoved += printTag(usage, dfp);
    decrementDepth();
    destFilePtrMoved += printTag(symbolTableRecordCloseTag, dfp);

    return destFilePtrMoved;
}