#include "RTree.c"

typedef struct linkedList* LinkedList;
typedef struct linkedNode* LinkedNode;
/*All typedefs above this*/

RTree InsertNewDataEntry(float coordinates[dim],char* tupleIdentifier,RTree rtree);
Node ChooseLeaf(Data dataEntry,RTree rtree);
RTree AdjustTree(Node node1, Node node2);                               //node2 can be null if original node was not split
Node CBSSplitNode(Node node);                                           //node that is going to be split will TEMPORARILY have M+1 entries

LinkedList createNewLinkedList();
LinkedList addToLinkedList(Data data, LinkedList list);

LinkedList searchRecursive(Node node, float S[2][dim],LinkedList list);
LinkedList search(RTree rtree, float S[2][dim]);

bool overlaps(float I[2][dim],float S[2][dim]);
/*Function declarations above this*/

struct linkedList
{
    LinkedNode head;
    int count;
};

struct linkedNode
{
    Data data;
    LinkedNode next;
};
/*All structure definitions above this*/

RTree InsertNewDataEntry(float coordinates[dim],char* tupleIdentifier,RTree rtree)
{
    Data dataEntry = createDataItem(coordinates,tupleIdentifier);
    if(isEmpty(rtree))
    {
        Data array[1] = {dataEntry};
        createNewLeafNode(1,array);
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

    AdjustTree(node,nodeNewOnSplit);
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

RTree AdjustTree(Node node1, Node node2)                               //node2 can be null if original node was not split
{
    printf("Incomplete\n");
    return NULL;
}

Node CBSSplitNode(Node node)                                           //node that is going to be split will TEMPORARILY have M+1 entries
{
    printf("Incomplete\n");
    return NULL;
}

LinkedList createNewLinkedList()
{
    LinkedList list = (LinkedList) malloc(sizeof(struct linkedList));
    list->count = 0;
    list->head = NULL;
    return list;
}

LinkedList addToLinkedList(Data data, LinkedList list)
{
    LinkedNode linkedNode = (LinkedNode) malloc(sizeof(struct linkedNode));
    linkedNode->data = data;
    linkedNode->next = NULL;

    if(list->count == 0)
    {
        list->head = linkedNode;
    }
    else
    {
        LinkedNode temp = list->head;
        list->head = linkedNode;
        linkedNode->next = temp;
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
    
    printf("hello working world\n");
    return 0;
}
*/