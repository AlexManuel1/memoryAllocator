#include <stdio.h>
#include "memo.h"

int main() {
    printf("Hello World\n");
    int* arr = (int*) memo(5 * sizeof(int));
    char* string = (char*) memo(35000 * sizeof(char));
    char* string1 = (char*) memo(63192);
    string1[63191] = 'd';
    printf("string1[63278] = %c\n", string1[63191]);
    printf("allocated address: %p\n", arr);
    int arrSize = 5;
    for (int i = 0; i < arrSize; i++) {
        arr[i] = i;
    }
    for (int i = 0; i < arrSize; i++) {
       printf("arr[%d] = %d\n", i, arr[i]);
    }
    return 0;
}