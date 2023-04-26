#include "RTree.c"

typedef struct LinkedList* LinkedList;
typedef struct LinkedNode* LinkedNode;
/*All typedefs above this*/

//insert and split algorithm functions will precede search functions practically as you cannot search what you cannot create
LinkedList createNewLinkedList();
LinkedList addLinkedNode(LinkedList list, Node node);
LinkedList searchRecursive(Node node, int S[2][dim],LinkedList list);
LinkedList search(RTree rtree, int S[2][dim]);
bool overlaps(int I[2][dim],int S[2][dim]);
/*Function declarations above this*/

struct LinkedList
{
    LinkedNode head;
    int count;
};

struct LinkedNode
{
    Node node;
    LinkedNode next;
};
/*All structure definitions above this*/

LinkedList createNewLinkedList()
{
    LinkedList list = (LinkedList) malloc(sizeof(struct LinkedList));
    list->count = 0;
    list->head = NULL;
    return list;
}

LinkedList addLinkedNode(LinkedList list, Node node)
{
    LinkedNode linkedNode = (LinkedNode) malloc(sizeof(struct LinkedNode));
    linkedNode->node = node;
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
    if(isInternal(node))
    {
        Internal internal = (Internal) node->E;
        for(int i=0;i<internal->childrenCount;i++)
        {
            Node child = internal->Child[i];
            if(overlaps(child->I,S))                //if child area overlaps with S, then continue searchRecursive on child
                searchRecursive(node,S,list);
        }
    }
    else
    {
        addLinkedNode(list,node);                   //if that child turns out to be a leaf, then add to list
    }
    return list;                                    //return list after recursive search
}

LinkedList search(RTree rtree, int S[2][dim])       //returns a LinkedList of LinkedNodes containing the leaf nodes of our tree that overlap with search area
{
    LinkedList list = createNewLinkedList();        //empty list created with count as 0

    if(isEmpty(rtree))
        return list;
    Node node = rtree->Root;                        //empty list returned if R-tree is empty
    if(!overlaps(node->I,S))
        return list;                                //empty list returned if Root area does not overlap with S
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