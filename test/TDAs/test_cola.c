
#include "../../code/TDAs/cola/cola.h"
#include "../../code/tools/testing.h"
#include "test_cola.h"

#define ERROR -1

const int elementos_cola[] = {1, 2, 3, 4, 5, 6};
const size_t cant_elementos_cola = 6;

int inicializar_cola_con_varios_elementos(cola_t * cola, int (*insercion)(cola_t *, void *), int* elementos, size_t cantidad){
    size_t i = 0;
    int estado = 0;
    while ((i < cantidad) && (estado == 0)){
        estado = insercion(cola, (void *)(elementos + i));
        i++;
    }
    return estado;
}

void cola_pruebas_basicas(){
    cola_t *cola = cola_crear();

    test_nuevo_sub_grupo("Encolar");
    int estado_cola = inicializar_cola_con_varios_elementos(cola, cola_encolar, (int*)elementos_cola, cant_elementos_cola);
    test_afirmar(estado_cola == 0, "Se encoló varios elementos");
    test_afirmar((cola_elementos(cola) == cant_elementos_cola), "Se insertó la cantidad adecuada");

    test_nuevo_sub_grupo("Desencolar");
    int * primer_elemento = (int*) cola_primero(cola);
    test_afirmar(cola_desencolar(cola) == 0, "Se desencola un elemento");
    test_afirmar(cola_primero(cola) != primer_elemento, "Se desencoló el primer elemento");

    test_nuevo_sub_grupo("Desencolar y vaciar");
    while(cola_elementos(cola) != 0){
        cola_desencolar(cola);
    }
    test_afirmar(cola_elementos(cola) == 0, "La cola se desencola por completo y queda vacia");
    test_afirmar(cola_desencolar(cola) == ERROR, "No se puede desencolar una cola vacia");

    cola_destruir(cola);
}

void cola_pruebas_creacion(){
    test_nuevo_sub_grupo("Operaciones de creación");
    cola_t * cola= cola_crear();
    test_afirmar(cola != NULL, "Se crea una cola");
    test_afirmar(cola_vacia(cola), "La cola está vacia");
    test_afirmar(cola_elementos(cola) == 0, "Tiene 0 elementos");
    test_afirmar(cola_primero(cola) == NULL, "No existe un primer elemento");
    cola_destruir(cola);
}

void pruebas_cola(){
    test_nuevo_grupo("PRUEBAS DE COLA");
    cola_pruebas_creacion();
    cola_pruebas_basicas();
}