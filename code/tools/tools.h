#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>
#include <stddef.h>

#define ERROR -1
#define EXITO 0

#define RESET "\e[0;0m"
#define LIMPIAR "clear"

//apertura de archivos
#define LECTURA "r"
#define ESCRITURA "w"
#define ADJUNTAR "a"

// FONDO "\e[0;47m"
// SUBRAYADO "\e[4;37m"
// PARPADEO "\e[5;37m"
#define NEGRO "\e[0;30m"
#define ROJO "\e[0;31m"
#define VERDE "\e[0;32m"
#define AMARILLO "\e[0;33m"
#define AZUL "\e[0;34m"
#define CELESTE "\e[1;34m"
#define ROSA "\e[0;35m"
#define CYAN "\e[0;36m"
#define BLANCO "\e[0;37m"

#define FONDO_NEGRO "\e[1;40m"
#define FONDO_ROJO "\e[1;41m"
#define FONDO_VERDE "\e[1;42m"
#define FONDO_AMARILLO "\e[1;43m"
#define FONDO_AZUL "\e[1;44m"
#define FONDO_ROSA "\e[1;45m"
#define FONDO_CYAN "\e[1;46m"
#define FONDO_BLANCO "\e[107m"

#define SI 's'
#define NO 'n'

#define TILDE "✓"
#define CRUZ "✗"

#define MAX_LINES 80
#define MAX_COLS 80

/*
*Imprime por pantalla un array de numeros dada la cantidad de elementos
*/
void print_array(int* array, size_t size);

/*
* Devuelve una barra de porcentaje, teniendo el total y la cantidad sobre el mismo
* Pre : cantidad < total, ni total ni tamanio == 0
* Post: NULL en caso de error o puntero a str con barra (SE DEBE LIBERAR)
*/
char* barra_porcentaje(size_t cantidad, size_t total, size_t tamanio_barra);

/*
*Imprime por pantalla una advertencia según description
*/
void warning(const char * descripcion);

/* 
* Limpia el buffer de lectura siempre que exista y no sea un /n
* Pre : -
* Post: Buffer limpio para la siguiente lectura
*/
void limpiar_buffer();

/* 
* Lee una linea de un FILE* utilizando un buffer externo y la devuelve
* FILE* stdin para leer de la terminal
* Pre : tam_buffer mayor a 0, FILE* distinto a NULL
* Post: Linea leida de como máximo el tamanio del buffer o NULL en caso de error
* (buffer liberado para una próxima lectura)
*/
char* leer_linea(char* buffer, int tam_buffer, FILE* archivo);

/*
*Imprime por pantalla una prueba con su descripcion
*Si la afirmación no se cumple se considera un error
*/
void test_afirmar(int afirmacion, const char *descripcion);

/*
*Muestra por pantalla el título para un nuevo grupo
*/
void test_nuevo_grupo(const char *descripcion);

/*
*Muestra por pantalla el título y una separación para un
*nuevo subgrupo
*/
void test_nuevo_sub_grupo(const char *descripcion);

/*
*Muestra por pantalla la cantidad de pruebas corridas y errores
*/
void test_mostrar_reporte();

#endif /* __TOOLS_H__ */