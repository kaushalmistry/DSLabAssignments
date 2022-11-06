#include<stdio.h>
#include "myUGraph.h"



// Give an algorithm that determines whether or not a given
// undirected graph G = (V, E) contains a cycle. Your algorithm // should run in O(V) time, independent of |E|
// Score - [40 Marks]
int cycleExist(myUGraph *G)
{

}

//---------------------------------------------------------------------------------------



int main()
{
    myUGraph G;

    readUGraph("./TestCases/mediumG.txt", &G);

    printf("#vertices = %d #edged = %d\n", G.numOfVertices, G.numOfEdges);
    printUGraphAdjList(&G);

    printUGraphAdjList(&G);

    //Calls to other functions

    destroyUGraph(&G);

    return 0;
}
