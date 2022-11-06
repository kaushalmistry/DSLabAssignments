#ifndef MYUGRAPH_H_
#define MYUGRAPH_H_

struct edgeListNode{
    int vertex;
    int edgeType; // 1 tree, 2 forward
    struct edgeListNode *linkPtr;
};
typedef struct edgeListNode edgeListNode;
typedef struct edgeListNode * edgeListNodePtr;

struct adjListEntry{
    int dfsNum;
    int lowNum;
    int bfsLevelNum; //level number assigned by bfs
    edgeListNodePtr edgeListHeadPtr;
};

typedef struct adjListEntry adjListEntry;

struct myUGraph{
    int numOfVertices; // Number of vertices
    int numOfEdges;  // Number of edges
    int numOfComp;  // Number of connected components
    adjListEntry *adjList; // Array
};

typedef struct myUGraph myUGraph;


//Allocate space for vertex array and initialize
void createUGraph(myUGraph *G, int numOfVertices);
void readUGraph(char *fileName, myUGraph *G);
// Add an undirected edge u -- v in G
void addUEdge(myUGraph *G, int u, int v);
void printUGraphAdjList(myUGraph *G);
void printUGraph(myUGraph *G, char *opFileName);
void destroyUGraph(myUGraph *G);
#endif
