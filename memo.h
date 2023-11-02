#ifndef MEMO_H
#define MEMO_H

#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

typedef struct freeNode {
    size_t size;
    struct freeNode *next;
} FreeNode; 

typedef struct freeNode *FreeNodePtr;

typedef struct allocatedHeader {
    int size;
    int magic;
} *AllocatedHeader;

void* allocateMemory(void* memoryBlock, size_t memoryBlockSize, size_t allocationSize); 
size_t getPageSize();
size_t roundUpToPageSizeAndDouble(size_t size);
FreeNodePtr firstFit(size_t size);
void* memo(size_t size);

#endif