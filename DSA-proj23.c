#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <limits.h>
#include <float.h>

#define dim 2               //number of dimensions = 2
#define M 4
#define m 2

typedef struct rTree* RTree;
typedef struct node* Node;
typedef struct entry* Entry;
typedef struct data* Data;
typedef enum nodeType NodeType;

typedef struct linkedList* LinkedList;
typedef struct linkedNode* LinkedNode;

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
    //Data_Contents contents;
};

struct rTree
{
    Node root;
};
enum nodeType{LEAF = 0,INTERNAL = 1};

struct linkedList
{
    LinkedNode start;
    LinkedNode end;
    int count;
};

struct linkedNode
{
    Data data;
    LinkedNode next;
};

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

/*All enums defined above this*/

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

float calculateArea(Entry E1);
float calculateCombinedArea(Entry E1, Entry E2);

bool updateMBR(Node parent,Entry newChild);

RTree InsertNewDataEntry(float coordinates[dim],char* tupleIdentifier,RTree rtree);
Node ChooseLeaf(Data dataEntry,RTree rtree);
bool AdjustTree(RTree rtree,Node node1, Node node2);                                //node2 is null if original node was not split

Node CBSSplitNode(Node node);                                           //node that is going to be split will TEMPORARILY have M+1 entries
void calcCovRect(float answer[dim],Entry entry);
int calcGroup(float objRect[dim],float covRect[dim]);
int chooseSplitDimension(Entry C[1<<dim][M+1],int countC[1<<dim]);
float calculateOverlap(Node node1, Node node2);   //calculates the extent of overlap between two nodes
int objectIndexNearSplitDim(Node node,float covRect[dim],int splitDim);

LinkedList createNewLinkedList();
LinkedList addToLinkedList(Data data, LinkedList list);

LinkedList searchRecursive(Node node, float S[2][dim],LinkedList list);
LinkedList search(RTree rtree, float S[2][dim]);

bool overlaps(float I[2][dim],float S[2][dim]);

void printNode(Node node, int depth);
void printRTree(RTree rtree);
RTree import_from_file(char *filename);

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

float calculateArea(Entry E1)
{
    float area = 1;
    for(int i=0;i<dim;i++)
    {
        float max,min;
        min = E1->I[0][i];
        max = E1->I[1][i];
        area *= (max-min);
    }

    return area;
}

float calculateCombinedArea(Entry E1, Entry E2)
{
    float area = 1;
    for(int i=0;i<dim;i++)
    {
        float max,min;
        min = (E1->I[0][i] < E2->I[0][i] ? E1->I[0][i] : E2->I[0][i]);
        max = (E1->I[1][i] > E2->I[1][i] ? E1->I[1][i] : E2->I[1][i]);
        area *= (max-min);
    }

    return area;
}

bool updateMBR(Node parent,Entry newEntry)
{
    for(int i=0;i<dim;i++)
    {
        if(parent->I[0][i] > newEntry->I[0][i])
            parent->I[0][i] = newEntry->I[0][i];
        if(parent->I[1][i] < newEntry->I[1][i])
            parent->I[1][i] = newEntry->I[1][i];
    }
    return true;
}

RTree InsertNewDataEntry(float coordinates[dim],char* tupleIdentifier,RTree rtree)
{
    Data dataEntry = createDataItem(coordinates,tupleIdentifier);
    if(isEmpty(rtree))
    {
        Data array[1] = {dataEntry};
        rtree->root = createNewLeafNode(1,array);
        return rtree;
    }

    Node node = ChooseLeaf(dataEntry,rtree);
    node->entries[node->entryCount] = (Entry)dataEntry;
    node->entryCount++;
    Node nodeNewOnSplit = NULL;                                     //will become non-NULL if there is a split going to happen

    if(node->entryCount > M)                                        //greater than M can only imply M+1,as the node will be split when that happens
    {
        nodeNewOnSplit = CBSSplitNode(node);                        //returns the new node (the second one) that is created on splitting
    }
    else
    {
        updateMBR(node,(Entry)dataEntry);
    }
    AdjustTree(rtree,node,nodeNewOnSplit);                                //updateMBR of ancestors will happen in AdjustTree
    return rtree;
}

Node ChooseLeaf(Data dataEntry,RTree rtree)
{
    Node node = rtree->root;
    while(! node->isLeaf)
    {
        float minEnlargement = calculateCombinedArea((Entry)node->entries[0],(Entry)dataEntry) - calculateArea((Entry)node->entries[0]);
        int chosenChildIndex = 0;

        for(int i=1;i<node->entryCount;i++)
        {
            float enlargement = calculateCombinedArea((Entry)node->entries[i],(Entry)dataEntry) - calculateArea((Entry)node->entries[i]);
            if(enlargement < minEnlargement ||                                                                                                  //looking for child that would lead to minimum increase in area
            (enlargement==minEnlargement && calculateArea((Entry)node->entries[i]) < calculateArea((Entry)node->entries[chosenChildIndex])))    //in case of tie, choose the child with smaller area
            {
                chosenChildIndex = i;
                minEnlargement = enlargement;
            }
        }
        
        node = (Node)node->entries[chosenChildIndex];
    }

    return node;
}

bool AdjustTree(RTree rtree, Node node1, Node node2)                                //node2 is null if original node was not split
{
    while(!isRoot(node1))
    {
        Node parent = node1->parent;
        Node parentNewOnSplit = NULL;
        updateMBR(parent,(Entry)node1);
        if(node2 != NULL)
        {
            parent->entries[parent->entryCount] = (Entry)node2;
            parent->entryCount++;
            updateMBR(parent,(Entry)node2);

            if(parent->entryCount > M)
            {
                parentNewOnSplit = CBSSplitNode(parent);
            }
        }
        node1 = parent;
        node2 = parentNewOnSplit;
    }
    if(node2 != NULL)
    {
        Node children[2] = {node1,node2};
        Node newRoot = createNewInternalNode(2,children);
        rtree->root = newRoot;
    }
    return true;
}

Node CBSSplitNode(Node node)                                            //node that is going to be split will TEMPORARILY have M+1 entries
{                                                                       //this function will also defineMBR of the split nodes
    Entry C[1<<dim][M+1];                                               //using the concept of bit-masking to implement n-dimensional CBS Algorithm
    int countC[1<<dim] = {};                                            // = {} initialises all values in int array with 0

    float covRect[dim];
    calcCovRect(covRect,(Entry)node);
    for(int i=0;i<node->entryCount;i++)
    {
        float objRect[dim];
        calcCovRect(objRect,node->entries[i]);
        int group = calcGroup(objRect,covRect);
        C[group][countC[group]] = node->entries[i];
        countC[group]++;
    }

    int splitDim = chooseSplitDimension(C,countC);

    Entry _e[] = {NULL}; 
    Node temp1 = createNewNode(! node->isLeaf,0,_e);
    Node temp2 = createNewNode(! node->isLeaf,0,_e);

    for(int i=0;i<(1<<dim);i++)
    {
        if(i&(1<<splitDim))
        {
            for(int j=0;j<countC[i];j++)
            {
                temp2->entries[temp2->entryCount] = C[i][j];
                temp2->entryCount++;
            }
        }
        else
        {
            for(int j=0;j<countC[i];j++)
            {
                temp1->entries[temp1->entryCount] = C[i][j];
                temp1->entryCount++;
            }
        }
    }
    if(temp1->entryCount<temp2->entryCount)
    {
            Node swap = temp1;
            temp1 = temp2;
            temp2 = swap;
    }                                           //after this, temp1 will have more or equal number of entries as temp2

    while(temp2->entryCount<m)
    {
        int index1 = objectIndexNearSplitDim(temp1,covRect,splitDim);
        Entry move = temp1->entries[index1];

        temp1->entries[index1] = temp1->entries[temp1->entryCount - 1];
        temp1->entries[temp1->entryCount - 1] = NULL;
        temp2->entries[temp2->entryCount] = move;

        temp1->entryCount--;
        temp2->entryCount++;
    }

    if(! temp2->isLeaf)
    {
        for(int i=0;i<temp2->entryCount;i++)
        {
            ((Node)temp2->entries[i])->parent = temp2;
        }
    }

    temp2->parent = temp1->parent = node->parent;
    defineMBR(temp1);
    defineMBR(temp2);
    
    *node = *temp1;
    free(temp1);
    
    return temp2;
}


int chooseSplitDimension(Entry C[1<<dim][M+1],int countC[1<<dim])
{
    int splitDim=0;
    int minDif=INT_MAX;
    float overlap=FLT_MAX;
    float area=FLT_MAX;
    for (int k=0;k<dim;k++){
        int kDif;
        Entry _e[] = {NULL}; 
        Node temp1 = createNewNode(0,0,_e);
        Node temp2 = createNewNode(0,0,_e);
        for(int i=0;i<(1<<dim);i++){
            if(i&(1<<k))
            {
                for(int j=0;j<countC[i];j++)
                {
                    temp2->entries[temp2->entryCount] = C[i][j];
                    temp2->entryCount++;
                }
            }
            else
            {
                for(int j=0;j<countC[i];j++)
                {
                    temp1->entries[temp1->entryCount] = C[i][j];
                    temp1->entryCount++;
                }
            }
        }
        defineMBR(temp1);
        defineMBR(temp2);
        kDif=abs(temp1->entryCount - temp2->entryCount);
        float kOverlap=calculateOverlap(temp1,temp2);//check overlap by creating a new function that checks if obj is part of
        float kArea=calculateArea((Entry)temp1)+calculateArea((Entry)temp2);
        if(kDif<minDif){
            splitDim=k;
            minDif=kDif;
            overlap=kOverlap;
            area=kArea;
        }
        else if((kDif==minDif && dim!=2) || (countC[1]==countC[2] && dim==2)){  //in the CBS research paper, they have not checked tie breaker condition for countC[0] equal to countC[3], so we have explicitly avoided that check for dim = 2, though in all fairness, they should check that condition for tie breaker as well
            if(kOverlap<overlap){
                splitDim=k;
                minDif=kDif;
                overlap=kOverlap;
                area=kArea;
            }
            else if(kOverlap==overlap){
                if(area>kArea){
                    splitDim=k;
                    minDif=kDif;
                    overlap=kOverlap;
                    area=kArea;                
                }
            }
        }
        free(temp1);
        free(temp2); 
    }
    return splitDim;
}

float calculateOverlap(Node node1, Node node2){   //calculates the extent of overlap between two nodes
    float overlap=1;
    for (int i=0;i<dim;i++){
        float max_min=node1->I[0][i]>node2->I[0][i]? node1->I[0][i]:node2->I[0][i];
        float min_max=node1->I[1][i]<node2->I[1][i]? node1->I[1][i]:node2->I[1][i];
        overlap *= min_max - max_min;
        if(overlap<=0){
            overlap=0;
            break;
        }
    }
    return overlap;
}

int objectIndexNearSplitDim(Node node,float covRect[dim],int splitDim)
{
    int index=-1;
    float dist=FLT_MAX;
    for(int i=0;i<node->entryCount;i++){
        float iDist=abs((node->entries[i]->I[0][splitDim]+node->entries[i]->I[1][splitDim])/2 -covRect[splitDim]);
        if (iDist<dist){
            index=i;
            dist=iDist;
        }
    }
    return index;
}

void calcCovRect(float covRect[dim],Entry entry)
{
    for(int i=0;i<dim;i++)
    {
        covRect[i] = (entry->I[0][i] + entry->I[1][i])/2;
    }
}

int calcGroup(float objRect[dim],float covRect[dim])                    //calculating group number using the concept of bit-masking
{
    int group = 0;
    for(int i=0;i<dim;i++)
    {
        if(objRect[i]>covRect[i])
        {
            group += 1<<i;
        }
    }
    return group;
}


LinkedList createNewLinkedList()
{
    LinkedList list = (LinkedList) malloc(sizeof(struct linkedList));
    list->count = 0;
    list->start = NULL;
    list->end = NULL;
    return list;
}

LinkedList addToLinkedList(Data data, LinkedList list)
{
    LinkedNode linkedNode = (LinkedNode) malloc(sizeof(struct linkedNode));
    linkedNode->data = data;
    linkedNode->next = NULL;

    if(list->count == 0)
    {
        list->end = list->start = linkedNode;
    }
    else
    {
        list->end->next = linkedNode;
        list->end = linkedNode;
    }
    list->count++;
    return list;
}

LinkedList searchRecursive(Node node, float S[2][dim],LinkedList list)
{
    for(int i=0;i<node->entryCount;i++)
    {
        if(overlaps(node->entries[i]->I,S))
        {
            if(! node->isLeaf)
                searchRecursive((Node)node->entries[i],S,list);
            else
                addToLinkedList((Data)node->entries[i],list);
        }
    }
    return list;                                    //return list after recursive search
}

LinkedList search(RTree rtree, float S[2][dim])       //returns a LinkedList of LinkedNodes containing the leaf nodes of our tree that overlaps with S
{
    LinkedList list = createNewLinkedList();        //empty list created with count as 0

    if(isEmpty(rtree))
        return list;                                //empty list returned if R-tree is empty
    Node node = rtree->root;                        
    if(!overlaps(node->I,S))
        return list;                                //empty list returned if Root does not overlap with S
    return searchRecursive(node,S,list);            //searchRecursive
}

bool overlaps(float I[2][dim],float S[2][dim])
{
    bool isOverlap = true;
    for(int i=0;i<dim;i++)
    {
        if(I[1][i] < S[0][i] || I[0][i] > S[1][i])  //if the maximum of I's "i"th dimension is before the minimum of S's "i"th dimension or vice versa 
        {
            isOverlap = false;                      //then they do not overlap
            break;
        }
    }
    return isOverlap;                               //else I and S overlap
}

void printNode(Node node, int depth)
{
    char *indent = (char *) malloc(sizeof(char) * (depth + 1));
    for (int i = 0; i < depth; i++)
    {
        indent[i] = '\t';
    }
    indent[depth] = '\0';
    if(node->isLeaf)
    {
        for(int i=0;i<node->entryCount;i++)
        {
            printf("%s%d. (", indent, i+1);
            for(int j=0;j<dim;j++)
            {
                printf("%ld",(long)(((Data)(node->entries[i]))->coordinates[j]));
                if(j!=dim-1)
                    printf(",");
            }
            printf(")\n");
        }
    }
    else
    {
        for(int i=0;i<node->entryCount;i++)
        {
            printf("%s%d. (%s) Min Coordinates: (", indent, i+1, ((Node)(node->entries[i]))->isLeaf ? "Leaf Node" : "Internal Node");
            for(int j=0;j<dim;j++)
            {
                printf("%ld",(long)(((Node)(node->entries[i]))->I[0][j]));
                if (j!=dim-1)
                    printf(",");
            }
            printf(") Max Coordinates: (");
            for(int j=0;j<dim;j++)
            {
                printf("%ld",(long)(((Node)(node->entries[i]))->I[1][j]));
                if (j!=dim-1)
                    printf(",");
            }
            printf(")\n");
            printNode((Node)(node->entries[i]), depth+1);
        }
    }
}

void printRTree(RTree rtree)
{
    if(rtree == NULL)
    {
        printf("Empty RTree\n");
        return;
    }

    if(rtree->root == NULL)
    {
        printf("Empty RTree\n");
        return;
    }

    printf("(Root Node) ");
    printf("Min Coordinates: (");
    for(int j=0;j<dim;j++)
    {
        printf("%ld",(long)(((Node)(rtree->root))->I[0][j]));
        if (j!=dim-1)
            printf(",");
    }
    printf(") Max Coordinates: (");
    for(int j=0;j<dim;j++)
    {
        printf("%ld",(long)(((Node)(rtree->root))->I[1][j]));
        if (j!=dim-1)
            printf(",");
    }
    printf(")\n");
    
    printNode(rtree->root, 1);
}


RTree import_from_file(char *filename)
{
    RTree rtree = createNewRTree();
    FILE *fp = fopen(filename,"r");
    if(fp == NULL)
    {
        printf("File not found\n");
        return NULL;
    }

    char *c;
    while(!feof(fp))
    {
        float coordinates[dim];
        char tupleIdentifier[100];
        fscanf(fp,"%f",&coordinates[0]);
        for(int i=1;i<dim;i++)
        {
            fscanf(fp,"%c",c);
            fscanf(fp,"%f",&coordinates[i]);
        }
        if (!feof(fp)) fscanf(fp,"%c",c);
        rtree = InsertNewDataEntry(coordinates, tupleIdentifier, rtree);
    }
    fclose(fp);
    return rtree;
}

int main()
{

    // Groups are numbered via bit-masking. For 2 dimensions, the numbers would be like this:
    // ===========
    // |2   |   3|
    // |    |    |
    // |---------|
    // |    |    |  
    // |0   |   1|
    // ===========

    // RTree rTree = import_from_file("large_data.txt");
    RTree rTree = import_from_file("data.txt");
    printRTree(rTree);
    return 0;
}