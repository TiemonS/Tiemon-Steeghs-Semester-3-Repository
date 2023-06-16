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
    bool spaceFound = false;
    //Ehet element voor het doorspieden van de lijst
    struct element *tempElement = FreeList->head;
    //het element wat toegevoegd gaat worden aan de alloc list
    struct element *newElement = (struct element *)malloc(sizeof(struct element));

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
        if (tempElement->size == nrofBytes)
        {
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
        //dat alloceren we de bytes in de alloc list
        ListAddTail(AllocList, newElement->address, nrofBytes);
        // fprintf(stderr, "newElement->address: %d - newElement->size(nrofBytes): %d\n", newElement->address, newElement->size);
        return newElement->address;
    }
    // free(newElement), er wordt al een nieuwe aangemaakt in ListAddTail(), dus kan memory leaks veroorzaken
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
    struct element *elementToBeRemoved = (struct element *)malloc(sizeof(struct element));
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
    if (!elementToBeRemoved->address)
    {
        return -1;
    }
    //de size het aantal bytes opslaan omdat we dit moeten onthouden voor later
    int tempSizeValue = elementToBeRemoved->size;
    //het address wat wordt vrijgegeven removen
    ListRemove(AllocList, &elementToBeRemoved);

    //dan gaan we kijken of er gemerged kan worden in de free list
    tempElement = FreeList->head;
    bool merged = false;
    for (int i = 0; i < FreeList->count; i++)
    {
        if (tempElement->address - tempSizeValue == addr) // als address - size in bytes = address van element dan kan er gemerged worden met een element dat een groter address heeft
        {
            fprintf(stderr, "Merge uitgevoerd met dit GROTERE element, Adress:%d Size:%d\n", tempElement->address, tempElement->size);
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
            fprintf(stderr, "Merge uitgevoerd met dit KLEINERE element, Adress:%d Size:%d\n", tempElement->address, tempElement->size);
            break;
        }
        else if (tempElement->next == NULL)
        {
            break; // i = freeList->count - 1; // break kan ook
        }
        else
        {
            tempElement = tempElement->next;
        }
    }
    // fprintf(stderr, "sgoeud\n");
    tempElement = FreeList->head;
    for (int i = 0; i < FreeList->count; i++)
    {
        if (tempElement->address == addr + tempSizeValue) // this means that the elements can be merged (example: part 2 documentation, freeing address 51, size 20, 3rd image)
        {
            break;
            // i = freeList->count + 1;
        }
        else
        {
            tempElement = tempElement->next;
        }
    }
    if (tempElement != NULL && !merged)
    {
        tempElement->address -= tempSizeValue;
        tempElement->size += tempSizeValue;
        merged = true;
    }
    if (merged) // Als de freelist gemerged is, kijken of er nog meer gemerged kan worden?
    {
        tempElement = FreeList->head;
        // fprintf(stderr, "tempElement->address: %d - tempElement->size: %d\n", tempElement->address, tempElement->size);
        // fprintf(stderr, "tempElement->next->address: %d = tempElement->next->size: %d\n", tempElement->next->address, tempElement->next->size);
        // if (tempElement->next == NULL)
        // {
        //     return tempSizeValue;
        // }
        for (int i = 0; i < FreeList->count; i++)
        {
            // fprintf(stderr, "jaapmetbaap\n");
            if (tempElement->next != NULL)
            {
                if (tempElement->address + tempElement->size == tempElement->next->address)
                {
                    // fprintf(stderr, "jaapzonderbaap\n");
                    tempElement->size += tempElement->next->size;
                    ListRemove(FreeList, &tempElement->next);
                    break; // i = freeList->count - 1;
                }
            }
            // fprintf(stderr, "jaapmetzonderbaap\n");
            if (tempElement->next == NULL)
            {
                // fprintf(stderr, "goddommenogmahl\n");
                break;
            }
            else
            {
                tempElement = tempElement->next;
            }
        }
        // fprintf(stderr, "jaap\n");
    }
    else if (FreeList->count == 0)
    {
        ListAddTail(FreeList, addr, tempSizeValue);
    }
    else
    {
        // fprintf(stderr, "hier gaat t fout\n");
        tempElement = FreeList->head;
        // int mahmoud = 0;
        for (int i = 0; i < FreeList->count; i++)
        {
            // fprintf(stderr, "keesmetvlees\n");
            if (tempElement == NULL)
            {
                // fprintf(stderr, "keeszondervlees\n");
                ListAddTail(FreeList, addr, tempSizeValue);
                break;
            }
            else if (tempElement->address > addr)
            {
                // fprintf(stderr, "keesmetzondervlees\n");
                // fprintf(stderr, "kees");
                tempElement = ListGetPreviousElement(FreeList, tempElement->address);
                if (tempElement == NULL)
                {
                    ListAddAfter(FreeList, addr, tempSizeValue, NULL);
                    break;
                }
                ListAddAfter(FreeList, addr, tempSizeValue, tempElement);
                break;
            }
            tempElement = tempElement->next;
        }
    }
    return tempSizeValue;
}

int MergeList() 
{
    return 0;
}
