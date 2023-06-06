#include "linked_list.h"

#include <stdlib.h>

//het uiteinde van van de linked list wordt hierin bijgehouden
static Element* head = NULL;

/* function: ListAddTail
 * pre: -
 * post: an element is added to the end of the linked list
 * returns: 0 on success, -1 on fail
 */
int ListAddTail(int address, int size)
{
    Element* newElement = (Element*)malloc(sizeof(Element));
    if (newElement == NULL) {
        return -1; // Memory allocation failed
    }

    newElement->address = address;
    newElement->size = size;
    newElement->next = NULL;

    if (head == NULL) {
        head = newElement;
    } else {
        Element* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newElement;
    }

    return 0;
}

/* function: ListAddAfter
 * pre: -
 * post: a new element with given data is added after element
 *       in the linked list. If element is NULL, the new element
 *       is added to the front of the list.
 * returns: 0 on success, -1 on fail
 */
int ListAddAfter(int address, int size, Element* element)
{
    if (element == NULL) {
        Element* newElement = (Element*)malloc(sizeof(Element));
        if (newElement == NULL) {
            return -1; // Memory allocation failed
        }

        newElement->address = address;
        newElement->size = size;
        newElement->next = head;
        head = newElement; // Update the head to point to the new element

        return 0;
    }

    Element* newElement = (Element*)malloc(sizeof(Element));
    if (newElement == NULL) {
        return -1; // Memory allocation failed
    }

    newElement->address = address;
    newElement->size = size;
    newElement->next = element->next;
    element->next = newElement;

    return 0;
}


/* function: ListGetHead
 * pre: -
 * post: first element in linked list is returned
 * returns: first element on success, NULL if list is empty
 */
Element* ListGetHead()
{
    return head;
}


/* function: ListRemoveTail
 * pre: -
 * post: last element is removed from list
 * returns: 0 on success, -1 on fail
 */
int ListRemoveTail()
{
    //de head mag niet NULL zijn dan is de list leeg
    if (head == NULL) {
        return -1; 
    }

    //als de head het enige element is wordt de head geremoved
    if (head->next == NULL) {
        free(head);
        head = NULL;
        return 0;
    }

    //elk element doorgaan totdat je aan het einde van de list bent
    Element* current = head;
    Element* previous = NULL;
    while (current->next != NULL) {
        previous = current;
        current = current->next;
    }
    free(current);
    previous->next = NULL;

    return 0;
}

/* function: ListRemove
 * pre: -
 * post: element is removed from list, pointer from callee is made NULL
 * returns: 0 on success, -1 on fail
 */
int ListRemove(Element** element)
{
    if (head == NULL || *element == NULL) {
        return -1; // List is empty or element is NULL
    }

    if (*element == head) {
        Element* temp = head;
        head = head->next;
        free(temp);
        *element = NULL;
        return 0;
    }

    Element* current = head;
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

    return 0;
}

/* function: ListRemoveAll
 * pre: -
 * post: all existing elements from list are removed
 */
void ListRemoveAll()
{
    Element* current = head;
    while (current != NULL) {
        Element* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
}