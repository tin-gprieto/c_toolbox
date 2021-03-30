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
    char* linea_leida = NULL;

    linea_leida = fgets(buffer, tam_buffer, archivo);

    if(!linea_leida)
        return NULL;

    size_t final_linea = strlen(linea_leida);

    if(final_linea == 0 || linea_leida[final_linea - 1] != '\n'){
        char basura[1024];
        leer_linea(basura, 1024, archivo);
    }
    linea_leida[final_linea - 1] = 0;
    buffer = '\0';
    return linea_leida;
}

void print_barra_porcentaje(size_t cantidad, size_t total){
    if(cantidad > total){
        warning("el cálculo del porcentaje");
        return; 
    }
    float porcentaje = (((float)cantidad)/(float)total) * 100;
    char bar[MAX_BAR];
    for(int i = 0; i<(porcentaje/10); i++)
        bar[i] = '0';

    printf ("|");
    for(int i = 0; i< MAX_BAR; i++){
        if(bar[i] == '0'){
            printf (FONDO_VERDE "  " RESET);
        }else{
            printf ("  ");
        }
            
    }
    printf ("|");
    printf("%.1f \n", porcentaje);
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
    printf("    %i pruebas corridas - %i errores - " RESET,
           __test_cantidad_de_pruebas_corridas,
           __test_cantidad_de_pruebas_fallidas)
    if(__test_cantidad_de_pruebas_fallidas == 0 )
        printf(VERDE " OK \n\n" RESET);
    else
        printf(ROJO " ERROR \n\n" RESET);
}