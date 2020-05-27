//
// Created by 2_20173878_邓志豪 on 2020/5/27.
//

#include "queue.h"
#include <iostream>
Queue::Queue(int qs) : qsize(qs){
    front = rear = NULL;
    length = 0;
}

Queue::~Queue(){
    Node *tmp;
    while(front != NULL){
        tmp = front;
        front = front->next;
        delete tmp;
    }
}

bool Queue::queueisfull() const {
    return length==qsize;
}

bool Queue::queueisempty() const {
    return length==0;
}

int Queue::queuecount() const {
    return length;
}

bool Queue::enqueue(Item &item) {
    if(queueisfull()) return false;
    Node *add = new Node;
    add->item = item;
    add->next = NULL;
    length++;
    if(front == NULL) front = add;
    else rear->next = add;
    rear = add;
    return true;
}

bool Queue::dequeue(Item &item) {
    if(queueisempty()) return false;
    item = front->item;
    length--;
    Node *tmp = front;
    front = front->next;
    delete tmp;
    if(length == 0) rear = NULL;
    return true;
}

void Queue::showqueue() {
    Node *tmp;
    tmp = front;
    while(tmp != NULL){
        std::cout<<tmp->item;
        tmp = tmp->next;
    }
    std::cout<<std::endl;
}