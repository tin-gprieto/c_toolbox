#include "interfaz.h"
#include "../tools/tools.h"

#define MAX_STRING 100
#define MAX_OPCIONES 10

typedef struct menu{
    char titulo[MAX_DESCRIPCION];
    char opciones[MAX_OPCIONES];
    char descripciones[MAX_OPCIONES][MAX_DESCRIPCION];
    size_t cant_opciones;
    size_t cant_opc_fijas;
}menu_t;

typedef struct menu_info{
    menu_t menu;
    funcion_grafica_t mostrar;
}menu_info_t;

typedef struct interfaz{
    char estado;
    dimension_t dimension;
    estetica_t estetica;
    menu_t* menus;
    size_t cant_menus;
    menu_info_t* infos;
    size_t cant_infos;
}interfaz_t;

#define FONDO "\e[1;44m"
#define LIMPIAR "clear"

static const char INTERFAZ_INICIAL = ' ';
static const int INICIO = 0;
static const int FIN = 1;
static const size_t LONGITUD_OPCION = 11;
static const size_t LONGITUD_BARRA = 40;

#define SEP_EXTENSION "."

#define DESCRIPCION_SALIR  "Salir"
#define DESCRIPCION_VOLVER  "Volver"
#define DESCRIPCION_AVANZAR  "Avanzar"
#define DESCRIPCION_AVANZAR_INFO  "Avanzar / Salir"

/******************************     GRÁFICA     ******************************************/

/* 
* Imprime el margen de la interfaz
* Pre : -
* Post: Espacios por pantalla
*/
void imprimir_margen(size_t margen){
    for(int i=0; i < margen; i++)
            printf(" ");
}
/* 
* Dado string identificado de color (constantes) 
* imprime por pantalla dicho color
* Pre : Color válido entre las opciones
* Post: Color próximo para un printf
*/
void determinar_color(const char * color){
    if(strcmp(color, AMARILLO) == 0)
        printf(AMARILLO);
    else if(strcmp(color, NEGRO) == 0)
        printf(NEGRO);
    else if(strcmp(color, ROSA) == 0)
        printf(ROSA);
    else if(strcmp(color, AZUL) == 0)
        printf(AZUL);
    else if(strcmp(color, CELESTE) == 0)
        printf(CELESTE);
    else if(strcmp(color, CYAN) == 0)
        printf(CYAN);
    else if(strcmp(color, ROJO) == 0)
        printf(ROJO);
    else if(strcmp(color, VERDE) == 0)
        printf(VERDE);
    else if(strcmp(color, BLANCO) == 0)
        printf(BLANCO);
    else if(strcmp(color,RESET) == 0)
        printf(RESET);
}
/* 
* Dado string identificado de color FONDO(constantes) 
* imprime por pantalla dicho color
* Pre : Color fondo válido entre las opciones
* Post: Color fondo próximo para un printf
*/
void determinar_color_fondo(const char * color){
    if(strcmp(color, FONDO_AMARILLO) == 0)
        printf(FONDO_AMARILLO);
    else if(strcmp(color, FONDO_NEGRO) == 0)
        printf(FONDO_NEGRO);
    else if(strcmp(color, FONDO_ROJO) == 0)
        printf(FONDO_ROJO);
    else if(strcmp(color, FONDO_VERDE) == 0)
        printf(FONDO_VERDE);
    else if(strcmp(color, FONDO_AZUL) == 0)
        printf(FONDO_AZUL);
    else if(strcmp(color, FONDO_ROSA) == 0)
        printf(FONDO_ROSA);
    else if(strcmp(color, FONDO_CYAN) == 0)
        printf(FONDO_CYAN);
    else if(strcmp(color, FONDO_BLANCO) == 0)
        printf(FONDO_BLANCO);
}
/* 
*  Imprime un espacio en la interfaz para alinear texto
* Pre : Recibe que tipo de margen es, si INICIO o FIN del renglón
* y la longitud de dicho margen
* Post: Imprime por pantalla un margen con espacio vacío (con fondo)
*/
void imprimir_espacio(interfaz_t* interfaz, int tipo_espacio, size_t espaciado){
    if(tipo_espacio == INICIO)
        imprimir_margen(interfaz->dimension.margen);
    determinar_color_fondo(interfaz->estetica.color_fondo);
    for(int i=0; i < espaciado; i++)
        printf(" ");
    if(tipo_espacio == FIN){
        determinar_color(RESET);
        printf("\n");
    }
}
/* 
* Imprime un renglón dentro de la interfaz
* Pre : -
* Post: Un linea vacía en la interfaz
*/
void imprimir_enter(interfaz_t* interfaz){
    imprimir_margen(interfaz->dimension.margen);
    imprimir_espacio(interfaz, FIN, interfaz->dimension.max);
    determinar_color(RESET);
}
/* 
* Pre : Recibe que tipo de marco es, si INICIO o FIN de la pantalla
* Post: Imprime por pantalla el marco de pantalla 
* (indica el inicio y fin la interfaz)
*/
void imprimir_marco(interfaz_t* interfaz, int marco){
    if(marco == FIN)
        imprimir_enter(interfaz);

    imprimir_margen(interfaz->dimension.margen);
    for(int i=0; i< interfaz->dimension.max; i++){
        if(marco == FIN)
            determinar_color_fondo(interfaz->estetica.color_fondo);
        printf("_");
    }
    determinar_color(RESET);
    printf("\n");
    if(marco == INICIO)
        imprimir_enter(interfaz);
}
/* 
* Pre : Recibe algún color AMARILLO, ROJO o VERDE
* Post: Devuelve por pantalla una barra, según el color,
* en el centro de la pantalla (con fondo y margen)
*/
void imprimir_barra(interfaz_t* interfaz, int barra, const char* color){
    size_t espaciado = (interfaz->dimension.max - LONGITUD_BARRA) / 2; 
    imprimir_espacio(interfaz, INICIO, espaciado);
    determinar_color(color);
    determinar_color_fondo(interfaz->estetica.color_fondo);
    printf("________________________________________"RESET);
    imprimir_espacio(interfaz, FIN, interfaz->dimension.max - LONGITUD_BARRA - espaciado);
    if(barra==INICIO)
        imprimir_enter(interfaz);
}
/* 
* Imprime una linea completa de interfaz dado un contenido
* Pre : margen y extensión del string menor al limite, color válido entre las opciones
* Post: Un linea de texto alineada en la interfaz
*/
void imprimir_linea(interfaz_t* interfaz, size_t espaciado, const char* color, const char* linea){
    imprimir_enter(interfaz);
    imprimir_espacio(interfaz, INICIO, espaciado);
    determinar_color(color);
    determinar_color_fondo(interfaz->estetica.color_fondo);
    printf("%s", linea);
    determinar_color(RESET);
    size_t limite = interfaz->dimension.max - espaciado - strlen(linea);
    imprimir_espacio(interfaz, FIN, limite);
    imprimir_enter(interfaz);
}
/* 
* Imprime sobre una misma linea dos strings con eleccion de color por separado
* Pre : margen y extensión de los strings menor al limite, 
* colores válidos entre las opciones
* Post: Un linea de texto alineada en la interfaz
*/
void imprimir_linea_partida(interfaz_t* interfaz, const char* color_1, const char* linea_1, const char* color_2, const char* linea_2){
    imprimir_enter(interfaz);
    imprimir_espacio(interfaz, INICIO, interfaz->dimension.espaciado);
    size_t limite;
    determinar_color(color_1);
    determinar_color_fondo(interfaz->estetica.color_fondo);
    printf("%s", linea_1);
    determinar_color(RESET);
    determinar_color(color_2);
    determinar_color_fondo(interfaz->estetica.color_fondo);
    printf("%s", linea_2);
    determinar_color(RESET);
    limite = interfaz->dimension.max - interfaz->dimension.espaciado - strlen(linea_1) - strlen(linea_2);
    imprimir_espacio(interfaz, FIN, limite);
    imprimir_enter(interfaz);
}

//interfaz.h
void reportar_error(interfaz_t* interfaz, const char *descripcion){
    imprimir_margen(interfaz->dimension.margen);
    printf(ROJO "%s - ERROR - %s \n" RESET, CRUZ, descripcion);
    system("sleep 2");
}

/***********************************      GESTIÓN DE MENÚ     *******************************************/

/* 
* Dada un letra, lo busca en un vector de letras
* Pre : cantidad de opciones
* Post: Devuelve la posicion de la letra dentro del vector
*/
size_t buscar_opcion(char opciones[], size_t tope, char opcion){
    bool encontro = false;
    size_t pos = 0;
    while( (pos < tope) && (!encontro)){
        if(opciones[pos]  == opcion)
            encontro = true;
        else
            pos++;
    }
    if(!encontro)
        return tope;
    return pos;
}
/* 
* Intercambia las letras según sus posiciones en el array
* Pre : posiciones válidas (menores al tope)
* Post: letras intercambiadas
*/
void swap_letra(char opciones[MAX_OPCIONES], size_t letra_1, size_t letra_2){
    char aux = opciones[letra_1];
    opciones[letra_1] = opciones[letra_2];
    opciones[letra_2] = aux;
}
/* 
* Manda la letra de la posicion al final
* Pre : posicion de la letra a eliminar
* Post: letra a eliminar al final del vector
*/
void eliminar_letra(char opciones[], size_t pos, size_t tope){
    if(pos >= tope - 1)
        return;
    for(size_t i = pos; i < tope - 1; i++)
        swap_letra(opciones, i, i + 1);
        
}
/* 
* Intercambia los strings según sus posiciones en el array
* Pre : posiciones válidas (menores al tope)
* Post: strings intercambiadas
*/
void swap_string(char descripciones[MAX_OPCIONES][MAX_DESCRIPCION], size_t str_1, size_t str_2){
    char aux[MAX_STRING];
    strcpy(aux, descripciones[str_1]);
    strcpy(descripciones[str_1], descripciones[str_2]);
    strcpy(descripciones[str_2], aux);
}
/* 
* Manta el string de la posicion al final
* Pre : posicion del string a eliminar
* Post: string a eliminar al final del vector
*/
void eliminar_descripcion(char descripciones[MAX_OPCIONES][MAX_DESCRIPCION], size_t pos, size_t tope){
    if(pos >= tope - 1)
        return;
    for(size_t i = pos; i < tope - 1; i++)
        swap_string(descripciones, i, i + 1);
}
/* 
* Valida la clave ingresada por el usuario
* Pre : Recibe una clave y las opciones que tiene para ser válida
* Post: Devuelve verdader si la clave cumple con alguna de las opciones
*/
bool clave_valida(char clave, char opciones[], size_t cantidad){
    bool validez = false;
    int i = 0;
    while(i < cantidad && !validez){
        if(opciones[i] == clave)
            validez = true;
        i++;
    }
    return validez;
}
/* 
* Pide alguna de las opciones y devuelve la letra que ingrese el usuario
* Pre : Recibe todas las opciones posibles y la cantidad de las mismas
* Post: Devuelve un letra ingresada y válida dentro de las opciones
*/
char pedir_clave(interfaz_t* interfaz, char* opciones, size_t cantidad){
    imprimir_margen(interfaz->dimension.margen);
    printf("Ingrese alguna opción : ");
    char clave;
    scanf(" %c", &clave);
    while(!clave_valida((char)clave, opciones, cantidad)){
        limpiar_buffer();
        reportar_error(interfaz, "Opcion inválida");
        imprimir_margen(interfaz->dimension.margen);
        printf("Ingrese nuevamente una opción : ");
        scanf(" %c", &clave);
    }
    limpiar_buffer();
    return clave;

}
/* 
* Imprime la opción de un menu
* Pre : Recibe la clave y la descripción de una opción
* Post: Imprime la información alineada a la interfaz 
*/
void imprimir_opcion(interfaz_t* interfaz, menu_t menu, size_t pos){
    char linea_opcion[LONGITUD_OPCION];            
    char linea_descripcion[interfaz->dimension.max - LONGITUD_OPCION];
    sprintf(linea_opcion," >> ( %c )  ", menu.opciones[pos]);             
    sprintf(linea_descripcion,"-  %s", menu.descripciones[pos]);             
    imprimir_linea_partida(interfaz, interfaz->estetica.color_titulos, linea_opcion, interfaz->estetica.color_letras, linea_descripcion);   
}
/* 
* Muestra por pantalla todas las opciones de un menu
* Pre : tipo_menu válido dentro los menus de interfaz, interfaz no nula
* Post: Opciones del menu en pantalla
*/
void menu_mostrar_opciones(interfaz_t* interfaz, menu_t menu){
    for(size_t pos = menu.cant_opc_fijas; pos < menu.cant_opciones; pos++)
        imprimir_opcion(interfaz, menu, pos);

    imprimir_barra(interfaz, INICIO, interfaz->estetica.color_titulos);

    for(size_t pos = 0; pos <  menu.cant_opc_fijas; pos++)
        imprimir_opcion(interfaz, menu, pos);

    imprimir_marco(interfaz, FIN);
    interfaz->estado = pedir_clave(interfaz, menu.opciones, menu.cant_opciones);
}
/*
* Dada la interfaz y la posición de un menú, imprime el encabezado del menú para mostrar 
* Pre : Comienzo de la interfaz para el menú, interfaz creada y posicion de un menú válida
* Post: Encabezado del menú (con el título) impreso por pantalla
*/
void menu_encabezado(interfaz_t* interfaz, size_t pos_menu){
    size_t espaciado = (interfaz->dimension.max - strlen(interfaz->menus[pos_menu].titulo)) / 2;
    imprimir_marco(interfaz, INICIO);
    imprimir_enter(interfaz);
    imprimir_barra(interfaz, INICIO, interfaz->estetica.color_titulos);
    imprimir_linea(interfaz, espaciado, interfaz->estetica.color_titulos, interfaz->menus[pos_menu].titulo);
    imprimir_barra(interfaz, FIN, interfaz->estetica.color_titulos);
    imprimir_enter(interfaz);
}
/*
* Dada la interfaz y la posición de un menú, imprime el encabezado del menú para mostrar 
* Pre : Comienzo de la interfaz para el menú, interfaz creada y posicion de un menú válida
* Post: Encabezado del menú (con el título) impreso por pantalla
*/
void info_encabezado(interfaz_t* interfaz, size_t pos_menu){
    size_t espaciado = (interfaz->dimension.max - strlen(interfaz->infos[pos_menu].menu.titulo)) / 2;
    imprimir_marco(interfaz, INICIO);
    imprimir_linea(interfaz, espaciado, interfaz->estetica.color_titulos, interfaz->infos[pos_menu].menu.titulo);
    imprimir_barra(interfaz, FIN, interfaz->estetica.color_titulos);
    imprimir_enter(interfaz);
}
/*
* Dado un string corrobora que sea un archivo txt y
* Pre : String pedido al usuario y menor a MAX_STRING
* Post: Verdadero si abre correctamente un archivo y es txt
*/
bool ruta_archivo_valida(char* ruta, const char* extension){
    char* partido = strtok(ruta, SEP_EXTENSION);
    partido = strtok(NULL, SEP_EXTENSION);
    if(!partido || strcmp(partido, extension) != 0)
        return false;
    strcat(ruta, SEP_EXTENSION);
    strcat(ruta, extension);
    FILE* prueba = fopen(ruta, LECTURA);
    if(!prueba)
        return false;
    fclose(prueba);
    return true;
}
/*
* Carga una opcion y su descripción a la última posicion de un menú
* Pre : Menu creado
* Post: Menu con nueva opción cargada
*/
void cargar_opcion(menu_t* menu, char opcion, const char* descripcion){
    if(!menu) return;
    size_t tope = menu->cant_opciones;
    menu->opciones[tope] = opcion;
    strcpy(menu->descripciones[tope], descripcion);
    menu->cant_opciones ++;
}

/*
* Evalua si el tipo de menu es un tipo válido
* Pre : tipo de menu ingresado en menu_insertar
* Post: Verdadero si el tipo corresponde a alguna de las constantes
*/
bool tipo_menu_valido(size_t tipo_menu){
    return tipo_menu == TIPO_MENU_INICIO || tipo_menu == TIPO_MENU_MEDIO || tipo_menu == TIPO_MENU_FINAL;
}

/*
* Inicializa las opciones fijas del menu según su tipo
* Pre : Menú en posicion ya creado, tipo de menu válido
* Post: Deja al menú en posición con sus opciones fijas cargadas 
* y la cantidad de opciones_fijas que corresponde
*/
int menu_inicializar_opciones(interfaz_t* interfaz, size_t pos_menu, size_t tipo_menu){
    if(pos_menu == 0){
        menu_cargar_opcion(interfaz, pos_menu, OPCION_SALIR, DESCRIPCION_SALIR);
        interfaz->menus[pos_menu].cant_opc_fijas = 1;
        return EXITO;
    }else{
        if(tipo_menu == TIPO_MENU_MEDIO){
            menu_cargar_opcion(interfaz, pos_menu, OPCION_AVANZAR, DESCRIPCION_AVANZAR);
            menu_cargar_opcion(interfaz, pos_menu, OPCION_VOLVER, DESCRIPCION_VOLVER);
            menu_cargar_opcion(interfaz, pos_menu, OPCION_SALIR, DESCRIPCION_SALIR);
            interfaz->menus[pos_menu].cant_opc_fijas = 3;
            return EXITO;
        }else if(tipo_menu == TIPO_MENU_FINAL){
            menu_cargar_opcion(interfaz, pos_menu, OPCION_VOLVER, DESCRIPCION_VOLVER);
            menu_cargar_opcion(interfaz, pos_menu, OPCION_SALIR, DESCRIPCION_SALIR);
            interfaz->menus[pos_menu].cant_opc_fijas = 2;
            return EXITO;
        }
    }
    return ERROR;
}

/****************************       FUNCIONES INTERFAZ .H       ***********************************/

//interfaz.h
estetica_t set_estetica(char color_fondo[MAX_COLOR], char color_letras[MAX_COLOR], char color_titulos[MAX_COLOR]){
    estetica_t estetica;
    strcpy(estetica.color_fondo, color_fondo);
    strcpy(estetica.color_letras, color_letras);
    strcpy(estetica.color_titulos, color_titulos);
    return estetica;
}

//interfaz.h
dimension_t set_dimension(size_t maximo, size_t margen, size_t espaciado){
    dimension_t dimension;
    dimension.max = maximo;
    dimension.margen = margen;
    dimension.espaciado = espaciado;
    return dimension;
}

//interfaz.h
void interfaz_destruir(interfaz_t* interfaz){
    if(!interfaz) return;
    free(interfaz->menus);
    free(interfaz->infos);
    free(interfaz);
}

//interfaz.h
interfaz_t* interfaz_crear(dimension_t dimension, estetica_t estetica){
    interfaz_t* interfaz = malloc(sizeof(interfaz_t));
    if(!interfaz) return NULL;
    interfaz->estado = INTERFAZ_INICIAL;
    
    interfaz->dimension.max = dimension.max;
    interfaz->dimension.margen = dimension.margen;
    interfaz->dimension.espaciado = dimension.espaciado;
    
    strcpy(interfaz->estetica.color_fondo, estetica.color_fondo);
    strcpy(interfaz->estetica.color_letras, estetica.color_letras);
    strcpy(interfaz->estetica.color_titulos, estetica.color_titulos);

    interfaz->menus = NULL;
    interfaz->infos = NULL;
    interfaz->cant_menus = 0;
    interfaz->cant_infos = 0;
    
    return interfaz;
}

//interfaz.h
void menu_eliminar_opcion(interfaz_t* interfaz, size_t pos_menu, char opcion){
    if(!interfaz || pos_menu >= interfaz->cant_menus){
        reportar_error(interfaz, "Interfaz o Menú inválidos");
        return;
    }
    size_t cantidad = interfaz->menus[pos_menu].cant_opciones;
    size_t pos = buscar_opcion(interfaz->menus[pos_menu].opciones,cantidad, opcion);
    if(pos == ERROR){
        reportar_error(interfaz, "Opción a eliminar inválida");
        return;
    }
    eliminar_letra(interfaz->menus[pos_menu].opciones, pos, cantidad);
    eliminar_descripcion(interfaz->menus[pos_menu].descripciones, pos, cantidad);
    interfaz->menus[pos_menu].cant_opciones --;
}

//interfaz.h
void menu_cargar_opcion(interfaz_t* interfaz, size_t pos_menu, char opcion, const char* descripcion){
    if (!interfaz || pos_menu >= interfaz->cant_menus){
        reportar_error(interfaz, "Interfaz o Menú inválidos");
        return;
    }
    cargar_opcion(&(interfaz->menus[pos_menu]), opcion, descripcion);
}

//interfaz.h
int menu_insertar(interfaz_t* interfaz, char titulo[MAX_DESCRIPCION], size_t tipo_menu){
    if(!interfaz || !titulo || !tipo_menu_valido(tipo_menu))
        return ERROR;
    size_t espaciado = (interfaz->dimension.max - strlen(titulo)) / 2;
    if(strlen(titulo) > interfaz->dimension.max - espaciado)
        return ERROR;
    size_t tope = interfaz->cant_menus;
    if(tope != 0 && tipo_menu == TIPO_MENU_INICIO)
        return ERROR;
    menu_t* aux = realloc(interfaz->menus, (size_t) sizeof(menu_t)*(tope + 1));
    if(!aux){
        interfaz_destruir(interfaz);
        return ERROR;
    }
    interfaz->menus = aux;
    interfaz->cant_menus ++;
    strcpy(interfaz->menus[tope].titulo, titulo);
    interfaz->menus[tope].cant_opciones = 0;
    interfaz->menus[tope].cant_opc_fijas = 0;
    menu_inicializar_opciones(interfaz, tope, tipo_menu);
    return EXITO;
}

//interfaz.h
void menu_mostrar(interfaz_t* interfaz, size_t pos_menu){
    if(!interfaz || pos_menu >= interfaz->cant_menus){
        reportar_error(interfaz, "Interfaz o Menú inválidos");
        return;
    }
    system(LIMPIAR);
    menu_encabezado(interfaz, pos_menu);
    menu_mostrar_opciones(interfaz, interfaz->menus[pos_menu]);
}

//interfaz.h 
int informacion_insertar(interfaz_t* interfaz, char titulo[MAX_DESCRIPCION], funcion_grafica_t mostrar){
    if(!interfaz || !mostrar || !titulo) 
        return ERROR;
    size_t espaciado = (interfaz->dimension.max - strlen(titulo)) / 2;
    if(strlen(titulo) > interfaz->dimension.max - espaciado)
        return ERROR;
    size_t tope = interfaz->cant_infos;
    menu_info_t* aux = realloc(interfaz->infos, (size_t) sizeof(menu_info_t)*(tope + 1));
    if(!aux){
        interfaz_destruir(interfaz);
        return ERROR;
    }
    interfaz->infos = aux;
    interfaz->infos[tope].mostrar = mostrar;

    strcpy(interfaz->infos[tope].menu.titulo, titulo);
    interfaz->infos[tope].menu.cant_opciones = 0;
    cargar_opcion(&(interfaz->infos[tope].menu), OPCION_AVANZAR, DESCRIPCION_AVANZAR_INFO);
    cargar_opcion(&(interfaz->infos[tope].menu), OPCION_VOLVER, DESCRIPCION_VOLVER);
    interfaz->infos[tope].menu.cant_opc_fijas = 2;
    interfaz->cant_infos++;
    return EXITO;
}

//interfaz.h 
void informacion_linea(interfaz_t* interfaz, const char* color, const char* linea){
    if (!interfaz){
        reportar_error(interfaz, "Interfaz inválida");
        return;
    }
    imprimir_linea(interfaz, interfaz->dimension.espaciado, color, linea);
}

//interfaz.h 
void informacion_mostrar(interfaz_t* interfaz, size_t menu_info, void* informacion, void* aux){
    if(!interfaz || menu_info >= interfaz->cant_infos){
        reportar_error(interfaz, "Interfaz o Menú inválidos");
        return;
    }
    system(LIMPIAR);
    info_encabezado(interfaz, menu_info);
    funcion_grafica_t funcion = interfaz->infos[menu_info].mostrar;
    funcion(interfaz, informacion, aux);
    menu_mostrar_opciones(interfaz, interfaz->infos[menu_info].menu);
}

//interfaz.h
char interfaz_estado(interfaz_t* interfaz){
    return interfaz->estado;
}

//interfaz.h
void interfaz_cambiar_estado(interfaz_t* interfaz, char nuevo_estado){
    if(!interfaz)
        return;
    interfaz->estado = nuevo_estado;
}

//interfaz.h
char* pedir_string(interfaz_t* interfaz, const char* descripcion){
    imprimir_margen(interfaz->dimension.margen);
    printf("Ingrese " VERDE "%s" RESET " que desea : ", descripcion);
    char buffer[MAX_STRING];
    char* str ;
    str = leer_linea(buffer, MAX_STRING, stdin);
    return str;
}

//interfaz.h
char* pedir_archivo(interfaz_t* interfaz, const char* extension, const char* descripcion){
    imprimir_margen(interfaz->dimension.margen);
    printf("Ingrese la ruta del archivo de " VERDE "%s" RESET ":", descripcion);
    char buffer[MAX_STRING];
    char* ruta_archivo;
    ruta_archivo = leer_linea(buffer, MAX_STRING, stdin);
    while(!ruta_archivo_valida(ruta_archivo, extension)){
        reportar_error(interfaz, "Hubo un problema con la ruta ingresada");
        imprimir_margen(interfaz->dimension.margen);
        printf("Ingrese nuevamente : ");
        ruta_archivo = leer_linea(buffer, MAX_STRING, stdin);
    }
    return ruta_archivo;
}