#include <stdio.h>
#include <stdlib.h>
// #include "RTree.h"
#include "Helper.h"

void printNode(Node node, int depth)
{
    char *indent = (char *)malloc(sizeof(char) * (depth + 1));
    for (int i = 0; i < depth; i++)
    {
        indent[i] = '\t';
    }
    if(node->isLeaf)
    {
        printf("%sLeaf Node\n", indent);
        // printf("Entry Count: %d\n",node->entryCount);
        for(int i=0;i<node->entryCount;i++)
        {
            // printf("Data Entry %d:  ",i);
            // printf("Coordinates: ");
            printf("%s%d. (", indent, i+1);
            for(int j=0;j<dim;j++)
            {
                printf("%06ld",(long)(((Data)(node->entries[i]))->coordinates[j]));
                if(j!=dim-1)
                    printf(" ");
            }
            printf(")\n");
            // printf("Tuple Identifier: %s  ",((Data)(node->entries[i]))->tupleIdentifier);
        }
    }
    else
    {
        // Node *nodes = (Node *)malloc(sizeof(Node)*node->entryCount);
        printf("%sInternal Node\n", indent);
        // printf("Entry Count: %d\n",node->entryCount);
        // for (int i = 0; i < node->entryCount; i++)
        // {
        //     nodes[i] = (Node)(node->entries[i]);
        // }
        for(int i=0;i<node->entryCount;i++)
        {
            // printf("Node Entry %d: ",i);
            printf("%s%d. Min Coordinates: (", indent, i+1);
            for(int j=0;j<dim;j++)
            {
                printf("%06ld",(long)(((Node)(node->entries[i]))->I[0][j]));
                if (j!=dim-1)
                    printf(" ");
            }
            printf(") Max Coordinates: (");
            for(int j=0;j<dim;j++)
            {
                printf("%06ld",(long)(((Node)(node->entries[i]))->I[1][j]));
                if (j!=dim-1)
                    printf(" ");
            }
            printf(")\n");
            printNode((Node)(node->entries[i]), depth+1);
        }
    }
}

void printNode2(Node node, int depth)
{
    if(node->isLeaf)
    {
        // printf("Leaf Node\n");
        // printf("Entry Count: %d\n",node->entryCount);
        for(int i=0;i<node->entryCount;i++)
        {
            // printf("Entry %d:\n",i);
            // printf("Coordinates: ");
            for(int j=0;j<dim;j++)
            {
                printf("%ld",(long)((Data)(node->entries[i]))->coordinates[j]);
                if(j!=dim-1)
                    printf(" ");
            }
            printf("\n");
            // printf("Tuple Identifier: %s\n",((Data)(node->entries[i]))->tupleIdentifier);
        }
    }
    else
    {
        // printf("Internal Node\n");
        // printf("Entry Count: %d\n",node->entryCount);
        for(int i=0;i<node->entryCount;i++)
        {
            printNode2((Node)(node->entries[i]), depth+1);
            // printf("Entry %d:\n",i);
            // printf("Coordinates: ");
            for(int j=0;j<dim;j++)
            {
                1;
                // printf("%f ",((Data)(node->entries[i]))->I[0][j]);
            }
            // printf("\n");
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

    printf("Root Node:\n");
    printNode(rtree->root, 1);
    // printNode2(rtree->root, 1);
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

    // long counter = 0;
    char *c;
    while(!feof(fp))
    {
        float coordinates[dim];
        char tupleIdentifier[100];
        fscanf(fp,"%f",&coordinates[0]);
        // if (feof(fp)) break;
        fscanf(fp,"%c",c);
        // if (feof(fp)) break;
        fscanf(fp,"%f",&coordinates[1]);
        if (!feof(fp)) fscanf(fp,"%c",c);
        rtree = InsertNewDataEntry(coordinates, tupleIdentifier, rtree);
        // counter++;
    }
    fclose(fp);
    // printf("Number of entries: %ld\n",counter);
    return rtree;
}

// int main()
// {
//     RTree rTree = import_from_file("large_data.txt");
//     return 0;
// }