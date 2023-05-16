#include "challenge.h"
#include <stdio.h>
#include <stdlib.h>

int FindSmallestNumberThatIsRepeatedKTimes(
            int* array, int size, int k, int* smallestNumber)
{
    int freq[size];
    int i, minNum = INT8_MAX;

    // Initialize frequency array
    for (i = 0; i < size; i++) {
        freq[i] = 0;
    }

    // Count the frequency of each number in the array
    for (i = 0; i < size; i++) {
        freq[array[i]]++;
    }

    // Find the smallest number that is repeated k times
    for (i = 0; i < size; i++) {
        if (freq[array[i]] == k && array[i] < minNum) {
            minNum = array[i];
        }
    }

    if (minNum != INT8_MAX) {
        *smallestNumber = minNum;
        return 0;
    } 
    else 
    {
        return -1;
    }
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_0(
            int* array, int size, int k, int* difference)
{
    // Check if the input is valid
    if (array == NULL || difference == NULL || size <= 0 || k <= 0 || k > size) {
        printf("Invalid input!\n");
        return;
    }

    // Sort the array in ascending order using bubble sort
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    // Calculate the sum of the first k elements and the last k elements
    int minSum = 0;
    int maxSum = 0;
    for (int i = 0; i < k; i++) {
        minSum += array[i];
        maxSum += array[size - i - 1];
    }

    // Calculate the difference
    *difference = maxSum - minSum;

    return -1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_1(
            int* array, int size, int k, int* difference)
{
    return -1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_2(
            int* array, int size, int k, int* difference)
{
    return -1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_3(
            int* array, int size, int k, int* difference)
{
    return -1;
}
