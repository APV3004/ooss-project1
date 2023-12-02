#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_LINEA 16
#define NUM_ROWS 8
#define MEM_SIZE 4096

typedef struct {
    unsigned char ETQ;
    unsigned char Data[TAM_LINEA];
    } T_CACHE_LINE;


void CleanCACHE(T_CACHE_LINE tbl[NUM_ROWS]);
void DumpCACHE(T_CACHE_LINE *tbl);
void ParseAdress(unsigned int addr, int *LABEL, int*word, int *line, int *block);
void TreatFailureMiss(T_CACHE_LINE *tbl, char *MRAM, int LABEL,int line, int block);



// Global variables
int globaltime = 0;
int numerrors = 0;

// Function to initialize the cache
void CleanCACHE(T_CACHE_LINE tbl[NUM_ROWS]) {
    for (int i = 0; i < NUM_ROWS; i++) {
        tbl[i].ETQ = 0xFF; // Initialize Label field to 0xFF
        for (int j = 0; j < TAM_LINEA; j++) {
            tbl[i].Data[j] = 0x23; // Initialize data field to 0x23
        }
    }
}
