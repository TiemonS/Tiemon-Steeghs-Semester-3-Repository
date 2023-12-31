#include <stddef.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct element Element;
struct element
{
    int address;
    int size;
    Element* next;
};

typedef struct LinkedList {
    Element* head;
    int count;
} LinkedList;

struct LinkedList *CreateList(); 

/* function: ListAddTail
 * pre: -
 * post: an element is added to the end of the linked list
 * returns: 0 on success, -1 on fail
 */
int ListAddTail(LinkedList *linkedList, int address, int size);

/* function: ListAddAfter
 * pre: -
 * post: a new element with given data is added after element
 *       in the linked list. If element is NULL, the new element
 *       is added to the front of the list.
 * returns: 0 on success, -1 on fail
 */
int ListAddAfter(LinkedList *linkedList, int address, int size, Element* element);


/* function: ListGetHead
 * pre: -
 * post: first element in linked list is returned
 * returns: first element on success, NULL if list is empty
 */
Element* ListGetHead(LinkedList *listPtr);


/* function: ListRemoveTail
 * pre: -
 * post: last element is removed from list
 * returns: 0 on success, -1 on fail
 */
int ListRemoveTail(LinkedList *listPtr);

/* function: ListRemove
 * pre: -
 * post: element is removed from list, pointer from callee is made NULL
 * returns: 0 on success, -1 on fail
 */
int ListRemove(LinkedList *linkedList, Element** element);

/* function: ListRemoveAll
 * pre: -
 * post: all existing elements from list are removed
 */
void ListRemoveAll(LinkedList *listPtr);


#endif
