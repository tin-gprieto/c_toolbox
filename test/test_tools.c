#include "../code/tools/tools.h"
#include "../code/tools/testing.h"

#define MAX_STR 20

int main(){

    const char* extension = "csv";
    char str_vacio [MAX_STR]= "";
    char str_sin_extension [MAX_STR]= "hola";
    char str_con_dos_extensiones_incorrecto [MAX_STR]= "hola.csv.hola";
    char str_con_dos_extensiones_correcto [MAX_STR]= "hola.hola.csv";
    char str_correcto [MAX_STR]= "hola.csv";
    char str_incorrecto [MAX_STR]= "hola.txt";
    
    test_nuevo_grupo("Test corroborar extension");
    test_afirmar(!ruta_corroborar_extension(str_vacio, extension), "Un string vacío no corrobora");
    test_afirmar(!ruta_corroborar_extension(str_sin_extension, extension), "Un string sin extension no corrobora"); 
    test_afirmar(ruta_corroborar_extension(str_correcto, extension), "Un string que tiene la extensión corrobora");
    test_afirmar(!ruta_corroborar_extension(str_incorrecto, extension), "Un string que tiene la extensión distinta no corrobora");
    test_afirmar(ruta_corroborar_extension(str_con_dos_extensiones_correcto, extension),  "Un string con otro punto pero termina con la extensión corrobora");
    test_afirmar(!ruta_corroborar_extension(str_con_dos_extensiones_incorrecto, extension), "Un string con otro punto y sin que termine con la extensión no corrobora");
    test_mostrar_reporte();
    test_nuevo_grupo("Test barra de porcentaje");
    print_barra_porcentaje(0,100,10);
    print_barra_porcentaje(100,100,10);
    print_barra_porcentaje(50,100,10);
    print_barra_porcentaje(33,100,10);
    print_barra_porcentaje(99,100,10);
    printf("\n");

    return 0;
}