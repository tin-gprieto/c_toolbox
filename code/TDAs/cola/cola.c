#include "cola.h"

#define ERROR -1

/*
*Devuelve el puntero al nodo que se encuentre en la posicion 
*pasada por parametro
* Pre: Posicion que esté dentro de la cola
* Post: Elemento en posicion o NULL si no lo encuentra
*/
nodo_cola_t* cola_nodo_en_posicion(nodo_cola_t* nodo, size_t contador, size_t posicion){
    if(!nodo)
        return NULL;
    if(contador == posicion)
        return nodo;
    return cola_nodo_en_posicion(nodo->siguiente, contador + 1, posicion);
}

/*
*Reasigna los nodos para que quede al inicio de la cola
* Pre: nuevo_nodo y cola creados
* Post: nuevo_nodo es el nuevo nodo_inicio y su siguiente es el nodo_inicio anterior
*/
void cola_anclar_al_inicio(cola_t* cola, nodo_cola_t* nuevo_nodo){
    if(cola && nuevo_nodo){
        nuevo_nodo->siguiente = cola->nodo_inicio;
        cola->nodo_inicio = nuevo_nodo;
    }
}

/*
*Reasigna los nodos para que quede al final de la cola
* Pre: nuevo_nodo creado
* Post: nodo_final y anterior nodo_final apuntando al nuevo_nodo
*/
void cola_anclar_al_final(cola_t* cola, nodo_cola_t* nuevo_nodo){
    if (cola && nuevo_nodo){
        (cola->nodo_fin)->siguiente = nuevo_nodo;
        cola->nodo_fin = nuevo_nodo;
    }
}

/*
*Crea e inserta un elemento en posicion reemplazando 
el lugar del que lo ocupaba anteriormente
* Pre: cola creada y la posicion no puede ser ni la primera ni la última
* Post: Elemento en posicion y entre dos nodos de la cola
*/
int cola_crear_e_insertar_entre_nodos(cola_t * cola, void* elemento, size_t posicion){
    if(!cola) 
        return ERROR;
    if((posicion == 0)||(posicion == cola->cantidad))
        return ERROR;
    nodo_cola_t *nuevo_nodo = malloc(sizeof(nodo_cola_t));
    if (!nuevo_nodo)
        return ERROR;
    nuevo_nodo->elemento = elemento;

    nodo_cola_t *antecesor = cola_nodo_en_posicion(cola->nodo_inicio, 0, (posicion-1));
    nodo_cola_t *reemplazado = antecesor->siguiente;
    antecesor->siguiente = nuevo_nodo;
    nuevo_nodo->siguiente = reemplazado;

    cola->cantidad++;
    return 0;
}
/*
*Crea e inserta un elemento al inicio de la cola
* Pre: cola creada
* Post: Elemento insertado al inicio de la cola, en el caso de
*ser el primer elemento insertado también es el último.
*/
int cola_crear_e_insertar_al_inicio(cola_t* cola, void* elemento){
    if(!cola)
        return ERROR;
    nodo_cola_t *nuevo_nodo = malloc(sizeof(nodo_cola_t));
    if (!nuevo_nodo)
        return ERROR;
    nuevo_nodo->elemento = elemento;
    cola_anclar_al_inicio(cola, nuevo_nodo);
    if(cola->cantidad == 0)
        cola->nodo_fin = nuevo_nodo;
    cola->cantidad++;
    return 0;
}

/*
*Crea un nuevo nodo y lo inserta al final de la cola
*(si la cola no tiene elementos, lo inserta al inicio)
* Pre: cola creada
* Post: Elemento insertado al final de la cola
*/
int cola_crear_e_insertar_al_final(cola_t* cola, void* elemento){
    if (!cola)
        return ERROR;
    nodo_cola_t *nuevo_nodo = malloc(sizeof(nodo_cola_t));
    if (!nuevo_nodo)
        return ERROR;
    nuevo_nodo->elemento = elemento;
    nuevo_nodo->siguiente = NULL;
    cola_anclar_al_final(cola, nuevo_nodo);
    cola->cantidad++;
    return 0;    
}

/*
*Borra el elemento en posicion
* Pre: cola creada y posicion de un elemento que pertenezca
* Post: Elemento en posicion borrado y lugar ocupado por el siguiente
*/
void cola_borrar_en_posicion(cola_t* cola, size_t posicion){
    if ((!cola) || (posicion == 0))
        return;
    nodo_cola_t *antecesor = cola_nodo_en_posicion(cola->nodo_inicio, 0, posicion - 1);
    nodo_cola_t *borrado = antecesor->siguiente;
    antecesor->siguiente = borrado->siguiente;
    free(borrado);
    cola->cantidad--;
}

/*
*Borra el ultimo elemento de la cola
* Pre: cola/Pila creada y con más de un elemento
* Post: Borra el ultimo elemento y deja como tope o nodo_fin a su antecesor
*/
void cola_borrar_ultimo_elemento(cola_t* cola){
    if (!cola)
        return;
    size_t ultima_posicion= cola->cantidad - 1;
    nodo_cola_t *antecesor = cola_nodo_en_posicion(cola->nodo_inicio, 0, ultima_posicion - 1);
    free(antecesor->siguiente);
    antecesor->siguiente = NULL;
    cola->nodo_fin = antecesor;
    cola->cantidad--;
}

/*
*Borra el primer elemento de la cola y el segundo pasa a ser el primero
* Pre: cola/Pila creada y con un elemento o Cola creada 
* Post: Primer elemento borrado y su siguiente como nuevo primer elemento
*/
void cola_borrar_primer_elemento(cola_t* cola){
    if ((!cola) || (cola->cantidad == 0))
        return;
    nodo_cola_t *aux;
    aux = (cola->nodo_inicio)->siguiente;
    free(cola->nodo_inicio);
    cola->nodo_inicio = aux;
    cola->cantidad--;
    if(cola->cantidad == 0)
        cola->nodo_fin = NULL;
}

/*
*Borra el ultimo elemento de la cola
(si la cola tiene un solo elemento, borra el primero)
* Pre: cola creada 
* Post: Último elemento eliminado
*/
int cola_borrar_al_final(cola_t * cola){
    if ((!cola) || (cola->cantidad == 0))
        return ERROR;
    if (cola->cantidad == 1){
        cola_borrar_primer_elemento(cola);
    }else{
        cola_borrar_ultimo_elemento(cola);
    }
    return 0;
}

/*
*Recorre todos los nodos mientras se cumpla la funcion
o no se termine la cola enlazada
* Pre: nodo_inicio perteneciente a una cola
* Post: cantidad de elementos recorridos y funcion ejecutada 
*con cada elemento hasta que corte
*/
size_t cola_mover_por_nodos(nodo_cola_t *nodo, bool (*funcion)(void *, void *), void *contexto){
    if (!nodo)
        return 0;
    if (!funcion(nodo->elemento, contexto))
        return 0;
    return 1 + cola_mover_por_nodos(nodo->siguiente, funcion, contexto);
}

/*
*Destruye todos los nodos que esten unidos hasta que el último apunte a NULL
* Pre: nodo_inicio perteneciente a la cola para destruir
* Post: Todos los nodo liberados de la memoria dinamica
*/
void cola_nodo_destruir(nodo_cola_t* nodo){
    if(!nodo) 
        return;
    cola_nodo_destruir(nodo->siguiente);
    free(nodo);
}

/**********************************     FUNCIONES COLA.H    *************************************/

cola_t* cola_crear(){
   cola_t* cola = malloc(sizeof(cola_t));
    if(!cola)
        return NULL;
    cola->nodo_inicio = NULL;
    cola->nodo_fin = NULL;
    cola->cantidad = 0;
    return cola;
}

bool cola_vacia(cola_t* cola){
    return (!cola || cola->cantidad == 0); 
}

size_t cola_elementos(cola_t* cola){
    if (!cola)
        return 0;
    return cola->cantidad;
}

int cola_encolar(cola_t* cola, void* elemento){
    if (!cola)
        return ERROR;
    if (cola->cantidad == 0)
        return cola_crear_e_insertar_al_inicio(cola, elemento);
    return cola_crear_e_insertar_al_final(cola, elemento);
}

int cola_desencolar(cola_t* cola){
    if ((!cola) || (cola->cantidad == 0))
        return ERROR;
    cola_borrar_primer_elemento(cola);
    return 0;
}

void* cola_primero(cola_t* cola){
    if((!cola) || (!cola->nodo_inicio)) 
        return NULL;
    return (cola->nodo_inicio)->elemento;
}

void cola_destruir(cola_t* cola){
    if (!cola)
        return;
    if(cola->cantidad != 0)
        cola_nodo_destruir(cola->nodo_inicio);
    free(cola);
}