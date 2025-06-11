#ifndef _QTYPE_H  // header guard
#define _QTYPE_H

#include<atomic>
#include<mutex>

typedef unsigned int Key;  // ���� Ŭ���� ���� �켱����
typedef void* Value;

typedef struct {
    Key key;
    Value value;
} Item;

typedef struct {
    bool success;   // true: ����, false: ����
    Item item;
    // �ʵ� �߰� ����
} Reply;

typedef struct node_t {
    Item item;
    struct node_t* next;
    std::atomic<bool>marked;
} Node;

typedef struct {
    Node* head, tail;
    std::mutex queue_mutex;
    std::atomic<int>size;
} Queue;

// ���� �����Ӱ� �߰�/����: ���ο� �ڷ��� ���� ��

#endif
