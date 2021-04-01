#include "../code/hash.h"
#include "../toolbox/code/tools/tools.h"

#define MAX_CLAVE 10
#define MAX_VECTOR 10
#define MAX_STRING 10

typedef struct vector{
    char claves[MAX_VECTOR][MAX_STRING];
    int vector[MAX_VECTOR];
    size_t tope;
}vector_t;

void destructor_elemento_heap(void* elemento){
    free(elemento);
}

void pruebas_creacion(){
    test_nuevo_grupo("Pruebas de Creación");
    hash_t* hash = hash_crear(NULL, 10);
    test_afirmar(hash != NULL, "Se crea correctamente un hash sin destructor");
    test_afirmar(hash_cantidad(hash) == 0, "El hash está vacio");
    hash_destruir(hash);
    hash_t* hash2 = hash_crear(destructor_elemento_heap, 5);
    test_afirmar(hash2 != NULL, "Se crea correctamente un hash con destructor");
    test_afirmar(hash_cantidad(hash2) == 0, "El hash está vacio");
    hash_destruir(hash2);
    hash_t* error_2 = hash_crear(NULL, 0);
    test_afirmar(error_2 == NULL, "No se crea un hash de capacidad 0");
}

void pruebas_en_stack(){
    test_nuevo_grupo("Pruebas en el Stack");
    char clave1[MAX_CLAVE] = "1234";
    int elemento1 = 23;
    hash_t* hash = hash_crear(NULL, 10);
    
    test_nuevo_sub_grupo("Pruebas de inserción");
    test_afirmar(hash_insertar(hash, clave1, (int*)&elemento1) == EXITO, "Se inserta correctamente un elemento");
    test_afirmar(hash_cantidad(hash) == 1, "El hash tiene 1 elemento");
    test_afirmar(hash->ocupado == 1, "La tabla del hash tiene 1 espacio ocupado");
    test_afirmar(hash_contiene(hash, clave1), "El hash contiene al elemento por su clave");
    test_afirmar((int*)hash_obtener(hash, clave1) == &elemento1, "Se pudo obtener el elemento con su clave");

    test_nuevo_sub_grupo("Pruebas de Borrado");
    test_afirmar(hash_quitar(hash, clave1) == EXITO, "Se quita el único elemento del hash");
    test_afirmar(hash_contiene(hash, clave1) == false, "El hash no contiene al elemento quitado");
    test_afirmar(hash_cantidad(hash) == 0, "El hash está vacio");
    test_afirmar(hash->ocupado == 0, "La tabla de hash no tiene espacios ocupados");
    hash_destruir(hash);
}

void pruebas_en_heap(){
    test_nuevo_grupo("Pruebas en Heap(memoria dinámica)");
    char clave1[MAX_CLAVE] = "1234";
    int* elemento_heap = malloc(sizeof(int));
    if(!elemento_heap) return;
    (*elemento_heap) = 1;
    hash_t* hash = hash_crear(destructor_elemento_heap, 10);

    test_nuevo_sub_grupo("Pruebas de inserción");
    test_afirmar(hash_insertar(hash, clave1, elemento_heap) == EXITO, "Se inserta un elemento en memoria dinámica");
    test_afirmar(hash_cantidad(hash) == 1, "El hash tiene 1 elemento");
    test_afirmar(hash->ocupado == 1, "La tabla del hash tiene 1 espacio ocupado");
    test_afirmar(hash_contiene(hash, clave1), "El hash contiene al elemento por su clave");
    test_afirmar((int*)hash_obtener(hash, clave1) == elemento_heap, "Se pudo obtener el elemento con su clave");

    test_nuevo_sub_grupo("Pruebas de Borrado");
    test_afirmar(hash_quitar(hash, clave1) == EXITO, "Se quita un elemento en un hash dinámico");
    test_afirmar(hash_cantidad(hash) == 0, "El hash no tiene elementos");
    test_afirmar(hash->ocupado == 0, "La tabla del hash no tiene espacios ocupados");
    test_afirmar(hash_contiene(hash, clave1) == false, "El hash no contiene al elemento por su clave");
    test_afirmar((int*)hash_obtener(hash, clave1) == NULL, "No se pudo obtener el elemento con su clave");

    hash_destruir(hash);
}

void pruebas_casos_limite(){

    test_nuevo_grupo("Pruebas casos límite");
    char clave[MAX_CLAVE] = "A";
    char clave_falsa[MAX_CLAVE] = "B";
    int elemento = 0;
    hash_t* hash = hash_crear(NULL, 10);

    test_nuevo_sub_grupo("Pruebas con NULL");
    test_afirmar(hash_cantidad(NULL) == 0,"Cantidad 0 con hash nulo"); 
    test_afirmar(hash_contiene(NULL, clave) == false, "No contiene una clave si es un hash nulo"); 
    test_afirmar(hash_insertar(NULL, "A", &elemento) == ERROR, "No se inserta en un hash nulo"); 
    test_afirmar(hash_quitar(NULL, "A") == ERROR, "No se quita ningún elemento en un hash nulo");
    test_afirmar(hash_insertar(hash, NULL, (int*)&elemento) == ERROR, "No se insertar con un elemento con una clave nula");
    test_afirmar(hash_insertar(hash, clave, NULL) == EXITO, "Se inserta un elemento NULL");
    test_afirmar(hash_obtener(hash, clave) == NULL, "Se pudo obtener el elemento nulo con su clave");

    test_nuevo_sub_grupo("Pruebas por la negativa");
    hash_insertar(hash, clave, &elemento);
    test_afirmar(hash_obtener(hash, clave_falsa) == NULL, "No se obtiene un elemento no insertado");
    test_afirmar(hash_contiene(hash, clave_falsa) == false, "No contiene una clave no insertada");
    test_afirmar(hash_quitar(hash, clave_falsa) == ERROR, "No se quita un elemento que no hay");
    hash_destruir(hash);
}

void pruebas_basicas(){
    pruebas_creacion();
    pruebas_en_stack();
    pruebas_en_heap();
}

void pruebas_avanzadas(){
    test_nuevo_grupo("Pruebas Avanzadas");
    char clave1[MAX_CLAVE] = "ba";
    int elemento1 = 1;
    char clave2[MAX_CLAVE] = "ab";
    int elemento2 = 2;
    int elemento3 = 3;
    hash_t* hash = hash_crear(NULL, 10);
    
    test_nuevo_sub_grupo("Insertar una colisión");
    hash_insertar(hash, clave1, (int*)&elemento1);
    test_afirmar(hash_insertar(hash, clave2, (int*)&elemento2) == EXITO, "Se insertan dos elementos que colisionan");
    test_afirmar(hash_cantidad(hash) == 2, "El hash tiene dos elementos");
    test_afirmar(hash_contiene(hash, clave1) && hash_contiene(hash, clave2), "El hash contiene ambos elementos");
    test_afirmar((int*)hash_obtener(hash, clave1) == &elemento1, "Se pudo obtener el 1er elemento con su clave");
    test_afirmar((int*)hash_obtener(hash, clave2) == &elemento2, "Se pudo obtener el 2do elemento con su clave");
    test_afirmar(hash->ocupado == 1, "Solamente se ocupa un lugar de la tabla de hash");
    
    test_nuevo_sub_grupo("Borrar una colisión");
    test_afirmar(hash_quitar(hash, clave1) == EXITO, "Se quita el primer elemento");
    test_afirmar(hash_contiene(hash, clave1) == false, "No contiene al primer elemento");
    test_afirmar(hash_contiene(hash, clave2) == true, "Contiene al segundo elemento");
    test_afirmar((int*)hash_obtener(hash, clave1) == NULL, "No se puede obtener el elemento quitado");
    test_afirmar((int*)hash_obtener(hash, clave2) == &elemento2, "Se pudo obtener el 2do elemento con su clave");
    test_afirmar(hash_cantidad(hash) == 1, "El hash tiene un solo elemento");
    test_afirmar(hash->ocupado == 1, "Solamente se ocupa un lugar de la tabla de hash");
    
    test_nuevo_sub_grupo("Insertar una clave repetida");
    test_afirmar(hash_insertar(hash, clave2, &elemento3) == EXITO, "Se inserta un elemento con una clave preexistente");
    test_afirmar(hash_cantidad(hash) == 1, "El hash solo tiene un elemento");
    test_afirmar((int*)hash_obtener(hash, clave2) == &elemento3, "Se obtiene el nuevo elemento con la clave");
    hash_destruir(hash);

    test_nuevo_sub_grupo("Probar el Rehash");
    hash_t* rehash = hash_crear(NULL, 5);
    char c_1 [MAX_CLAVE] = "1";
    hash_insertar(rehash, c_1, &elemento1);
    char c_2 [MAX_CLAVE] = "2";
    hash_insertar(rehash, c_2, &elemento1);
    char c_3 [MAX_CLAVE] = "3";
    hash_insertar(rehash, c_3, &elemento1);
    test_afirmar(rehash->ocupado == 3 && rehash->tamanio == 5, "El hash se encuentra casi lleno");
    char c_4 [MAX_CLAVE] = "4";
    test_afirmar(hash_insertar(rehash, c_4, &elemento1) == EXITO, "Se inserta el elemento que genera rehash");
    test_afirmar(rehash->tamanio == 15, "El tamaño de la tabla se agrandó");
    test_afirmar((rehash->ocupado/rehash->tamanio) < 0.75, "El factor de carga vuelve a ser menor que el máximo permitido");
    hash_destruir(rehash);
}
bool comparar_vectores(int* vector_1, int* vector_2, size_t tamanio){
    size_t i = 0;
    bool resultado = true;
    while ((i<tamanio) && resultado == true){
        if(vector_1[i] != vector_2[i])
            resultado = false;
        else
            i++;
    }
    return resultado;
}
bool mostrar_clave(hash_t* hash, const char* clave, void* aux){
    int elemento = *(int*)hash_obtener(hash, clave);
    printf("      Clave: %s - Valor: %i\n", clave, elemento);
    return false;
}

bool cargar_vector(hash_t* hash, const char* clave, void* aux){
    int* elemento = (int*) hash_obtener(hash, clave);
    vector_t* vector = aux;
    vector->vector[vector->tope] = *elemento;
    vector->tope ++;
    return false;
}

void inicializar_hash_completo(hash_t* hash, vector_t* vector){
    vector->vector[0] = 0;
    vector->vector[1] = 1;
    vector->vector[2] = 2;
    vector->vector[3] = 3;
    strcpy(vector->claves[0], "<"); //0
    strcpy(vector->claves[1], "2"); //0
    strcpy(vector->claves[2], "3"); //1
    strcpy(vector->claves[3], "0"); //8
    vector->tope = 4;
    size_t i = 0;
    int estado = EXITO; 
    while(i < vector->tope && estado == EXITO){
        estado = hash_insertar(hash, vector->claves[i], &(vector->vector[i]));
        i++;
    }
}

void pruebas_iterador_interno(){
    hash_t* hash = hash_crear(NULL, 10);
    vector_t cargado;
    inicializar_hash_completo(hash, &cargado);
    test_nuevo_grupo("Iterador Interno");
    vector_t interno;
    interno.tope = 0;
    test_afirmar(hash_con_cada_clave(hash, cargar_vector, &interno) == hash_cantidad(hash), "Se recorre todo el hash");
    test_afirmar(comparar_vectores(cargado.vector, interno.vector, 4), "Se pasó por todos los elementos");
    test_afirmar(hash_con_cada_clave(hash, mostrar_clave, NULL) == hash_cantidad(hash), "Se recorre todo el hash con auxiliar nulo");
    test_afirmar(hash_con_cada_clave(hash, NULL, NULL) == 0, "No se recorre el hash sin función");
    hash_destruir(hash);
    hash_t* vacio = hash_crear(NULL, 5);
    test_afirmar(hash_con_cada_clave(vacio, cargar_vector, &interno) == 0, "No se recorre el hash vacío");
    hash_destruir(vacio);
}

void pruebas_iterador_externo(){
    hash_t* hash = hash_crear(NULL, 10);
    vector_t cargado;
    inicializar_hash_completo(hash, &cargado);
    test_nuevo_grupo("Iterador Externo");
    vector_t externo;
    externo.tope = 0;
    hash_iterador_t* iterador = hash_iterador_crear(hash);
    test_afirmar(iterador != NULL, "Se crea un iterador externo correctamente");
    test_afirmar(hash_iterador_tiene_siguiente(iterador),"El iterador tiene siguiente");
    while(hash_iterador_tiene_siguiente(iterador)){
        const char* clave = hash_iterador_siguiente(iterador);
        externo.vector[externo.tope] = *(int*)hash_obtener(hash, clave);
        externo.tope++;
    }
    hash_iterador_destruir(iterador);
    test_afirmar(comparar_vectores(cargado.vector, externo.vector, 4), "Se pasó por todos los elementos");
    hash_destruir(hash);
    hash_t* vacio = hash_crear(NULL, 5);
    test_afirmar(hash_iterador_crear(vacio) == NULL, "No se crea un iterador de un hash vacío");
    hash_destruir(vacio);
}

int main(){
    pruebas_basicas();
    pruebas_casos_limite();
    pruebas_avanzadas();
    pruebas_iterador_interno();
    pruebas_iterador_externo();
    test_mostrar_reporte();
}