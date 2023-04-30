#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdio.h>
#include<math.h>
#include "RTree.h"
/*Libraries to include above this*/

// #define dim 2               //number of dimensions = 2
// #define M 4
// #define m 2
// /*Macros defined above this*/

// typedef struct rTree* RTree;
// typedef struct node* Node;
// typedef struct entry* Entry;
// typedef struct data* Data;
// typedef enum nodeType NodeType;
// /*All typedefs above this*/

// enum nodeType{LEAF = 0,INTERNAL = 1};
// /*All enums defined above this*/

// RTree createNewRTree();
// bool isEmpty(RTree rtree);
// Data createDataItem(float coordinates[dim],char* tupleIdentifier);
// Node createNewNode(NodeType type,int entryCount,Entry entries[]);   //called implicitly while creating leaf or internal node
// Node createNewLeafNode(int entryCount,Data dataEntries[]);
// Node createNewInternalNode(int entryCount,Node nodeEntries[]);
// bool defineMBR(Node node);
// int nodeLevel(Node node);
// int nodeHeight(Node node);
// bool isRoot(Node node);
/*Function declarations above this*/


/*All structure definitions above this*/

RTree createNewRTree()
{
    RTree rtree = (RTree) malloc(sizeof(struct rTree));
    rtree->root = NULL;
    return rtree;
}

bool isEmpty(RTree rtree)
{
    if (rtree->root == NULL)
        return true;
    return false;
}

Data createDataItem(float coordinates[dim],char* tupleIdentifier)
{
    Data data = (Data) malloc(sizeof(struct data));

    for(int i=0;i<dim;i++)
    {
        data->I[0][i] = data->I[1][i] = data->coordinates[i] = coordinates[i];
    }

    data->tupleIdentifier = (char*)malloc(sizeof(tupleIdentifier));
    if(tupleIdentifier !=NULL)
        strcpy(data->tupleIdentifier,tupleIdentifier);
    else
        strcpy(data->tupleIdentifier,"");

    return data;
}

Node createNewNode(NodeType type,int entryCount,Entry entries[])    //called implicitly while creating leaf or internal node
{
    if(entryCount>M || entryCount<0)
        return NULL;

    Node node = (Node) malloc(sizeof(struct node));
    node->isLeaf = !type;
    node->entryCount = entryCount;

    for(int i=0;i<entryCount;i++)
    {
        node->entries[i] = entries[i];
    }

    for(int i=entryCount;i<M+1;i++)
    {
        node->entries[i] = NULL;
    }
    
    defineMBR(node);
    return node;    
}

Node createNewLeafNode(int entryCount, Data dataEntries[])
{
    Node node = createNewNode(LEAF,entryCount,(Entry*) dataEntries);
    return node;
}

Node createNewInternalNode(int entryCount,Node nodeEntries[])
{
    Node node = createNewNode(INTERNAL,entryCount,(Entry*) nodeEntries);
    if(node == NULL)
        return node;
    for(int i=0;i<entryCount;i++)
    {
        ((Node)node->entries[i])->parent = node;
    }

    return node;
}

bool defineMBR(Node node)
{
    if(node->entryCount <= 0)
        return false;                                                  //false is returned if node has no entries

    for(int i=0;i<dim;i++)
    {
        node->I[0][i] = node->entries[0]->I[0][i];
        node->I[1][i] = node->entries[0]->I[1][i];
    }
    for(int i=0;i<dim;i++)
    {
        for(int j=0;j<node->entryCount;j++)
        {
            if(node->I[0][i] > node->entries[j]->I[0][i])
                node->I[0][i] = node->entries[j]->I[0][i];
            if(node->I[1][i] < node->entries[j]->I[1][i])
                node->I[1][i] = node->entries[j]->I[1][i];
        }
    }
    return true;
}

int nodeLevel(Node node)
{
    int level = 0;
    while(! node->isLeaf)
    {
        level++;
        node = (Node)node->entries[0];
    }
    return level;
}

int nodeHeight(Node node)
{
    int height = 0;
    while(!isRoot(node))
    {
        height++;
        node = node->parent;
    }
    return height;
}

bool isRoot(Node node)
{
    if(node->parent == NULL)
    {
        return true;
    }
    return false;
}

/*R-Tree ADT and basic functions defined above*/

/*
//For debugging purposes://
int main()
{
    printf("Hello\n");
    float coordinates[dim] = {1,0};
    float coordinates2[dim] = {2,3};
    float coordinates3[dim] = {1,9};
    float coordinates4[dim] = {-1,8.9};
    Data item = createDataItem(coordinates,"IAmDaBomb");
    Data item2 = createDataItem(coordinates2,"MeDaBomb");
    Data item3 = createDataItem(coordinates3,"BombaBombaBombBomb");
    Data item4 = createDataItem(coordinates4,"YouDaBomb");

    Data items[] = {item,item2};
    Data items2[] = {item3,item4};
    Node leaf = createNewLeafNode(sizeof(items)/sizeof(items[0]),items);
    Node leaf2 = createNewLeafNode(sizeof(items2)/sizeof(items2[0]),items2);

    Node arrayOfLeaves[] = {leaf,leaf2};
    Node internal = createNewInternalNode(sizeof(arrayOfLeaves)/sizeof(arrayOfLeaves[0]),arrayOfLeaves);

    RTree rtree = createNewRTree();
    rtree->root = internal;
    for(int i=0;i<dim;i++)
    {
        printf("%f %f\n",rtree->root->I[0][i],rtree->root->I[1][i]);
    }
    return 0;
}
*/