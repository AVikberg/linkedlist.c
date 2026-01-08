#include <stdlib.h>

#define MAX_NODEVALUE_LENGTH 256


struct Node { char* value; struct Node* next; };
struct LinkedList { struct Node* first; };

struct Node* getNode(struct LinkedList* list, size_t index);

struct Node* createNode(struct LinkedList* list, const char* value);
struct Node* createNodeFirst(struct LinkedList* list, const char* value);
struct Node* createNodeAt(struct LinkedList* list, const char* value, size_t index);

void removeNodeAt(struct LinkedList* list, size_t index);
void removeAll(struct LinkedList* list);
