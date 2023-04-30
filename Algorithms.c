// #include "RTree.h"
#include <limits.h> //to be removed
#include <float.h>   //to be removed
#include <stdio.h>  //to be removed
#include <stdlib.h> //to be removed
#include "Algorithms.h"
/*All structure definitions above this*/

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
        else if(kDif==minDif){
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

/*
//For debugging purposes://
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

    RTree rtree = createNewRTree();

    float coordinates1[dim] = {1,9};
    float coordinates2[dim] = {2,20};
    float coordinates3[dim] = {2,19};
    float coordinates4[dim] = {3,20};
    float coordinates5[dim] = {2,10};
    float coordinates6[dim] = {8,5};
    float coordinates7[dim] = {4,5};
    float coordinates8[dim] = {3,4};
    float coordinates9[dim] = {3,5};
    float coordinates10[dim] = {2,4};
    float coordinates11[dim] = {2,5};
    float coordinates12[dim] = {8,15};
    float coordinates13[dim] = {8,14};
    float coordinates14[dim] = {7,15};
    float coordinates15[dim] = {9,14};
    float coordinates16[dim] = {9,15};
    float coordinates17[dim] = {9,16};
    float coordinates18[dim] = {9,17};
    float coordinates19[dim] = {12,17};
    float coordinates20[dim] = {11,18};
    float coordinates21[dim] = {1,20};

    
    InsertNewDataEntry(coordinates1,NULL,rtree);
    InsertNewDataEntry(coordinates2,NULL,rtree);
    InsertNewDataEntry(coordinates3,NULL,rtree);
    InsertNewDataEntry(coordinates4,NULL,rtree);
    InsertNewDataEntry(coordinates5,NULL,rtree);
    InsertNewDataEntry(coordinates6,NULL,rtree);
    InsertNewDataEntry(coordinates7,NULL,rtree);
    InsertNewDataEntry(coordinates8,NULL,rtree);
    InsertNewDataEntry(coordinates9,NULL,rtree);
    InsertNewDataEntry(coordinates10,NULL,rtree);
    InsertNewDataEntry(coordinates11,NULL,rtree);
    InsertNewDataEntry(coordinates12,NULL,rtree);
    InsertNewDataEntry(coordinates13,NULL,rtree);
    InsertNewDataEntry(coordinates14,NULL,rtree);
    InsertNewDataEntry(coordinates15,NULL,rtree);
    InsertNewDataEntry(coordinates16,NULL,rtree);
    InsertNewDataEntry(coordinates17,NULL,rtree);
    InsertNewDataEntry(coordinates18,NULL,rtree);
    InsertNewDataEntry(coordinates19,NULL,rtree);
    InsertNewDataEntry(coordinates20,NULL,rtree);
    InsertNewDataEntry(coordinates21,NULL,rtree);
    
    
    // printf("hello working world\n");
    // float searchQuery[2][dim] = {6,5,6,5};
    // printf("Search tupleIdentifier at (6,5) : %s\n",search(rtree,searchQuery)->start->data->tupleIdentifier);

    // float S[2][dim] = {{1,0},{5,6}};
    // LinkedList list = search(rtree,S);

    // printf("\nLet's print the search results' list(data contained from x=1 to 5 and y=0 to 6):\n");
    // LinkedNode linkedNode = list->start;
    // for(int i=0;i<list->count;i++)
    // {
    //     printf("%s(%f %f)\n",linkedNode->data->tupleIdentifier,linkedNode->data->coordinates[0],linkedNode->data->coordinates[1]);
    //     linkedNode = linkedNode->next;
    // }
    // Node node = (Node)rtree->root->entries[2];
    // node = (Node)node->entries[0];

    // printf("Node Height = %d\n",nodeHeight(node));
    // printf("Node level = %d\n",nodeLevel(node));
    // printf("\nrtree MBR is: (%f %f) to (%f %f)\n",rtree->root->I[0][0],rtree->root->I[0][1],rtree->root->I[1][0],rtree->root->I[1][1]);
    
    // float objRect[dim];
    // calcCovRect(objRect,(Entry)coordinates);

    // float covRect[dim];
    // calcCovRect(covRect,(Entry)rtree->root);
    
    // printf("Group number of (1,0): %d\n",calcGroup(coordinates,covRect));
    // printf("Group number of (2,3): %d\n",calcGroup(coordinates2,covRect));
    // printf("Group number of (1,8): %d\n",calcGroup(coordinates3,covRect));
    // printf("Group number of (-1.8.9): %d\n",calcGroup(coordinates4,covRect));
    // printf("Group number of (6,5): %d\n\n",calcGroup(coordinates5,covRect));
    
    return 0;
}
*/