#include "lista.h"
#include <stdio.h>
#define minimo( X , Y )  ( ( X ) <= ( Y ) ? ( X ) : ( Y ) )

void crearLista (tLista *p)
{
    *p = NULL;
}

int listaVacia (const tLista *p)
{
    return *p == NULL;
}

int listaLlena (const tLista *p, size_t cantBytes)
{
    tNodo *aux = (tNodo *) malloc (sizeof(tNodo)) ;
    void *info = malloc (cantBytes) ;

    free (aux) ;
    free (info) ;
    return aux==NULL || info== NULL;

}

void vaciarLista (tLista *p)
{
    while (*p)
    {
        tNodo *aux = *p;
        *p = aux->sig;
        free (aux->info) ;
        free (aux) ;
    }

}

int ponerAlComienzo (tLista *p, const void *d, size_t cantBytes)
{
    tNodo *nue;
    if ((nue = (tNodo*) malloc(sizeof ( tNodo)))==NULL ||
        (nue->info = malloc(cantBytes) )== NULL)
    {
        free (nue) ;
        return 0;
    }

    memcpy (nue->info, d, cantBytes) ;
    nue->tamInfo = cantBytes;
    nue->sig = * p;
    *p = nue;
    return 1;
}

int sacarPrimeroLista(tLista *p, void *d, size_t cantBytes)
{
    tNodo *aux = *p;
    if (aux == NULL)
        return 0;

    *p = aux->sig;
    memcpy (d, aux->info, minimo (cantBytes, aux->tamInfo) ) ;
    free (aux->info) ;
    free (aux) ;
    return 1;
}

int verPrimeroLista (const tLista *p, void *d, size_t cantBytes)
{
    if (*p == NULL)
        return 0;

    memcpy (d, (*p) ->info, minimo (cantBytes, (*p) ->tamInfo) ) ;
    return 1;

}

int ponerAlFinal (tLista *p, const void *d, size_t cantBytes)
{
    tNodo *nue;

    while(*p)
        p=&(*p) ->sig;

    if ((nue = (tNodo*) malloc (sizeof ( tNodo)))== NULL ||
        (nue->info = malloc (cantBytes) )==NULL)
    {
        free (nue) ;
        return 0;
    }

    memcpy (nue->info, d, cantBytes) ;
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    *p = nue;
    return 1;
}

int sacarUltimoLista (tLista *p, void *d, size_t cantBytes)
{
    if (*p == NULL)
        return 0;

    while ((*p) ->sig)
        p = & (*p)->sig;

    memcpy (d, (*p) ->info, minimo (cantBytes, (*p) ->tamInfo) ) ;
    free ( (*p) ->info);
    free (*p) ;
    *p = NULL;
    return 1;
}

int verUltimoLista (const tLista *p, void *d, size_t cantBytes)
{
    if (*p == NULL)
        return 0;

    while ( (*p) ->sig)
        p = & (*p)->sig;

    memcpy (d, (*p) ->info, minimo (cantBytes, (*p) ->tamInfo) ) ;
    return 1;
}

void mapp(tLista *p,void (*action)( void*, void*),void *parametro)
{
    while(*p)
    {
        action((*p)->info,parametro);
        p=&(*p)->sig;
    }
}

void filter(tLista *p,int (*filtro)(void*,void*),void *parametro)
{
    tNodo *elim;
    while(*p)
    {
        if(filtro((*p)->info,parametro))
        {
            elim=*p;
            *p=elim->sig;
            free(elim->info);
            free(elim);
        }
        else
        {
            p=&(*p)->sig;
        }
    }

}

void reduce(tLista *p,void *result,void (*action) (void*,void*,void*),void *parametro)
{
    while(*p)
    {
        action((*p)->info,result,parametro);
        p=&(*p)->sig;
    }
}

void reinsert_nodo_menor(tLista *p,tLista *men)
{
    tNodo *aux=*men;
    *men=aux->sig;

    aux->sig=*p;
    *p=aux;

}
void intercambio_info_nodos(tLista *n1,tLista *n2)
{
    unsigned aux_tam;
    void *aux=(*n1)->info;
    (*n1)->info=(*n2)->info;
    (*n2)->info=aux;

    aux_tam=(*n1)->tamInfo;
    (*n1)->tamInfo=(*n2)->tamInfo;
    (*n2)->tamInfo=aux_tam;
}
tNodo ** buscar_menor_nodo(const tLista *p,int (*cmp)(const void*,const void*))
{
    const tLista *men=p;
    if(!*p)
    {
        return NULL;
    }
    p=&(*p)->sig;
    while(*p)
    {
        if(cmp((*p)->info,(*men)->info)<0)
        {
            men=p;
        }
        p=&(*p)->sig;
    }

    return (tNodo**)men;
}
void ordenar_sel_lista(tLista *p,int (*cmp)(const void*,const void*))
{

    /** se busca el menor
        se intercambia
        se avanza
    **/
    tNodo **men;
    if(!*p)
        return;

    while((*p)->sig)
    {
        men=buscar_menor_nodo(p,cmp);
        if(p!=men)
        {
            //intercambio_info_nodos(pl,men);
            reinsert_nodo_menor(p,men); // se usa esta porque funciona con distintos tipos de NODOS
        }
        p=&(*p)->sig;
    }

}
int poner_orden_lista(tLista *p,const void* pd,size_t tam,int (*cmp)(const void*,const void*))
{
    tNodo *nue; //crear nuevo nodo con malloc para agrandar el espacio
    int rc;

    while(*p && (rc=cmp(pd,(*p)->info))>0)
    {
        p=&(*p)->sig;
    }

    if(*p && !rc)
        return 0; //DUPLICADO

    nue=(tNodo*)malloc(sizeof(tNodo));
    if(!nue)
    {
        return 0; //LISTA LLENA
    }
    nue->info=malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return 0;// LISTA LLENA
    }

    nue->tamInfo=tam;
    memcpy(nue->info,pd,tam);
    nue->sig=*p;
    *p=nue;

    return 1; //OK
}

int poner_pos_lista(tLista *p, const void* pd,size_t tam, unsigned pos)
{
    tNodo *nue;

    while(*p && pos) // se cuenta desde el 0
    {
        p=&(*p)->sig;
        pos--;
    }
    if(pos>0)// depende del problema aca si manda la pos 35 y hay 5 elementos es INVALIDO pero si el ejercicio lo pide puede Añadirse la final
    {
        return 0; //POS INVALIDA
    }

    nue=(tNodo*)malloc(sizeof(tNodo));
    if(!nue)
    {
        return 0; //LISTA LLENA
    }

    nue->info=malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return 0;// LISTA LLENA
    }

    nue->tamInfo=tam;
    memcpy(nue->info,pd,tam);
    nue->sig=*p;
    *p=nue;

    return 1;
}


int sacar_elem_lista(tLista *p, void* pd,size_t tam,int (*cmp)(const void*,const void*))
{
    int rc;
    tNodo *elim;

    while(*p && (rc=cmp(pd,(*p)->info))!=0)
    {
        p=&(*p)->sig;

    }
    if(*p==NULL)
        return 0; //NO EXISTE

    elim=*p;
    memcpy(pd,elim->info,minimo(tam,elim->tamInfo));

    *p = elim->sig;
    free (elim->info);
    free (elim) ;

    return 1;
}

int sacar_elem_ord_lista(tLista *p, void* pd,size_t tam,int (*cmp)(const void*,const void*))
{
    int rc;
    tNodo *elim;

    while(*p && (rc=cmp(pd,(*p)->info))>0) //no BUSQUEDA BINARIA
    {
        p=&(*p)->sig;

    }

    if(*p==NULL || rc)
        return 0; //NO EXISTE

    elim=*p;
    memcpy(pd,elim->info,minimo(tam,elim->tamInfo));

    *p = elim->sig;
    free (elim->info);
    free (elim) ;

    return 1;
}


void eliminarDuplicadoListaOrdenada(tLista *p,int (*cmp)(const void*,const void*),void (*action)(void*,const void*))
{
    tLista *aux;
    tNodo *pelim;
    while(*p)
    {
        aux=&(*p)->sig;
        while(*aux && !cmp((*p)->info,(*aux)->info))
        {
            action((*p)->info,(*aux)->info);

            pelim=*aux;
            *aux=pelim->sig;

            free(pelim->info);
            free(pelim);
        }
        p=&(*p)->sig;
    }

}


