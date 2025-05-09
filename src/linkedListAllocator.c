#include "linkedListAllocator.h"

void heapBlockInit(void){
     initialHeapBlock = (heapBlock*)&_heap_start; 
     initialHeapBlock->size = (size_t)(&_heap_end - &_heap_start - sizeof(heapBlock)); // size of the space from the final address size - the size of the struct
     initialHeapBlock->free = true;
     initialHeapBlock->nextFreeBlock = NULL; 

}

heapBlock* findSuitableFree(heapBlock* block, size_t size){
  struct heapBlock* scroller = block;
  while(scroller != NULL){
        if(scroller->free){
          if(scroller->size > size){
          return scroller;
          }
        }
        scroller = scroller->nextFreeBlock;
  }
  return NULL; // if didn't find any free blocks, send NULL 
}

heapBlock* blockSplitter(heapBlock *block, size_t size){
 if(size >= block->size || block->free == false){
 while(true){
    // it's basically an error, sit here as there's no error handling implemented 
    // no return, because findSuitableFreeBlock failed
   }
 }
 // create a new block for the remaining unallocated memory 
 heapBlock* newBlock = (heapBlock*)((uint8_t*)(block) + size); //since the provided block should be already aligned 
 newBlock->size = block->size - size; // size of the free block minus the requested size
 newBlock->free = true;
 newBlock->nextFreeBlock = NULL;

 block->nextFreeBlock = newBlock; // point to the new free block
 block->size = size; // requested size 
 block->free = false;

 return block;
}

void *malloc_internal(size_t size){
    if(size == 0){
        return NULL;
    }else if(initialHeapBlock == NULL){
        heapBlockInit(); // init memory if it wasn't already
    }
    size = (((size + sizeof(heapBlock)) + 3) & ~3);
    heapBlock* block = NULL;

    block = findSuitableFree(initialHeapBlock, size); // search always starts from the first block
    if( block == NULL) {
        return NULL;
    }

    block = blockSplitter(block, size);
    if( block == NULL) {
        return NULL;
    }

    return (uint8_t*)block + sizeof(heapBlock); //return the pointer to the start of the space after the header
}

void combineFreeBlocks(void) {
    heapBlock* scroller = initialHeapBlock; 
    while(scroller != NULL){
        if(scroller->free && scroller->nextFreeBlock != NULL && scroller->nextFreeBlock->free){
            heapBlock* nextFree = scroller->nextFreeBlock;
            scroller->size += nextFree->size; 
            scroller->nextFreeBlock = nextFree->nextFreeBlock;
            scroller->free = true;
        }else{
            scroller = scroller->nextFreeBlock;
        }
    }
}

void free_internal(void* pointer){ // find the header of the pointer and set it to free, call the combineFreeBlocks function to merge it with the previous block
    if(pointer == NULL){
        // do nothing 
        return;
    }
    heapBlock* block = (heapBlock*)((uint8_t*)pointer-sizeof(heapBlock));
    block->free = true;  

    combineFreeBlocks(); // check if any nearby blocks are free as well and combine them
}

