#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>

enum { IN_MAX = 5000 };

typedef struct Node {
    short From;
    short To;
    int  Weight;
}Node;

int  QuickSortImpl(Node* array, int right, int left) {
    int pivot = array[(right + left) / 2].Weight;
    while (left <= right) {

        while (array[left].Weight < pivot) {
            ++left;
        }

        while (array[right].Weight > pivot) {
            --right;
        }

        if (left <= right) {
            if (array[left].Weight > array[right].Weight) {
                Node rewriting = array[left];
                array[left] = array[right];
                array[right] = rewriting;
            }

            ++left;
            --right;
        }

    }

    return left;
}

void QuickSort(Node* array, int left, int right) {

    int support = QuickSortImpl(array, right, left);
    if (left < support - 1) {

        QuickSort(array, left, support - 1);

    }
    if (right > support) {

        QuickSort(array, support, right);

    }
}

bool CheckVertexEdge(const int N, const int M) {

    if (N > IN_MAX || N < 0) {
        printf("bad number of vertices");
        return false;
    }

    if (M > N * (N - 1) / 2 || M < 0) {
        printf("bad number of edges");
        return false;
    }

    else {
        return true;
    }
}

bool CheckValue(const short value, const int N) {

    if (value >= 1 && value <= N) {
        return true;
    }

    else {
        printf("bad vertex");
        return false;
    }

}

bool CheckWeight(const int weight) {
    if (weight > 0 && weight <= INT_MAX) {
        return true;
    }

    else {
        printf("bad length");
        return false;
    }

}

void ErrorNumber() {
    printf("bad number of lines");
}

void NoSpanning() {
    printf("no spanning tree\n");
}

Node* CreateVertex(int N, int M) {
    Node* vertex;
    if (M != 0) {
        vertex = calloc(M, sizeof(Node));
    }
    else {
        vertex = calloc(N, sizeof(Node));
    }
    return vertex;
}

void FreeArr(int* visit, int* edges, Node* vert) {
    free(visit);
    free(edges);
    free(vert);
}

bool InputVertex(Node* vert, const int N, const int M) {
    if (N == 0) {
        NoSpanning();
        return false;
    }

    short vFrom, vTo = 0;
    int weight = 0;

    for (int i = 0; i < M; ++i) {

        if (scanf("%hd %hd %d", &vFrom, &vTo, &weight) != 3) {
            ErrorNumber();
            return false;
        }

        if (CheckValue(vFrom, N) == false || CheckValue(vTo, N) == false) {
            return false;
        }

        if (CheckWeight(weight) == false) {
            return false;
        }

        vert[i].From = vFrom;
        vert[i].To = vTo;
        vert[i].Weight = weight;

    }

    return true;

}


void OutputEdges(const int* edges, const int N) {
        for (int i = 0; i < N; ++i) {
            if (edges[i] != 0) {
                printf("%d %d\n", i + 1, edges[i]);
            }
        }

}

void FillVisitArr(int* visit, const int N) {
    for (int i = 0; i < N; ++i) {
        visit[i] = i;
    }
}

int FindRoot(const int* arr, const int index) {
    return (arr[index] == index) ? index : FindRoot(arr, arr[index]);
}

void ChangingValues(const Node* vert, int* edges, const int index) {
    if (edges[vert[index].From - 1] == 0) {
        edges[vert[index].From - 1] = vert[index].To;
    }

    else {
        edges[vert[index].To - 1] = vert[index].From;
    }
}

void  KraskalAlgorithm(Node* vert, const int N, const int M) {

    QuickSort(vert, 0, M - 1);

    int* visit = malloc(N * sizeof(int));
    assert(visit != NULL);

    FillVisitArr(visit, N);

    int* edges = calloc(N, sizeof(int));
    assert(edges != NULL);

    int count = 1;

    for (int i = 0; i < M; ++i) {

        int to = FindRoot(visit, vert[i].To - 1);
        int from = FindRoot(visit, vert[i].From - 1);

        if (to != from) {
            count++;
            visit[to] = from;
            ChangingValues(vert, edges, i);
        }

    }

    if (count != N) {
        NoSpanning();
    }

    else {
        OutputEdges(edges, N);
    }

    FreeArr(visit, edges, vert);
}

int main() {
    int N, M = 0;

    if (scanf("%d %d", &N, &M) != 2) {
        ErrorNumber();
        return 0;
    }

    if (CheckVertexEdge(N, M) == false) {
        return 0;
    }

    Node* vertex = CreateVertex(N, M);
    assert(vertex != NULL);

    if (InputVertex(vertex, N, M) == false) {
        free(vertex);
        return 0;
    }

    KraskalAlgorithm(vertex, N, M);

    return 0;
}

