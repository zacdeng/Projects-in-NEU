//
// Created by 2_20173878_邓志豪 on 2020/5/27.
//
#include "stack.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <complex>
#define MAX_LEN 20
using namespace std;

int Priority(string op);		// 获得操作符的优先级
void Compute(string num1, string num2, string op, char *chResult);		// 计算表达式的值

int main(){
    Queue q;  //声明一个队列
    Stack OS, NS;  //声明OS栈和NS栈
    //输入表达式并放入队列中
    char ch[MAX_LEN];
    string s;
    cout<<"Please input the expression"<<endl;
    cout<<"Enter 'q' to quit"<<endl;
    while(cin>>ch && strcmp(ch,"q")!=0){
        q.enqueue(s=ch);
    }
    //显示表达式
    q.showqueue();
    //表达式求值
    string x;           // 扫描的表达式
    string op;			// 栈顶运算符
    string num1, num2;	// 两个操作数
    char chResult[MAX_LEN];	    // 每步运算结果
    string FinalResult; //最终结果
    q.dequeue(x);   // 扫描表达式
    OS.pushstack(s=";"); // ;压栈
    while(true){
        if(isdigit(x[0])){  //判断是否为数字（输入正确的情况下第一个是数0~9则可以确定整个string为数）
            NS.pushstack(x);
            q.dequeue(x);
            continue;
        }
        else{
            op = OS.gettopstack();		// 获得OS栈顶运算符

            if (Priority(x) > Priority(op)){  // 运算符的优先级〉栈顶运算符
                OS.pushstack(x);
                q.dequeue(x);
                continue;
            }

            if (x[0] == ';' && op[0] == ';'){	// 扫描结束
                NS.popstack(FinalResult);
                break;
            }

            if (Priority(x) <= Priority(op)){   // 不大于栈顶运算符
                NS.popstack(num2);
                NS.popstack(num1);
                OS.popstack(op);
                Compute(num1, num2, op, chResult);
                NS.pushstack(s=chResult);
            }
        }
    }
    cout<<"The result of the expression is: "<<FinalResult<<endl;
    return 0;
}

int Priority(string op)
{
    int nPriority = 0;
    switch (op[0])
    {
        case '^':
            nPriority = 3;
            break;
        case '*':
        case '/':
            nPriority = 2;
            break;
        case '+':
        case '-':
            nPriority = 1;
            break;
        case ';':
            nPriority = 0;
    }
    return nPriority;
}
void Compute(string num1, string num2, string op, char *chResult)
{
    double fNum1,fNum2;
    double fResult = 0;

    fNum1 = stof(num1);
    fNum2 = stof(num2);
    switch (op[0])
    {
        case '^':
            fResult = pow(fNum1,fNum2);
            break;
        case '*':
            fResult = fNum1*fNum2;
            break;
        case '/':
            fResult = fNum1/fNum2;
            break;
        case '+':
            fResult = fNum1+fNum2;
            break;
        case '-':
            fResult = fNum1-fNum2;
            break;
    }
    sprintf(chResult,"%.4f",fResult);	//把计算的结果转化为字符串
    return;
}
