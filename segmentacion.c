#include <stdio.h>

#define NUM_SEGMENTS 4

typedef struct {
    int base;
    int limit;
    char name[20];
} Segment;

// tabla de segmentos
Segment segment_table[NUM_SEGMENTS] = {
    {1000, 400, "codigo"},
    {4000, 200, "datos"},
    {6000, 150, "pila"},
    {8000, 300, "heap"}
};

void print_table() {
    printf("\nTabla de segmentos:\n");
    printf("  %-5s %-12s %-8s %-8s\n", "ID", "Nombre", "Base", "Limite");
    for (int i = 0; i < NUM_SEGMENTS; i++)
        printf("  %-5d %-12s %-8d %-8d\n",
               i, segment_table[i].name,
               segment_table[i].base, segment_table[i].limit);
}

void translate(int segment, int displacement) {
    printf("\nSegmento: %d | Desplazamiento: %d\n", segment, displacement);

    if (segment >= NUM_SEGMENTS) {
        printf("Error: segmento %d no existe\n", segment);
        return;
    }

    Segment s = segment_table[segment];

    // si el desplazamiento supera el limite es violacion
    if (displacement >= s.limit) {
        printf("Error: violacion de segmento (desplazamiento %d >= limite %d)\n",
               displacement, s.limit);
        return;
    }

    printf("Direccion fisica: base(%d) + desplazamiento(%d) = %d\n",
           s.base, displacement, s.base + displacement);
}

int main() {
    printf("=== SEGMENTACION ===\n");
    print_table();

    translate(0, 50);   // codigo - ok
    translate(1, 100);  // datos - ok
    translate(2, 200);  // pila - violacion
    translate(3, 299);  // heap - ok
    translate(5, 10);   // no existe - error

    return 0;
}