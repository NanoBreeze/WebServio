#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

/* This linked list is for storing the header key/value pairs. The key is a char* and the value is also a char*
RFC 7230 recommends using a hashmap but since performance isn't an issue and linked lists are easier to write
and debug than hashmaps, I will be writing a linked list to store them instead.

Note: this linked list is not ordered. Has head and tail pointer. Appends are only to the tail

Append: O(1)
Find value of key: O(n)
Check if key already exists: O(n)
*/

typedef struct node {
    char* key;
    char* value;
    struct node* next;
} Node;

typedef struct linkedList {
    Node* head;
    Node* tail;
    int count;
} LinkedList;



void freeLinkedList(LinkedList** linkedList);
void append(LinkedList* linkedList, char* key, char* value);
char* find(LinkedList* linkedList, char* key);
LinkedList* createLinkedList();



#endif // LINKEDLIST_H_INCLUDED
