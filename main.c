#include <stdio.h>
#include "Helper.h"
// #include "RTree.h"
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

    RTree rTree = import_from_file("large_data.txt");
    // RTree rtree = createNewRTree();

    // float coordinates1[dim] = {1,9};
    // float coordinates2[dim] = {2,20};
    // float coordinates3[dim] = {2,19};
    // float coordinates4[dim] = {3,20};
    // float coordinates5[dim] = {2,10};
    // float coordinates6[dim] = {8,5};
    // float coordinates7[dim] = {4,5};
    // float coordinates8[dim] = {3,4};
    // float coordinates9[dim] = {3,5};
    // float coordinates10[dim] = {2,4};
    // float coordinates11[dim] = {2,5};
    // float coordinates12[dim] = {8,15};
    // float coordinates13[dim] = {8,14};
    // float coordinates14[dim] = {7,15};
    // float coordinates15[dim] = {9,14};
    // float coordinates16[dim] = {9,15};
    // float coordinates17[dim] = {9,16};
    // float coordinates18[dim] = {9,17};
    // float coordinates19[dim] = {12,17};
    // float coordinates20[dim] = {11,18};
    // float coordinates21[dim] = {1,20};

    
    // InsertNewDataEntry(coordinates1,NULL,rtree);
    // InsertNewDataEntry(coordinates2,NULL,rtree);
    // InsertNewDataEntry(coordinates3,NULL,rtree);
    // InsertNewDataEntry(coordinates4,NULL,rtree);
    // InsertNewDataEntry(coordinates5,NULL,rtree);
    // InsertNewDataEntry(coordinates6,NULL,rtree);
    // InsertNewDataEntry(coordinates7,NULL,rtree);
    // InsertNewDataEntry(coordinates8,NULL,rtree);
    // InsertNewDataEntry(coordinates9,NULL,rtree);
    // InsertNewDataEntry(coordinates10,NULL,rtree);
    // InsertNewDataEntry(coordinates11,NULL,rtree);
    // InsertNewDataEntry(coordinates12,NULL,rtree);
    // InsertNewDataEntry(coordinates13,NULL,rtree);
    // InsertNewDataEntry(coordinates14,NULL,rtree);
    // InsertNewDataEntry(coordinates15,NULL,rtree);
    // InsertNewDataEntry(coordinates16,NULL,rtree);
    // InsertNewDataEntry(coordinates17,NULL,rtree);
    // InsertNewDataEntry(coordinates18,NULL,rtree);
    // InsertNewDataEntry(coordinates19,NULL,rtree);
    // InsertNewDataEntry(coordinates20,NULL,rtree);
    // InsertNewDataEntry(coordinates21,NULL,rtree);
    
    /*
    printf("hello working world\n");
    float searchQuery[2][dim] = {6,5,6,5};
    printf("Search tupleIdentifier at (6,5) : %s\n",search(rtree,searchQuery)->start->data->tupleIdentifier);

    float S[2][dim] = {{1,0},{5,6}};
    LinkedList list = search(rtree,S);

    printf("\nLet's print the search results' list(data contained from x=1 to 5 and y=0 to 6):\n");
    LinkedNode linkedNode = list->start;
    for(int i=0;i<list->count;i++)
    {
        printf("%s(%f %f)\n",linkedNode->data->tupleIdentifier,linkedNode->data->coordinates[0],linkedNode->data->coordinates[1]);
        linkedNode = linkedNode->next;
    }
    Node node = (Node)rtree->root->entries[2];
    node = (Node)node->entries[0];

    printf("Node Height = %d\n",nodeHeight(node));
    printf("Node level = %d\n",nodeLevel(node));
    printf("\nrtree MBR is: (%f %f) to (%f %f)\n",rtree->root->I[0][0],rtree->root->I[0][1],rtree->root->I[1][0],rtree->root->I[1][1]);
    
    float objRect[dim];
    calcCovRect(objRect,(Entry)coordinates);

    float covRect[dim];
    calcCovRect(covRect,(Entry)rtree->root);
    
    printf("Group number of (1,0): %d\n",calcGroup(coordinates,covRect));
    printf("Group number of (2,3): %d\n",calcGroup(coordinates2,covRect));
    printf("Group number of (1,8): %d\n",calcGroup(coordinates3,covRect));
    printf("Group number of (-1.8.9): %d\n",calcGroup(coordinates4,covRect));
    printf("Group number of (6,5): %d\n\n",calcGroup(coordinates5,covRect));
    */
    return 0;
}