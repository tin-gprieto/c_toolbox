#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define ERROR -1

/*
*Devuelve el puntero al nodo que se encuentre en la posicion 
*pasada por parametro
* Pre: Posicion que esté dentro de la lista
* Post: Elemento en posicion o NULL si no lo encuentra
*/
nodo_lista_t* lista_nodo_en_posicion(nodo_lista_t* nodo, size_t contador, size_t posicion){
    if(!nodo)
        return NULL;
    if(contador == posicion)
        return nodo;
    return lista_nodo_en_posicion(nodo->siguiente, contador + 1, posicion);
}

/*
*Reasigna los nodos para que quede al inicio de la lista
* Pre: nuevo_nodo y lista creados
* Post: nuevo_nodo es el nuevo nodo_inicio y su siguiente es el nodo_inicio anterior
*/
void lista_anclar_al_inicio(lista_t* lista, nodo_lista_t* nuevo_nodo){
    if(lista && nuevo_nodo){
        nuevo_nodo->siguiente = lista->nodo_inicio;
        lista->nodo_inicio = nuevo_nodo;
    }
}

/*
*Reasigna los nodos para que quede al final de la lista
* Pre: nuevo_nodo creado
* Post: nodo_final y anterior nodo_final apuntando al nuevo_nodo
*/
void lista_anclar_al_final(lista_t* lista, nodo_lista_t* nuevo_nodo){
    if (lista && nuevo_nodo){
        (lista->nodo_fin)->siguiente = nuevo_nodo;
        lista->nodo_fin = nuevo_nodo;
    }
}

/*
*Crea e inserta un elemento en posicion reemplazando 
el lugar del que lo ocupaba anteriormente
* Pre: lista creada y la posicion no puede ser ni la primera ni la última
* Post: Elemento en posicion y entre dos nodos de la lista
*/
int lista_crear_e_insertar_entre_nodos(lista_t * lista, void* elemento, size_t posicion){
    if(!lista) 
        return ERROR;
    if((posicion == 0)||(posicion == lista->cantidad))
        return ERROR;
    nodo_lista_t *nuevo_nodo = malloc(sizeof(nodo_lista_t));
    if (!nuevo_nodo)
        return ERROR;
    nuevo_nodo->elemento = elemento;

    nodo_lista_t *antecesor = lista_nodo_en_posicion(lista->nodo_inicio, 0, (posicion-1));
    nodo_lista_t *reemplazado = antecesor->siguiente;
    antecesor->siguiente = nuevo_nodo;
    nuevo_nodo->siguiente = reemplazado;

    lista->cantidad++;
    return 0;
}
/*
*Crea e inserta un elemento al inicio de la lista
* Pre: Lista creada
* Post: Elemento insertado al inicio de la lista, en el caso de
*ser el primer elemento insertado también es el último.
*/
int lista_crear_e_insertar_al_inicio(lista_t* lista, void* elemento){
    if(!lista)
        return ERROR;
    nodo_lista_t *nuevo_nodo = malloc(sizeof(nodo_lista_t));
    if (!nuevo_nodo)
        return ERROR;
    nuevo_nodo->elemento = elemento;
    lista_anclar_al_inicio(lista, nuevo_nodo);
    if(lista->cantidad == 0)
        lista->nodo_fin = nuevo_nodo;
    lista->cantidad++;
    return 0;
}

/*
*Crea un nuevo nodo y lo inserta al final de la lista
*(si la lista no tiene elementos, lo inserta al inicio)
* Pre: Lista creada
* Post: Elemento insertado al final de la lista
*/
int lista_crear_e_insertar_al_final(lista_t* lista, void* elemento){
    if (!lista)
        return ERROR;
    nodo_lista_t *nuevo_nodo = malloc(sizeof(nodo_lista_t));
    if (!nuevo_nodo)
        return ERROR;
    nuevo_nodo->elemento = elemento;
    nuevo_nodo->siguiente = NULL;
    lista_anclar_al_final(lista, nuevo_nodo);
    lista->cantidad++;
    return 0;    
}

/*
*Borra el elemento en posicion
* Pre: Lista creada y posicion de un elemento que pertenezca
* Post: Elemento en posicion borrado y lugar ocupado por el siguiente
*/
void lista_borrar_en_posicion(lista_t* lista, size_t posicion){
    if ((!lista) || (posicion == 0))
        return;
    nodo_lista_t *antecesor = lista_nodo_en_posicion(lista->nodo_inicio, 0, posicion - 1);
    nodo_lista_t *borrado = antecesor->siguiente;
    antecesor->siguiente = borrado->siguiente;
    free(borrado);
    lista->cantidad--;
}

/*
*Borra el ultimo elemento de la lista
* Pre: Lista/Pila creada y con más de un elemento
* Post: Borra el ultimo elemento y deja como tope o nodo_fin a su antecesor
*/
void lista_borrar_ultimo_elemento(lista_t* lista){
    if (!lista)
        return;
    size_t ultima_posicion= lista->cantidad - 1;
    nodo_lista_t *antecesor = lista_nodo_en_posicion(lista->nodo_inicio, 0, ultima_posicion - 1);
    free(antecesor->siguiente);
    antecesor->siguiente = NULL;
    lista->nodo_fin = antecesor;
    lista->cantidad--;
}

/*
*Borra el primer elemento de la lista y el segundo pasa a ser el primero
* Pre: Lista/Pila creada y con un elemento o Cola creada 
* Post: Primer elemento borrado y su siguiente como nuevo primer elemento
*/
void lista_borrar_primer_elemento(lista_t* lista){
    if ((!lista) || (lista->cantidad == 0))
        return;
    nodo_lista_t *aux;
    aux = (lista->nodo_inicio)->siguiente;
    free(lista->nodo_inicio);
    lista->nodo_inicio = aux;
    lista->cantidad--;
    if(lista->cantidad == 0)
        lista->nodo_fin = NULL;
}

/*
*Borra el ultimo elemento de la lista
(si la lista tiene un solo elemento, borra el primero)
* Pre: Lista creada 
* Post: Último elemento eliminado
*/
int lista_borrar_al_final(lista_t * lista){
    if ((!lista) || (lista->cantidad == 0))
        return ERROR;
    if (lista->cantidad == 1){
        lista_borrar_primer_elemento(lista);
    }else{
        lista_borrar_ultimo_elemento(lista);
    }
    return 0;
}

/*
*Recorre todos los nodos mientras se cumpla la funcion
o no se termine la lista enlazada
* Pre: nodo_inicio perteneciente a una lista
* Post: cantidad de elementos recorridos y funcion ejecutada 
*con cada elemento hasta que corte
*/
size_t lista_mover_por_nodos(nodo_lista_t *nodo, bool (*funcion)(void *, void *), void *contexto){
    if (!nodo)
        return 0;
    if (!funcion(nodo->elemento, contexto))
        return 0;
    return 1 + lista_mover_por_nodos(nodo->siguiente, funcion, contexto);
}

/*
*Destruye todos los nodos que esten unidos hasta que el último apunte a NULL
* Pre: nodo_inicio perteneciente a la lista para destruir
* Post: Todos los nodo liberados de la memoria dinamica
*/
void lista_nodo_destruir(nodo_lista_t* nodo){
    if(!nodo) 
        return;
    lista_nodo_destruir(nodo->siguiente);
    free(nodo);
}

//FUNCIONES .H

lista_t* lista_crear(){
    lista_t* lista = malloc(sizeof(lista_t));
    if(!lista)
        return NULL;
    lista->nodo_inicio = NULL;
    lista->nodo_fin = NULL;
    lista->cantidad = 0;
    return lista;
}

int lista_insertar(lista_t* lista, void* elemento){
    if(!lista) 
        return ERROR;
    if (lista->cantidad == 0)
        return lista_crear_e_insertar_al_inicio(&(*lista), elemento);
    return lista_crear_e_insertar_al_final(&(*lista), elemento);
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if (!lista)
        return ERROR;
    if ((posicion == 0) || (lista->cantidad == 0))
        return lista_crear_e_insertar_al_inicio(lista, elemento);
    if (lista->cantidad <= posicion)
        return lista_crear_e_insertar_al_final(lista, elemento);
    return lista_crear_e_insertar_entre_nodos(lista, elemento, posicion);
}

int lista_borrar(lista_t* lista){
    return lista_borrar_al_final(&(*lista));
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
    if((!lista) || (lista->cantidad == 0))
        return ERROR;
    if (posicion == 0){
        lista_borrar_primer_elemento(lista);
    }else  if(lista->cantidad - 1 <= posicion){
        lista_borrar_ultimo_elemento(lista);
    }else{
        lista_borrar_en_posicion(lista, posicion);
    }
    return 0;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if((!lista) || lista->cantidad <= posicion) 
        return NULL;
    nodo_lista_t* nodo_buscado = lista_nodo_en_posicion(lista->nodo_inicio, 0, posicion);     
    return nodo_buscado->elemento;
}

void* lista_ultimo(lista_t* lista){
    if ((!lista) || (!lista->nodo_fin))
        return NULL;
    return (lista->nodo_fin)->elemento;
}

bool lista_vacia(lista_t* lista){
    return (!lista || lista->cantidad == 0); 
}

size_t lista_elementos(lista_t* lista){
    if (!lista)
        return 0;
    return lista->cantidad;
}

void lista_destruir(lista_t* lista){
    if (!lista)
        return;
    if(lista->cantidad != 0)
        lista_nodo_destruir(lista->nodo_inicio);
    free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if(!lista) 
        return NULL;
    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    if(!iterador)
        return NULL;
    iterador->corriente = lista->nodo_inicio;
    iterador->lista = lista;
    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if (!(iterador->corriente) || !(iterador->lista))
        return false;
    return true;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if (!(iterador->lista) || !(iterador->corriente))
        return false;
    iterador->corriente = (iterador->corriente)->siguiente;
    if(!iterador->corriente)
        return false;
    return true;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if (!(iterador->corriente) || !(iterador->lista))
        return NULL;
    return (iterador->corriente)->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    if(iterador)
        free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    if(!lista || !funcion)
        return 0;
    return lista_mover_por_nodos(lista->nodo_inicio, funcion, contexto);
}
