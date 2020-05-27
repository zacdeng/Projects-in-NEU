//
// Created by 2_20173878_邓志豪 on 2020/5/27.
// 栈c++实现头文件
//

#ifndef CLION_FILES_STACK_H
#define CLION_FILES_STACK_H

#include <string>
typedef std::string Item;

class Stack{
private:
    enum {MAX = 20};
    Item items[MAX];
    int top; //栈顶指针
public:
    Stack() {top = 0;}
    ~Stack();
    bool stackisempty() const ; //判断栈是否为空
    bool stackisfull() const ;  //判断栈是否满
    bool pushstack(Item &item); //压栈
    bool popstack(Item &item);  //出栈
    Item gettopstack(){return items[top];}  //获取栈顶指针
};
#endif //CLION_FILES_STACK_H
