#include "./symbol-table.h"
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

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

    symbolTableRecord->name = malloc(strlen(name) * sizeof(char));
    strcpy(symbolTableRecord->name, name);
    symbolTableRecord->type = malloc(strlen(type) * sizeof(char));
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
    GList *list = this->list;
    while (list)
    {
        struct SymbolTableRecord *data = ((struct SymbolTableRecord *)list->data);
        printf("name: %s \ntype: %s \nkind: %d\n\n", data->name, data->type, data->kind);
        list = list->next;
    }
}