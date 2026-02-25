#include <stdio.h>
#include <stdbool.h>

#define MEMORY_SIZE 1024
#define PARTITIONS 4
#define PART_SIZE (MEMORY_SIZE / PARTITIONS) // 256 bytes por particion

typedef struct {
    int process_id;
    int size;
    bool occupied;
} Partition;

// memoria dividida en partes iguales
Partition memory[PARTITIONS];

void initialize_mem() {
    for (int i = 0; i < PARTITIONS; i++) {
        memory[i].process_id = -1;
        memory[i].size = 0;
        memory[i].occupied = false;
    }
}

void print_mem() {
    printf("\nEstado de memoria:\n");
    for (int i = 0; i < PARTITIONS; i++) {
        printf("  Particion %d [%d bytes]: ", i, PART_SIZE);
        if (memory[i].occupied)
            printf("proceso %d (%d bytes)\n", memory[i].process_id, memory[i].size);
        else
            printf("libre\n");
    }
}

void allocate_mem(int pid, int size) {
    // no cabe en una particion
    if (size > PART_SIZE) {
        printf("Error: %d bytes supera el tamaño de particion (%d)\n", size, PART_SIZE);
        return;
    }
    // busca la primera particion libre
    for (int i = 0; i < PARTITIONS; i++) {
        if (!memory[i].occupied) {
            memory[i].process_id = pid;
            memory[i].size = size;
            memory[i].occupied = true;
            printf("Proceso %d asignado a particion %d\n", pid, i);
            return;
        }
    }
    printf("Error: no hay particiones libres para proceso %d\n", pid);
}

void free_mem(int pid) {
    for (int i = 0; i < PARTITIONS; i++) {
        if (memory[i].process_id == pid) { // corregido: antes tenia ! que invertia la condicion
            memory[i].process_id = -1;
            memory[i].size = 0;
            memory[i].occupied = false;
            printf("Proceso %d liberado de particion %d\n", pid, i);
            return;
        }
    }
    printf("Proceso %d no encontrado\n", pid);
}

int main() {
    // inicializamos la memoria
    initialize_mem();

    // asignamos 3 procesos
    allocate_mem(1, 100);
    allocate_mem(2, 200);
    allocate_mem(3, 256);

    // vemos como quedo
    print_mem();

    // liberamos el proceso 2
    free_mem(2);

    // asignamos uno nuevo en el espacio libre
    allocate_mem(4, 50);

    // estado final
    print_mem();

    return 0;
}