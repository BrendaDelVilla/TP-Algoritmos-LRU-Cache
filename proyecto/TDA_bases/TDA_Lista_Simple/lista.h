#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdlib.h>
#include <string.h>

typedef struct sNodo {
    void *info;
    size_t tamInfo;
    struct sNodo *sig;
} tNodo;

typedef tNodo *tLista;


void crearLista (tLista *p) ;
int listaVacia (const tLista *p) ;
int listaLlena (const tLista *p, size_t cantBytes) ;
void vaciarLista (tLista *p) ;


int ponerAlComienzo (tLista *p, const void *d, size_t cantBytes) ;
int sacarPrimeroLista (tLista *p, void *d, size_t cantBytes) ;
int verPrimeroLista (const tLista *p, void *d, size_t cantBytes);


int ponerAlFinal (tLista *p, const void *d, size_t cantBytes) ;
int sacarUltimoLista (tLista *p, void *d, size_t cantBytes) ;
int verUltimoLista (const tLista *p, void *d, size_t cantBytes) ;


void mapp(tLista *p,void (*action)( void*, void*),void *parametro);//=recorrerLista
void filter(tLista *p,int (*filtro)(void*,void*),void *parametro);
void reduce(tLista *p,void *result,void (*action) (void*,void*,void*),void *parametro);

void reinsert_nodo_menor(tLista *p,tLista *men);
void intercambio_info_nodos(tLista *n1,tLista *n2);
tNodo ** buscar_menor_nodo(const tLista *p,int (*cmp)(const void*,const void*));
void ordenar_sel_lista(tLista *p,int (*cmp)(const void*,const void*));

int poner_orden_lista(tLista *p,const void* pd,size_t tam,int (*cmp)(const void*,const void*));
int poner_pos_lista(tLista *p, const void* pd,size_t tam, unsigned pos);
int sacar_elem_lista(tLista *p,void* pd,size_t tam,int (*cmp)(const void*,const void*)); // a diferencia de FILTER(que recorre todo)
                                                                                                //esta funcion lo hace HASTA encontrar el UNICO ELEMENTO y lo elimina
int sacar_elem_ord_lista(tLista *p,void* pd,size_t tam,int (*cmp)(const void*,const void*));



void eliminarDuplicadoListaOrdenada(tLista *p,
                                    int (*cmp)(const void*,const void*),
                                    void (*action)(void*,const void*));

#endif // LISTA_H_INCLUDED
