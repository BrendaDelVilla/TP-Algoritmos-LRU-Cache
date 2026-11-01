#include "simulador.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../Tweener_Generador/tweener.h"
#include "../Tweener_Generador/generador_lotes.h"

#include "../TDA t_lru_cache/lru_simple.h"
#include "../TDA t_lru_cache_abb/lru_cache_abb.h"

#define TOTAL_USUARIOS 500
#define POSTEOS_POR_FEED 20
#define TOTAL_POSTEOS_LEIDOS (TOTAL_USUARIOS * POSTEOS_POR_FEED)

int cmp_int(const void *a, const void *b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return ia - ib;
}

void simular_sin_cache(const char *archivo_usuarios, FILE *fp_posteos) {
    FILE *fp_usuarios = fopen(archivo_usuarios, "rb");
    if (!fp_usuarios) return;

    t_usuario u;
    t_posteo p;
    clock_t inicio, fin;
    int *p_feed;

    inicio = clock();
    // Usamos la función de lectura de tu compañero
    while (leer_siguiente_usuario(fp_usuarios, &u) == 1) {
        // Aritmética de punteros: p_feed itera desde la dirección base de u.feed hasta el límite
        for (p_feed = u.feed; p_feed < u.feed + POSTEOS_POR_FEED; p_feed++) {
            // Desreferenciamos el puntero para obtener el ID (*p_feed)
            leer_posteo_por_id(fp_posteos, *p_feed, &p);
        }
    }
    fin = clock();
    fclose(fp_usuarios);

    double tiempo_total = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("   -> Tiempo Total: %.6f s | X_Usuario: %.6f s | X_Posteo: %.8f s\n",
           tiempo_total, tiempo_total / TOTAL_USUARIOS, tiempo_total / TOTAL_POSTEOS_LEIDOS);
    printf("   -> Lecturas a disco: %d | Aciertos: 0 | Fallos: %d\n", TOTAL_POSTEOS_LEIDOS, TOTAL_POSTEOS_LEIDOS);
}

void simular_cache_simple(const char *archivo_usuarios, FILE *fp_posteos, int capacidad) {
    FILE *fp_usuarios = fopen(archivo_usuarios, "rb");
    if (!fp_usuarios) return;

    tLru_cache *cache = crear_lrucache(capacidad, cmp_int);
    t_usuario u;
    t_posteo p;
    t_posteo *ptr_posteo;
    int aciertos = 0, fallos = 0, lecturas = 0;
    clock_t inicio, fin;
    int *p_feed;

    inicio = clock();
    while (leer_siguiente_usuario(fp_usuarios, &u) == 1) {
        for (p_feed = u.feed; p_feed < u.feed + POSTEOS_POR_FEED; p_feed++) {
            // Obtenemos el id desreferenciando el puntero
            int id_buscado = *p_feed;
            ptr_posteo = (t_posteo *)obtener_lrucache(cache, &id_buscado);

            if (ptr_posteo) {
                aciertos++;
            } else {
                fallos++;
                lecturas++;
                leer_posteo_por_id(fp_posteos, id_buscado, &p);
                agregar_lrucache(cache, &id_buscado, sizeof(int), &p, sizeof(t_posteo));
            }
        }
    }
    fin = clock();

    fclose(fp_usuarios);
    vaciar_lrucache(cache);
    free(cache);

    double tiempo_total = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("   -> Tiempo Total: %.6f s | X_Usuario: %.6f s | X_Posteo: %.8f s\n",
           tiempo_total, tiempo_total / TOTAL_USUARIOS, tiempo_total / TOTAL_POSTEOS_LEIDOS);
    printf("   -> Lecturas a disco: %d | Aciertos: %d | Fallos: %d\n", lecturas, aciertos, fallos);
}

void simular_cache_abb(const char *archivo_usuarios, FILE *fp_posteos, int capacidad, int es_ordenado) {
    FILE *fp_usuarios = fopen(archivo_usuarios, "rb");
    if (!fp_usuarios) return;

    tLru_cache_abb *cache = crear_lru_cache_abb(capacidad, cmp_int);
    t_usuario u;
    t_posteo p;
    t_posteo *ptr_posteo;
    int aciertos = 0, fallos = 0, lecturas = 0;
    clock_t inicio, fin;
    int *p_feed;

    inicio = clock();
    while (leer_siguiente_usuario(fp_usuarios, &u) == 1) {
        for (p_feed = u.feed; p_feed < u.feed + POSTEOS_POR_FEED; p_feed++) {
            int id_buscado = *p_feed;
            ptr_posteo = (t_posteo *)obtener_lru_cache_abb(cache, &id_buscado);

            if (ptr_posteo) {
                aciertos++;
            } else {
                fallos++;
                lecturas++;
                leer_posteo_por_id(fp_posteos, id_buscado, &p);
                agregar_lru_cache_abb(cache, &id_buscado, sizeof(int), &p, sizeof(t_posteo));
            }
        }
    }
    fin = clock();

    if (es_ordenado) {
        printf("   -> Altura final del Arbol Indice: %u\n", altura_lru_cache_abb(cache));
    }

    fclose(fp_usuarios);
    vaciar_lru_cache_abb(cache);
    free(cache);

    double tiempo_total = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("   -> Tiempo Total: %.6f s | X_Usuario: %.6f s | X_Posteo: %.8f s\n",
           tiempo_total, tiempo_total / TOTAL_USUARIOS, tiempo_total / TOTAL_POSTEOS_LEIDOS);
    printf("   -> Lecturas a disco: %d | Aciertos: %d | Fallos: %d\n", lecturas, aciertos, fallos);
}

void ejecutar_bateria_pruebas(const char *nombre_lote, const char *archivo_usuarios, FILE *fp_posteos) {
    int capacidades[] = {50, 500, 1500};
    int num_capacidades = 3;
    int es_ordenado = (strstr(nombre_lote, "ORDENADO") != NULL);

    printf("=========================================================\n");
    printf(" INICIANDO PRUEBAS: %s\n", nombre_lote);
    printf("=========================================================\n");

    printf("\n[1] ACCESO DIRECTO A DISCO (SIN CACHE)\n");
    simular_sin_cache(archivo_usuarios, fp_posteos);

    for (int i = 0; i < num_capacidades; i++) {
        printf("\n[2] LRU CACHE SIMPLE (Parte 1) - Capacidad: %d\n", capacidades[i]);
        simular_cache_simple(archivo_usuarios, fp_posteos, capacidades[i]);
    }

    for (int i = 0; i < num_capacidades; i++) {
        printf("\n[3] LRU CACHE ABB (Parte 2) - Capacidad: %d\n", capacidades[i]);
        simular_cache_abb(archivo_usuarios, fp_posteos, capacidades[i], es_ordenado);
    }
    printf("\n");
}
