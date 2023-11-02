#include "memo.h"

// initialize free list
FreeNodePtr freeListHead = NULL;

/*
    get size of page in operating system (memory is allocated in pages)
*/
size_t getPageSize() {
    return sysconf(_SC_PAGE_SIZE);
}

/*
    print current state of free list
*/
void printFreeList() {
    int nodeCount = 0;
    FreeNodePtr freeListNode = freeListHead;
    while (freeListNode) {
        nodeCount += 1;
        printf("%d -- size=%zu, *next=%p\n", nodeCount, freeListNode->size, freeListNode->next);
        freeListNode = freeListNode->next;
    }
    printf("Number of Nodes: %d\n", nodeCount);
}

/*
    round up the given size to a page multiple and double it. We do this to reduce
    fragmentation in our free list. If we do not double, we may end up with small
    free nodes in the free list
*/
size_t roundUpToPageSizeAndDouble(size_t size) {
    const size_t PageSize = getPageSize();
    const double dSize = (double) size, dPageSize = (double) PageSize;
    printf("PageSize: %zu\n", PageSize);
    const size_t numPageFitDoubled = ceil(dSize / dPageSize) * PageSize * 2;
    printf("Size Requested: %f, Size Allocating: %zu\n", dSize, numPageFitDoubled);
    return numPageFitDoubled;
}

/*
    finds the first node in the free list to allocate memory
    returns: pointer to node or NULL
*/
FreeNodePtr firstFit(size_t size) {
    FreeNodePtr freeListNode = freeListHead;
    FreeNodePtr prevFreeListNode = NULL;
    int nodeCount = 0;
    while (freeListNode) {
        nodeCount += 1;
        if (freeListNode->size >= size) {
            printf("Removing node %d\n", nodeCount);
            // we found a free node, remove and return
            if (!prevFreeListNode) {
                freeListHead = NULL;
            } else {
                prevFreeListNode->next = freeListNode->next;
            }
            return freeListNode;
        }
        prevFreeListNode = freeListNode;
        freeListNode = freeListNode->next;
    }
    return NULL;
}

/*
    We allocate memory given a block of memory and return a pointer to it (initialize
    header before the pointer to indicate size of allocated memory block). We then
    add the memory leftover to the free list.
*/
void* allocateMemory(void* memoryBlock, size_t memoryBlockSize, size_t allocationSize) {
    printf("\x1b[35m---------- Allocation Process ---------\033[0m\n");
    AllocatedHeader header = (AllocatedHeader) memoryBlock;
    header->size = allocationSize;
    header->magic = 1;
    void *allocatedBlock = header + 1;

    printf("Allocated memory header data\n");
    printf("    Header address: %p\n", header);
    printf("    Allocation starting address: %p\n" , allocatedBlock);
    printf("    Allocation size: %d\n", header->size);
    printf("    Allocated memory block ending address: %p\n", ((char*) memoryBlock) + allocationSize + sizeof(header));

    const size_t allocatedBlockSizeWithHeader = header->size + sizeof(AllocatedHeader);
    const int freeBlockSize = memoryBlockSize - (allocatedBlockSizeWithHeader + (int)sizeof(AllocatedHeader));

    // add remaining memory to free list if (including allocation header size) >= 0
    if (freeBlockSize >= ((int)sizeof(FreeNode))) { // make room for header
        printf("Creating new Free Node\n");
        const void *freeBlockStart = (void*) (((char*) allocatedBlock) + header->size);
        FreeNodePtr freeListNode = (FreeNodePtr) freeBlockStart;
        freeListNode->size = freeBlockSize;
        freeListNode->next = freeListHead;
        freeListHead = freeListNode;
        printf("Free Block data\n"); 
        printf("    starting address: %p\n", freeBlockStart);
        printf("    size: %zu\n", freeListHead->size);
        printf("\n");
    } else {
        printf("Skipping creation of new Free Node\n");
    }
    return allocatedBlock;
}

/*
    initialize memory allocation
*/
void* memo(size_t size) {
    printf("\n\033[0;32m--------- Memo Called: Begin Allocation -----------\033[0m\n\n");
    void *allocatedMemory;
    // print free list initial snapshot
    printf("\x1b[35m--------- Free List: initial snapshot -----------\033[0m\n");
    printFreeList();
    printf("\n");
    const FreeNodePtr freeListNode = firstFit(size);
    if (freeListNode) {
        /*
            node found in free list, allocate requested size 
            and add the remaining memory to the free list
        */
        allocatedMemory = allocateMemory(freeListNode, freeListNode->size, size);
    } else {
        /*
            no node found in free list, round memory up to nearest 
            multiple of page size, allocate requested size, and add
            the remaining memory to the free list. 
        */
        const size_t pageSizeMultiple = roundUpToPageSizeAndDouble(size);
        void* memoryBlock = mmap(NULL, pageSizeMultiple, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        allocatedMemory = allocateMemory(memoryBlock, pageSizeMultiple, size);
    }
    // print free list initial snapshot
    printf("\x1b[35m--------- Free List: final snapshot -----------\033[0m\n");
    printFreeList();
    printf("\n");
    return allocatedMemory;
}
