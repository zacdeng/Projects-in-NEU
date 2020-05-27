// ---
#include "stdafx.h"
#include "iostream"
#include <conio.h>
#include <complex>
//CodeBlocks ���ͷ�ļ� #include <stdio.h> #include <string.h> #include <stdlib.h>

// ����ʵ���и��ַ������������һ��ʵ����

const int MAX_LEN = 10;		// �ַ����ĳ���
const int MAX_SIZE = 30;	// ջ��ӵ����Ԫ�ظ���

// ����һ�����еĽṹ
struct QUEUE {
	int nMaxSize;	// ���ֵ
	int nCount;		// ����
	int nFront;		// ͷ
	int nRear;		// β
	char szQueue[MAX_SIZE][MAX_LEN];
};

//����һ��ջ�Ľṹ
struct STACK {
	int nMaxSize;	// ���ֵ
	int nTop;		// ջ��
	char szStack[MAX_SIZE][MAX_LEN];
};

// ���еĲ���
void InitQueue(QUEUE *q,int nMaxSize);
void InQueue(QUEUE *q, char *pItem);
void OutQueue(QUEUE *q, char *pItem);

//ջ�Ĳ���
void InitStack(STACK *s,int nMaxSize);
void PushStack(STACK *s, char *pItem);
void PopStack(STACK *s, char *pItem);
void GetTopStack(STACK *s, char *pItem);

int Priority(char *op);		// ��ò����������ȼ�
void Compute(char *num1, char *num2, char *op, char *chResult);		// ������ʽ��ֵ


int main()
{
	// ����һ������
    QUEUE q;
	InitQueue(&q,20);
	// ����OSջ��NSջ
	STACK OS, NS;
    ...
	// ������ʽ�������뵽���е���
    ...
	// ��ʾ���ʽ
	...
	// ���ʽ��ֵ
	char x[MAX_LEN];					// ɨ��ı��ʽ
	char op[MAX_LEN];					// ջ�������
	char num1[MAX_LEN], num2[MAX_LEN];	// ����������
	char chResult[MAX_LEN];				// ������
	OutQueue(&q, x);					// ɨ����ʽ
	PushStack(&OS,";");					// ��ѹջ
	while (true)
	{
		if (isdigit(x[0]))				// ���� 
            ...
        else							// ��Ϊ���������û�п��ǿո��
		{
			GetTopStack(&OS, op);		// ���OSջ�������
			if (Priority(x) > Priority(op))		// ����������ȼ���ջ�������
                ...
			if (x[0] == ';' && op[0] == ';')			// ɨ�����
                ...
			if (Priority(x) <= Priority(op))		// ������ջ�������
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
	sprintf(chResult,"%.4f",fResult);	//�Ѽ���Ľ��ת��Ϊ�ַ���
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

void InQueue(QUEUE *q, char *pItem)		// û�п��Ƕ���������
{
	strcpy(q->szQueue[q->nRear], pItem);
	q->nCount++;
	q->nRear++;							// ������ѭ�����У�q->nRear = q->nRear++ % q->nMaxSize ;
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
void PushStack(STACK *s, char *pItem)	//û�п���ջ��������
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