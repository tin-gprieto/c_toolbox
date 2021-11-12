#include "TDAs/test_abb.h"
#include "TDAs/test_hash.h"
#include "TDAs/test_heap.h"
#include "TDAs/test_lista.h"
#include "TDAs/test_cola.h"
#include "TDAs/test_pila.h"
#include "../code/tools/testing.h"

int main(){
    pruebas_lista();
    pruebas_cola();
    pruebas_pila();
    pruebas_abb();
    pruebas_heap();
    pruebas_hash();
    test_mostrar_reporte();
    return 0;
}