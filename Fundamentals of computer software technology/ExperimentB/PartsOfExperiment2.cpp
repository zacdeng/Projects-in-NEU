// ---
#include "stdafx.h"
#include "iostream"
#include <conio.h>
#include <complex>
//CodeBlocks 需加头文件 #include <stdio.h> #include <string.h> #include <stdlib.h>

// 程序实现有各种方法，这里给出一个实例。

const int MAX_LEN = 10;		// 字符串的长度
const int MAX_SIZE = 30;	// 栈或队的最大元素个数

// 定义一个队列的结构
struct QUEUE {
	int nMaxSize;	// 最大值
	int nCount;		// 个数
	int nFront;		// 头
	int nRear;		// 尾
	char szQueue[MAX_SIZE][MAX_LEN];
};

//定义一个栈的结构
struct STACK {
	int nMaxSize;	// 最大值
	int nTop;		// 栈顶
	char szStack[MAX_SIZE][MAX_LEN];
};

// 队列的操作
void InitQueue(QUEUE *q,int nMaxSize);
void InQueue(QUEUE *q, char *pItem);
void OutQueue(QUEUE *q, char *pItem);

//栈的操作
void InitStack(STACK *s,int nMaxSize);
void PushStack(STACK *s, char *pItem);
void PopStack(STACK *s, char *pItem);
void GetTopStack(STACK *s, char *pItem);

int Priority(char *op);		// 获得操作符的优先级
void Compute(char *num1, char *num2, char *op, char *chResult);		// 计算表达式的值


int main()
{
	// 声明一个队列
    QUEUE q;
	InitQueue(&q,20);
	// 声明OS栈和NS栈
	STACK OS, NS;
    ...
	// 输入表达式，并放入到队列当中
    ...
	// 显示表达式
	...
	// 表达式求值
	char x[MAX_LEN];					// 扫描的表达式
	char op[MAX_LEN];					// 栈顶运算符
	char num1[MAX_LEN], num2[MAX_LEN];	// 两个操作数
	char chResult[MAX_LEN];				// 运算结果
	OutQueue(&q, x);					// 扫描表达式
	PushStack(&OS,";");					// ；压栈
	while (true)
	{
		if (isdigit(x[0]))				// 是数 
            ...
        else							// 认为是运算符，没有考虑空格等
		{
			GetTopStack(&OS, op);		// 获得OS栈顶运算符
			if (Priority(x) > Priority(op))		// 运算符的优先级〉栈顶运算符
                ...
			if (x[0] == ';' && op[0] == ';')			// 扫描结束
                ...
			if (Priority(x) <= Priority(op))		// 不大于栈顶运算符
                ...
        }
    }
}


int Priority(char *op )
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
void Compute(char *num1, char *num2, char *op, char *chResult)
{
	double fNum1,fNum2;
	double fResult = 0;

	fNum1 = atof(num1);
	fNum2 = atof(num2);
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


//-----------------------------------
void InitQueue(QUEUE *q,int nMaxSize)
{
	q->nMaxSize = nMaxSize;
	q->nCount = 0;
	q->nFront = 0;
	q->nRear = 0;
}

void InQueue(QUEUE *q, char *pItem)		// 没有考虑队满的问题
{
	strcpy(q->szQueue[q->nRear], pItem);
	q->nCount++;
	q->nRear++;							// 不考虑循环队列；q->nRear = q->nRear++ % q->nMaxSize ;
}

void OutQueue(QUEUE *q, char *pItem)	
{
	;
	;
	;						
}

void InitStack(STACK *s,int nMaxSize)
{
	s->nMaxSize = nMaxSize;
	s->nTop = 0;
}
void PushStack(STACK *s, char *pItem)	//没有考虑栈满的问题
{
	s->nTop++;
	strcpy(s->szStack[s->nTop], pItem);
}
void PopStack(STACK *s, char *pItem)	
{
	;
	;
}
void GetTopStack(STACK *s, char *pItem)	
{
	;
}