#include "../code/interfaz/interfaz.h"

#define MAX_STRING 50

typedef struct estructura{
    int numero[4];
    char string[4][MAX_STRING];
    size_t cantidad;
}estructura_t;

void funcion_mostrar_linea(interfaz_t* interfaz, void* nada, void* aux_nada){
    interfaz_informacion_imprimir_linea(interfaz, BLANCO, "Informacion linea");
}

void funcion_mostrar(interfaz_t* interfaz, void* info, void* aux){
    estructura_t* vector = (estructura_t*) info;
    size_t tope = vector->cantidad;
    for(size_t i = 0; i < tope; i++){
        char linea[200];
        sprintf(linea, "%i - %s", vector->numero[i], vector->string[i]);
        interfaz_informacion_imprimir_linea((interfaz_t*)interfaz, VERDE, linea);
    }
}

void inicializar_menu(interfaz_t* interfaz){
    interfaz_menu_cargar_opcion(interfaz, 0, '1', "Primer_opcion"); 
    interfaz_menu_cargar_opcion(interfaz, 0, '2', "Segunda_opcion"); 
    interfaz_menu_cargar_opcion(interfaz, 0, '3', "Tercer_opcion");
    interfaz_menu_cargar_opcion(interfaz, 0, '4', "Cuarta_opcion");
    interfaz_menu_cargar_opcion(interfaz, 1, '1', "Primer_opcion"); 
    interfaz_menu_cargar_opcion(interfaz, 1, '2', "Segunda_opcion"); 
    interfaz_menu_cargar_opcion(interfaz, 1, '3', "Tercer_opcion");

}

void inicializar_informacion(estructura_t* estructura){
    estructura->numero[0]= 3;
    estructura->numero[1]= 5;
    estructura->numero[2]= 6;
    estructura->numero[3]= 7;
    strcpy(estructura->string[0] , "Informacion 1");
    strcpy(estructura->string[1] , "Informacion 2");
    strcpy(estructura->string[2] , "Informacion 3");
    strcpy(estructura->string[3] , "Informacion 4");
    estructura->cantidad = 4;
}

int main(){
    interfaz_t* interfaz = interfaz_crear(interfaz_set_dimension(70,20,10), interfaz_set_estetica(FONDO_NEGRO, BLANCO, AMARILLO));
    if(!interfaz){
        warning("la creación del interfaz");
    }
    
    interfaz_menu_insertar(interfaz, "Menu inicio");
    interfaz_menu_insertar(interfaz, "Menu desarrollo");
    interfaz_informacion_insertar(interfaz, "Informacion varia", funcion_mostrar);
    interfaz_informacion_insertar(interfaz, "Informacion linea", funcion_mostrar_linea);
    
    inicializar_menu(interfaz);
    estructura_t estructura;
    inicializar_informacion(&estructura);
    
    interfaz_menu_mostrar(interfaz, 0);
    interfaz_menu_mostrar(interfaz, 1);
    interfaz_informacion_mostrar(interfaz, 0, &estructura, NULL);
    interfaz_informacion_mostrar(interfaz, 1, NULL, NULL);

    int numero = atoi(interfaz_pedir_string(interfaz, "un numero"));
    printf("Número ingresado: %i \n", numero);
    char* str = interfaz_pedir_string(interfaz, "un string");
    printf("String ingresado: %s \n", str);
    char* ruta = interfaz_pedir_archivo(interfaz, "txt","una ruta de archivo");
    printf("Ruta ingresada: %s \n", ruta);
    
    interfaz_destruir(interfaz);
    return 0;
}