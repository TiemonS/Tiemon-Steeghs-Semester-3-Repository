#include <stdio.h>
#include <stdlib.h>

#include "parsing.h"
#include "testcase.h"
#include "treedata.h"

TestCase* parseData(int* AmountOfTestCases) 
{
    int amountOfTestCases;
    // Read the number of test cases
    if (scanf("%d", &amountOfTestCases) != 1) {
        printf("Error: could not read number of testcases\n");
        exit(1);
    }
    printf("Amount of test cases recieved: %d\n", amountOfTestCases);
    *AmountOfTestCases = amountOfTestCases;

    // Allocate memory for the array of TestCases
    TestCase* testCases = (TestCase*)malloc(amountOfTestCases * sizeof(TestCase));

    for (int t = 0; t < amountOfTestCases; t++) {
        //Lees de waarde van N en M
        if(scanf("%d %d", &testCases[t].N, &testCases[t].M) != 2) 
        {
            printf("Error: could not read values of N and M\n");
            exit(1);
        } 

        // Allocate memory voor de connections array
        testCases[t].connections = (int**)malloc(testCases[t].M * sizeof(int*));

        //alle connections in de array stoppen
        for (int i = 0; i < testCases[t].M; i++) {
            // Allocate memory voor de losse connections
            testCases[t].connections[i] = (int*)malloc(2 * sizeof(int));
            //het lezen van de losse X en Y waardes van alle connections
            if(scanf("%d %d", &testCases[t].connections[i][0], &testCases[t].connections[i][1]) != 2) 
            {
                printf("Error: could not read X and Y of Connection\n");
                exit(1);
            } 
        }
    }   

    return testCases;
}

TreeData parseTreeData() {
    TreeData treeData;

    // Read the number of tree nodes
    if (scanf("%d", &treeData.numNodes) != 1) {
        printf("Error: could not read number of tree nodes\n");
        exit(1);
    }
    printf("Number of tree nodes received: %d\n", treeData.numNodes);

    // Allocate memory for the nodes array
    treeData.nodes = (int**)malloc(treeData.numNodes * sizeof(int*));

    // Read and store the node values
    for (int i = 0; i < treeData.numNodes; i++) {
        treeData.nodes[i] = (int*)malloc(3 * sizeof(int));
        if(scanf("%d %d %d", &treeData.nodes[i][0], &treeData.nodes[i][1], &treeData.nodes[i][2]) != 3) 
        {
        printf("Error: could not read number of testcases\n");
        exit(1);
        }
    }

    return treeData;
}