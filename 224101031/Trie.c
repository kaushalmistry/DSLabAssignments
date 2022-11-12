#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CHILD 5 //'A', 'T', 'C', 'G', and special character '$'
#define BUFSIZE 256

//Node structure for compressed trie
//You may add other fields depending on your needs.
//You have to justify why the addition of extra fields helps towards efficiency
struct trieNode{
    int startIdx;
    int endIdx;
    bool isEndOfWord;
    struct trieNode* children[MAX_CHILD];
};
typedef struct trieNode trieNode;
typedef struct trieNode *trieNodePtr;

/**
 * @brief Get the Node object for the new node of the suffix tree
 * 
 * @return trieNodePtr - Newly generated node
 */
trieNodePtr getNode(int start, int end) {
    trieNodePtr newTrieNode = (trieNodePtr)malloc(sizeof(trieNode));

	if (newTrieNode == NULL)
	{
        printf("MALLOC failure");
        exit(EXIT_FAILURE);
    }
    int i;

    newTrieNode->isEndOfWord = false;
    newTrieNode->startIdx = start;
    newTrieNode->endIdx = end;

    for (i = 0; i < MAX_CHILD; i++)
        newTrieNode->children[i] = NULL;

	return newTrieNode;
}

/**
 * @brief Utility function to calculate the length of the input text
 * 
 * @param fileName - Input file name
 * @return int 
 */
int calculateLength(char * fileName) {
    FILE* fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("Error while opening the input file.");
        exit(EXIT_FAILURE);
    }

    char c;
    int len = 0;
    while (!feof(fp)) {
        fscanf(fp, "%c", &c);
        len++;
    }
    fclose(fp);

    return len;
}

//[5 points] Read text from the file
/**
 * @brief Reading the text input from the text file
 * 
 * @param fileName 
 * @param T 
 */
void readText(char *fileName, char *T)
{
    FILE* fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("Error while opening the input file.");
        exit(EXIT_FAILURE);
    }

    // Reading char by char and appending it to the string T
    int i = 0;
    while (!feof(fp)) {
        fscanf(fp, "%c", T+i);
        i++;
    }

    fclose(fp);
}

/**
 * @brief Hashing the symbols to the index 
 * 
 * @param c - char
 * @return int - Their hash index
 */
int charToIndex(char c) {
    switch (c) {
        case 'A':
            return 0;
        case 'T':
            return 1;
        case 'C':
            return 2;
        case 'G':
            return 3;
        default:
            return 4;
    }
}

/**
 * @brief Inserting the new word from T into the trie
 * 
 * @param root - Root
 * @param T - String T
 * @param start - start index of the word
 * @param end - end index of the word
 */
void insert(trieNodePtr root, char* T, int start, int end)
{
	int index;

	trieNodePtr pCrawl = root;

    // Checking if the key exists or not 
    int l = start;
    index = charToIndex(T[l]);
    while (l <= end) {
        if (!pCrawl->children[index]) { // If it is not then create the new node in trie
            pCrawl->children[index] = getNode(l, end);
        } else { // Check till which position the string is matching

            // Crawl through the string to find the match till
            pCrawl = pCrawl->children[index];
            int tempStart = pCrawl->startIdx;
            int tempEnd = pCrawl->endIdx;
            int j = tempStart;

            // printf("\ntempStart = %d", tempStart);
            // printf("\ntempEnd = %d", tempEnd);
            // printf("\ni = %d", l);
            // printf("\nj = %d", j);
            // printf("\nEnd = %d", end);
            // Crawiling through
            while (l <= end && j <= tempEnd) {
                if (T[l] != T[j])
                    break;
                l++;
                j++;
            }
            // printf("\ntempStart = %d", tempStart);
            // printf("\ntempEnd = %d", tempEnd);
            // printf("\ni = %d", l);
            // printf("\nj = %d", j);
            if (j > tempEnd) {
                index = charToIndex(T[l]);
                // printf("\nUpdated index = %d", index); 
            } else {
                pCrawl->startIdx = j - 1;
                pCrawl->endIdx = j - 1;
                
                // Creating two branches
                trieNodePtr n1 = getNode(j, tempEnd);
                trieNodePtr n2 = getNode(l, end);

                // Updating the current node
                pCrawl->children[charToIndex(T[l])] = n2;
                pCrawl->children[charToIndex(T[j])] = n1;

                pCrawl->isEndOfWord = false; // Marking it as non leaf
            }

        }
    }
}


//[40 points] Create suffix tree from the text T
/**
 * @brief Create a Suffix Tree 
 * 
 * @param root - Root of the trie
 * @param T - String T
 */
void createSuffixTree(trieNodePtr *root, char *T)
{
    int len = strlen(T) - 1;
    *root = getNode(-1, -1);

    int word = 0;

    // Looping through all the sub strings
    while (word <= len) {
        insert(*root, T, word, len);
        word++;
    }

}

//Find all patterns P in the suffix tree and returns starting positions of the pattern P in array patLocArr
//patLocArrLen denotes the number of times the pattern P appears
//[20 points]
void findAllPatterns(trieNodePtr root, char *P, int *patLocArr, int *patLocArrLen)
{

}

/**
 * @brief Print Helper to add the node in dot file recursively
 * 
 * @param root - Root
 * @param fp - File pointer
 */
void printHelper(trieNodePtr root, FILE* fp) {
    for (int i = 0; i < 4; i++) {
        if (root->children[i]) {
            fprintf(fp, "%u [label=\"(%d, %d)\"];\n", root->children[i], root->children[i]->startIdx, root->children[i]->endIdx);
            fprintf(fp, "%u->%u;\n", root, root->children[i]);
            printHelper(root->children[i], fp);
        }
    }
}


//[10 points]
/**
 * @brief Creating dot file of the trie
 * 
 * @param root 
 * @param fileName 
 */
void displaySuffixTree(trieNodePtr root, char *fileName)
{
    FILE* fp = fopen(fileName, "w");

    if (root != NULL) {
        fprintf(fp, "digraph {");
        fprintf(fp, "root;\n");
    
        for (int i = 0; i < 4; i++) {
            if (root->children[i]) {
                fprintf(fp, "%u [label=\"(%d, %d)\"];\n", root->children[i], root->children[i]->startIdx, root->children[i]->endIdx);
                fprintf(fp, "root->%u;\n", root->children[i]);
                printHelper(root->children[i], fp);
            }
        }
        
        fprintf(fp, "}");

    }

    fclose(fp);
}

//[5 points] Prints the number of nodes, total size in main memory of the suffixtree
void statistics(trieNodePtr root)
{

}

int main()
{
    char *T, *P;
    int *patLocArr, patLocArrLen;
    trieNodePtr root = NULL;

    char* fileName = "test.txt";

    T = (char*) malloc (sizeof(char) * calculateLength(fileName));
    readText(fileName, T);

    // printf("%s\n", T);

    createSuffixTree(&root, T);

    displaySuffixTree(root, "test.dot");

    system("dot -Tpng test.dot -o test.png");

    // P = "ATCG...";// find this pattern in suffix tree
    // findAllPatterns(root, P, patLocArr, &patLocArrLen);

    // for (int i = 0; i < patLocArrLen; i++)
    //     printf("%d ", patLocArr[i]);

    printf("\n");
}
