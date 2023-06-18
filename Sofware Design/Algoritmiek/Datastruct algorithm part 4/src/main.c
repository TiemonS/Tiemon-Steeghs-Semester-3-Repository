#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "testcase.h"
#include "parsing.h"

#define MAX_NODES 10000

void PrintData(TestCase* testCases, int amountOfTestcases);

int bfsShortestPath(TestCase* testCase, int startNode, int endNode) {
    int numNodes = testCase->N;

    // Create a visited array to keep track of visited nodes
    bool* visited = (bool*)malloc((numNodes + 1) * sizeof(bool));
    for (int i = 1; i <= numNodes; i++) {
        visited[i] = false;
    }

    // Create a queue for BFS traversal
    int* queue = (int*)malloc((numNodes + 1) * sizeof(int));
    int front = 0, rear = 0;

    // Mark the start node as visited and enqueue it
    visited[startNode] = true;
    queue[rear++] = startNode;

    // Create a distance array to keep track of distances from the start node
    int* distance = (int*)malloc((numNodes + 1) * sizeof(int));
    for (int i = 1; i <= numNodes; i++) {
        distance[i] = 0;
    }

    // Perform BFS
    while (front != rear) {
        // Dequeue a node from the queue
        int currentNode = queue[front++];
        
        // Check if the end node is reached
        if (currentNode == endNode) {
            break;
        }
        
        // Traverse the connected nodes
        for (int i = 0; i < testCase->M; i++) {
            int node1 = testCase->connections[i][0];
            int node2 = testCase->connections[i][1];
            
            if (node1 == currentNode && !visited[node2]) {
                // Mark the node as visited
                visited[node2] = true;
                
                // Enqueue the node
                queue[rear++] = node2;
                
                // Update the distance
                distance[node2] = distance[currentNode] + 1;
            }
            
            if (node2 == currentNode && !visited[node1]) {
                // Mark the node as visited
                visited[node1] = true;
                
                // Enqueue the node
                queue[rear++] = node1;
                
                // Update the distance
                distance[node1] = distance[currentNode] + 1;
            }
        }
    }

    // Free the allocated memory
    free(visited);
    free(queue);

    int ShortestPath = distance[endNode];

    free(distance);
    
    // Return the shortest path distance to the end node
    return ShortestPath;
}

int main() 
{
    TestCase* testCases;
    int amountOfTestcases;
    testCases = parseData(&amountOfTestcases);
    
    //eerst de data uitprinten
    //PrintData(testCases, amountOfTestcases);

    //dan het bfs algoritme toepassen om de korste weg te vinden voor alle testcases
    for (size_t i = 0; i < amountOfTestcases; i++)
    {
        int shortestPath = bfsShortestPath(&testCases[i], 1, testCases[i].N);
        printf("Shortest path: %d\n", shortestPath);
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

void PrintData(TestCase* testCases, int amountOfTestcases) 
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

//Commando format:
//valgrind --leak-check=full --show-leak-kinds=all -s  ./output/main <testdata/in3_1