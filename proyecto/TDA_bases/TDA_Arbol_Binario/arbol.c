#include "arbol.h"


void crearArbol(tArbol *p){
    *p = NULL;
}

int insertarArbolI(tArbol* p, void* d, unsigned tam,int(*cmp)(const void*, const void*)){

    tNodoArbol *nue;
    int rc;

    while(*p){
        if((rc= cmp(d,(*p)->dato))<0)
            p = &(*p)->izq;
        else if (rc>0)
            p = &(*p)->der;
        else
            return DUPLICADO;
    }

    nue = (tNodoArbol*)malloc(sizeof(tNodoArbol));
    if(!nue)
        return SIN_MEM;
    nue->dato = malloc(tam);
    if(!nue->dato)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->dato,d,tam);
    nue->tam= tam;
    nue->der= NULL;
    nue->izq= NULL;
    *p=nue;

    return OK;
}

int insertarRecArbolBinBusq(tArbol *p, const void *d, unsigned tam, int (*cmp)(const void *, const void *)){
    tNodoArbol *nue;
    int rc;
    if(*p){
        if ((rc = cmp(d,(*p)->dato))<0)
            return insertarRecArbolBinBusq(&(*p)->izq,d,tam,cmp);
        if(rc>0)
            return insertarRecArbolBinBusq(&(*p)->der,d,tam,cmp);
        return DUPLICADO;
    }
    nue = (tNodoArbol*)malloc(sizeof(tNodoArbol));
    if(!nue)
        return SIN_MEM;
    nue->dato = malloc(tam);
    if(!nue->dato)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->dato,d,tam);
    nue->tam= tam;
    nue->der= NULL;
    nue->izq= NULL;
    *p=nue;

    return OK;

}
void recorrerPreorden(tArbol* p, unsigned n, void *params, void (*accion)(void *, unsigned, unsigned, void *))
{
    if(!*p)
        return;
    accion((*p)->dato, (*p)->tam,n,params);
    recorrerPreorden(&(*p)->izq,n+1,params,accion);
    recorrerPreorden(&(*p)->der,n+1,params,accion);
}

void recorrerPosorden(tArbol* p, unsigned n, void *params, void (*accion)(void *, unsigned, unsigned, void *)){
    if(!*p)
        return;
    recorrerPosorden(&(*p)->izq,n+1,params,accion);
    recorrerPosorden(&(*p)->der,n+1,params,accion);
    accion((*p)->dato, (*p)->tam,n,params);

}

void recorrerInorden(tArbol* p, unsigned n, void *params, void (*accion)(void *, unsigned, unsigned, void *)){

    if(!*p)
        return;
    recorrerInorden(&(*p)->izq,n+1,params,accion);
    accion((*p)->dato, (*p)->tam,n,params);
    recorrerInorden(&(*p)->der,n+1,params,accion);



}

int contarNodos(const tArbol* pa)
{
    int izq=0, der=0;

    if (!*pa)
        return 0;

    izq = contarNodos(&(*pa)->izq);
    der = contarNodos(&(*pa)->der);

    return 1 + izq + der;
}
int contarHojas(const tArbol* pa)
{
    if (!*pa)
        return 0;

    if (!(*pa)->izq && !(*pa)->der)
        return 1;

    return contarHojas(&(*pa)->izq) + contarHojas(&(*pa)->der);
}
int contarNohoja(const tArbol* pa)
{
    if (!*pa)
        return 0;

    if ((*pa)->izq != NULL || (*pa)->der != NULL)
        return 1+contarNohoja(&(*pa)->izq) + contarNohoja(&(*pa)->der);
    return 0;
}

unsigned alturaArbol(const tArbol *pa)
{
    int izq=0, der=0;
    if (*pa == NULL)
        return 0;

    izq = alturaArbol(&(*pa)->izq);
    der = alturaArbol(&(*pa)->der);

    return 1 + (izq > der ? izq : der);
}
int contarNodosConIzq(const tArbol* pa)
{
    if (!*pa)
        return 0;

    if((*pa)->izq)
        return 1+contarNodosConIzq(&(*pa)->izq)+contarNodosConIzq(&(*pa)->der);

    return 0;//SI NO ES NULO Y NO TIENE HIJO IZQ
}
unsigned cantNodosNivel(const tArbol *pa, unsigned nivel)
{
    if(!*pa)
        return 0;

    if(nivel==0)
        return 1;
    nivel--;
    return cantNodosNivel(&(*pa)->izq,nivel)+cantNodosNivel(&(*pa)->der,nivel);
}

unsigned cantNodosHastaNivel(const tArbol *pa, unsigned nivel)
{
    if(!*pa)
        return 0;

    if(nivel==0)
        return 1;
    nivel--;
    return 1+cantNodosHastaNivel(&(*pa)->izq,nivel)+cantNodosHastaNivel(&(*pa)->der,nivel);
}

unsigned cantNodosDesdeNivel(const tArbol *pa, unsigned nivel)
{
    if(!*pa)
        return 0;
    if(nivel==0)
        return 1+cantNodosDesdeNivel(&(*pa)->izq,nivel)+cantNodosDesdeNivel(&(*pa)->der,nivel);;
    nivel--;
    return cantNodosDesdeNivel(&(*pa)->izq,nivel)+cantNodosDesdeNivel(&(*pa)->der,nivel);

}
unsigned buscarClaveBool(const tArbol *pa, int(*cmp)(const void*, const void*),void *clave){
    int rc;

    if(!*pa)
        return 0;

    if((rc=cmp(clave,(*pa)->dato))==0)
        return 1;
    else if(rc<0)
        return buscarClaveBool(&(*pa)->izq,cmp,clave);
    else
        return buscarClaveBool(&(*pa)->der,cmp,clave);

}
unsigned cantNodosPorClaveDer(const tArbol *pa, int(*cmp)(const void*, const void*),void *clave){
    int rc;

    if(!*pa)
        return 0;

    if((rc=cmp(clave,(*pa)->dato))==0)
        return contarNodos(&(*pa)->der);

    else if(rc<0)
        return cantNodosPorClaveDer(&(*pa)->izq,cmp,clave);
    else
        return cantNodosPorClaveDer(&(*pa)->der,cmp,clave);
}
int eliminarHoja(tArbol *pa, int(*cmp)(const void*, const void*),void *clave)
{
    int rc;

    if(!*pa)
        return 0;

    if((rc=cmp(clave,(*pa)->dato))==0)
    {
        if((*pa)->der==NULL && (*pa)->izq==NULL)
        {
            free((*pa)->dato);
            free(*pa);
            *pa=NULL;
            return 1;
        }
        else
            return 0;
    }
    else if(cmp<0)
        return eliminarHoja(&(*pa)->izq,cmp,clave);
    else
        return eliminarHoja(&(*pa)->der,cmp,clave);
}

int eliminarTodasHoja(tArbol *pa)
{
    if(!*pa)
        return 0;

    if((*pa)->der==NULL && (*pa)->izq==NULL)
    {
        free((*pa)->dato);
        free(*pa);
        *pa=NULL;
        return 1;
    }
    return eliminarTodasHoja(&(*pa)->izq)+eliminarTodasHoja(&(*pa)->der);
}
void vaciarArbol(tArbol* pa)
{
    if (!*pa)
        return;

    vaciarArbol(&(*pa)->izq);
    vaciarArbol(&(*pa)->der);

    free((*pa)->dato);
    free(*pa);
    *pa = NULL;
}
tArbol* buscarClave(tArbol*pa,int(*cmp)(const void*, const void*),void *clave)
{
    if(!*pa)
        return NULL;

    int rc;
    if((rc=cmp(clave,(*pa)->dato))<0)
        return buscarClave(&(*pa)->izq, cmp,clave);
    else if(rc>0)
        return buscarClave(&(*pa)->der, cmp,clave);
    else
        return pa;
}

void intercambio(tArbol *elim, tArbol *paR)
{
    void *auxDato = (*elim)->dato;
    unsigned auxTam = (*elim)->tam;

    (*elim)->dato = (*paR)->dato;
    (*elim)->tam = (*paR)->tam;

    (*paR)->dato = auxDato;
    (*paR)->tam = auxTam;
}

int eliminarDelArbol(tArbol *pa, void *dato, unsigned tam, int(*cmp)(const void*, const void*))
{
    if(!*pa)
        return 0;

    tArbol* elim = buscarClave(pa, cmp, dato);
    if(!elim)
        return 0;

    if((*elim)->der == NULL && (*elim)->izq == NULL)
    {
        free((*elim)->dato);
        free(*elim);
        *elim = NULL;
        return OK;
    }


    tArbol* paR;
    tArbol nodoAEliminar;

    int altI = alturaArbol(&(*elim)->izq);
    int altD = alturaArbol(&(*elim)->der);

    if(altD > altI)
    {

        paR = &(*elim)->der;
        while((*paR)->izq)
            paR = &(*paR)->izq;
    }
    else
    {

        paR = &(*elim)->izq;
        while((*paR)->der)
            paR = &(*paR)->der;
    }


    nodoAEliminar = *paR;


    intercambio(elim, paR);


    if((*paR)->der)
        *paR = (*paR)->der;
    else if((*paR)->izq)
        *paR = (*paR)->izq;
    else
        *paR = NULL;


    free(nodoAEliminar->dato);
    free(nodoAEliminar);

    return OK;
}
