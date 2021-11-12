#include "../../code/TDAs/pila/pila.h"
#include "../../code/tools/testing.h"
#include "test_pila.h"

#define ERROR -1

const int elementos_pila[] = {1, 2, 3, 4, 5, 6};
const size_t cant_elementos_pila = 6;

int inicializar_pila_con_varios_elementos(pila_t * pila, int (*insercion)(pila_t *, void *), int* elementos, size_t cantidad){
    size_t i = 0;
    int estado = 0;
    while ((i < cantidad) && (estado == 0)){
        estado = insercion(pila, (void *)(elementos + i));
        i++;
    }
    return estado;
}

void pila_pruebas_basicas(){
    test_nuevo_sub_grupo("OPERACIONES DE PILA");
    pila_t *pila = pila_crear();

    test_nuevo_sub_grupo("Apilar");
    int estado_pila = inicializar_pila_con_varios_elementos(pila, pila_apilar, (int*)elementos_pila, cant_elementos_pila);
    test_afirmar(estado_pila == 0, "Se apiló varios elementos");
    test_afirmar((pila_elementos(pila) == cant_elementos_pila), "Se insertó la cantidad adecuada");

    test_nuevo_sub_grupo("Desapilar");
    int * ultimo_elemento = (int *) pila_tope(pila);
    test_afirmar(pila_desapilar(pila) == 0,  "Se desapila un elemento");
    test_afirmar(pila_tope(pila) != ultimo_elemento, "Se desapiló el último elemento");

    test_nuevo_sub_grupo("Desapilar y vaciar");
    while(pila_elementos(pila) != 0){
        pila_desapilar(pila);
    }
    test_afirmar(pila_elementos(pila) == 0, "La pila se desapila por completo y queda vacia");
    test_afirmar(pila_desapilar(pila) == ERROR, "No se puede desapilar una pila vacia");

    pila_destruir(pila);
}

void pila_pruebas_creacion(){
    test_nuevo_sub_grupo("OPERACIONES DE CREACIÓN");
    pila_t * pila= pila_crear();
    test_afirmar(pila != NULL, "Se crea una pila");
    test_afirmar(pila_vacia(pila), "La pila está vacia");
    test_afirmar(pila_elementos(pila) == 0, "Tiene 0 elementos");
    test_afirmar(pila_tope(pila) == NULL, "No existe un último elemento");
    pila_destruir(pila);
}

void pruebas_pila(){
    test_nuevo_grupo("PRUEBAS DE PILA");
    pila_pruebas_creacion();
    pila_pruebas_basicas();
}