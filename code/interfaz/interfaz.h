#ifndef __INTERFAZ_H__
#define __INTERFAZ_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../tools/tools.h"

#define MAX_DESCRIPCION 80
#define MAX_COLOR 8

#define OPCION_SALIR 'S'
#define OPCION_VOLVER 'B'
#define OPCION_AVANZAR 'N'

typedef struct dimension{
    size_t max;
    size_t margen;
    size_t espaciado;
}dimension_t;

typedef struct estetica{
    char color_fondo[MAX_COLOR];
    char color_letras[MAX_COLOR];
    char color_titulos[MAX_COLOR];
}estetica_t;

typedef struct interfaz interfaz_t;

/*
* Función para menú información: Recibe la interfaz,
* un puntero a la información que se desae mostrar, y un puntero a un auxiliar.
* Se complementa con informacion_imprimir_linea para implementarla,
* y se ejecuta con mostrar_informacion.
*/
typedef void (*funcion_grafica_t)(interfaz_t*, void*, void*);

/*
* Dados los colores del fondo, las letras y el título devuelve 
* un struct estetica_t con esa información
* Pre : Colores válidos (constantes de tools.c)
* Post: Valores almacenados en estetica_t
*/
estetica_t interfaz_set_estetica(char color_fondo[MAX_COLOR], char color_letras[MAX_COLOR], char color_titulos[MAX_COLOR]);

/*
* Dados las longitudes de anchura maxima, margen y espaciado devuelve 
* un struct simension_t con esa información
* Pre : Maximo distinto a 0
* Post: Valores almacenados en dimension_t
*/
dimension_t interfaz_set_dimension(size_t maximo, size_t margen, size_t espaciado);

/* 
* Crea la interfaz en memoria dinamica
* Pre : dimensio y esteticas seteadas
* Post: puntero a interfaz lista para usarse o NULL en caso de algún error
*/
interfaz_t* interfaz_crear(dimension_t dimension, estetica_t estetica);

/* 
* Destruye el espacio asignado a la interfaz y a los menus
* Pre : Interfaz creada
* Post: Memoria liberada
*/
void interfaz_destruir(interfaz_t* interfaz);

/* 
* Devuelve el estado actual de la interfaz
* (Modo en el que debe ejecutarse según lo elegido por el usuario)
* Pre : Interfaz creada 
* Post: Elección del usuario
*/
char interfaz_estado(interfaz_t* interfaz);

/*
* Inserta en la interfaz un nuevo menú con su título
* Pre : Interfaz creada
* Post: Nuevo menú en la interfaz
*/
int interfaz_menu_insertar(interfaz_t* interfaz, char titulo[MAX_DESCRIPCION]);

/* 
* Dada una interfaz, carga una letra y su descripcion para el menú
* Opciones reservadas:- Salir:'S' - Volver:'B' - Avanzar:'N'
* Pre : interfaz creada, menú dentro del rango posible
* Post: Opcion y descripción cargados al final del menu
*/
void interfaz_menu_cargar_opcion(interfaz_t* interfaz, size_t pos_menu, char opcion, const char* descripcion);

/* 
* Elimina una de las opciones del menu
* Pre : Recibe un vector de menús, el tipo de menú en el que se encuetra
* y la opcion que se desea eliminar
* Post: Eliminar del vector opciones (dentro del menu) a la opcion deseada
*/
void interfaz_menu_eliminar_opcion(interfaz_t* interfaz, size_t pos_menu, char opcion);

/*
* Muestra un menú de la interfaz junto a sus opciones, dada su posición en la interfaz
* (orden que fue insertado)
* Pre : Interfaz creada, pos_menu menor a la cantidad de menús insertados
* Post: Información del menú por la pantalla y pide opción para avanzar (Las cargadas al menú + SALIR)
*/
void interfaz_menu_mostrar(interfaz_t* interfaz, size_t pos_menu);

/*
* Inserta un menú información a la interfaz, junto con su título y la función que muestre la información
* Pre : Interfaz creada y funcion_grafica_t implementada
* Post: Nuevo menú info en la interfaz
*/
int interfaz_informacion_insertar(interfaz_t* interfaz, char titulo[MAX_DESCRIPCION], funcion_grafica_t mostrar);

/*
* Imprime por pantalla una línea(string) dentro de la interfaz
* (Utilizar con funcion_grafica_t)
* Pre : Interfaz creada y color válido
* Post: Información por pantalla dentro de la interfaz
*/
void interfaz_informacion_imprimir_linea(interfaz_t* interfaz,  const char* color, const char* linea);

/*
* Dada una información y un auxiliar, muestra la información con el menú que corresponde:
* Pre : Interfaz creada, pos_info menor a la cantidad de menu_info, información y aux (puede ser NULL) 
* pasados que correspondan con los tipos de datos que maneja la función grafica de dicho menu.
* Post: Información con el menú impreso por la pantalla y pide opción para avanzar (AVANZAR)
*/
void interfaz_informacion_mostrar(interfaz_t* interfaz, size_t pos_menu, void* informacion, void* aux);

/*
* Dado un string constante, imprime por pantalla su mensaje de forma tal que
* el usuario sepa que hay un error
* Pre : Descripcion completa del problema
* Post: Información por pantalla
*/
void interfaz_reportar_error(interfaz_t* interfaz, const char *descripcion);

/* 
* Dada una descripcion, pide al usuario una cadena de caracteres
* Pre : Interfaz creada
* Post: Cadena de caracteres ingresada por el usuario (hasta \n)
*/
char* interfaz_pedir_string(interfaz_t* interfaz, const char* descripcion);

/* 
* Pide una ruta de archivo al usuario y devuelve como retorno 
* Pre : Interfaz creada
* Post: ruta válida del archivo (sin fallo de apertura)
*/
char* interfaz_pedir_archivo(interfaz_t* interfaz, const char* extension, const char* descripcion);

#endif /* __INTERFAZ_H__ */