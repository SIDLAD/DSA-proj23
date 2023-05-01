#include <stdbool.h>

#define dim 2               //number of dimensions = 2
#define M 4
#define m 2

typedef struct rTree* RTree;
typedef struct node* Node;
typedef struct entry* Entry;
typedef struct data* Data;
typedef enum nodeType NodeType;

struct entry
{
    float I[2][dim];
};

struct node
{
    float I[2][dim];
    bool isLeaf;
    int entryCount;
    Entry entries[M+1];                                               //entries can be an array of child-nodes or data entries depending on whether the node is leaf node or internal node
    //NOTE: entries can be at max M, but leaves space for M+1 so that a node to be split during insertion can TEMPORARILY store an extra entry
    Node parent;
};

struct data
{
    float I[2][dim];
    float coordinates[dim];
    char* tupleIdentifier;
};

struct rTree
{
    Node root;
};
enum nodeType{LEAF = 0,INTERNAL = 1};

RTree createNewRTree();
bool isEmpty(RTree rtree);
Data createDataItem(float coordinates[dim],char* tupleIdentifier);
Node createNewNode(NodeType type,int entryCount,Entry entries[]);   //called implicitly while creating leaf or internal node
Node createNewLeafNode(int entryCount,Data dataEntries[]);
Node createNewInternalNode(int entryCount,Node nodeEntries[]);
bool defineMBR(Node node);
int nodeLevel(Node node);
int nodeHeight(Node node);
bool isRoot(Node node);