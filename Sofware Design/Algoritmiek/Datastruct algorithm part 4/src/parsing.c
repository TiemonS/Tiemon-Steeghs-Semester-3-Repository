#include <stdio.h>
#include <stdlib.h>

#include "parsing.h"
#include "testcase.h"

TestCase* parseData(int* AmountOfTestCases) 
{
    int amountOfTestCases;
    // Read the number of test cases
    if (scanf("%d", &amountOfTestCases) != 1) {
        printf("Error: could not read array size\n");
        exit(1);
    }
    printf("Amount of test cases recieved: %d\n", amountOfTestCases);
    *AmountOfTestCases = amountOfTestCases;

    // Allocate memory for the array of TestCases
    TestCase* testCases = (TestCase*)malloc(amountOfTestCases * sizeof(TestCase));

    for (int t = 0; t < amountOfTestCases; t++) {
        //Lees de waarde van N en M
        scanf("%d %d", &testCases[t].N, &testCases[t].M); 

        // Allocate memory voor de connections array
        testCases[t].connections = (int**)malloc(testCases[t].M * sizeof(int*));

        //alle connections in de array stoppen
        for (int i = 0; i < testCases[t].M; i++) {
            // Allocate memory voor de losse connections
            testCases[t].connections[i] = (int*)malloc(2 * sizeof(int));
            scanf("%d %d", &testCases[t].connections[i][0], &testCases[t].connections[i][1]); // Read the connection X and Y
        }
    }   

    return testCases;
}