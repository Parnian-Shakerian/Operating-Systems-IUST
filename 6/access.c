#include <stdio.h>
#include <stdlib.h>

int main() {
    int* data = (int*)malloc(100 * sizeof(int));
    data[0] = 99400064;
    data[1] = 42;

    int* invalidPtr = data; // int* funnyPtr = data + 50;
    free(invalidPtr);  // free(funnyPtr);  
    return 0;
}

