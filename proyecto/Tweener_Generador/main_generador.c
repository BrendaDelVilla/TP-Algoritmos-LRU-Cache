#include <stdio.h>
#include <stdlib.h>
#include "generador_lotes.h"
#include "simulador.h" // Vinculamos las funciones de simulación

#define CANT_POSTEOS 50000
#define CANT_USUARIOS_LOTE 500

int main() {
    unsigned int semilla;

    // --- FASE 1: GENERACIÓN DE DATOS ---
    printf("Ingrese la semilla para la generacion de lotes: ");
    scanf("%u", &semilla);

    srand(semilla);

    generar_lote_ordenado("lote_ordenado.dat", CANT_USUARIOS_LOTE);
    generar_lote_disperso("lote_disperso.dat", CANT_USUARIOS_LOTE, CANT_POSTEOS);
    generar_lote_concentrado("lote_concentrado.dat", CANT_USUARIOS_LOTE, CANT_POSTEOS);
    generar_archivo_maestro("posteos.dat", CANT_POSTEOS);

    printf("Archivo maestro y lotes de prueba generados exitosamente con la semilla %u.\n\n", semilla);

    // --- FASE 2: SIMULACIÓN LRU CACHE ---
    printf("Iniciando mediciones del Simulador LRU Cache...\n");

    // Abrimos el archivo maestro recién generado
    FILE *fp_posteos = fopen("posteos.dat", "rb");
    if (!fp_posteos) {
        printf("Error: No se pudo abrir posteos.dat para la simulacion.\n");
        return 1;
    }

    // Ejecutamos las pruebas consumiendo los archivos locales
    ejecutar_bateria_pruebas("LOTE CONCENTRADO", "lote_concentrado.dat", fp_posteos);
    ejecutar_bateria_pruebas("LOTE DISPERSO",    "lote_disperso.dat",    fp_posteos);
    ejecutar_bateria_pruebas("LOTE ORDENADO",    "lote_ordenado.dat",    fp_posteos);

    fclose(fp_posteos);

    return 0;
}
