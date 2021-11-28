#include "testing.h"
#define FONDO_VERDE "\e[1;42m"

size_t __test_cantidad_de_pruebas_corridas = 0;
size_t __test_cantidad_de_pruebas_fallidas = 0;
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
        printf(VERDE "   %s ", TILDE);
    }else{
        __test_cantidad_de_pruebas_fallidas++;
        printf(ROJO "   %s ", CRUZ);
    }
    if(__test_cantidad_de_pruebas_corridas < 9) printf(BLANCO "0");
    printf(BLANCO "%li - %s\n",__test_cantidad_de_pruebas_corridas + 1, __test_prueba_actual);
    fflush(stdout);
    __test_prueba_actual = NULL;
    __test_cantidad_de_pruebas_corridas++;
}

void test_titulo(const char *descripcion){
    printf(CYAN "   %s\n"BLANCO, descripcion);
}

void test_nuevo_grupo(const char *descripcion){
    printf(AMARILLO "\n   %s\n   ", descripcion);
    while (*(descripcion++))
        printf("=");
    printf(RESET "\n");
}

void test_nuevo_sub_grupo(const char *descripcion){
    printf(AZUL "\n   %s\n   " RESET, descripcion);
    while (*(descripcion++))
        printf("_");
    printf("\n\n");
}

void reporte_barra_porcentaje(size_t cantidad, size_t total){

    float porcentaje = (((float)cantidad)/(float)total);
    float tamanio_barra = 30;

    printf("    |");
    for(int i = 0; i < porcentaje * tamanio_barra; i++)
        printf(FONDO_VERDE " ");
    for(int i = 0; i < tamanio_barra - (porcentaje * tamanio_barra); i++)
        printf(BLANCO " ");
        
    printf(BLANCO "|   %.1f °/. \n\n\n", porcentaje*100);
}

void test_mostrar_reporte(){
    printf(BLANCO "   ____________________________________________\n");
    printf("\n    %li pruebas corridas - %li errores -" RESET,
           __test_cantidad_de_pruebas_corridas,
           __test_cantidad_de_pruebas_fallidas);
    if(__test_cantidad_de_pruebas_fallidas == 0 )
        printf(VERDE " OK \n\n" RESET);
    else
        printf(ROJO " ERROR \n\n" RESET);
    size_t cantidad_pruebas_pasadas = __test_cantidad_de_pruebas_corridas - __test_cantidad_de_pruebas_fallidas;
    reporte_barra_porcentaje(cantidad_pruebas_pasadas, __test_cantidad_de_pruebas_corridas);
}