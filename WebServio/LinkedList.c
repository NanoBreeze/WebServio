
#include "LinkedList.h"
#include <stdlib.h>
#include <string.h>


void freeLinkedList(LinkedList** linkedList) {

    if (!*linkedList) { return; }

    while ((*linkedList)->head) {
        Node* temp = (*linkedList)->head->next;
        free((*linkedList)->head);
        (*linkedList)->head = temp;
    }

    free(*linkedList);
    *linkedList = NULL;
}


void append(LinkedList* linkedList, char* key, char* value) {

    if (!linkedList || !key) { return; }

    Node* newNode = (Node*) malloc(sizeof(Node));

    if (newNode == NULL) {
        return;
    }

    newNode->key = key;
    newNode->value = value;
    newNode->next = NULL;

    if (!linkedList->head) {
        linkedList->head = newNode;
    }
    else {
        linkedList->tail->next = newNode;
    }

    linkedList->tail = newNode;

    linkedList->count++;
}


char* find(LinkedList* linkedList, char* key) {

    if (!linkedList || !key) { return NULL; }

    Node* current = linkedList->head;

    while (current && strcmp(current->key, key)) {
        current = current->next;
    }

    if (current) {
        return current->value;
    }
    return NULL;
}


LinkedList* createLinkedList() {
    LinkedList* linkedList = (LinkedList*) malloc(sizeof(LinkedList));

    if (!linkedList) { return NULL; }

    linkedList->head = NULL;
    linkedList->tail = NULL;
    linkedList->count = 0;

    return linkedList;
}

