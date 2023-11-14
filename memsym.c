#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_LINEA 16
#define NUM_ROWS




void CleanCACHE(T_CACHE_LINE tbl[NUM_ROWS]);
void DumpCACHE(T_CACHE_LINE *tbl);
void ParseAdress(unsigned int addr, int *LABEL, int*word, int *line, int *block);
void TreatFailureMiss(T_CACHE_LINE *tbl, char *MRAM, int LABEL,int line, int block);

typedef struct {
    unsigned char ETQ;
    unsigned char Data[TAM_LINEA];
    } T_CACHE_LINE;



char* leeLineaDinamicaFichero(FILE * f) {
    char * linea;
    char c = 0;
    int count = 0;
    
    while ((c = getc(f) != '\n' && !feof(f))) {
        linea = (char*)realloc(linea, sizeof(char) * (count +1));
        linea[count] = c;
        count++;
    }
    linea[count] = '\0';
    return linea;
}