#ifndef TESTCASE_H
#define TESTCASE_H


//N = de eind waarde de we moeten bereiken
//M = het aantal connecties wat er is voor deze testcase
typedef struct {
    int N; // Number of nodes
    int M; // Number of connections
    int** connections; // Array of connections
} TestCase;

#endif 
