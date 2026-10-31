#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generador_lotes.h"

void generar_lote_ordenado(const char* filename, int cant_usuarios) {
    FILE* pf;
    int i, j;
    int id_global;
    t_usuario f_user;
    char aux_user[30];

    id_global = 1;
    pf = fopen(filename, "wb");
    if (pf) {
        for (i = 0; i < cant_usuarios; i++) {
            // Uso de buffer auxiliar para evitar warning de overflow
            sprintf(aux_user, "user_%d", i + 1);
            strncpy(f_user.usuario, aux_user, 14);
            f_user.usuario[14] = '\0';

            for (j = 0; j < 20; j++) {
                f_user.feed[j] = id_global;
                id_global++;
            }
            fwrite(&f_user, sizeof(t_usuario), 1, pf);
        }
        fclose(pf);
    }
}

void generar_lote_disperso(const char* filename, int cant_usuarios, int max_posts) {
    FILE* pf;
    int i, j;
    t_usuario f_user;
    char aux_user[30];

    pf = fopen(filename, "wb");
    if (pf) {
        for (i = 0; i < cant_usuarios; i++) {
            sprintf(aux_user, "user_%d", i + 1);
            strncpy(f_user.usuario, aux_user, 14);
            f_user.usuario[14] = '\0';

            for (j = 0; j < 20; j++) {
                f_user.feed[j] = (rand() % max_posts) + 1;
            }
            fwrite(&f_user, sizeof(t_usuario), 1, pf);
        }
        fclose(pf);
    }
}

void generar_lote_concentrado(const char* filename, int cant_usuarios, int max_posts) {
    FILE* pf;
    int i, j;
    int viral_pool;
    t_usuario f_user;
    char aux_user[30];

    viral_pool = 100; // Pool reducido para simular repeticiones
    pf = fopen(filename, "wb");
    if (pf) {
        for (i = 0; i < cant_usuarios; i++) {
            sprintf(aux_user, "user_%d", i + 1);
            strncpy(f_user.usuario, aux_user, 14);
            f_user.usuario[14] = '\0';

            for (j = 0; j < 20; j++) {
                // 80% de chances de pedir un post del pool viral
                if (rand() % 100 < 80) {
                    f_user.feed[j] = (rand() % viral_pool) + 1;
                } else {
                    f_user.feed[j] = (rand() % max_posts) + 1;
                }
            }
            fwrite(&f_user, sizeof(t_usuario), 1, pf);
        }
        fclose(pf);
    }
}

void generar_archivo_maestro(const char* filename, int cant_posts) {
    FILE* pf;
    t_posteo post;
    int i;
    char aux_texto[150];

    pf = fopen(filename, "wb");
    if (pf) {
        for (i = 1; i <= cant_posts; i++) {
            post.id = i;
            sprintf(aux_texto, "Contenido de prueba para el posteo numero %d.", i);
            strncpy(post.texto, aux_texto, 140);
            post.texto[140] = '\0';

            fwrite(&post, sizeof(t_posteo), 1, pf);
        }
        fclose(pf);
    }
}

int leer_siguiente_usuario(FILE* pf, t_usuario* usuario_leido) {
    int estado;
    estado = 0; // 0 indica error o fin de archivo (EOF)

    if (pf) {
        if (fread(usuario_leido, sizeof(t_usuario), 1, pf) == 1) {
            estado = 1; // 1 indica que se leyó correctamente TODO_BIEN
        }
    }

    return estado;
}

int leer_posteo_por_id(FILE* pf_maestro, int id, t_posteo* post_leido) {
    int estado;
    long desplazamiento;

    estado = 0;

    if (pf_maestro && id > 0) {
        /* Como nuestros IDs arrancan en 1, el ID 1 está en la posición 0,
           el ID 2 en la posición 1, etc. Multiplicamos por el tamaño del struct */
        desplazamiento = (id - 1) * sizeof(t_posteo);

        // Saltamos directo a ese byte usando SEEK_SET (desde el principio del archivo)
        fseek(pf_maestro, desplazamiento, SEEK_SET);

        if (fread(post_leido, sizeof(t_posteo), 1, pf_maestro) == 1) {
            estado = 1;
        }
    }

    return estado;
}
