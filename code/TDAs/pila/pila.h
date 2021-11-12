#ifndef __PILA_H__
#define __PILA_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct nodo_pila{
    void* elemento;
    struct nodo_pila* siguiente;
}nodo_pila_t;

typedef struct pila{
    nodo_pila_t* nodo_inicio;
    nodo_pila_t* nodo_fin;
    size_t cantidad;
}pila_t;

/*
 * Crea una pila reservando la memoria necesaria.
 * Devuelve un puntero a la pila creada o NULL en caso de error.
 */
pila_t* pila_crear();

/* 
 * Apila un elemento.
 * Devuelve 0 si pudo o -1 en caso contrario.
 */
int pila_apilar(pila_t* pila, void* elemento);

/* 
 * Desapila un elemento.
 * Devuelve 0 si pudo desapilar o -1 si no pudo.
 */
int pila_desapilar(pila_t* pila);

/*
 * Devuelve el elemento en el tope de la pila o NULL
 * en caso de estar vacía.
 */
void* pila_tope(pila_t* pila);

/* 
 * Devuelve true si la pila está vacía o false en caso contrario.
 */
bool pila_vacia(pila_t* pila);

/*
 * Devuelve la cantidad de elementos almacenados en la pila.
 */
size_t pila_elementos(pila_t* pila);

/*
 * Libera la memoria reservada por la pila.
 */
void pila_destruir(pila_t* pila);

#endif /* __PILA_H__ */