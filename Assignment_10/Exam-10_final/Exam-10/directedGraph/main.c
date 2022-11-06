#include<stdio.h>
#include "myStack.h"
#include "directedGraph.h"



//Non-recursive DFS routine to assign dfs number to each vertices, classify edges
void dfs(myGraph *G)
{

}

// Returns the component graph in which each vertex represents a strongly connected component in G
void SCC(myGraph *G, myGraph *compGraph)
{

}

// A directed graph G = (V, E)is semiconnected if, for all pairs // of vertices u and v, we have a path from u to vor v to u.
// Time complexity O(V+E)
// Score - [30 Marks]
int isSemiConnected(myGraph *G)
{

}

//Find the longest path and its length in the component graph
// Score - [30 Marks]
void longestPath(myGraph *G, int *path, int *pathLen)
{

}

//---------------------------------------------------------------------------------------



int main()
{
    myGraph G;

    readDirectedGraph("./TestCases/mediumDG.txt", &G);

    printf("#vertices = %d #edged = %d\n", G.numOfVertices, G.numOfEdges);
    printAdjList(&G);

    printGraph(&G, "./dfs_mediumDG.dot");

    //Calls to other functions

    deleteGraph(&G);

    return 0;
}
