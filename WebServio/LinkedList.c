
#include "LinkedList.h"
#include <stdlib.h>
#include <string.h>


void freeLinkedList(LinkedList** linkedList) {

    if (!*linkedList) { return; }

    while ((*linkedList)->head) {
        Node* temp = (*linkedList)->head->next;

        free((*linkedList)->head->key);
        free((*linkedList)->head->value);
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

    newNode->key = strdup(key);
    newNode->value = strdup(value);

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

//O(N^2) double iteration
bool containsDuplicate(LinkedList* linkedList) {

    if (!linkedList) { return false; }
    if (linkedList->count < 2) { return false; }

    Node* outer = linkedList->head;


    while (outer) {
        Node* inner = outer->next;
        while (inner) {
            if (strcmp(inner->key, outer->key) == 0) {
                return true;
            }
            inner = inner->next;
        }
        outer = outer->next;
    }

    return false;
}


