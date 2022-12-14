#include <stdio.h>
#include "myStack.h"
#include "directedGraph.h"

int min(int a, int b)
{
    return a < b ? a : b;
}

//[30 points] Non-recursive DFS routine to assign dfs number to each vertices, classify edges
void dfs(myGraph *G)
{
    int clock = 0;
    myStack s;
    createStack(&s, G->numOfVertices);
    int *visited = (int *)malloc(sizeof(int) * G->numOfVertices);
    int *parent = (int *)malloc(sizeof(int) * G->numOfVertices);
    for (int i = 0; i < G->numOfVertices; i++)
    {
        visited[i] = 0;
        parent[i] = -1;
    }

    printf("DFS: ");
    for (int v = 0; v < G->numOfVertices; v++)
    {
        if (visited[v] == 0)
        {
            printf("\v v = %d\n", v);

            push(&s, v);
            while (s.top != -1)
            {
                int u;
                pop(&s, &u);
                if (visited[u] == 0)
                {
                    printf("%d (%d)  ", u, parent[u]);
                    G->adjList[u].dfsNum = clock++;
                    G->adjList[u].lowNum = G->adjList[u].dfsNum;
                    visited[u] = 1;
                    edgeListNodePtr tmp = G->adjList[u].edgeListHeadPtr;
                    while (tmp)
                    {
                        int w = tmp->vertex;
                        if (visited[w] == 0)
                        {
                            push(&s, w);

                            if (parent[w] != -1)
                            {
                                edgeListNodePtr edge = G->adjList[parent[w]].edgeListHeadPtr;
                                while (edge)
                                {
                                    if (edge->vertex == w)
                                    {
                                        edge->edgeType = 2;
                                        break;
                                    }
                                    edge = edge->linkPtr;
                                }
                            }

                            parent[w] = u;
                            tmp->edgeType = 1;
                        }
                        else if (visited[w] == 1)
                        {
                            G->adjList[u].lowNum = min(G->adjList[u].lowNum, G->adjList[w].dfsNum);
                            tmp->edgeType = 3;
                        }

                        else if (visited[w] == 2)
                        {
                            tmp->edgeType = 4;
                        }
                        tmp = tmp->linkPtr;
                    }
                }
                else if (visited[u] == 1)
                {
                    G->adjList[parent[u]].lowNum = min(G->adjList[u].lowNum, G->adjList[parent[u]].lowNum);
                    visited[u] = 2;
                }
                else
                {
                    int explored;
                    pop(&s, &explored);
                }
            }

            printf("\nVisited array: ");
            for (int x = 0; x < G->numOfVertices; x++)
                printf("%d  ", visited[x]);
        }
    }
    printf("\n");
    destroyStack(&s);
}

void dfs2(myGraph *G)
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
                        printf("\n Component %d: ", compCnt);
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
    printf("\ncomponents = %d\n", G->numOfComp);
    destroyStack(&s);
}

// [20 points] Returns 1 if the graph is strongly connected, 0 ow -- use low number
int isStronglyConnected(myGraph *G)
{
    if (G->numOfComp == 1)
        return 0;

    return 0;
}

//[50 points] Returns the component graph in which each vertex represents a strongly connected component in G
void SCC(myGraph *G, myGraph *compGraph)
{

    int clock = 0;
    myStack s;
    createStack(&s, G->numOfVertices);
    myStack s2;
    createStack(&s2, G->numOfVertices);
    int *visited = (int *)malloc(sizeof(int) * G->numOfVertices);
    int *parent = (int *)malloc(sizeof(int) * G->numOfVertices);
    for (int i = 0; i < G->numOfVertices; i++)
    {
        visited[i] = 0;
        parent[i] = -1;
    }

    printf("SCC: ");
    for (int v = 0; v < G->numOfVertices; v++)
    {
        if (visited[v] == 0)
        {

            push(&s, v);
            while (s.top != -1)
            {
                int u;
                pop(&s, &u);
                if (visited[u] == 0)
                {
                    G->adjList[u].dfsNum = clock++;
                    G->adjList[u].lowNum = G->adjList[u].dfsNum;
                    visited[u] = 1;
                    edgeListNodePtr tmp = G->adjList[u].edgeListHeadPtr;
                    while (tmp)
                    {
                        int w = tmp->vertex;
                        if (visited[w] == 0)
                        {
                            push(&s, w);

                            if (parent[w] != -1)
                            {
                                edgeListNodePtr edge = G->adjList[parent[w]].edgeListHeadPtr;
                                while (edge)
                                {
                                    if (edge->vertex == w)
                                    {
                                        edge->edgeType = 2;
                                        break;
                                    }
                                    edge = edge->linkPtr;
                                }
                            }

                            parent[w] = u;
                            tmp->edgeType = 1;
                        }
                        else if (visited[w] == 1)
                        {
                            G->adjList[u].lowNum = min(G->adjList[u].lowNum, G->adjList[w].dfsNum);
                            tmp->edgeType = 3;
                        }

                        else if (visited[w] == 2)
                        {
                            tmp->edgeType = 4;
                        }
                        tmp = tmp->linkPtr;
                    }
                }
                else if (visited[u] == 1)
                {

                    if (G->adjList[u].dfsNum == G->adjList[u].lowNum)
                    {
                        int comp;
                        while (s2.top != -1)
                        {
                            pop(&s2, &comp);
                            printf("%d ", comp);
                            if (u == comp)
                                break;
                        }
                        printf("\n");
                    }

                    G->adjList[parent[u]].lowNum = G->adjList[u].lowNum > G->adjList[parent[u]].lowNum ? G->adjList[parent[u]].lowNum : G->adjList[u].lowNum;
                    visited[u] = 2;
                }
                else
                {
                    int explored;
                    pop(&s, &explored);
                }
            }
        }
    }
    printf("\n");
    destroyStack(&s);    
    destroyStack(&s2);

}

//---------------------------------------------------------------------------------------

int main()
{
    myGraph G;

    readDirectedGraph("./TestCases/digraph2.txt", &G);

    printf("#vertices = %d #edged = %d\n", G.numOfVertices, G.numOfEdges);
    printAdjList(&G);

    dfs2(&G);
    printGraph(&G, "./dfs_mediumDG.dot");

    system("dot -Tpng ./dfs_mediumDG.dot -o ./dfs_mediumDG.png");

    // Calls to other functions

    myGraph scc;

    // SCC(&G, &scc);

    deleteGraph(&G);

    return 0;
}
