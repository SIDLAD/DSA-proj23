#include <stdio.h>
#include <stdlib.h>
// #include "RTree.h"
#include "Helper.h"

void printNode(Node node)
{
    if(node->isLeaf)
    {
        printf("Leaf Node\n");
        printf("Entry Count: %d\n",node->entryCount);
        for(int i=0;i<node->entryCount;i++)
        {
            printf("Entry %d:\n",i);
            printf("Coordinates: ");
            for(int j=0;j<dim;j++)
            {
                printf("%f ",((Data)(node->entries[i]))->coordinates[j]);
            }
            printf("\n");
            printf("Tuple Identifier: %s\n",((Data)(node->entries[i]))->tupleIdentifier);
        }
    }
    else
    {
        printf("Internal Node\n");
        printf("Entry Count: %d\n",node->entryCount);
        for(int i=0;i<node->entryCount;i++)
        {
            printf("Entry %d:\n",i);
            printf("Coordinates: ");
            for(int j=0;j<dim;j++)
            {
                printf("%f ",((Data)(node->entries[i]))->I[0][j]);
            }
            printf("\n");
            // printf("Height: %d\n",((Data)(node->entries[i]))->height);
        }
    }
}

void printRTree(RTree rtree) //INCOMPLETE
{
    if(rtree->root == NULL)
    {
        printf("Empty RTree\n");
        return;
    }
    printf("Root Node:\n");
    printNode(rtree->root);
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

    /* for general dimensions
    while(!fp->eof())
    {
        float coordinates[dim];
        char tupleIdentifier[100];
        for(int j=0;j<dim;j++)
        {
            fscanf(fp,"%f",&coordinates[j]);
            scanf("%c");
        }
        // fscanf(fp,"%s",tupleIdentifier);
        Data data = createDataItem(coordinates,tupleIdentifier);
        insertData(rtree,data);
    }*/

    char *c;
    //for 2 dimensions
    while(!feof(fp))
    {
        float coordinates[dim];
        char tupleIdentifier[100];
        fscanf(fp,"%f",&coordinates[0]);
        // if (feof(fp)) break;
        fscanf(fp,"%c",c);
        // if (feof(fp)) break;
        fscanf(fp,"%f",&coordinates[1]);
        if (feof(fp)) break;
        fscanf(fp,"%c",c);
        rtree = InsertNewDataEntry(coordinates, tupleIdentifier, rtree);
    }
    fclose(fp);
    return rtree;
}

// int main()
// {
//     RTree rTree = import_from_file("large_data.txt");
//     return 0;
// }