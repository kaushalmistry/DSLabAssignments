#include <stdio.h>
#include <stdlib.h>
#include "myUGraph.h"
#define BUFSIZE 256

void createUGraph(myUGraph *G, int numOfVertices)
{

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

}
void printUGraphAdjList(myUGraph *G){

}
void printUGraph(myUGraph *G, char *opFileName){

}

void destroyUGraph(myUGraph *G){
}




