// ---
#include "stdafx.h"
#include "iostream"
#include <conio.h>
//CodeBlocks 需加头文件 #include <stdio.h>


// 程序实现有各种方法，这里给出一个实例。

// 定义一个线性表
const int nMaxSize = 15;	// 最大值
int nLen = 0;				// 表中元素个数
int nLinearList[nMaxSize];		

// 定义操作
void LSort();
void LOut();
void LInsert(int n);
void LDelete(int n);

int main()
{
	// 输入数据并放入线性表中
	printf("Please input data\n");	// std::cout << "Please input data\n";
	int nIn = 0;
	for (int i = 1; i <= 10; i++)
	{
		scanf("%d",&nIn);	//		std::cin >> nIn;
		nLinearList[i] = nIn;
		nLen++;
	}
	
	LSort();	// 排序线性表
	LOut();		// 输出结果

	printf("Please input a data to insert \n");
	scanf("%d",&nIn);
	LInsert(nIn);	// 输入一个数字，并插入到线性表中
	LOut();	

	printf("Please input a data to delete \n");
	scanf("%d",&nIn);
	LDelete(nIn);	// 输入一个数字，并从线性表中删除
	LOut();	

	char chTmp;
	printf("Please input a char to finish this program.");
	chTmp = getch();
	return 0;
}

void LSort()			// 冒泡排序,由大到小
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

