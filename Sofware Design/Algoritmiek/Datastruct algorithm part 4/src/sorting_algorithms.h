#include "testcase.h"
#include "treedata.h"

#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

int bfsShortestPath(TestCase* testCase, int startNode, int endNode);

int findDepth(int nodeIndex, int** nodes, int numNodes); 

int getTreeDepth(TreeData treeData); 

#endif 