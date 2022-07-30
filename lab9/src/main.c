#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
enum { IN_MAX = 5000 };

void ErrorBadNumber() {
    printf("bad number of lines");
}

void SetZeroChar(void* x) {
    char* curX = x;
    *curX = 0;
}

void SetZeroInt(void* x) {
    int* curX = x;
    *curX = 0;
}

void Transform(void* arr, size_t count, size_t size, void (*t)(void*)) {
    char* newArr = arr;
    for (size_t i = 0; i < count; ++i) {
        t(&newArr[i * size]);
    }
}

typedef struct Graph {
    int* Matrix;
    int NumVert;
}Graph;

Graph CreateGraph(const int N) {

    Graph graph = { NULL, N };

    int sizeMatrix = (N + N * N) / 2;

    graph.Matrix = malloc(sizeMatrix * sizeof(int));

    assert(graph.Matrix != NULL);

    Transform(graph.Matrix, sizeMatrix, sizeof(int), SetZeroInt);
    return graph;
}

void FreeArr(char* visitArr, long long int* pathLengthArr, Graph graph, int* printArr) {
    free(graph.Matrix);
    free(visitArr);
    free(pathLengthArr);
    free(printArr);
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

bool CheckValue(const int value, const int N) {

    if (value >= 1 && value <= N) {
        return true;
    }

    else {
        printf("bad vertex");
        return false;
    }

}

void OutputShortestPath(const long long* pathLengthArr, const int count, const int start, const int finish, int* printArr) {
    if (pathLengthArr[finish] == LLONG_MAX) {
        printf("no path");
    }

    else if (pathLengthArr[finish] > INT_MAX && count == 2) {
        printf("overflow");
    }

    else {
        int index = finish;
        while (index != start) {
            printf("%d ", index + 1);
            index = printArr[index];
        }
        printf("%d ", index + 1);
    }
}

void OutputDistancesEachVertex(const Graph graph, const long long* pathLengthArr) {
    for (int i = 0; i < graph.NumVert; ++i) {
        long long length = INT_MAX;
        if (pathLengthArr[i] == LLONG_MAX) {
            printf("oo ");
        }

        else if (pathLengthArr[i] > length) {
            printf("INT_MAX+ ");
        }

        else {
            printf("%lld ", pathLengthArr[i]);
        }
    }
}

void FillLLongMaxArr(long long int* array, const Graph graph) {
    for (int i = 0; i < graph.NumVert; ++i) {
        array[i] = LLONG_MAX;
    }
}

int MaxValue(const int v1, const int v2) {
    return (v1 > v2) ? v1 : v2;
}

int MinValue(const int v1, const int v2) {
    return (v1 < v2) ? v1 : v2;
}

int GetEdgeIndex(const int edgeBegin, const int edgeEnd, const Graph graph) {
    return (2 * graph.NumVert - edgeBegin + 1) * edgeBegin / 2 + edgeEnd - edgeBegin;
}

int SearchIndex(const int curValueFrom, const int curValueTo, const Graph graph) {
    int minIndex = MinValue(curValueFrom, curValueTo);
    int maxIndex = MaxValue(curValueFrom, curValueTo);
    return GetEdgeIndex(minIndex, maxIndex, graph);
}

int FindSource(const long long* pathLengthArr, const char* visitArr, const Graph graph) {
    int indexSource = 0;
    long long int minLength = LLONG_MAX;
    for (int i = 0; i < graph.NumVert; ++i) {
        if (visitArr[i] == 0 && minLength > pathLengthArr[i]) {
            indexSource = i;
            minLength = pathLengthArr[i];
        }
    }
    return indexSource;
}

void FindShortestPath(Graph graph, int indexSource, long long* pathLengthArr, char* visitArr, int* count, int* printArr) {
    for (int i = 0; i < graph.NumVert; ++i) {
        int index = SearchIndex(indexSource, i, graph);
        if (graph.Matrix[index] != 0 && visitArr[i] == 0) {
            long long curLength = (long long)graph.Matrix[index] + pathLengthArr[indexSource];
            if (curLength <= pathLengthArr[i]) {
                pathLengthArr[i] = curLength;
                printArr[i] = indexSource;
            }
            if (*count != 2 && pathLengthArr[i] > INT_MAX) {
                ++(*count);
            }
        }
    }
}

void DijkstraAlgorithm(Graph graph, int count, const int S, const int F) {

    char* visitArr = malloc(graph.NumVert * sizeof(char));
    assert(visitArr != NULL);
    Transform(visitArr, graph.NumVert, sizeof(char), SetZeroChar);


    long long* pathLengthArr = malloc(graph.NumVert * sizeof(long long int));
    assert(pathLengthArr != NULL);
    FillLLongMaxArr(pathLengthArr, graph);
    pathLengthArr[S] = 0;


    int* printArr = malloc(graph.NumVert * sizeof(int));
    assert(printArr != NULL);
    Transform(printArr, graph.NumVert, sizeof(int), SetZeroInt);

    int index = S;

    for (int i = 0; i < graph.NumVert; ++i) {

        visitArr[index] = 1;
        FindShortestPath(graph, index, pathLengthArr, visitArr, &count, printArr);
        index = FindSource(pathLengthArr, visitArr, graph);

    }

    OutputDistancesEachVertex(graph, pathLengthArr);

    printf("\n");

    OutputShortestPath(pathLengthArr, count, S, F, printArr);

    FreeArr(visitArr, pathLengthArr, graph, printArr);

}

bool Input(Graph graph) {
    int M, curValueFrom, curValueTo = 0;
    int weight = 0;
    if (scanf("%d", &M) != 1) {
        ErrorBadNumber();
        return false;
    }

    if (CheckVertexEdge(graph.NumVert, M) == false) {
        return false;
    }

    for (int i = 0; i < M; ++i) {

        if (scanf("%d %d %d", &curValueFrom, &curValueTo, &weight) != 3) {
            ErrorBadNumber();
            return false;
        }

        if (!CheckValue(curValueTo, graph.NumVert) || !CheckValue(curValueFrom, graph.NumVert)) {
            return false;
        }

        if (weight < 0 || weight > INT_MAX) {
            printf("bad length");
            return false;
        }

        int index = SearchIndex(curValueFrom-1, curValueTo-1, graph);
        graph.Matrix[index] = weight;
    }

    return true;
}

int main() {
    int N, start, finish;

    if (scanf("%d %d %d", &N, &start, &finish) != 3) {
        ErrorBadNumber();
        return 0;
    }

    if (CheckValue(start, N) == false || CheckValue(finish, N) == false) {
        return 0;
    }

    Graph graph = CreateGraph(N);

    if (Input(graph) == false) {
        free(graph.Matrix);
        return 0;
    }

    int count = 0;

    DijkstraAlgorithm(graph, count, start - 1, finish - 1);

    return 0;
}

