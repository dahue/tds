#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <glib.h>
#include <stdbool.h>

enum flag{EF_INDF, E_ID, E_FUNC, E_PARAM};
enum type{E_INDF, E_VOID, E_INTEGER, E_BOOL};

struct Symbol {
    enum flag flag;
    char* name;
    enum type type;
    int value_int;
    bool value_bool;
    GList *param;

};

struct Symbol *newSymbol();

char *toString(struct Symbol *s);

GList *newSymbolTable(GList *stack);

GList *addLevel(GList *stack);

GList *removeLevel(GList *stack);

GList *insertSymbol(GList *stack, struct Symbol *symbol);

struct Symbol *exists(GList *stack, char *name);

struct Symbol *findSymbol(GList *stack, char *name);

#endif
