#include <stdio.h>
#include <stdbool.h>

#define PAGE_SIZE 4
#define NUM_PAGES 8
#define NUM_FRAMES 8
#define TLB_SIZE 4

// tabla de paginas: pagina -> marco
int page_table[NUM_PAGES] = {2, 5, 1, 7, 0, 3, 6, 4};

typedef struct {
    int page;
    int frame;
    bool valid;
} TLBEntry;

TLBEntry tlb[TLB_SIZE];
int tlb_index = 0;

void initialize_tlb() {
    for (int i = 0; i < TLB_SIZE; i++)
        tlb[i].valid = false;
}

// busca la pagina en el TLB
int search_tlb(int page) {
    for (int i = 0; i < TLB_SIZE; i++)
        if (tlb[i].valid && tlb[i].page == page)
            return tlb[i].frame;
    return -1;
}

// agrega al TLB con reemplazo circular
void update_tlb(int page, int frame) {
    tlb[tlb_index].page  = page;
    tlb[tlb_index].frame = frame;
    tlb[tlb_index].valid = true;
    tlb_index = (tlb_index + 1) % TLB_SIZE;
}

void translate(int logical_address) {
    int page   = logical_address / PAGE_SIZE;
    int offset = logical_address % PAGE_SIZE;

    printf("\nDireccion logica: %d -> pagina: %d, offset: %d\n",
           logical_address, page, offset);

    if (page >= NUM_PAGES) {
        printf("Error: pagina %d fuera de rango\n", page);
        return;
    }

    int frame = search_tlb(page);

    if (frame != -1) {
        printf("TLB hit  -> marco: %d\n", frame);
    } else {
        frame = page_table[page];
        printf("TLB miss -> tabla de paginas -> marco: %d\n", frame);
        update_tlb(page, frame);
    }

    printf("Direccion fisica: %d\n", frame * PAGE_SIZE + offset);
}

int main() {
    printf("=== PAGINACION ===\n");
    initialize_tlb();

    translate(0);
    translate(6);
    translate(13);
    translate(6);  // TLB hit
    translate(20);
    translate(13); // TLB hit

    return 0;
}