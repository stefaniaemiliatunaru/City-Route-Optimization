/* TUNARU Stefania-Emilia 311CB*/
#include "header.h"

int main(int argc, char * argv[]) {
    char cerinta[OSUTA];
    snprintf(cerinta, strlen(argv[1])+1, "%s", argv[1]);
    /* Task 1*/
    if (strcmp(cerinta, "1") == 0) {
        char matrice[OMIE][OSUTA] = {{0}};
        TGL* harta = CitGraf(matrice);  // construiesc graful
        CompConex(harta);
        DistrG(&harta);
    }
    if (strcmp(cerinta, "2") == 0) {
        char matrice[OMIE][OSUTA] = {{0}};
        int greutate = 0;
        TGL2* harta = CitGraf2(matrice, &greutate);  // construiesc graful
        Dijkstra(1, harta, matrice, greutate);
        DistrG2(&harta);
    }
    return 0;
}
