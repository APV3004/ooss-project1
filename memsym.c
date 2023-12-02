#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_LINEA 16
#define NUM_ROWS 8
#define MEM_SIZE 4096
#define MAX_TEXT_LEN 100

typedef struct {
    unsigned char ETQ;
    unsigned char Data[TAM_LINEA];
    } T_CACHE_LINE;

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
        printf("%d\t Data: %X ", i, tlb[i].ETQ);
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
    
    numerrors++;

    printf("T: %d, CACHE Fault %d, ADDR %04X Label %X line %02X word %02X block %02X\n", globaltime, numerrors, block, LABEL, line, block, block * TAM_LINEA);

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

int main() {

    T_CACHE_LINE cache[NUM_ROWS];
    unsigned char Simul_RAM[MEM_SIZE];
    char text[MAX_TEXT_LEN];
    unsigned int address;
    int total_accesses = 0;

    FILE *ram_file = fopen("/Users/adrip/OneDrive - U-tad/SSOO/ooss-project1/CONTENTS_RAM.bin", "rb");
    FILE *addr_file = fopen("/Users/adrip/OneDrive - U-tad/SSOO/ooss-project1/accesos_memoria.txt", "r");

    // Check if files exist
    if (addr_file == NULL) {
        printf("ERROR. File could not be opened");
        return -1;
    }

      // Read RAM contents
    fread(Simul_RAM, sizeof(unsigned char), MEM_SIZE, ram_file);
    fclose(ram_file);

    // Initialize cache
    CleanCACHE(cache);

    // Read addresses from the file
    while (fscanf(addr_file, "%x", &address) == 1) {
        int LABEL, word, line, block;
        ParseAddress(address, &LABEL, &word, &line, &block);
                // Check if the address label equals to Label of the cache line
        if (cache[line].ETQ != LABEL) {
            TreatFailureMiss(cache, Simul_RAM, LABEL, line, block);
        } else {
            // Cache hit
            printf("T: %d, CACHE hit, ADDR %04X Label %X line %02X word %02X DATA %02X\n",
                   globaltime, address, LABEL, line, word, cache[line].Data[word]);
        }

        // Read characters and add to the text array
        char c = cache[line].Data[word];
        text[word] = c;

        // Display cache contents
        DumpCACHE(cache);

        // Increase the number of accesses by 1
        total_accesses++;

        // Sleep for 1 second
        sleep(1);
    }

    // Print the statistics
    printf("Total accesses: %d\n", total_accesses);
    printf("Number of misses: %d\n", numerrors);
    printf("Average access time: %.2f\n", (float)globaltime / total_accesses);

    // Print text read character by character from the cache
    printf("Text read from cache: ");
    for(int i = 0; i < total_accesses; i++) {
        printf("%c", text[i]);
    }
    
    // Dump cache contents into a binary file
    FILE *cache_file = fopen("CONTENTS_CACHE.bin", "wb");
    fwrite(cache, sizeof(T_CACHE_LINE), NUM_ROWS, cache_file);
    fclose(cache_file);

    return 0;

}