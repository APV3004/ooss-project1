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

void DumpCACHE(T_CACHE_LINE *tlb) {
    for (int i = 0; i < NUM_ROWS; i++) {
        printf("Line %d: Label %X Data: ", i, tlb[i].ETQ);
        for (int j = TAM_LINEA - 1; j >= 0; j--) {
            printf("%02X ", tlb[i].Data[j]);
        }
        printf("\n");
    }
}


// Function to parse memory address
void ParseAddress(unsigned int addr, int *LABEL, int *word, int *line, int *block) {
    *word = addr & 0xF;
    *line = (addr >> 4) & 0x7;
    *LABEL = (addr >> 7) & 0x1F;
    *block = addr >> 4; // each block is 16 bytes
}

void TreatFailureMiss(T_CACHE_LINE *tbl, char *MRAM, int LABEL, int line, int block) {
    int numfaults = 0;
    numfaults++;

    printf("T: %d, CACHE Fault %d, ADDR %04X Label %X line %02X word %02X block %02X\n", globaltime, numfaults, block, LABEL, line, block, block * TAM_LINEA);

    // Increment global time
    globaltime += 20;

    // Copy the block from RAM to cache
    printf("Loading Block %02X into Line %02X\n", block, line);
    int startAddress = block * TAM_LINEA;
    tbl[line].ETQ = LABEL;

    for (int i = 0; i < TAM_LINEA; i++) {
        tbl[line].Data[i] = MRAM[startAddress + i];
    }
}