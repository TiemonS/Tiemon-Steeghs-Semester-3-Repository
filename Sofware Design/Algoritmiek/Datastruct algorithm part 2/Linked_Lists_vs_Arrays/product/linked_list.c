#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

//de methode voor het aanmaken van een linked list
struct LinkedList *CreateList()
{
    struct LinkedList *linkedList = (struct LinkedList *)malloc(sizeof(struct LinkedList));

    linkedList->head = NULL;
    linkedList->count = 0;
    return linkedList;
}

/* function: ListAddTail
 * pre: -
 * post: an element is added to the end of the linked list
 * returns: 0 on success, -1 on fail
 */
int ListAddTail(LinkedList *linkedList, int address, int size)
{
    Element* newElement = (Element*)malloc(sizeof(Element));
    if (newElement == NULL) {
        return -1; // Memory allocation failed
    }

    newElement->address = address;
    newElement->size = size;
    newElement->next = NULL;

    if (linkedList->head == NULL) {
        linkedList->head = newElement;
    } else {
        Element* current = linkedList->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newElement;
    }

    linkedList->count++;

    return 0;
}

/* function: ListAddAfter
 * pre: -
 * post: a new element with given data is added after element
 *       in the linked list. If element is NULL, the new element
 *       is added to the front of the list.
 * returns: 0 on success, -1 on fail
 */
int ListAddAfter(LinkedList *linkedList, int address, int size, Element* element)
{
    if (address <= 0 || size <= 0) {
        return -1; // Invalid data or size
    }

    Element* newElement = (Element*)malloc(sizeof(Element));
    if (newElement == NULL) {
        return -1; // Memory allocation failed
    }

    // Copy the data and size
    newElement->address = address;
    newElement->size = size;
    newElement->next = NULL;

    if (element == NULL) {
        newElement->next = linkedList->head;
        linkedList->head = newElement;
    } else {
        newElement->next = element->next;
        element->next = newElement;
    }

    linkedList->count++;

    return 0;
}


/* function: ListGetHead
 * pre: -
 * post: first element in linked list is returned
 * returns: first element on success, NULL if list is empty
 */
Element* ListGetHead(LinkedList *linkedList)
{
    return linkedList->head;
}


/* function: ListRemoveTail
 * pre: -
 * post: last element is removed from list
 * returns: 0 on success, -1 on fail
 */
int ListRemoveTail(LinkedList *linkedList)
{
    if (linkedList->head == NULL) {
        return -1; // List is empty
    }

    if (linkedList->head->next == NULL) {
        free(linkedList->head);
        linkedList->head = NULL;
    } else {
        Element* current = linkedList->head;
        Element* previous = NULL;
        while (current->next != NULL) {
            previous = current;
            current = current->next;
        }
        free(current);
        previous->next = NULL;
    }

    linkedList->count--;

    return 0;
}

/* function: ListRemove
 * pre: -
 * post: element is removed from list, pointer from callee is made NULL
 * returns: 0 on success, -1 on fail
 */
int ListRemove(LinkedList *linkedList, Element** element)
{
   if (linkedList->head == NULL || *element == NULL) {
        return -1; // List is empty or element is NULL
    }

    if (*element == linkedList->head) {
        Element* temp = linkedList->head;
        linkedList->head = linkedList->head->next;
        free(temp);
        *element = NULL;
    } else {
        Element* current = linkedList->head;
        Element* previous = NULL;
        while (current != NULL && current != *element) {
            previous = current;
            current = current->next;
        }

        if (current == NULL) {
            return -1; // Element not found in the list
        }

        previous->next = current->next;
        free(current);
        *element = NULL;
    }

    linkedList->count--;

    return 0;
}

/* function: ListRemoveAll
 * pre: -
 * post: all existing elements from list are removed
 */
void ListRemoveAll(LinkedList *linkedList)
{
    Element* current = linkedList->head;
    while (current != NULL) {
        Element* temp = current;
        current = current->next;
        free(temp);
    }
    linkedList->head = NULL;
    linkedList->count = 0;
}

struct element *ListGetPreviousElement(LinkedList *list, int address)
{
    // assert(list != NULL);
    if (list == NULL)
    {
        return NULL;
    }

    struct element *returnValue = NULL;

    struct element *previousElement = NULL;
    struct element *currentElement = list->head;

    if (currentElement != NULL)
    {
        while (currentElement->address != address)
        {
            if (currentElement->next == NULL)
            {
                // list has ended without finding the element
                return returnValue;
            }
            else
            {
                previousElement = currentElement;
                currentElement = currentElement->next;
            }
        }
    }

    returnValue = previousElement;

    return returnValue;
}

struct element* ListGetElement(struct LinkedList* linkedList, int addr) {
    struct element* tempElement = linkedList->head;

    for (int i = 0; i < linkedList->count; i++) 
    {
        if (tempElement->address == addr) {
            return tempElement;  // Found the element with the matching address
        }
        tempElement = tempElement->next;
    }

    return NULL;  // Element not found
}