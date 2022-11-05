#include<stdio.h>
#include "myStack.h"
#include "directedGraph.h"



//[30 points] Non-recursive DFS routine to assign dfs number to each vertices, classify edges
void dfs(myGraph *G){
	int noOfVertices = G->numOfVertices;
	int noOfEdges = G->numOfEdges;
	adjListEntry *adjList = G->adjList;
	int time = 1;
	int count = 0;
	for(int i=0;i<noOfVertices;i++){
		
		if( adjList[i].dfsNum==-1){
			myStack stackR;
			createStack(&stackR,noOfVertices);	
			
				
			myStack stackS;
			createStack(&stackS,noOfVertices);
			int *onStack = (int*)malloc(sizeof(int)*noOfVertices);
			int *isParent = (int*)malloc(sizeof(int)*noOfVertices);
			int *parent = (int*)malloc(sizeof(int)*noOfVertices);
			for(int i=0;i<noOfVertices;i++){
				onStack[i] = 0;
				isParent[i] = 0;
				parent[i] = -1;
			}
			
			push(&stackR,i);
			parent[i] = i;
			push(&stackS,i);
			onStack[i] = 1;
			
			adjList[i].dfsNum=time;
			adjList[i].lowNum=time;
			
			time++;
			int data=-1;
			
			while(pop(&stackR,&data)==0){
				
				if(isParent[data]==0){	
					push(&stackR,data);
					//push(&stackS,data);
					//onStack[data] = 1;
					isParent[data]=1;

					edgeListNode *temp = adjList[data].edgeListHeadPtr;
					while(temp!=NULL){
							
						if(adjList[temp->vertex].dfsNum==-1){
							push(&stackR,temp->vertex);
							push(&stackS,temp->vertex);
							onStack[temp->vertex] = 1;
							temp->edgeType=1;
							
							adjList[temp->vertex].dfsNum=time;
							adjList[temp->vertex].lowNum=time;
						
							time++;
							parent[temp->vertex] = data;	
							
						}else{
							/*if(temp->vertex== parent[data]){
								temp = temp->linkPtr;
								continue;
							}	*/
						
							if(onStack[temp->vertex]==1 /*&& temp->vertex!= parent[data]*/){
								temp->edgeType = 3;
								int nlow = adjList[data].lowNum;
								int cdfs = adjList[temp->vertex].dfsNum;
								adjList[data].lowNum = nlow>cdfs ? cdfs : nlow;
							
							}else if(adjList[temp->vertex].lowNum < adjList[data].lowNum){
								temp->edgeType = 4;
							}else if(adjList[temp->vertex].lowNum > adjList[data].lowNum){
								temp->edgeType = 2;
							}									
						}		
						temp = temp->linkPtr;
					}
				}else{
					edgeListNode *temp = adjList[data].edgeListHeadPtr;
					while(temp!=NULL){
						if(temp->edgeType==1){
							int nlow = adjList[data].lowNum;
							int clow = adjList[temp->vertex].lowNum;
							adjList[data].lowNum = nlow>clow ? clow : nlow;
						}
						temp = temp->linkPtr;
					}
					if(adjList[data].lowNum==adjList[data].dfsNum){
						count+=1;
						int t=-1;
						
						adjList[data].compNum = data;
						
						//printf("\ncomponent value: %d ",data);
						while(pop(&stackS,&t)==0 && t!=data){
							onStack[t] = 0;	
							//printf("%d ",t);
							adjList[t].compNum = data;
						}
						//printf("\n");					
					}
				}
			}
		}
	}
	
   	G->numOfComp  = count;
}

// [20 points] Returns 1 if the graph is strongly connected, 0 ow -- use low number
int isStronglyConnected(myGraph *G)
{
	printf("\nNo of components: %d\n",G->numOfComp);
	if(G->numOfComp ==1){
		return 1;
	}else{
		return 0;
	}
}

//[50 points] Returns the component graph in which each vertex represents a strongly connected component in G 
void SCC(myGraph *G, myGraph *compGraph)
{
	int noOfVertices = G->numOfVertices;
	int noOfEdges = G->numOfEdges;
	int noOfComp = G->numOfComp;
	adjListEntry *adjList = G->adjList;
	

    createGraph(compGraph,noOfVertices);

 	adjListEntry *compAdjList = compGraph->adjList;
	
 	
	for(int i=0;i<noOfVertices;i++){
		
		if(adjList[i].compNum != i){
			//printf("v: %d comp: %d\n",i,adjList[i].compNum);
			addEdge(compGraph,adjList[i].compNum,i);
		}else{
			compGraph->adjList[i].dfsNum = 1;
		}
	}   
}
/*
 * printComponentGraph: used to print graphical representation of component graph
 *
 *
 */
void printComponentGraph(myGraph *G,myGraph *compGraph, char *opDotFileName)
{
    FILE *opFilePtr;
    
    edgeListNodePtr edgeNodePtr;

    opFilePtr = fopen(opDotFileName, "w");
    if (opFilePtr == NULL){
        printf("printGraph(): can't create file %s\n", opDotFileName);
        exit(EXIT_FAILURE);
    }
	adjListEntry *adjList = G->adjList;
	adjListEntry *compAdjList = compGraph->adjList;
	
	

    fprintf(opFilePtr, "digraph g{\n");	
	
    for (int i = 0; i < G->numOfVertices; i++){
    	
    	if(compAdjList[i].dfsNum==1){
    	
		    fprintf(opFilePtr, "%d [label = \"",i);
		    
		    edgeNodePtr = compAdjList[i].edgeListHeadPtr;
		    fprintf(opFilePtr,"%d",i);
		    while(edgeNodePtr){
		 		fprintf(opFilePtr,",%d",edgeNodePtr->vertex);
		        edgeNodePtr = edgeNodePtr->linkPtr;
		    }  
		    fprintf(opFilePtr, "\"];\n");  
		} 
    }
	
	int *parent = (int*)malloc(sizeof(int)*G->numOfVertices);
	for(int i=0;i<G->numOfVertices;i++){
		parent[i] = -1;
	}
	for (int i = 0; i < G->numOfVertices; i++){
    	edgeNodePtr = adjList[i].edgeListHeadPtr;
        int compNum = adjList[i].compNum;
        
        while(edgeNodePtr!=NULL){
        	int ecompNum = adjList[edgeNodePtr->vertex].compNum;	        		
       		if(ecompNum!=compNum /*&& edgeNodePtr->edgeType==1*/){
        		if(parent[compNum]==-1 || parent[ecompNum]==-1){
        			fprintf(opFilePtr,"%d -> %d;\n",compNum,ecompNum);
        			parent[compNum] =1;
        			parent[ecompNum]=1;
        			//break;	
        		}
           	}	
        	edgeNodePtr = edgeNodePtr->linkPtr;	
        }
    }	
	
	
    fprintf(opFilePtr,"}\n");

    fclose(opFilePtr);
}



//---------------------------------------------------------------------------------------


int main()
{
    myGraph G;

    readDirectedGraph("./TestCases/mediumDG.txt", &G);

    printf("#vertices = %d #edged = %d\n", G.numOfVertices, G.numOfEdges);
    printAdjList(&G);

    dfs(&G);
    //printAdjList(&G);
    printGraph(&G, "./dfs_mediumDG.dot");
	system("dot -Tpng dfs_mediumDG.dot -o dfs_mediumDG.png");
    //Calls to other functions
    printf("\n--------------------------------------------------------------------\n");
    if(isStronglyConnected(&G)){
    	printf("\nIs Graph StronglyConnected: Yes\n");
    }else{
    	printf("\nIs Graph StronglyConnected: No\n");
    }
    
    
    myGraph compGraph;
    printf("\n\nAll connected Component\n");
    SCC(&G,&compGraph);
    int len = compGraph.numOfVertices;
    for(int i=0;i<len;i++){
    	if(compGraph.adjList[i].dfsNum==1){
    		printf("Component: %d ",i);
    		edgeListNodePtr edgeNodePtr = compGraph.adjList[i].edgeListHeadPtr;
    		while(edgeNodePtr!=NULL){
    			printf("%d ",edgeNodePtr->vertex);
    			edgeNodePtr = edgeNodePtr->linkPtr;
    		}
    		printf("\n");
    	}
    }
    printComponentGraph(&G,&compGraph,"dfs_compnent.dot");
    system("dot -Tpng dfs_compnent.dot -o dfs_compnent.png");
    printf("\nConnected component graph generated in file dfs_compnent.png\n");
    
    deleteGraph(&G);

    return 0;
}
