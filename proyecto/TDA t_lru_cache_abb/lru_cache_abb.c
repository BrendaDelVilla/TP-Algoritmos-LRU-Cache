#include "lru_cache_abb.h"

unsigned altura_lru_cache_abb(const tLru_cache_abb *cache)
{
    return alturaArbol(&cache->abb);
}
int compararIndice(const void *a, const void *b)
{
    const tIndiceABB *ia = a;
    const tIndiceABB *ib = b;

    return ia->cmp(ia->clave, ib->clave);
}
tArbol *buscarIndice(tLru_cache_abb *cache, void *clave)
{
    tIndiceABB buscado;

    buscado.clave = clave;
    buscado.nodo = NULL;
    buscado.cmp = cache->cmp;

    return buscarClave(
        &cache->abb,
        compararIndice,
        &buscado
    );
}
void moverAlComienzo(tLru_cache_abb *cache, tNodoD *nodo)
{
    if (cache->lista == nodo)
        return;


    if (nodo->ant)
        nodo->ant->sig = nodo->sig;

    if (nodo->sig)
        nodo->sig->ant = nodo->ant;

    /* Si era el último, creo que actualiza el puntero */
    if (cache->ultimo == nodo)
        cache->ultimo = nodo->ant;

    /* lo ponemos al inicio */
    nodo->ant = NULL;
    nodo->sig = cache->lista;

    if (cache->lista)
        cache->lista->ant = nodo;

    cache->lista = nodo;
}
void liberarNodoLista(tNodoD *nodo)
{
    tCacheABB *entrada = nodo->info;

    free(entrada->clave);
    free(entrada->dato);
    free(nodo->info);
    free(nodo);
}
void vaciar_lru_cache_abb(tLru_cache_abb *cache)
{
    tNodoD *act;
    tNodoD *sig;

    if (!cache)
        return;

    act = cache->lista;

    while (act)
    {
        sig = act->sig;
        liberarNodoLista(act);
        act = sig;
    }

    cache->lista = NULL;
    cache->ultimo = NULL;
    cache->cantidad = 0;

    vaciarArbol(&cache->abb);
}

tLru_cache_abb *crear_lru_cache_abb(int capacidad, int (*cmp)(const void *, const void *))
{
    tLru_cache_abb *cache;
    if (capacidad <= 0 || cmp == NULL)
        return NULL;

    cache = malloc(sizeof(tLru_cache_abb));
    if(!cache)
        return NULL;

    crearListaD(&cache->lista);
    crearArbol(&cache->abb);
    cache->ultimo = NULL;
    cache->capacidad = capacidad;
    cache->cantidad = 0;
    cache->cmp = cmp;
    return cache;
}

void *obtener_lru_cache_abb(tLru_cache_abb *cache, void *clave)
{
    tArbol *pos;
    tIndiceABB *indice;
    tCacheABB *entrada;

    if (!cache || !clave)
        return NULL;

    pos = buscarIndice(cache, clave);

    if (!pos)
        return NULL;

    indice = (*pos)->dato;
    moverAlComienzo(cache, indice->nodo);

    entrada = indice->nodo->info;

    return entrada->dato;
}
int borrar_lru_cache_abb(tLru_cache_abb *cache, void *clave)
{
    tArbol *pos;
    tIndiceABB *indice;
    tNodoD *nodo;
    int rc;

    if (!cache || !clave)
        return 0;

    pos = buscarIndice(cache, clave);

    if (!pos)
        return 0;

    indice = (*pos)->dato;
    nodo = indice->nodo;

    rc = eliminarDelArbol(&cache->abb,indice,sizeof(tIndiceABB),compararIndice);

    if (rc != OK)
        return rc;

    /* Desconecto el nodo de la lista. */
    if (nodo->ant)
        nodo->ant->sig = nodo->sig;
    else
        cache->lista = nodo->sig;

    if (nodo->sig)
        nodo->sig->ant = nodo->ant;
    else
        cache->ultimo = nodo->ant;

    liberarNodoLista(nodo);
    cache->cantidad--;

    return OK;
}

int agregar_lru_cache_abb(tLru_cache_abb *cache, void *clave, size_t tamClave,const void *dato, size_t tamDato)
{
    tArbol *pos;
    tIndiceABB nuevoIndice;
    tNodoD *nuevoNodo;
    tCacheABB nueva;
    tCacheABB *entrada;
    void *nuevaClave;
    void *nuevoDato;
    int rc;

    if (!cache || !clave || !dato ||
        !cache->cmp || tamClave == 0 || tamDato == 0)
        return SIN_MEM;

    if (cache->capacidad == 0)
        return SIN_MEM;

    // Busco la clave en el indice.
    pos = buscarIndice(cache, clave);

    /*
      Si ya existe reemplazamos solamente el dato
      La clave almacenada y su índice permanecen iguales.
     */
    if (pos)
    {
        tIndiceABB *indice = (*pos)->dato;

        entrada = indice->nodo->info;

        nuevoDato = malloc(tamDato);

        if (!nuevoDato)
            return SIN_MEM;

        memcpy(nuevoDato, dato, tamDato);

        free(entrada->dato);

        entrada->dato = nuevoDato;
        entrada->tamDato = tamDato;

        moverAlComienzo(cache, indice->nodo);

        return OK;
    }


    nuevaClave = malloc(tamClave);

    if (!nuevaClave)
        return SIN_MEM;

    nuevoDato = malloc(tamDato);

    if (!nuevoDato)
    {
        free(nuevaClave);
        return SIN_MEM;
    }

    memcpy(nuevaClave, clave, tamClave);
    memcpy(nuevoDato, dato, tamDato);

    nueva.clave = nuevaClave;
    nueva.tamClave = tamClave;
    nueva.dato = nuevoDato;
    nueva.tamDato = tamDato;

    //inserto la entrada al comienzo de la lista
    rc = insertarAlComienzo(
        &cache->lista,
        &nueva,
        sizeof(tCacheABB)
    );

    if (!rc)
    {
        free(nuevaClave);
        free(nuevoDato);
        return SIN_MEM;
    }

    nuevoNodo = cache->lista;

    if (!cache->ultimo)
        cache->ultimo = nuevoNodo;


      //inserto en el arbol una referencia al nodo de la lista

    nuevoIndice.clave =
        ((tCacheABB *)nuevoNodo->info)->clave;

    nuevoIndice.nodo = nuevoNodo;
    nuevoIndice.cmp = cache->cmp;

    rc = insertarArbolI(
        &cache->abb,
        &nuevoIndice,
        sizeof(tIndiceABB),
        compararIndice
    );

    if (rc != OK)
    {
        // Deshace la insercion en la lista
        cache->lista = nuevoNodo->sig;

        if (cache->lista)
            cache->lista->ant = NULL;

        if (cache->ultimo == nuevoNodo)
            cache->ultimo = NULL;

        liberarNodoLista(nuevoNodo);

        return rc;
    }

    cache->cantidad++;

    //supera la capacidad lo expulso del LRU
    if (cache->cantidad > cache->capacidad)
    {
        tNodoD *victima = cache->ultimo;
        tCacheABB *entradaVictima = victima->info;
        tIndiceABB indiceVictima;

        indiceVictima.clave = entradaVictima->clave;
        indiceVictima.nodo = victima;
        indiceVictima.cmp = cache->cmp;


          // primero elimino la referencia del arbol dps la entrada de la lista

        rc = eliminarDelArbol(&cache->abb,&indiceVictima,sizeof(tIndiceABB),compararIndice);

        if (rc != OK)
            return rc;

        cache->ultimo = victima->ant;

        if (cache->ultimo)
            cache->ultimo->sig = NULL;
        else
            cache->lista = NULL;

        liberarNodoLista(victima);
        cache->cantidad--;
    }

    return OK;
}




