#include "include/extrafunc.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

int randomize(int min, int max) {
    srand(time(0));
    return rand() % (max - min + 1) + min;
}

void randomize_list(int* list, int size) {
    srand(time(0));
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = list[i];
        list[i] = list[j];
        list[j] = temp;
    }
}

void randomize_string(char* str) {
    srand(time(0));
    int n = strlen(str);
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}
