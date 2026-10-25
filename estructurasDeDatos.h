#ifndef ESTRUCTURASDEDATOS_H_INCLUDED
#define ESTRUCTURASDEDATOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Defines pila
#define TAM_PILA 50
#define ERROR_PILA_LLENA 500
#define ERROR_PILA_VACIA -500
//Defines cola
#define ERROR_SIN_MEM 123
#define ERROR_COLA_VACIA 456
//Defines lista
#define ERROR_LISTA_VACIA -888
#define ERROR_ELEM_DUP -999
#define ACEPTA_DUP 1
#define NO_ACEPTA_DUP 0
#define ELEM_ENCONTRADO 1
#define ELEM_NO_ENCONTRADO 0
#define HAY_OCURRENCIA 0

#define TODO_OK 0
#define minimo(X,Y) ((X) < (Y)) ? (X) : (Y)

typedef struct sNodo
{
    void *dato;
    unsigned tam;
    struct sNodo *sig;
} tNodo;

typedef struct
{
    char pila[TAM_PILA];
    unsigned tope;
} tPila;

typedef struct
{
    tNodo *pri, *ult;
} tCola;

typedef tNodo *tLista;

//Funciones Pila
void crearPila(tPila *pl);
void vaciarPila(tPila *pl);
int ponerEnPila(tPila *pl, const void *dato, unsigned cantBytes);
int sacarDePila(tPila *pl, void *buffer, unsigned cantBytes);
int pilaLlena(const tPila *pl, unsigned cantBytes);
int pilaVacia(const tPila *pl);
int verTope(const tPila *pl, void *buffer, unsigned cantBytes);

//Funciones Cola
void crearCola(tCola *pc);
int colaLlena(const tCola *pc, unsigned cantBytes);
int ponerEnCola(tCola *pc, const void *dato, unsigned cantBytes);
int verPrimero(const tCola *pc, void *buffer, unsigned cantBytes);
int colaVacia(const tCola *pc);
int sacarDeCola(tCola *pc, void *buffer, unsigned cantBytes);
void vaciarCola(tCola *pc);

//Funciones Lista
void crearLista(tLista *pl);
void vaciarLista(tLista *pl);
int listaLlena(tLista *pl);
int listaVacia(tLista *pl);
int insertarAlFinal(tLista *pl, void *dato, unsigned tam);
int insertarSinDuplicados(tLista *pl, void *dato, unsigned tam, int (*cmp)(const void *elem1, const void *elem2), void (*accion)(void *elemEnLista, void *datoAccion)); //Agregar parametro de una funcion de accion
int insertarEnOrden(tLista *pl, void *dato, unsigned tam, int (*cmp)(const void *elem1, const void *elem2), int aceptaDuplicados, void (*accion)(void *elemEnLista, void *datoAccion)); //Agregar parametro de si acepta dup. Si no acepta, puede que no haga nada, o que haga una accion
int mostrarLista(const tLista *pl, void (*muestra)(const void *dato));
int buscarEnLista(const tLista *pl, const void *buscado, void *devolver, int (*cmp)(const void *elem1, const void *elem2), void (*accion)(void *dato, void *devolucion));

#endif // ESTRUCTURASDEDATOS_H_INCLUDED
