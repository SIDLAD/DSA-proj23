#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "RTree.h"

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