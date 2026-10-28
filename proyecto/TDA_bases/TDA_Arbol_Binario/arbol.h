#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define SIN_MEM -1
#define DUPLICADO -2

typedef struct sNodo{
void *dato;
unsigned tam;
struct sNodo *der;
struct sNodo *izq;}tNodoArbol;

typedef tNodoArbol *tArbol;



void crearArbol(tArbol *p);
int insertarArbolI(tArbol* p, void* d, unsigned tam,int(*cmp)(const void*, const void*));
int insertarArbolR(tArbol* p, void* d, unsigned tam,int(*cmp)(const void*, const void*));

void recorrerPreorden(tArbol* p, unsigned n, void *params, void (*accion)(void *, unsigned, unsigned, void *));
void recorrerPosorden(tArbol* p, unsigned n, void *params, void (*accion)(void *, unsigned, unsigned, void *));
void recorrerInorden(tArbol* p, unsigned n, void *params, void (*accion)(void *, unsigned, unsigned, void *));

int contarNodos(const tArbol *pa);
int contarNohoja(const tArbol* pa);
int contarHojas(const tArbol* pa);
unsigned alturaArbol(const tArbol* pa);
int contarNodosConIzq(const tArbol* pa);
unsigned cantNodosNivel(const tArbol *pa, unsigned nivel);
unsigned cantNodosHastaNivel(const tArbol *pa, unsigned nivel);
unsigned cantNodosDesdeNivel(const tArbol *pa, unsigned nivel);
unsigned buscarClaveBool(const tArbol *pa, int(*cmp)(const void*, const void*),void *clave);
unsigned cantNodosPorClaveDer(const tArbol *pa, int(*cmp)(const void*, const void*),void *clave);
int eliminarHoja(tArbol *pa, int(*cmp)(const void*, const void*),void *clave);
int eliminarTodasHoja(tArbol *pa);
tArbol* buscarClave(tArbol*pa,int(*cmp)(const void*, const void*),void *clave);
void intercambio(tArbol *elim, tArbol *paR);
int eliminarDelArbol(tArbol *pa,void *dato, unsigned tam, int(*cmp)(const void*, const void*));
void vaciarArbol(tArbol* pa);

#endif // ARBOL_H_INCLUDED
