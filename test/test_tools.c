#include "../code/tools/tools.h"

int main(){
    char* str = barra_porcentaje(4, 160, 45);
    printf("     Barra:     %s      \n", str);
    free(str);
    return 0;
}