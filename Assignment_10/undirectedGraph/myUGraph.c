#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "myUGraph.h"
#define BUFSIZE 256

void createUGraph(myUGraph *G, int numOfVertices)
{
    assert(numOfVertices > 0);
    G->numOfVertices = numOfVertices;
    G->numOfEdges = 0;
    G->numOfComp = -1;

    G->adjList = (adjListEntry *)malloc(numOfVertices*sizeof(adjListEntry));
    if (G->adjList == NULL)
    {
        printf("createGraph: malloc failure\n");
        exit(EXIT_FAILURE);
    }

    // Initilize
    for (int v = 0; v < numOfVertices; v++)
    {
        G->adjList[v].dfsNum = G->adjList[v].lowNum = -1;
        G->adjList[v].edgeListHeadPtr = NULL;
    }

}
void readUGraph(char *fileName, myUGraph *G)
{
    FILE *ipFilePtr;
    int i, u, v, n;
    char buf[BUFSIZE];

    ipFilePtr = fopen(fileName, "r+");
    if (ipFilePtr == NULL)
    {
        printf("readUGraph(): File %s does not exist.\n", fileName);
        exit(EXIT_FAILURE);
    }
    // First line of the file contains the number of vertices in the undirected graph
    fgets(buf, BUFSIZE, ipFilePtr);
    n = atoi(buf);

    createUGraph(G, n);

    //Read edges
    while(fgets(buf, BUFSIZE, ipFilePtr))
    {
        sscanf(buf,"%d %d", &u, &v);
        addUEdge(G, u, v);
    }
    fclose(ipFilePtr);
}

void addUEdge(myUGraph *G, int u, int v){
    assert(u >= 0 && u < G->numOfVertices);
    assert(v >= 0 && v < G->numOfVertices);

    edgeListNodePtr edgeNodePtr;
    edgeNodePtr = (edgeListNodePtr)malloc(sizeof(edgeListNode));
    if (edgeNodePtr == NULL)
    {
        printf("addEdge(): malloc failure.\n");
        exit(EXIT_FAILURE);
    }

    //Add edge u --> v
    G->numOfEdges++;
    edgeNodePtr->vertex = v;
    edgeNodePtr->edgeType  = -1;
    // Insert node at the beginning of the edge list (G[u])
    edgeNodePtr->linkPtr = G->adjList[u].edgeListHeadPtr;
    G->adjList[u].edgeListHeadPtr = edgeNodePtr;

    edgeListNodePtr edgeNodePtr2;
    edgeNodePtr2 = (edgeListNodePtr)malloc(sizeof(edgeListNode));
    if (edgeNodePtr2 == NULL)
    {
        printf("addEdge(): malloc failure.\n");
        exit(EXIT_FAILURE);
    }

    //Add edge v --> u
    G->numOfEdges++;
    edgeNodePtr2->vertex = u;
    edgeNodePtr2->edgeType  = -1;
    // Insert node at the beginning of the edge list (G[u])
    edgeNodePtr2->linkPtr = G->adjList[v].edgeListHeadPtr;
    G->adjList[v].edgeListHeadPtr = edgeNodePtr2;
}

void printUGraphAdjList(myUGraph *G){
    int v;
    edgeListNodePtr edgeNodePtr;

    for (v = 0; v < G->numOfVertices; v++)
    {
        printf("[%d] : ", v);
        edgeNodePtr = G->adjList[v].edgeListHeadPtr;
        while(edgeNodePtr)
        {
            printf("%d -> ", edgeNodePtr->vertex);
            edgeNodePtr = edgeNodePtr->linkPtr;
        }
        printf("\n");
    }
}

void printUGraph(myUGraph *G, char *opFileName){
FILE *opFilePtr;
    int v;
    
    edgeListNodePtr edgeNodePtr;

    opFilePtr = fopen(opFileName, "w");
    if (opFilePtr == NULL)
    {
        printf("printGraph(): can't create file %s\n", opFileName);
        exit(EXIT_FAILURE);
    }

    fprintf(opFilePtr, "digraph g{\n");

    for (v = 0; v < G->numOfVertices; v++)
    {
        fprintf(opFilePtr, "%d [label = \"%d \\n(%d,%d)\"]\n", v, v, G->adjList[v].dfsNum, G->adjList[v].lowNum);
        edgeNodePtr = G->adjList[v].edgeListHeadPtr;
        while(edgeNodePtr)
        {
            //printf("%d %d\n",i, edgeNodePtr->vertex);
            switch(edgeNodePtr->edgeType) // 1 tree, 2 forward, 3 back, 4 cross
            {
                case 1:
                    fprintf(opFilePtr, "%d -> %d [label = \"T\"]\n", v, edgeNodePtr->vertex);
                    break;
                case 2:
                    fprintf(opFilePtr, "%d -> %d [label = \"F\"]\n", v, edgeNodePtr->vertex);
                    break;
                case 3:
                    fprintf(opFilePtr, "%d -> %d [label = \"B\"]\n", v, edgeNodePtr->vertex);
                    break;
                case 4:
                    fprintf(opFilePtr, "%d -> %d [label = \"C\"]\n", v, edgeNodePtr->vertex);
                    break;
                default:
                    fprintf(opFilePtr, "%d -> %d \n", v, edgeNodePtr->vertex);
                    break;
            }

            edgeNodePtr = edgeNodePtr->linkPtr;
        }

    }

    fprintf(opFilePtr,"}\n");

    fclose(opFilePtr);
}

void destroyUGraph(myUGraph *G){
    int v;
    edgeListNodePtr edgeNodePtr, p;

    for (v = 0; v < G->numOfVertices; v++)
    {
        edgeNodePtr = G->adjList[v].edgeListHeadPtr;
        while(edgeNodePtr)
        {
            p = edgeNodePtr;
            edgeNodePtr = edgeNodePtr->linkPtr;
            free(p);
        }
    }
    free(G->adjList);
}




