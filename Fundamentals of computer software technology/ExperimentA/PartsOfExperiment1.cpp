// ---
#include "stdafx.h"
#include "iostream"
#include <conio.h>
//CodeBlocks ���ͷ�ļ� #include <stdio.h>


// ����ʵ���и��ַ������������һ��ʵ����

// ����һ�����Ա�
const int nMaxSize = 15;	// ���ֵ
int nLen = 0;				// ����Ԫ�ظ���
int nLinearList[nMaxSize];		

// �������
void LSort();
void LOut();
void LInsert(int n);
void LDelete(int n);

int main()
{
	// �������ݲ��������Ա���
	printf("Please input data\n");	// std::cout << "Please input data\n";
	int nIn = 0;
	for (int i = 1; i <= 10; i++)
	{
		scanf("%d",&nIn);	//		std::cin >> nIn;
		nLinearList[i] = nIn;
		nLen++;
	}
	
	LSort();	// �������Ա�
	LOut();		// ������

	printf("Please input a data to insert \n");
	scanf("%d",&nIn);
	LInsert(nIn);	// ����һ�����֣������뵽���Ա���
	LOut();	

	printf("Please input a data to delete \n");
	scanf("%d",&nIn);
	LDelete(nIn);	// ����һ�����֣��������Ա���ɾ��
	LOut();	

	char chTmp;
	printf("Please input a char to finish this program.");
	chTmp = getch();
	return 0;
}

void LSort()			// ð������,�ɴ�С
{
...
}

void LOut()
{
	printf( "\n");
	for (int i = 1; i <= nLen; i++)
	{
		printf( "%d, ", nLinearList[i] );
	}
	printf( "\n");
}
void LInsert(int n)
{
...
}
void LDelete(int n)
{
...
}

