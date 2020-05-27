//
// Created by 2_20173878_邓志豪 on 2020/5/27.
// 队列c++实现头文件
//

#ifndef CLION_FILES_QUEUE_H
#define CLION_FILES_QUEUE_H

#include <string>
typedef std::string Item;

class Queue{
private:
    enum {MAX = 20};
    struct Node{  //节点
        Item item;
        Node *next;
    };
    Node *front;
    Node *rear;
    int length;  //队列当前长度
    const int qsize; //队列长度限制
public:
    Queue(int qs=MAX);
    ~Queue();
    bool queueisempty() const;  //判断队列是否为空
    bool queueisfull() const;   //判断队列是否为满
    int queuecount() const;     //返回队列当前元素个数
    bool enqueue(Item &item);   //入队
    bool dequeue(Item &item);   //出队
    void showqueue();           //显示队列（显示表达式）
};
#endif //CLION_FILES_QUEUE_H
