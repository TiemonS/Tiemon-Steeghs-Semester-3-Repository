#include "challenge.h"
#include <stdio.h>
#include <stdlib.h>

int FindSmallestNumberThatIsRepeatedKTimes(
            int* array, int size, int k, int* smallestNumber)
{
    if (array == NULL || size <= 0 || k <= 0 || smallestNumber == NULL)
    {
        printf("Invalid input!\n");
        return -1; 
    }
    

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
        return -1;
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

void Merge(int* arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // maak tijdelijke arrays
    int L[n1], R[n2];

    // kopieer data naar tijdelijke arrays L[] en R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // voeg de tijdelijke arrays samen en sorteer ze
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // kopieer de resterende elementen van L[], indien aanwezig
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // kopieer de resterende elementen van R[], indien aanwezig
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void MergeSort(int* arr, int l, int r) {
    if (l < r) {
        // bereken het middenpunt
        int m = l + (r - l) / 2;

        // sorteer eerst en daarna
        MergeSort(arr, l, m);
        MergeSort(arr, m + 1, r);

        // voeg de gesorteerde arrays samen
        Merge(arr, l, m, r);
    }
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_1(int* array, int size, int k, int* difference) {
    if (k > size) {
        return -1; // ongeldige waarde voor k
    }

    // sorteer de array met behulp van MergeSort
    MergeSort(array, 0, size - 1);

    // bereken de som van de k hoogste en k laagste elementen
    int sumMax = 0;
    int sumMin = 0;
    for (int i = 0; i < k; i++) {
        sumMax += array[size - i - 1];
        sumMin += array[i];
    }

    // bereken het verschil en sla het op in de difference pointer
    *difference = sumMax - sumMin;

    return 0;
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