#include <stdio.h>
#include <stdlib.h>

void Swap(int* first, int* second) {
    int digitArray = *second;
    *second = *first;
    *first = digitArray;
}
void HeapSortImp(int* array, int root, int count) {
    int indexMax = root;
    int leftChild = 2 * root + 1;
    int rightChild = 2 * root + 2;
    if (leftChild < count && array[leftChild] > array[indexMax]) {
        indexMax = leftChild;
    }
    if (rightChild < count && array[rightChild] > array[indexMax]) {
        indexMax = rightChild;
    }
    if (indexMax != root) {
        Swap(&array[indexMax], &array[root]);
        HeapSortImp(array, indexMax, count);
    }
}
void HeapSort(int* array, int count) {
    for (int i = (count / 2) - 1; i >= 0; --i) {
        HeapSortImp(array, i, count);
    }
    for (int j = count - 1; j >= 0; --j) {
        Swap(&array[0], &array[j]);
        HeapSortImp(array, 0, j);
    }
}

int main() {
    int N;
    if (scanf("%d", &N) != 1) {
        return 0;
    }
    int* array = malloc(sizeof(int) * N);
    for (int i = 0; i < N; ++i) {
        if (scanf("%d", &array[i]) != 1) {
            free(array);
            return 0;
        }
    }
    HeapSort(array, N);
    for (int i = 0; i < N; ++i) {
        printf("%d ", array[i]);
    }
    free(array);
    return 0;
}

