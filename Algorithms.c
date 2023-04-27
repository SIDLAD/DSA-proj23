#include "RTree.c"

typedef struct linkedList* LinkedList;
typedef struct linkedNode* LinkedNode;
/*All typedefs above this*/

RTree InsertNewDataEntry(Data dataEntry,RTree rtree);
Node ChooseLeaf(Data dataEntry,RTree rtree);
RTree AdjustTree(Node node1, Node node2);                               //node2 can be null if original node was not split
bool CBSSplitNode(Node node);                                           //node that is going to be split will TEMPORARILY have M+1 entries

LinkedList createNewLinkedList();
LinkedList addToLinkedList(Data data, LinkedList list);

LinkedList searchRecursive(Node node, int S[2][dim],LinkedList list);
LinkedList search(RTree rtree, int S[2][dim]);

bool overlaps(int I[2][dim],int S[2][dim]);
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

//TODO: working on it right now
RTree InsertNewDataEntry(Data dataEntry,RTree rtree)
{
    // Node node = ChooseLeaf(dataEntry,rtree);
    // if()
    return rtree;
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

LinkedList searchRecursive(Node node, int S[2][dim],LinkedList list)
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

LinkedList search(RTree rtree, int S[2][dim])       //returns a LinkedList of LinkedNodes containing the leaf nodes of our tree that overlaps with S
{
    LinkedList list = createNewLinkedList();        //empty list created with count as 0

    if(isEmpty(rtree))
        return list;                                //empty list returned if R-tree is empty
    Node node = rtree->root;                        
    if(!overlaps(node->I,S))
        return list;                                //empty list returned if Root does not overlap with S
    return searchRecursive(node,S,list);            //searchRecursive
}

bool overlaps(int I[2][dim],int S[2][dim])
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