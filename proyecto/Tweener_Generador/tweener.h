#ifndef TWEENER_H_INCLUDED
#define TWEENER_H_INCLUDED

// Estructura: {id: int, texto: char[141]}
typedef struct {
    int id;
    char texto[141];
} t_posteo;

// Estructura: {usuario: char[15], feed: int[20]}
typedef struct {
    char usuario[15];
    int feed[20];
} t_usuario;

#endif // TWEENER_H_INCLUDED
