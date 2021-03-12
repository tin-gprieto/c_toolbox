#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>
#include <stdlib.h>

#define ERROR -1
#define EXITO 0


/*
 * Condicion del heap. Recibe dos elementos (padre-hijo) del heap y devuelve
 * verdadero si el primer elemento cumple la condicion sobre el segundo.
 * Es decir es menor en el caso de un heap minimal o mayor en el caso de
 * un heap maximal.
 */
typedef bool (*heap_condicion)(void*, void*);

/*
 * Destructor de elementos. Cada vez que un elemento deja el heap
 * se invoca al destructor pasandole el elemento.
 */
typedef void (*heap_destructor)(void*);

typedef struct heap{
    void** vector;
    size_t tope;
    heap_condicion condicion;
    heap_destructor destructor;
}heap_t;

/*
*Crea un heap en la memoria dinámica (debe ser destruido) a partir
*de una condicion (obligatoria) y un destructor de elementos (opcional)
* Pre : Función condición del heap 
* Post: Heap creado
*/
heap_t* heap_crear(heap_condicion condicion, heap_destructor destructor);

/*
* Destruye el heap junto a todos los elementos que tenga
* Pre : Heap creado
* Post: Memoria reservada a todo el heap liberada
*/
void heap_destruir(heap_t* heap);

/*
* Devuelve el elemento que se encuentra en la raíz
* Pre : Heap creado
* Post: El elemento en la raíz o NULL en caso de que no haya raíz
*/
void* heap_raiz(heap_t* heap);

/*
* Pre : Heap creado
* Post: Verdadero si el heap está vacío - Falso si tiene elementos
*/
bool heap_vacio(heap_t* heap);

/*
* Devuelve la cantidad de elementos que hay en el heap
* Pre : Heap creado
* Post: Cantidad de elementos
*/
size_t heap_elementos(heap_t* heap);

/*
* Inserta un elemento en el heap donde se cumpla la condición
* Pre : Heap creado y puntero a elemento pasado como parámetro
* Post: EXITO(0) en caso de que se inserte correctamente - ERROR(-1) si no se pudo insertar
*/
int heap_insertar(heap_t* heap, void* elemento);

/*
* Elimina la raíz del heap dejandolo válido según su relación padre-hijo
* Pre : Heap creado
* Post: EXITO(0) en caso de que se elimine correctamente - ERROR(-1) si no se pudo borrar 
*/
int heap_borrar(heap_t* heap);



#endif /* __HEAP_H__ */