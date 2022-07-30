#include <stdio.h>
#include <stdlib.h>
void swap(int* array, int right, int left) {
    int rewriting = array[right];
    array[right] = array[left];
    array[left] = rewriting;
}
int  quick_sort_impl(int* array, int right, int left) {
    int pivot = array[(right + left) / 2];
    while (left <= right) {
        while (array[left] < pivot) {
            left += 1;
        }
        while (array[right] > pivot) {
            right -= 1;
        }
        if (left <= right) {
            if (array[left] > array[right]) {
                swap(array, right, left);
            }
            left += 1;
            right -= 1;
        }
    }
    return left;
}
void quick_sort(int* array, int left, int right) {
    int support = quick_sort_impl(array, right, left);
    if (left < support - 1) {
        quick_sort(array, left, support - 1);
    }
    if (right > support) {
        quick_sort(array, support, right);
    }
}

int main() {
    int N;
    if (scanf("%d", &N) != 1) {
        return 0;
    }
    int* array = (int*)malloc(N * sizeof(int));
    if (array == NULL) {
        return 0;
    }
    for (int i = 0; i < N; ++i) {
        if (scanf("%d", &array[i]) != 1) {
            free(array);
            return 0;
        }
    }

    if (N > 1) {
        int right = N - 1;
        int left = 0;
        quick_sort(array, left, right);
    }
    for (int i = 0; i < N; ++i) {
        printf("%d ", array[i]);
    }
    free(array);
    return 0;
}

