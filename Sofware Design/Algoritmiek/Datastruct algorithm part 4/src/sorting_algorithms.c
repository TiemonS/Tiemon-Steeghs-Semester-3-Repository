#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sorting_algorithms.h"
#include "testcase.h"
#include "treedata.h"

int bfsShortestPath(TestCase* testCase, int startNode, int endNode) 
{
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

int findDepth(int nodeIndex, int** nodes, int numNodes) 
{
    int maxDepth = 0;

    for (int i = 0; i < numNodes; i++) {
        if (nodes[i][0] == nodeIndex) {
            int leftChild = nodes[i][1];
            int rightChild = nodes[i][2];

            int leftDepth = 0;
            if (leftChild != 0) {
                leftDepth = findDepth(leftChild, nodes, numNodes);
            }

            int rightDepth = 0;
            if (rightChild != 0) {
                rightDepth = findDepth(rightChild, nodes, numNodes);
            }

            int depth = (leftDepth > rightDepth) ? leftDepth : rightDepth;
            if (depth > maxDepth) {
                maxDepth = depth;
            }
        }
    }

    return 1 + maxDepth;
}

int getTreeDepth(TreeData treeData) 
{
    int** nodes = treeData.nodes;
    int numNodes = treeData.numNodes;

    if (numNodes == 0) {
        return 0; // Empty tree
    }

    int rootIndex = 1; // Assuming the root node index is always 1

    return findDepth(rootIndex, nodes, numNodes);
}