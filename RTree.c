#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdio.h>
#include<math.h>
/*Libraries to include above this*/

#define dim 2               //number of dimensions = 2
#define M 4
#define m 2
/*Macros defined above this*/

typedef struct RTree* RTree;
typedef struct Node* Node;
typedef struct Internal* Internal;
typedef struct Leaf* Leaf;

typedef enum nodeType nodeType;
/*All typedefs above this*/

enum nodeType{LEAF = 0,INTERNAL = 1};
/*All enums defined above this*/

RTree createNewRTree();
bool isEmpty(RTree rtree);
Node createNewNode(nodeType type);
Node createNewLeafNode(char* tupleIdentifier,int coordinates[dim]);
Node createNewInternalNode(Node* children, int childrenCount);
bool updateMBR(Node node);
int nodeLevel(Node node);
bool isRoot(Node node);
bool isLeaf(Node node);
bool isInternal(Node node);
/*Function declarations above this*/

struct Node
{
    Node Parent;
    bool isLeaf;
    int I[2][dim];          //Minimum Bounding region
    void * E;
};

struct Internal
{
    Node Child[M];          // array of pointers pointing to its children
    int childrenCount;
};

struct Leaf
{
    char* tupleIdentifier;  //name of Entity        
    int coordinates[dim];   //coordinates
    //Data data;            //points to a location in the disk storing Data
};

struct RTree
{
    Node Root;
};
/*All structure definitions above this*/

RTree createNewRTree()
{
    RTree rtree = (RTree) malloc(sizeof(struct RTree));
    rtree->Root = NULL;
    return rtree;
}

bool isEmpty(RTree rtree)
{
    if (rtree->Root == NULL)
        return true;
    return false;
}

Node createNewNode(nodeType type)
{
    Node node = (Node) malloc(sizeof(struct Node));
    node->isLeaf = !(type);
    if(node->isLeaf)
    {
        node->E = malloc(sizeof(struct Leaf));
    }
    else
    {
        node->E = malloc(sizeof(struct Internal));
    }

    return node;
}

Node createNewLeafNode(char* tupleIdentifier,int coordinates[dim])
{
    Node node = createNewNode(LEAF);
    Leaf leaf = (Leaf)node->E;
    
    leaf->tupleIdentifier = (char*) malloc(sizeof(tupleIdentifier));
    strcpy(leaf->tupleIdentifier, tupleIdentifier);

    for(int i=0;i<dim;i++)
    {
        leaf->coordinates[i] = coordinates[i];
    }

    updateMBR(node);
    return node;
}

Node createNewInternalNode(Node* children, int childrenCount)
{
    if(!(childrenCount>=m &&childrenCount<=M))
        return NULL;                                    //if NULL returned, internal node is NOT created as number of children is not between m and M

    Node node = createNewNode(INTERNAL);
    Internal internal = (Internal)node->E;

    internal->childrenCount = childrenCount;
    for(int i=0;i<childrenCount;i++)
    {
        children[i]->Parent = node;
        internal->Child[i] = children[i];
    }

    updateMBR(node);
    return node;
}

bool updateMBR(Node node)
{
    if(isLeaf(node))
    {
        Leaf leaf = (Leaf)node->E;
        for(int i=0;i<dim;i++)
        {
            node->I[0][i] = node->I[1][i] = leaf->coordinates[i];
        }
    }
    else if(isInternal(node))
    {
        Internal internal = (Internal)node->E;
        if(internal->childrenCount < m) return false;
        for(int j=0;j<dim;j++)
        {
            node->I[0][j] = (internal->Child[0]->I[0][j]);
            node->I[1][j] = (internal->Child[0]->I[1][j]);
        }
        for(int i=1;i<internal->childrenCount;i++)
        {
            for(int j=0;j<dim;j++)
            {
                node->I[0][j] = fmin(node->I[0][j],internal->Child[i]->I[0][j]);
                node->I[1][j] = fmax(node->I[1][j],internal->Child[i]->I[1][j]);
            }
        }
    }
    else
        return false;
    return true;
}

int nodeLevel(Node node)                                //O(log n) operation
{
    int count = 0;
    while(isInternal(node))
    {
        count++;
        node = ((Internal)node->E)->Child[0];
    }
    return count;
}

bool isRoot(Node node)
{
    if(node->Parent == NULL)
        return true;
    return false;
}

bool isLeaf(Node node)
{
    if(node->isLeaf)
        return true;
    return false;
}

bool isInternal(Node node)
{
    if(!(node->isLeaf))
        return true;
    return false;
}
/*R-Tree ADT and basic functions defined above*/

/*
//For debugging purposes://
int main()
{
    int l[2] = {1,3};
    int p[2] = {2,4};
    Node a = createNewLeafNode("bi",l);

    Node b = createNewLeafNode("gr",p);
    Node nar[2]={a,b};
    int c[2] = {1,2};
    Node e = createNewInternalNode(nar,sizeof(nar)/sizeof(nar[0]));
    Node d = createNewLeafNode("as",c);
    Internal internal = (Internal)e->E;
}
*/