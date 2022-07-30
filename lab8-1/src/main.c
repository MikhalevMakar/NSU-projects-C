#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

enum { IN_MAX = 5000 };

typedef struct Graph {
    int SizeEdges;
    int SizeVert;
    int* Matrix;
}Graph;

bool CheckVertexEdge(const int N, const int M) {

    if (N > IN_MAX || N < 0) {
        printf("bad number of vertices\n");
        return false;
    }

    if (M > N * (N - 1) / 2 || M < 0) {
        printf("bad number of edges\n");
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
        printf("bad vertex\n");
        return false;
    }
}

void ErrorBadNumber() {
    printf("bad number of lines\n");
}

void ErrorNoSpanning() {
    printf("no spanning tree\n");
}

void OutputEdges(const int* edges, const int N) {
    for (int i = 0; i < N; ++i) {
        if (edges[i] != 0) {
            printf("%d %d\n", i + 1, edges[i]);
        }
    }
}

void FreeArr(int* vertPath, int* visit, Graph graph, long long* curPaths) {
    free(vertPath);
    free(visit);
    free(graph.Matrix);
    free(curPaths);
}

bool InputNnM(int* N, int* M) {
    if (scanf("%d %d", N, M) != 2) {
        ErrorBadNumber();
        return false;
    }

    if (*N == 0) {
        ErrorNoSpanning();
        return false;
    }

    if (CheckVertexEdge(*N, *M) == false) {
        return false;
    }

    else {
        return true;
    }
}

int MaxValue(const int v1, const int v2) {
    return (v1 > v2) ? v1 : v2;
}

int MinValue(const int v1, const int v2) {
    return (v1 < v2) ? v1 : v2;
}

int GetEdgeIndex(const int edgeBegin, const int edgeEnd, const Graph graph) {
    return (2 * graph.SizeVert - edgeBegin + 1) * edgeBegin / 2 + edgeEnd - edgeBegin;
}

int SearchIndex(const int curValueFrom, const int curValueTo, const Graph graph) {
    int minIndex = MinValue(curValueFrom, curValueTo);
    int maxIndex = MaxValue(curValueFrom, curValueTo);
    return GetEdgeIndex(minIndex, maxIndex, graph);
}

Graph CreateGraph(const int N, const int M) {
    Graph graph = { M, N, NULL };

    int sizeMatrix = (N + N * N) / 2;

    graph.Matrix = calloc(sizeMatrix, sizeof(int));

    assert(graph.Matrix != NULL);

    return graph;
}

bool InputValue(Graph graph) {
    short curValueFrom, curValueTo;
    int    weight;

    for (int i = 0; i < graph.SizeEdges; ++i) {

        if (scanf("%hd %hd %d", &curValueFrom, &curValueTo, &weight) != 3) {
            ErrorBadNumber();
            return false;
        }

        if (!CheckValue(curValueTo, graph.SizeVert) || !CheckValue(curValueFrom, graph.SizeVert)) {
            return false;
        }

        if (weight < 0 || weight > INT_MAX) {
            printf("bad length");
            return false;
        }

        int index = SearchIndex(curValueFrom - 1, curValueTo - 1, graph);
        graph.Matrix[index] = weight;
    }
    return true;
}


void FillLLONGMAX(Graph graph, long long* curPaths) {
    for (int i = 0; i < graph.SizeVert; ++i) {
        curPaths[i] = LLONG_MAX;
    }
}

int FindSource(const long long* pathLengthArr, const int* visitArr, const Graph graph) {
    int indexSource = 0;
    long long minLength = LLONG_MAX;

    for (int i = 0; i < graph.SizeVert; ++i) {
            if ( visitArr[i] == 0 && minLength > pathLengthArr[i]) {
                indexSource = i;
                minLength = pathLengthArr[i];
            }
    }

    return indexSource;
}

void FindShortestPath(Graph graph, int* parent, long long* pathLengthArr, int* visit, int indexSource) {
    for (int i = 0; i < graph.SizeVert; ++i) {
        int curIndex = SearchIndex(indexSource, i, graph);
        if (graph.Matrix[curIndex] != 0 && visit[i] == 0) {
            long long curLength = graph.Matrix[curIndex];
            if (curLength < pathLengthArr[i]) {
                pathLengthArr[i] = curLength;
                parent[i] = indexSource+1;
            }
        }
    }

}


void AlgorithmPrima(Graph graph) {
    int* parent = calloc(graph.SizeVert, sizeof(int));
    assert(parent != NULL);

    int* visit = calloc(graph.SizeVert, sizeof(int));
    assert(visit != NULL);

    long long* pathLength = calloc(graph.SizeVert, sizeof(long long));
    assert(pathLength != NULL);
    FillLLONGMAX(graph, pathLength);

    int index = 0;
    pathLength[index] = 0;
    int vertexCount = graph.SizeVert;
    
    while (vertexCount != 1) {
        vertexCount--;
        visit[index] = 1;
        FindShortestPath(graph, parent, pathLength, visit, index);
        index = FindSource(pathLength, visit, graph);

        if (index == 0) {
            ErrorNoSpanning();
            FreeArr(parent, visit, graph, pathLength);
            return;
        }
    }

    OutputEdges(parent, graph.SizeVert);
    FreeArr(parent, visit, graph, pathLength);
}

int main() {

    int N, M;

    if (InputNnM(&N, &M) == false) {
        return 0;
    }

    Graph graph = CreateGraph(N, M);

    if (InputValue(graph) == false) {
        free(graph.Matrix);
        return 0;
    }

    AlgorithmPrima(graph);

    return 0;
}

