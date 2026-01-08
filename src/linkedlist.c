#include <string.h>
#include "linkedlist.h"


struct Node* getNode(struct LinkedList* list, size_t index)
{
    struct Node* current = list->first;
    for (int i = 0; i < index; i++)
    {
        if (!current)
            return NULL;
        
        current = current->next;
    }
    return current;
}


struct Node* createNode(struct LinkedList* list, const char* value)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->next = NULL;
    newNode->value = malloc(sizeof(char[MAX_NODEVALUE_LENGTH]));
    strcpy(newNode->value, value);

    if (!list->first)
    {
        list->first = newNode;
        return NULL;
    }

    struct Node* current = list->first;
    while (current->next)
        current = current->next;
    
    current->next = newNode;

    return newNode;
}

struct Node* createNodeFirst(struct LinkedList* list, const char* value)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->next = NULL;
    newNode->value = malloc(sizeof(char[MAX_NODEVALUE_LENGTH]));
    strcpy(newNode->value, value);

    if (!list->first)
    {
        list->first = newNode;
        return newNode;
    }
    newNode->next = list->first;
    list->first = newNode;

    return newNode;
}

struct Node* createNodeAt(struct LinkedList* list, const char* value, size_t index)
{
    if (!index)
        return createNodeFirst(list, value);

    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->next = NULL;
    newNode->value = malloc(sizeof(char[MAX_NODEVALUE_LENGTH]));
    strcpy(newNode->value, value);

    struct Node* current = getNode(list, index - 1);
    if (!current)
        return NULL;

    newNode->next = current->next;    
    current->next = newNode;
    return newNode;
}


void removeNodeAt(struct LinkedList* list, size_t index)
{
    struct Node* current = getNode(list, index);

    if (!current)
        return;

    if (!index)
        list->first = list->first->next;
    else
    {
        struct Node* previous = getNode(list, index - 1);
        if (previous)
            previous->next = current->next;
    }

    free(current->value);
    free(current);
}

void removeAll(struct LinkedList* list)
{
    if (!list->first)
        return;
    
    struct Node* current;
    while (list->first)
    {
        current = list->first;
        list->first = list->first->next;
        free(current->value);
        free(current);
    }
}
