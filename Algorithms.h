#include "RTree.h"

typedef struct linkedList* LinkedList;
typedef struct linkedNode* LinkedNode;
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