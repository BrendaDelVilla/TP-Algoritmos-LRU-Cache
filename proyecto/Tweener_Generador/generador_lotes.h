#ifndef GENERADOR_LOTES_H_INCLUDED
#define GENERADOR_LOTES_H_INCLUDED

#include "tweener.h"

// generación de lotes y archivo binario que almacena la totalidad de estructuras t_posteo generados, representa la 'base de datos' de la red social
void generar_lote_ordenado(const char* filename, int cant_usuarios);
void generar_lote_disperso(const char* filename, int cant_usuarios, int max_posts);
void generar_lote_concentrado(const char* filename, int cant_usuarios, int max_posts);
void generar_archivo_maestro(const char* filename, int cant_posts);

// funciones de lectura
int leer_siguiente_usuario(FILE* pf, t_usuario* usuario_leido);
int leer_posteo_por_id(FILE* pf_maestro, int id, t_posteo* post_leido);

#endif // GENERADOR_LOTES_H_INCLUDED
