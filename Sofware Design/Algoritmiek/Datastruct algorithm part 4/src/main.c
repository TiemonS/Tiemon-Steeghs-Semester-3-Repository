#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "testcase.h"
#include "parsing.h"
#include "treedata.h"
#include "sorting_algorithms.h"

#define MAX_NODES 10000

void PrintTestCaseData(TestCase* testCases, int amountOfTestcases);
void PrintTreeData(TreeData treeData);


int main(int argc, char* argv[]) 
{
    if (argc != 2)
    {
        printf("Please provide argument to select assignment.\n");
        return -1;
    }
    char selectedAssignment = atoi(argv[1]);
    
    //De input verwerken op basis van wat de gebruiker kiest
    // 1 = testcases met bfs
    // 2 = tree debth met recursion
    if (selectedAssignment == 1)
    {
        TestCase* testCases;
        int amountOfTestcases;
        testCases = parseData(&amountOfTestcases);

        //dan het bfs algoritme toepassen om de korste weg te vinden voor alle testcases
        for (size_t i = 0; i < amountOfTestcases; i++)
        {
            int shortestPath = bfsShortestPath(&testCases[i], 1, testCases[i].N);
            printf("Shortest path: %d\n", shortestPath);
        }    

        //het bevrijden van het gealloceerde geheugen voor de testcases
        if (amountOfTestcases > 0)
        {
            for (int t = 0; t < amountOfTestcases; t++) {
                for (int i = 0; i < testCases[t].M; i++) {
                    free(testCases[t].connections[i]);
                }
                free(testCases[t].connections);
            }
            free(testCases);
        }
    }
    else if(selectedAssignment == 2) 
    {
        TreeData treeData;
        treeData = parseTreeData();

        // de treedepth berekeken met de verkregen data
        int treedepth = getTreeDepth(treeData);
        printf("Tree depth:%d\n", treedepth);

        // Het bevrijden van geheugen voor de treedata
        for (int i = 0; i < treeData.numNodes; i++) {
            free(treeData.nodes[i]);
        }
        free(treeData.nodes);
    }
   
    //eerst de data uitprinten
    //PrintData(testCases, amountOfTestcases);
    //PrintTreeData(treeData);
    return 0;
}

void PrintTestCaseData(TestCase* testCases, int amountOfTestcases) 
{
    for (int t = 0; t < amountOfTestcases; t++) {
        printf("\nTest Case %d:\n", t + 1);
        printf("N = %d, M = %d\n", testCases[t].N, testCases[t].M);
        printf("Connections:\n");
        for (int i = 0; i < testCases[t].M; i++) {
            printf("%d %d\n", testCases[t].connections[i][0], testCases[t].connections[i][1]);
        }
    }
}

void PrintTreeData(TreeData treeData) {
    printf("Number of tree nodes: %d\n", treeData.numNodes);
    printf("Nodes:\n");
    for (int i = 0; i < treeData.numNodes; i++) {
        printf("%d %d %d\n", treeData.nodes[i][0], treeData.nodes[i][1], treeData.nodes[i][2]);
    }
}

//Commando format:
//valgrind --leak-check=full --show-leak-kinds=all -s  ./output/main <testdata/in3_1