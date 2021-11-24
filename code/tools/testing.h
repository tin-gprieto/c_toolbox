#ifndef __TESTING_H__
#define __TESTING_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stddef.h>

#define RESET "\e[0;0m"
#define NEGRO "\e[0;30m"
#define ROJO "\e[0;31m"
#define VERDE "\e[0;32m"
#define AMARILLO "\e[0;33m"
#define AZUL "\e[0;34m"
#define CELESTE "\e[1;34m"
#define ROSA "\e[0;35m"
#define CYAN "\e[0;36m"
#define BLANCO "\e[0;37m"

#define TILDE "✓"
#define CRUZ "✗"

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

#endif /* __TESTING_H__ */