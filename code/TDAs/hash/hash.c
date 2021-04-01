#include "hash.h"

#define FACTOR_MAXIMO 0.75
#define AUMENTO_REHASH 3

#define ERROR -1
#define EXITO 0

const int INSERCION_NUEVA = 0;
const int INSERCION_REPETIDA = 1;
const int INSERCION_COLISION = 2;

typedef struct dato{
    void* elemento;
    const char* clave;
}dato_t;

typedef void (*dato_destructor_t)(dato_t *, hash_destructor_t);

/*
* Dado un dato y el destructor de elementos,
* destruye su elemento y libera el espacio ocupado por el dato
* Pre : Dato creado
* Post: Dato destruido y elemento destruido (si cuenta con destructor)
*/
void dato_destruir(dato_t * dato, hash_destructor_t destructor_elemento){
    if(!dato) return;
    if(destructor_elemento)
        destructor_elemento(dato->elemento);
    free(dato);
}
/*
* Dado el hash, calcula su factor de carga
* Pre : Hash creado (e inicializado)
* Post: Factor de carga o -1 en caso de error
*/
float factor_de_carga(hash_t* hash){
    if(!hash) return ERROR;
    return (float) hash->ocupado / (float) hash->tamanio;
}
/*
* Dada una clave, calcula el valor de Hash que le corresponde 
* Pre : Clave existente
* Post: Valor de hash calculado (debe usarse aplicando módulo con el tamanio de la tabla)
*/
size_t valor_hash(const char * clave){
    size_t valor = 0;
    for(int i=0; i < strlen(clave); i++)
        valor += (size_t) clave[i];
    return valor;
}
/*
* Destruye una lista de datos, dado el destructor de datos y de elementos,
* En caso de que el destructor de datos sea NULL, se destruirá la lista sin los datos
* Pre : Lista creada
* Post: Lista destruida, junto a sus elementos si corresponde
*/
void lista_colisiones_destruir(lista_t* colisiones, dato_destructor_t destructor_dato, hash_destructor_t destructor_elemento){
    if(!colisiones) return;
    if(destructor_dato){
        lista_iterador_t * iterador = lista_iterador_crear(colisiones);
        if(!iterador) return;
        while(lista_iterador_tiene_siguiente(iterador)){
            dato_t* actual = lista_iterador_elemento_actual(iterador);
            destructor_dato(actual, destructor_elemento);
            lista_iterador_avanzar(iterador);
        }
        lista_iterador_destruir(iterador);
    }
    lista_destruir(colisiones);
}
/*
* Destruye una tabla completa, si el destructor de dato es NULL, no destruye sus datos
* Pre : Hash creado
* Post: Tabla destruida por completo
*/
void tabla_destruir(hash_t* hash, dato_destructor_t destructor_dato){
    if(!hash || !hash->tabla) return;
    for(int i = 0; i < hash->tamanio; i++){
        if(hash->tabla[i])
            lista_colisiones_destruir(hash->tabla[i], destructor_dato, hash->destructor);                
    }
    free(hash->tabla);
}
/*
* Crea un tabla dado su tamaño
* Pre : Tamanio distinto a 0
* Post: Puntero a tabla creada o NULL en caso de error
*/
lista_t** tabla_crear(size_t tamanio){
    lista_t** tabla = calloc(tamanio, sizeof(lista_t*));
    if(!tabla)
        return NULL;
    for (size_t i = 0; i < tamanio; i++)
        tabla[i] = NULL;
    return tabla;
}

//hash.h
void hash_destruir(hash_t* hash){
    tabla_destruir(hash, dato_destruir);
    free(hash);
}

//hash.h
hash_t* hash_crear(hash_destructor_t destruir_elemento, size_t capacidad){
    if(capacidad <= 0) return NULL;
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;
    hash->destructor = destruir_elemento;
    hash->tamanio = capacidad;
    hash->elementos = 0;
    hash->ocupado = 0;
    lista_t** aux = tabla_crear(hash->tamanio);
    if(!aux){
        free(hash); 
        return NULL;
    }
    hash->tabla = aux;
    return hash;
}
/*
* Busca un dato dentro de una lista de datos
* Pre : Lista creada y clave no nula
* Post: La posicion del dato dentro de la lista o -1 si no lo encuentra o hay un error
*/
int buscar_dato(lista_t* colisiones, const char* clave){
    lista_iterador_t* iterador = lista_iterador_crear(colisiones);
    if(!iterador) return ERROR;
    bool encontro = false;
    int pos = 0;
    while(lista_iterador_tiene_siguiente(iterador) && !encontro){
        dato_t* actual = lista_iterador_elemento_actual(iterador);
        if(strcmp(actual->clave, clave) == 0)
            encontro = true;
        else{
            lista_iterador_avanzar(iterador);
            pos++;
        }
    }
    lista_iterador_destruir(iterador);
    if(!encontro) return ERROR;
    return pos;
}
/*
* Dada una clave, busca en una lista si se repite 
* Pre : Lista creada y clave no nula
* Post: Puntero al dato repetido o NULL si no se repite o hay un error
*/
dato_t* buscar_dato_repetido(lista_t* colisiones, const char* clave){
    if(!colisiones) return NULL;
    int pos = buscar_dato(colisiones, clave);
    if(pos == ERROR) return NULL;
    dato_t* repetido = lista_elemento_en_posicion(colisiones, (size_t) pos);
    return repetido;
}
/*
* Dada su posición en la tabla, inserta un dato
* Pre : Tabla creada, posicion calculada y dato creado
* Post: -1 en caso de ERROR o el tipo de insercion (constantes INSERCION)
*/
int insertar_dato_en_tabla(lista_t** tabla, size_t pos, dato_t* dato, hash_destructor_t destructor){
    int estado;
    if(!tabla[pos]){
        lista_t* lista = lista_crear();
        if(!lista)  return ERROR;
        tabla[pos] = lista;
        estado = lista_insertar(lista, dato);
        if(estado == ERROR) return ERROR;
        return INSERCION_NUEVA;
    }
    dato_t* repetido = buscar_dato_repetido(tabla[pos], dato->clave);
    if(repetido){
        if(repetido->elemento && destructor) 
            destructor(repetido->elemento);
        repetido->elemento = dato->elemento;
        free(dato);
        return INSERCION_REPETIDA;
    }
    estado = lista_insertar(tabla[pos], dato);
    if(estado == ERROR) return ERROR;
    return INSERCION_COLISION;
}
/*
* Pasa los datos de una lista a otra
* Pre : Lista de datos creada (no vacia)
* Post: 0 si se pasaron todos los elementos a la nueva lista o -1 si hubo un error
*/
int copiar_datos_a_lista(lista_t* datos, lista_t* lista){
    if(!datos) return ERROR;
    lista_iterador_t* iterador = lista_iterador_crear(datos);
    if(!iterador) return ERROR;
    int estado = EXITO;
    while(lista_iterador_tiene_siguiente(iterador) && estado != ERROR){
        dato_t* actual = lista_iterador_elemento_actual(iterador);
        estado = lista_insertar(lista, actual);
        lista_iterador_avanzar(iterador);
    }
    lista_iterador_destruir(iterador);
    return estado;
}
/*
* Dada una tabla, copia todos sus datos a una lista
* Pre : tabla creada y tamanio de la misma
* Post: Lista creada con todos los datos de la tabla o NULL en caso de error
*/
lista_t* tabla_copiar(lista_t** tabla, size_t tamanio){
    lista_t* datos = lista_crear();
    if(!datos) return NULL;
    int estado = EXITO;
    size_t i = 0;
    while((i < tamanio) && (estado != ERROR)){
        if(tabla[i])
            estado = copiar_datos_a_lista(tabla[i], datos);
        i++;
    }
    if(estado == ERROR){
        lista_destruir(datos);
        return NULL;
    }
    return datos;
}
/*
* Dada una lista con todos los datos, los inserta a una nueva tabla
* Pre : Tabla creada (e inicializada) vacia, tamanio de la misma y 
* lista creada con todos los datos copiados
* Post: cantidad de espacios ocupados en la nueva tabla o -1 en caso de error
*/
int reinsertar_datos(lista_t** tabla, size_t tamanio, lista_t* datos, hash_destructor_t destructor){
    lista_iterador_t * iterador = lista_iterador_crear(datos);
    if(!iterador) return ERROR;
    int insercion = EXITO;
    int ocupado = 0;
    while(lista_iterador_tiene_siguiente(iterador) && insercion != ERROR){
        dato_t* dato = lista_iterador_elemento_actual(iterador);
        size_t pos = valor_hash(dato->clave) % tamanio;
        insercion = insertar_dato_en_tabla(tabla, pos, dato, destructor);
        if(insercion == INSERCION_NUEVA)
            ocupado ++;
        lista_iterador_avanzar(iterador);
    }
    lista_iterador_destruir(iterador);
    if(insercion == ERROR)
        return ERROR;
    return ocupado;

}
/*
* Dado un hash para rehashear, lo rehashea (agranda la tabla y reinserta los elementos)
* Pre : Hash creado y que necesite un rehash
* Post: 0 si se pudo rehashear o -1 si no pudo (error)
*/
int rehashear_tabla(hash_t* hash){
    lista_t* datos = tabla_copiar(hash->tabla, hash->tamanio);
    if(!datos) return ERROR;
    size_t nuevo_tamanio = hash->tamanio * AUMENTO_REHASH;
    lista_t** nueva_tabla = tabla_crear(nuevo_tamanio);
    if(!nueva_tabla){
        lista_destruir(datos);
        return ERROR;
    } 
    int estado = reinsertar_datos(nueva_tabla, nuevo_tamanio, datos, hash->destructor);
    if(estado == ERROR){
        lista_destruir(datos);
        return ERROR;
    }
    tabla_destruir(hash, NULL); //Destruir tabla anterior (sin destruir los datos)
    lista_destruir(datos); //Destruir lista auxiliar (sin destruir los datos)
    hash->tabla = nueva_tabla; 
    hash->tamanio = nuevo_tamanio;
    hash->ocupado = (size_t) estado;
    if(factor_de_carga(hash) > FACTOR_MAXIMO)
        return rehashear_tabla(hash);
    return EXITO;
}

//hash.h
int hash_insertar(hash_t* hash, const char* clave, void* elemento){
    if(!hash || !clave) return ERROR;
    dato_t* dato = malloc(sizeof(dato_t));
    if(!dato) return ERROR;
    dato->clave = clave;
    dato->elemento = elemento;
    size_t pos = valor_hash(clave) % hash->tamanio;
    int insercion = insertar_dato_en_tabla(hash->tabla, pos, dato, hash->destructor);
    if(insercion == ERROR) return ERROR;
    if(insercion == INSERCION_NUEVA)
        hash->ocupado++;
    if(insercion != INSERCION_REPETIDA)
        hash->elementos ++;
    if(factor_de_carga(hash) > FACTOR_MAXIMO)
        return rehashear_tabla(hash);
    return EXITO;
}
//hash.h
int hash_quitar(hash_t* hash, const char* clave){
    if(!hash || !clave) return ERROR;
    size_t pos_tabla = valor_hash(clave) % hash->tamanio;
    int pos_lista = buscar_dato(hash->tabla[pos_tabla], clave);
    if(pos_lista == ERROR) return ERROR;
    dato_t* eliminado = lista_elemento_en_posicion(hash->tabla[pos_tabla], (size_t)pos_lista);
    if(!eliminado) return ERROR;
    dato_destruir(eliminado, hash->destructor);
    int estado = lista_borrar_de_posicion(hash->tabla[pos_tabla], (size_t) pos_lista);
    if(estado == ERROR) return ERROR;
    if(lista_vacia(hash->tabla[pos_tabla])){
        lista_destruir(hash->tabla[pos_tabla]);
        hash->tabla[pos_tabla] = NULL;
        hash->ocupado --;
    }
    hash->elementos --;
    return EXITO;
}
//hash.h
void* hash_obtener(hash_t* hash, const char* clave){
    if(!hash || !clave) return NULL;
    size_t pos_tabla = valor_hash(clave) % hash->tamanio;
    int pos_lista = buscar_dato(hash->tabla[pos_tabla], clave);
    if(pos_lista == ERROR) return NULL;
    dato_t* dato = lista_elemento_en_posicion(hash->tabla[pos_tabla], (size_t)pos_lista);
    if(!dato) return NULL;
    return dato->elemento;
}
//hash.h
bool hash_contiene(hash_t* hash, const char* clave){
    if(!hash || !clave) return false;
    size_t pos_tabla = valor_hash(clave) % hash->tamanio;
    int pos_lista = buscar_dato(hash->tabla[pos_tabla], clave);
    if(pos_lista == ERROR) return false;
    return true;
}
//hash.h
size_t hash_cantidad(hash_t* hash){
    if(hash) return hash->elementos;
    return 0;
}
//hash.h
size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
    if(!hash || !funcion)
        return 0;
    size_t elementos_contados = 0;
    size_t pos = 0;
    bool se_corta = false;
    while(elementos_contados < hash->elementos && !se_corta && pos < hash->tamanio){
        if(hash->tabla[pos]){
            lista_iterador_t* iterador = lista_iterador_crear(hash->tabla[pos]);
            while(lista_iterador_tiene_siguiente(iterador) && !se_corta){
                dato_t* actual = lista_iterador_elemento_actual(iterador);
                se_corta = funcion(hash, actual->clave, aux);
                elementos_contados ++;
                lista_iterador_avanzar(iterador);
            }
            lista_iterador_destruir(iterador);
        }
        pos ++;
    }
    return elementos_contados;
}
//hash.h
hash_iterador_t* hash_iterador_crear(hash_t* hash){
    if(!hash || hash->elementos == 0) return NULL;
    hash_iterador_t* iter = malloc(sizeof(hash_iterador_t));
    if(!iter) return NULL;
    iter->hash = hash;
    iter->pos_tabla = 0;
    iter->pos_lista_actual = 0;
    iter->contador = 0;
    return iter;
}
/*
* Avanza al próximo elemento del hash
* Pre : hash creado, posiciones inicializadas a 0 en la creación
* Post: Devuelve la clave antes de avanzar o NULL en caso de no poder avanzar
*/
const char* hash_avanzar(hash_t* hash, size_t* pos_tabla, size_t* pos_lista){
    if((*pos_tabla) >= hash->tamanio) return NULL;
    if(!hash->tabla[*pos_tabla]){
        (*pos_tabla) ++;
        return hash_avanzar(hash, pos_tabla, pos_lista);
    }
    dato_t* actual = (dato_t*)lista_elemento_en_posicion(hash->tabla[*pos_tabla], *pos_lista);
    if(!actual){
        (*pos_tabla) ++;
        (*pos_lista) = 0;
        return hash_avanzar(hash, pos_tabla, pos_lista);
    }else
        (*pos_lista)++;
    return actual->clave;
}
//hash.h
const char* hash_iterador_siguiente(hash_iterador_t* iterador){
    if(!iterador) return NULL;
    const char* clave = hash_avanzar(iterador->hash, &(iterador->pos_tabla), &(iterador->pos_lista_actual));
    if(!clave) return NULL;
    iterador->contador ++;
    return clave;
}
//hash.h
bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
    if(!iterador) return false;
    size_t max_elementos = iterador->hash->elementos;
    if(iterador->contador >= max_elementos) return false;
    return true;
}
//hash.h
void hash_iterador_destruir(hash_iterador_t* iterador){
    free(iterador);
}
