#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <glib.h>
#include <stdbool.h>

enum flag{
    F_INDF, 
    F_ID,
    F_ID_GLOBAL,
    F_FUNC, 
    F_END_FUNC,
    F_PARAM, 
    F_CONST, 
    F_PLUS_OP, 
    F_MUL_OP, 
    F_AND_OP, 
    F_EQ_OP, 
    F_NOT_OP, 
    F_ASSIGN_OP,
    F_RETURN
};
enum type{
    T_INDF, 
    T_VOID, 
    T_INTEGER, 
    T_BOOL
};

struct Symbol {
    enum flag flag;
    char* name;
    enum type type;
    int value_int;
    bool value_bool;
    GList *param;
    int reg;
    int lineno;
    int offset;
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
