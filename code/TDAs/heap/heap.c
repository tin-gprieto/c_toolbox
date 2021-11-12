#include "heap.h"

#define VACIO 0
#define RAIZ 0
#define NINGUNO 0
#define DERECHO 1
#define IZQUIERDO -1

/*
* Intercabia la posicion de dos elementos en el vector
* Pre : posicion de los elementos válidos dentro del vector
* Post: elementos de las posiciones elem_1 y elem_2 intecambiados de lugar
*/
void swap(void** vector, size_t elem_1, size_t elem_2){
    if(!vector) return;
    void* aux = vector[elem_1];
    vector[elem_1] = vector[elem_2];
    vector[elem_2] =aux;
}
/*
* Posicion del padre según posicion
* Pre : posicion válida para el heap
* Post: posicion del padre dentro del vector heap
*/
size_t pos_padre(size_t posicion){
    return (posicion-1)/2;
}
/*
* Posicion del hijo derecho según posicion
* Pre : posicion válida para el heap
* Post: posicion del hijo derecho dentro del vector heap
*/
size_t pos_hijo_derecho(size_t posicion){
    return (2*posicion) + 1;
}
/*
* Posicion del hijo izquierdo según posicion
* Pre : posicion válida para el heap
* Post: posicion del hijo izquierdo dentro del vector heap
*/
size_t pos_hijo_izquierdo(size_t posicion){
    return (2*posicion) + 2;
}
/*
* Desde la posicion chequea que el padre cumplan 
* con la condicion en relacion a sus hijos, sino los intercambia
* Pre : Heap creado 
* y primer llamado con la última posicion ocupada del vector heap
* Post: Heap ordenado y válido según la condicion
*/
void sift_up(heap_t* heap, size_t posicion){
    if(!heap || posicion == RAIZ) return;
    if(!(heap->condicion(heap->vector[pos_padre(posicion)], heap->vector[posicion]))){
        swap(heap->vector, posicion, pos_padre(posicion));
        sift_up(heap, pos_padre(posicion));
    }
    return;
}

/*
* Busca cual de los dos hijos (en caso de tenerlos) 
* deberia intecambiarse con el padre
* Pre : Heap creado, posicion del padre dentro del vector heap (válido para el mismo)
* Post: DERECHO/IZQUIERDO si debe cambiarse alguno de los hijos o NINGUNO si no hay intercambio
*/
int hijo_incorrecto(heap_t* heap, size_t pos, heap_condicion condicion){
    if(!heap || pos == heap->tope) return NINGUNO;

    void* padre = heap->vector[pos];
    void* hijo_derecho = NULL;
    void* hijo_izquierdo = NULL;
    
    if(pos_hijo_derecho(pos) < heap->tope)
        hijo_derecho = heap->vector[pos_hijo_derecho(pos)];
    if(pos_hijo_izquierdo(pos) < heap->tope)
        hijo_izquierdo = heap->vector[pos_hijo_izquierdo(pos)];

    if(hijo_derecho && hijo_izquierdo){
        if(!condicion(padre, hijo_derecho) && !condicion(padre, hijo_izquierdo)){
            if(condicion(hijo_derecho, hijo_izquierdo))
                return DERECHO;
            return IZQUIERDO;
        }
    }

    if(hijo_derecho && !condicion(padre, hijo_derecho))
        return DERECHO;

    if(hijo_izquierdo && !condicion(padre, hijo_izquierdo))
        return IZQUIERDO;

    return NINGUNO;
}
/*
* Desde de la posición chequea que los hijos cumplan la condicion
* en relación al padre, sino los intercambia
* Pre :  Heap creado, primer llamado con la posición 0 (RAIZ)
* Post: Heap ordenado y válido según la condicion
*/
void sift_down(heap_t* heap, size_t posicion){
    if(!heap || posicion == heap->tope) return;
    int hijo = hijo_incorrecto(heap, posicion, heap->condicion);
    if(hijo == DERECHO){
        swap(heap->vector, posicion, pos_hijo_derecho(posicion));
        sift_down(heap,pos_hijo_derecho(posicion));
    }else if(hijo == IZQUIERDO){
        swap(heap->vector,  posicion, pos_hijo_izquierdo(posicion));
        sift_down(heap,pos_hijo_izquierdo(posicion));
    }
    return;
}

//heap.h
heap_t* heap_crear(heap_condicion condicion, heap_destructor destructor){
    if(!condicion) return NULL;
    heap_t* aux = malloc(sizeof(heap_t));
    if(!aux) return NULL;
    aux->condicion = condicion;
    aux->destructor = destructor;
    aux->vector = NULL;
    aux->tope = VACIO;
    return aux;
}
//heap.h
bool heap_vacio(heap_t* heap){
    if(heap) return !(heap->tope);
    return true;
}
//heap.h
void heap_destruir(heap_t* heap){
    if(!heap) return;
    if(!heap_vacio(heap)){
        if(heap->destructor)
            for(int i = 0; i < heap->tope; i++)
                heap->destructor(heap->vector[i]); 
        free(heap->vector);
    }
    free(heap);
}
//heap.h
void* heap_raiz(heap_t* heap){
    if(heap && !heap_vacio(heap)) return heap->vector[RAIZ];
    return NULL;
}
//heap.h
int heap_insertar(heap_t* heap, void* elemento){
    if(!heap) return ERROR;
    void** aux = realloc(heap->vector, (sizeof(void**) * (heap->tope + 1)));
    if(!aux) return ERROR;
    heap->vector = aux;
    heap->vector[heap->tope] = elemento;
    heap->tope++;
    sift_up(heap, heap->tope - 1);
    return EXITO;
}
//heap.h
int heap_borrar(heap_t* heap){
    if(!heap || heap_vacio(heap))
        return ERROR;
    if(heap->destructor)
        heap->destructor(heap->vector[RAIZ]);
    if(heap->tope > 1){
        swap(heap->vector, RAIZ, (heap->tope - 1));
        void** aux = realloc(heap->vector, (sizeof(void*)*(heap->tope - 1)));
        if(!aux) return ERROR;
        heap->vector = aux;
        heap->tope --;  
        sift_down(heap, RAIZ); 
    }else{
        free(heap->vector);
        heap->tope --;
    }
    return EXITO;
}
//heap.h
size_t heap_elementos(heap_t* heap){
    if(heap)
        return heap->tope;
    return VACIO;
}