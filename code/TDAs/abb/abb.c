#include "abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define ERROR -1
#define EXITO 0
#define DERECHA 1
#define IZQUIERDA -1
#define COMPARADOR 0
#define ERROR_PUNTERO NULL

#define RAIZ           1
#define REEMPLAZO      0
#define NO_ENCONTRADO -1
#define ERROR         -1

typedef struct array{
    void** vector;
    size_t tamanio;
    size_t posicion;
} array_t;

typedef struct familia{
    nodo_abb_t* nodo_padre;
    nodo_abb_t* nodo;
    int descendencia;
} familia_t;

/*
*Pre : Nodo no nulo 
*Post: Verdadero si no tiene hijos
*/
bool es_hoja(nodo_abb_t* nodo){
    if(!nodo) return true; 
    if ((!nodo->izquierda) && (!nodo->derecha))
        return true;
    return false;
}
/*
*Pre : Arbol no nulo y con raiz (al menos un elemento)
*Post: Veradero si el elemento coincide con el elemento de
la raíz del árbol
*/
bool es_raiz(abb_t *arbol, void * elemento){
    if(!arbol || !arbol->nodo_raiz) return false;
    return(arbol->nodo_raiz->elemento == elemento); 
}
/*
* Dado un nodo y su destructor, destruye el elemento
y borra el nodo
* Pre : Nodo no nulo y destructor existente
* Post: Nodo destruido
*/
void nodo_abb_liberar(nodo_abb_t* nodo_borrar, abb_liberar_elemento destructor){
    if(nodo_borrar && destructor)
        destructor(nodo_borrar->elemento);
    free(nodo_borrar);
}

/*
*Destruye todos los nodos del arbol junto con sus elementos
*Pre : En la primera llamada el nodo debe ser la raíz
*Post: Se destruyen todos los nodos del arbol junto con sus elementos
*/
void destruir_nodos_abb(abb_t* arbol, nodo_abb_t* nodo){
    if(!nodo)
        return;
    if(nodo->izquierda)
        destruir_nodos_abb(arbol, nodo->izquierda);
    if(nodo->derecha)
        destruir_nodos_abb(arbol, nodo->derecha);
    nodo_abb_liberar(nodo, arbol->destructor);
}

/*
*Inserta un nuevo nodo según corresponda con el comparador
*Pre : nuevo_nodo previamente creado en el heap e inicializado, primera llamada con
nodo_raiz como corriente
*Post: Devuelve EXITO si lo inserta correctamente o ERROR en caso contrario
*/
int nodo_abb_insertar(abb_comparador comparador, nodo_abb_t* nuevo_nodo, nodo_abb_t* corriente){
    if(!corriente)
        return ERROR;
    int comparacion = comparador(nuevo_nodo->elemento, corriente->elemento);
    if(comparacion > COMPARADOR || comparacion == COMPARADOR){
        if(!(corriente->derecha))
            corriente->derecha = nuevo_nodo;
        else
            nodo_abb_insertar(comparador, nuevo_nodo, corriente->derecha);
    }else if (comparacion < COMPARADOR){
        if (!(corriente->izquierda))
            corriente->izquierda = nuevo_nodo;
        else
            nodo_abb_insertar(comparador, nuevo_nodo, corriente->izquierda);
    } 
    return EXITO;
}
/*
*Se inicializa un nodo como hoja junto con su elemento
*Pre : Nodo no nulo 
*Post: Nodo hoja con elemento
*/
void nodo_abb_inicializar(nodo_abb_t* nodo, void* elemento){
    if(!nodo)
        return;
    nodo->elemento = elemento; 
    nodo->izquierda = NULL;
    nodo->derecha = NULL;
}
abb_t *arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    if(!comparador) 
        return ERROR_PUNTERO;
    abb_t* arbol = malloc(sizeof(abb_t));
    if(!arbol) 
        return ERROR_PUNTERO;
    arbol->nodo_raiz = NULL;
    arbol->comparador = comparador;
    arbol->destructor = destructor;
    return arbol;
}

/*
*Busca un nodo dado un elemento
*Pre : debe existir comparador y la primera llamda debe ser con la raíz del arbol
*Post: nodo con el elemento o NULL si no lo encuentra
*/
nodo_abb_t* nodo_abb_buscar(abb_comparador comparador, nodo_abb_t *corriente, void *elemento){
    if(!corriente)
        return NULL;
    int comparacion = comparador(elemento, corriente->elemento);
    if (comparacion > COMPARADOR)
        return nodo_abb_buscar(comparador, corriente->derecha, elemento);
    else if (comparacion < COMPARADOR)
        return nodo_abb_buscar(comparador, corriente->izquierda, elemento);
    return corriente;
}
int arbol_insertar(abb_t* arbol, void* elemento){
    if(!arbol)
        return ERROR;
    nodo_abb_t* nuevo_nodo = malloc(sizeof(nodo_abb_t));
    if(!nuevo_nodo)
        return ERROR;
    nodo_abb_inicializar(nuevo_nodo, elemento);
    if(!arbol->nodo_raiz){
        arbol->nodo_raiz = nuevo_nodo;
        return EXITO;
    }
    return nodo_abb_insertar(arbol->comparador, nuevo_nodo, arbol->nodo_raiz);
}
/*
* Borra una raiz con hijos
* Pre : NO puede ser hoja
* Post: Reemplzo como raiz del árbol y heredando los hijos de la raíz
*/
int nodo_raiz_borrar(abb_t* arbol, familia_t borrado, familia_t reemplazo){
    if (borrado.nodo != reemplazo.nodo_padre){
        //hijos menores del reemplazo son hijos mayores del padre
        reemplazo.nodo_padre->derecha = reemplazo.nodo->izquierda;
        //hereda sus hijos
        reemplazo.nodo->derecha = borrado.nodo->derecha;
        reemplazo.nodo->izquierda = borrado.nodo->izquierda;
    }else if(reemplazo.descendencia == IZQUIERDA){
        reemplazo.nodo->derecha = borrado.nodo->derecha;
    }
    arbol->nodo_raiz = reemplazo.nodo;
    nodo_abb_liberar(borrado.nodo, arbol->destructor);
    return EXITO;
}

/*
*Según la descendencia del nodo borrado (si es hijo izquierdo o derecho),
*el nodo reemplazado pasa a ser el hijo del padre del borrado.
*Pre : reemplazo y borrado (con descendencia y padre) buscados 
*Post: el reemplazo ocupa el lugar del borrado
*/
void heredar_padre(familia_t borrado, familia_t reemplazo){
    if (borrado.descendencia == IZQUIERDA)
        borrado.nodo_padre->izquierda = reemplazo.nodo;
    if (borrado.descendencia == DERECHA)
        borrado.nodo_padre->derecha = reemplazo.nodo;
}
/*
* Borra un nodo que tiene hijo y no es raiz
* Pre : NO puede ser hoja ni raiz que
* Post: Reemplazo en la posicion del borrado
*/
int nodo_abb_borrar(familia_t borrado, familia_t reemplazo, abb_liberar_elemento destructor){
    if (borrado.nodo == reemplazo.nodo_padre){
        //El reemplazado es su hijo
        if(reemplazo.descendencia == IZQUIERDA){
            //Es su hijo izquierdo, no tiene hijos menores
            if(borrado.nodo->derecha) //Si tiene hijo mayor, lo hereda el reemplazo
                reemplazo.nodo->derecha = borrado.nodo->derecha;
            heredar_padre(borrado, reemplazo); 
        }
        else if (reemplazo.descendencia == DERECHA)
            heredar_padre(borrado, reemplazo);
    }else{
        //hijos menores del reemplazo son hijos mayores del padre
        reemplazo.nodo_padre->derecha = reemplazo.nodo->izquierda;
        heredar_padre(borrado, reemplazo);
        //hereda sus hijos
        reemplazo.nodo->derecha = borrado.nodo->derecha;
        reemplazo.nodo->izquierda = borrado.nodo->izquierda;
    }
    nodo_abb_liberar(borrado.nodo, destructor);
    return EXITO;
}
/*
* Borra una hoja (no tiene hijos)
* Pre : El nodo borrado debe ser hoja y pertenecer al árbol
* Post: puntero del padre nulo y 0 ya que lo borra
*/
int nodo_hoja_borrar(familia_t borrado, abb_t* arbol){
    if(es_raiz(arbol, borrado.nodo->elemento)){
        arbol->nodo_raiz = NULL;
        nodo_abb_liberar(borrado.nodo, arbol->destructor);
        return EXITO;
    }
    if(borrado.descendencia == DERECHA)
        borrado.nodo_padre->derecha = NULL;
    if(borrado.descendencia == IZQUIERDA)
        borrado.nodo_padre->izquierda = NULL;
    nodo_abb_liberar(borrado.nodo, arbol->destructor);
    return 0;
}
/*
*Pre : Padre no nulo y con al menos un hijo
*Post: Devuelve verdadero si algunos de los hijos del padre
tiene como elemento al hijo.
*/
bool es_padre(nodo_abb_t * padre, void* hijo){
    if(!padre) return false;
    if(padre->izquierda)
        if(padre->izquierda->elemento == hijo) return true;
    if(padre->derecha)
        if(padre->derecha->elemento == hijo) return true;
    return false;
}
/*
*Buscar el padre de un nodo que contenga al elemento
*Pre : La priemra llamada a la función debe ser el nodo_raiz como corriente
*Post: Devuelve el nodo del padre o NULL en caso de que no haya 
(Si no hay padre, tampoco hay nodo)
*/
nodo_abb_t* nodo_abb_padre(abb_comparador comparador, nodo_abb_t* corriente, void* elemento){
    if (!corriente)
        return NULL;
    if(es_padre(corriente, elemento))
        return corriente;
    int comparacion = comparador(elemento, corriente->elemento);
    if (comparacion > COMPARADOR)
        return nodo_abb_padre(comparador, corriente->derecha, elemento);
    else if (comparacion < COMPARADOR)
        return nodo_abb_padre(comparador, corriente->izquierda, elemento);
    return NULL;
}
/*
*Dado un padre, determina cual es el hijo que tiene el elemento y de que lado está
* Pre : El padre debe tener algún hijo que contenda el elemento pasado por parámetro
* Post: Devuelve el lado en el que se encuentra el hijo (DERECHA / IZQUIERDA) o ERROR
en el caso de que el elemento no sea el de alguno de sus hijos o que no tenga hijos
*/
int determinar_descendecia(nodo_abb_t* padre, nodo_abb_t** hijo, void* elemento){
    if(padre->derecha){
        if(padre->derecha->elemento == elemento){
            *hijo = padre->derecha;
            return DERECHA;
        }
    }if(padre->izquierda){
        if(padre->izquierda->elemento == elemento){
            *hijo = padre->izquierda;
            return IZQUIERDA;
        }
    }
    return ERROR;
}
/*
*Dado un árbol y un elemento, busca el elemento pasado por parametro junto a su padre
*Pre : El elemento debe ser el elemento que se quiera eliminar y que pertenezca al árbol
*Post: Devuelve RAIZ si el elemento se encuentra en la raiz, REEMPLAZO si requiere un
nodo que lo reemplace o ERROR si el elemento no se encuentra en el árbol
*/
int buscar_borrado(abb_t *arbol, familia_t* borrado, void* elemento){
    if(es_raiz(arbol, elemento)){
        borrado->nodo = arbol->nodo_raiz;
        borrado->nodo_padre = NULL;
        return RAIZ;
    }
    borrado->nodo_padre = nodo_abb_padre(arbol->comparador, arbol->nodo_raiz, elemento);
    if (!borrado->nodo_padre) //No existe elemento en árbol
        return ERROR;
    borrado->descendencia = determinar_descendecia(borrado->nodo_padre, &(borrado->nodo), elemento);
    return REEMPLAZO;
}
/*
*Dado un nodo, busca el mayor de sus hijos.
*Pre : el primer nodo debe ser el hijo menor del borrado
*Post: nodo del predecesor inorden
*/
nodo_abb_t* nodo_abb_buscar_predecesor_inorden(nodo_abb_t* hijo){
    if(!hijo->derecha) //es el mayor de los predecesores
        return hijo;
    return nodo_abb_buscar_predecesor_inorden(hijo->derecha);
}
/*
*Dado un nodo que se que deba borrar, busca su reemplazo junto al padre del mismo
*Pre : el elemento borrado debe estar en el árbol
*Post: Devuelve el nodo reemplazante, con su padre y la descendencia 
*/
familia_t buscar_reemplazo(abb_t *arbol, nodo_abb_t* borrado){
    familia_t aux;
    if(es_hoja(borrado)){
        aux.nodo = NULL;
        aux.nodo_padre =NULL;
        return aux;
    }
    if(!borrado->izquierda){  //no tiene hijos menores
        aux.nodo = borrado->derecha;
        aux.nodo_padre = borrado;
        aux.descendencia = DERECHA;
        return aux;
    } 
    aux.nodo = nodo_abb_buscar_predecesor_inorden(borrado->izquierda);
    aux.nodo_padre = nodo_abb_padre(arbol->comparador, arbol->nodo_raiz, aux.nodo->elemento);
    aux.descendencia = determinar_descendecia(aux.nodo_padre, &(aux.nodo), aux.nodo->elemento);
    return aux;
}
int arbol_borrar(abb_t *arbol, void *elemento){
    if(!arbol || !arbol->nodo_raiz || !elemento)
        return ERROR;
    familia_t borrado; 
    int estado = buscar_borrado(arbol, &borrado, elemento);
    if(estado == ERROR)
        return ERROR;
    familia_t reemplazo = buscar_reemplazo(arbol, borrado.nodo);
    if(!reemplazo.nodo)
        return nodo_hoja_borrar(borrado, arbol);
    if (estado == RAIZ)
        return nodo_raiz_borrar(arbol, borrado, reemplazo);
    return nodo_abb_borrar(borrado, reemplazo, arbol->destructor);
}
void * arbol_buscar(abb_t *arbol, void *elemento){
    if((!arbol) || (!elemento)) 
        return ERROR_PUNTERO;
    nodo_abb_t *nodo_buscado = nodo_abb_buscar(arbol->comparador, arbol->nodo_raiz, elemento);
    if(!nodo_buscado)
        return NULL;
    return nodo_buscado->elemento;
}
void * arbol_raiz(abb_t *arbol){
    if((!arbol) || (!arbol->nodo_raiz))
        return ERROR_PUNTERO;
    return arbol->nodo_raiz->elemento;
}
bool arbol_vacio(abb_t *arbol){
    if (!arbol) return true;
    return !(arbol->nodo_raiz); 
}
/*
*Pre : Array previamente inicializado
*Post: Devuelve verdadero ya hizo su última insercion
*
*/
bool ultima_insercion(array_t array){
    return (array.posicion == array.tamanio);
}
/*
* Dado un elemento, lo inserta al final del array en el caso de que
no esté completo
* Pre : el array pasado por paramento debe ser del struct array_t
* Post: Verdadero si está completo, Falso si no está completo y cargó el elemento
*/
bool insertar_en_array(void* elemento, void* array){
    ((array_t*)array)->vector[((array_t*)array)->posicion] = elemento;
    (((array_t*)array)->posicion)++;
    if(ultima_insercion(*(array_t*)array)) return true;
    return false;
}
/*
* Recorre el árbol en inorden y ejecuta la funcion mientras se falsa
* Pre : termina debe ser falso en la primera llamada 
        el contador debe estar inicializado
        el corriente debe ser la raiz del árbol en la primera llamada
* Post: Funcion aplicada a cada elemento mientras ésta sea falsa
*/
void recorrer_inorden(nodo_abb_t* corriente, bool (*funcion)(void*, void*), void* extra, bool *termina, size_t* contador){
    if(!corriente || (*termina))
        return;

    if(corriente->izquierda) recorrer_inorden(corriente->izquierda, funcion, extra, termina, contador);
    if((*termina))
        return; 

    (*termina) = funcion(corriente->elemento, extra);
    (*contador)++;
    
    if(corriente->derecha) recorrer_inorden(corriente->derecha, funcion, extra, termina, contador);
}
/*
* Recorre el árbol en preorden y ejecuta la funcion mientras se falsa
* Pre : termina debe ser falso en la primera llamada 
        el contador debe estar inicializado
        el corriente debe ser la raiz del árbol en la primera llamada
* Post: Funcion aplicada a cada elemento mientras ésta sea falsa
*/
void recorrer_preorden(nodo_abb_t* corriente, bool (*funcion)(void*, void*), void* extra, bool *termina, size_t* contador){
    if (!corriente || (*termina))
        return;

    if((*termina))
        return;
    (*termina) = funcion(corriente->elemento, extra);
    (*contador)++;
    
    if(corriente->izquierda) recorrer_preorden(corriente->izquierda, funcion, extra, termina, contador);
    if(corriente->derecha)recorrer_preorden(corriente->derecha, funcion, extra, termina, contador);
}
    
/*
* Recorre el árbol en postorden y ejecuta la funcion mientras se falsa
* Pre : termina debe ser falso en la primera llamada 
        el contador debe estar inicializado
        el corriente debe ser la raiz del árbol en la primera llamada
* Post: Funcion aplicada a cada elemento mientras ésta sea falsa
*/
void recorrer_postorden(nodo_abb_t* corriente, bool (*funcion)(void*, void*), void* extra, bool *termina, size_t* contador){
    if (!corriente || (*termina))
        return;
    if(corriente->izquierda) recorrer_postorden(corriente->izquierda, funcion, extra, termina, contador);
    if(corriente->derecha)recorrer_postorden(corriente->derecha, funcion, extra, termina, contador);
    
    if((*termina))
        return;
    (*termina) = funcion(corriente->elemento, extra);
    (*contador)++;
}
/*
* Dado un vector genérico (vacío) y su tamanio, inicializar un array_t
* Pre : array previamente creado y vacio
* Post: Devuelve un array_t inicializado (primera posicion = 0)
*/
array_t inicializar_array(void** array, size_t tamanio){
    array_t aux;
    aux.vector = array;
    aux.tamanio = tamanio;
    aux.posicion = 0;
    return aux;
}
/*
* Dado un árcbol, un vector genérico (vacío) y su tamanio, y una función de recorrido,
* recorre el árbol llenando el vector hasta su tamanio o hasta que se recorra completamente
* Pre : árbol creado con su comparador, función recorrido del estilo de inorde, preorden o postorden;
* Post: Cantidad de elementos recorridos 
*/
size_t recorrer_arbol(abb_t *arbol, void **array, size_t tamanio_array, void (*recorrido)(nodo_abb_t*, bool(*)(void*, void*), void*, bool*, size_t*)){
    if (!arbol || !arbol->nodo_raiz || tamanio_array <= 0)
        return 0;
    array_t vector = inicializar_array(array, tamanio_array);
    bool termina = false;
    size_t contador = 0;
    recorrido(arbol->nodo_raiz, insertar_en_array, &vector, &termina, &contador);
    return contador ;
}
size_t arbol_recorrido_inorden(abb_t *arbol, void **array, size_t tamanio_array){
    return recorrer_arbol(arbol, array, tamanio_array, recorrer_inorden);
}
size_t arbol_recorrido_preorden(abb_t *arbol, void **array, size_t tamanio_array){
    return recorrer_arbol(arbol, array, tamanio_array, recorrer_preorden);
}
size_t arbol_recorrido_postorden(abb_t *arbol, void **array, size_t tamanio_array){
    return recorrer_arbol(arbol, array, tamanio_array, recorrer_postorden);
}
void arbol_destruir(abb_t *arbol){
    if(!arbol) return;
    if (arbol->nodo_raiz) 
        destruir_nodos_abb(arbol, arbol->nodo_raiz);
    free(arbol);

}
/*
*Pre : Recorrido recibido por parametro
*Post: Devuelve verdadero si el recorrido pasado por parámetro
cumple con alguna de las constantes
*/
bool recorrido_valido(int recorrido){
    return (recorrido == ABB_RECORRER_INORDEN || 
            recorrido == ABB_RECORRER_PREORDEN || 
            recorrido == ABB_RECORRER_POSTORDEN);
}
size_t abb_con_cada_elemento(abb_t *arbol, int recorrido, bool (*funcion)(void *, void *), void *extra){
    if(!arbol || !arbol->nodo_raiz || !funcion || !recorrido_valido(recorrido))
        return 0;
    bool termina = false;
    size_t cant_iterada = 0;
    if(recorrido == ABB_RECORRER_INORDEN)
        recorrer_inorden(arbol->nodo_raiz, funcion, extra, &termina, &cant_iterada);
    if (recorrido == ABB_RECORRER_PREORDEN)
        recorrer_preorden(arbol->nodo_raiz, funcion, extra, &termina, &cant_iterada);
    if (recorrido == ABB_RECORRER_POSTORDEN)
        recorrer_postorden(arbol->nodo_raiz, funcion, extra, &termina, &cant_iterada);
    return cant_iterada;
}
