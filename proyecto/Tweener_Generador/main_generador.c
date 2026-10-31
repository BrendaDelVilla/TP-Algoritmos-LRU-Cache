#include <stdio.h>
#include <stdlib.h>
#include "generador_lotes.h"

// 50.000 posteos y un mínimo de 200 registros (usuarios) solicitados
#define CANT_POSTEOS 50000
#define CANT_USUARIOS_LOTE 500

int main() {
    unsigned int semilla;

    printf("Ingrese la semilla para la generacion de lotes: ");
    scanf("%u", &semilla);

    srand(semilla);

    generar_lote_ordenado("lote_ordenado.dat", CANT_USUARIOS_LOTE);
    generar_lote_disperso("lote_disperso.dat", CANT_USUARIOS_LOTE, CANT_POSTEOS);
    generar_lote_concentrado("lote_concentrado.dat", CANT_USUARIOS_LOTE, CANT_POSTEOS);

    generar_archivo_maestro("posteos.dat", CANT_POSTEOS);

    printf("Archivo maestro y lotes de prueba generados exitosamente con la semilla %u.\n", semilla);

    return 0;
}
