#include "pila.h"

/*
*Devuelve el puntero al nodo que se encuentre en la posicion 
*pasada por parametro
* Pre: Posicion que esté dentro de la pila
* Post: Elemento en posicion o NULL si no lo encuentra
*/
nodo_t* nodo_en_posicion(nodo_t* nodo, size_t contador, size_t posicion){
    if(!nodo)
        return NULL;
    if(contador == posicion)
        return nodo;
    return nodo_en_posicion(nodo->siguiente, contador + 1, posicion);
}

/*
*Reasigna los nodos para que quede al inicio de la pila
* Pre: nuevo_nodo y pila creados
* Post: nuevo_nodo es el nuevo nodo_inicio y su siguiente es el nodo_inicio anterior
*/
void anclar_al_inicio(pila_t* pila, nodo_t* nuevo_nodo){
    if(pila && nuevo_nodo){
        nuevo_nodo->siguiente = pila->nodo_inicio;
        pila->nodo_inicio = nuevo_nodo;
    }
}

/*
*Reasigna los nodos para que quede al final de la pila
* Pre: nuevo_nodo creado
* Post: nodo_final y anterior nodo_final apuntando al nuevo_nodo
*/
void anclar_al_final(pila_t* pila, nodo_t* nuevo_nodo){
    if (pila && nuevo_nodo){
        (pila->nodo_fin)->siguiente = nuevo_nodo;
        pila->nodo_fin = nuevo_nodo;
    }
}

/*
*Crea e inserta un elemento en posicion reemplazando 
el lugar del que lo ocupaba anteriormente
* Pre: pila creada y la posicion no puede ser ni la primera ni la última
* Post: Elemento en posicion y entre dos nodos de la pila
*/
int crear_e_insertar_entre_nodos(pila_t * pila, void* elemento, size_t posicion){
    if(!pila) 
        return ERROR;
    if((posicion == 0)||(posicion == pila->cantidad))
        return ERROR;
    nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
    if (!nuevo_nodo)
        return ERROR;
    nuevo_nodo->elemento = elemento;

    nodo_t *antecesor = nodo_en_posicion(pila->nodo_inicio, 0, (posicion-1));
    nodo_t *reemplazado = antecesor->siguiente;
    antecesor->siguiente = nuevo_nodo;
    nuevo_nodo->siguiente = reemplazado;

    pila->cantidad++;
    return 0;
}
/*
*Crea e inserta un elemento al inicio de la pila
* Pre: pila creada
* Post: Elemento insertado al inicio de la pila, en el caso de
*ser el primer elemento insertado también es el último.
*/
int crear_e_insertar_al_inicio(pila_t* pila, void* elemento){
    if(!pila)
        return ERROR;
    nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
    if (!nuevo_nodo)
        return ERROR;
    nuevo_nodo->elemento = elemento;
    anclar_al_inicio(pila, nuevo_nodo);
    if(pila->cantidad == 0)
        pila->nodo_fin = nuevo_nodo;
    pila->cantidad++;
    return 0;
}

/*
*Crea un nuevo nodo y lo inserta al final de la pila
*(si la pila no tiene elementos, lo inserta al inicio)
* Pre: pila creada
* Post: Elemento insertado al final de la pila
*/
int crear_e_insertar_al_final(pila_t* pila, void* elemento){
    if (!pila)
        return ERROR;
    nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
    if (!nuevo_nodo)
        return ERROR;
    nuevo_nodo->elemento = elemento;
    nuevo_nodo->siguiente = NULL;
    anclar_al_final(pila, nuevo_nodo);
    pila->cantidad++;
    return 0;    
}

/*
*Borra el elemento en posicion
* Pre: pila creada y posicion de un elemento que pertenezca
* Post: Elemento en posicion borrado y lugar ocupado por el siguiente
*/
void borrar_en_posicion(pila_t* pila, size_t posicion){
    if ((!pila) || (posicion == 0))
        return;
    nodo_t *antecesor = nodo_en_posicion(pila->nodo_inicio, 0, posicion - 1);
    nodo_t *borrado = antecesor->siguiente;
    antecesor->siguiente = borrado->siguiente;
    free(borrado);
    pila->cantidad--;
}

/*
*Borra el ultimo elemento de la pila
* Pre: pila/Pila creada y con más de un elemento
* Post: Borra el ultimo elemento y deja como tope o nodo_fin a su antecesor
*/
void borrar_ultimo_elemento(pila_t* pila){
    if (!pila)
        return;
    size_t ultima_posicion= pila->cantidad - 1;
    nodo_t *antecesor = nodo_en_posicion(pila->nodo_inicio, 0, ultima_posicion - 1);
    free(antecesor->siguiente);
    antecesor->siguiente = NULL;
    pila->nodo_fin = antecesor;
    pila->cantidad--;
}

/*
*Borra el primer elemento de la pila y el segundo pasa a ser el primero
* Pre: pila/Pila creada y con un elemento o Cola creada 
* Post: Primer elemento borrado y su siguiente como nuevo primer elemento
*/
void borrar_primer_elemento(pila_t* pila){
    if ((!pila) || (pila->cantidad == 0))
        return;
    nodo_t *aux;
    aux = (pila->nodo_inicio)->siguiente;
    free(pila->nodo_inicio);
    pila->nodo_inicio = aux;
    pila->cantidad--;
    if(pila->cantidad == 0)
        pila->nodo_fin = NULL;
}

/*
*Borra el ultimo elemento de la pila
(si la pila tiene un solo elemento, borra el primero)
* Pre: pila creada 
* Post: Último elemento eliminado
*/
int pila_borrar_al_final(pila_t * pila){
    if ((!pila) || (pila->cantidad == 0))
        return ERROR;
    if (pila->cantidad == 1){
        borrar_primer_elemento(pila);
    }else{
        borrar_ultimo_elemento(pila);
    }
    return 0;
}

/*
*Recorre todos los nodos mientras se cumpla la funcion
o no se termine la pila enlazada
* Pre: nodo_inicio perteneciente a una pila
* Post: cantidad de elementos recorridos y funcion ejecutada 
*con cada elemento hasta que corte
*/
size_t mover_por_nodos(nodo_t *nodo, bool (*funcion)(void *, void *), void *contexto){
    if (!nodo)
        return 0;
    if (!funcion(nodo->elemento, contexto))
        return 0;
    return 1 + mover_por_nodos(nodo->siguiente, funcion, contexto);
}

/*
*Destruye todos los nodos que esten unidos hasta que el último apunte a NULL
* Pre: nodo_inicio perteneciente a la pila para destruir
* Post: Todos los nodo liberados de la memoria dinamica
*/
void nodo_destruir(nodo_t* nodo){
    if(!nodo) 
        return;
    nodo_destruir(nodo->siguiente);
    free(nodo);
}

/***********************    FUNCIONES PILA.H    **********************/

pila_t* pila_crear(){
    pila_t* pila = malloc(sizeof(pila_t));
    if(!pila)
        return NULL;
    pila->nodo_inicio = NULL;
    pila->nodo_fin = NULL;
    pila->cantidad = 0;
    return pila;
}
bool pila_vacia(pila_t* pila){
    return (!pila || pila->cantidad == 0); 
}

size_t pila_elementos(pila_t* pila){
    if (!pila)
        return 0;
    return pila->cantidad;
}

int pila_apilar(pila_t* pila, void* elemento){
    if(!pila) 
        return ERROR;
    if (pila->cantidad == 0)
        return crear_e_insertar_al_inicio(pila, elemento);
    return crear_e_insertar_al_final(pila, elemento);
}

int pila_desapilar(pila_t* pila){
    return pila_borrar_al_final(pila);
}

void* pila_tope(pila_t* pila){
    if (!pila)
        return NULL;
    return (pila->nodo_fin)->elemento;
}

void pila_destruir(pila_t* pila){
    if (!pila)
        return;
    if(pila->cantidad != 0)
        nodo_destruir(pila->nodo_inicio);
    free(pila);
}
