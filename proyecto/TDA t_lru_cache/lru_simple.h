#ifndef LRU_SIMPLE_H_INCLUDED
#define LRU_SIMPLE_H_INCLUDED

#include <stddef.h>

//#include "lista.h"
#include "../TDA_bases/TDA_Lista_Simple/lista.h"

// struct cache
typedef struct {
    void *clave;
    size_t tamClave;
    void *dato;
    size_t tamDato;
}tCache;


typedef struct {
    tLista lista; //lista simple
    int capacidad;
    int cantidad;
    int (*accion)(const void *, const void *);
}tLru_cache;


tLru_cache *crear_lrucache(int capacidad, int (*cmp)(const void *, const void *));

int sacarClaveCache(tLista *p, const void *clave,tCache *aux,int (*cmp)(const void *, const void *));
int agregar_lrucache(tLru_cache *cache, const void *clave, size_t tamClave, const void *dato, size_t tamDato);

void *obtener_lrucache(tLru_cache *cache, const void *clave);

int borrar_lrucache(tLru_cache *cache, const void *clave);

void vaciar_lrucache(tLru_cache *cache);

#endif // LRU_SIMPLE_H_INCLUDED
