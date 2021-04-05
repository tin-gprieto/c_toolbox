#include "../code/interfaz/interfaz.h"
#include "../code/tools/tools.h"

#define MAX_STRING 50

typedef struct estructura{
    int numero[4];
    char string[4][MAX_STRING];
    size_t cantidad;
}estructura_t;

void funcion_mostrar(interfaz_t* interfaz, void* info, void* aux){
    estructura_t* vector = (estructura_t*) info;
    size_t tope = vector->cantidad;
    for(size_t i = 0; i < tope; i++){
        char linea[200];
        sprintf(linea, "%i - %s", vector->numero[i], vector->string[i]);
        informacion_linea((interfaz_t*)interfaz, VERDE, linea);
    }
}

void inicializar_menu(interfaz_t* interfaz){
    menu_cargar_opcion(interfaz, 0, 'A', "Primer_opcion"); 
    menu_cargar_opcion(interfaz, 0, 'B', "Segunda_opcion"); 
    menu_cargar_opcion(interfaz, 0, 'C', "Tercer_opcion");
    menu_cargar_opcion(interfaz, 0, 'D', "Cuarta_opcion");
    menu_cargar_opcion(interfaz, 1, 'A', "Primer_opcion"); 
    menu_cargar_opcion(interfaz, 1, 'B', "Segunda_opcion"); 
    menu_cargar_opcion(interfaz, 2, 'A', "Primer_opcion"); 
    menu_cargar_opcion(interfaz, 2, 'B', "Segunda_opcion"); 
    menu_cargar_opcion(interfaz, 2, 'C', "Tercer_opcion");
}

void inicializar_informacion(estructura_t* estructura){
    estructura->numero[0]= 3;
    estructura->numero[1]= 5;
    estructura->numero[2]= 6;
    estructura->numero[3]= 7;
    strcpy(estructura->string[0] , "Analisis");
    strcpy(estructura->string[1] , "Fisica");
    strcpy(estructura->string[2] , "Programacion");
    strcpy(estructura->string[3] , "Estadistica");
    estructura->cantidad = 4;
}

int main(){
    interfaz_t* interfaz = interfaz_crear(set_dimension(70,20,10), set_estetica(FONDO_NEGRO, BLANCO, CELESTE));
    if(!interfaz){
        warning("la creación del interfaz");
    }
    if(menu_insertar(interfaz, "Menu inicio", TIPO_MENU_INICIO) == ERROR){
        interfaz_destruir(interfaz);
        return ERROR;
    }
    if(menu_insertar(interfaz, "Menu desarrollo", TIPO_MENU_MEDIO) == ERROR){
        interfaz_destruir(interfaz);
        return ERROR;
    }
    if(menu_insertar(interfaz, "Menu final", TIPO_MENU_FINAL) == ERROR){
        interfaz_destruir(interfaz);
        return ERROR;
    }
    
    inicializar_menu(interfaz);
    
    menu_mostrar(interfaz, 0);
    menu_mostrar(interfaz, 1);
    menu_mostrar(interfaz, 2);
    
    estructura_t estructura;

    inicializar_informacion(&estructura);

    informacion_insertar(interfaz, "MATERIAS", funcion_mostrar);
    informacion_mostrar(interfaz, 0, &estructura, NULL);

    interfaz_destruir(interfaz);
    return 0;
}