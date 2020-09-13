#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

struct info_var_decl {
	char* nombre;
	int valor;
};

typedef struct info_var_decl info_var;

int insertar(char * var, int valor);

int existe(char* var);

int buscar (char* var);


#endif
