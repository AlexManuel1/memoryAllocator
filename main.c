#include <stdio.h>
#include "memo.h"

int main() {
    printf("Hello World\n");
    int* arr = (int*) memo(5 * sizeof(int));
    char* string = (char*) memo(1000 * sizeof(char));
    char* string1 = (char*) memo(1000 * sizeof(char));
    char* string2 = (char*) memo(30724 * sizeof(char));
    char* string3 = (char*) memo(30724 * sizeof(char));
    char* string4 = (char*) memo(34802 * sizeof(char));
    printf("allocated address: %p\n", arr);
    int arrSize = 5;
    for (int i = 0; i < arrSize; i++) {
        arr[i] = i;
    }
    for (int i = 0; i < arrSize; i++) {
       printf("arr[%d] = %d\n", i, arr[i]);
    }
    string4[34801] = 'd';
    printf("string4[34801] = %c\n", string4[34801]);
    return 0;
}