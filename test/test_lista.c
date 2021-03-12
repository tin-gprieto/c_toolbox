#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "test.h"
#include "lista.h"

#define ERROR -1

int inicializar_con_varios_elementos(lista_t *lista, int (*insercion)(lista_t *, void *), int* elementos, size_t cantidad){
    size_t i = 0;
    int estado = 0;
    while ((i < cantidad) && (estado == 0)){
        estado = insercion(lista, (void *)(elementos + i));
        i++;
    }
    return estado;
}

bool se_inserto_bien(lista_t *lista, int elementos[], size_t cantidad){
    bool son_iguales = true;
    size_t i = (lista->cantidad) - cantidad;
    int* lista_elemento;

    while(son_iguales && (i<cantidad)){
        lista_elemento = (int*) lista_elemento_en_posicion(lista, i);
        if(lista_elemento != (elementos+i))
            son_iguales = false;
        i++;   
    }
    return son_iguales;

}

bool existe_elemento(lista_t *lista, void* elemento){
    lista_iterador_t *iterador = lista_iterador_crear(lista);
    bool encuentra = false;
    while (lista_iterador_tiene_siguiente(iterador) && !encuentra){
        if(lista_iterador_elemento_actual(iterador) == elemento)
            encuentra = true;
        lista_iterador_avanzar(iterador);
    }
    lista_iterador_destruir(iterador);
    return encuentra;
}

bool vaciar_y_mostrar(lista_t *lista, void *(*elemento)(lista_t *), int (*borrar)(lista_t *), char *descripcion){
    int elimina = 0;
    printf("%s: ", descripcion);
    while ((elimina == 0) && (!lista_vacia(lista))){
        int* corriente = (int*)(elemento(lista));
        printf(" %i ", *corriente);
        elimina = borrar(lista);
    }
    printf("\n");
    return lista_vacia(lista);
}

void probar_operaciones_pila(){
    int elementos[] = {1, 2, 3, 4, 5, 6};
    size_t cant_elementos = 6;
    lista_t *pila = lista_crear();

    test_nuevo_sub_grupo("Apilar");
    int estado_pila = inicializar_con_varios_elementos(pila, lista_apilar, elementos, cant_elementos);
    test_afirmar(estado_pila == 0, "Se apiló varios elementos");
    test_afirmar((lista_elementos(pila) == cant_elementos), "Se insertó la cantidad adecuada");

    test_nuevo_sub_grupo("Desapilar");
    int * ultimo_elemento = (int *) lista_ultimo(pila);
    test_afirmar(lista_desapilar(pila) == 0,  "Se desapila un elemento");
    test_afirmar(!existe_elemento(pila, ultimo_elemento), "Se desapiló el último elemento");

    test_nuevo_sub_grupo("Desapilar y vaciar");
    bool se_vacia = vaciar_y_mostrar(pila, lista_ultimo, lista_desapilar, "॰ Pila");
    test_afirmar(se_vacia, "La pila se desapila por completo y queda vacia");
    test_afirmar(lista_desapilar(pila) == ERROR, "No se puede desapilar una pila vacia");

    lista_destruir(pila);
}

void probar_operaciones_cola(){
    int elementos[] = {1, 2, 3, 4, 5, 6};
    size_t cant_elementos = 6;
    lista_t *cola = lista_crear();

    test_nuevo_sub_grupo("Encolar");
    int estado_cola = inicializar_con_varios_elementos(cola, lista_encolar, elementos, cant_elementos);
    test_afirmar(estado_cola == 0, "Se encoló varios elementos");
    test_afirmar((lista_elementos(cola) == cant_elementos), "Se insertó la cantidad adecuada");

    test_nuevo_sub_grupo("Desencolar");
    int * primer_elemento = (int*) lista_primero(cola);
    test_afirmar(lista_desencolar(cola) == 0, "Se desencola un elemento");
    test_afirmar(!existe_elemento(cola, primer_elemento), "Se desencoló el primer elemento");

    test_nuevo_sub_grupo("Desencolar y vaciar");
    
    bool se_vacia = vaciar_y_mostrar(cola, lista_primero, lista_desencolar, "॰ Cola");
    test_afirmar(se_vacia, "La cola se desencola por completo y queda vacia");
    test_afirmar(lista_desencolar(cola) == ERROR, "No se puede desencolar una cola vacia");
    lista_destruir(cola);
    
}

bool mostrar_elemento(void *elemento, void *contexto){
    if (elemento)
        printf(" %i ", *(int *)elemento);
    return true;
}

bool mostrar_3_elementos(void *elemento, void *contador){
    if (elemento && contador){
        if ((*(int *)contador) > 2)
            return false;
        printf(" %i ", *(int *)elemento);
        (*(int *)contador)++;
    }
    return true;
}

void probar_operaciones_it_interno(){

    int elementos[] = {1, 2, 3, 4, 5, 6};
    size_t cant_elementos = 6;
    lista_t *lista = lista_crear();
    inicializar_con_varios_elementos(lista, lista_insertar, elementos, cant_elementos);

    test_nuevo_sub_grupo("Recorrido completo");
    size_t elementos_recorridos = 0;
    printf("॰ Lista");
    elementos_recorridos = lista_con_cada_elemento(lista, mostrar_elemento, NULL);
    printf("\n");
    test_afirmar((elementos_recorridos == lista_elementos(lista)), "Se recorrió todos los elementos de la lista");
    
    test_nuevo_sub_grupo("Recorrido parcial");
    int contador = 0;
    printf("॰ Lista");
    elementos_recorridos = lista_con_cada_elemento(lista, mostrar_3_elementos, (void *)&contador);
    printf("\n");
    test_afirmar((elementos_recorridos == 3), "Se recorrió los primeros 3 elementos de la lista");
    lista_destruir(lista);

    test_nuevo_sub_grupo("Recorrido con lista vacía");
    lista_t* lista_vacia = lista_crear();
    elementos_recorridos = lista_con_cada_elemento(lista_vacia, mostrar_elemento, NULL);
    test_afirmar(elementos_recorridos == 0, "No se recorre ningún elemento de una lista vacía");
    lista_destruir(lista_vacia);
}

void imprimir_con_it_externo(lista_t *lista, lista_iterador_t *iterador){
    size_t elementos_recorridos = 0;
    printf("॰ Lista");
    while (lista_iterador_tiene_siguiente(iterador)){
        printf(" %i ", *(int *)lista_iterador_elemento_actual(iterador));
        lista_iterador_avanzar(iterador);
        elementos_recorridos++;
    }
    printf("\n");
    test_afirmar((elementos_recorridos == lista_elementos(lista)), "Se recorrió todos los elementos de la lista");
}

void iterar_varios_elementos(){
    int elementos[] = {1, 2, 3, 4, 5, 6};
    size_t cant_elementos = 6;
    lista_t *lista = lista_crear();
    inicializar_con_varios_elementos(lista, lista_insertar, elementos, cant_elementos);
    lista_iterador_t *iterador = lista_iterador_crear(lista);

    test_afirmar((iterador != NULL), "Se crea un iterador correctamente");
    test_afirmar(((iterador->corriente) == (lista->nodo_inicio)), "El iterador comienza apuntando al primer elemento");
    test_afirmar(((iterador->lista) == lista), "El iterador contiene a la lista");
    imprimir_con_it_externo(&(*lista), &(*iterador));

    lista_iterador_destruir(iterador);
    lista_destruir(lista);
}

void iterar_un_solo_elemento(){
    lista_t *lista = lista_crear();
    int elemento = 23;
    lista_insertar(lista, &elemento);
    lista_iterador_t *iterador = lista_iterador_crear(lista);

    test_afirmar(lista_iterador_elemento_actual(iterador) == &elemento, "El iterador apunta al único elemento");
    test_afirmar(!lista_iterador_avanzar(iterador), "El iterador no puede avanzar");
    test_afirmar(!lista_iterador_tiene_siguiente(iterador), "No tiene elemento siguiente");

    lista_iterador_destruir(iterador);
    lista_destruir(lista);
}

void iterar_lista_vacia(){
    lista_t *lista = lista_crear();
    lista_iterador_t *iterador = lista_iterador_crear(lista);

    test_afirmar(iterador != NULL, "Se crea un iterador para una lista vacia");
    test_afirmar(!lista_iterador_tiene_siguiente(iterador), "No tiene siguiente");
    test_afirmar(!lista_iterador_avanzar(iterador), "No puede avanzar");
    test_afirmar(!lista_iterador_elemento_actual(iterador), "No contiene ningún elemento");

    lista_destruir(lista);
    lista_iterador_destruir(iterador);
}

void probar_operaciones_it_externo(){
    test_nuevo_sub_grupo("Iteración de varios elementos");
    iterar_varios_elementos();
    
    test_nuevo_sub_grupo("Iteración de un solo elemento");
    iterar_un_solo_elemento();
    
    test_nuevo_sub_grupo("Iteración de una lista vacía");
    iterar_lista_vacia();
}

void insertar_varios_elementos(int* elementos, size_t cant_elementos){
    lista_t *lista = lista_crear();
    int estado = inicializar_con_varios_elementos(lista, lista_insertar, elementos, cant_elementos);
    test_afirmar(estado == 0, "Se insertan varios elementos");
    test_afirmar((lista_elementos(lista) == cant_elementos), "Se insertó la cantidad adecuada");
    bool insercion = se_inserto_bien(lista, elementos, cant_elementos);
    test_afirmar((int)insercion, "Se insertaron en orden correcto");

    lista_destruir(lista);
}

void insertar_y_borrar_en_posicion(int *elementos, size_t cant_elementos){
    float elemento = 3.5;
    size_t posicion = 4;
    lista_t *lista = lista_crear();
    inicializar_con_varios_elementos(lista, lista_insertar, elementos, cant_elementos);

    test_afirmar((lista_insertar_en_posicion(lista, &elemento, posicion) == 0), "Se inserta un elemento en una posicion de la lista");
    test_afirmar((lista_elementos(lista) == (cant_elementos + 1)), "Tiene la cantidad adecuada");
    
    test_afirmar((lista_borrar_de_posicion(lista, posicion) == 0), "Se borra el elemento insertado anteriormente");
    test_afirmar((lista_elementos(lista) == cant_elementos), "Tiene la cantidad adecuada");
    test_afirmar((lista_elemento_en_posicion(lista, posicion) != &elemento), "El elemento borrado no está más");

    bool insercion = se_inserto_bien(lista, elementos, cant_elementos);
    test_afirmar((int)insercion, "La lista volvió al orden anterior de la inserción");

    lista_destruir(lista);
}

void insertar_y_borrar_al_principio(int *elementos, size_t cant_elementos){
    float elemento = 3.5;
    lista_t *lista = lista_crear();
    inicializar_con_varios_elementos(lista, lista_insertar, elementos, cant_elementos);

    test_afirmar((lista_insertar_en_posicion(lista, &elemento, 0) == 0), "Se inserta un elemento en la posicion 0");
    test_afirmar((lista_elementos(lista) == (cant_elementos + 1)), "Tiene la cantidad adecuada");

    test_afirmar((lista_borrar_de_posicion(lista, 0) == 0), "Se borra el elemento de la posicion 0");
    test_afirmar((lista_elementos(lista) == cant_elementos), "Tiene la cantidad adecuada");
    test_afirmar((lista_elemento_en_posicion(lista, 0) != &elemento), "El elemento borrado no está más");

    bool insercion = se_inserto_bien(lista, elementos, cant_elementos);
    test_afirmar((int)insercion, "La lista volvió al orden anterior de la inserción");

    lista_destruir(lista);
}

void insertar_y_borrar_al_final(int *elementos, size_t cant_elementos){
    int elemento = 7;
    lista_t *lista = lista_crear();
    inicializar_con_varios_elementos(lista, lista_insertar, elementos, cant_elementos);
    void *ultimo_elemento = lista_ultimo(lista);

    test_afirmar((lista_insertar_en_posicion(lista, &elemento, cant_elementos) == 0), "Se inserta despues del último elemento");
    test_afirmar((lista_ultimo(lista) != ultimo_elemento), "El último elemento no es el elemento anterior");
    test_afirmar((lista_elementos(lista) == (cant_elementos + 1)), "Tiene la cantidad adecuada");

    test_afirmar((lista_borrar_de_posicion(lista, cant_elementos) == 0), "Se borra el elemento anteriormente insertado");
    test_afirmar((lista_elementos(lista) == cant_elementos), "Tiene la cantidad adecuada");
    test_afirmar((lista_ultimo(lista) != &elemento), "El elemento no está en el final");

    lista_destruir(lista);
}

void insertar_y_borrar_fuera_de_lista(int* elementos, size_t cant_elementos){
    float elemento = 3.5;
    size_t posicion = 500;
    lista_t *lista = lista_crear();
    inicializar_con_varios_elementos(lista, lista_insertar, elementos, cant_elementos);

    test_afirmar((lista_insertar_en_posicion(lista, &elemento, posicion) == 0), "Se inserta un elemento en una posicion fuera de lista");
    test_afirmar((lista_elementos(lista) == (cant_elementos + 1)), "Tiene la cantidad adecuada");
    test_afirmar((lista_ultimo(lista) == &elemento), "El elemento se encuentra al final");
    test_afirmar((lista_elemento_en_posicion(lista, posicion) == NULL), "No existe elemento en la posicion fuera de la lista");

    test_afirmar((lista_borrar_de_posicion(lista, 500) == 0), "Se borra el nuevo elemento");
    test_afirmar((lista_ultimo(lista) != &elemento), "El elemento no está en el final");
    test_afirmar((lista_elementos(lista) == cant_elementos), "Tiene la cantidad adecuada");
    
    lista_destruir(lista);
}

void insertar_y_borrar_en_lista_vacia(){
    lista_t *lista = lista_crear();
    float elemento = 3.5;
    test_afirmar((lista_borrar_de_posicion(lista, 0) == ERROR), "No se puede borrar en una lista vacía");
    test_afirmar((lista_insertar_en_posicion(lista, &elemento, 0) == 0), "Se inserta un elemento en la posicion 0");
    test_afirmar((lista_elementos(lista) == 1), "Tiene la cantidad adecuada");
    test_afirmar((lista_elemento_en_posicion(lista, 0) == &elemento), "El elemento se encuentra la posicion 0");
    test_afirmar(lista_primero(lista) == lista_ultimo(lista), "El primer elemento también es el último");
    lista_destruir(lista);
}

void insertar_muchos_elementos(){
    lista_t *lista_enorme = lista_crear();
    int estado = 0;
    int i = 0;
    size_t cant_elementos = 500;
    int *elementos = NULL;
    while ((estado != ERROR) && (i < cant_elementos)){
        estado = lista_insertar(lista_enorme, (void *)&elementos[i]);
        i++;
    }
    test_afirmar((estado != ERROR), "No hubo problemas en insertar muchos elementos");
    test_afirmar((lista_elementos(lista_enorme) == cant_elementos), "Se insertó la cantidad de elementos esperada");
    lista_destruir(lista_enorme);
}

void probar_operaciones_avanzadas(){
    int elementos[] = {1, 2, 3, 4, 5, 6};
    size_t cant_elementos = 6;
    test_nuevo_sub_grupo("Insertar varios elementos");
    insertar_varios_elementos(elementos, cant_elementos);
    
    test_nuevo_sub_grupo("Insertar y borrar en posicion");
    insertar_y_borrar_en_posicion(elementos, cant_elementos);
    
    test_nuevo_sub_grupo("Insertar y borrar al principio con posicion");
    insertar_y_borrar_al_principio(elementos, cant_elementos);
    
    test_nuevo_sub_grupo("Insertar y borrar al final con posicion");
    insertar_y_borrar_al_final(elementos, cant_elementos);
    
    test_nuevo_sub_grupo("Insertar y borrar fuera de lista");
    insertar_y_borrar_fuera_de_lista(elementos, cant_elementos);
    
    test_nuevo_sub_grupo("Insertar y borrar en lista_vacia con posicion");
    insertar_y_borrar_en_lista_vacia();
    
    test_nuevo_sub_grupo("Insertar muchos elementos (500)");
    insertar_muchos_elementos();
}

void probar_operaciones_basicas(){

    int segundo_elemento = 2;
    int primer_elemento = 1;
    lista_t *lista = lista_crear();

    test_nuevo_sub_grupo("Insertar el primer elemento");
    test_afirmar((lista_insertar(lista, &primer_elemento) == 0), "Se agrega el primer elemento");
    test_afirmar(lista_primero(lista) == lista_ultimo(lista), "El primer elemento también es el último");
    test_afirmar(lista_elementos(lista) == 1, "Tiene 1 elemento");
    test_afirmar((lista_elemento_en_posicion(lista, 0) == &primer_elemento), "El elemento se encuentra en posicion 0 ");
    nodo_t* siguiente = (lista->nodo_inicio)->siguiente;  
    test_afirmar(siguiente == NULL, "El siguiente nodo es NULL");

    
    test_nuevo_sub_grupo("Insertar un segundo elemento");
    test_afirmar(lista_insertar(lista, &segundo_elemento) == 0, "Se agrega otro elemento");
    test_afirmar(lista_ultimo(lista) == &segundo_elemento, "Es el ultimo elemento de la lista");
    test_afirmar(lista_elementos(lista) == 2, "Tiene 2 elementos");
    test_afirmar((lista_elemento_en_posicion(lista, 1) == &segundo_elemento), "El elemento se encuentra en posicion 1");

    
    test_nuevo_sub_grupo("Borrar los dos elementos");
    test_afirmar((lista_borrar(lista) == 0), "Se borra el último elemento");
    test_afirmar(lista->cantidad == 1, "Tiene 1 elemento");
    test_afirmar(lista_primero(lista) == lista_ultimo(lista), "El último elemento también es el primero");
    test_afirmar(lista_elementos(lista) == lista->cantidad, "La cantidad coincide con lista_elementos");
    test_afirmar((lista_borrar(lista) == 0), "Se borra el único elemento");
    test_afirmar(lista_vacia(lista), "La lista está vacia");
    test_afirmar((lista_borrar(lista) == ERROR), "No se puede borrar algo de una lista vacia");

    lista_destruir(lista);
}

void probar_operaciones_comunes(){
    lista_t * lista= lista_crear();
    test_afirmar(lista != NULL, "Se crea una lista");
    test_afirmar(lista_vacia(lista), "La lista está vacia");
    test_afirmar(lista_elementos(lista) == 0, "Tiene 0 elementos");
    test_afirmar(lista_primero(lista) == NULL, "No existe un primer elemento");
    test_afirmar(lista_ultimo(lista) == NULL, "No existe un último elemento");
    lista_destruir(lista);
}

int main(){
    test_nuevo_grupo("OPERACIONES DE CREACIÓN");
    probar_operaciones_comunes();
    test_nuevo_grupo("OPERACIONES DE LISTA");
    probar_operaciones_basicas();
    test_nuevo_grupo("OPERACIONES AVANZADAS DE LISTA");
    probar_operaciones_avanzadas();
    test_nuevo_grupo("OPERACIONES CON ITERADOR EXTERNO");
    probar_operaciones_it_externo();
    test_nuevo_grupo("OPERACIONES CON ITERADOR INTERNO");
    probar_operaciones_it_interno();
    test_nuevo_grupo("OPERACIONES DE COLA");
    probar_operaciones_cola();
    test_nuevo_grupo("OPERACIONES DE PILA");
    probar_operaciones_pila();
    test_mostrar_reporte();
    return 0;
}

