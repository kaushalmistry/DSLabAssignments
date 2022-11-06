#include<stdio.h>
#include "myUGraph.h"
#include "myStack.h"

#define INT_MIN -9999999

int min(int a, int b) {
    return a < b ? a : b;
}

// Give an algorithm that determines whether or not a given
// undirected graph G = (V, E) contains a cycle. Your algorithm 
// should run in O(V) time, independent of |E|
// Score - [40 Marks]
int cycleExist(myUGraph *G)
{
    // The same non recursive DFS algorithm is used as it was used for directed graph
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
            parent[u] = -1;

            int v;

            while (s.top != -1) {
                v = s.arrStack[s.top];

                if (curHead[v]) {
                    w = curHead[v]->vertex;

                    // This is to traverse the node in DFS fashion
                    if (visited[w] == 0) {
                        push(&s, w);
                        push(&componentStack, w);
                        inStack2[w] = 1;
                        inStack[w] = 1;
                        visited[w] = 1;
                        parent[w] = v;

                        G->adjList[w].dfsNum = clock;
                        G->adjList[w].lowNum = clock;
                        clock++;
                        curHead[v]->edgeType = 1;
                    } else { 
                        // The back edge is detected
                        if (parent[v] != w) { // This ensures that the back edge is not identically the same edge
                            return 1; // Found the back edge so it is having the cycle
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
                }
            }
        }
    }

    destroyStack(&s);
    destroyStack(&componentStack);
    return 0; // No cycle found
}

//---------------------------------------------------------------------------------------



int main()
{
    myUGraph G;

    readUGraph("./TestCases/noCycle.txt", &G);

    printf("#vertices = %d #edged = %d\n", G.numOfVertices, G.numOfEdges);
    printUGraphAdjList(&G);

   


    //Calls to other functions
    if (cycleExist(&G)) {
        printf("\nCycle found\n");
    } else {
        printf("\nCycle not found\n");
    }
     printUGraph(&G, "graph.dot");
    
    system("dot -Tpng ./graph.dot -o ./graph.png");

    destroyUGraph(&G);

    return 0;
}
