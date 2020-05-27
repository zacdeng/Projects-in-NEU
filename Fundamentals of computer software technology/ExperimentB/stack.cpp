//
// Created by 2_20173878_邓志豪 on 2020/5/27.
//

#include "stack.h"
Stack::~Stack() {}

bool Stack::stackisempty() const {
    return top==0;
}
bool Stack::stackisfull() const{
    return top==MAX;
}
bool Stack::pushstack(Item &item){
    if(top<MAX){
        top++;
        items[top]=item;
        return true;
    } else
        return false;
}
bool Stack::popstack(Item &item){
    if(top>0){
        item=items[top];
        top--;
        return true;
    } else
        return false;
}
