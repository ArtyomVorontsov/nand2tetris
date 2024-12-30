#include "./symbol-table.h"
#include <stdio.h>
#include <stdlib.h>

struct SymbolTable *createSymbolTable()
{
    struct SymbolTable *symbolTable = malloc(sizeof(struct SymbolTable));

    return symbolTable;
}

void reset(struct SymbolTable *this)
{
    g_list_free_full(this->list, (GDestroyNotify)g_free);
}

void define(struct SymbolTable *this, char *name, char *type, enum KIND kind)
{
    struct SymbolTableRecord *symbolTableRecord = malloc(sizeof(struct SymbolTableRecord));

    this->list = g_list_append(this->list, symbolTableRecord);
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
