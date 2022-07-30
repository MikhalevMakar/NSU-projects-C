#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void ScanArray(char* array) {
    int index = 0;
    while (true) {
        char c = getchar();
        if (c == '\n') {
            array[index] = '\0';
            break;
        }
        array[index] = c;
        index++;
    }
}

void Swap(char* array, int first, int second) {
    char temporary = array[first];
    array[first] = array[second];
    array[second] = temporary;
}

void ReverseArray(char* array, unsigned int maxIndex) {
    for (unsigned int i = maxIndex + 1, j = strlen(array) - 1; i < j; ++i, --j) {
        Swap(array, i, j);
    }
}

int Check(char* array) {
    int arrayCount[10] = { 0 };
    for(unsigned int i = 0; i < strlen(array); ++i) {
        int index = (int)array[i] - 48;
        if (array[i] < '0' || array[i] > '9') {
            printf("bad input");
            return 0;
        }
        arrayCount[index] += 1;
        if (arrayCount[index] == 2) {
            printf("bad input");
            return 0;
        }
    }
    return 1;
}

void Rearrangement(char* array, int N) {
    unsigned int maxIndex = 0;
    if (Check(array) == 0) {
        return;
    }
    for (int count = 0; count < N; count++) {
        for (unsigned int i = 0; i < strlen(array); ++i) {
            if (array[i] < array[i + 1]) {
                maxIndex = i;
            }
        }
        unsigned int minIndex = maxIndex + 1;
        for (unsigned int j = maxIndex + 1; j < strlen(array); ++j) {
            if (array[minIndex] > array[j] && array[maxIndex] < array[j]) {
                minIndex = j;
            }
        }
        if (array[maxIndex] > array[minIndex]) {
            return;
        }
        else if (array[maxIndex] < array[minIndex]) {
            Swap(array, minIndex, maxIndex);
            ReverseArray(array, maxIndex);
            printf("%s\n", array);
        }
    }
}

int main() {
    char array[126];
    ScanArray(array);
    int N;
    if (scanf("%d", &N) != 1) {
        return 0;
    }
    Rearrangement(array, N);
    return 0;
}

