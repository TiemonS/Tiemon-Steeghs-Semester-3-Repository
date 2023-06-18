#include <stdio.h>
#include <stdlib.h>

#include "testcase.h"
#include "parsing.h"

int main() 
{
    TestCase* testCases;
    int amountOfTestcases;
    testCases = parseData(&amountOfTestcases);

    // Example usage: Print the parsed data
    for (int t = 0; t < amountOfTestcases; t++) {
        printf("\nTest Case %d:\n", t + 1);
        printf("N = %d, M = %d\n", testCases[t].N, testCases[t].M);
        printf("Connections:\n");
        for (int i = 0; i < testCases[t].M; i++) {
            printf("%d %d\n", testCases[t].connections[i][0], testCases[t].connections[i][1]);
        }
    }
    
    //het bevrijden van het gealloceerde geheugen
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
    

    return 0;
}
