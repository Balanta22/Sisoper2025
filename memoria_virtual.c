#include <stdio.h>
#include <stdbool.h>

#define NUM_FRAMES 3
#define NUM_PAGES  7

typedef struct {
    int page;
    bool valid;
} Frame;

Frame ram[NUM_FRAMES];
int page_faults = 0;

void initialize_ram() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        ram[i].page  = -1;
        ram[i].valid = false;
    }
    page_faults = 0;
}

void print_ram() {
    printf("  RAM: [");
    for (int i = 0; i < NUM_FRAMES; i++)
        printf(" %d", ram[i].valid ? ram[i].page : -1);
    printf(" ]\n");
}

int find_page(int page) {
    for (int i = 0; i < NUM_FRAMES; i++)
        if (ram[i].valid && ram[i].page == page)
            return i;
    return -1;
}

int find_free_frame() {
    for (int i = 0; i < NUM_FRAMES; i++)
        if (!ram[i].valid)
            return i;
    return -1;
}

// =====================
// FIFO
// =====================

int fifo_queue[NUM_FRAMES];
int fifo_front = 0;
int fifo_count = 0;

void fifo_init() {
    fifo_front = 0;
    fifo_count = 0;
}

void fifo_access(int page) {
    if (find_page(page) != -1) {
        printf("Pagina %d -> ya en RAM\n", page);
        return;
    }

    page_faults++;
    printf("Pagina %d -> FALLO\n", page);
    int free = find_free_frame();

    if (free != -1) {
        ram[free].page  = page;
        ram[free].valid = true;
        fifo_queue[fifo_count % NUM_FRAMES] = free;
        fifo_count++;
    } else {
        int victim = fifo_queue[fifo_front % NUM_FRAMES];
        printf("  Victima FIFO: pagina %d\n", ram[victim].page);
        ram[victim].page = page;
        fifo_queue[fifo_front % NUM_FRAMES] = victim;
        fifo_front++;
    }
    print_ram();
}

// =====================
// LRU
// =====================

int lru_time = 0;
int lru_last_used[NUM_FRAMES];

void lru_init() {
    lru_time = 0;
    for (int i = 0; i < NUM_FRAMES; i++)
        lru_last_used[i] = 0;
}

void lru_access(int page) {
    lru_time++;
    int frame = find_page(page);

    if (frame != -1) {
        printf("Pagina %d -> ya en RAM\n", page);
        lru_last_used[frame] = lru_time;
        return;
    }

    page_faults++;
    printf("Pagina %d -> FALLO\n", page);
    int free = find_free_frame();

    if (free != -1) {
        ram[free].page  = page;
        ram[free].valid = true;
        lru_last_used[free] = lru_time;
    } else {
        // busca el marco menos usado recientemente
        int lru_frame = 0;
        for (int i = 1; i < NUM_FRAMES; i++)
            if (lru_last_used[i] < lru_last_used[lru_frame])
                lru_frame = i;

        printf("  Victima LRU: pagina %d\n", ram[lru_frame].page);
        ram[lru_frame].page = page;
        lru_last_used[lru_frame] = lru_time;
    }
    print_ram();
}

int main() {
    int accesses[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int n = sizeof(accesses) / sizeof(accesses[0]);

    printf("=== MEMORIA VIRTUAL - FIFO ===\n");
    initialize_ram();
    fifo_init();
    for (int i = 0; i < n; i++)
        fifo_access(accesses[i]);
    printf("Fallos FIFO: %d\n", page_faults);

    printf("\n=== MEMORIA VIRTUAL - LRU ===\n");
    initialize_ram();
    lru_init();
    for (int i = 0; i < n; i++)
        lru_access(accesses[i]);
    printf("Fallos LRU: %d\n", page_faults);

    return 0;
}