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

int __test_cantidad_de_pruebas_corridas = 0;
int __test_cantidad_de_pruebas_fallidas = 0;
const char *__test_prueba_actual = NULL;

void __test_atajarse(void (*handler)(int)){
    signal(SIGABRT, handler);
    signal(SIGSEGV, handler);
    signal(SIGBUS, handler);
    signal(SIGILL, handler);
    signal(SIGFPE, handler);
}

void __test_morir(int signum){
    if (__test_prueba_actual)
        printf(ROJO "\n\nERROR MIENTRAS SE EJECUTABA LA PRUEBA: " AMARILLO "'%s'\n\n" BLANCO, __test_prueba_actual);
    else
        printf(ROJO "\n\nFINALIZACION ANORMAL DE LAS PRUEBAS\n\n" BLANCO);
    fflush(stdout);
    __test_atajarse(SIG_DFL);
}

void test_afirmar(int afirmacion, const char *descripcion){
    __test_prueba_actual = descripcion;
    __test_atajarse(__test_morir);
    if (afirmacion) {
        printf(VERDE "   %s " ,TILDE);
    }else{
        __test_cantidad_de_pruebas_fallidas++;
        printf(ROJO "   %s ",CRUZ);
    }
    printf(BLANCO " %s\n", __test_prueba_actual);
    fflush(stdout);
    __test_prueba_actual = NULL;
    __test_cantidad_de_pruebas_corridas++;
}

void test_nuevo_grupo(const char *descripcion){
    printf(AMARILLO "\n   %s\n   ", descripcion);
    while (*(descripcion++))
        printf("=");
    printf(BLANCO "\n");
}

void test_nuevo_sub_grupo(const char *descripcion){
    printf("   _________________________________");
    printf(CYAN "\n   %s\n" RESET, descripcion);
}

void test_mostrar_reporte(){
    printf(BLANCO "\n   ---------------------------------\n");
    printf("    %i pruebas corridas - %i errores -" RESET,
           __test_cantidad_de_pruebas_corridas,
           __test_cantidad_de_pruebas_fallidas);
    if(__test_cantidad_de_pruebas_fallidas == 0 )
        printf(VERDE " OK \n\n" RESET);
    else
        printf(ROJO " ERROR \n\n" RESET);
}