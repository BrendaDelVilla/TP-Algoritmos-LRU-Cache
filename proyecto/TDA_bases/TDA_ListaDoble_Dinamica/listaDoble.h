#ifndef LISTADOBLE_H_INCLUDED
#define LISTADOBLE_H_INCLUDED

#include <stddef.h>
typedef struct sNodoD
{
    void *info;
    size_t tamInfo;
    struct sNodoD *sig,
                  *ant;
}tNodoD;

typedef tNodoD *tListaD;


void crearListaD(tListaD *p);
int vaciarListaD(tListaD *p);
int listaDVacia(const tListaD *p);
int listaDllena(const tListaD *p,size_t tam);
int insertarAlFinal(tListaD *p,const void *pd,size_t tam);
int insertarAlComienzo(tListaD *p,const void *pd,size_t tam);
int mostrarDeIzqADer(const tListaD *p, void (*mostrar)(const void*));
int mostrarDeDerAIzq(const tListaD *p, void (*mostrar)(const void*));
void ordenarListaD(tListaD *p, int (*cmp)(const void*,const void*));


int ponerOrdListaD(tListaD *p, const void* pd,size_t tam,int (*cmp)(const void*,const void*));
int elimOrdListaD(tListaD *p,void* pd,size_t tam,int (*cmp)(const void*,const void*));

#endif // LISTADOBLE_H_INCLUDED
