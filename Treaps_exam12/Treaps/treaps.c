#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct tnode
{
    int key, priority;
    int left, right;
    int parent; // optional
};

typedef struct tnode tnode;

struct Treap
{
    int root;
    int freeList;
    tnode *tnodeArr;
    int numElements;
};

typedef struct Treap Treap;

//[5 points]
void init(Treap *T, int numElements)
{
    T->tnodeArr = (tnode *)malloc(numElements * sizeof(tnode));
    if (T->tnodeArr == NULL)
        exit(EXIT_FAILURE);

    T->numElements = numElements;

    // Initialize free list here
    // Initializing free list here. Initially it points to the every next node in the list
    for (int i = 0; i < numElements; i++)
    {
        T->tnodeArr[i].left = i + 1;
    }

    T->tnodeArr[numElements - 1].left = -1; // Initializing last node of free list as -1 as it indicates end of free list

    T->root = -1;
    T->freeList = 0;
}


// [20 points]
void deleteTreap(Treap *T, int key) {

}

int max(int a, int b) {
    return a > b ? a : b;
}

int heightHelper(Treap* T, int root) {
    if (root == -1) {
        return 0;
    }
    return max(heightHelper(T, T->tnodeArr[root].left), heightHelper(T, T->tnodeArr[root].right)) + 1;
}
// [5 points]
int height(Treap *T) {
    return heightHelper(T, T->root);
}

void displayTreapHelper(Treap *T, int root, FILE *fptr)
{
    if (root != -1)
    {
        // The below line is to show the balance factor and size along with the key of the node
        fprintf(fptr, "%d [label = \"%d, %d\n(%d, %d)\"];\n", T->tnodeArr[root].key, T->tnodeArr[root].key, T->tnodeArr[root].priority, root, T->tnodeArr[root].parent);
        if (T->tnodeArr[root].left != -1)
        {
            fprintf(fptr, "%d -> %d [color = red];\n", T->tnodeArr[root].key, T->tnodeArr[T->tnodeArr[root].left].key);
            displayTreapHelper(T, T->tnodeArr[root].left, fptr);
        }
        if (T->tnodeArr[root].right != -1)
        {
            fprintf(fptr, "%d -> %d;\n", T->tnodeArr[root].key, T->tnodeArr[T->tnodeArr[root].right].key);
            displayTreapHelper(T, T->tnodeArr[root].right, fptr);
        }
    }
}

// [10 points]
void displayTreap(Treap *T, char *fileName)
{ // produce dot file
    FILE *fptr;

    fptr = fopen(fileName, "w");
    if (fptr == NULL)
    {
        printf("Error while opening the file with name %s", fileName);
        return;
    }
    fprintf(fptr, "digraph bst{\n");
    displayTreapHelper(T, T->root, fptr);
    fprintf(fptr, "}\n");

    fclose(fptr);
}

void leftRotate(Treap *T, int node)
{
    int child = T->tnodeArr[node].right;
    int temp = T->tnodeArr[child].left;
    int grandParent = T->tnodeArr[node].parent;

    
    T->tnodeArr[child].left = node;
    T->tnodeArr[node].right = temp;

    T->tnodeArr[node].parent = child;

    if (grandParent != -1)
    {
        if (T->tnodeArr[grandParent].left == node)
            T->tnodeArr[grandParent].left = child;
        else
            T->tnodeArr[grandParent].right = child;

        T->tnodeArr[child].parent = grandParent;
    }
    else
    {
        T->tnodeArr[child].parent = -1;
        T->root = child;
    }
}

void rightRotate(Treap *T, int node)
{
    int child = T->tnodeArr[node].left;
    int temp = T->tnodeArr[child].right;
    int grandParent = T->tnodeArr[node].parent;

    T->tnodeArr[child].right = node;
    T->tnodeArr[node].left = temp;

    T->tnodeArr[node].parent = child;

    if (grandParent != -1)
    {
        if (T->tnodeArr[grandParent].left == node)
            T->tnodeArr[grandParent].left = child;
        else
            T->tnodeArr[grandParent].right = child;

        T->tnodeArr[child].parent = grandParent;
    }
    else
    {
        T->tnodeArr[child].parent = -1;
        T->root = child;
    }
}

tnode *initNewNode(Treap *T, int key, int priority, int position)
{
    if (T->tnodeArr[position].left == -1)
    {
        printf("The array is full.");
        exit(EXIT_FAILURE);
    }
    T->freeList = T->tnodeArr[T->freeList].left;
    T->tnodeArr[position].key = key;
    T->tnodeArr[position].priority = priority;
    T->tnodeArr[position].left = -1;
    T->tnodeArr[position].right = -1;
    T->tnodeArr[position].parent = -1;
}

void handleRotations(Treap *T, int pos)
{
    int ancester = T->tnodeArr[pos].parent;

    printf("\nRotation handler = %d %d", pos, ancester);
    int count = 1;

    while (ancester != -1)
    {
    printf("\nRotation handler keys = %d %d", T->tnodeArr[pos].key, T->tnodeArr[ancester].key);
        if (T->tnodeArr[pos].priority <= T->tnodeArr[ancester].priority)
        {
            // It satisfies the priority condition of Heap i.e. heap property
            break;
        }
        else
        {
            if (T->tnodeArr[ancester].left == pos)
            {
                // printf("\nRight is called");
                rightRotate(T, ancester);
            }
            else
            {
                // printf("\nLeft is called");
                leftRotate(T, ancester);
            }

            ancester = T->tnodeArr[pos].parent;
        }
    }
}

// [20 points] -- no duplicate keys
void insertTreap(Treap *T, int key, int priority)
{
    // Root is not there initially
    if (T->root == -1)
    {
        int pos = T->freeList;
        initNewNode(T, key, priority, pos);
        T->root = pos;
    }
    // Search for the node with the key if it exists or not and if not exists then add new node
    else
    {
        int cur = T->root;
        int prev = -1;
        int foundFlag = 0;
        while (cur != -1)
        {
            if (key == T->tnodeArr[cur].key)
            {
                // The key already found
                printf("\nThe key %d already found!\n", key);
                foundFlag = 1;
                break;
            }
            prev = cur;
            if (key < T->tnodeArr[cur].key)
            {
                cur = T->tnodeArr[cur].left;
            }
            else
            {
                cur = T->tnodeArr[cur].right;
            }
        }

        if (!foundFlag)
        {
            int pos = T->freeList;
            initNewNode(T, key, priority, pos);

            if (key < T->tnodeArr[prev].key)
            {
                T->tnodeArr[prev].left = pos;
            }
            else
            {
                T->tnodeArr[prev].right = pos;
            }

            T->tnodeArr[pos].parent = prev;

            handleRotations(T, pos);
        }
    }
}

void freeTreap(Treap* T) {
    free(T);
}

int main()
{
    srand(time(NULL));

    // Generate a table of tree height for different number of nodes e.g., 100, 1000, 5000, 10000, 50000
    Treap T;
    init(&T, 100);
    
    insertTreap(&T, 68, 0);
    insertTreap(&T, 131, 42);
    insertTreap(&T, 128, 18);
    insertTreap(&T, 164, 12);
    insertTreap(&T, 95, 27);
    insertTreap(&T, 4, 46);
    insertTreap(&T, 155, 43);
    

    displayTreap(&T, "treaps.dot");
    system("dot -Tpng treaps.dot -o treaps.png");

    deleteTreap(&T, 40);
    

    printf("\nHeight of the Treap = %d\n", height(&T));

    // freeTreap(&T);

}
