#include "memory.h"
#include "linked_list.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const int StartAddress = 1000;
struct LinkedList* FreeList;
struct LinkedList* AllocList;


/* function: ConstructMemory
 * pre: -
 * post: memory administration is constructed
 */
void ConstructMemory(int size)
{
    //Het aanmaken van de twee lists
    FreeList = CreateList();
    AllocList = CreateList();

    //de free list vullen met zijn startaddress en bytes
    ListAddTail(FreeList, StartAddress, size);
}

/* function: DestructMemory
 * pre: -
 * post: memory administration is destructed
 */
void DestructMemory()
{
    ListRemoveAll(FreeList);
    ListRemoveAll(AllocList);
    free(FreeList);
    free(AllocList);
    FreeList = NULL;
    AllocList = NULL;
}


/* function: PrintList
 * pre: -
 * post: if stream is valid, list information is printed to stream and 0 is returned
 *       if stream is NULL, -1 is returned
 */
int PrintList(FILE* stream)
{
    if (stream == NULL) {
        return -1;
    }

    // Het uitprinten van de free list
    fprintf(stream, "Free List:\n");
    //Eerst de head pakken van de list
    Element* freeElement = ListGetHead(FreeList);
    //zolang freeelement niet de laaste is blijven we printen
    while (freeElement != NULL) {
        fprintf(stream, "Address: %d, Size: %d\n", freeElement->address, freeElement->size);
        freeElement = freeElement->next;
    }

    //Het uitprinten van de alloc list
    fprintf(stream, "\nAllocated List:\n");
    Element* allocElement = ListGetHead(AllocList);
    while (allocElement != NULL) {
        fprintf(stream, "Address: %d, Size: %d\n", allocElement->address, allocElement->size);
        allocElement = allocElement->next;
    }

    return 0;
}

/* function: ClaimMemory
 * pre: nrofBytes > 0
 * post: if no memory block of nrofBytes available return -1
 *       otherwise the first block that is large enough is claimed and the start address is returned
 */
int ClaimMemory(int nrofBytes)
{   
    if (nrofBytes <= 0)
    {
        fprintf(stderr, "Invalid number of bytes: %d", nrofBytes);
        return -1;
    }

    bool spaceFound = false;
    //Ehet element voor het doorspieden van de lijst
    struct element *tempElement = FreeList->head;
    //het element wat toegevoegd gaat worden aan de alloc list
    struct element *newElement =  NULL;

    //kijken of de free list niet leeg is
    if (tempElement == NULL)
    {
        fprintf(stderr, "Head van de freelist = NULL");
        return -1;
    }
    //de for loop van het doorgaan van de freelist
    for (int i = 0; i < FreeList->count; i++)
    {
        //kijken of het gevraagde aantal bytes gelijk is aan het totale aantal bytes over
        //fprintf(stderr, "tempElement->size == %d", tempElement->size);
        if (tempElement->size == nrofBytes)
        {
            newElement = (struct element*)malloc(sizeof(struct element));
            //het address van new element gelijk maken aan het freelist address
            newElement->address = tempElement->address; 
            //het address goed update doormiddel van het aantal bytes erbij op te tellen
            tempElement->address += nrofBytes;
            //alle bytes uit het element zijn in gebruik dus remove hem
            ListRemove(FreeList, &tempElement);
            spaceFound = true;
            break;
        }
        //is het niet gelijk aan het totale aantal, is er dan uberhaupt nog ruimte?
        else if (tempElement->size > nrofBytes)
        {
            newElement = (struct element*)malloc(sizeof(struct element));
            //het address van new element gelijk maken aan het freelist address
            newElement->address = tempElement->address;
            //het address goed update doormiddel van het aantal bytes erbij op te tellen
            tempElement->address += nrofBytes;
            spaceFound = true;
            //het aantal bytes wat in gebruik gaat van de freelist afhalen
            tempElement->size -= nrofBytes;
            break; 
        }
        tempElement = tempElement->next;
    }
    
    //is er ruimte gevonden?
    if (spaceFound)
    {
        //dan alloceren we de bytes in de alloc list
        AddElementToRightLocation(AllocList, newElement->address, nrofBytes);
        //ListAddTail(AllocList, newElement->address, nrofBytes);
        // fprintf(stderr, "newElement->address: %d - newElement->size(nrofBytes): %d\n", newElement->address, newElement->size);
        //eerst de waarde opslaan voordat de memory wordt vrijgegeven
        int newAddress = newElement->address;
        free(newElement);
        return newAddress;
         
    }
    free(newElement); // er wordt al een nieuwe aangemaakt in ListAddTail(), dus kan memory leaks veroorzaken
    return -1;
}

/* function: FreeMemory
 * pre: Parameter addr must match the start of an allocatd memory block. Otherwhise return -1.
 * post: Memory is freed and the number of freed bytes is returned
 *
 */
int FreeMemory(int addr)
{
    //het element wat uit de alloc lijst gehaald gaat worden
    struct element *elementToBeRemoved = NULL;
    //elementToBeRemoved->address = 0; //als ik dit niet doe ontstaan er errors op het moment dat address 0 is

    //het element wat gebruikt wordt om door de linkedlists heen te gaan
    struct element *tempElement = AllocList->head;

    //in de alloclist opzoek gaan naar het address wat we willen vrijgeven
    for (int i = 0; i < AllocList->count; i++)
    {
        if (tempElement->address == addr)
        {
            elementToBeRemoved = tempElement;
            break;
        }
        //als het address nog niet is gevonden gaan we naar het volgende element kijken
        else
        {
            tempElement = tempElement->next;
        }
    }
    //het address bestaat niet return -1
    if (elementToBeRemoved == NULL)
    {
        return -1;
    }
    //de size het aantal bytes opslaan omdat we dit moeten onthouden voor later
    int tempSizeValue = elementToBeRemoved->size;
    //het address wat wordt vrijgegeven removen
    ListRemove(AllocList, &elementToBeRemoved);
    //free(elementToBeRemoved);

    //dan gaan we kijken of er gemerged kan worden in de free list
    tempElement = FreeList->head;
    bool merged = false;
    for (int i = 0; i < FreeList->count; i++)
    {
        if (tempElement->address - tempSizeValue == addr) // als address - size in bytes = address van element dan kan er gemerged worden met een element dat een groter address heeft
        {
            //fprintf(stderr, "Merge uitgevoerd met dit GROTERE element, Adress:%d Size:%d\n", tempElement->address, tempElement->size);
            // fprintf(stderr, "tempSizeValue (elementToBeRemoved->value): %d\n", tempSizeValue);

            //er kan gemerged worden dus tel de bytes op
            tempElement->size += tempSizeValue;
            //van het address moet het aantal bytes ook afgehaald worden om het juiste adres te krijgen
            tempElement->address -= tempSizeValue; 
            merged = true;
            break;
        }
        else if (tempElement->address + tempElement->size == addr) // als address + size in bytes = address van element kan er gemerged worden met een element met een kleiner address
        {
            //er kan gemerged worden dus tel de bytes op
            tempElement->size += tempSizeValue;
            merged = true;
            // return tempSizeValue;
            //fprintf(stderr, "Merge uitgevoerd met dit KLEINERE element, Adress:%d Size:%d\n", tempElement->address, tempElement->size);
            break;
        }
        else if (tempElement->next == NULL)
        {
            break;
        }
        //als er nog geen mergebaar element is gevonden gaan we naar het volgende element
        else
        {
            tempElement = tempElement->next;
        }
    }
    
    //Kijken of er meer gemerged kan worden
    if (merged) // Als de freelist gemerged is, kijken of er nog meer gemerged kan worden?
    {
        MergeFreeList();
    }
    //als er niet gemerged kan worden en de freelist is leeg voeg dan nieuw element toe
    else if (FreeList->count == 0)
    {
        ListAddTail(FreeList, addr, tempSizeValue);
    }
    //als er niet gemerged kan worden met er een nieuw element worden toegevoegd aan de freelist
    else
    {
        AddElementToRightLocation(FreeList, addr, tempSizeValue);
    }
    return tempSizeValue;
}

void MergeFreeList()
{
    if (FreeList->count <= 1)
        return;

    Element* current = FreeList->head;
    Element* next = current->next;

    while (next != NULL)
    {
        if (current->address + current->size == next->address)
        {
            // Merge the two blocks
            current->size += next->size;

            // Remove the next block from the free list
            ListRemove(FreeList, &next);

            // Free the memory of the next block
            free(next);

            // Update the next pointer
            next = current->next;
        }
        else
        {
            // Move to the next pair of elements
            current = next;
            next = next->next;
        }
    }
}

void AddElementToRightLocation(LinkedList* linkedlist, int addr, int tempSizeValue) 
{
    Element* current = linkedlist->head;
    Element *prevElement = NULL; // Keep track of the previous element
    bool added = false; // Flag to track if the new element has been added

    for (int i = 0; i < linkedlist->count; i++)
    {
        if (current == NULL)
        {
            // Reached the end of the list, add the new element at the tail
            ListAddTail(linkedlist, addr, tempSizeValue);
            added = true;
            break;
        }
        else if (current->address > addr)
        {
            // Found a larger address, insert the new element before it
            if (prevElement == NULL)
            {
                // Insert at the head of the list
                ListAddAfter(linkedlist, addr, tempSizeValue, NULL);
            }
            else
            {
                // Insert after the previous element
                ListAddAfter(linkedlist, addr, tempSizeValue, prevElement);
            }
            added = true;
            break;
        }
        else
        {
            prevElement = current;
            current = current->next;
        }
    }
    if (!added)
    {
    // The new element has the highest address, add it at the tail
    ListAddTail(linkedlist, addr, tempSizeValue);
    }
}

//TODO
//free memory verder begrijpen merge enz
//fouten in de input full fixen (het allocated en free van 1050 vlak achter elkaar)

//als ik de check voor added weghaal uit free memory gooit hij geen segmentation fault meer?

//andere fout: het programma kan maar een keer extra mergen
//bij vaker dan een keer raken er bytes verloren zie free memory element 1050 met 30 bytes wordt random ook geremoved
// bij de input full merged hij ook niet volledig omdat 1050 weg is en 1020 + 30 = 1050 en niet 1080

//fout gevonden doordat tijdens het merge het volgende element niet werd opgeslagen kreeg je dus
//dat in een rij van drie elementen [1000, 1020, 1050] dat 1000 ging wijzen naar NULL omdat 1020 geremoved werd
//daarom zag 1000 1050 ook niet meer en was deze dus weg. Doormiddel van eerst op te slaan wat het element na 1020 is
//kan je ervoor zorgen dat de elementen pointers goed worden opgeschoven