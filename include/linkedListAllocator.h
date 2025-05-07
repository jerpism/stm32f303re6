#ifndef LINEARALLOCATOR_H
#define LINEARALLOCATOR_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
//#include <inttypes.h>

typedef struct heapBlock{ // header for the memory blocks
  size_t size;
  bool free;
  struct heapBlock* nextFreeBlock;
} heapBlock; 


extern uint8_t _heap_start; // addresses are located in the linker script
extern uint8_t _heap_end;
static heapBlock *initialHeapBlock = NULL; // visible only when in the linkedListAllocator scope? 

void heapBlockInit(void); // init the starting initialHeapBlock 
heapBlock* blockSplitter(heapBlock *block, size_t size); // splits the free block into two blocks, requested size and remaining one 
heapBlock* findSuitableFree(heapBlock* block, size_t size); // scroll through heap blocks and find a free one with the right size 
void* malloc(size_t size); // alloc command used by the user 
void free(void* pointer); // free cmmand used by the user 
void combineFreeBlocks(void); //command that combines free blocks that it manages to find 

#endif
