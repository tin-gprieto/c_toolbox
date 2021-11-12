#include "../code/interfaz/interfaz.h"

#define MAX_STRING 50

typedef struct estructura{
    int numero[4];
    char string[4][MAX_STRING];
    size_t cantidad;
}estructura_t;

void funcion_mostrar_linea(interfaz_t* interfaz, void* nada, void* aux_nada){
    informacion_imprimir_linea(interfaz, BLANCO, "Informacion linea");
}

void funcion_mostrar(interfaz_t* interfaz, void* info, void* aux){
    estructura_t* vector = (estructura_t*) info;
    size_t tope = vector->cantidad;
    for(size_t i = 0; i < tope; i++){
        char linea[200];
        sprintf(linea, "%i - %s", vector->numero[i], vector->string[i]);
        informacion_imprimir_linea((interfaz_t*)interfaz, VERDE, linea);
    }
}

void inicializar_menu(interfaz_t* interfaz){
    menu_cargar_opcion(interfaz, 0, 'A', "Primer_opcion"); 
    menu_cargar_opcion(interfaz, 0, 'B', "Segunda_opcion"); 
    menu_cargar_opcion(interfaz, 0, 'C', "Tercer_opcion");
    menu_cargar_opcion(interfaz, 0, 'D', "Cuarta_opcion");
    menu_cargar_opcion(interfaz, 1, 'A', "Primer_opcion"); 
    menu_cargar_opcion(interfaz, 1, 'B', "Segunda_opcion"); 
    menu_cargar_opcion(interfaz, 1, 'C', "Tercer_opcion");

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
    interfaz_t* interfaz = interfaz_crear(set_dimension(70,20,10), set_estetica(FONDO_NEGRO, BLANCO, AMARILLO));
    if(!interfaz){
        warning("la creación del interfaz");
    }
    
    menu_insertar(interfaz, "Menu inicio");
    menu_insertar(interfaz, "Menu desarrollo");
    informacion_insertar(interfaz, "Informacion varia", funcion_mostrar);
    informacion_insertar(interfaz, "Informacion linea", funcion_mostrar_linea);
    
    inicializar_menu(interfaz);
    estructura_t estructura;
    inicializar_informacion(&estructura);
    
    menu_mostrar(interfaz, 0);
    menu_mostrar(interfaz, 1);
    informacion_mostrar(interfaz, 0, &estructura, NULL);
    informacion_mostrar(interfaz, 1, NULL, NULL);

    int numero = atoi(interfaz_pedir_string(interfaz, "un numero"));
    printf("Número ingresado: %i \n", numero);
    char* str = interfaz_pedir_string(interfaz, "un string");
    printf("String ingresado: %s \n", str);
    char* ruta = interfaz_pedir_archivo(interfaz, "txt","una ruta de archivo");
    printf("Ruta ingresada: %s \n", ruta);
    
    interfaz_destruir(interfaz);
    return 0;
}