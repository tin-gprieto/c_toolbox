Toolbox
=============

## Descripción 

Toolbox es un conjunto de archivos que integran mis herramientas personales a la hora de implementar programas en lenguaje C.
Principalmente está compuesto por varias carpetas que clasifican las herramientas, las cuales son:
- **Interfaz** : Conjunto de funciones para mostrar por terminal una interfaz de interación con el usuario. El objetivo principal de éstas funciones es poder brindar, en una estética agradable, una interacción fluida con el usuario dónde se le pida ingresar ópciones o mostrar información.
- **TDAs**: Implementación de tipos de datos abstractos básicos que no se encuentran en C nativo. Las estructuras de datos que componen el archivo son: Lista, Cola, Pila, Árbol de búsqueda binario, Heap y Hash.
- **Tools**: Funciones particulares que ayudan en la útilizacíon del lenguaje y el testeo unitario.

## Interfaz

La interfaz consta de varias partes:
1. Creación y utilización 
2. Menú de opciones
3. Menú de Información
4. Funcionalidades extra
5. Glosario de constantes

### 1.Creación y utilización

- Funciones **Set** : Son dos funciones, **estética** y **dimensión**, que son requeridas para crear una interfaz, se pueden utilizar para inicializar variables o directamente en la función de creación.

        estetica_t set_estetica(char* color_fondo, char* color_letras, char* color_titulos)  *1
    
        dimension_t set_dimension(size_t maximo, size_t margen, size_t espaciado)

*1 - Los colores disponibles se encuentran en el glosario de constantes

- Funciones **Crear** : La estructura necesaria para la utilzación de la interfaz es creada en memoria dinámica, por ésto mismo tiene una función para **crearla** (de la interfaz unícamente con la estética y las dimensiones) y otra función para **destruirla** (por completo, incluyendo los menús agregados).

        interfaz_t* interfaz_crear(dimension_t dimension, estetica_t estetica)

        void interfaz_destruir(interfaz_t* interfaz)

- Función **Estado** : El estado de la interfaz, devuelto por ésta función, tiene como significado la última opción que haya elegido el usuario en algún menú.

        char interfaz_estado(interfaz_t* interfaz)

### 2.Menú de opciones

El menú de opciones básicamente consiste en un menú en dónde el implementador pueda ir agragando las diferentes **opciones personalizadas** que crea necesarias en su programa. Las funciones básicas para el funcionamiento de los menús son:

- Agrega un nuevo menú a la interfaz (con su título)

        int menu_insertar(interfaz_t* interfaz, char* titulo)

- Agrega una nueva opción al menú en posicion (junto a una descripción)
    
        void menu_cargar_opcion(interfaz_t* interfaz, size_t pos_menu, char opcion, const char* descripcion)

- Elimina la opción del menú en posicion

        void menu_eliminar_opcion(interfaz_t* interfaz, size_t pos_menu, char opcion)

- Muestrá el menú por pantalla, pidiendole una opción al usuario

        void menu_mostrar(interfaz_t* interfaz, size_t pos_menu)

Además de las opciones personalizadas, hay ciertas **opciones por defecto**, que estarán vinculadas con la desición de salir del menú o volver al anterior, éstas opciones van a estar relacionadas según en el orden que se agreguen los menús a la interfaz. Para el primer menú agregado, es decir el **Menú Inicio**, unicamente tendrá la opción de salir; en cambio, para los próximos, los **Menú Desarrollo**, tendrá la opción de volver y de salir (ver constantes en el glosario).

#### Menú de opciones Inicio
![](/screenshots/Menu_Inicio.png?raw=true)

#### Menú de opciones Desarrollo
![](/screenshots/Menu_Desarrollo.png?raw=true)

### 3.Menú información

Este menú se diferencia del menú de opciones ya que su función principal es mostrarle información al usuario. En este caso, el menú cuenta, además de con su título, con opciones por defecto (volver o salir) y la información dada. 

Para mostrar dicha información se requiere inicializar el menú con una función del estilo de: 

        void (*funcion_grafica_t)(interfaz_t*, void*, void*)

- Para la cual se podrá hacer uso de una función auxiliar, que imprime dentro de la interfaz:

        void informacion_imprimir_linea(interfaz_t* interfaz,  const char* color, const char* linea)  *1

*1 - Los colores disponibles se encuentran en el glosario de constantes

Por el otro lado, las **funciones de utilización** son:

- Inserta un nuevo menú de información a la interfaz, junto a su título y la función con la que muestra los datos:

        int informacion_insertar(interfaz_t* interfaz, char* titulo, funcion_grafica_t mostrar)
    
- Muestra el menú información en la posicion junto con los datos que debe recibir el menú según su función:

        void informacion_mostrar(interfaz_t* interfaz, size_t pos_menu, void* informacion, void* aux)

![](/screenshots/Menu_Información.png?raw=true)

### 4.Funcionalidades extra

Estas funciones adicionales consisten en interacciones con el usuario dentro del marco de la interfaz, éstas son:

- Advertencia según una breve descripción:

        void interfaz_reportar_error(interfaz_t* interfaz, const char *descripcion)

- Pide un string dada una descripcion de la cadena a pedir:

        char* interfaz_pedir_string(interfaz_t* interfaz, const char* descripcion)

- Pide la ruta de un archivo (chequeando que puede abrirse) dada un descripción:

        char* interfaz_pedir_archivo(interfaz_t* interfaz, const char* extension, const char* descripcion)

### 5.Glosario de constantes

#### Colores (Secuencia de escape ANSI)
(Incluidas en tools.c)

    NEGRO            "\e[0;30m"
    ROJO             "\e[0;31m"
    VERDE            "\e[0;32m"
    AMARILLO         "\e[0;33m"
    AZUL             "\e[0;34m"
    CELESTE          "\e[1;34m"
    ROSA             "\e[0;35m"
    CYAN             "\e[0;36m"
    BLANCO           "\e[0;37m"

    FONDO_NEGRO      "\e[1;40m"
    FONDO_ROJO       "\e[1;41m"
    FONDO_VERDE      "\e[1;42m"
    FONDO_AMARILLO   "\e[1;43m"
    FONDO_AZUL       "\e[1;44m"
    FONDO_ROSA       "\e[1;45m"
    FONDO_CYAN       "\e[1;46m"
    FONDO_BLANCO     "\e[107m"

#### Opciones por defecto

    OPCION_SALIR 'S'
    OPCION_VOLVER 'V'
