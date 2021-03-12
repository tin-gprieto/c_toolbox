#ifndef __COLA_H__
#define __COLA_H__

#include <stdbool.h>
#include <stddef.h>

typedef struct nodo{
    void* elemento;
    struct nodo* siguiente;
}nodo_t;

typedef struct cola{
    nodo_t* nodo_inicio;
    nodo_t* nodo_fin;
    size_t cantidad;
}cola_t;

/*
 * Crea una cola reservando la memoria necesaria.
 * Devuelve un puntero a una cola creada o NULL en caso de error.
 */
cola_t* cola_crear();

/* 
 * Encola un elemento.
 * Devuelve 0 si pudo encolar o -1 si no pudo.
 */
int cola_encolar(cola_t* cola, void* elemento);

/* 
 * Desencola un elemento.
 * Devuelve 0 si pudo desencolar o -1 si no pudo.
 */
int cola_desencolar(cola_t* cola);

/*
 * Devuelve el primer elemento de la cola o NULL en caso de estar
 * vacía.
 */
void* cola_primero(cola_t* cola);

/* 
 * Devuelve true si la cola está vacía o false en caso contrario.
 */
bool cola_vacia(cola_t* cola);

/*
 * Devuelve la cantidad de elementos almacenados en la cola.
 */
size_t cola_elementos(cola_t* cola);

/*
 * Libera la memoria reservada por la cola.
 */
void cola_destruir(cola_t* cola);

#endif /* __COLA_H__ */