# TP-Algoritmos-LRU-Cache

🚀 TDA LRU Cache - Algoritmos y Estructuras de Datos



Repositorio oficial del Trabajo Práctico "LRU Cache" para la materia Algoritmos y Estructuras de Datos de la Universidad Nacional de La Matanza (UNLaM) - Comisión 01-1900.



📝 Descripción del Proyecto



Este proyecto implementa un Tipo de Dato Abstracto (TDA) que simula una memoria caché con LRU (Least Recently Used). El objetivo es validar la eficacia de esta caché en el feed de "Tweener", una red social simulada

El trabajo incluye dos versiones de la caché para comparar su rendimiento:



Caché Simple: Implementada con una lista simplemente enlazada genérica.



Caché Optimizada: Implementada combinando una lista doblemente enlazada (para orden de uso) y un árbol binario de búsqueda (como índice por clave).



🗂️ Estructura del Repositorio



El desarrollo se dividió en módulos independientes y una carpeta de bibliotecas compartidas (DRY) para facilitar el trabajo en equipo:



/tdas\_base: Bibliotecas genéricas compartidas (Lista Simple, Lista Doblemente Enlazada).



/TDA t\_lru\_cache: Archivos .h y .c de la caché con lista simple (Parte 1).



/TDA t\_lru\_cache2: Archivos .h y .c de la caché con lista doble y árbol binario (Parte 2).



/datos\_generador: Motor de creación de los archivos binarios de prueba (Lotes Concentrado, Disperso y Ordenado).



/simulador: Motor que consume los datos y mide las métricas de rendimiento con clock().



⚙️ Requisitos y Compilación



El proyecto está desarrollado en C y preparado para compilarse utilizando el compilador MinGW.



Para ejecutarlo:



Cloná este repositorio localmente (podés usar GitHub Desktop o la terminal).



Abrí tu proyecto de módulo en Code::Blocks o el main.c del simulador.



Asegurate de incluir en el Build target tanto los archivos .c de tu módulo como los archivos .c necesarios de la carpeta /tdas\_base.



Compilá y ejecutá.



👥 Integrantes del Equipo



\[Brenda Villa] 



\[Sídney Maribel Gamarro Bravo] 



\[Tobias Delmonaco] 



\[Santiago Ivan Codina] 

