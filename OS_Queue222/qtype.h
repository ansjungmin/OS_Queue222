#ifndef _QTYPE_H  
#define _QTYPE_H

#include<atomic>
#include<mutex>

typedef unsigned int Key;  
typedef void* Value;

typedef struct {
    Key key;
    Value value;
} Item;

typedef struct {
    bool success;   
    Item item;

} Reply;

typedef struct node_t {
    Item item;
    struct node_t* next;
    std::atomic<bool>marked;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    std::mutex queue_mutex;
    std::atomic<int>size;
} Queue;



#endif
