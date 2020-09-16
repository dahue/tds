/* Tabla de simbolos para el evaluador de expresiones*/

#include "TS.h"

#include <string.h>

#define max_cant_var 10 // tamaño maximo de la tabla de simbolos

info_var TS[max_cant_var]; // tabla de simbolos implementada sobre un arreglo (evaluar entajas desventajas!!!)

int cant_var = 0; // cantidad de variables insertada en la tabla de simbolos

/* Inserta la variable con su valor en la TS */
/* Retorna 1 si pudo insertar correctamente, 0 si ya existe, -1 si la TS esta llena*/
int insertar(char * var, int valor){
	if (cant_var==max_cant_var) 
	    return -1; // TS llena, no se pueden insertar más variables
	
	if (existe(var)!=1){
	    TS[cant_var].nombre = var;
	    TS[cant_var].valor = valor;
	    cant_var++;
	    return 1; // se inserto correctamente 
	}else    
	    return 0; // ya existe, no se inserta
	}

/* Retorna 1 si la variable existe en la TS, 0 en otro caso */
int existe(char* var){
	int i;
	for(i=0;i<=cant_var-1;i++){
		if (strcmp(TS[i].nombre, var)==0)		
		   return 1; 
		} 
	return 0;	
	}

/* Retorna el valor asociado a una variable*/
/* Precondicion: la variable existe en la TS*/
int buscar_valor (char* var){
	int i;
	for(i=0;i<=cant_var-1;i++){
		if (strcmp(TS[i].nombre, var)==0)		
		   return TS[i].valor; 
		} 
	}
