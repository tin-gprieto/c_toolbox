#include "tools.h"

const int MAX_BAR = 10;
const int MAX_STRING = 1024;
const char* FILE_DELIM = ".";

void warning(const char * descripcion){
    printf(ROJO "%s - Hubo un problema con %s" RESET "\n", CRUZ, descripcion);
}

void print_array(int* array, size_t size){
    printf("    [");
    for(int i=0; i<size; i++)
        printf(" %i ", array[i]);
    printf("]\n");
}

void limpiar_buffer(){
    char c;
    while ((c = (char)getchar()) != '\n' && c != EOF);
}

char* leer_linea(char* buffer, int tam_buffer, FILE* archivo){
    if(!archivo || tam_buffer <= 0) return NULL;
    char* linea_leida = NULL;

    linea_leida = fgets(buffer, tam_buffer, archivo);

    if(!linea_leida) return NULL;

    size_t final_linea = strlen(linea_leida);

    if(final_linea == 0 || linea_leida[final_linea - 1] != '\n'){
        char basura[1024];
        leer_linea(basura, 1024, archivo);
    }
    linea_leida[final_linea - 1] = 0;
    buffer = '\0';
    return linea_leida;
}

void print_barra_porcentaje(size_t cantidad, size_t total, size_t tamanio_barra){
    if(cantidad > total || total == 0 || tamanio_barra == 0) return;
    float porcentaje = (((float)cantidad)/(float)total);
    float tamanio = (float) tamanio_barra;

    printf("\n    |");
    for(int i = 0; i < porcentaje * tamanio; i++)
        printf(FONDO_VERDE " ");
    for(int i = 0; i < tamanio - (porcentaje * tamanio); i++)
        printf(BLANCO " ");
        
    printf(BLANCO "|   %.1f °/. \n\n", porcentaje*100);
}

char* buscar_extension(char* ruta_archivo){
    char* string_partido = strtok(ruta_archivo, FILE_DELIM);
    char* siguiente_lectura = strtok(NULL, FILE_DELIM);
    while(siguiente_lectura != NULL){
        string_partido = siguiente_lectura;
        siguiente_lectura = strtok(NULL, FILE_DELIM);
    }
    return string_partido;
}

bool ruta_cumple_extension(const char* ruta_archivo, const char* extension){
    char aux[MAX_STRING];
    strcpy(aux, ruta_archivo);
    char* ruta_extension = buscar_extension(aux);
    return (ruta_extension && strcmp(ruta_extension, extension) == 0) ? true : false;
}