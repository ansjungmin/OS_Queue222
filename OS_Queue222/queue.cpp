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

    Node* current;
    {
        lock_guard<mutex> lock(queue->queue_mutex);
        current = queue->head;
        queue->head = nullptr;
        queue->tail = nullptr;
    }

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
    Reply reply = { false, {0, nullptr} };

    if (queue == nullptr) return reply;

    Node* new_node = nalloc(item);
    if (new_node == nullptr) return reply;

    lock_guard<mutex> lock(queue->queue_mutex);
    Node* prev = queue->head;
    Node* current = queue->head->next;

    while (current != nullptr && current->item.key > item.key) {
        prev = current;
        current = current->next;
    }

    new_node->next = current;
    prev->next = new_node;

    if (current == nullptr) {
        queue->tail = new_node;
    }

    queue->size.fetch_add(1);
    reply.success = true;
    reply.item = item;

    return reply;
}

Reply dequeue(Queue* queue) {
    Reply reply = { false, {0, nullptr} };

    if (queue == nullptr) return reply;

    lock_guard<mutex> lock(queue->queue_mutex);

    Node* first_node = queue->head->next;

    if (first_node == nullptr) {
        return reply;
    }
    queue->head->next = first_node->next;
    if (first_node == queue->tail) {
        queue->tail = queue->head;
    }

    reply.success = true;
    reply.item = first_node->item;

    queue->size.fetch_sub(1);
    nfree(first_node);

    return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
    if (queue == nullptr) return nullptr;

    Queue* result_queue = init();
    if (result_queue == nullptr) return nullptr;

    lock_guard<mutex> lock(queue->queue_mutex);

    Node* current = queue->head->next;  

    while (current != nullptr) {
        if (current->item.key >= start && current->item.key <= end) {
            Node* cloned = nclone(current);
            if (cloned != nullptr) {
                result_queue->tail->next = cloned;
                result_queue->tail = cloned;
                result_queue->size.fetch_add(1);
            }
        }
        current = current->next;
    }

    return result_queue;
}
