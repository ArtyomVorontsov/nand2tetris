#include "./symbol-table.h"
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

GList *symbolTableStackPush(GList *stack, gpointer data)
{
    printf("Symbol table depth (push): %d\n", g_list_length(stack) + 1);
    // TODO: move to separate stack component
    return g_list_prepend(stack, data); // Prepend adds the element to the front
}

GList *symbolTableStackPop(GList *stack, gpointer *data)
{
    printf("\nSymbol table print:\n");
    if (stack != NULL)
        ((struct SymbolTable *)stack->data)->print((struct SymbolTable *)stack->data);
    printf("Symbol table depth (pop): %d\n", g_list_length(stack));

    // TODO: move to separate stack component
    if (stack != NULL)
    {
        *data = stack->data;       // Get the data from the top element
        GList *next = stack->next; // Get the rest of the stack
        g_list_free_1(stack);      // Free the top node
        return next;               // Return the rest of the stack
    }
    *data = NULL; // If stack is empty, return NULL
    return NULL;
}

gpointer symbolTableStackPeek(GList *stack)
{
    // TODO: move to separate stack component
    if (stack != NULL)
    {
        return stack->data; // Return the data at the top of the stack
    }
    return NULL; // Return NULL if stack is empty
}

struct SymbolTable *createSymbolTable()
{
    struct SymbolTable *symbolTable = malloc(sizeof(struct SymbolTable));

    symbolTable->define = define;
    symbolTable->indexOf = indexOf;
    symbolTable->typeOf = typeOf;
    symbolTable->print = print;
    symbolTable->kindOf = kindOf;
    symbolTable->reset = reset;
    symbolTable->varCount = varCount;
    symbolTable->list = NULL;

    return symbolTable;
}

void reset(struct SymbolTable *this)
{
    g_list_free_full(this->list, (GDestroyNotify)g_free);
}

void define(struct SymbolTable *this, char *name, char *type, enum KIND kind)
{
    struct SymbolTableRecord *symbolTableRecord = malloc(sizeof(struct SymbolTableRecord));

    symbolTableRecord->name = malloc((strlen(name) + 10) * sizeof(char));
    strcpy(symbolTableRecord->name, name);
    symbolTableRecord->type = malloc((strlen(type) + 10) * sizeof(char));
    strcpy(symbolTableRecord->type, type);
    symbolTableRecord->kind = kind;

    this->list = g_list_append(this->list, symbolTableRecord);

    GList *list = NULL;
}

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

int kindOf(struct SymbolTable *this, char *name)
{
    GList *list = g_list_first(this->list);

    struct SymbolTableRecord *foundListItem = NULL;
    while (list)
    {
        if (((struct SymbolTableRecord *)list->data)->name == name)
        {
            foundListItem = list->data;
            break;
        }
        list = list->next;
    }

    if (foundListItem == NULL)
    {
        return -1;
    }

    return foundListItem->kind;
}

char *typeOf(struct SymbolTable *this, char *name)
{
    GList *list = g_list_first(this->list);

    struct SymbolTableRecord *foundListItem = NULL;
    while (list)
    {
        if (((struct SymbolTableRecord *)list->data)->name == name)
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

int indexOf(struct SymbolTable *this, char *name)
{
    GList *list = g_list_first(this->list);

    int index = 0;
    while (list)
    {

        if (((struct SymbolTableRecord *)list->data)->name == name)
        {
            break;
        }
        index++;
        list = list->next;
    }

    if (list == NULL)
    {
        index = -1;
    }

    return index;
}

void print(struct SymbolTable *this)
{
    if (this)
    {
        GList *list = this->list;
        while (list)
        {
            struct SymbolTableRecord *data = ((struct SymbolTableRecord *)list->data);

            char *kind[] = {
                "STATIC",
                "FIELD",
                "ARG",
                "VAR"};

            printf("name: %s \ntype: %s \nkind: %s\n\n", data->name, data->type, kind[data->kind]);
            list = list->next;
        }
    }
}