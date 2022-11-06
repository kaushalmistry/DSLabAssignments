#include<stdio.h>
#include "myStack.h"
#include "directedGraph.h"
#define INT_MIN -9999999

int min(int a, int b) {
    return a < b ? a : b;
}

//Non-recursive DFS routine to assign dfs number to each vertices, classify edges
void dfs(myGraph *G)
{
    
    int clock = 1;
    int compCnt = 0;
    int noOfVertices = G->numOfVertices;
    edgeListNodePtr curHead[noOfVertices];


    int *visited = (int *)malloc(sizeof(int) * G->numOfVertices);
    int *parent = (int *)malloc(sizeof(int) * G->numOfVertices);
    int *inStack = (int *)malloc(sizeof(int) * G->numOfVertices);
    int *inStack2 = (int *)malloc(sizeof(int) * G->numOfVertices);

    for (int i = 0; i < G->numOfVertices; i++)
    {
        visited[i] = 0;
        parent[i] = -1;
        inStack[i] = 0;
        curHead[i] = G->adjList[i].edgeListHeadPtr;
    }

    myStack s;
    createStack(&s, G->numOfVertices);

    myStack componentStack;
    createStack(&componentStack, G->numOfVertices);

    printf("DFS: ");
    int w;
    for (int u = 0; u < noOfVertices; u++)
    {
        if (visited[u] == 0)
        {
            push(&s, u);
            G->adjList[u].dfsNum = clock;
            G->adjList[u].lowNum = clock;
            clock++;
            visited[u] = 1;
            push(&componentStack, u);
            inStack[u] = 1;
            inStack2[u] = 1;

            int v;

            while (s.top != -1) {
                v = s.arrStack[s.top];

                if (curHead[v]) {
                    w = curHead[v]->vertex;
                    if (visited[w] == 0) {
                        push(&s, w);
                        push(&componentStack, w);
                        inStack2[w] = 1;
                        inStack[w] = 1;
                        visited[w] = 1;

                        G->adjList[w].dfsNum = clock;
                        G->adjList[w].lowNum = clock;
                        clock++;
                        curHead[v]->edgeType = 1;
                    } else {
                        if (G->adjList[w].dfsNum > G->adjList[v].dfsNum) {
                            curHead[v]->edgeType = 2;
                        } 
                        else if (inStack2[w] == 0) {
                            curHead[v]->edgeType = 4;
                        } else {
                            curHead[v]->edgeType = 3;
                        }
                        if (inStack[w] == 1) {
                            G->adjList[v].lowNum = min(G->adjList[v].lowNum, G->adjList[w].dfsNum);
                        }
                    }
                    curHead[v] = curHead[v]->linkPtr;
                } else {
                    pop(&s, &v);
                    inStack2[v] = 0;
                    if (s.top != -1) {
                        int x = s.arrStack[s.top];
                        G->adjList[x].lowNum = min(G->adjList[x].lowNum, G->adjList[v].lowNum);
                    }

                    if (G->adjList[v].dfsNum == G->adjList[v].lowNum) {
                        int data = -1;
                        printf("\nComponent %d: ", compCnt);
                        while (componentStack.top != -1 && data != v) {
                            pop(&componentStack, &data);
                            inStack[data] = 0;
                            G->adjList[data].compNum = compCnt;
                            printf("%d ", data);
                        }
                        compCnt++;
                        G->numOfComp = compCnt;
                    }
                }
            }
        }
    }
    printf("\n\nTotal number of components = %d\n", G->numOfComp);
    destroyStack(&s);
    destroyStack(&componentStack);
}

// Returns the component graph in which each vertex represents a strongly connected component in G
void SCC(myGraph *G, myGraph *compGraph)
{
    int n = G->numOfComp;

    int leaders[n];
    for (int i = 0; i < G->numOfVertices; i++) {
        if (G->adjList[i].dfsNum == G->adjList[i].lowNum) {
            leaders[G->adjList[i].compNum] = i;
        }
    }

    int compEdges[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            compEdges[i][j] = 0;
        }
    }
    int u, v;
    edgeListNodePtr edgePtr;
    for (u = 0; u < G->numOfVertices; u++) {
        edgePtr = G->adjList[u].edgeListHeadPtr;

        while (edgePtr) {
            v = edgePtr->vertex;

            if (G->adjList[u].compNum != G->adjList[v].compNum) {
                compEdges[G->adjList[u].compNum][G->adjList[v].compNum] = 1;
            }
            edgePtr = edgePtr->linkPtr;
        }
    }

    createGraph(compGraph, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (compEdges[i][j] == 1) {
                addEdge(compGraph, i, j);
            }
        }
    }
}

// A directed graph G = (V, E)is semiconnected if, for all pairs 
// of vertices u and v, we have a path from u to v or v to u.
// Time complexity O(V+E)
// Score - [30 Marks]
int isSemiConnected(myGraph *G1)
{
    myGraph G;
    SCC(G1, &G);
    // Generating the SCC for the given graph and will check for semi connected


    // Applying the topological sort to find the topological order
    int n = G.numOfVertices;
    int inDegree[n];
    int cnt = 0;
    int order[n];

    for (int i = 0; i < n; i++) {
        inDegree[i] = order[i] = 0;
    }

    edgeListNodePtr edgePtr;
    // Calculating the in degrees of the graph vertices
    for (int i = 0; i < n; i++) {
        edgePtr = G.adjList[i].edgeListHeadPtr;

        while (edgePtr)
        {
            inDegree[edgePtr->vertex]++;
            edgePtr = edgePtr->linkPtr;
        }
    }

    // Finding the topological order
    int u, v, newDistance;
    while (cnt < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (inDegree[i] == 0) {
                order[cnt] = i;
                found = 1;
                u = i;
                inDegree[i] = -1;
                cnt++;
                break;
            }
        }
        
        if (!found) {
            return 0; // The graph is not semi connected as we can't find the topological order for it as it would be disconnected
        }

        edgePtr = G.adjList[u].edgeListHeadPtr;

        while (edgePtr)
        {
            v = edgePtr->vertex;
            inDegree[v]--;
            edgePtr = edgePtr->linkPtr;
        }
    }

    // Checking if the topological order is a chain or not
    for (int i = 1; i < n; i++) {
        u = order[i-1];
        v = order[i];

        edgePtr = G.adjList[u].edgeListHeadPtr;
        int flag = 0;
        while (edgePtr) {
            if (edgePtr->vertex == v) {
                flag = 1;
                break;
            }
            
            edgePtr = edgePtr->linkPtr;
        }

        // If it is not a chain then it is not semi connected and returns 0
        if (!flag) {
            return 0;
        }
    }

    // If the order maintains the chain then it is semi connected and returns 0
    return 1;
}

//Find the longest path and its length in the component graph of G
// Score - [30 Marks]
void longestPath(myGraph *G, int *path, int *pathLen)
{
    // Considering the edge weights of all the edges to be 1

    // Relaxing the edges in topological order fashion
    int n = G->numOfVertices;
    int parent[n];
    int inDegree[n];
    int distance[n];

    for (int i = 0; i < n; i++) {
        parent[i] = inDegree[i] = 0;
        distance[i] = INT_MIN;
    }

    edgeListNodePtr edgePtr;
    for (int i = 0; i < n; i++) {
        edgePtr = G->adjList[i].edgeListHeadPtr;

        while (edgePtr)
        {
            inDegree[edgePtr->vertex]++;
            edgePtr = edgePtr->linkPtr;
        }
    }

    // Finding the topological order along with maintaing the parent

    int cnt = 0;
    int u, v, newDistance;
    while (cnt < n) {
        for (int i = 0; i < n; i++) {
            if (inDegree[i] == 0) {
                u = i;
                cnt++;
                inDegree[i] = -1;
                break;
            }
        }

        if (distance[u] == INT_MIN) {
            distance[u] = 0;
        }

        edgePtr = G->adjList[u].edgeListHeadPtr;

        while (edgePtr)
        {
            v = edgePtr->vertex;

            // Updating the new distance if it is larger
            newDistance = distance[u] + 1;

            // Relaxing all the vertices to have their highest distance
            if (newDistance > distance[v]) {
                parent[v] = u;
                distance[v] = newDistance;
                if (inDegree[v] == -1) {
                    inDegree[v] = 0;
                }
            }

            // Updating the in degree of the vertex
            inDegree[v]--;
            edgePtr = edgePtr->linkPtr;
        }
        
    }

    // Finding the maximum distance node
    *pathLen = 0;
    int nodeWithLargestDistance = 0;
    for (int i = 0; i < n; i++) {
        if (distance[i] > *pathLen) {
            *pathLen = distance[i];
            nodeWithLargestDistance = i;
        }
    }
    *pathLen = *pathLen;

    int temp[*pathLen];

    // Finding the path from the maximum distance node in reverse order
    for (int i = 0; i <= *pathLen; i++) {
        temp[i] = nodeWithLargestDistance;
        nodeWithLargestDistance = parent[nodeWithLargestDistance];
    }

    // Updating the path from source to destination
    int k = 0;
    for (int i = *pathLen; i >= 0; i--) {
        path[k] = temp[i];
        k++;
    }

}

//---------------------------------------------------------------------------------------



int main()
{
    myGraph G;

    readDirectedGraph("./TestCases/digraph2.txt", &G);

    printf("#vertices = %d #edged = %d\n", G.numOfVertices, G.numOfEdges);
    printAdjList(&G);

    dfs(&G);
    printGraph(&G, "./dfs_mediumDG.dot");
    system("dot -Tpng ./dfs_mediumDG.dot -o ./dfs_mediumDG.png");
    //Calls to other functions

    myGraph compGraph;
    SCC(&G, &compGraph);

    printGraph(&compGraph, "./scc.dot");
    system("dot -Tpng ./scc.dot -o ./scc.png");

    int* path = (int*) calloc (compGraph.numOfVertices, sizeof(int));
    int pathLen;
    longestPath(&compGraph, path, &pathLen);

    printf("\n\nPath length = %d", pathLen);
    printf("\nPath = ");
    for (int i = 0; i <= pathLen; i++) {
        printf("%d ", path[i]);
    }
    printf("\n");


    if (isSemiConnected(&G)) {
        printf("\nThe graph is semi connected.\n");
    } else {
        printf("\nThe graph is not semi connected.\n");
    }

    deleteGraph(&G);
    deleteGraph(&compGraph);

    return 0;
}
