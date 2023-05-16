#include <stdio.h>
#include <stdlib.h>
#include "parsing.h"
#include "arrayData.h"

ArrayData ParseData() 
{
    ArrayData arrayData;

    // de eerste waarde lezen, dit is de grootte van de array
    if (scanf("%d", &arrayData.arraySize) != 1) {
        printf("Error: could not read array size\n");
        exit(1);
    }

    //het vrijmaken van geheugen voor de array
    arrayData.array = malloc(arrayData.arraySize * sizeof(int));

    for (int i = 0; i < arrayData.arraySize; i++) {
        //alle getallen toevoegen aan de array
        if (scanf("%d", &arrayData.array[i]) != 1) {
            printf("Error: could not read array element %d\n", i);
            exit(1);
        }
    }

    // dan tenslotte de laatste waarde lezen, de waarde van de variabel k
    if (scanf("%d", &arrayData.k) != 1) {
        printf("Error: could not read value of k\n");
        exit(1);
    }
    
    //als alle data is verkregen wordt hij teruggestuurd
    return arrayData;
}