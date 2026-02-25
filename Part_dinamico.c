#include <stdio.h>
#include <stdbool.h>

#define MEMORY_SIZE 1024
#define MAX_BLOCKS 20

typedef struct {
    int start;
    int size;
    int process_id;
    bool free;
} Block;

Block blocks[MAX_BLOCKS];
int block_count = 1;

void initialize_memory() {
    blocks[0].start = 0;
    blocks[0].size = MEMORY_SIZE;
    blocks[0].free = true;
    blocks[0].process_id = -1;
    block_count = 1;
}

void print_memory() {
    printf("\nMemoria actual:\n");
    for (int i = 0; i < block_count; i++) {
        printf("  [%d] inicio:%d  tamaño:%d  %s",
            i, blocks[i].start, blocks[i].size,
            blocks[i].free ? "LIBRE" : "OCUPADO");
        if (!blocks[i].free)
            printf(" (PID %d)", blocks[i].process_id);
        printf("\n");
    }
}

// parte el bloque i y asigna el proceso
void assign(int i, int pid, int size) {
    if (blocks[i].size > size) {
        for (int j = block_count; j > i + 1; j--)
            blocks[j] = blocks[j - 1];
        blocks[i + 1].start = blocks[i].start + size;
        blocks[i + 1].size  = blocks[i].size - size;
        blocks[i + 1].free  = true;
        blocks[i + 1].process_id = -1;
        blocks[i].size = size;
        block_count++;
    }
    blocks[i].free = false;
    blocks[i].process_id = pid;
}

// primer bloque libre que alcance
void first_fit(int pid, int size) {
    for (int i = 0; i < block_count; i++) {
        if (blocks[i].free && blocks[i].size >= size) {
            assign(i, pid, size);
            printf("FIRST FIT: proceso %d en bloque %d\n", pid, i);
            return;
        }
    }
    printf("FIRST FIT: no hay espacio para proceso %d\n", pid);
}

// bloque libre mas pequeño que alcance
void best_fit(int pid, int size) {
    int best = -1;
    for (int i = 0; i < block_count; i++) {
        if (blocks[i].free && blocks[i].size >= size)
            if (best == -1 || blocks[i].size < blocks[best].size)
                best = i;
    }
    if (best == -1) { printf("BEST FIT: no hay espacio para proceso %d\n", pid); return; }
    assign(best, pid, size);
    printf("BEST FIT: proceso %d en bloque %d\n", pid, best);
}

// bloque libre mas grande disponible
void worst_fit(int pid, int size) {
    int worst = -1;
    for (int i = 0; i < block_count; i++) {
        if (blocks[i].free && blocks[i].size >= size)
            if (worst == -1 || blocks[i].size > blocks[worst].size)
                worst = i;
    }
    if (worst == -1) { printf("WORST FIT: no hay espacio para proceso %d\n", pid); return; }
    assign(worst, pid, size);
    printf("WORST FIT: proceso %d en bloque %d\n", pid, worst);
}

void free_block(int pid) {
    for (int i = 0; i < block_count; i++) {
        if (blocks[i].process_id == pid) {
            blocks[i].free = true;
            blocks[i].process_id = -1;
            printf("Proceso %d liberado\n", pid);
        }
    }
}

int main() {

    // prueba first fit
    printf("=== FIRST FIT ===\n");
    initialize_memory();
    first_fit(1, 200);
    first_fit(2, 300);
    first_fit(3, 100);
    free_block(2);
    first_fit(4, 150);
    print_memory();

    // prueba best fit
    printf("\n=== BEST FIT ===\n");
    initialize_memory();
    best_fit(1, 200);
    best_fit(2, 300);
    best_fit(3, 100);
    free_block(2);
    best_fit(4, 150);
    print_memory();

    // prueba worst fit
    printf("\n=== WORST FIT ===\n");
    initialize_memory();
    worst_fit(1, 200);
    worst_fit(2, 300);
    worst_fit(3, 100);
    free_block(2);
    worst_fit(4, 150);
    print_memory();

    return 0;
}