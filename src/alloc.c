#include <stdint.h>
/* Heap size in words */
#define HEAPSIZE 4096 
#define NULL (void*)0

/* Crude allocator just to have dynamic allocation available 
 * shouldn't really be used for anything */
__attribute__((section(".heap"))) static uint32_t _heap[HEAPSIZE];

static const uint8_t * const start_heap = (const uint8_t *)&_heap;
static const uint8_t * const end_heap = (const uint8_t *)&_heap[HEAPSIZE-1];

static uint8_t *heap_ptr = (uint8_t*)start_heap;

static inline void *sbrk(size_t n){
    uint8_t *base;

    base = heap_ptr;
    heap_ptr += n;

    return (void *)base;

}

void *malloc(size_t n){
    return sbrk(n);
}

// can't really free anything to just set it to a null pointer
void free(void *p){
    p = NULL;
}
