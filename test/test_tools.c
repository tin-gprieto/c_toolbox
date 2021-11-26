#include "../code/tools/tools.h"
#include "../code/tools/testing.h"

#define MAX_STR 20

int main(){

    const char* extension = "csv";
    const char str_vacio [MAX_STR]= "";
    const char str_sin_extension [MAX_STR]= "hola";
    const char str_con_dos_extensiones_incorrecto [MAX_STR]= "hola.csv.hola";
    const char str_con_dos_extensiones_correcto [MAX_STR]= "hola.hola.csv";
    const char str_correcto [MAX_STR]= "hola.csv";
    const char str_incorrecto [MAX_STR]= "hola.txt";
    
    test_nuevo_grupo("Test corroborar extension");
    printf("\n    %s - %s - %s - %s - %s - %s \n",
        str_vacio, str_sin_extension, str_correcto, str_incorrecto, str_con_dos_extensiones_correcto, str_con_dos_extensiones_incorrecto);
    test_afirmar(!ruta_cumple_extension(str_vacio, extension), "Un string vacío no corrobora");
    test_afirmar(!ruta_cumple_extension(str_sin_extension, extension), "Un string sin extension no corrobora"); 
    test_afirmar(ruta_cumple_extension(str_correcto, extension), "Un string que tiene la extensión corrobora");
    test_afirmar(!ruta_cumple_extension(str_incorrecto, extension), "Un string que tiene la extensión distinta no corrobora");
    test_afirmar(ruta_cumple_extension(str_con_dos_extensiones_correcto, extension),  "Un string con otro punto pero termina con la extensión corrobora");
    test_afirmar(!ruta_cumple_extension(str_con_dos_extensiones_incorrecto, extension), "Un string con otro punto y sin que termine con la extensión no corrobora");
    printf("\n    %s - %s - %s - %s - %s - %s \n",
        str_vacio, str_sin_extension, str_correcto, str_incorrecto, str_con_dos_extensiones_correcto, str_con_dos_extensiones_incorrecto);
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