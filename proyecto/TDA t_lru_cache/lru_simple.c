#include "lru_simple.h"

#include <string.h>

tLru_cache *crear_lrucache(int capacidad, int (*cmp)(const void *, const void *))
{
    tLru_cache *cache = (tLru_cache *)malloc(sizeof(tLru_cache));
    if (cache == NULL)
        return NULL;


    crearLista(&(cache->lista));
    cache->capacidad=capacidad;
    cache->cantidad=0;
    cache->accion=cmp;

    return cache;
}


int sacarClaveCache(tLista *p, const void *clave,tCache *aux,int (*cmp)(const void *, const void *))
{
    tNodo *elim;
    tCache *actual;

    while(*p)
    {
        actual = (tCache *)(*p)->info;

        if(cmp(clave, actual->clave) == 0)
        {
            *aux = *actual;

            elim = *p;
            *p = elim->sig;

            free(elim->info);
            free(elim);

            return 1;
        }

        p = &(*p)->sig;
    }

    return 0;
}

int agregar_lrucache(tLru_cache *cache, const void *clave, size_t tamClave,const void *dato, size_t tamDato)
{
    tCache *cc;
    tCache aux;
    int existe;

    if(!cache || !clave || !dato )
        return 0;

    cc = malloc(sizeof(tCache));// se tiene que reservar memoria para clave y dato
    if(!cc)
        return 0;

    cc->clave = malloc(tamClave);
    cc->dato = malloc(tamDato);

    if(!cc->clave || !cc->dato)
    {
        free(cc->clave);
        free(cc->dato);
        free(cc);
        return 0;
    }

    memcpy(cc->clave, clave, tamClave);
    memcpy(cc->dato, dato, tamDato);

    cc->tamClave = tamClave;
    cc->tamDato = tamDato;

    existe = sacarClaveCache(&(cache->lista), clave,&aux, cache->accion); //reemplaza sacar_elem_lista por el tema de la clave

    if(existe)
    {
        free(aux.clave);
        free(aux.dato);
        cache->cantidad--;
    }

    else if(cache->cantidad >= cache->capacidad)
    {
        if(!sacarUltimoLista(&(cache->lista), &aux, sizeof(tCache))) //si no lo saco
        {
            free(cc->clave);
            free(cc->dato);
            free(cc);
            return 0;
        }

        free(aux.clave);
        free(aux.dato);
        cache->cantidad--;
    }

    if(!ponerAlComienzo(&(cache->lista), cc, sizeof(tCache)))// si no se pudo agregar
    {
        free(cc->clave);
        free(cc->dato);
        free(cc);
        return 0;
    }

    free(cc);
    cache->cantidad++;

    return 1;
}

void *obtener_lrucache(tLru_cache *cache, const void *clave)
{
    tLista *p;
    tNodo *actual;
    tCache *info_cache;

    if (!cache || !cache->lista)
        return NULL;

    p=&(cache->lista);

    while (*p)
    {
        actual = *p;
        info_cache =(tCache*)actual->info;

        if(cache->accion(clave, info_cache->clave) == 0)
        {
            if (actual !=cache->lista) //si no esta al comienzo
            {
                *p =actual->sig;

                actual->sig = cache->lista; //apunta al inicio ahora
                cache->lista =actual;
            }

            return info_cache->dato; //devuelvo el dato
        }

        p=&((*p)->sig);
    }

    return NULL;
}

int borrar_lrucache(tLru_cache *cache, const void *clave)
{
    tLista *p;
    tCache aux;

    int existe;

    if(!cache || !cache->lista)
        return 0;

    p=&(cache->lista);

    existe=sacarClaveCache(p,clave,&aux,cache->accion); //busca y si existe lo elimina
    if(existe) //encontrado
    {

        free(aux.clave);
        free(aux.dato);
        cache->cantidad--;
        return 1;
    }
    return 0;
}

void vaciar_lrucache(tLru_cache *cache)
{

    tLista *p;
    tCache *actual;

    if(!cache || !cache->lista)
        return;

    p=&(cache->lista);

    while(*p)
    {
        actual = (tCache *)(*p)->info;

        free(actual->clave);
        free(actual->dato);

        p=&(*p)->sig;

    }
    vaciarLista(&(cache->lista));

    cache->cantidad=0;
}
