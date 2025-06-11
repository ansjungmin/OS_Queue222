#include <iostream>
#include <mutex>
#include <atomic>
#include "queue.h"
using namespace std;

Queue* init(void) {
    Queue* queue = new Queue;
    if (queue == nullptr) return nullptr;

    Node* dummy_head = new Node;
    dummy_head->item.key = 0;
    dummy_head->item.value = nullptr;
    dummy_head->next = nullptr;
    dummy_head->marked.store(false);

    queue->head = dummy_head;
    queue->tail = dummy_head;
    queue->size.store(0);

    return queue;
}


void release(Queue* queue) {
    if (queue == nullptr) return;

    lock_guard<mutex> lock(queue->queue_mutex);

    Node* current = queue->head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }

    delete queue;
}


Node* nalloc(Item item) {
    Node* node = new Node;
    if (node == nullptr) return nullptr;

    node->item = item;
    node->next = nullptr;
    node->marked.store(false);

    return node;
}


void nfree(Node* node) {
    if (node != nullptr) {
        delete node;
    }
}


Node* nclone(Node* node) {
    if (node == nullptr) return nullptr;

    Node* cloned = nalloc(node->item);
    return cloned;
}


Reply enqueue(Queue* queue, Item item) {
	
}

Reply dequeue(Queue* queue) {
	
}

Queue* range(Queue* queue, Key start, Key end) {
	
}
