#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
enum { N_MAX = 2000, OneByte = 8 };

bool  CheckVertexEdge(int m, int n) {
    if (m < 0 || m >= n * (n + 1) / 2) {
        printf("bad number of edges");
        return false;
    }

    if (n < 0 || n > N_MAX) {
        printf("bad number of vertices");
        return false;
    }
    
    else {
        return true;
    }
}

bool CheckValue(int value, int n) {
    if (value < 0 || value > n) {
        printf("bad vertex");
        return false;
    }

    else {
        return true;
    }
}

void NUllingArrSizeN(char* colorArr, int n) {
    for (int i = 0; i < n; ++i) {
        colorArr[i] = 0;
    }
}
void NullingArrSizeMatrix(char* array, int sizeMatrix) {
    for (int i = 0; i < sizeMatrix; ++i) {
        array[i] = 0;
    }
}
void Output(int* arr, int n) {
    for (int i = n-1; i >= 0; --i) {
        printf("%d ", arr[i]);
    }
}

void FreeArr(char* colorArr, char* matrixArr) {
    free(colorArr);
    free(matrixArr);
}

bool DFS(int valueFrom, char* matrixArr, char* colorArr, int* printArr, int n, int* indexPrintArr) {
    int indexColorArr = valueFrom - 1;
    if (colorArr[indexColorArr] == 2) {
        return true;
    }
    colorArr[indexColorArr] += 1;
    for (int i = 0; i < n; ++i) {
        
        int index = (valueFrom-1) * n + i;
        int checkValue = 128 >> index % OneByte;

        if ((matrixArr[index / OneByte] & checkValue) == checkValue) {
            if (colorArr[i] == 1) {
                printf("impossible to sort");
                return false;
            }

            if (DFS(i + 1, matrixArr, colorArr, printArr, n, indexPrintArr) == false) {
                return false;
            }
        }
            
    }
    colorArr[indexColorArr] += 1;
    printArr[*indexPrintArr] = valueFrom;
    (*indexPrintArr)++;
    return true;
}

void TopologySort(char* matrixArr, char* colorArr, int n) {
    int* printArr =  malloc(n*sizeof(int));
    assert(printArr != NULL);
    int index1 = 0;
    for (int i = 0; i < n; ++i) {
        if (DFS(i + 1, matrixArr, colorArr, printArr, n, &index1) == false) {
            free(printArr);
            return;
        }
    }
    Output(printArr, n);
    free(printArr);
}

bool Input(char* colorArr, char* matrixArr, int sizeMatrix, int n, int m) {
    NullingArrSizeMatrix(matrixArr, sizeMatrix);
    NUllingArrSizeN(colorArr, n);
    int valueFrom, valueTo = 0;
    for (int i = 0; i < m; ++i) {

        if (scanf("%d %d", &valueFrom, &valueTo) != 2) {
            printf("bad number of lines");
            return false;
        }

        if (CheckValue(valueFrom, n) == false || CheckValue(valueTo, n) == false) {
            return false;
        }

        int index = n * (valueFrom - 1) + (valueTo - 1);
        matrixArr[index / OneByte] |= 128 >> index % OneByte;
    }
    return true;
}
int main(void) {
    int n, m = 0;

    if (scanf("%d %d", &n, &m) != 2) {
        printf("bad number of lines");
        return 0;
    }

    if (CheckVertexEdge(m, n) == false) {
        return 0;
    }

    char* colorArr = (char*)malloc(n * sizeof(char));
    assert(colorArr);
    int sizeMatrix = n * n / (sizeof(char) * OneByte) + 1;
    char* matrixArr = (char*)malloc(sizeMatrix * sizeof(char));
    assert(matrixArr != NULL);

    if (Input(colorArr, matrixArr, sizeMatrix, n, m) == false) {
        FreeArr(colorArr, matrixArr);
        return 0;
    }
    
    TopologySort(matrixArr, colorArr, n);
    FreeArr(colorArr, matrixArr);
    return 0;
}
