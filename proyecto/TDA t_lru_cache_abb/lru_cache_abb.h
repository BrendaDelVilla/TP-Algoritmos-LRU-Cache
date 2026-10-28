#ifndef LRU_CACHE_ABB_H_INCLUDED
#define LRU_CACHE_ABB_H_INCLUDED

#include <stddef.h>
#include "../TDA_bases/TDA_ListaDoble_Dinamica/listaDoble.h"
#include "../TDA_bases/TDA_Arbol_Binario/arbol.h"




typedef struct {
    void *clave;
    size_t tamClave;
    void *dato;
    size_t tamDato;
} tCacheABB;
typedef struct {
    tListaD lista;
    tArbol abb;
    tNodoD *ultimo;
    int capacidad;
    int cantidad;
    int (*cmp)(const void *, const void *);
} tLru_cache_abb;

typedef struct {
    void *clave;                              /* apunta a la clave dentro de tCacheABB de la lista */
    int (*cmp)(const void *, const void *);   /* función de comparación de claves de usuario */
    tNodoD *nodo;                             /* nodo de la lista que contiene el elemento */
} tIndiceABB;

tLru_cache_abb *crear_lru_cache_abb(int capacidad, int (*cmp)(const void *, const void *));
int agregar_lru_cache_abb(tLru_cache_abb *cache,  void *clave, size_t tamClave, const void *dato, size_t tamDato);
void *obtener_lru_cache_abb(tLru_cache_abb *cache, void *clave);
int borrar_lru_cache_abb(tLru_cache_abb *cache, void *clave);
void vaciar_lru_cache_abb(tLru_cache_abb *cache);
unsigned altura_lru_cache_abb(const tLru_cache_abb *cache);


void liberarNodoLista(tNodoD *nodo);
void moverAlComienzo(tLru_cache_abb *cache, tNodoD *nodo);
int compararIndice(const void *a, const void *b);
tArbol *buscarIndice(tLru_cache_abb *cache, void *clave);
#endif // LRU_CACHE_ABB_H_INCLUDED

