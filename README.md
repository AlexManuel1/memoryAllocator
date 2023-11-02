# Memory Allocator Written in C

- allocator using first fit algorithm
- requests memory from OS using mmap()
- maintains a free list to keep track of available chunks of memory to prevent multiple calls to mmap()

# How to run

`gcc -Wall main.c memo.c -o main.out & ./main.out`