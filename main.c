/**
 * @contributors:   SIDLAD,
 *                  VedangNadkarni,
 *                  shaipranesh2,
 *                  Aryan-Seth,
 *                  krteyu
*/

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

    // RTree rTree = import_from_file("large_data.txt");
    RTree rTree = import_from_file("data.txt");
    printRTree(rTree);
    return 0;
}