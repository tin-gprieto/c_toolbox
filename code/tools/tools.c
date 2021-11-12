#include "tools.h"
#define MAX_BAR 10

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

char* barra_porcentaje(size_t cantidad, size_t total, size_t tamanio_barra){
    if(total == 0 || tamanio_barra == 0) return NULL;
    if(cantidad > total) return NULL; 

    float porcentaje = (((float)cantidad)/(float)total);

    char barra[tamanio_barra];
    barra[0] = '[';

    for(int i = 1; i < porcentaje * (float)tamanio_barra; i++)
        barra[i] = '=';

    for(int i = 1; i< tamanio_barra; i++){
        if(barra[i] != '=')
            barra[i] = ' ';    
    }

    barra[tamanio_barra - 1] = ']';

    char* resultado = calloc(tamanio_barra + 20, sizeof(char));
    if(!resultado)  return NULL;
    sprintf(resultado, "%s      %.1f °/. \n", barra, porcentaje*100);
    return resultado;
}