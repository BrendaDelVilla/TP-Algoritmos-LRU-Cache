#include "listaDoble.h"

#include <stdlib.h>
#include <string.h>
void crearListaD(tListaD *p)
{
    *p=NULL;
}

int vaciarListaD(tListaD *p)
{
    int cant=0;
    tNodoD *act=*p,*aux;

    if(act)
    {
        while(act->ant)
            act=act->ant;

        while(act)
        {
            aux=act->sig;

            free(act->info);
            free(act);
            act=aux;
            cant++;
        }
        *p=NULL;
    }

    return cant;
}

int listaDVacia(const tListaD *p)
{
    return *p==NULL;
}

int listaDllena(const tListaD *p,size_t tam)
{
    tNodoD *nue=(tNodoD*)malloc(sizeof(tNodoD));
    void *aux=malloc(tam);

    free(nue);
    free(aux);

    return aux==NULL || nue==NULL;

}

int insertarAlComienzo(tListaD *p,const void *pd,size_t tam)
{
    tNodoD *act=*p,
           *nue;

    if(act)
    {
        while(act->ant)
            act=act->ant;
    }
    if((nue=(tNodoD*)malloc(sizeof(tNodoD))) == NULL ||
        (nue->info=malloc(tam))==NULL)
    {
        free(nue);
        return 0;
    }

    memcpy(nue->info,pd,tam);
    nue->tamInfo=tam;
    nue->sig=act;
    nue->ant=NULL;

    if(act)
        act->ant=nue;
    *p=nue;

    return 1;
}

int insertarAlFinal(tListaD *p,const void *pd,size_t tam)
{
    tNodoD *act=*p,
           *nue;

    if(act)
    {
        while(act->sig)
            act=act->sig;
    }
    if((nue=(tNodoD*)malloc(sizeof(tNodoD))) == NULL ||
        (nue->info=malloc(tam))==NULL)
    {
        free(nue);
        return 0;
    }

    memcpy(nue->info,pd,tam);
    nue->tamInfo=tam;
    nue->sig=NULL;
    nue->ant=act;

    if(act)
        act->sig=nue;
    *p=nue;

    return 1;
}

int mostrarDeIzqADer(const tListaD *p, void (*mostrar)(const void*))
{
    tNodoD *act=*p;
    int cant=0;

    if(act)
    {
        mostrar(NULL);
        while(act->ant)
            act=act->ant;
        while(act)
        {
            mostrar(act->info);
            act=act->sig;
            cant++;
        }
    }

    return cant;
}
int mostrarDeDerAIzq(const tListaD *p, void (*mostrar)(const void*))
{
    tNodoD *act=*p;
    int cant=0;

    if(act)
    {
        mostrar(NULL);
        while(act->sig)
            act=act->sig;
        while(act)
        {
            mostrar(act->info);
            act=act->ant;
            cant++;
        }
    }

    return cant;
}

void ordenarListaD(tListaD *p, int (*cmp)(const void*,const void*))
{
    tNodoD *act=*p,
           *sup=NULL,
           *inf=NULL;

    size_t tam;
    void *menor;
    int marca=1;

    if(act==NULL)
        return;

    while(act->ant)
        act=act->ant;

    while(marca)
    {
        marca=0;
        while(act->sig !=sup)
        {
            if(cmp(act->info,act->sig->info)>0)
            {
                menor=act->info;
                tam=act->tamInfo;

                marca=1;
                act->info=act->sig->info;
                act->sig->info=menor;
                act->tamInfo=act->sig->tamInfo;
                act->sig->tamInfo=tam;
            }
            act=act->sig;
        }
        sup=act;

        while(act->ant !=inf)
        {
            if(cmp(act->info,act->ant->info)<0)
            {
                menor=act->info;
                tam=act->tamInfo;

                marca=1;

                act->info=act->ant->info;
                act->ant->info=menor;
                act->tamInfo=act->ant->tamInfo;
                act->ant->tamInfo=tam;
            }
            act=act->ant;
        }
        inf=act;
    }
}

int ponerOrdListaD(tListaD *p, const void* pd,size_t tam,int (*cmp)(const void*,const void*))
{
    tNodoD *auxSig=*p,
           *auxAnt=auxSig? auxSig->ant : NULL,
           *nue;

    while(auxSig && cmp(pd,auxSig->info)>0)
    {
        auxAnt=auxSig;
        auxSig=auxSig->sig;
    }

    while(auxAnt && cmp(pd,auxAnt->info)<0)
    {
        auxSig=auxAnt;
        auxAnt=auxAnt->ant;
    }

    if( (auxSig && cmp(pd,auxSig->info)==0) || (auxAnt && cmp(pd,auxAnt->info)==0))
    {
        return 0; //DUPLICADO
    }

    if((nue= (tNodoD*)malloc(sizeof(tNodoD)))==NULL || (nue->info= malloc(tam))==NULL)
    {
        free(nue);
        return 0; // SIN MEMORIA
    }

    memcpy(nue->info,pd,tam);
    nue->tamInfo=tam;

    nue->sig=auxSig;
    nue->ant=auxAnt;
    if(auxSig)
        auxSig->ant=nue;
    if(auxAnt)
        auxAnt->sig=nue;

    *p=nue;

    return 1;
}

int elimOrdListaD(tListaD *p,void* pd,size_t tam,int (*cmp)(const void*,const void*))
{
    tNodoD *elim=*p;

    if(!elim)
        return 0; //LISTA VACIA

    while(elim->sig && cmp(pd,elim->info)>0)
    {
        elim=elim->sig;
    }

    while(elim->ant&& cmp(pd,elim->info)<0)
    {
        elim=elim->ant;
    }

    if(cmp(pd,elim->info)!=0)
        return -1;

    if(elim->ant)
         elim->ant->sig=elim->sig;

    if(elim->sig)
        elim->sig->ant=elim->ant;


    if(*p==elim)
        *p=elim->sig? elim->sig : elim->ant;

    memcpy(pd,elim->info,tam);

    free(elim->info);
    free(elim);

    return 1;
}
