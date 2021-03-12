#include "../toolbox/tools.h"
#include "../toolbox/tdas/heap.h"

#define VACIO 0
#define MAX_VECTOR 20

bool heap_minimal(void* padre, void* hijo){
    return (*(int*)padre) < (*(int*)hijo);
}

void destructor_dinamico(void* elemento){
    free (elemento);
}

int insertar_vector(heap_t* heap, int* vector, size_t tope){
    size_t i = 0;
    int estado = EXITO;
    while((i<tope) && (estado == EXITO)){
        estado = heap_insertar(heap, (void*)&vector[i]);
        i++;
    }
    return estado;
}

int borrar_vector(heap_t* heap, int* vector, size_t* tope){
    int estado = EXITO;
    while(!heap_vacio(heap) && (estado == EXITO)){
        vector[(*tope)] = *(int*)heap_raiz(heap);
        (*tope)++;
        estado = heap_borrar(heap);
    }
    return estado;
}

void pruebas_avanzadas(){
    test_nuevo_grupo("Pruebas avanzadas");
    heap_t* heap = heap_crear(heap_minimal, NULL);
    int vector[MAX_VECTOR] = {4, 7, 8, 2, 5, 6, 1, 3, 9, 0};
    int ordenado[MAX_VECTOR] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t tope = 10;
    int heapsort[MAX_VECTOR];
    size_t nuevo_tope = 0;

    int insercion = insertar_vector(heap, vector, tope);
    test_afirmar(insercion == EXITO, "Se insertaron correctamente varios elementos");
    int raiz = *(int*)heap_raiz(heap);
    test_afirmar(raiz == vector[9], "La raíz es el menor de los elementos");
    
    int borrado = borrar_vector(heap, heapsort, &nuevo_tope);
    test_afirmar(comparar_vectores(heapsort, ordenado, tope), "Se realizó el heapsort correctamente");
    test_afirmar(borrado == EXITO, "Se pudo borrar todo el heap de a un elemento");
    test_afirmar(tope == nuevo_tope, "Se eliminaron la cantidad correcta de elementos");
    test_afirmar(heap_vacio(heap), "El heap se encuentra vacío");
    test_afirmar(heap_elementos(heap) == VACIO, "El heap queda con 0 elementos");
    test_afirmar(heap_raiz(heap) == NULL, "El heap no tiene raíz");
    
    heap_destruir(heap);
}

void pruebas_basicas(){
    test_nuevo_grupo("Pruebas básicas");
    heap_t* heap = heap_crear(heap_minimal, NULL);
    int numero = 23;
    void* elemento = &numero;
  
    test_afirmar(heap_insertar(heap, elemento) == EXITO, "Se inserta un elemento");
    test_afirmar(!heap_vacio(heap), "El heap no está vacio");
    test_afirmar(heap_elementos(heap) == 1, "El heap tiene un elemento");
    test_afirmar(heap_raiz(heap) == elemento, "El elemento insertado es la raíz");
 
    test_afirmar(heap_borrar(heap) == EXITO, "Se borra el único elemento (raíz)");
    test_afirmar(heap_vacio(heap), "El heap queda vacio");
    test_afirmar(heap_elementos(heap) == VACIO, "El heap queda con 0 elementos");
    test_afirmar(heap_raiz(heap) == NULL, "El heap no tiene raíz");
    test_afirmar(heap_borrar(heap) == ERROR, "No se puede borrar en un heap vacío");
   
    heap_destruir(heap);
}

void pruebas_creacion(){
    test_nuevo_grupo("Pruebas creación");
    test_afirmar(heap_crear(NULL, NULL) == NULL, "No se crea un heap sin ninguna funcion");
    test_afirmar(heap_crear(NULL, destructor_dinamico) == NULL, "No se crea un heap sin una condicion");
    heap_t* sin_destructor = heap_crear(heap_minimal, NULL);
    test_afirmar( sin_destructor != NULL, "Se crea un heap unicamente con la condicion");
    heap_destruir(sin_destructor);
    heap_t* completo = heap_crear(heap_minimal, destructor_dinamico);
    test_afirmar( completo != NULL, "Se crea un heap con ambas funciones");
    test_afirmar(heap_vacio(completo), "El heap creado está vacio");
    test_afirmar(heap_elementos(completo) == VACIO, "El heap creado tiene 0 elementos");
    test_afirmar(heap_raiz(completo) == NULL, "El heap no tiene raíz");
    heap_destruir(completo);
}

int main(){
    pruebas_creacion();
    pruebas_basicas();
    pruebas_avanzadas();    
    test_mostrar_reporte();
}