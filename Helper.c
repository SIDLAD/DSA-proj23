#include <stdio.h>
#include <stdlib.h>
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
        for(int i=0;i<node->entryCount;i++)
        {
            printf("%s%d. (", indent, i+1);
            for(int j=0;j<dim;j++)
            {
                printf("%06ld",(long)(((Data)(node->entries[i]))->coordinates[j]));
                if(j!=dim-1)
                    printf(" ");
            }
            printf(")\n");
        }
    }
    else
    {
        printf("%sInternal Node\n", indent);
        for(int i=0;i<node->entryCount;i++)
        {
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
        fscanf(fp,"%c",c);
        fscanf(fp,"%f",&coordinates[1]);
        if (!feof(fp)) fscanf(fp,"%c",c);
        rtree = InsertNewDataEntry(coordinates, tupleIdentifier, rtree);
    }
    fclose(fp);
    return rtree;
}